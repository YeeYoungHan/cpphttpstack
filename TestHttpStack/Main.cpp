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
#include "Log.h"
#include "MemoryDebug.h"

CHttpStack gclsStack;

int main( int argc, char * argv[] )
{
	if( argc < 2 )
	{
		printf( "[Usage] %s {Document root path} {cert pem file}\n", argv[0] );
		return 0;
	}

#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	CSimpleHttpServer clsServer;
	CTcpStackSetup clsSetup;

#ifdef WIN32
	CLog::SetDirectory( "c:\\temp\\http" );
#ifdef _DEBUG
	CLog::SetLevel( LOG_INFO | LOG_DEBUG | LOG_NETWORK | LOG_HTTP2 );
#endif
#endif

	// HTTP ���� ��Ʈ ��ȣ�� �����Ѵ�.
	clsSetup.m_iListenPort = 8080;
	clsSetup.m_iMaxSocketPerThread = 1;
	clsSetup.m_iThreadMaxCount = 0;
	clsSetup.m_bUseThreadPipe = false;
	
	/*
	//thread pool �� ����� ���
	clsSetup.m_iMaxSocketPerThread = 10;
	clsSetup.m_iThreadMaxCount = 10;
	clsSetup.m_bUseThreadPipe = true;
	*/

	// HTTP �������� ����� Document root ������ �����Ѵ�.
	clsServer.m_strDocumentRoot = argv[1];

	if( argc >= 3 )
	{
		clsSetup.m_iListenPort = 8443;
		clsSetup.m_bUseTls = true;
		clsSetup.m_strCertFile = argv[2];
		clsSetup.m_bUseHttp2 = true;
		clsSetup.m_bUseHttp2HeaderLog = true;
		//clsSetup.m_strCipherList = "ECDHE-RSA-AES128-SHA256:ECDHE-RSA-AES256-SHA384:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384";
		clsSetup.m_strCipherList = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:ECDHE-RSA-AES128-SHA256:ECDHE-RSA-AES256-SHA384";
	}

	if( CDirectory::IsDirectory( clsServer.m_strDocumentRoot.c_str() ) == false )
	{
		printf( "[%s] is not directory\n", clsServer.m_strDocumentRoot.c_str() );
		return 0;
	}

	// HTTP ������ �����Ѵ�. HTTP ��û�� ���ŵǸ� �̿� ���� �̺�Ʈ�� CSimpleHttpServer ��ü�� �����Ѵ�.
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

	// ��� �����尡 ����� ������ ����Ѵ�.
	sleep(2);

#ifdef WIN32
	CHttpStack::Release();
	CLog::Release();
#endif

	return 0;
}
