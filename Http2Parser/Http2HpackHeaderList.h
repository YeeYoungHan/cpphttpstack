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

#ifndef _HTTP2_HPACK_HEADER_LIST_H_
#define _HTTP2_HPACK_HEADER_LIST_H_

#include "Http2HpackHeader.h"
#include <list>
#include <map>

typedef std::list< CHttp2HpackHeader > HTTP2_HPACK_HEADER_LIST;

/**
 * @ingroup Http2Parser
 * @brief HPACK 헤더 리스트 클래스
 */
class CHttp2HpackHeaderList
{
public:
	CHttp2HpackHeaderList();
	~CHttp2HpackHeaderList();

	bool Insert( CHttp2HpackHeader * pclsHpack );
	bool Select( int iIndex, CHttp2HpackHeader ** ppclsHpack );

	void SetMaxCount( int iMaxCount );

private:
	HTTP2_HPACK_HEADER_LIST m_clsList;
	int m_iMaxCount;
};

#endif
