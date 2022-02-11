/* 
 * Copyright (C) 2021 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
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

#include "HttpDomainCookie.h"
#include "TimeUtility.h"

bool CHttpDomainCookieInfo::Insert( CHttpSetCookie * pclsSetCookie )
{
	HTTP_DOMAIN_COOKIE_PATH_LIST::iterator itPL;
	const char * pszPath = pclsSetCookie->m_strPath.c_str();
	const char * pszName = pclsSetCookie->m_strName.c_str();
	bool bFound = false;

	for( itPL = m_clsList.begin(); itPL != m_clsList.end(); ++itPL )
	{
		if( !strcmp( pszPath, itPL->m_strPath.c_str() ) && !strcmp( pszName, itPL->m_strName.c_str() ) )
		{
			bFound = true;

			time_t iTime;

			time( &iTime );

			if( pclsSetCookie->m_iExpireTime < iTime )
			{
				m_clsList.erase( itPL );
			}
			else
			{
				itPL->m_strValue = pclsSetCookie->m_strValue;
				itPL->m_iExpireTime = pclsSetCookie->m_iExpireTime;
				itPL->m_bSecure = pclsSetCookie->m_bSecure;
			}

			break;
		}
	}

	if( bFound == false )
	{
		CHttpDomainCookiePath clsPath;

		clsPath.m_strPath = pclsSetCookie->m_strPath;
		clsPath.m_strName = pclsSetCookie->m_strName;
		clsPath.m_strValue = pclsSetCookie->m_strValue;
		clsPath.m_iExpireTime = pclsSetCookie->m_iExpireTime;
		clsPath.m_bSecure = pclsSetCookie->m_bSecure;

		m_clsList.push_back( clsPath );
	}

	return true;
}

bool CHttpDomainCookieInfo::Select( const char * pszPath, std::string & strCookie )
{
	HTTP_DOMAIN_COOKIE_PATH_LIST::iterator itPL;
	int iPathLen = (int)strlen( pszPath );
	int iLen;

	strCookie.clear();

	for( itPL = m_clsList.begin(); itPL != m_clsList.end(); ++itPL )
	{
		iLen = (int)itPL->m_strPath.length();

		if( ( iPathLen >= iLen ) && !strncmp( pszPath, itPL->m_strPath.c_str(), iLen ) )
		{
			if( strCookie.empty() == false ) strCookie.append( "; " );

			strCookie.append( itPL->m_strName );
			strCookie.append( "=" );
			strCookie.append( itPL->m_strValue );
		}
	}

	if( strCookie.empty() ) return false;

	return true;
}

bool CHttpDomainCookie::Insert( CHttpSetCookie * pclsSetCookie )
{
	HTTP_DOMAIN_COOKIE_MAP::iterator itMap;

	itMap = m_clsMap.find( pclsSetCookie->m_strDomain );
	if( itMap == m_clsMap.end() )
	{
		CHttpDomainCookieInfo clsInfo;

		m_clsMap.insert( HTTP_DOMAIN_COOKIE_MAP::value_type( pclsSetCookie->m_strDomain, clsInfo ) );
		itMap = m_clsMap.find( pclsSetCookie->m_strDomain );
		if( itMap == m_clsMap.end() )
		{
			return false;
		}
	}

	return itMap->second.Insert( pclsSetCookie );
}

bool CHttpDomainCookie::Select( const char * pszDomain, const char * pszPath, std::string & strCookie )
{
	HTTP_DOMAIN_COOKIE_MAP::iterator itMap;

	itMap = m_clsMap.find( pszDomain );
	if( itMap == m_clsMap.end() )
	{
		return false;
	}

	return itMap->second.Select( pszPath, strCookie );
}

bool CHttpDomainCookie::IsEmpty( )
{
	return m_clsMap.empty();
}
