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

#ifndef _HTTP2_HEADER_H_
#define _HTTP2_HEADER_H_

#include "SipPlatformDefine.h"

/**
 * @defgroup Http2Parser Http2Parser
 * HTTP/2 메시지 파서/생성 라이브러리
 */

/**
 * @ingroup Http2Parser
 * @brief HTTP/2 헤더 생성 클래스
 */
class CHttp2Header
{
public:
	CHttp2Header();
	~CHttp2Header();

	bool AddIndex( uint32_t iIndex );
	bool AddIndexValue( uint32_t iIndex, const char * pszValue, bool bIndexing = false );
	bool AddNameValue( const char * pszName, const char * pszValue, bool bIndexing = false );
	void Clear();
	void Delete();

	uint8_t	* m_pszPacket;
	int m_iPacketLen;

private:
	bool Create();
	int AddInt( uint8_t cPrefix, uint8_t cPrefixBit, uint32_t iIndex );

	int m_iPacketSize;
};

#endif
