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

#include "Http2HpackHeaderList.h"

CHttp2HpackHeaderList::CHttp2HpackHeaderList() : m_iMaxCount(4096)
{
}

CHttp2HpackHeaderList::~CHttp2HpackHeaderList()
{
}

/**
 * @ingroup Http2Parser
 * @brief HPACK 헤더를 HPACK 리스트에 저장한다.
 * @param pclsHpack HPACK 헤더
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2HpackHeaderList::Insert( CHttp2HpackHeader * pclsHpack )
{
	CHttp2HpackHeader clsHpack;
	HTTP2_HPACK_HEADER_LIST::iterator itHL;

	m_clsList.push_front( clsHpack );
	itHL = m_clsList.begin();
	if( itHL == m_clsList.end() )
	{
		return false;
	}

	itHL->m_iIndex = pclsHpack->m_iIndex;
	itHL->m_strName = pclsHpack->m_strName;
	itHL->m_strValue = pclsHpack->m_strValue;

	for( int iCount = (int)m_clsList.size(); iCount > m_iMaxCount; --iCount )
	{
		m_clsList.pop_back();
	}

	return true;
}

/**
 * @ingroup Http2Parser
 * @brief HPACK 헤더 리스트에서 입력된 인덱스에 해당하는 HPACK 헤더가 존재하면 해당 HPACK 헤더를 저장한다.
 * @param iIndex			인덱스
 * @param ppclsHpack	[out] HPACK 헤더의 포인터의 포인터
 * @returns HPACK 인덱스에 해당하는 HPACK 헤더가 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2HpackHeaderList::Select( int iIndex, CHttp2HpackHeader ** ppclsHpack )
{
	HTTP2_HPACK_HEADER_LIST::iterator itHL;
	int iCount = 62;

	for( itHL = m_clsList.begin(); itHL != m_clsList.end(); ++itHL )
	{
		if( iCount == iIndex )
		{
			*ppclsHpack = &(*itHL);
			return true;
		}

		++iCount;
	}

	return false;
}

/**
 * @ingroup Http2Parser
 * @brief 최대 리스트 길이를 저장한다.
 * @param iMaxCount 최대 리스트 길이
 */
void CHttp2HpackHeaderList::SetMaxCount( int iMaxCount )
{
	m_iMaxCount = iMaxCount;
}
