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

#ifndef _HTTP_DOMAIN_COOKIE_H_
#define _HTTP_DOMAIN_COOKIE_H_

#include "HttpSetCookie.h"
#include <list>
#include <map>

class CHttpDomainCookiePath
{
public:
	std::string m_strPath;
	std::string m_strName;
	std::string m_strValue;
	time_t			m_iExpireTime;
	bool				m_bSecure;
};

typedef std::list< CHttpDomainCookiePath > HTTP_DOMAIN_COOKIE_PATH_LIST;

class CHttpDomainCookieInfo
{
public:
	bool Insert( CHttpSetCookie * pclsSetCookie );
	bool Select( const char * pszPath, std::string & strCookie );

	HTTP_DOMAIN_COOKIE_PATH_LIST m_clsList;
};

// key = domain or ip
typedef std::map< std::string, CHttpDomainCookieInfo > HTTP_DOMAIN_COOKIE_MAP;

class CHttpDomainCookie
{
public:
	bool Insert( CHttpSetCookie * pclsSetCookie );
	bool Select( const char * pszDomain, const char * pszPath, std::string & strCookie );
	bool IsEmpty( );

private:
	HTTP_DOMAIN_COOKIE_MAP m_clsMap;
};

#endif
