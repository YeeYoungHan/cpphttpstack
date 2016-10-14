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
#include "ServerUtility.h"
#include "TimeUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

THREAD_API TcpThread( LPVOID lpParameter );

CTcpThreadInfo::CTcpThreadInfo() : m_iIndex(0), m_hSend(INVALID_SOCKET), m_hRecv(INVALID_SOCKET), m_bStop(false)
{

}

CTcpThreadInfo::~CTcpThreadInfo()
{
	Close();
}

/**
 * @ingroup TcpStack
 * @brief 소켓을 종료한다.
 */
void CTcpThreadInfo::Close()
{
	if( m_hSend != INVALID_SOCKET )
	{
		closesocket( m_hSend );
		m_hSend = INVALID_SOCKET;
	}

	if( m_hRecv != INVALID_SOCKET )
	{
		closesocket( m_hRecv );
		m_hRecv = INVALID_SOCKET;
	}
}

CTcpThreadList::CTcpThreadList() : m_pclsStack(NULL)
{
}

CTcpThreadList::~CTcpThreadList()
{
}

/**
 * @ingroup TcpStack
 * @brief 쓰레드 리스트를 시작한다.
 * @param pclsStack	CTcpStack 객체
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CTcpThreadList::Create( CTcpStack * pclsStack )
{
	// pipe 연산을 수행해야 하므로 1개 추가한다.
	m_iMaxSocketPerThread = pclsStack->m_clsSetup.m_iMaxSocketPerThread + 1;
	m_pclsStack = pclsStack;

	for( int i = 0; i < pclsStack->m_clsSetup.m_iThreadInitCount; ++i )
	{
		if( AddThread() == false )
		{
			return false;
		}
	}

	return true;
}

/**
 * @ingroup TcpStack
 * @brief 쓰레드 리스트를 종료한다.
 */
void CTcpThreadList::Destroy()
{
	THREAD_LIST::iterator	it;

	for( it = m_clsList.begin(); it != m_clsList.end(); ++it )
	{
		(*it)->m_bStop = true;
	}

	for( int i = 0; i < 100; ++i )
	{
		MiliSleep( 20 );

		bool bAllStop = true;

		for( it = m_clsList.begin(); it != m_clsList.end(); ++it )
		{
			if( (*it)->m_bStop )
			{
				bAllStop = false;
				break;
			}
		}

		if( bAllStop ) break;
	}

	for( it = m_clsList.begin(); it != m_clsList.end(); ++it )
	{
		(*it)->Close();
		delete (*it);
	}
}

/**
 * @ingroup TcpStack
 * @brief 쓰레드에 명령을 전송한다.
 * @param pszData				명령
 * @param iDataLen			pszData 길이
 * @param iThreadIndex	명령을 전송할 쓰레드 인덱스
 *											-1 을 입력하면 최대 소켓 개수를 넘지 않은 쓰레드에 명령이 전송된다.
 * @param piThreadIndex	실제 전송된 쓰레드 인덱스를 저장하는 변수
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CTcpThreadList::SendCommand( const char * pszData, int iDataLen, int iThreadIndex, int * piThreadIndex )
{
	bool	bRes = false, bFound = false;

	if( iThreadIndex < -1 )
	{
		return false;
	}

	if( iThreadIndex >= (int)m_clsList.size() )
	{
		return false;
	}

	m_clsMutex.acquire();
	if( iThreadIndex == -1 )
	{
		THREAD_LIST::iterator	it;
		int iMinCount = 2000000000;

		// 소켓을 최소 사용하는 쓰레드를 검색한다.
		for( it = m_clsList.begin(), iThreadIndex = 0; it != m_clsList.end(); ++it, ++iThreadIndex )
		{
			if( iMinCount > (*it)->m_clsSessionList.m_iPoolFdCount )
			{
				iMinCount = (*it)->m_clsSessionList.m_iPoolFdCount;
				if( iMinCount == 0 ) break;
			}
		}

		if( iMinCount < m_iMaxSocketPerThread )
		{
			for( it = m_clsList.begin(), iThreadIndex = 0; it != m_clsList.end(); ++it, ++iThreadIndex )
			{
				if( iMinCount == (*it)->m_clsSessionList.m_iPoolFdCount )
				{
					bRes = _SendCommand( (*it)->m_hSend, pszData, iDataLen );
					if( piThreadIndex ) *piThreadIndex = iThreadIndex;
					bFound = true;
					break;
				}
			}
		}

		if( bFound == false )
		{
			if( AddThread() )
			{
				bRes = _SendCommand( m_clsList[m_clsList.size()-1]->m_hSend, pszData, iDataLen );
				if( piThreadIndex ) *piThreadIndex = iThreadIndex;
			}
		}
	}
	else
	{
		bRes = _SendCommand( m_clsList[iThreadIndex]->m_hSend, pszData, iDataLen );
		if( piThreadIndex ) *piThreadIndex = iThreadIndex;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief 모든 쓰레드에 명령을 전달한다.
 * @param pszData		명령
 * @param iDataLen	pszData 길이
 */
void CTcpThreadList::SendCommandAll( const char * pszData, int iDataLen )
{
	THREAD_LIST::iterator	it;

	m_clsMutex.acquire();
	for( it = m_clsList.begin(); it != m_clsList.end(); ++it )
	{
		_SendCommand( (*it)->m_hSend, pszData, iDataLen );
	}
	m_clsMutex.release();
}

/**
 * @ingroup TcpStack
 * @brief 명령을 수신한다.
 * @param hSocket		pipe 수신 핸들
 * @param pszData		명령 저장 변수
 * @param iDataSize pszData 변수 크기
 * @returns 수신한 길이를 리턴한다.
 */
int CTcpThreadList::RecvCommand( Socket hSocket, char * pszData, int iDataSize )
{
	int	n;

#ifdef WIN32
	n = recv( hSocket, pszData, iDataSize, 0 );
#else
	n = read( hSocket, pszData, iDataSize );
#endif
	
	return n;
}

/**
 * @ingroup TcpStack
 * @brief TCP 패킷을 전송한다.
 * @param iThreadIndex	TCP 쓰레드 번호
 * @param iSessionIndex TCP 세션 번호
 * @param pszPacket			패킷
 * @param iPacketLen		패킷 길이
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CTcpThreadList::Send( int iThreadIndex, int iSessionIndex, const char * pszPacket, int iPacketLen )
{
	return m_clsList[iThreadIndex]->m_clsSessionList.Send( iSessionIndex, pszPacket, iPacketLen );
}

/**
 * @ingroup TcpStack
 * @brief TCP 쓰레드 번호에 대한 정보를 가져온다.
 * @param iThreadIndex		TCP 쓰레드 번호
 * @param ppclsThreadInfo TCP 쓰레드 정보
 * @returns true 를 리턴한다.
 */
bool CTcpThreadList::Select( int iThreadIndex, CTcpThreadInfo ** ppclsThreadInfo )
{
	*ppclsThreadInfo = m_clsList[iThreadIndex];

	return true;
}

/**
 * @ingroup TcpStack
 * @brief 쓰레드 정보를 하나의 문자열에 저장한다.
 * @param strBuf 쓰레드 정보를 저장할 문자열 변수
 */
void CTcpThreadList::GetString( CMonitorString & strBuf )
{
	THREAD_LIST::iterator	it;

	strBuf.Clear();

	for( it = m_clsList.begin(); it != m_clsList.end(); ++it )
	{
		strBuf.AddRow( (*it)->m_clsSessionList.m_iPoolFdCount );
	}
}

/**
 * @ingroup TcpStack
 * @brief 쓰레드를 추가한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpThreadList::AddThread()
{
	if( (int)m_clsList.size() >= m_pclsStack->m_clsSetup.m_iThreadMaxCount )
	{
		CLog::Print( LOG_ERROR, "%s thread count(%d) >= max thread count(%d)", __FUNCTION__, (int)m_clsList.size(), m_pclsStack->m_clsSetup.m_iThreadMaxCount );
		return false;
	}

	Socket	arrSocket[2];
	CTcpThreadInfo * pclsTcpThreadInfo = new CTcpThreadInfo();
	if( pclsTcpThreadInfo == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsTcpThreadInfo->m_iIndex = m_clsList.size();

	if( pclsTcpThreadInfo->m_clsSessionList.Init( pclsTcpThreadInfo->m_iIndex, m_iMaxSocketPerThread ) == false )
	{
		CLog::Print( LOG_ERROR, "%s m_clsSessionList.Init error", __FUNCTION__ );
		delete pclsTcpThreadInfo;
		return false;
	}

	if( pipe( arrSocket ) == -1 )
	{
		CLog::Print( LOG_ERROR, "%s pipe error", __FUNCTION__ );
		delete pclsTcpThreadInfo;
		return false;
	}

	pclsTcpThreadInfo->m_hRecv = arrSocket[0];
	pclsTcpThreadInfo->m_hSend = arrSocket[1];
	pclsTcpThreadInfo->m_clsSessionList.Insert( pclsTcpThreadInfo->m_hRecv );
	pclsTcpThreadInfo->m_pclsStack = m_pclsStack;

	m_clsList.push_back( pclsTcpThreadInfo );

	bool bRes = StartThread( "TcpThread", TcpThread, pclsTcpThreadInfo );
	if( bRes == false )
	{
		delete pclsTcpThreadInfo;
	}

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief 쓰레드에 명령을 전송한다.
 * @param hSocket		pipe 전송 핸들
 * @param pszData		명령
 * @param iDataLen	pszData 길이
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CTcpThreadList::_SendCommand( Socket hSocket, const char * pszData, int iDataLen )
{
	int	n;

#ifdef WIN32
	n = send( hSocket, pszData, iDataLen, 0 );
#else
	n = write( hSocket, pszData, iDataLen );
#endif
	if( n != iDataLen )
	{
		return false;
	}

	return true;
}
