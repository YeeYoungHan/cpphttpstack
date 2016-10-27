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

#ifndef _SOAP_ARG_H_
#define _SOAP_ARG_H_

#include <string>
#include <list>

/**
 * @ingroup WsdlParser
 * @brief SOAP 메소드 인자 타입
 */
enum ESoapArgType
{
	E_SAT_NULL = 0,
	E_SAT_STRING,
	E_SAT_INT
};

/**
 * @ingroup WsdlParser
 * @brief SOAP 메소드 인자 정의 클래스
 */
class CSoapArg
{
public:
	CSoapArg();
	~CSoapArg();

	bool SetType( const char * pszType );
	bool GetCode( std::string & strCode, bool bInput );
	bool GetVariable( std::string & strCode, bool bInput );

	/** 메소드 인자 이름 */
	std::string		m_strName;

	/** 메소드 인자 타입 */
	ESoapArgType	m_eType;
};

typedef std::list< CSoapArg > SOAP_ARG_LIST;

#endif
