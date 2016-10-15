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

#include "SimpleHttpServer.h"
#include "Directory.h"
#include "MemoryDebug.h"

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {Document root path}\n", argv[0] );
		return 0;
	}

#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	CSimpleHttpServer clsServer;
	CHttpStack clsStack;
	CTcpStackSetup clsSetup;

	clsSetup.m_iListenPort = 8080;
	clsServer.m_strDocumentRoot = argv[1];

	if( CDirectory::IsDirectory( clsServer.m_strDocumentRoot.c_str() ) != 0 )
	{
		printf( "[%s] is not directory\n", clsServer.m_strDocumentRoot.c_str() );
		return 0;
	}

	if( clsStack.Start( &clsSetup, &clsServer ) == false )
	{
		printf( "clsStack.Start error\n" );
		return 0;
	}

	while( clsServer.m_bStop == false )
	{
		sleep(1);
	}

	clsStack.Stop();

	// 모든 쓰레드가 종료될 때까지 대기한다.
	sleep(2);

	return 0;
}
