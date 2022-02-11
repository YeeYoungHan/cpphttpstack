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

static bool TestHtmlGetText( int iLine, const char * pszInput, const char * pszOutput )
{
	CHtmlElement clsHtml;
	std::string strOutput;

	if( clsHtml.Parse( pszInput, (int)strlen(pszInput) ) == -1 )
	{
		printf( "%s line(%d) Parse error - input(%s)\n", __FUNCTION__, iLine, pszInput );
		return false;
	}

	clsHtml.GetText( strOutput );

	if( strcmp( pszOutput, strOutput.c_str() ) )
	{
		printf( "%s line(%d) Parse error - input(%s) output(%s) != result(%s)\n", __FUNCTION__, iLine, pszInput, pszOutput, strOutput.c_str() );
		return false;
	}

	return true;
}

static bool Allow( CHtmlElement * pclsElement )
{
	const char * pszName = pclsElement->GetName();

	if( !strcmp( pszName, "p" ) ) return true;

	return false;
}

static bool TestHtmlGetTextAllow( int iLine, const char * pszInput, const char * pszOutput )
{
	CHtmlElement clsHtml;
	std::string strOutput;

	if( clsHtml.Parse( pszInput, (int)strlen(pszInput) ) == -1 )
	{
		printf( "%s line(%d) Parse error - input(%s)\n", __FUNCTION__, iLine, pszInput );
		return false;
	}

	clsHtml.GetText( strOutput, Allow );

	if( strcmp( pszOutput, strOutput.c_str() ) )
	{
		printf( "%s line(%d) Parse error - input(%s) output(%s) != result(%s)\n", __FUNCTION__, iLine, pszInput, pszOutput, strOutput.c_str() );
		return false;
	}

	return true;
}

bool TestHtmlGetText()
{
	if( TestHtmlGetText( __LINE__, "<html><body>1234</body></html>", "1234" ) == false ) return false;
	if( TestHtmlGetText( __LINE__, "<html><body><div>1234</div><div>5678</div></body></html>", "1234 5678" ) == false ) return false;
	if( TestHtmlGetText( __LINE__, "<html><body><div>1234<div>abcd</div></div><div>5678</div></body></html>", "1234 abcd 5678" ) == false ) return false;

	if( TestHtmlGetTextAllow( __LINE__, "<html><body><div>1234<p>abcd</p></div><div>5678</div></body></html>", "abcd" ) == false ) return false;
	if( TestHtmlGetTextAllow( __LINE__, "<html><body><div>1234<p>abcd</p></div><div>5678</div><p>QUIT</p></body></html>", "abcd QUIT" ) == false ) return false;

	return true;
}
