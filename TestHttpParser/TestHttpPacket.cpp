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
#include "HttpPacket.h"
#include <stdlib.h>
#include "MemoryDebug.h"

static bool TestHttpPacketChunk( const char * pszRecvBody, const char * pszBody, int iLine )
{
	std::string strPacket = "HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json; charset=UTF-8\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n";

	strPacket.append( pszRecvBody );

	CHttpPacket clsPacket;

	// 하나의 패킷으로 모두 수신된 경우
	if( clsPacket.AddPacket( strPacket.c_str(), (int)strPacket.length() ) == false )
	{
		printf( "%s line(%d) clsPacket.AddPacket error\n", __FUNCTION__, iLine );
		return false;
	}

	if( clsPacket.IsCompleted() == false )
	{
		printf( "%s line(%d) clsPacket.IsCompleted error\n", __FUNCTION__, iLine );
		return false;
	}

	CHttpMessage * pclsMessage = clsPacket.GetHttpMessage();

	if( strcmp( pclsMessage->m_strBody.c_str(), pszBody ) )
	{
		printf( "%s line(%d) body(%s) != want body(%s)\n", __FUNCTION__, iLine, pclsMessage->m_strBody.c_str(), pszBody );
		return false;
	}

	clsPacket.ClearMessage();

	// 패킷에 1byte 씩 수신된 경우
	int iPacketLen = (int)strPacket.length();
	for( int i = 0; i < iPacketLen; ++i )
	{
		if( clsPacket.AddPacket( strPacket.c_str() + i, 1 ) == false )
		{
			printf( "%s line(%d:%d) clsPacket.AddPacket error\n", __FUNCTION__, iLine, __LINE__ );
			return false;
		}
	}

	if( clsPacket.IsCompleted() == false )
	{
		printf( "%s line(%d:%d) clsPacket.IsCompleted error\n", __FUNCTION__, iLine, __LINE__ );
		return false;
	}

	if( strcmp( pclsMessage->m_strBody.c_str(), pszBody ) )
	{
		printf( "%s line(%d:%d) body(%s) != want body(%s)\n", __FUNCTION__, iLine, __LINE__, pclsMessage->m_strBody.c_str(), pszBody );
		return false;
	}

	// 패킷에 N 개의 HTTP 응답이 수신된 경우
	std::string strNPacket;
	int iMaxCount = 10;

	for( int i = 0; i < iMaxCount; ++i )
	{
		strNPacket.append( strPacket );
	}

	int iPos = 0, iLen;

	iPacketLen = (int)strNPacket.length();
	for( int i = 0; i < iPacketLen; ++i )
	{
		iLen = rand() / 255;

		if( iLen > ( iPacketLen - iPos ) )
		{
			iLen = iPacketLen - iPos;
		}

		if( clsPacket.AddPacket( strPacket.c_str() + iPos, iLen ) == false )
		{
			printf( "%s line(%d:%d) clsPacket.AddPacket error\n", __FUNCTION__, iLine, __LINE__ );
			return false;
		}

		iPos += iLen;

		if( clsPacket.IsCompleted() )
		{
			if( strcmp( pclsMessage->m_strBody.c_str(), pszBody ) )
			{
				printf( "%s line(%d:%d) body(%s) != want body(%s)\n", __FUNCTION__, iLine, __LINE__, pclsMessage->m_strBody.c_str(), pszBody );
				return false;
			}

			clsPacket.ClearMessage();
		}
	}	

	return true;
}

static bool TestHttpPacketContentLength( const char * pszRecvBody, const char * pszBody, int iLine )
{
	char szPacket[1024];
	int iContentLength = (int)strlen( pszRecvBody );
	std::string strPacket;

	snprintf( szPacket, sizeof(szPacket), "HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json; charset=UTF-8\r\n"
		"Content-Length: %d\r\n"
		"\r\n"
		, iContentLength );

	strPacket = szPacket;
	strPacket.append( pszRecvBody );

	CHttpPacket clsPacket;

	// 하나의 패킷으로 모두 수신된 경우
	if( clsPacket.AddPacket( strPacket.c_str(), (int)strPacket.length() ) == false )
	{
		printf( "%s line(%d) clsPacket.AddPacket error\n", __FUNCTION__, iLine );
		return false;
	}

	if( clsPacket.IsCompleted() == false )
	{
		printf( "%s line(%d) clsPacket.IsCompleted error\n", __FUNCTION__, iLine );
		return false;
	}

	CHttpMessage * pclsMessage = clsPacket.GetHttpMessage();

	if( strcmp( pclsMessage->m_strBody.c_str(), pszBody ) )
	{
		printf( "%s line(%d) body(%s) != want body(%s)\n", __FUNCTION__, iLine, pclsMessage->m_strBody.c_str(), pszBody );
		return false;
	}

	return true;
}

static bool TestHttpPacketNoContentLength( const char * pszRecvBody, const char * pszBody, int iLine )
{
	std::string strPacket;

	strPacket = "HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json; charset=UTF-8\r\n"
		"\r\n";
	strPacket.append( pszRecvBody );

	CHttpPacket clsPacket;

	// 하나의 패킷으로 모두 수신된 경우
	if( clsPacket.AddPacket( strPacket.c_str(), (int)strPacket.length() ) == false )
	{
		printf( "%s line(%d) clsPacket.AddPacket error\n", __FUNCTION__, iLine );
		return false;
	}

	if( clsPacket.IsCompleted() == false )
	{
		printf( "%s line(%d) clsPacket.IsCompleted error\n", __FUNCTION__, iLine );
		return false;
	}

	CHttpMessage * pclsMessage = clsPacket.GetHttpMessage();

	if( strcmp( pclsMessage->m_strBody.c_str(), pszBody ) )
	{
		printf( "%s line(%d) body(%s) != want body(%s)\n", __FUNCTION__, iLine, pclsMessage->m_strBody.c_str(), pszBody );
		return false;
	}

	return true;
}

bool TestHttpPacket( )
{
	if( TestHttpPacketChunk( "4\r\nWiki\r\n5\r\npedia\r\nE\r\n in\r\n\r\nchunks.\r\n0\r\n\r\n"
				, "Wikipedia in\r\n\r\nchunks.", __LINE__ ) == false ) return false;

	if( TestHttpPacketContentLength( "1234567890", "1234567890", __LINE__ ) == false ) return false;

	if( TestHttpPacketNoContentLength( "1234567890", "1234567890", __LINE__ ) == false ) return false;

	return true;
}
