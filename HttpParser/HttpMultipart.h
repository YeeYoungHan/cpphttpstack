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

#ifndef _HTTP_MULTIPART_H_
#define _HTTP_MULTIPART_H_

#include "StringUtility.h"
#include <map>

class CHttpMultipartData
{
public:
	std::string m_strContentType;
	std::string m_strFileName;
	std::string m_strValue;
};

typedef std::map< std::string, CHttpMultipartData * > HTTP_MULTIPART_DATA_MAP;

class CHttpMultipart
{
public:
	CHttpMultipart();
	~CHttpMultipart();

	void SetBoundary( const char * pszBoundary );
	void SetContentType( const char * pszContentType );
	void Clear();

	int Parse( const std::string & strText );
	int Parse( const char * pszText, int iTextLen );
	int ToString( std::string & strText );

private:
	int ParseData( const char * pszText, int iTextLen );

	std::string m_strBoundary;

public:
	HTTP_MULTIPART_DATA_MAP	m_clsMap;
};

#endif
