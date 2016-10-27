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

#include "SoapArg.h"
#include "Log.h"

CSoapArg::CSoapArg() : m_eType(E_SAT_NULL)
{
}

CSoapArg::~CSoapArg()
{
}

/**
 * @ingroup WsdlParser
 * @brief 인자 타입 문자열로 타입을 설정한다.
 * @param pszType 인자 타입 문자열
 * @return 성공하면 true 을 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSoapArg::SetType( const char * pszType )
{
	const char * pszPos = strstr( pszType, ":" );

	if( pszPos )
	{
		pszType = pszPos + 1;
	}

	if( !strcmp( pszType, "string" ) )
	{
		m_eType = E_SAT_STRING;
	}
	else if( !strcmp( pszType, "int" ) )
	{
		m_eType = E_SAT_INT;
	}
	else
	{
		CLog::Print( LOG_ERROR, "%s type(%s) is not defined", __FUNCTION__, pszType );
		return false;
	}

	return true;
}

/**
 * @ingroup WsdlParser
 * @brief 소스 코드의 함수 인자 생성에 사용할 문자열을 생성한다.
 * @param strCode 변수 이름을 저장할 변수
 * @param bInput	입력 변수인가?
 * @returns 성공하면 true 을 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSoapArg::GetCode( std::string & strCode, bool bInput )
{
	std::string strVar;

	if( GetVariable( strVar, bInput ) == false ) return false;

	switch( m_eType )
	{
	case E_SAT_STRING:
		strCode = "std::string & ";
		strCode.append( strVar );
		break;
	case E_SAT_INT:
		strCode = "int & ";
		strCode.append( strVar );
		break;
	case E_SAT_NULL:
		return false;
	}

	return true;
}

/**
 * @ingroup WsdlParser
 * @brief 소스 코드 생성에 사용할 변수 이름을 생성한다.
 * @param strCode 변수 이름을 저장할 변수
 * @param bInput	입력 변수인가?
 * @returns 성공하면 true 을 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSoapArg::GetVariable( std::string & strCode, bool bInput )
{
	switch( m_eType )
	{
	case E_SAT_STRING:
		if( strncmp( m_strName.c_str(), "str", 3 ) )
		{
			strCode = "str";
		}
		strCode.append( m_strName );
		break;
	case E_SAT_INT:
		if( strncmp( m_strName.c_str(), "i", 1 ) )
		{
			strCode = "i";
		}
		strCode.append( m_strName );
		break;
	case E_SAT_NULL:
		return false;
	}

	if( bInput == false )
	{
		strCode.append( "Out" );
	}

	return true;
}
