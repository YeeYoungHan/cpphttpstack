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
 * HTML ���� �ļ�/���� ���̺귯��
 */

/**
 * @ingroup HtmlParser
 * @brief HTML element �� ����
 */
enum EHtmlElementType
{
	E_HET_NULL,
	E_HET_SCRIPT,
	E_HET_NOT_CLOSED
};

/**
 * @ingroup HtmlParser
 * @brief HTML class map. key �� class �̸��̰� value �� �ǹ̰� ����.
 */
typedef std::map< std::string, char > HTML_CLASS_MAP;

/**
 * @ingroup HtmlParser
 * @brief CHtmlElement ��ü ����Ʈ
 */
typedef std::list< CHtmlElement > HTML_ELEMENT_LIST;

/**
 * @ingroup HtmlParser
 * @brief HTML element ���ڿ��� �Ľ��Ͽ��� �����ϴ� Ŭ����
 */
class CHtmlElement
{
public:
	CHtmlElement();
	~CHtmlElement();

	int Parse( const char * pszText, int iTextLen );
	int Parse( std::string & strText );

	bool ParseFile( const char * pszFileName );

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
