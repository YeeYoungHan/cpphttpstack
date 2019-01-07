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

#include "TestGoogleDownload.h"

bool ParseBody( const char * pszExt, std::string & strBody, CStringMap & clsUrlMap )
{
	const char * pszBody = strBody.c_str();
	std::string strExt = ".";
	strExt.append( pszExt );

	while( pszBody )
	{
		const char * pszA = strstr( pszBody, "<a href=\"/url?q=" );
		if( pszA == NULL ) break;

		pszA += 16;

		const char * pszEnd = strstr( pszA, "\">" );
		if( pszEnd == NULL ) break;

		int iLen = (int)(pszEnd - pszA );

		std::string strUrl;

		strUrl.append( pszA, iLen );

		const char * pszPos = strstr( strUrl.c_str(), strExt.c_str() );
		if( pszPos )
		{
			iLen = (int)(pszPos - strUrl.c_str()) + strExt.length();
			strUrl.erase( iLen );
			clsUrlMap.Insert( strUrl.c_str(), "0" );

			printf( "file url(%s)\n", strUrl.c_str() );
		}

		pszBody = pszEnd + 2;
	}

	return true;
}


bool DownloadFile( const char * pszFolder, CStringMap & clsUrlMap )
{
	STRING_MAP clsMap;
	STRING_MAP::iterator itMap;
	std::string strFileName;

	if( clsUrlMap.SelectWithValue( "0", clsMap ) == false ) return false;

	for( itMap = clsMap.begin(); itMap != clsMap.end(); ++itMap )
	{
		clsUrlMap.Update( itMap->first.c_str(), "1" );

		std::string strUrl = itMap->first;

		if( GetFileName( strUrl, strFileName ) == false ) continue;

		std::string strFilePath = pszFolder;
		CDirectory::AppendName( strFilePath, strFileName.c_str() );

		if( IsExistFile( strFilePath.c_str() ) ) continue;

#ifndef _DEBUG
		CHttpClient clsClient;
		std::string strContentType, strBody;

		if( clsClient.DoGet( strUrl.c_str(), strContentType, strBody ) )
		{
			FILE * fd = fopen( strFilePath.c_str(), "wb" );
			if( fd )
			{
				fwrite( strBody.c_str(), 1, strBody.length(), fd );
				fclose( fd );

				printf( "[%s] saved\n", strFilePath.c_str() );
			}
		}
#endif
	}

	return true;
}

bool Execute( const char * pszExt, const char * pszSearch, const char * pszFolder )
{
	std::string strSearch, strUtf8, strContentType, strBody;
	char szPage[11], szHex[3];
	CHttpClient clsClient;
	CStringMap clsUrlMap;

	CDirectory::Create( pszFolder );

	if( AnsiToUtf8( pszSearch, strUtf8 ) == false )
	{
		printf( "AnsiToUtf8(%s) error\n", pszSearch );
		return false;
	}

	int iLen = (int)strUtf8.length();

	for( int i = 0; i < iLen; ++i )
	{
		snprintf( szHex, sizeof(szHex), "%02X", (uint8_t)strUtf8.at(i) );
		strSearch.append( "%" );
		strSearch.append( szHex );
	}

	for( int iPage = 0; iPage <= 200 ; iPage += 10 )
	{
		snprintf( szPage, sizeof(szPage), "%d", iPage );

		// https://www.google.com/search?q=%EA%B0%80+filetype:pdf&start=0
		std::string strUrl = "https://www.google.com/search?q=";
		strUrl.append( strSearch );
		strUrl.append( "+filetype:" );
		strUrl.append( pszExt );
		strUrl.append( "&start=" );
		strUrl.append( szPage );

		printf( "url(%s)\n", strUrl.c_str() );
		
		if( clsClient.DoGet( strUrl.c_str(), strContentType, strBody ) == false )
		{
			printf( "code(%d)\n", clsClient.GetStatusCode() );
			break;
		}

		if( ParseBody( pszExt, strBody, clsUrlMap ) == false )
		{
			break;
		}

		DownloadFile( pszFolder, clsUrlMap );
	}

	return true;
}

char * garrSearch[] = { "°¡", "³ª", "´Ù", "¶ó", "¸¶", "¹Ù", "»ç", "¾Æ", "ÀÚ", "Â÷", "Ä«", "ÆÄ", "ÇÏ",
	"°¼", "³Ä", "´ô", "·ª", "¸Ï", "¹ò", "»þ", "¾ß", "Àð", "Ã­", "Ä¼", "ÆÙ", "Çá",
	"°Å", "³Ê", "´õ", "·¯", "¸Ó", "¹ö", "¼­", "¾î", "Àú", "Ã³", "Ä¿", "ÆÛ", "Çã",
	"°Ü", "³à", "µ®", "·Á", "¸ç", "º­", "¼Å", "¿©", "Á®", "ÃÄ", "ÄÑ", "Æì", "Çô",
	"°í", "³ë", "µµ", "·Î", "¸ð", "º¸", "¼Ò", "¿À", "Á¶", "ÃÊ", "ÄÚ", "Æ÷", "È£",
	"±³", "´¢", "µÍ", "·á", "¹¦", "ºÌ", "¼î", "¿ä", "ÁÒ", "ÃÝ", "Äì", "Ç¥", "È¿",
	"±×", "´À", "µå", "¸£", "¹Ç", "ºê", "½º", "À¸", "Áî", "Ã÷", "Å©", "ÇÁ", "Èå",
	"±â", "´Ï", "µð", "¸®", "¹Ì", "ºñ", "½Ã", "ÀÌ", "Áö", "Ä¡", "Å°", "ÇÇ", "È÷",
	NULL };

int main( int argc, char * argv[] )
{
	if( argc != 4 )
	{
		printf( "[Usage] %s {file ext} {search text} {download folder}\n", argv[0] );
		return 0;
	}

	const char * pszExt = argv[1];
	const char * pszSearch = argv[2];
	const char * pszFolder = argv[3];

	if( !strcmp( pszSearch, "ALL" ) )
	{
		for( int i = 0; garrSearch[i]; ++i )
		{
			Execute( pszExt, garrSearch[i], pszFolder );
		}
	}
	else
	{
		Execute( pszExt, pszSearch, pszFolder );
	}

	return 0;
}
