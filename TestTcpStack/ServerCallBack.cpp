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

#include "TestTcpStack.h"

CServerCallBack gclsServerCallBack;

CServerCallBack::CServerCallBack()
{
}

CServerCallBack::~CServerCallBack()
{
}

bool CServerCallBack::InComingConnected( CTcpSessionInfo * pclsSessionInfo )
{
	return true;
}

void CServerCallBack::SessionClosed( CTcpSessionInfo * pclsSessionInfo )
{

}

bool CServerCallBack::RecvPacket( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo )
{
	gclsServer.SendAll( pszPacket, iPacketLen );
	//gclsServer.SendAllExcept( pszPacket, iPacketLen, pclsSessionInfo->m_iThreadIndex, pclsSessionInfo->m_iSessionIndex );

	return true;
}

bool CServerCallBack::IsSendAll( CTcpSessionInfo * pclsSessionInfo )
{
	//if( pclsSessionInfo->m_iSessionIndex % 2 == 0 ) return false;

	return true;
}
