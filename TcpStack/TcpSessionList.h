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

#ifndef _TCP_SESSION_LIST_H_
#define _TCP_SESSION_LIST_H_

#include "SipTcp.h"
#include "SipMutex.h"
#include "TlsFunction.h"
#include <string>

class CTcpComm;
class ITcpStackCallBack;

/**
 * @ingroup TcpStack
 * @brief TCP ���Ǻ��� ���뿡�� ����� ������ ���� �������̽�
 */
class ITcpSessionApp
{
public:
	virtual ~ITcpSessionApp(){};
};

/**
 * @ingroup TcpStack
 * @brief TCP ������ ���� �����ϴ� TCP ���� ����
 */
class CTcpSessionInfo
{
public:
	CTcpSessionInfo();
	~CTcpSessionInfo();

	bool Send( const char * pszPacket, int iPacketLen );
	void Log( const char * pszPacket, int iPacketLen, bool bSend );
	void Clear();

	std::string		m_strIp;
	int						m_iPort;

	Socket				m_hSocket;
	SSL						* m_psttSsl;

	CSipMutex			m_clsMutex;

	time_t				m_iConnectTime;
	time_t				m_iRecvTime;
	time_t				m_iSendTime;	// Ŭ���̾�Ʈ�� ����� ��츸 �����Ѵ�.

	int						m_iThreadIndex;
	int						m_iSessionIndex;

	ITcpSessionApp * m_pclsApp;
};

/**
 * @ingroup TcpStack
 * @brief TCP ������ ���� �����ϴ� TCP ���� ������ �����ϴ� Ŭ���� - m_bUseThreadPipe �� true �� ���� ���ȴ�.
 */
class CTcpSessionList
{
public:
	CTcpSessionList( );
	~CTcpSessionList( );

	bool Init( int iThreadIndex, int iPollFdMax );
	bool Insert( Socket hSocket );
	int Insert( CTcpComm & clsTcpComm );
	bool Delete( int iIndex );
	void DeleteAll( );
	void DeleteTimeout( int iTimeout, int iNoPacketTimeout
		, void (*BeforeDelete)( CTcpSessionList * pclsSessionList, int iIndex, void * pclsArg ), void * pclsArg );

	bool Send( int iIndex, const char * pszPacket, int iPacketLen );
	bool SendAll( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack );
	bool SendAllExcept( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack, int iThreadIndex, int iSessionIndex );

	struct pollfd * m_psttPollFd;
	CTcpSessionInfo * m_pclsSession;

	int	m_iPollFdMax;
	int m_iPoolFdCount;

private:
	void Insert( int iIndex, CTcpComm & clsTcpComm );
	void ClearFd( int iIndex );
};

#endif
