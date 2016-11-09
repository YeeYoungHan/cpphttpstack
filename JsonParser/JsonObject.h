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

#ifndef _JSON_OBJECT_H_
#define _JSON_OBJECT_H_

#include "JsonString.h"
#include "JsonNumber.h"
#include "JsonInt.h"
#include "JsonArray.h"
#include <map>

typedef std::map< std::string, CJsonType * > JSON_OBJECT_MAP;

/**
 * @ingroup JsonParser
 * @brief JSON object 를 저장하는 클래스
 */
class CJsonObject : public CJsonType
{
public:
	CJsonObject();
	virtual ~CJsonObject();

	virtual int Parse( const char * pszText, int iTextLen );
	virtual int ToString( std::string & strText );

	void Clear();
	static CJsonType * GetJsonType( const char * pszText, int iTextLen, int iPos );
	static void JsonToString( CJsonType * pclsType, std::string & strText );

	JSON_OBJECT_MAP m_clsMap;
};

#endif
