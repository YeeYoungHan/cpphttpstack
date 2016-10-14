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

#ifndef _TCP_STACK_CALLBACK_H_
#define _TCP_STACK_CALLBACK_H_

#include "TcpSessionList.h"

/**
 * @ingroup TcpStack
 * @brief CTcpStack 을 사용하는 응용 callback 인터페이스
 */
class ITcpStackCallBack
{
public:
	virtual ~ITcpStackCallBack(){};

	/**
	 * @brief TCP 서버로 클라이언트가 연결된 경우 호출된다.
	 * @param pclsSessionInfo 세션 정보
	 */
	virtual bool InComingConnected( CTcpSessionInfo * pclsSessionInfo ) = 0;
	virtual void SessionClosed( CTcpSessionInfo * pclsSessionInfo ) = 0;

	virtual bool RecvPacket( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo ) = 0;
};

#endif
