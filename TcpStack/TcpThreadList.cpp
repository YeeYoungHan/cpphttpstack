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
#include "TcpThread.h"
#include "ServerUtility.h"
#include "TimeUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

CTcpThreadInfo::CTcpThreadInfo() : m_iIndex(0), m_hSend(INVALID_SOCKET), m_hRecv(INVALID_SOCKET)
{

}

CTcpThreadInfo::~CTcpThreadInfo()
{
	Close();
}

/**
 * @ingroup TcpStack
 * @brief ������ �����Ѵ�.
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

CTcpThreadList::CTcpThreadList() : m_iMaxSocketPerThread(0), m_iThreadIndex(0), m_pclsStack(NULL)
{
}

CTcpThreadList::~CTcpThreadList()
{
}

/**
 * @ingroup TcpStack
 * @brief ������ ����Ʈ�� �����Ѵ�.
 * @param pclsStack	CTcpStack ��ü
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpThreadList::Create( CTcpStack * pclsStack )
{
	if( pclsStack->m_clsSetup.m_iMaxSocketPerThread <= 0 )
	{
		CLog::Print( LOG_ERROR, "%s m_iMaxSocketPerThread(%d) is invalid", __FUNCTION__, pclsStack->m_clsSetup.m_iMaxSocketPerThread );
		return false;
	}

	// pipe ������ �����ؾ� �ϹǷ� 1�� �߰��Ѵ�.
	m_iMaxSocketPerThread = pclsStack->m_clsSetup.m_iMaxSocketPerThread + 1;
	m_pclsStack = pclsStack;

	bool bError = false;

	m_clsMutex.acquire();
	for( int i = 0; i < pclsStack->m_clsSetup.m_iThreadInitCount; ++i )
	{
		if( AddThread() == false )
		{
			bError = true;
			break;
		}
	}
	m_clsMutex.release();

	if( bError )
	{
		Destroy();
		return false;
	}

	return true;
}

/**
 * @ingroup TcpStack
 * @brief ������ ����Ʈ�� �����Ѵ�.
 */
void CTcpThreadList::Destroy()
{
	THREAD_LIST::iterator	itTL;
	CTcpComm		clsTcpComm;

	SendCommandAll( (char *)&clsTcpComm, sizeof(clsTcpComm) );

	m_clsMutex.acquire();
	m_clsList.clear();
	m_clsMutex.release();
}

/**
 * @ingroup TcpStack
 * @brief �����忡 ����� �����Ѵ�.
 * @param pszData				���
 * @param iDataLen			pszData ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpThreadList::SendCommand( const char * pszData, int iDataLen )
{
	bool	bRes = false, bFound = false;
	THREAD_LIST::iterator	itTL;
	int iMinCount = 2000000000;

	m_clsMutex.acquire();

	// ������ �ּ� ����ϴ� �����带 �˻��Ѵ�.
	for( itTL = m_clsList.begin(); itTL != m_clsList.end(); ++itTL )
	{
		if( iMinCount > (*itTL)->m_clsSessionList.m_iPoolFdCount )
		{
			iMinCount = (*itTL)->m_clsSessionList.m_iPoolFdCount;
			if( iMinCount == 0 ) break;
		}
	}

	if( iMinCount < m_iMaxSocketPerThread )
	{
		for( itTL = m_clsList.begin(); itTL != m_clsList.end(); ++itTL )
		{
			if( iMinCount == (*itTL)->m_clsSessionList.m_iPoolFdCount )
			{
				bRes = _SendCommand( (*itTL)->m_hSend, pszData, iDataLen );
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
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief ������ ������ ��ȣ�� ������� ����� �����Ѵ�.
 * @param pszData				���
 * @param iDataLen			pszData ����
 * @param iThreadIndex	������ �ε���
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpThreadList::SendCommand( const char * pszData, int iDataLen, int iThreadIndex )
{
	bool	bRes = false;
	THREAD_LIST::iterator	itTL;

	m_clsMutex.acquire();
	for( itTL = m_clsList.begin(); itTL != m_clsList.end(); ++itTL )
	{
		if( (*itTL)->m_iIndex == iThreadIndex )
		{
			bRes = _SendCommand( (*itTL)->m_hSend, pszData, iDataLen );
			break;
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief ��� �����忡 ����� �����Ѵ�.
 * @param pszData		���
 * @param iDataLen	pszData ����
 */
void CTcpThreadList::SendCommandAll( const char * pszData, int iDataLen )
{
	THREAD_LIST::iterator	itTL;

	m_clsMutex.acquire();
	for( itTL = m_clsList.begin(); itTL != m_clsList.end(); ++itTL )
	{
		_SendCommand( (*itTL)->m_hSend, pszData, iDataLen );
	}
	m_clsMutex.release();
}

/**
 * @ingroup TcpStack
 * @brief ����� �����Ѵ�.
 * @param hSocket		pipe ���� �ڵ�
 * @param pszData		��� ���� ����
 * @param iDataSize pszData ���� ũ��
 * @returns ������ ���̸� �����Ѵ�.
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
 * @brief Ư�� ���ǿ� TCP ��Ŷ�� �����Ѵ�.
 * @param iThreadIndex	TCP ������ ��ȣ
 * @param iSessionIndex TCP ���� ��ȣ
 * @param pszPacket			��Ŷ
 * @param iPacketLen		��Ŷ ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpThreadList::Send( int iThreadIndex, int iSessionIndex, const char * pszPacket, int iPacketLen )
{
	bool bRes = false;

	m_clsMutex.acquire();
	int iCount = (int)m_clsList.size();
	for( int i = 0; i < iCount; ++i )
	{
		if( m_clsList[i]->m_iIndex == iThreadIndex )
		{
			bRes = m_clsList[i]->m_clsSessionList.Send( iSessionIndex, pszPacket, iPacketLen );
			break;
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief ��� ���ǿ� TCP ��Ŷ�� �����Ѵ�.
 * @param pszPacket			��Ŷ
 * @param iPacketLen		��Ŷ ����
 * @param pclsCallBack	���Ǻ��� ���� ������ �����ϴ� callback ��ü
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpThreadList::SendAll( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack )
{
	THREAD_LIST::iterator	itTL;

	m_clsMutex.acquire();
	for( itTL = m_clsList.begin(); itTL != m_clsList.end(); ++itTL )
	{
		(*itTL)->m_clsSessionList.SendAll( pszPacket, iPacketLen, pclsCallBack );
	}
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup TcpStack
 * @brief ��� ���ǿ� TCP ��Ŷ�� �����Ѵ�.
 * @param pszPacket			��Ŷ
 * @param iPacketLen		��Ŷ ����
 * @param pclsCallBack	���Ǻ��� ���� ������ �����ϴ� callback ��ü
 * @param iThreadIndex	�������� ���� ������ ������ �ε���
 * @param iSessionIndex �������� ���� ���� �ε���
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpThreadList::SendAllExcept( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack, int iThreadIndex, int iSessionIndex )
{
	THREAD_LIST::iterator	itTL;

	m_clsMutex.acquire();
	for( itTL = m_clsList.begin(); itTL != m_clsList.end(); ++itTL )
	{
		(*itTL)->m_clsSessionList.SendAllExcept( pszPacket, iPacketLen, pclsCallBack, iThreadIndex, iSessionIndex );
	}
	m_clsMutex.release();

	return true;
}

typedef std::list< int > THREAD_INDEX_LIST;

/**
 * @ingroup TcpStack
 * @brief TCP Ŭ���̾�Ʈ�� ������� ���� �����带 �����Ѵ�.
 */
void CTcpThreadList::DeleteNoUseThread()
{
	THREAD_LIST::iterator	itTL;
	THREAD_INDEX_LIST clsDeleteList;
	THREAD_INDEX_LIST::iterator itTIL;
	int iUseCount = 0;

	m_clsMutex.acquire();
	for( itTL = m_clsList.begin(); itTL != m_clsList.end(); ++itTL )
	{
		if( (*itTL)->m_clsSessionList.m_iPoolFdCount == 1 )
		{
			clsDeleteList.push_back( (*itTL)->m_iIndex );
		}
		else
		{
			++iUseCount;
		}
	}
	m_clsMutex.release();

	if( clsDeleteList.size() > 0 )
	{
		// �ʱ� ���� ������ ������ŭ�� �����Ǿ�� �Ѵ�.
		if( m_pclsStack->m_clsSetup.m_iThreadInitCount > iUseCount )
		{
			int iDeleteCount = m_pclsStack->m_clsSetup.m_iThreadInitCount - iUseCount;

			if( iDeleteCount >= (int)clsDeleteList.size() )
			{
				clsDeleteList.clear();
			}
			else
			{
				THREAD_INDEX_LIST::iterator itNext;

				for( itTIL = clsDeleteList.begin(); itTIL != clsDeleteList.end(); )
				{
					itNext = itTIL;
					++itNext;
					clsDeleteList.erase( itTIL );
					itTIL = itNext;

					--iDeleteCount;
					if( iDeleteCount == 0 ) break;
				}
			}
		}

		for( itTIL = clsDeleteList.begin(); itTIL != clsDeleteList.end(); ++itTIL )
		{
			DeleteThread( *itTIL );
		}
	}
}

/**
 * @ingroup TcpStack
 * @brief ������ ������ �����Ѵ�.
 * @param iThreadIndex ������ �ε���
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpThreadList::DeleteThread( int iThreadIndex )
{
	THREAD_LIST::iterator	itTL;
	CTcpComm		clsTcpComm;
	bool bRes = false;

	SendCommand( (char *)&clsTcpComm, sizeof(clsTcpComm), iThreadIndex );

	m_clsMutex.acquire();
	for( itTL = m_clsList.begin(); itTL != m_clsList.end(); ++itTL )
	{
		if( (*itTL)->m_iIndex == iThreadIndex )
		{
			// �����忡�� �ڷᱸ���� ����ϹǷ� ������ ������ ���� �����Ѵ�.
			// delete *itTL;
			m_clsList.erase( itTL );
			bRes = true;
			break;
		}
	}

	if( bRes )
	{
		int iMaxIndex = 0;

		for( itTL = m_clsList.begin(); itTL != m_clsList.end(); ++itTL )
		{
			if( (*itTL)->m_iIndex > iMaxIndex )
			{
				iMaxIndex = (*itTL)->m_iIndex;
			}
		}

		m_iThreadIndex = iMaxIndex;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief ������ ������ �ϳ��� ���ڿ��� �����Ѵ�.
 * @param strBuf ������ ������ ������ ���ڿ� ����
 */
void CTcpThreadList::GetString( CMonitorString & strBuf )
{
	THREAD_LIST::iterator	itTL;

	strBuf.Clear();

	m_clsMutex.acquire();
	for( itTL = m_clsList.begin(); itTL != m_clsList.end(); ++itTL )
	{
		strBuf.AddRow( (*itTL)->m_clsSessionList.m_iPoolFdCount );
	}
	m_clsMutex.release();
}

/**
 * @ingroup TcpStack
 * @brief �����带 �߰��Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpThreadList::AddThread()
{
	if( m_pclsStack->m_clsSetup.m_iThreadMaxCount != 0 )
	{
		int iListCount = (int)m_clsList.size();

		if( iListCount >= m_pclsStack->m_clsSetup.m_iThreadMaxCount )
		{
			CLog::Print( LOG_ERROR, "%s thread count(%d) >= max thread count(%d)", __FUNCTION__, iListCount, m_pclsStack->m_clsSetup.m_iThreadMaxCount );
			return false;
		}
	}

	Socket	arrSocket[2];
	CTcpThreadInfo * pclsTcpThreadInfo = new CTcpThreadInfo();
	if( pclsTcpThreadInfo == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsTcpThreadInfo->m_iIndex = GetThreadIndex();

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

	bool bRes = StartThread( "TcpPipeThread", TcpPipeThread, pclsTcpThreadInfo );
	if( bRes == false )
	{
		DeleteThread( pclsTcpThreadInfo->m_iIndex );
		delete pclsTcpThreadInfo;
	}

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief �����忡 ����� �����Ѵ�.
 * @param hSocket		pipe ���� �ڵ�
 * @param pszData		���
 * @param iDataLen	pszData ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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

/**
 * @ingroup TcpStack
 * @brief ������ ������ �����Ѵ�.
 * @returns ������ ������ �����Ѵ�.
 */
int CTcpThreadList::GetCount()
{
	int iCount = 0;

	m_clsMutex.acquire();
	iCount = (int)m_clsList.size();
	m_clsMutex.release();

	return iCount;
}

/**
 * @ingroup TcpStack
 * @brief ���� ����� ������ ��ȣ�� �����´�.
 * @returns ���� ����� ������ ��ȣ�� �����Ѵ�.
 */
int CTcpThreadList::GetThreadIndex()
{
	int iThreadIndex = 0;

	++m_iThreadIndex;
	if( m_iThreadIndex > 2000000000 )
	{
		m_iThreadIndex = 1;
	}

	while( SelectThreadIndex( m_iThreadIndex ) )
	{
		++m_iThreadIndex;
		if( m_iThreadIndex > 2000000000 )
		{
			m_iThreadIndex = 1;
		}
	}

	iThreadIndex = m_iThreadIndex;

	return iThreadIndex;
}

/**
 * @ingroup TcpStack
 * @brief ������ ��ȣ�� ��������� �˻��Ѵ�.
 * @param iThreadIndex ������ ��ȣ
 * @returns ������ ��ȣ�� ������̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpThreadList::SelectThreadIndex( int iThreadIndex )
{
	THREAD_LIST::iterator itTL;

	for( itTL = m_clsList.begin(); itTL != m_clsList.end(); ++itTL )
	{
		if( (*itTL)->m_iIndex == iThreadIndex )
		{
			return true;
		}
	}

	return false;
}
