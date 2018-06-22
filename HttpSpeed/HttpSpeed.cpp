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

#include "HttpSpeed.h"

int main( int argc, char * argv[] )
{
	if( argc != 4 )
	{
		printf( "[Usage] %s {url} {request count} {thread count}\n", argv[0] );
		return 0;
	}

	gclsArg.m_strUrl = argv[1];
	gclsArg.m_iRequestCount = atoi( argv[2] );
	int iThreadCount = atoi( argv[3] );

	if( gclsArg.m_iRequestCount < 1 )
	{
		printf( "request count(%d) is not correct\n", gclsArg.m_iRequestCount );
		return 0;
	}

	if( iThreadCount < 1 )
	{
		printf( "thread count(%d) is not correct\n", iThreadCount );
		return 0;
	}

	if( gclsArg.m_iRequestCount < iThreadCount )
	{
		printf( "request count(%d) < thread count(%d)\n", gclsArg.m_iRequestCount, iThreadCount );
		return 0;
	}

	// 테스트 쓰레드 시작
	StartTestThread( iThreadCount );

	// 테스트 종료할 때까지 대기
	while( 1 )
	{
		if( gclsArg.m_clsDoneCount.GetCount() == gclsArg.m_iRequestCount )
		{
			break;
		}

		MiliSleep(20);
	}

	// 테스트 결과 출력
	gclsArg.Print();

	// 쓰레드 종료할 때까지 대기
	for( int i = 0; i < 10; ++i )
	{
		if( gclsArg.m_clsThreadCount.GetCount() == 0 )
		{
			break;
		}

		MiliSleep(20);
	}

	return 0;
}
