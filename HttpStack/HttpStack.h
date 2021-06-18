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

#ifndef _HTTP_STACK_H_
#define _HTTP_STACK_H_

#include "TcpStack.h"
#include "HttpPacket.h"
#include "Http2Packet.h"
#include "Http2Conversion.h"
#include "WebSocketPacket.h"
#include "HttpStackCallBack.h"

/**
 * @ingroup HttpStack
 * @brief HTTP 세션 타입
 */
enum EHttpSessionType
{
	E_HST_NULL = 0,
	E_HST_WEB_SOCKET,
	E_HST_HTTP_2
};

/**
 * @defgroup HttpStack HttpStack
 * HTTP 통신 라이브러리
 */

class CHttpMessageSendRecv
{
public:
	CHttpMessage m_clsSend;
	CHttpMessage m_clsRecv;
};

// key = HTTP/2 frame id
typedef std::map< int, CHttpMessageSendRecv > HTTP_MESSAGE_MAP;

/**
 * @ingroup HttpStack
 * @brief HTTP 서버의 각 세션 정보를 저장하는 클래스
 */
class CHttpStackSession : public ITcpSessionApp
{
public:
	CHttpStackSession() : m_eType(E_HST_NULL){};
	virtual ~CHttpStackSession(){};

	CHttpMessageSendRecv * GetMessage( int iId )
	{
		HTTP_MESSAGE_MAP::iterator itMap = m_clsMessageMap.find( iId );
		if( itMap == m_clsMessageMap.end() )
		{
			CHttpMessageSendRecv clsData;

			m_clsMessageMap.insert( HTTP_MESSAGE_MAP::value_type( iId, clsData ) );
			itMap = m_clsMessageMap.find( iId );
			if( itMap == m_clsMessageMap.end() )
			{
				return NULL;
			}
		}

		return &itMap->second;
	}

	CHttpPacket				m_clsHttpPacket;
	CHttp2Packet			m_clsHttp2Packet;
	CHttp2Conversion	m_clsSendConversion;
	CHttp2Conversion	m_clsRecvConversion;
	CHttp2FrameList		m_clsFrameList;
	CWebSocketPacket	m_clsWsPacket;
	EHttpSessionType	m_eType;

	HTTP_MESSAGE_MAP	m_clsMessageMap;
};

/**
 * @ingroup HttpStack
 * @brief HTTP 서버 클래스
 */
class CHttpStack : public ITcpStackCallBack
{
public:
	CHttpStack();
	virtual ~CHttpStack();

	bool Start( CTcpStackSetup * pclsSetup, IHttpStackCallBack * pclsCallBack );
	bool Stop( );
	static void Release();

	// HttpStackWebSocket.hpp
	bool SendWebSocketPacket( const char * pszClientIp, int iClientPort, const char * pszData, int iDataLen );

	// ITcpStackCallBack
	virtual bool InComingConnected( CTcpSessionInfo * pclsSessionInfo );
	virtual void SessionClosed( CTcpSessionInfo * pclsSessionInfo );
	virtual bool RecvPacket( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo );

private:
	// HttpStackHttp2.hpp
	bool RecvPacketHttp2Pri( CTcpSessionInfo * pclsSessionInfo, CHttpStackSession * pclsApp );
	bool RecvPacketHttp2( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo, CHttpStackSession * pclsApp );

	// HttpStackWebSocket.hpp
	bool RecvPacketWebSocket( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo, CHttpStackSession * pclsApp );
	bool MakeWebSocketResponse( CHttpMessage * pclsRecv, CHttpMessage * pclsSend );

	CTcpStack m_clsTcpStack;
	IHttpStackCallBack * m_pclsCallBack;
	static const EVP_MD * m_psttMd;
};

#endif
