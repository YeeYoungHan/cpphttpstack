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

#include "TcpStack.h"
#include "TcpThread.h"
#include "ServerUtility.h"
#include "TimeUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

CTcpStack::CTcpStack() : m_pclsCallBack(NULL), m_hTcpListenFd(INVALID_SOCKET), m_bStop(false)
{
	InitNetwork();
}

CTcpStack::~CTcpStack()
{
}

/**
 * @ingroup TcpStack
 * @brief TCP stack �� �����Ѵ�.
 * @param pclsSetup			TCP stack ����
 * @param pclsCallBack	TCP stack callback
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpStack::Start( CTcpStackSetup * pclsSetup, ITcpStackCallBack * pclsCallBack )
{
	if( pclsSetup == NULL || pclsCallBack == NULL ) return false;

	m_clsSetup = *pclsSetup;
	m_pclsCallBack = pclsCallBack;
	m_clsClientMap.Create( this );

	if( pclsSetup->m_iListenPort > 0 )
	{
		if( pclsSetup->m_bUseTls )
		{
			if( SSLServerStart( pclsSetup->m_strCertFile.c_str(), pclsSetup->m_strCipherList.c_str() ) == false )
			{
				CLog::Print( LOG_ERROR, "SSLServerStart error" );
				return false;
			}

			if( pclsSetup->m_bUseHttp2 )
			{
				SSLServerSetHttp2();
			}
		}

		if( pclsSetup->m_strListenIp.empty() )
		{
			m_hTcpListenFd = TcpListen( pclsSetup->m_iListenPort, 255 );
		}
		else
		{
			m_hTcpListenFd = TcpListen( pclsSetup->m_iListenPort, 255, pclsSetup->m_strListenIp.c_str() );
		}

		if( m_hTcpListenFd == INVALID_SOCKET )
		{
			CLog::Print( LOG_ERROR, "TcpListen(%d) error(%d)", pclsSetup->m_iListenPort, GetError() );
			return false;
		}

		if( StartThread( "TcpListenThread", TcpListenThread, this ) == false )
		{
			Stop();
			return false;
		}
	}
	else if( pclsSetup->m_bUseTls )
	{
		SSLClientStart();
	}

	if( m_clsSetup.m_bUseThreadPipe )
	{
		if( m_clsThreadList.Create( this ) == false )
		{
			Stop();
			return false;
		}
	}

	return true;
}

/**
 * @ingroup TcpStack
 * @brief TCP stack �� �����Ѵ�.
 * @returns true �� �����Ѵ�.
 */
bool CTcpStack::Stop( )
{
	m_bStop = true;

	if( m_clsSetup.m_bUseThreadPipe )
	{
		m_clsThreadList.Destroy();
	}
	else
	{
		// TCP �����尡 ����� ������ �ִ� 2�� ����Ѵ�.
		for( int i = 0; i < 100; ++i )
		{
			if( m_clsSessionMap.GetCount() == 0 ) break;
			MiliSleep( 20 );
		}
	}

	// TCP listen �����尡 ����� ������ �ִ� 2�� ����Ѵ�.
	for( int i = 0; i < 100; ++i )
	{
		if( IsTcpListenThreadRun() == false )	break;
		MiliSleep( 20 );
	}

	if( m_hTcpListenFd != INVALID_SOCKET )
	{
		closesocket( m_hTcpListenFd );
		m_hTcpListenFd = INVALID_SOCKET;
	}

	if( m_clsSetup.m_bUseTls )
	{
		if( m_clsSetup.m_iListenPort > 0 )
		{
			SSLServerStop();
		}
		else
		{
			SSLClientStop();
		}
	}

	return true;
}

/**
 * @ingroup TcpStack
 * @brief Ư�� ���ǿ� TCP ��Ŷ�� �����Ѵ�.
 * @param pszIp				IP �ּ�
 * @param iPort				��Ʈ ��ȣ
 * @param pszPacket		��Ŷ
 * @param iPacketLen	��Ŷ ����
 * @param bConnectIfNoSession	TCP ������ �������� ������ ���ο� TCP ������ ������ ��, ��Ŷ�� �����ϴ°�?
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpStack::Send( const char * pszIp, int iPort, const char * pszPacket, int iPacketLen, bool bConnectIfNoSession )
{
	if( m_clsSetup.m_bUseThreadPipe )
	{
		return m_clsClientMap.Send( pszIp, iPort, pszPacket, iPacketLen, bConnectIfNoSession );
	}

	if( m_clsSessionMap.Send( pszIp, iPort, pszPacket, iPacketLen ) == false )
	{
		// m_clsSessionMap �� �������� �����Ƿ� TCP connect �ϰ� TcpNoPipeThread �� �����ϴ� ������ �����Ѵ�.
		return m_clsClientMap.Send( pszIp, iPort, pszPacket, iPacketLen, bConnectIfNoSession );
	}

	return true;
}

/**
 * @ingroup TcpStack
 * @brief Ư�� ���ǿ� TCP ��Ŷ�� �����Ѵ�.
 * @param iThreadIndex	TCP ������ ��ȣ
 * @param iSessionIndex TCP ���� ��ȣ
 * @param pszPacket			��Ŷ
 * @param iPacketLen		��Ŷ ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpStack::Send( int iThreadIndex, int iSessionIndex, const char * pszPacket, int iPacketLen )
{
	if( m_clsSetup.m_bUseThreadPipe )
	{
		return m_clsThreadList.Send( iThreadIndex, iSessionIndex, pszPacket, iPacketLen );
	}

	return m_clsSessionMap.Send( iThreadIndex, pszPacket, iPacketLen );
}

/**
 * @ingroup TcpStack
 * @brief ��� ���ǿ� TCP ��Ŷ�� �����Ѵ�.
 * @param pszPacket			��Ŷ
 * @param iPacketLen		��Ŷ ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpStack::SendAll( const char * pszPacket, int iPacketLen )
{
	if( m_clsSetup.m_bUseThreadPipe )
	{
		return m_clsThreadList.SendAll( pszPacket, iPacketLen, m_pclsCallBack );
	}

	return m_clsSessionMap.SendAll( pszPacket, iPacketLen, m_pclsCallBack );
}

/**
 * @ingroup TcpStack
 * @brief Ư�� ������ ������ ��� ���ǿ� TCP ��Ŷ�� �����Ѵ�.
 * @param pszPacket			��Ŷ
 * @param iPacketLen		��Ŷ ����
 * @param iThreadIndex	�������� ���� ������ ������ �ε���
 * @param iSessionIndex �������� ���� ���� �ε���
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpStack::SendAllExcept( const char * pszPacket, int iPacketLen, int iThreadIndex, int iSessionIndex )
{
	if( m_clsSetup.m_bUseThreadPipe )
	{
		return m_clsThreadList.SendAllExcept( pszPacket, iPacketLen, m_pclsCallBack, iThreadIndex, iSessionIndex );
	}

	return m_clsSessionMap.SendAllExcept( pszPacket, iPacketLen, m_pclsCallBack, iThreadIndex, iSessionIndex );
}
