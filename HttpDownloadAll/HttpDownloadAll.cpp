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

#include "HttpDownloadAll.h"

bool DownloadFile( const char * pszUrl, const char * pszFileName )
{
	CHttpClient clsHttpClient;
	std::string strContentType, strBody;
	std::string strUrl = pszUrl;
	int iUrlLen = strUrl.length();
	
	if( strUrl.at( iUrlLen - 1 ) == '/' )
	{
		strUrl.append( pszFileName );
	}
	else
	{
		strUrl.append( "/" );
		strUrl.append( pszFileName );
	}

	if( clsHttpClient.DoGet( strUrl.c_str(), strContentType, strBody ) == false )
	{
		printf( "url(%s) get error\n", strUrl.c_str() );
		return false;
	}

	FILE * fd = fopen( pszFileName, "wb" );
	if( fd == NULL )
	{
		printf( "file(%s) open error\n", pszFileName );
		return false;
	}

	fwrite( strBody.c_str(), 1, strBody.length(), fd );

	fclose( fd );

	return true;
}

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {url}\n", argv[0] );
		return 0;
	}

	char * pszUrl = argv[1];
	CHttpClient clsHttpClient;
	std::string strContentType, strBody;
	CHtmlElement clsHtml;

	if( clsHttpClient.DoGet( pszUrl, strContentType, strBody ) == false )
	{
		printf( "url(%s) get error\n", pszUrl );
		return 0;
	}

	clsHtml.Parse( strBody.c_str(), strBody.length() );
	CHtmlElement * pclsHtmlBody = clsHtml.SelectElement( "body" );
	if( pclsHtmlBody == NULL )
	{
		printf( "html -> body is not found\n" );
		return 0;
	}

	CHtmlElement * pclsHtmlPre = pclsHtmlBody->SelectElement( "pre" );
	if( pclsHtmlPre == NULL )
	{
		printf( "html -> body -> pre is not found\n" );
		return 0;
	}

	HTML_ELEMENT_LIST clsAList;
	HTML_ELEMENT_LIST::iterator itAL;
	std::string strFileName;

	pclsHtmlPre->SelectElementList( "a", clsAList );

	for( itAL = clsAList.begin(); itAL != clsAList.end(); ++itAL )
	{
		if( itAL->SelectAttribute( "href", strFileName ) )
		{
			if( !strncmp( strFileName.c_str(), "..", 2 ) ) continue;
			if( strstr( strFileName.c_str(), "/" ) ) continue;

			printf( "[%s]\n", strFileName.c_str() );

			DownloadFile( pszUrl, strFileName.c_str() );
		}
	}

	return 0;
}
