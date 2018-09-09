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

int TestHttpClientUpload( int argc, char * argv[] )
{
	std::string strSendBody, strRecvBodyType, strRecvBody;
	CHttpClient clsClient;
	
	CLog::SetLevel( LOG_DEBUG | LOG_NETWORK );

	POST_NAME_VALUE_MAP clsPostNameValueMap;

	clsPostNameValueMap.insert( POST_NAME_VALUE_MAP::value_type( "id", "userid" ) );
	clsPostNameValueMap.insert( POST_NAME_VALUE_MAP::value_type( "pw", "password" ) );

	if( clsClient.DoUpload( "http://127.0.0.1:8080/board/file_upload", "C:\\Temp\\test\\IE.LOG", "file", clsPostNameValueMap, strRecvBodyType, strRecvBody ) )
	{
		printf( "[%s]\n", strRecvBody.c_str() );
	}
	else
	{
		printf( "clsClient.DoUpload error\n" );
	}

	return 0;
}
