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

static bool TestHtmlAttribute( CHtmlElement & clsHtml, int iLine, const char * pszName, const char * pszValue )
{
	std::string strValue;

	if( clsHtml.SelectAttribute( pszName, strValue ) == false )
	{
		printf( "%s line(%d) name(%s) not found\n", __FUNCTION__, iLine, pszName );
		return false;
	}

	if( strcmp( strValue.c_str(), pszValue ) )
	{
		printf( "%s line(%d) name(%s) value(%s) != want(%s)\n", __FUNCTION__, iLine, pszName, strValue.c_str(), pszValue );
		return false;
	}

	return true;
}

bool TestHtmlAttribute( )
{
	const char * pszHtml = "<abbr id=\"anId\" class='aClass' style=\"color:blue;\" title='Hypertext \"Markup\" Language'>HTML</abbr>";
	CHtmlElement clsHtml;

	if( clsHtml.Parse( pszHtml, (int)strlen(pszHtml) ) == -1 )
	{
		printf( "%s line(%d) Parse error - input(%s)\n", __FUNCTION__, __LINE__, pszHtml );
		return false;
	}

	if( TestHtmlAttribute( clsHtml, __LINE__, "id", "anId" ) == false ) return false;
	if( TestHtmlAttribute( clsHtml, __LINE__, "class", "aClass" ) == false ) return false;
	if( TestHtmlAttribute( clsHtml, __LINE__, "style", "color:blue;" ) == false ) return false;
	if( TestHtmlAttribute( clsHtml, __LINE__, "title", "Hypertext \"Markup\" Language" ) == false ) return false;

	return true;
}
