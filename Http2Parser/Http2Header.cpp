/* 
 * Copyright (C) 2021 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
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

#include "Http2Define.h"
#include "Http2Header.h"
#include <stdlib.h>
#include "Log.h"
#include "MemoryDebug.h"

CHttp2Header::CHttp2Header() : m_pszPacket(NULL), m_iPacketLen(0), m_iPacketSize(0)
{
}

CHttp2Header::~CHttp2Header()
{
	Delete();
}

/**
 * @ingroup Http2Parser
 * @brief 인덱스만 존재하는 HTTP/2 헤더를 추가한다.
 * @param iIndex 인덱스 번호
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Header::AddIndex( uint32_t iIndex )
{
	int iIndexLen = AddInt( 0x80, 1, iIndex );
	if( iIndexLen == 0 ) return false;

	m_iPacketLen += iIndexLen;

	return true;
}

/**
 * @ingroup Http2Parser
 * @brief 인덱스 & 값이 존재하는 HTTP/2 헤더를 추가한다.
 * @param iIndex		인덱스 번호
 * @param pszValue	헤더 값
 * @param bIndexing 증분 인덱싱이면 true 를 입력하고 그렇지 않으면 false 를 입력한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Header::AddIndexValue( uint32_t iIndex, const char * pszValue, bool bIndexing )
{
	int iIndexLen;
	
	if( bIndexing )
	{
		iIndexLen = AddInt( 0x40, 2, iIndex );
		if( iIndexLen == 0 ) return false;
	}
	else
	{
		iIndexLen = AddInt( 0x00, 4, iIndex );
		if( iIndexLen == 0 ) return false;
	}

	m_iPacketLen += iIndexLen;

	int iValueLen = (int)strlen( pszValue );
	int iIntLen = AddInt( 0x00, 1, iValueLen );
	if( iIntLen == 0 )
	{
		m_iPacketLen -= iIndexLen;
		return false;
	}

	if( ( m_iPacketLen + iIntLen + iValueLen ) > m_iPacketSize )
	{
		m_iPacketLen -= iIndexLen;
		return false;
	}

	m_iPacketLen += iIntLen;
	memcpy( m_pszPacket + m_iPacketLen, pszValue, iValueLen );
	m_iPacketLen += iValueLen;

	return true;
}

/**
 * @ingroup Http2Parser
 * @brief 이름 & 값이 존재하는 HTTP/2 헤더를 추가한다.
 * @param pszName		헤더 이름
 * @param pszValue	헤더 값
 * @param bIndexing 증분 인덱싱이면 true 를 입력하고 그렇지 않으면 false 를 입력한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Header::AddNameValue( const char * pszName, const char * pszValue, bool bIndexing )
{
	int iIntLen;
	
	if( bIndexing )
	{
		iIntLen = AddInt( 0x40, 2, 0 );
		if( iIntLen == 0 ) return false;
	}
	else
	{
		iIntLen = AddInt( 0x00, 4, 0 );
		if( iIntLen == 0 ) return false;
	}

	int iPacketLen = iIntLen;
	m_iPacketLen += iIntLen;
	
	int iNameLen = (int)strlen( pszName );
	iIntLen = AddInt( 0x00, 1, iNameLen );
	if( iIntLen == 0 )
	{
		m_iPacketLen -= iPacketLen;
		return false;
	}

	if( ( m_iPacketLen + iPacketLen + iIntLen + iNameLen ) > m_iPacketSize )
	{
		m_iPacketLen -= iPacketLen;
		return false;
	}

	m_iPacketLen += iIntLen;
	iPacketLen += iIntLen;

	memcpy( m_pszPacket + m_iPacketLen, pszName, iNameLen );
	m_iPacketLen += iNameLen;
	iPacketLen += iNameLen;

	int iValueLen = (int)strlen( pszValue );
	iIntLen = AddInt( 0x00, 1, iValueLen );
	if( iIntLen == 0 )
	{
		m_iPacketLen -= iPacketLen;
		return false;
	}

	if( ( m_iPacketLen + iPacketLen + iIntLen + iValueLen ) > m_iPacketSize )
	{
		m_iPacketLen -= iPacketLen;
		return false;
	}

	m_iPacketLen += iIntLen;
	memcpy( m_pszPacket + m_iPacketLen, pszValue, iValueLen );
	m_iPacketLen += iValueLen;

	return true;
}

/**
 * @ingroup Http2Parser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CHttp2Header::Clear()
{
	m_iPacketLen = 0;
}

/**
 * @ingroup Http2Parser
 * @brief 메모리를 해제한다.
 */
void CHttp2Header::Delete()
{
	if( m_pszPacket )
	{
		free( m_pszPacket );
		m_pszPacket = NULL;
	}

	m_iPacketSize = 0;
	m_iPacketLen = 0;
}

/**
 * @ingroup Http2Parser
 * @brief 메모리를 할당한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Header::Create()
{
	if( m_pszPacket == NULL )
	{
		m_pszPacket = (uint8_t *)malloc( HTTP2_HEADER_SIZE );
		if( m_pszPacket == NULL )
		{
			CLog::Print( LOG_ERROR, "%s malloc error", __FUNCTION__ );
			return false;
		}

		m_iPacketSize = HTTP2_HEADER_SIZE;
		m_iPacketLen = 0;
	}

	return true;
}

/**
 * @ingroup Http2Parser
 * @brief HTTP/2 헤더에 정수를 저장한다.
 * @param cPrefix			prefix
 * @param cPrefixBit	prefix bit 개수
 * @param iIndex			인덱스
 * @returns 성공하면 저장된 패킷 길이를 리턴하고 그렇지 않으면 0 을 리턴한다.
 */
int CHttp2Header::AddInt( uint8_t cPrefix, uint8_t cPrefixBit, uint32_t iIndex )
{
	if( Create() == false ) return 0;

	if( ( m_iPacketLen + 1 ) > m_iPacketSize )
	{
		return 0;
	}

	uint8_t cMax = 0xFF >> cPrefixBit;
	int iPos = 1;

	if( iIndex < cMax )
	{
		m_pszPacket[m_iPacketLen] = cPrefix | iIndex;
		return 1;
	}

	m_pszPacket[m_iPacketLen] = cPrefix | cMax;
	iIndex -= cMax;

	cMax = 0xFF >> 1;

	for( iPos = 1; ; ++iPos )
	{
		if( ( m_iPacketLen + iPos ) > m_iPacketSize )
		{
			return 0;
		}

		if( iIndex < cMax )
		{
			m_pszPacket[m_iPacketLen+iPos] = iIndex;
			++iPos;
			break;
		}

		m_pszPacket[m_iPacketLen+iPos] = 0xF0 + iIndex % 128;
		iIndex /= 128;
	}

	return iPos;
}
