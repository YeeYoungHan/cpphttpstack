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

#ifndef _HTTP_SPEED_ARG_H_
#define _HTTP_SPEED_ARG_H_

#include "SipPlatformDefine.h"
#include <string>
#include <map>
#include "SafeCount.h"
#include "SipMutex.h"

// key = error code, value = count
typedef std::map< int, int > ERROR_MAP;

class CHttpSpeedArg
{
public:
	CHttpSpeedArg();
	~CHttpSpeedArg();

	void IncreaseError( int iErrorCode );
	void UpdateTime( int iMiliSecond );
	void Print();

	std::string m_strUrl;
	int					m_iRequestCount;

	CSafeCount	m_clsThreadCount;
	CSafeCount	m_clsStartCount;
	CSafeCount	m_clsDoneCount;
	CSafeCount	m_clsSuccessCount;

	ERROR_MAP		m_clsErrorMap;

	int					m_iMinMiliSec;
	int					m_iMaxMiliSec;
	int					m_iAvgMiliSec;
	int64_t			m_iTotalMiliSec;

	CSipMutex		m_clsMutex;
};

extern CHttpSpeedArg gclsArg;

#endif
