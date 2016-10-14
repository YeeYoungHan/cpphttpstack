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

#ifndef _TCP_STACK_SETUP_H_
#define _TCP_STACK_SETUP_H_

#include <string>

/**
 * @ingroup TcpStack
 * @brief TCP stack 설정
 */
class CTcpStackSetup
{
public:
	CTcpStackSetup();
	~CTcpStackSetup();

	/** TCP listen 포트 번호 - TCP listen 하지 않는 경우에는 설정하지 않으면 된다. */
	int m_iListenPort;

	/** 최초 실행 Thread 개수 */
	int m_iThreadInitCount;

	/** 최대 실행 Thread 개수 */
	int m_iThreadMaxCount;

	/** 하나의 Thread 에서 사용할 수 있는 최대 소켓 개수 */
	int m_iMaxSocketPerThread;

	/** TCP 수신 timeout */
	int m_iTcpRecvTimeout;

	/** TCP 연결 후, 최초 패킷이 수신되지 않은 경우의 timeout */
	int m_iTcpNoPacketTimeout;

	/** TCP 연결 timeout */
	int m_iTcpConnectTimeout;

	/** TLS 를 사용하는가? */
	bool m_bUseTls;

	/** TLS 서버를 위한 개인키/인증서 PEM 저장 파일 이름 ( full path ) */
	std::string m_strCertFile;
};

#endif
