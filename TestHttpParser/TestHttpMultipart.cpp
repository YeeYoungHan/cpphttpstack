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
#include "HttpMultipart.h"
#include "MemoryDebug.h"

bool TestHttpMultipart( )
{
	const char * pszBoundary = "---------------------------7e21152f00c8";
	const char * pszBody = "-----------------------------7e21152f00c8\r\n"
		"Content-Disposition: form-data; name=\"id\"\r\n"
		"\r\n"
		"id1\r\n"
		"-----------------------------7e21152f00c8\r\n"
		"Content-Disposition: form-data; name=\"pw\"\r\n"
		"\r\n"
		"pw1\r\n"
		"-----------------------------7e21152f00c8\r\n"
		"Content-Disposition: form-data; name=\"zfile\"; filename=\"1234.txt\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"1234\r\n"
		"-----------------------------7e21152f00c8--\r\n";
	CHttpMultipart clsMultipart;
	std::string strText;

	clsMultipart.SetBoundary( pszBoundary );
	if( clsMultipart.Parse( pszBody, (int)strlen(pszBody) ) == -1 )
	{
		printf( "clsMultipart.Parse error\n" );
		return false;
	}

	clsMultipart.ToString( strText );
	if( strcmp( strText.c_str(), pszBody ) )
	{
		printf( "clsMultipart Parse or ToString error\n" );
		return false;
	}

	return true;
}
