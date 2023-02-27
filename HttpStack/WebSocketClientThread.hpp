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
 * @brief WebSocket Client 쓰레드
 * @param lpParameter CWebSocketClient 의 포인터
 * @returns 0 을 리턴한다.
 */
THREAD_API WebSocketClientThread( LPVOID lpParameter )
{
	CWebSocketClient * pclsClient = (CWebSocketClient *)lpParameter;
	CWebSocketPacket	clsWsPacket;
	CWebSocketPacketHeader clsHeader;
	pollfd sttPoll[1];
	int n;
	char szPacket[TCP_INIT_BUF_SIZE];

	pclsClient->m_bThreadRun = true;
	CLog::Print( LOG_INFO, "%s started", __FUNCTION__ );

	TcpSetPollIn( sttPoll[0], pclsClient->m_hSocket );

	while( pclsClient->m_bStop == false )
	{
		n = poll( sttPoll, 1, 1000 );
		if( n <= 0 ) continue;

		pclsClient->m_clsMutex.acquire();
		
		if( pclsClient->m_psttSsl )
		{
			n = SSLRecv( pclsClient->m_psttSsl, szPacket, sizeof(szPacket) );
		}
		else
		{
			n = recv( pclsClient->m_hSocket, szPacket, sizeof(szPacket), 0 );
		}

		pclsClient->m_clsMutex.release();

		if( n <= 0 ) break;

		std::string strData, strHex;

		StringToHex( szPacket, n, strHex );

		CLog::Print( LOG_NETWORK, "WsRecv(%s:%d) hex[%s]", pclsClient->m_strServerIp.c_str(), pclsClient->m_iServerPort, strHex.c_str() );

		clsWsPacket.AddPacket( szPacket, n );

		while( clsWsPacket.GetData( clsHeader, strData ) )
		{
			if( clsHeader.m_iOpCode == 1 || clsHeader.m_iOpCode == 2 )
			{
				pclsClient->m_pclsCallBack->RecvWebSocket( strData.c_str(), strData.length() );
			}
			else if( clsHeader.m_iOpCode == 8 )
			{
				CLog::Print( LOG_DEBUG, "%s recv Close", __FUNCTION__ );
				pclsClient->m_bStop = true;
				break;
			}
			else if( clsHeader.m_iOpCode == 9 )
			{
				CLog::Print( LOG_DEBUG, "%s recv Ping", __FUNCTION__ );

				if( pclsClient->Send( E_WST_PONG, strData.c_str(), strData.length() ) == false )
				{
					CLog::Print( LOG_ERROR, "%s Send error", __FUNCTION__ );
					break;
				}
			}
		}
	}

	pclsClient->m_bThreadRun = false;
	pclsClient->Close();

	CLog::Print( LOG_INFO, "%s terminated", __FUNCTION__ );
	pclsClient->m_bStop = false;

	return 0;
}
