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

#ifndef _JSON_TYPE_H_
#define _JSON_TYPE_H_

#include "SipPlatformDefine.h"
#include <string>

#define JSON_TYPE_STRING	0x01
#define JSON_TYPE_NUMBER	0x02
#define JSON_TYPE_INT			0x12
#define JSON_TYPE_DOUBLE	0x22
#define JSON_TYPE_OBJECT	0x03
#define JSON_TYPE_ARRAY		0x04
#define JSON_TYPE_BOOL		0x05
#define JSON_TYPE_NULL		0x00

enum EJsonNewLine
{
	E_JNL_NULL = 0,
	E_JNL_ALL,
	E_JNL_TOP
};

/**
 * @ingroup JsonParser
 * @brief JSON 변수 추상 클래스
 */
class CJsonType
{
public:
	CJsonType();
	virtual ~CJsonType();

	virtual int Parse( const char * pszText, int iTextLen ) = 0;
	virtual int ToString( std::string & strText, EJsonNewLine eNewLine = E_JNL_NULL, int iDepth = 0 ) = 0;
	virtual int GetStringLen( ) = 0;
	virtual CJsonType * Copy( ) = 0;

	const char * GetTypeString();

	uint8_t		m_cType;

	static std::string	m_strNewLine;
};

#endif
