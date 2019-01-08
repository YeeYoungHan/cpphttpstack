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

bool gbStop = false;

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
	if( argc < 3 )
	{
		printf( "[Usage] %s url {ext} {search text} {index}\n", argv[0] );
		printf( "[Usage] %s download {download folder}\n", argv[0] );
		return 0;
	}

	const char * pszCommand = argv[1];

	if( !strcmp( pszCommand, "url" ) )
	{
		if( argc < 5 )
		{
			printf( "argc < 5\n" );
			return 0;
		}

		const char * pszExt = argv[2];
		const char * pszSearch = argv[3];
		const char * pszIndex = argv[4];

		int iIndex = atoi( pszIndex );
		if( !strcmp( pszSearch, "ALL" ) )
		{
			for( int i = iIndex; garrSearch[i]; ++i )
			{
				printf( "index(%d)\n", iIndex );
				PrintUrl( pszExt, garrSearch[i] );
				if( gbStop ) break;
			}
		}
		else
		{
			PrintUrl( pszExt, pszSearch );
		}
	}
	else if( !strcmp( pszCommand, "download" ) )
	{

	}

	return 0;
}
