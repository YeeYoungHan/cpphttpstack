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
#include "UserMap.h"

CUserMap gclsUserMap;

CUserMap::CUserMap()
{
}

CUserMap::~CUserMap()
{
}

bool CUserMap::Insert( const char * pszUserId, const char * pszIp, int iPort )
{
	bool bRes = false;
	std::string strKey;
	USER_MAP::iterator itMap;
	USER_KEY_MAP::iterator itKeyMap;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itKeyMap = m_clsKeyMap.find( strKey );
	if( itKeyMap == m_clsKeyMap.end() )
	{
		itMap = m_clsMap.find( pszUserId );
		if( itMap == m_clsMap.end() )
		{
			CUserInfo clsInfo;

			clsInfo.m_strIp = pszIp;
			clsInfo.m_iPort = iPort;

			m_clsMap.insert( USER_MAP::value_type( pszUserId, clsInfo ) );
			m_clsKeyMap.insert( USER_KEY_MAP::value_type( strKey, pszUserId ) );

			bRes = true;
		}
	}
	m_clsMutex.release();

	return bRes;
}

bool CUserMap::Select( const char * pszUserId, CUserInfo & clsUserInfo )
{
	bool bRes = false;
	USER_MAP::iterator itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap != m_clsMap.end() )
	{
		clsUserInfo = itMap->second;

		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CUserMap::SelectUserId( const char * pszIp, int iPort, std::string & strUserId )
{
	bool bRes = false;
	std::string strKey;
	USER_KEY_MAP::iterator itKeyMap;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itKeyMap = m_clsKeyMap.find( strKey );
	if( itKeyMap != m_clsKeyMap.end() )
	{
		strUserId = itKeyMap->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CUserMap::Delete( const char * pszIp, int iPort )
{
	bool bRes = false;
	std::string strKey;
	USER_MAP::iterator itMap;
	USER_KEY_MAP::iterator itKeyMap;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itKeyMap = m_clsKeyMap.find( strKey );
	if( itKeyMap != m_clsKeyMap.end() )
	{
		itMap = m_clsMap.find( itKeyMap->second );
		if( itMap != m_clsMap.end() )
		{
			m_clsMap.erase( itMap );
		}

		m_clsKeyMap.erase( itKeyMap );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

void CUserMap::GetKey( const char * pszIp, int iPort, std::string & strKey )
{
	char szPort[11];

	snprintf( szPort, sizeof(szPort), "%d", iPort );

	strKey = pszIp;
	strKey.append( "_" );
	strKey.append( szPort );
}
