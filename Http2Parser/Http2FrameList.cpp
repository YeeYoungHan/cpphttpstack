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

#include "Http2FrameList.h"
#include "Log.h"
#include "MemoryDebug.h"

CHttp2FrameList::CHttp2FrameList()
{
}

CHttp2FrameList::~CHttp2FrameList()
{
	DeleteAll();
}

/**
 * @ingroup Http2Parser
 * @brief 새로운 frame 객체를 생성한다.
 * @returns 성공하면 새로운 frame 객체 포인터를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CHttp2Frame * CHttp2FrameList::CreateFrame()
{
	CHttp2Frame * pclsFrame = NULL;

	if( m_clsNotUseList.empty() == false )
	{
		pclsFrame = *(m_clsNotUseList.begin());
		m_clsNotUseList.pop_front();
	}
	else
	{
		pclsFrame = new CHttp2Frame();
		if( pclsFrame == NULL )
		{
			CLog::Print( LOG_ERROR, "%s new CHttp2Frame() error", __FUNCTION__ );
			return NULL;
		}
	}

	m_clsList.push_back( pclsFrame );

	return pclsFrame;
}

/**
 * @ingroup Http2Parser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CHttp2FrameList::Clear()
{
	HTTP2_FRAME_LIST::iterator itFL;

	for( itFL = m_clsList.begin(); itFL != m_clsList.end(); ++itFL )
	{
		(*itFL)->Clear();
		m_clsNotUseList.push_back( *itFL );
	}

	m_clsList.clear();
}

/**
 * @ingroup Http2Parser
 * @brief 메모리를 해제한다.
 */
void CHttp2FrameList::DeleteAll()
{
	HTTP2_FRAME_LIST::iterator itFL;

	for( itFL = m_clsList.begin(); itFL != m_clsList.end(); ++itFL )
	{
		delete *itFL;
	}
	m_clsList.clear();

	for( itFL = m_clsNotUseList.begin(); itFL != m_clsNotUseList.end(); ++itFL )
	{
		delete *itFL;
	}
	m_clsNotUseList.clear();
}
