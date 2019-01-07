#include "TestGoogleDownload.h"

bool GetFileName( std::string & strUrl, std::string & strFileName )
{
	strFileName.clear();

	int iLen = (int)strUrl.length();

	for( int i = iLen - 1; i >= 0; --i )
	{
		if( strUrl.at(i) == '/' )
		{
			strFileName = strUrl.c_str() + i + 1;
			break;
		}
	}

	if( strFileName.empty() ) return false;

	iLen = (int)strFileName.length();
	const char * pszFileName = strFileName.c_str();
	std::string strTemp, strUtf8;
	bool bPercent = false;

	for( int i = 0; i < iLen; ++i )
	{
		if( pszFileName[i] == '%' ) 
		{
			bPercent = true;
			continue;
		}

		if( bPercent && ( ( pszFileName[i] >= '0' && pszFileName[i] <= '9' ) || ( pszFileName[i] >= 'A' && pszFileName[i] <= 'F' ) ) )
		{
			uint8_t arrChar[10];

			sscanf( pszFileName + i, "%02X", &arrChar[0] );
			++i;

			if( arrChar[0] == '%' ) continue;
			strUtf8.append( (char *)arrChar, 1 );
			bPercent = false;

			if( strUtf8.length() == 3 )
			{
				std::string strChar;
				Utf8ToAnsi( strUtf8.c_str(), strChar );
				if( strChar.at(0) == '?' || strChar.at(1) == '?' ) return true;
				strTemp.append( strChar );
				strUtf8.clear();
			}
		}
		else
		{
			strTemp.append( strUtf8 );
			strUtf8.clear();

			strTemp.push_back( pszFileName[i] );
		}
	}

	strFileName = strTemp;

	return true;
}
