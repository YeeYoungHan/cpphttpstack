/* 
 * Copyright (C) 2021 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

/**
 * @ingroup HttpStack
 * @brief WebSocket 클라이언트로 데이터를 전송한다.
 * @param pszClientIp WebSocket 클라이언트 IP 주소
 * @param iClientPort WebSocket 클라이언트 포트 번호
 * @param pszData			WebSocket 으로 전송할 payload 데이터
 * @param iDataLen		WebSocket 으로 전송할 payload 데이터 길이
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttpStack::SendWebSocketPacket( const char * pszClientIp, int iClientPort, const char * pszData, int iDataLen )
{
	int iPacketLen = 0;

	if( iDataLen > 65536 )
	{
		iPacketLen = 2 + 8 + iDataLen;
	}
	else if( iDataLen > 125 )
	{
		iPacketLen = 2 + 2 + iDataLen;
	}
	else
	{
		iPacketLen = 2 + iDataLen;
	}

	char * pszPacket = (char *)malloc( iPacketLen );
	if( pszPacket == NULL )
	{
		CLog::Print( LOG_ERROR, "%s malloc error", __FUNCTION__ );
		return false;
	}

	pszPacket[0] = (uint8_t)0x81;

	int iPayLoadPos = 2;

	if( iDataLen > 65536 )
	{
		pszPacket[1] = 127;

		uint64_t lDataLen = iDataLen;

		lDataLen = htonll( lDataLen );
		memcpy( pszPacket + 2, &lDataLen, 8 );
		iPayLoadPos = 2 + 8;
	}
	else if( iDataLen > 125 )
	{
		pszPacket[1] = 126;

		uint16_t sDataLen = iDataLen;

		sDataLen = htons( sDataLen );
		memcpy( pszPacket + 2, &sDataLen, 2 );
		iPayLoadPos = 2 + 2;
	}
	else
	{
		pszPacket[1] = iDataLen;
	}

	memcpy( pszPacket + iPayLoadPos, pszData, iDataLen );

	bool bRes = m_clsTcpStack.Send( pszClientIp, iClientPort, pszPacket, iPacketLen, false );

	free( pszPacket );

	return bRes;
}

bool CHttpStack::RecvPacketWebSocket( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo, CHttpStackSession * pclsApp )
{
	// WebSocket 프로토콜
	CWebSocketPacketHeader clsHeader;
	std::string strData;

	pclsApp->m_clsWsPacket.AddPacket( pszPacket, iPacketLen );

	while( pclsApp->m_clsWsPacket.GetData( clsHeader, strData ) )
	{
		if( clsHeader.m_iOpCode == 1 || clsHeader.m_iOpCode == 2 )
		{
			if( m_pclsCallBack->WebSocketData( pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort, strData ) == false )
			{
				return false;
			}
		}
		else if( clsHeader.m_iOpCode == 8 )
		{
			char szPacket[255];

			memset( szPacket, 0, sizeof(szPacket) );
			szPacket[0] = (uint8_t)0x88;
			iPacketLen = 2;

			if( pclsSessionInfo->Send( szPacket, iPacketLen ) == false )
			{
				CLog::Print( LOG_ERROR, "%s Send error", __FUNCTION__ );
				return false;
			}
		}
		else if( clsHeader.m_iOpCode == 9 )
		{
			char szPacket[255];

			memset( szPacket, 0, sizeof(szPacket) );
			szPacket[0] = (uint8_t)0x8A;
			iPacketLen = 2;
			
			if( strData.empty() == false && strData.length() < 125 )
			{
				szPacket[1] = strData.length();
				memcpy( szPacket + 2, strData.c_str(), strData.length() );
				iPacketLen += strData.length();
			}

			if( pclsSessionInfo->Send( szPacket, iPacketLen ) == false )
			{
				CLog::Print( LOG_ERROR, "%s Send error", __FUNCTION__ );
				return false;
			}
		}
	}

	return true;
}


/**
 * @ingroup HttpStack
 * @brief WebSocket 연결을 위한 HTTP 요청 메시지에 대한 HTTP 응답 메시지를 생성한다.
 * @param pclsRecv 수신된 HTTP 요청 메시지
 * @param pclsSend 전송할 HTTP 응답 메시지
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpStack::MakeWebSocketResponse( CHttpMessage * pclsRecv, CHttpMessage * pclsSend )
{
	CHttpHeader * pclsHeader = pclsRecv->GetHeader( "Sec-WebSocket-Key" );
	if( pclsHeader == NULL )
	{
		CLog::Print( LOG_ERROR, "%s Sec-WebSocket-Key is not found", __FUNCTION__ );
		return false;
	}

	std::string strKey = pclsHeader->m_strValue;

	pclsSend->m_iStatusCode = HTTP_SWITCHING_PROTOCOLS;
	pclsSend->AddHeader( "Upgrade", "websocket" );
	pclsSend->AddHeader( "Connection", "Upgrade" );

	EVP_MD_CTX	sttCtx;
	uint8_t			szDigest[EVP_MAX_MD_SIZE];
	char				szOutput[EVP_MAX_MD_SIZE*2+1];
	uint32_t		iDigestLen;

	memset( szDigest, 0, sizeof(szDigest) );
	memset( szOutput, 0, sizeof(szOutput) );

	EVP_DigestInit( &sttCtx, m_psttMd );
	EVP_DigestUpdate( &sttCtx, strKey.c_str(), strKey.length() );
	EVP_DigestUpdate( &sttCtx, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11", 36 );
	EVP_DigestFinal( &sttCtx, szDigest, &iDigestLen );

	Base64Encode( (char *)szDigest, iDigestLen, szOutput, sizeof(szOutput) );

	pclsSend->AddHeader( "Sec-WebSocket-Accept", szOutput );

	return true;
}
