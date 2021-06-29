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

#ifndef _HTML_ELEMENT_H_
#define _HTML_ELEMENT_H_

#include "SipPlatformDefine.h"
#include "HtmlAttribute.h"
#include <list>

class CHtmlElement;

/**
 * @defgroup HtmlParser HtmlParser
 * HTML 문서 파서/생성 라이브러리
 */

/**
 * @ingroup HtmlParser
 * @brief HTML element 옵션
 */
enum EHtmlElementOption
{
	E_HEO_NULL = 0,
	E_HEO_NOT_CHECK_END_TAG = 0x01		// end tag 유효성을 검사하지 않는다.
};

/**
 * @ingroup HtmlParser
 * @brief HTML element 의 종류
 */
enum EHtmlElementType
{
	E_HET_NULL,
	E_HET_SCRIPT,
	E_HET_NOT_CLOSED
};

/**
 * @ingroup HtmlParser
 * @brief HTML class map. key 는 class 이름이고 value 은 의미가 없다.
 */
typedef std::map< std::string, char > HTML_CLASS_MAP;

/**
 * @ingroup HtmlParser
 * @brief CHtmlElement 객체 리스트
 */
typedef std::list< CHtmlElement > HTML_ELEMENT_LIST;

/**
 * @ingroup HtmlParser
 * @brief HTML element 문자열을 파싱하여서 저장하는 클래스
 */
class CHtmlElement
{
public:
	CHtmlElement();
	~CHtmlElement();

	int Parse( const char * pszText, int iTextLen, int iOption = E_HEO_NULL );
	int Parse( std::string & strText, int iOption = E_HEO_NULL );

	bool ParseFile( const char * pszFileName, int iOption = E_HEO_NULL );

	int ToString( char * pszText, int iTextSize, bool bUseTab = false, int iDepth = 0 );
	void ToString( std::string & strText, bool bUseTab = false, int iDepth = 0 );

	void GetText( std::string & strText );
	void GetText( std::string & strText, bool (*Allow)( CHtmlElement * pclsElement ) );

	void Clear( );

	const char * SelectAttribute( const char * pszName );
	const char * SelectAttributeTrim( const char * pszName );
	bool SelectAttribute( const char * pszName, std::string & strValue );
	bool SelectAttributeTrim( const char * pszName, std::string & strValue );
	bool SelectAttribute( const char * pszName, int & iValue );

	CHtmlElement * SelectElement( const char * pszName, const int iIndex = 0 );
	CHtmlElement * SelectElement( const int iIndex );
	bool SelectElementList( const char * pszName, HTML_ELEMENT_LIST & clsList );

	const char * GetElementData( const char * pszName, const int iIndex = 0 );
	const char * GetElementDataTrim( const char * pszName, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, std::string & strData, const int iIndex = 0 );
	bool SelectElementTrimData( const char * pszName, std::string & strData, const int iIndex = 0 );

	const char * GetName();
	const char * GetData();

	bool IsDataEmpty();
	bool IsId( const char * pszId );
	bool IsClass( const char * pszClass );

	HTML_ELEMENT_LIST * GetElementList();

protected:
	void SetName( const char * pszText, int iNameLen );
	void AddAttribute( std::string & strName, std::string & strValue, char cSep );
	void AddClass( const char * pszClass, int iClassLen = -1 );

	std::string	m_strName;
	std::string m_strData;

	HTML_ATTRIBUTE_MAP	m_clsAttributeMap;
	HTML_ELEMENT_LIST		m_clsElementList;

	HTML_CLASS_MAP			m_clsClassMap;
	std::string					m_strId;

	EHtmlElementType		m_eType;
};

#endif
