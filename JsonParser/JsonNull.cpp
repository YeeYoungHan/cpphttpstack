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
#include "JsonNull.h"
#include "MemoryDebug.h"

CJsonNull::CJsonNull()
{
	m_cType = JSON_TYPE_NULL;
}

CJsonNull::~CJsonNull()
{
}

/**
 * @ingroup JsonParser
 * @brief JSON null 문자열 파싱하여서 자료구조에 저장한다.
 * @param pszText		JSON null 문자열
 * @param iTextLen	JSON null 문자열 길이
 * @returns JSON null 문자열 파싱에 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CJsonNull::Parse( const char * pszText, int iTextLen )
{
	if( !strncmp( pszText, "null", 4 ) )
	{
		return 4;
	}

	return -1;
}

/**
 * @ingroup JsonParser
 * @brief 자료구조를 JSON null 문자열로 변환한다.
 * @param strText		JSON 문자열 저장 변수
 * @param eNewLine	의미없는 변수
 * @param iDepth		의미없는 변수
 * @returns JSON boolean 문자열 길이를 리턴한다.
 */
int CJsonNull::ToString( std::string & strText, EJsonNewLine eNewLine, int iDepth )
{
	strText.append( "null" );

	return 4;
}

/**
 * @ingroup JsonParser
 * @brief ToString 메소드로 생성될 문자열 길이를 리턴한다.
 * @returns ToString 메소드로 생성될 문자열 길이를 리턴한다.
 */
int CJsonNull::GetStringLen( )
{
	return 4;
}

/**
 * @ingroup JsonParser
 * @brief 자신을 복제한 객체를 생성한다.
 * @returns 성공하면 자신을 복제한 객체를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CJsonType * CJsonNull::Copy( )
{
	CJsonNull * pclsNull = new CJsonNull();
	if( pclsNull == NULL ) return NULL;

	return pclsNull;
}
