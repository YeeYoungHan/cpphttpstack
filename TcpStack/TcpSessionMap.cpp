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
#include "TcpSessionMap.h"
#include "TcpStackCallBack.h"
#include "MemoryDebug.h"

CTcpSessionMap::CTcpSessionMap() : m_iThreadIndex(0)
{
}

CTcpSessionMap::~CTcpSessionMap()
{
}

/**
 * @ingroup TcpStack
 * @brief TCP ���� ������ �����Ѵ�.
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @param iPort		Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @param pclsSessionInfo TCP ���� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSessionMap::Insert( const char * pszIp, int iPort, CTcpSessionInfo * pclsSessionInfo )
{
	TCP_SESSION_MAP::iterator itTSM;
	std::string strKey;
	bool bRes = false;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itTSM = m_clsMap.find( strKey );
	if( itTSM == m_clsMap.end() )
	{
		pclsSessionInfo->m_iThreadIndex = GetThreadIndex();
		m_clsMap.insert( TCP_SESSION_MAP::value_type( strKey, pclsSessionInfo ) );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief TCP ���� ������ �����Ѵ�.
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @param iPort		Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSessionMap::Delete( const char * pszIp, int iPort )
{
	TCP_SESSION_MAP::iterator itTSM;
	std::string strKey;
	bool bRes = false;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itTSM = m_clsMap.find( strKey );
	if( itTSM != m_clsMap.end() )
	{
		// // thread �� ���� ������ �����Ͱ� second �̱� ������ delete �ϸ� �� �ȴ�.
		// delete itTSM->second;
		m_clsMap.erase( itTSM );

		int iMaxIndex = 0;

		for( itTSM = m_clsMap.begin(); itTSM != m_clsMap.end(); ++itTSM )
		{
			if( itTSM->second->m_iThreadIndex > iMaxIndex )
			{
				iMaxIndex = itTSM->second->m_iThreadIndex;
			}
		}

		m_iThreadIndex = iMaxIndex;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief ���� ������ �����Ѵ�.
 * @returns ���� ������ �����Ѵ�.
 */
int CTcpSessionMap::GetCount( )
{
	int iCount;

	m_clsMutex.acquire();
	iCount = (int)m_clsMap.size();
	m_clsMutex.release();

	return iCount;
}

/**
 * @ingroup TcpStack
 * @brief Ư�� ���ǿ� TCP ��Ŷ�� �����Ѵ�.
 * @param pszIp				IP �ּ�
 * @param iPort				��Ʈ ��ȣ
 * @param pszPacket		��Ŷ
 * @param iPacketLen	��Ŷ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSessionMap::Send( const char * pszIp, int iPort, const char * pszPacket, int iPacketLen )
{
	TCP_SESSION_MAP::iterator itTSM;
	std::string strKey;
	bool bRes = false;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itTSM = m_clsMap.find( strKey );
	if( itTSM != m_clsMap.end() )
	{
		bRes = itTSM->second->Send( pszPacket, iPacketLen );
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief Ư�� ���ǿ� TCP ��Ŷ�� �����Ѵ�.
 * @param iThreadIndex	TCP ������ ��ȣ
 * @param pszPacket			��Ŷ
 * @param iPacketLen		��Ŷ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSessionMap::Send( int iThreadIndex, const char * pszPacket, int iPacketLen )
{
	TCP_SESSION_MAP::iterator itTSM;
	bool bRes = false;

	m_clsMutex.acquire();
	for( itTSM = m_clsMap.begin(); itTSM != m_clsMap.end(); ++itTSM )
	{
		if( itTSM->second->m_iThreadIndex == iThreadIndex )
		{
			bRes = itTSM->second->Send( pszPacket, iPacketLen );
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
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSessionMap::SendAll( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack )
{
	TCP_SESSION_MAP::iterator itTSM;

	m_clsMutex.acquire();
	for( itTSM = m_clsMap.begin(); itTSM != m_clsMap.end(); ++itTSM )
	{
		if( pclsCallBack->IsSendAll( itTSM->second ) )
		{
			itTSM->second->Send( pszPacket, iPacketLen );
			pclsCallBack->AfterSendAllPerSession( itTSM->second, pszPacket, iPacketLen );
		}
	}
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup TcpStack
 * @brief Ư�� ������ ������ ��� ���ǿ� TCP ��Ŷ�� �����Ѵ�.
 * @param pszPacket			��Ŷ
 * @param iPacketLen		��Ŷ ����
 * @param pclsCallBack	���Ǻ��� ���� ������ �����ϴ� callback ��ü
 * @param iThreadIndex	�������� ���� ������ ������ �ε���
 * @param iSessionIndex �������� ���� ���� �ε���
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSessionMap::SendAllExcept( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack, int iThreadIndex, int iSessionIndex )
{
	TCP_SESSION_MAP::iterator itTSM;

	m_clsMutex.acquire();
	for( itTSM = m_clsMap.begin(); itTSM != m_clsMap.end(); ++itTSM )
	{
		if( pclsCallBack->IsSendAll( itTSM->second ) )
		{
			if( itTSM->second->m_iThreadIndex == iThreadIndex && itTSM->second->m_iSessionIndex == iSessionIndex ) continue;

			itTSM->second->Send( pszPacket, iPacketLen );
			pclsCallBack->AfterSendAllPerSession( itTSM->second, pszPacket, iPacketLen );
		}
	}
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup TcpStack
 * @brief �ڷᱸ���� Ű�� �����Ѵ�.
 * @param pszIp TCP ���� IP �ּ�
 * @param iPort TCP ���� ��Ʈ ��ȣ
 * @param strKey �ڷᱸ���� Ű�� ������ ����
 */
void CTcpSessionMap::GetKey( const char * pszIp, int iPort, std::string & strKey )
{
	char szPort[6];

	snprintf( szPort, sizeof(szPort), "%d", iPort );

	strKey = pszIp;
	strKey.append( ":" );
	strKey.append( szPort );
}

/**
 * @ingroup TcpStack
 * @brief ���� ����� ������ ��ȣ�� �����´�.
 * @returns ���� ����� ������ ��ȣ�� �����Ѵ�.
 */
int CTcpSessionMap::GetThreadIndex()
{
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

	return m_iThreadIndex;
}

/**
 * @ingroup TcpStack
 * @brief ������ ��ȣ�� ��������� �˻��Ѵ�.
 * @param iThreadIndex ������ ��ȣ
 * @returns ������ ��ȣ�� ������̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpSessionMap::SelectThreadIndex( int iThreadIndex )
{
	TCP_SESSION_MAP::iterator itTSM;

	for( itTSM = m_clsMap.begin(); itTSM != m_clsMap.end(); ++itTSM )
	{
		if( itTSM->second->m_iThreadIndex == iThreadIndex )
		{
			return true;
		}
	}

	return false;
}
