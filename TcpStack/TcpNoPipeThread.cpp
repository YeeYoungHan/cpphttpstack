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
#include "TlsFunction.h"
#include "TimeUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

/**
 * @ingroup TcpStack
 * @brief pipe 를 사용하지 않는 TCP 세션 쓰레드 함수
 * @param lpParameter CThreadListEntry 객체의 포인터
 * @returns 0 을 리턴한다.
 */
THREAD_API TcpNoPipeThread( LPVOID lpParameter )
{
	CTcpThreadInfo * pclsThreadInfo = (CTcpThreadInfo *)lpParameter;
	CTcpStack * pclsStack = pclsThreadInfo->m_pclsStack;


	return 0;
}
