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

#include "SipPlatformDefine.h"
#include "TcpStack.h"
#include "Log.h"
#include "MemoryDebug.h"

/**
 * @ingroup TcpStack
 * @brief TCP 서버 쓰레드
 * @param lpParameter CTcpStack 객체
 * @returns 0 을 리턴한다.
 */
THREAD_API TcpListenThread( LPVOID lpParameter )
{
	CTcpStack * pclsStack = (CTcpStack *)lpParameter;
	CTcpComm clsTcpComm;
	pollfd sttPoll[1];
	int		n;

	CLog::Print( LOG_INFO, "TcpListenThread started(port=%d)", pclsStack->m_clsSetup.m_iListenPort );

	TcpSetPollIn( sttPoll[0], pclsStack->m_hTcpListenFd );

	while( pclsStack->m_bStop == false )
	{
		n = poll( sttPoll, 1, 1000 );
		if( n <= 0 )
		{
			continue;
		}

		clsTcpComm.m_hSocket = TcpAccept( pclsStack->m_hTcpListenFd, clsTcpComm.m_szIp, sizeof(clsTcpComm.m_szIp), &clsTcpComm.m_iPort );
		if( clsTcpComm.m_hSocket != INVALID_SOCKET )
		{
			if( pclsStack->m_clsThreadList.SendCommand( (char *)&clsTcpComm, sizeof(clsTcpComm) ) == false )
			{
				CLog::Print( LOG_ERROR, "%s m_clsThreadList.SendCommand error", __FUNCTION__ );
			}
		}

		// QQQ: 초기 실행 쓰레드 개수보다 많은 경우, thread 에 pipe socket 만 존재하면 해당 쓰레드를 종료한다.
	}

	CLog::Print( LOG_INFO, "TcpListenThread terminated(port=%d)", pclsStack->m_clsSetup.m_iListenPort );

	return 0;
}
