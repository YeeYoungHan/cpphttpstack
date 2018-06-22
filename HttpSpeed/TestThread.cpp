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


/**
 * @ingroup HttpSpeed
 * @brief HTTP 클라이언트 Thread
 * @param lpParameter HTTP 연결 정보 객체
 * @returns 0 을 리턴한다.
 */
THREAD_API TestThread( LPVOID lpParameter )
{
	CHttpClient clsHttpClient;
	std::string strContentType, strBody;
	struct timeval sttStart, sttEnd;
	bool bRes;
	int iMiliSecond;

	gclsArg.m_clsThreadCount.Increase();

	while( gclsArg.m_iRequestCount > gclsArg.m_clsStartCount.GetCount()  )
	{
		gclsArg.m_clsStartCount.Increase();

		gettimeofday( &sttStart, NULL );
		bRes = clsHttpClient.DoGet( gclsArg.m_strUrl.c_str(), strContentType, strBody );
		gettimeofday( &sttEnd, NULL );

		if( bRes )
		{
			gclsArg.m_clsSuccessCount.Increase();
		}
		else
		{
			gclsArg.IncreaseError( clsHttpClient.GetStatusCode() );
		}

		iMiliSecond = DiffTimeval( &sttStart, &sttEnd );

		gclsArg.UpdateTime( iMiliSecond );
	}

	gclsArg.m_clsThreadCount.Decrease();

	return 0;
}

bool StartTestThread( int iThreadCount )
{
	for( int i = 0; i < iThreadCount; ++i )
	{
		if( StartThread( "TestThread", TestThread, NULL ) == false )
		{
			break;
		}
	}

	return true;
}
