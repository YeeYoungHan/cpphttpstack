/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
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

#include "HttpStack.h"
#include "HttpStatusCode.h"
#include "Log.h"
#include "Base64.h"
#include "MemoryDebug.h"

const EVP_MD * CHttpStack::m_psttMd = NULL;

CHttpStack::CHttpStack() : m_pclsCallBack(NULL)
{
	if( m_psttMd == NULL )
	{
		OpenSSL_add_all_digests();
		m_psttMd = EVP_get_digestbyname( "sha1" );
	}
}

CHttpStack::~CHttpStack()
{
}

/**
 * @ingroup HttpStack
 * @brief HTTP 서버를 시작한다.
 * @param pclsSetup			설정 객체
 * @param pclsCallBack	callback 객체
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttpStack::Start( CTcpStackSetup * pclsSetup, IHttpStackCallBack * pclsCallBack )
{
	if( pclsCallBack == NULL )
	{
		CLog::Print( LOG_ERROR, "%s IHttpStackCallBack is NULL", __FUNCTION__ );
		return false;
	}

	m_pclsCallBack = pclsCallBack;

	return m_clsTcpStack.Start( pclsSetup, this );
}

/**
 * @ingroup HttpStack
 * @brief 
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttpStack::Stop( )
{
	return m_clsTcpStack.Stop();
}

bool CHttpStack::InComingConnected( CTcpSessionInfo * pclsSessionInfo )
{
	return true;
}

void CHttpStack::SessionClosed( CTcpSessionInfo * pclsSessionInfo )
{
	if( pclsSessionInfo->m_pclsApp )
	{
		CHttpStackSession * pclsApp = (CHttpStackSession *)pclsSessionInfo->m_pclsApp;

		if( pclsApp->m_bWebSocket )
		{
			m_pclsCallBack->WebSocketClosed( pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort );
		}
	}
}

bool CHttpStack::RecvPacket( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo )
{
	if( pclsSessionInfo->m_pclsApp == NULL )
	{
		pclsSessionInfo->m_pclsApp = new CHttpStackSession();
		if( pclsSessionInfo->m_pclsApp == NULL )
		{
			CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
			return false;
		}
	}

	CHttpStackSession * pclsApp = (CHttpStackSession *)pclsSessionInfo->m_pclsApp;

	if( pclsApp->m_bWebSocket )
	{
		// QQQ: WebSocket 패킷 파싱 & callback 기능 추가할 것
	}
	else
	{
		if( pclsApp->m_clsPacket.AddPacket( pszPacket, iPacketLen ) == false )
		{
			CLog::Print( LOG_ERROR, "%s m_clsPacket.AddPacket error", __FUNCTION__ );
			return false;
		}

		if( pclsApp->m_clsPacket.IsCompleted() )
		{
			CHttpMessage * pclsRecv = pclsApp->m_clsPacket.GetHttpMessage();
			if( pclsRecv->IsRequest() )
			{
				CHttpMessage clsSend;
				CHttpHeader * pclsHeader;
				bool bClose = false;

				pclsHeader = pclsRecv->GetHeader( "Upgrade" );
				if( pclsHeader && !strcmp( pclsHeader->m_strValue.c_str(), "websocket" ) )
				{
					if( MakeWebSocketResponse( pclsRecv, &clsSend ) == false )
					{
						return false;
					}

					pclsApp->m_bWebSocket = true;
				}
				else
				{
					pclsHeader = pclsRecv->GetHeader( "Connection" );
					if( pclsHeader == NULL || !strcmp( pclsHeader->m_strValue.c_str(), "close" ) )
					{
						bClose = true;
					}

					if( m_pclsCallBack->RecvHttpRequest( pclsRecv, &clsSend ) == false )
					{
						CLog::Print( LOG_ERROR, "%s RecvHttpRequest error", __FUNCTION__ );
						return false;
					}
				}

				int iNewBufLen = 8192 + clsSend.m_strBody.length();
				char * pszBuf = (char *)malloc( iNewBufLen );
				if( pszBuf == NULL )
				{
					CLog::Print( LOG_ERROR, "%s malloc error", __FUNCTION__ );
					return false;
				}
				
				int iBufLen = clsSend.ToString( pszBuf, iNewBufLen );
				if( iBufLen == -1 )
				{
					CLog::Print( LOG_ERROR, "%s ToString error", __FUNCTION__ );
					free( pszBuf );
					return false;
				}

				if( pclsSessionInfo->Send( pszBuf, iBufLen ) == false )
				{
					CLog::Print( LOG_ERROR, "%s Send error", __FUNCTION__ );
					free( pszBuf );
					return false;
				}

				free( pszBuf );

				if( bClose )
				{
					return false;
				}

				if( pclsApp->m_bWebSocket )
				{
					m_pclsCallBack->WebSocketConnected( pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort );
				}
			}
			else
			{
				CLog::Print( LOG_ERROR, "%s http request error", __FUNCTION__ );
				return false;
			}
		}
	}

	return true;
}

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
