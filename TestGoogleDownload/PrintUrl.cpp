#include "TestGoogleDownload.h"

static FILE * gsttFd = NULL;

bool ParseBody( const char * pszExt, std::string & strBody )
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

			if( gsttFd == NULL )
			{
				gsttFd = fopen( URL_PRINT_FILEPATH, "a" );
				if( gsttFd == NULL )
				{
					printf( "file(%s) open error\n", URL_PRINT_FILEPATH );
					gbStop = true;
					return false;
				}
			}

			fprintf( gsttFd, "%s\n", strUrl.c_str() );
			printf( "%s\n", strUrl.c_str() );
		}

		pszBody = pszEnd + 2;
	}

	return true;
}

bool PrintUrl( const char * pszExt, const char * pszSearch )
{
	std::string strSearch, strUtf8, strContentType, strBody;
	char szPage[11], szHex[3];
	CHttpClient clsClient;
	CStringMap clsUrlMap;

	clsClient.SetUserAgent( "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36" );

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
			if( clsClient.GetStatusCode() == 503 )
			{
				gbStop = true;
			}
			break;
		}

		if( ParseBody( pszExt, strBody ) == false )
		{
			break;
		}

		Sleep(60000);
	}

	return true;
}
