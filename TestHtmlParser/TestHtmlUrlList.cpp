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

/**
 * @ingroup HttpSimulator
 * @brief URL 에서 최상위 URL 과 현재 URL 의 부모 URL 를 가져온다.
 * @param strUrl				URL	(예: http://127.0.0.1/test/index.html )
 * @param strRootUrl		[out] 최상위 URL (예: http://127.0.0.1 )
 * @param strParentUrl	[out] 부모 URL (예: http://127.0.0.1/test )
 * @returns true 를 리턴한다.
 */
static bool GetRootParentUrl( std::string & strUrl, std::string & strRootUrl, std::string & strParentUrl )
{
	const char * pszUrl = strUrl.c_str();
	int iLen = (int)strUrl.length();
	int iParentUrlEnd = 0;
	char cType = 0;

	for( int i = 4; i < iLen; ++i )
	{
		if( cType == 0 )
		{
			if( pszUrl[i-2] == ':' && pszUrl[i-1] == '/' && pszUrl[i] == '/' )
			{
				cType = 1;
			}
		}
		else if( cType == 1 )
		{
			if( pszUrl[i] == '/' )
			{
				strRootUrl.append( pszUrl, i );
				cType = 2;
			}
		}
		else if( cType == 2 )
		{
			if( pszUrl[i] == '/' )
			{
				iParentUrlEnd = i;
			}
			else if( pszUrl[i] == '?' )
			{
				break;
			}
		}
	}

	if( iParentUrlEnd )
	{
		strParentUrl.append( pszUrl, iParentUrlEnd );
	}

	if( strRootUrl.empty() )
	{
		strRootUrl = strUrl;
	}

	if( strParentUrl.empty() )
	{
		strParentUrl = strUrl;
	}

	return true;
}

bool TestHtmlUrlList( const char * pszHtmlFileName )
{
	CHtmlElementUrl clsHtml;
	STRING_LIST clsUrlList;
	STRING_LIST::iterator itSL;

	if( clsHtml.ParseFile( pszHtmlFileName, E_HEO_NOT_CHECK_END_TAG ) == false )
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

	printf( "==================================================\n" );

	std::string strRootUrl, strParentUrl, strNewUrl;
	std::string strUrl = "http://127.0.0.1:8080/test/index.html";

	GetRootParentUrl( strUrl, strRootUrl, strParentUrl );

	printf( "root url(%s) parent url(%s)\n", strRootUrl.c_str(), strParentUrl.c_str() );

	for( itSL = clsUrlList.begin(); itSL != clsUrlList.end(); ++itSL )
	{
		if( (*itSL)[0] == 'h' )
		{
			continue;
		}
		else if( (*itSL)[0] == '/' )
		{
			strNewUrl = strRootUrl + *itSL;
		}
		else
		{
			strNewUrl = strParentUrl + *itSL;
		}

		*itSL = strNewUrl;
	}

	for( itSL = clsUrlList.begin(); itSL != clsUrlList.end(); ++itSL )
	{
		printf( "%s\n", itSL->c_str() );
	}

	return true;
}
