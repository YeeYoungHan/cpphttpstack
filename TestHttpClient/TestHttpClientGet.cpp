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

#include "HttpClient.h"
#include "Log.h"
#include "MemoryDebug.h"

int TestHttpClientGet( int argc, char * argv[] )
{
	std::string strUrl, strBodyType, strBody;
	CHttpClient clsClient;

	if( argc >= 3 )
	{
		strUrl = argv[2];
	}
	else if( argc == 2 )
	{
		strUrl = argv[1];
	}

	CLog::SetLevel( LOG_DEBUG | LOG_NETWORK );

	if( clsClient.DoGet( strUrl.c_str(), strBodyType, strBody ) )
	{
		printf( "BodyType[%s] BodyLen[%d]\n", strBodyType.c_str(), (int)strBody.length() );
		printf( "%s", strBody.c_str() );
	}
	else
	{
		printf( "clsClient.DoGet error\n" );
	}

	return 0;
}
