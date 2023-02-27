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

#include "SipPlatformDefine.h"
#include "WebSocketClient.h"
#include "WebSocketPacket.h"
#include "HttpStatusCode.h"
#include "HttpPacket.h"
#include "ServerUtility.h"
#include "TimeUtility.h"
#include "TimeString.h"
#include "StringUtility.h"
#include "Base64.h"
#include "Log.h"
#include "MemoryDebug.h"

#include "WebSocketClientThread.hpp"

CWebSocketClient::CWebSocketClient() : m_iRecvTimeout(10), m_bStop(false), m_bThreadRun(false), m_hSocket(INVALID_SOCKET), m_psttSsl(NULL), m_pclsCallBack(NULL)
{
	InitNetwork();
}

CWebSocketClient::~CWebSocketClient()
{
	Close();
}

/**
 * @ingroup HttpStack
 * @brief WebSocket 연결한다.
 * @param pszUrl WebSocket url ( 예: wss://127.0.0.1 )
 * @param pclsCallBack 콜백 클래스의 포인터
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWebSocketClient::Connect( const char * pszUrl, IWebSocketClientCallBack * pclsCallBack )
{
	if( IsClosed() == false )
	{
		CLog::Print( LOG_ERROR, "%s is already running", __FUNCTION__ );
		return false;
	}

	if( pszUrl == NULL )
	{
		CLog::Print( LOG_ERROR, "%s pszUrl is null", __FUNCTION__ );
		return false;
	}

	if( pclsCallBack == NULL )
	{
		CLog::Print( LOG_ERROR, "%s pclsCallBack is null", __FUNCTION__ );
		return false;
	}

	CHttpUri clsUri;
	int iUrlLen = (int)strlen( pszUrl );

	if( clsUri.Parse( pszUrl, iUrlLen ) == -1 )
	{
		CLog::Print( LOG_ERROR, "%s clsUri.Parse(%s) error", __FUNCTION__, pszUrl );
		return false;
	}

	const char * pszProtocol = clsUri.m_strProtocol.c_str();
	if( !strcmp( pszProtocol, "ws" ) )
	{
	}
	else if( !strcmp( pszProtocol, "wss" ) )
	{
		if( clsUri.m_iPort == 80 ) clsUri.m_iPort = 443;
	}
	else
	{
		CLog::Print( LOG_ERROR, "%s url(%s) is not websocket url", __FUNCTION__, pszUrl );
		return false;
	}

	CHttpMessage clsRequest;
	CHttpPacket	clsPacket;
	char szTime[21];
	std::string strBase64;

	GetDateTimeString( szTime, sizeof(szTime) );
	sprintf( szTime + 14, "00" );

	Base64Encode( szTime, strlen(szTime), strBase64 );

	clsRequest.SetRequest( "GET", &clsUri );
	clsRequest.AddHeader( "Upgrade", "websocket" );
	clsRequest.AddHeader( "Connection", "Upgrade" );
	clsRequest.AddHeader( "Sec-WebSocket-Key", strBase64.c_str() );
	clsRequest.AddHeader( "Sec-WebSocket-Version", "13" );
	
	char szPacket[8192];
	int iBufLen, n;
	std::string strPacket;
	CHttpMessage * pclsResponse = clsPacket.GetHttpMessage();

	clsRequest.m_strHttpVersion = "HTTP/1.1";
	
	iBufLen = clsRequest.ToString( strPacket );
	if( iBufLen <= 0 )
	{
		CLog::Print( LOG_ERROR, "%s clsRequest.ToString() error", __FUNCTION__ );
		return false;
	}

	m_strServerIp = clsUri.m_strHost;
	m_iServerPort = clsUri.m_iPort;

	m_hSocket = TcpConnect( clsUri.m_strHost.c_str(), clsUri.m_iPort );
	if( m_hSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s TcpConnect(%s:%d) error", __FUNCTION__, clsUri.m_strHost.c_str(), clsUri.m_iPort );
		return false;
	}

	CLog::Print( LOG_NETWORK, "TcpConnect(%s:%d) success", clsUri.m_strHost.c_str(), clsUri.m_iPort );

	// wss 프로토콜이면 TLS 로 연결한다.
	if( !strcmp( pszProtocol, "wss" ) )
	{
		if( SSLConnect( m_hSocket, &m_psttSsl ) == false )
		{
			CLog::Print( LOG_ERROR, "%s SSLConnect error", __FUNCTION__ );
			Close();
			return false;
		}

		CLog::Print( LOG_NETWORK, "SSLConnect(%s:%d) success", clsUri.m_strHost.c_str(), clsUri.m_iPort );
	}

	if( m_psttSsl )
	{
		if( SSLSend( m_psttSsl, strPacket.c_str(), strPacket.length() ) != iBufLen )
		{
			CLog::Print( LOG_ERROR, "%s SSLSend error", __FUNCTION__ );
			Close();
			return false;
		}
	}
	else
	{
		if( TcpSend( m_hSocket, strPacket.c_str(), strPacket.length() ) != iBufLen )
		{
			CLog::Print( LOG_ERROR, "%s TcpSend error", __FUNCTION__ );
			Close();
			return false;
		}
	}

	CLog::Print( LOG_NETWORK, "TcpSend(%s:%d) [%s]", clsUri.m_strHost.c_str(), clsUri.m_iPort, strPacket.c_str() );

	while( 1 )
	{
		memset( szPacket, 0, sizeof(szPacket) );

		if( m_psttSsl )
		{
			n = SSLRecv( m_psttSsl, szPacket, sizeof(szPacket) );
		}
		else
		{
			n = TcpRecv( m_hSocket, szPacket, sizeof(szPacket), m_iRecvTimeout );
		}

		if( n <= 0 )
		{
			break;
		}

		CLog::Print( LOG_NETWORK, "TcpRecv(%s:%d) [%.*s]", clsUri.m_strHost.c_str(), clsUri.m_iPort, n, szPacket );

		if( clsPacket.AddPacket( szPacket, n ) == false )
		{
			CLog::Print( LOG_ERROR, "%s clsPacket.AddPacket error", __FUNCTION__ );
			break;
		}

		if( clsPacket.IsCompleted() ) break;
	}

	if( pclsResponse->m_iStatusCode == HTTP_SWITCHING_PROTOCOLS )
	{
		m_pclsCallBack = pclsCallBack;

		if( StartThread( "WebSocketClientThread", WebSocketClientThread, this ) == false )
		{
			Close();
			return false;
		}

		return true;
	}

	Close();
	return false;
}

/**
 * @ingroup HttpStack
 * @brief WebSocket 연결을 종료한다.
 */
void CWebSocketClient::Close()
{
	if( m_bThreadRun && m_bStop == false )
	{
		m_bStop = true;

		while( m_bThreadRun )
		{
			MiliSleep(20);
		}
	}

	m_clsMutex.acquire();

	if( m_psttSsl )
	{
		SSLClose( m_psttSsl );
		m_psttSsl = NULL;
	}

	if( m_hSocket != INVALID_SOCKET )
	{
		closesocket( m_hSocket );
		m_hSocket = INVALID_SOCKET;
	}

	m_clsMutex.release();
}

/**
 * @ingroup HttpStack
 * @brief 입력받은 데이터를 WebSocket 패킷으로 변환하여 전송한다.
 * @param strData 전송할 데이터
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWebSocketClient::Send( std::string & strData )
{
	return Send( E_WST_TEXT, strData.c_str(), strData.length() );
}

/**
 * @ingroup HttpStack
 * @brief 입력받은 데이터를 WebSocket 패킷으로 변환하여 전송한다.
 * @param eType			WebSocket 패킷 타입
 * @param pszData		전송할 데이터
 * @param iDataLen	전송할 데이터 길이
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWebSocketClient::Send( EWebSocketType eType, const char * pszData, int iDataLen )
{
	int iPacketLen = 0;

	if( iDataLen > 65536 )
	{
		iPacketLen = 2 + 8 + iDataLen + 4;
	}
	else if( iDataLen > 125 )
	{
		iPacketLen = 2 + 2 + iDataLen + 4;
	}
	else
	{
		iPacketLen = 2 + iDataLen + 4;
	}

	char * pszPacket = (char *)malloc( iPacketLen );
	if( pszPacket == NULL )
	{
		CLog::Print( LOG_ERROR, "%s malloc error", __FUNCTION__ );
		return false;
	}

	switch( eType )
	{
	case E_WST_TEXT:
		pszPacket[0] = (uint8_t)0x81;
		break;
	case E_WST_BINARY:
		pszPacket[0] = (uint8_t)0x82;
		break;
	case E_WST_PING:
		pszPacket[0] = (uint8_t)0x89;
		break;
	case E_WST_PONG:
		pszPacket[0] = (uint8_t)0x8A;
		break;
	}

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

	pszPacket[1] |= 0x80;
	memset( pszPacket + iPayLoadPos, 0, 4 );
	iPayLoadPos += 4;

	memcpy( pszPacket + iPayLoadPos, pszData, iDataLen );

	bool bRes = SendTcp( pszPacket, iPacketLen );

	free( pszPacket );

	return bRes;
}

/**
 * @ingroup HttpStack
 * @brief 입력한 패킷을 TCP 로 전송한다.
 * @param pszPacket		패킷
 * @param iPacketLen	패킷의 길이
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWebSocketClient::SendTcp( const char * pszPacket, int iPacketLen )
{
	bool bRes = false;

	m_clsMutex.acquire();

	if( m_psttSsl )
	{
		if( SSLSend( m_psttSsl, pszPacket, iPacketLen ) == iPacketLen )
		{
			bRes = true;
		}
	}
	else if( m_hSocket != INVALID_SOCKET )
	{
		if( TcpSend( m_hSocket, pszPacket, iPacketLen ) == iPacketLen )
		{
			bRes = true;
		}
	}

	m_clsMutex.release();

	std::string strHex;

	StringToHex( pszPacket, iPacketLen, strHex );

	if( bRes )
	{
		CLog::Print( LOG_NETWORK, "WsSend(%s:%d) hex[%s]", m_strServerIp.c_str(), m_iServerPort, strHex.c_str() );
	}
	else
	{
		CLog::Print( LOG_ERROR, "WsSend(%s:%d) hex[%s] error", m_strServerIp.c_str(), m_iServerPort, strHex.c_str() );
	}

	return bRes;
}

/**
 * @ingroup HttpStack
 * @brief WebSocket 이 종료되었는지 확인한다.
 * @returns WebSocket 이 연결되어 있지 않으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWebSocketClient::IsClosed()
{
	if( m_hSocket == INVALID_SOCKET )
	{
		return true;
	}

	return false;
}
