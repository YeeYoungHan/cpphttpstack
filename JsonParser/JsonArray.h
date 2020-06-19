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

#ifndef _JSON_ARRAY_H_
#define _JSON_ARRAY_H_

#include "JsonString.h"
#include "JsonNumber.h"
#include "JsonInt.h"
#include "JsonObject.h"

#include <list>

typedef std::list< CJsonType * > JSON_LIST;
class CJsonObject;

/**
 * @ingroup JsonParser
 * @brief JSON 배열을 저장하는 클래스
 */
class CJsonArray : public CJsonType
{
public:
	CJsonArray();
	virtual ~CJsonArray();

	virtual int Parse( const char * pszText, int iTextLen );
	virtual int ToString( std::string & strText, EJsonNewLine eNewLine = E_JNL_NULL, int iDepth = 0 );
	virtual int GetStringLen( );
	virtual CJsonType * Copy( );

	int MakeString( std::string & strText, EJsonNewLine eNewLine = E_JNL_NULL );
	void Clear();

	bool SelectData( int iIndex, std::string & strValue );
	bool SelectData( int iIndex, int64_t & iValue );
	bool SelectData( int iIndex, bool & bValue );
	bool SelectData( int iIndex, CJsonObject ** ppclsObject );
	bool SelectData( int iIndex, CJsonArray ** ppclsArray );
	bool SelectData( int iIndex, CJsonType ** ppclsType );

	bool InsertData( std::string & strValue );
	bool InsertData( const char * pszValue );
	bool InsertData( int32_t iValue );
	bool InsertData( int64_t iValue );
	bool InsertData( bool bValue );
	bool InsertData( CJsonType * pclsType );
	bool InsertData( );

	bool InsertData( int iIndex, std::string & strValue );
	bool InsertData( int iIndex, const char * pszValue );
	bool InsertData( int iIndex, int32_t iValue );
	bool InsertData( int iIndex, int64_t iValue );
	bool InsertData( int iIndex, bool bValue );
	bool InsertData( int iIndex, CJsonType * pclsType );
	bool InsertDataNull( int iIndex );

	bool InsertArray( CJsonArray * pclsArray );

	bool DeleteData( int iIndex );

	int GetCount();

	JSON_LIST m_clsList;

private:
	bool InsertDataNoCopy( int iIndex, CJsonType * pclsType );
};

#endif
