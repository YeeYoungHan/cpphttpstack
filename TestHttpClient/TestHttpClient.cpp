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
#include "SipTcp.h"
#include "Log.h"
#include "MemoryDebug.h"

int main( int argc, char * argv[] )
{
	std::string strUrl, strBodyType, strBody;
	CHttpClient clsClient;

#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	if( argc >= 2 )
	{
		strUrl = argv[1];
	}

	InitNetwork();

	CLog::SetLevel( LOG_DEBUG | LOG_NETWORK );

	if( clsClient.DoGet( strUrl.c_str(), strBodyType, strBody ) )
	{
		printf( "BodyType[%s] BodyLen[%d]\n", strBodyType.c_str(), strBody.length() );
		printf( "%s", strBody.c_str() );
	}
	else
	{
		printf( "clsClient.DoGet error\n" );
	}

	return 0;
}
