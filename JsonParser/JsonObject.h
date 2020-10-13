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
#include "JsonBool.h"
#include "JsonNull.h"
#include <map>

typedef std::map< std::string, CJsonType * > JSON_OBJECT_MAP;

class CJsonArray;

/**
 * @defgroup JsonParser JsonParser
 * JSON 파서/생성 라이브러리
 */

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
	virtual int ToString( std::string & strText, EJsonNewLine eNewLine = E_JNL_NULL, int iDepth = 0 );
	virtual int GetStringLen( );
	virtual CJsonType * Copy( );

	int Parse( std::string & strText );
	int MakeString( std::string & strText, EJsonNewLine eNewLine = E_JNL_NULL );
	void Clear();

	bool SelectStringData( const char * pszName, std::string & strValue );

	bool SelectData( const char * pszName, std::string & strValue );
	bool SelectData( const char * pszName, int32_t & iValue );
	bool SelectData( const char * pszName, int64_t & iValue );
	bool SelectData( const char * pszName, bool & bValue );
	bool SelectData( const char * pszName, CJsonObject ** ppclsObject );
	bool SelectData( const char * pszName, CJsonArray ** ppclsArray );
	bool SelectData( const char * pszName, CJsonType ** ppclsType );

	bool InsertData( const char * pszName, const std::string & strValue );
	bool InsertData( const char * pszName, const char * pszValue );
	bool InsertData( const char * pszName, int32_t iValue );
	bool InsertData( const char * pszName, int64_t iValue );
	bool InsertData( const char * pszName, bool bValue );
	bool InsertData( const char * pszName, CJsonType * pclsType );
	bool InsertData( const char * pszName );

	bool InsertStringData( const char * pszName, int32_t iValue );
	bool InsertStringOrNullData( const char * pszName, std::string & strValue );
	bool InsertStringOrNullData( const char * pszName, const char * pszValue );

	bool UpdateData( const char * pszName, const std::string & strValue );
	bool UpdateData( const char * pszName, const char * pszValue );
	bool UpdateData( const char * pszName, int32_t iValue );
	bool UpdateData( const char * pszName, int64_t iValue );
	bool UpdateData( const char * pszName, bool bValue );
	bool UpdateData( const char * pszName, CJsonType * pclsType );
	bool UpdateData( const char * pszName );

	bool DeleteData( const char * pszName );

	bool Exist( const char * pszName );

	static CJsonType * GetJsonType( const char * pszText, int iTextLen, int iPos );
	static void JsonToString( CJsonType * pclsType, std::string & strText, EJsonNewLine eNewLine = E_JNL_NULL, int iDepth = 0 );
	static void AddTab( std::string & strText, int iDepth );

	JSON_OBJECT_MAP m_clsMap;
};

#endif
