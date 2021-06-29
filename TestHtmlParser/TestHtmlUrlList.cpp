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

#include "HtmlElementUrl.h"

bool TestHtmlUrlList( const char * pszHtmlFileName )
{
	CHtmlElementUrl clsHtml;
	STRING_LIST clsUrlList;
	STRING_LIST::iterator itSL;

	if( clsHtml.ParseFile( pszHtmlFileName ) == false )
	{
		printf( "clsHtml.ParseFile(%s) error\n", pszHtmlFileName );
		return false;
	}

	if( clsHtml.GetUrlList( clsUrlList ) == false )
	{
		printf( "clsHtml.GetUrlList error\n" );
		return false;
	}

	for( itSL = clsUrlList.begin(); itSL != clsUrlList.end(); ++itSL )
	{
		printf( "%s\n", itSL->c_str() );
	}

	return true;
}
