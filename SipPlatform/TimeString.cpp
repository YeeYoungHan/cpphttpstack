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

#include "SipPlatformDefine.h"
#include <stdio.h>
#include <stdlib.h>
#include "TimeString.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipPlatform
 * @brief 현재 시간을 년,월,일,시,분,초 구조체에 저장한다.
 * @param iTime 현재 시간
 * @param sttTm 년,월,일,시,분,초 구조체
 */
void LocalTime( time_t iTime, struct tm & sttTm )
{
#ifdef WIN32
	localtime_s( &sttTm, &iTime );
#else
	localtime_r( &iTime, &sttTm );	
#endif
}

/**
 * @ingroup SipPlatform
 * @brief 시간 변수를 시간 문자열 변수에 저장한다.
 * @param iTime			시간 변수
 * @param pszTime		시간 문자열이 저장될 변수
 * @param iTimeSize pszTime 변수의 크기
 */
void GetDateTimeString( time_t iTime, char * pszTime, int iTimeSize )
{
	struct tm	sttTm;

	LocalTime( iTime, sttTm );

	snprintf( pszTime, iTimeSize, "%04d%02d%02d%02d%02d%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday
		, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec );
}

/**
 * @ingroup SipPlatform
 * @brief 년월일시분초 문자열을 저장한다.
 * @param pszTime		년월일시분초 문자열 저장 변수
 * @param iTimeSize pszTime 변수의 크기
 */
void GetDateTimeString( char * pszTime, int iTimeSize )
{
	time_t		iTime;

	time( &iTime );

	GetDateTimeString( iTime, pszTime, iTimeSize );
}

/**
 * @ingroup SipPlatform
 * @brief 년월일 문자열을 저장한다.
 * @param iTime			시간
 * @param pszDate		년월일 문자열을 저장 변수
 * @param iDateSize pszDate 변수의 크기
 */
void GetDateString( time_t iTime, char * pszDate, int iDateSize )
{
	struct tm	sttTm;

	LocalTime( iTime, sttTm );

	snprintf( pszDate, iDateSize, "%04d%02d%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday );
}

/**
 * @ingroup SipPlatform
 * @brief 년월일 문자열을 저장한다.
 * @param pszDate		년월일 문자열을 저장 변수
 * @param iDateSize pszDate 변수의 크기
 */
void GetDateString( char * pszDate, int iDateSize )
{
	time_t		iTime;

	time( &iTime );

	GetDateString( iTime, pszDate, iDateSize );
}

/**
 * @ingroup SipPlatform
 * @brief 시분초 문자열을 저장한다.
 * @param iTime			시간
 * @param pszTime		시분초 문자열 저장 변수
 * @param iTimeSize pszTime 변수의 크기
 */
void GetTimeString( time_t iTime, char * pszTime, int iTimeSize )
{
	struct tm	sttTm;

	LocalTime( iTime, sttTm );

	snprintf( pszTime, iTimeSize, "%02d%02d%02d", sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec );
}

/**
 * @ingroup SipPlatform
 * @brief 시분초 문자열을 저장한다.
 * @param pszTime		시분초 문자열 저장 변수
 * @param iTimeSize pszTime 변수의 크기
 */
void GetTimeString( char * pszTime, int iTimeSize )
{
	time_t		iTime;

	time( &iTime );

	GetTimeString( iTime, pszTime, iTimeSize );
}

/**
 * @ingroup SipPlatform
 * @brief Set-Cookie 헤더에 포함된 expires 날짜를 time_t 로 변환한다.
 * @param pszTime Set-Cookie 헤더에 포함된 expires 날짜 예) Wed, 22-Jun-2022 05:04:13 GMT
 * @returns Set-Cookie 헤더에 포함된 expires 날짜를 time_t 로 리턴한다.
 */
time_t ParseCookieExpires( const char * pszTime )
{
	static const char * arrMonth[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL };

	struct tm	sttTm;
	char szBuf[11];
	char cType = 0;
	int iBufPos = 0;
	bool bClearBuf = false;

	memset( &sttTm, 0, sizeof(sttTm) );
	memset( szBuf, 0, sizeof(szBuf) );

	for( int i = 0; pszTime[i]; ++i )
	{
		if( pszTime[i] == ' ' )
		{
			switch( cType )
			{
			case 3:
				sttTm.tm_year = atoi( szBuf ) - 1900;
				break;
			case 6:
				sttTm.tm_sec = atoi( szBuf );
				break;
			}

			bClearBuf = true;
		}
		else if( pszTime[i] == '-' )
		{
			switch( cType )
			{
			case 1:
				sttTm.tm_mday = atoi( szBuf );
				break;
			case 2:
				for( int iIndex = 0; arrMonth[iIndex]; ++iIndex )
				{
					if( !strcasecmp( arrMonth[iIndex], szBuf ) )
					{
						sttTm.tm_mon = iIndex;
						break;
					}
				}
				break;
			}

			bClearBuf = true;
		}
		else if( pszTime[i] == ':' )
		{
			switch( cType )
			{
			case 4:
				sttTm.tm_hour = atoi( szBuf );
				break;
			case 5:
				sttTm.tm_min = atoi( szBuf );
				break;
			}

			bClearBuf = true;
		}
		else if( cType )
		{
			if( iBufPos >= ( (int)sizeof(szBuf) - 1 ) ) return 0;
			szBuf[iBufPos++] = pszTime[i];
		}

		if( bClearBuf )
		{
			memset( szBuf, 0, sizeof(szBuf) );
			iBufPos = 0;
			bClearBuf = false;
			++cType;
		}
	}

	time_t iTime = mktime( &sttTm );

	// Seoul = GMT + 9
	iTime += 9 * 3600;

	return iTime;
}
