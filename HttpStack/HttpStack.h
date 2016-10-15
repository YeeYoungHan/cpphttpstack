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
#include "HttpStackCallBack.h"

/**
 * @ingroup HttpStack
 * @brief HTTP 서버의 각 세션 정보를 저장하는 클래스
 */
class CHttpStackSession : public ITcpSessionApp
{
public:
	CHttpStackSession(){};
	virtual ~CHttpStackSession(){};

	CHttpPacket m_clsPacket;
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

	// ITcpStackCallBack
	virtual bool InComingConnected( CTcpSessionInfo * pclsSessionInfo );
	virtual void SessionClosed( CTcpSessionInfo * pclsSessionInfo );

	virtual bool RecvPacket( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo );

private:
	CTcpStack m_clsTcpStack;
	IHttpStackCallBack * m_pclsCallBack;
};

#endif
