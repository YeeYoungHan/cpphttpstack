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

#include "HttpSpeedArg.h"

CHttpSpeedArg gclsArg;

CHttpSpeedArg::CHttpSpeedArg() : m_iRequestCount(0), m_iMinMiliSec(-1), m_iMaxMiliSec(0), m_iAvgMiliSec(0), m_iTotalMiliSec(0)
{
}

CHttpSpeedArg::~CHttpSpeedArg()
{
}

void CHttpSpeedArg::IncreaseError( int iErrorCode )
{
	ERROR_MAP::iterator itMap;

	m_clsMutex.acquire();
	itMap = m_clsErrorMap.find( iErrorCode );
	if( itMap == m_clsErrorMap.end() )
	{
		m_clsErrorMap.insert( ERROR_MAP::value_type( iErrorCode, 1 ) );
	}
	else
	{
		++itMap->second;
	}
	m_clsMutex.release();
}

void CHttpSpeedArg::UpdateTime( int iMiliSecond )
{
	m_clsDoneCount.Increase();

	m_clsMutex.acquire();
	if( ( m_iMinMiliSec == -1 ) || ( m_iMinMiliSec > iMiliSecond ) )
	{
		m_iMinMiliSec = iMiliSecond;
	}

	if( iMiliSecond > m_iMaxMiliSec )
	{
		m_iMaxMiliSec = iMiliSecond;
	}

	m_iTotalMiliSec += iMiliSecond;

	m_iAvgMiliSec = (int)( m_iTotalMiliSec / m_clsDoneCount.GetCount() );
	m_clsMutex.release();
}

void CHttpSpeedArg::Print()
{
	printf( "min/max/avg ms => %d / %d / %d ms\n", m_iMinMiliSec, m_iMaxMiliSec, m_iAvgMiliSec );
	printf( "success count  => %d\n", m_clsSuccessCount.GetCount() );

	ERROR_MAP::iterator itMap;

	m_clsMutex.acquire();
	for( itMap = m_clsErrorMap.begin(); itMap != m_clsErrorMap.end(); ++itMap )
	{
		printf( "error(%d) => %d\n", itMap->first, itMap->second );
	}
	m_clsMutex.release();
}
