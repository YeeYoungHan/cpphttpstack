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
#include "HtmlElement.h"
#include "MemoryDebug.h"

static bool TestHtmlClass( CHtmlElement & clsHtml, int iLine, const char * pszClass, bool bResult )
{
	if( clsHtml.IsClass( pszClass ) == bResult )
	{
		return true;
	}

	printf( "%s line(%d) class(%s) %s\n", __FUNCTION__, iLine, pszClass, bResult ? "not found" : "found" );

	return false;
}

bool TestHtmlClass( const char * pszInput )
{
	CHtmlElement clsHtml;

	if( clsHtml.Parse( pszInput, (int)strlen(pszInput) ) == -1 )
	{
		printf( "%s line(%d) Parse error\n", __FUNCTION__, __LINE__ );
		return false;
	}

	if( clsHtml.IsId( "id1" ) == false )
	{
		printf( "%s line(%d) id1 is not found\n", __FUNCTION__, __LINE__ );
		return false;
	}

	if( clsHtml.IsId( "id2" ) )
	{
		printf( "%s line(%d) id2 is found\n", __FUNCTION__, __LINE__ );
		return false;
	}

	if( TestHtmlClass( clsHtml, __LINE__, "class1", true ) == false ) return false;
	if( TestHtmlClass( clsHtml, __LINE__, "class2", true ) == false ) return false;
	if( TestHtmlClass( clsHtml, __LINE__, "class3", true ) == false ) return false;
	if( TestHtmlClass( clsHtml, __LINE__, "class4", false ) == false ) return false;


	return true;
}

bool TestHtmlClass()
{
	if( TestHtmlClass( "<div id='id1' class='class1 class2 class3'></div>" ) == false ) return false;
	if( TestHtmlClass( "<div id = 'id1' class = 'class1 class2 class3'></div>" ) == false ) return false;
	if( TestHtmlClass( "<div id=' id1 ' class='\t class1 \t class2 \tclass3 '></div>" ) == false ) return false;
	if( TestHtmlClass( "<div id='\tid1\t' class='class1 class2 class3'></div>" ) == false ) return false;

	return true;
}
