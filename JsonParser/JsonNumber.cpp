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

#include "JsonNumber.h"

CJsonNumber::CJsonNumber()
{
	m_cType = JSON_TYPE_NUMBER;
}

CJsonNumber::~CJsonNumber()
{
}

/**
 * @ingroup JsonParser
 * @brief JSON 숫자 문자열 파싱하여서 자료구조에 저장한다.
 * @param pszText		JSON 숫자 문자열
 * @param iTextLen	JSON 숫자 문자열 길이
 * @returns JSON 숫자 문자열 파싱에 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CJsonNumber::Parse( const char * pszText, int iTextLen )
{
	m_strValue.clear();

	for( int i = 0; i < iTextLen; ++i )
	{
		if( isspace( pszText[i] ) || pszText[i] == ',' || pszText[i] == '}' || pszText[i] == ']' )
		{
			m_strValue.append( pszText, i );
			return i;
		}
	}

	return -1;
}

/**
 * @ingroup JsonParser
 * @brief 자료구조를 JSON 숫자 문자열로 변환한다.
 * @param strText JSON 문자열 저장 변수
 * @returns JSON 숫자 문자열 길이를 리턴한다.
 */
int CJsonNumber::ToString( std::string & strText )
{
	strText.append( m_strValue );

	return m_strValue.length();
}

bool CJsonNumber::IsDouble( )
{
	const char * pszValue = m_strValue.c_str();
	int iLen = m_strValue.length();

	for( int i = 0; i < iLen; ++i )
	{
		if( pszValue[i] == 'e' ) return true;
	}

	return false;
}
