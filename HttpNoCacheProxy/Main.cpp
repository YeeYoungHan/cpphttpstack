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

#include "HttpNoCacheProxy.h"
#include "Directory.h"
#include "Log.h"
#include "MemoryDebug.h"

CHttpStack gclsStack;

int main( int argc, char * argv[] )
{
	if( argc == 1 )
	{
		printf( "[Usage] %s {HTTP Server IP} {HTTP Server port} {Listen Port}\n", argv[0] );
		return 0;
	}

#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	int iLocalPort = 8080;

	if( argc >= 4 )
	{
		iLocalPort = atoi( argv[3] );
	}

	CHttpNoCacheProxy clsServer;
	CTcpStackSetup clsSetup;

	CLog::SetLevel( LOG_DEBUG );

	// HTTP 수신 포트 번호를 설정한다.
	clsSetup.m_iListenPort = iLocalPort;
	clsSetup.m_iMaxSocketPerThread = 1;
	clsSetup.m_iThreadMaxCount = 0;
	clsSetup.m_bUseThreadPipe = false;
	
	/*
	//thread pool 을 사용할 경우
	clsSetup.m_iMaxSocketPerThread = 10;
	clsSetup.m_iThreadMaxCount = 10;
	clsSetup.m_bUseThreadPipe = true;
	*/

	// HTTP 서버에서 사용할 Document root 폴더를 설정한다.
	clsServer.m_strServerIp = argv[1];

	if( argc >= 3 )
	{
		clsServer.m_iServerPort = atoi( argv[2] );
	}

	// HTTP 서버를 시작한다. HTTP 요청이 수신되면 이에 대한 이벤트를 CHttpNoCacheProxy 객체로 전달한다.
	if( gclsStack.Start( &clsSetup, &clsServer ) == false )
	{
		printf( "clsStack.Start error\n" );
		return 0;
	}

	while( clsServer.m_bStop == false )
	{
		sleep(1);
	}

	gclsStack.Stop();

	// 모든 쓰레드가 종료될 때까지 대기한다.
	sleep(2);

#ifdef WIN32
	CHttpStack::Release();
	CLog::Release();
#endif

	return 0;
}
