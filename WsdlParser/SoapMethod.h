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

#ifndef _SOAP_METHOD_H_
#define _SOAP_METHOD_H_

#include "SoapArg.h"
#include <map>

class CSoapClass;

/**
 * @ingroup WsdlParser
 * @brief SOAP 메소드 정의 클래스
 */
class CSoapMethod
{
public:
	CSoapMethod();
	~CSoapMethod();

	bool GetPrototypeCode( const char * pszClassName, std::string & strCode );
	bool GetCode( const char * pszClassName, CSoapClass * pclsSoap, std::string & strCode );

	/** 메소드 이름 */
	std::string m_strName;

	/** SOAP Action */
	std::string m_strSoapAction;

	/** 메소드 입력 인자 리스트 */
	SOAP_ARG_LIST m_clsInputList;

	/** 메소드 출력 인자 리스트 */
	SOAP_ARG_LIST m_clsOutputList;
};

typedef std::map< std::string, CSoapMethod > SOAP_METHOD_MAP;

#endif
