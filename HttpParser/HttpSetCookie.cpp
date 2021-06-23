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

#include "SipPlatformDefine.h"
#include "HttpSetCookie.h"
#include "HttpParameterList.h"
#include "TimeString.h"
#include <stdlib.h>

CHttpSetCookie::CHttpSetCookie() : m_iExpireTime(0), m_bSecure(false)
{
}

CHttpSetCookie::~CHttpSetCookie()
{
}

int CHttpSetCookie::Parse( const char * pszText, int iTextLen )
{
	Clear();

	CHttpParameterList clsParamList( ';', true );

	int n = clsParamList.Parse( pszText, iTextLen );
	if( n == -1 ) return -1;

	HTTP_PARAMETER_LIST::iterator itPL;
	const char * pszName;

	for( itPL = clsParamList.m_clsParamList.begin(); itPL != clsParamList.m_clsParamList.end(); ++itPL )
	{
		if( itPL == clsParamList.m_clsParamList.begin() )
		{
			m_strName = itPL->m_strName;
			m_strValue = itPL->m_strValue;
		}
		else
		{
			pszName = itPL->m_strName.c_str();

			if( !strcasecmp( pszName, "expires" ) )
			{
				if( m_iExpireTime == 0 )
				{
					m_iExpireTime = ParseCookieExpires( itPL->m_strValue.c_str() );
				}
			}
			else if( !strcasecmp( pszName, "max-age" ) )
			{
				int iMaxAge = atoi( itPL->m_strValue.c_str() );
				if( iMaxAge <= 0 )
				{
					m_iExpireTime = 0;
				}
				else
				{
					time( &m_iExpireTime );
					m_iExpireTime += iMaxAge;
				}
			}
			else if( !strcasecmp( pszName, "path" ) )
			{
				m_strPath = itPL->m_strValue;
			}
			else if( !strcasecmp( pszName, "domain" ) )
			{
				m_strDomain = itPL->m_strValue;
			}
			else if( !strcasecmp( pszName, "secure" ) )
			{
				m_bSecure = true;
			}
		}
	}

	if( m_strDomain.empty() == false )
	{
		// 도메인이 .google.com 이면 맨 앞의 . 을 제거한다.
		if( m_strDomain[0] == '.' )
		{
			m_strDomain.erase( 0, 1 );
		}
	}

	return n;
}

void CHttpSetCookie::Clear()
{
	m_strName.clear();
	m_strValue.clear();
	m_iExpireTime = 0;
	m_strDomain.clear();
	m_strPath.clear();
	m_bSecure = false;
}
