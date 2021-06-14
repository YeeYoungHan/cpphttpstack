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

#include "Http2HpackHeader.h"
#include "Http2HuffmanCode.h"
#include <stdlib.h>
#include "MemoryDebug.h"

CHttp2HpackHeader::CHttp2HpackHeader() : m_iIndex(0), m_bIncrementalIndexing(false), m_pszBuf(NULL), m_iBufSize(0)
{
}

CHttp2HpackHeader::~CHttp2HpackHeader()
{
	if( m_pszBuf )
	{
		free( m_pszBuf );
		m_pszBuf = NULL;
	}
}

/**
 * @ingroup Http2Parser
 * @brief HPACK 헤더를 파싱하여서 멤버 변수에 저장한다.
 * @param pszBody		HPACK 패킷
 * @param iBodyLen	HPACK 패킷 길이
 * @returns 성공하면 파싱한 HPACK 패킷 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CHttp2HpackHeader::Parse( const uint8_t * pszBody, int iBodyLen )
{
	int n, iBodyPos = 0;

	Clear();

	if( pszBody[0] & 0x80 )
	{
		n = ParseInt( pszBody, iBodyLen, 1, m_iIndex );
		if( n <= 0 ) return -1;

		++iBodyPos;
	}
	else if( pszBody[0] == 0x40 )
	{
		++iBodyPos;

		n = ParseString( pszBody + iBodyPos, iBodyLen - iBodyPos, m_strName );
		if( n <= 0 ) return -1;
		iBodyPos += n;

		n = ParseString( pszBody + iBodyPos, iBodyLen - iBodyPos, m_strValue );
		if( n <= 0 ) return -1;
		iBodyPos += n;

		m_bIncrementalIndexing = true;
	}
	else if( pszBody[0] & 0x40 )
	{
		n = ParseInt( pszBody, iBodyLen, 2, m_iIndex );
		if( n <= 0 ) return -1;
		iBodyPos += n;

		n = ParseString( pszBody + iBodyPos, iBodyLen - iBodyPos, m_strValue );
		if( n <= 0 ) return -1;
		iBodyPos += n;

		m_bIncrementalIndexing = true;
	}
	else if( pszBody[0] == 0x00 )
	{
		++iBodyPos;

		n = ParseString( pszBody + iBodyPos, iBodyLen - iBodyPos, m_strName );
		if( n <= 0 ) return -1;
		iBodyPos += n;

		n = ParseString( pszBody + iBodyPos, iBodyLen - iBodyPos, m_strValue );
		if( n <= 0 ) return -1;
		iBodyPos += n;
	}
	else if( pszBody[0] & 0x0F )
	{
		n = ParseInt( pszBody, iBodyLen, 4, m_iIndex );
		if( n <= 0 ) return -1;
		iBodyPos += n;

		n = ParseString( pszBody + iBodyPos, iBodyLen - iBodyPos, m_strValue );
		if( n <= 0 ) return -1;
		iBodyPos += n;
	}

	return iBodyPos;
}

/**
 * @ingroup Http2Parser
 * @brief HPACK 헤더를 문자열로 파싱한다.
 * @param pszBody		HPACK 패킷
 * @param iBodyLen	HPACK 패킷 길이
 * @param strOutput [out] HPACK 패킷에 저장된 문자열
 * @returns 성공하면 파싱한 HPACK 패킷 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CHttp2HpackHeader::ParseString( const uint8_t * pszBody, int iBodyLen, std::string & strOutput )
{
	int n, iBodyPos = 0, iLen;
	bool bHuffmanCode = false;

	strOutput.clear();

	if( pszBody[0] & 0x80 ) bHuffmanCode = true;

	n = ParseInt( pszBody, iBodyLen, 1, iLen );
	if( n <= 0 ) return -1;
	iBodyPos += n;

	if( iBodyPos + iLen > iBodyLen ) return -1;

	if( bHuffmanCode )
	{
		if( iLen * 3 > m_iBufSize )
		{
			if( m_pszBuf )
			{
				free( m_pszBuf );
				m_pszBuf = NULL;
				m_iBufSize = 0;
			}
		}

		if( m_pszBuf == NULL )
		{
			m_pszBuf = (uint8_t *)malloc( iLen * 3 );
			if( m_pszBuf == NULL ) return -1;

			m_iBufSize = iLen * 3;
		}

		n = HuffmanCodeDecode( pszBody + iBodyPos, iLen, m_pszBuf, m_iBufSize );
		if( n == -1 ) return -1;

		strOutput.append( (char *)m_pszBuf, n );
	}
	else
	{
		strOutput.append( (char *)pszBody + iBodyPos, iLen );
	}
	iBodyPos += iLen;

	return iBodyPos;
}

/**
 * @ingroup Http2Parser
 * @brief HPACK 헤더에서 정수를 파싱한다.
 * @param pszBody			HPACK 패킷
 * @param iBodyLen		HPACK 패킷 길이
 * @param iPrefixBit	prefix bit 길이
 * @param iOutput			[out] HPACK 헤더에 저장된 정수
 * @returns 성공하면 파싱한 HPACK 패킷 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CHttp2HpackHeader::ParseInt( const uint8_t * pszBody, int iBodyLen, int iPrefixBit, int & iOutput )
{
	uint8_t cMax = 0xFF >> iPrefixBit;
	uint8_t cCur = 0;
	int iPos = 0, iMulti = 1;

	iOutput = 0;

	for( ; iPos < iBodyLen; ++iPos )
	{
		switch( iPrefixBit )
		{
		case 1: cCur = pszBody[iPos] & 0x7F; break;
		case 2: cCur = pszBody[iPos] & 0x3F; break;
		case 3: cCur = pszBody[iPos] & 0x1F; break;
		case 4: cCur = pszBody[iPos] & 0x0F; break;
		case 5: cCur = pszBody[iPos] & 0x07; break;
		case 6: cCur = pszBody[iPos] & 0x03; break;
		case 7: cCur = pszBody[iPos] & 0x01; break;
		}

		iOutput += cCur * iMulti;

		if( cMax == cCur )
		{
			if( iPos && ( ( pszBody[iPos] >> 7 ) & 0x00 ) ) break;

			iPrefixBit = 1;
			cMax = 0xFF >> iPrefixBit;

			if( iPos >= 1 ) iMulti *= 128;
		}
		else
		{
			++iPos;
			break;
		}
	}

	return iPos;
}

/**
 * @ingroup Http2Parser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CHttp2HpackHeader::Clear()
{
	m_iIndex = 0;
	m_strName.clear();
	m_strValue.clear();
	m_bIncrementalIndexing = false;
}
