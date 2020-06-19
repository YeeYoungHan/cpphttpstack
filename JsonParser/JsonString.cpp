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
#include "JsonString.h"
#include "MemoryDebug.h"

CJsonString::CJsonString()
{
	m_cType = JSON_TYPE_STRING;
}

CJsonString::~CJsonString()
{
}

/**
 * @ingroup JsonParser
 * @brief JSON 문자열을 파싱하여서 자료구조에 저장한다.
 * @param pszText		JSON 문자열
 * @param iTextLen	JSON 문자열 길이
 * @returns JSON 문자열을 파싱에 성공하면 파싱한 문자열 길이를 리턴하고 그렇지 않으면 -1 을 리턴한다.
 */
int CJsonString::Parse( const char * pszText, int iTextLen )
{
	char cSep = '\0';
	int iPos = 0;

	m_strValue.clear();

	for( int i = 0; i < iTextLen; ++i )
	{
		if( cSep == '\0' )
		{
			if( pszText[i] == '"' || pszText[i] == '\'' )
			{
				cSep = pszText[i];
				iPos = i + 1;
			}
		}
		else if( pszText[i] == cSep )
		{
			m_strValue.append( pszText + iPos, i - iPos );
			return i + 1;
		}
	}

	return -1;
}

/**
 * @ingroup JsonParser
 * @brief 자료구조를 JSON 문자열로 저장한다.
 * @param strText		JSON 문자열 저장 변수
 * @param eNewLine	의미없는 변수
 * @param iDepth		의미없는 변수
 * @returns JSON 문자열 길이를 리턴한다.
 */
int CJsonString::ToString( std::string & strText, EJsonNewLine eNewLine, int iDepth )
{
	strText.append( "\"" );
	strText.append( m_strValue );
	strText.append( "\"" );

	return (int)m_strValue.length() + 2;
}

/**
 * @ingroup JsonParser
 * @brief ToString 메소드로 생성될 문자열 길이를 리턴한다.
 * @returns ToString 메소드로 생성될 문자열 길이를 리턴한다.
 */
int CJsonString::GetStringLen( )
{
	return (int)m_strValue.length() + 2;
}

/**
 * @ingroup JsonParser
 * @brief 자신을 복제한 객체를 생성한다.
 * @returns 성공하면 자신을 복제한 객체를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CJsonType * CJsonString::Copy( )
{
	CJsonString * pclsString = new CJsonString();
	if( pclsString == NULL ) return NULL;

	pclsString->m_strValue = m_strValue;

	return pclsString;
}
