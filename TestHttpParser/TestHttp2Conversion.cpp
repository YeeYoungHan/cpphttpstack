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
#include "Http2Conversion.h"
#include "Http2Define.h"
#include "Http2Frame.h"
#include "StringUtility.h"
#include "MemoryDebug.h"

#ifdef WIN32
#pragma comment( lib, "ws2_32" )
#endif

bool TestHttp2ConversionMessage( const char * pszPacket, const char * pszMessage, int iLine )
{
	CHttp2Conversion clsConversion;
	CHttp2Frame clsFrame;
	CHttpMessage clsMessage;
	std::string strPacket;
	char szMessage[8192];

	HexToString( pszPacket, strPacket );

	clsFrame.Set( HTTP2_FRAME_TYPE_HEADERS, HTTP2_FLAG_END_HEADER, 0x03, (uint8_t*)strPacket.c_str(), (int)strPacket.length() );

	if( clsConversion.MakeMessage( clsFrame, clsMessage ) == false )
	{
		printf( "clsConversion.MakeMessage() error - %s line(%d)\n", __FILE__, iLine );
		return false;
	}

	if( clsMessage.ToString( szMessage, sizeof(szMessage) ) <= 0 )
	{
		printf( "clsMessage.ToString() error - %s line(%d)\n", __FILE__, iLine );
		return false;
	}

	if( strcmp( szMessage, pszMessage ) )
	{
		printf( "want[%s] != output[%s] - %s line(%d)\n", pszMessage, szMessage, __FILE__, iLine );
		return false;
	}

	return true;
}

bool TestHttp2Conversion()
{
	if( TestHttp2ConversionMessage( "8c5f92497ca589d34d1f6a1271d882a60e1bf0acf7408bb0b296cb0b62d59e8313d788a8eb58594b6585b35c830b6db70f1296e4593e9403ea65b6a5040105403d7001b820a98b46ff"
		, "HTTP/2 400 Bad Request\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 0\r\nreferrer-policy: no-referrer\r\nDate: Wed, 09 Jun 2021 08:01:21 GMT\r\n\r\n", __LINE__ ) == false ) return false;

	return true;
}
