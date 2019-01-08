#include "TestGoogleDownload.h"

bool GetFileName( std::string & strUrl, std::string & strFileName )
{
	strFileName.clear();

	std::string strDecode;

	DecodeUri( strUrl, strDecode );

	int iLen = (int)strDecode.length();

	for( int i = iLen - 1; i >= 0; --i )
	{
		if( strDecode.at(i) == '/' )
		{
			strFileName = strDecode.c_str() + i + 1;
			break;
		}
	}

	if( strFileName.empty() ) return false;

	ReplaceString( strFileName, "\\", "" );
	ReplaceString( strFileName, "/", "" );
	ReplaceString( strFileName, ":", "" );
	ReplaceString( strFileName, "*", "" );
	ReplaceString( strFileName, "?", "" );
	ReplaceString( strFileName, "\"", "" );
	ReplaceString( strFileName, "<", "" );
	ReplaceString( strFileName, ">", "" );
	ReplaceString( strFileName, "|", "" );

	std::string strAnsi;

	Utf8ToAnsi( strFileName.c_str(), strAnsi );

	if( strstr( strAnsi.c_str(), "?" ) == NULL )
	{
		strFileName = strAnsi;
	}

	return true;
}
