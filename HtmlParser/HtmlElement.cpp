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

#include "HtmlElement.h"
#include "Log.h"
#include "MemoryDebug.h"

CHtmlElement::CHtmlElement() : m_bNotParseUntilNameEnd(false)
{
}

CHtmlElement::~CHtmlElement()
{
}

#define HTML_ELEMENT_NULL				0
#define HTML_ELEMENT_NAME				1
#define HTML_ELEMENT_ATTR				2
#define HTML_ELEMENT_DATA				3
#define HTML_ELEMENT_NAME_END		4
#define HTML_ELEMENT_DATA_PARSE	5
#define HTML_ELEMENT_COMMENT			6
#define HTML_ELEMENT_DECLARATION	8

/**
 * @ingroup HtmlParser
 * @brief HTML 문자열을 파싱하여서 멤버 변수에 저장한다.
 * @param pszText		HTML 문자열
 * @param iTextLen	HTML 문자열 길이
 * @returns 성공하면 파싱한 HTML 문자열의 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CHtmlElement::Parse( const char * pszText, int iTextLen )
{
	int		iPos, iStartPos = -1, iLen;
	char	cType = HTML_ELEMENT_NULL, cTypeOld = HTML_ELEMENT_NULL;
	std::string	strName, strValue;

	m_bNotParseUntilNameEnd = false;
	Clear();

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( cType == HTML_ELEMENT_COMMENT )
		{
			if( pszText[iPos] == '-' )
			{
				if( iPos + 3 < iTextLen && !strncmp( pszText + iPos + 1, "->", 2 ) )
				{
					iPos += 2;
					cType = cTypeOld;
				}
			}

			continue;
		}
		else if( cType == HTML_ELEMENT_DECLARATION )
		{
			if( pszText[iPos] == '>' )
			{
				cType = HTML_ELEMENT_NULL;
			}

			continue;
		}

		if( pszText[iPos] == ' ' || pszText[iPos] == '\t' || pszText[iPos] == '/' || pszText[iPos] == '\r' || pszText[iPos] == '\n' )
		{
			if( cType == HTML_ELEMENT_NAME )
			{
				SetName( pszText + iStartPos, iPos - iStartPos );
				cType = HTML_ELEMENT_ATTR;
				iStartPos = -1;
				strName.clear();
				strValue.clear();
			}
		}
		else if( pszText[iPos] == '<' )
		{
			if( iPos + 4 < iTextLen && !strncmp( pszText + iPos + 1, "!--", 3 ) )
			{
				cTypeOld = cType;
				cType = HTML_ELEMENT_COMMENT;
			}
			else if( iPos + 1 < iTextLen && ( pszText[iPos+1] == '?' || pszText[iPos+1] == '!' ) )
			{
				cTypeOld = cType;
				cType = HTML_ELEMENT_DECLARATION;
			}
			else if( cType == HTML_ELEMENT_NULL )
			{
				cType = HTML_ELEMENT_NAME;
				iStartPos = iPos + 1;
			}
			else if( pszText[iPos+1] == '/' )
			{
				if( cType == HTML_ELEMENT_DATA )
				{
					m_strData.append( pszText + iStartPos, iPos - iStartPos );		
				}

				iLen = (int)m_strName.length();

				if( iTextLen - ( iPos + 2 ) < (int)( iLen + 1 ) ) 
				{
					CLog::Print( LOG_ERROR, "end tag is not correct for start tag(%s)", m_strName.c_str() );
					return -1;
				}

				if( !strncmp( pszText + iPos + 2, m_strName.c_str(), iLen ) )
				{
					cType = HTML_ELEMENT_NAME_END;
					iPos += iLen + 1;
				}
				else
				{
					CLog::Print( LOG_ERROR, "end tag(%.*s) is not correct for start tag(%s)", iLen, pszText + iPos + 2, m_strName.c_str() );
					return -1;
				}
			}
			else if( m_bNotParseUntilNameEnd == false )
			{
				CHtmlElement clsElement;

				iLen = clsElement.Parse( pszText + iPos, iTextLen - iPos );
				if( iLen == -1 ) return -1;

				m_clsElementList.push_back( clsElement );

				cType = HTML_ELEMENT_DATA_PARSE;
				iPos += iLen - 1;
			}
		}
		else if( pszText[iPos] == '>' )
		{
			if( cType == HTML_ELEMENT_NAME )
			{
				SetName( pszText + iStartPos, iPos - iStartPos );
			}
			else if( cType == HTML_ELEMENT_NAME_END )
			{
				++iPos;
				break;
			}
			else if( iPos < 2 )
			{
				CLog::Print( LOG_ERROR, "iPos(%d) < 2 : pszText[iPos](%s)", iPos, pszText + iPos );
				return -1;
			}
			else if( pszText[iPos-1] == '/' )
			{
				++iPos;
				break;
			}
			
			if( cType != HTML_ELEMENT_DATA )
			{
				cType = HTML_ELEMENT_DATA;
				iStartPos = iPos + 1;
			}
		}
		else if( cType == HTML_ELEMENT_ATTR )
		{
			if( iStartPos == -1 )
			{
				iStartPos = iPos;
			}
			else if( pszText[iPos] == '=' && strName.empty() )
			{
				if( pszText[iPos+1] != '"' )
				{
					CLog::Print( LOG_ERROR, "iPos(%d+1) != '\"' : pszText[iPos](%s)", iPos, pszText + iPos );
					return -1;
				}

				strName.append( pszText + iStartPos, iPos - iStartPos );
				++iPos;
				iStartPos = iPos + 1;
			}
			else if( pszText[iPos] == '"' )
			{
				strValue.append( pszText + iStartPos, iPos - iStartPos );

				m_clsAttributeMap.insert( HTML_ATTRIBUTE_MAP::value_type( strName, strValue ) );

				strName.clear();
				strValue.clear();
				iStartPos = -1;
			}
		}
	}

	return iPos;
}

/**
 * @ingroup HtmlParser
 * @brief XML 문자열을 파싱하여서 멤버 변수에 저장한다.
 * @param strText XML 문자열
 * @returns 성공하면 파싱한 XML 문자열의 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CHtmlElement::Parse( std::string & strText )
{
	return Parse( strText.c_str(), (int)strText.length() );
}

/**
 * @ingroup HtmlParser
 * @brief 멤버 변수에 저장된 값을 이용하여서 XML 문자열을 생성한다.
 * @param pszText			XML 문자열을 저장할 변수
 * @param iTextSize		XML 문자열을 저장할 변수의 크기
 * @param bUseTab			TAB 문자를 사용하는가? 하위 element 시작시 TAB 문자를 넣어주고 싶으면 true 를 입력하고 그렇지 않으면 false 를 입력한다.
 * @param	iDepth			XML 하위 element 깊이. 맨 위의 XML element 는 0 이고 그 하위 element 는 1 이다.
 * @returns 생성된 XML 문자열의 길이를 리턴한다.
 */
int CHtmlElement::ToString( char * pszText, int iTextSize, bool bUseTab, int iDepth )
{
	int iLen = 0, n;

	if( bUseTab )
	{
		for( int i = 0; i < iDepth; ++i )
		{
			iLen += snprintf( pszText + iLen, iTextSize, "\t" );
		}
	}

	iLen += snprintf( pszText + iLen, iTextSize, "<%s", m_strName.c_str() );

	if( m_clsAttributeMap.empty() == false )
	{
		HTML_ATTRIBUTE_MAP::iterator	itAM;

		for( itAM = m_clsAttributeMap.begin(); itAM != m_clsAttributeMap.end(); ++itAM )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, " %s=\"%s\"", itAM->first.c_str(), itAM->second.c_str() );
		}
	}

	if( m_strData.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, ">%s</%s>\n", m_strData.c_str(), m_strName.c_str() );
	}
	else if( m_clsElementList.empty() == false )
	{
		HTML_ELEMENT_LIST::iterator	itEL;

		iLen += snprintf( pszText + iLen, iTextSize - iLen, ">\n" );

		for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
		{
			n = itEL->ToString( pszText + iLen, iTextSize - iLen, bUseTab, iDepth + 1 );
			if( n == -1 ) return -1;
			iLen += n;
		}

		if( bUseTab )
		{
			for( int i = 0; i < iDepth; ++i )
			{
				iLen += snprintf( pszText + iLen, iTextSize, "\t" );
			}
		}

		iLen += snprintf( pszText + iLen, iTextSize - iLen, "</%s>\n", m_strName.c_str() );
	}
	else
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "/>\n" );
	}

	return iLen;
}

/**
 * @ingroup HtmlParser
 * @brief 멤버 변수에 저장된 값을 이용하여서 XML 문자열을 생성한다.
 * @param strText XML 문자열을 저장할 변수
 * @param bUseTab			TAB 문자를 사용하는가? 하위 element 시작시 TAB 문자를 넣어주고 싶으면 true 를 입력하고 그렇지 않으면 false 를 입력한다.
 * @param	iDepth			XML 하위 element 깊이. 맨 위의 XML element 는 0 이고 그 하위 element 는 1 이다.
 */
void CHtmlElement::ToString( std::string & strText, bool bUseTab, int iDepth )
{
	if( bUseTab )
	{
		for( int i = 0; i < iDepth; ++i )
		{
			strText.append( "\t" );
		}
	}

	strText.append( "<" );
	strText.append( m_strName );

	if( m_clsAttributeMap.empty() == false )
	{
		HTML_ATTRIBUTE_MAP::iterator	itAM;

		for( itAM = m_clsAttributeMap.begin(); itAM != m_clsAttributeMap.end(); ++itAM )
		{
			strText.append( " " );
			strText.append( itAM->first );
			strText.append( "=\"" );
			strText.append( itAM->second );
			strText.append( "\"" );
		}
	}

	if( m_strData.empty() == false )
	{
		strText.append( ">" );
		strText.append( m_strData );
		strText.append( "</" );
		strText.append( m_strName );
		strText.append( ">\n" );
	}
	else if( m_clsElementList.empty() == false )
	{
		HTML_ELEMENT_LIST::iterator	itEL;

		strText.append( ">\n" );

		for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
		{
			itEL->ToString( strText, bUseTab, iDepth + 1 );
		}

		if( bUseTab )
		{
			for( int i = 0; i < iDepth; ++i )
			{
				strText.append( "\t" );
			}
		}

		strText.append( "</" );
		strText.append( m_strName );
		strText.append( ">\n" );
	}
	else
	{
		strText.append( "/>\n" );
	}
}

/**
 * @ingroup HtmlParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CHtmlElement::Clear( )
{
	m_strName.clear();
	m_strData.clear();
	m_clsAttributeMap.clear();
	m_clsElementList.clear();
}

void CHtmlElement::SetName( const char * pszText, int iNameLen )
{
	m_strName.append( pszText, iNameLen );

	const char * pszName = m_strName.c_str();

	if( !strcasecmp( pszName, "script" ) || !strcasecmp( pszName, "style" ) )
	{
		m_bNotParseUntilNameEnd = true;
	}
	else
	{
		m_bNotParseUntilNameEnd = false;
	}
}
