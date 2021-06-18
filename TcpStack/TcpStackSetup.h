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

#include "XmlElement.h"

/**
 * @ingroup TcpStack
 * @brief TCP stack ����
 */
class CTcpStackSetup
{
public:
	CTcpStackSetup();
	~CTcpStackSetup();

	bool Parse( CXmlElement & clsXml );

	/** TCP listen IP �ּ� - any �̸� �������� �Է��Ѵ�. */
	std::string m_strListenIp;

	/** TCP listen ��Ʈ ��ȣ - TCP listen ���� �ʴ� ��쿡�� �������� ������ �ȴ�. */
	int m_iListenPort;

	/** �� ���� Thread ���� - �ּ� ������ ���� */
	int m_iThreadInitCount;

	/** �ִ� ���� Thread ���� - 0 ���� ū ������ �����ϸ� �ش� ������ŭ�� �����带 �����Ѵ�. 0 �̸� �ý����� ����ϴ� ��ŭ �����带 �����Ѵ�. */
	int m_iThreadMaxCount;

	/** �ϳ��� Thread ���� ����� �� �ִ� �ִ� ���� ���� */
	int m_iMaxSocketPerThread;

	/** TCP ���� timeout */
	int m_iTcpRecvTimeout;

	/** TCP ���� ��, ���� ��Ŷ�� ���ŵ��� ���� ����� timeout */
	int m_iTcpNoPacketTimeout;

	/** TCP ���� timeout */
	int m_iTcpConnectTimeout;

	/** TLS �� ����ϴ��� �����Ѵ�. true �� �Է��ϸ� TLS �� ����ϴ� ���̰� false �� �Է��ϸ� �� ����Ѵ�.
			�� �׸��� true �� �����ϸ� m_strCertFile �� �����ؾ� �Ѵ�. */
	bool m_bUseTls;

	/** TLS ������ ���� ����Ű/������ PEM ���� ���� �̸� ( full path ) */
	std::string m_strCertFile;

	/** ������� ��ſ� pipe �� ����� ������ �����Ѵ�. ������� ����� pipe �� �Ѵٴ� ���� thread pool �� ����Ѵٴ� ���� �ǹ��Ѵ�. 
			thread pool �� �̿��� ��� true �� �Է��ϰ� �׷��� ������ false �� �Է��Ѵ�. */
	bool m_bUseThreadPipe;

	/** HTTP/2 �� ����ϸ� true �� �Է��ϰ� �׷��� ������ false �� �Է��Ѵ�. */
	bool m_bUseHttp2;
};

#endif
