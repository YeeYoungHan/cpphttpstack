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
#include "HttpParameter.h"
#include "MemoryDebug.h"

CHttpParameter::CHttpParameter( char cSep ) : m_cSep(cSep)
{
}

CHttpParameter::~CHttpParameter()
{
}

/**
 * @ingroup HttpParser
 * @brief Parameter 문자열을 파싱하여 CHttpParameter 클래스의 멤버 변수에 저장한다.
 * @param pszText		HTTP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CHttpParameter::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iValuePos = -1;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == '=' )
		{
			m_strName.append( pszText, iPos );
			iValuePos = iPos + 1;
		}
		else if( pszText[iPos] == m_cSep )
		{
			break;
		}
	}

	if( iPos > 0 )
	{
		if( iValuePos != -1 )
		{
			m_strValue.append( pszText + iValuePos, iPos - iValuePos );
		}
		else
		{
			m_strName.append( pszText, iPos );
		}

		return iPos;
	}

	return -1;
}

/**
 * @ingroup HttpParser
 * @brief HTTP 메시지에 포함된 문자열을 작성한다.
 * @param pszText		HTTP 헤더의 값을 저장할 문자열 변수
 * @param iTextSize	pszText 변수의 크기
 * @returns 성공하면 작성한 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CHttpParameter::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	if( m_strName.empty() ) return 0;

	if( m_strValue.empty() )
	{
		return snprintf( pszText, iTextSize, "%s", m_strName.c_str() );
	}

	return snprintf( pszText, iTextSize, "%s=%s", m_strName.c_str(), m_strValue.c_str() );
}

/**
 * @ingroup HttpParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CHttpParameter::Clear()
{
	m_strName.clear();
	m_strValue.clear();
}
