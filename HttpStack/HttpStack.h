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
#include "WebSocketPacket.h"
#include "HttpStackCallBack.h"

/**
 * @ingroup HttpStack
 * @brief HTTP ���� Ÿ��
 */
enum EHttpSessionType
{
	E_HST_NULL = 0,
	E_HST_WEB_SOCKET,
	E_HST_HTTP_2
};

/**
 * @defgroup HttpStack HttpStack
 * HTTP ��� ���̺귯��
 */

/**
 * @ingroup HttpStack
 * @brief HTTP ������ �� ���� ������ �����ϴ� Ŭ����
 */
class CHttpStackSession : public ITcpSessionApp
{
public:
	CHttpStackSession() : m_eType(E_HST_NULL){};
	virtual ~CHttpStackSession(){};

	CHttpPacket				m_clsHttpPacket;
	CWebSocketPacket	m_clsWsPacket;
	EHttpSessionType	m_eType;
};

/**
 * @ingroup HttpStack
 * @brief HTTP ���� Ŭ����
 */
class CHttpStack : public ITcpStackCallBack
{
public:
	CHttpStack();
	virtual ~CHttpStack();

	bool Start( CTcpStackSetup * pclsSetup, IHttpStackCallBack * pclsCallBack );
	bool Stop( );

	bool SendWebSocketPacket( const char * pszClientIp, int iClientPort, const char * pszData, int iDataLen );

	static void Release();

	// ITcpStackCallBack
	virtual bool InComingConnected( CTcpSessionInfo * pclsSessionInfo );
	virtual void SessionClosed( CTcpSessionInfo * pclsSessionInfo );

	virtual bool RecvPacket( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo );

private:
	bool MakeWebSocketResponse( CHttpMessage * pclsRecv, CHttpMessage * pclsSend );

	CTcpStack m_clsTcpStack;
	IHttpStackCallBack * m_pclsCallBack;
	static const EVP_MD * m_psttMd;
};

#endif
