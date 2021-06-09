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
#include "Http2Header.h"
#include "StringUtility.h"
#include "MemoryDebug.h"

bool TestHttp2Header( uint32_t iIndex, const char * pszValue, const char * pszPacket )
{
	CHttp2Header clsHeader;
	std::string strHex;

	if( clsHeader.AddIndexValue( iIndex, pszValue ) == false )
	{
		printf( "clsHeader.AddIndexValue error\n" );
		return false;
	}

	StringToHex( (char *)clsHeader.m_pszPacket, clsHeader.m_iPacketLen, strHex );

	if( strcmp( pszPacket, strHex.c_str() ) )
	{
		printf( "error\n" );
		return false;
	}

	return true;
}

bool TestHttp2Header()
{
	if( TestHttp2Header( 33, "1234", "0f120431323334" ) == false ) return false;

	return true;
}