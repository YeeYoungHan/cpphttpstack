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
	USER_MAP::iterator itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap == m_clsMap.end() )
	{
		CUserInfo clsInfo;

		clsInfo.m_strIp = pszIp;
		clsInfo.m_iPort = iPort;

		m_clsMap.insert( USER_MAP::value_type( pszUserId, clsInfo ) );

		bRes = true;
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

bool CUserMap::Delete( const char * pszIp, int iPort )
{
	bool bRes = false;
	USER_MAP::iterator itMap, itNext;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( !strcmp( itMap->second.m_strIp.c_str(), pszIp ) && itMap->second.m_iPort == iPort )
		{
			itNext = itMap;
			++itNext;
			m_clsMap.erase( itMap );

			if( itNext == m_clsMap.end() ) break;
			itMap = itNext;
			goto LOOP_START;
		}
	}
	m_clsMutex.release();

	return bRes;
}
