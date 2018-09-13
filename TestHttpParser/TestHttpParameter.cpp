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
#include "HttpParameterList.h"
#include <stdlib.h>
#include "MemoryDebug.h"

static bool TestHttpParameterList( const char * pszText, int iLine )
{
	CHttpParameterList clsList;
	char szText[8192];

	clsList.Parse( pszText );
	clsList.ToString( szText, sizeof(szText) );

	if( strcmp( pszText, szText ) )
	{
		printf( "[%d] input[%s] != output[%s]\n", iLine, pszText, szText );
		return false;
	}

	return true;
}

static bool TestHttpUrlParameter( const char * pszText, int iLine )
{
	CHttpParameterList clsList;
	char szText[8192];

	clsList.ParseUrl( pszText );
	clsList.ToString( szText, sizeof(szText) );

	const char * pszQuery = strstr( pszText, "?" );
	if( pszQuery )
	{
		++pszQuery;
	}

	if( strcmp( pszQuery, szText ) )
	{
		printf( "[%d] input[%s] != output[%s]\n", iLine, pszText, szText );
		return false;
	}

	return true;
}

bool TestHttpParameterList( )
{
	if( TestHttpParameterList( "n1=v1&n2=v2", __LINE__ ) == false ) return false;
	
	if( TestHttpUrlParameter( "http://www.naver.com/test?n1=v1&n2=v2", __LINE__ ) == false ) return false;

	return true;
}
