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

#include "SipPlatformDefine.h"
#include "XmlElement.h"
#include "StringUtility.h"
#include <stdlib.h>
#include "Log.h"
#include "MemoryDebug.h"

/**
 * @ingroup XmlParser
 * @brief ������
 */
CXmlElement::CXmlElement( const char * pszName )
{
	if( pszName )
	{
		SetName( pszName );
	}
}

/**
 * @ingroup XmlParser
 * @brief �Ҹ���
 */
CXmlElement::~CXmlElement()
{
}

#define XML_ELEMENT_NULL				0
#define XML_ELEMENT_NAME				1
#define XML_ELEMENT_ATTR				2
#define XML_ELEMENT_DATA				3
#define XML_ELEMENT_NAME_END		4
#define XML_ELEMENT_DATA_PARSE	5
#define XML_ELEMENT_COMMENT			6
#define XML_ELEMENT_CDATA				7
#define XML_ELEMENT_DECLARATION	8

/**
 * @ingroup XmlParser
 * @brief XML ���ڿ��� �Ľ��Ͽ��� ��� ������ �����Ѵ�.
 * @param pszText		XML ���ڿ�
 * @param iTextLen	XML ���ڿ� ����
 * @returns �����ϸ� �Ľ��� XML ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CXmlElement::Parse( const char * pszText, int iTextLen )
{
	int		iPos, iStartPos = -1, iLen;
	char	cType = XML_ELEMENT_NULL, cTypeOld = XML_ELEMENT_NULL;
	std::string	strName, strValue;

	Clear();

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( cType == XML_ELEMENT_COMMENT )
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
		else if( cType == XML_ELEMENT_DECLARATION )
		{
			if( pszText[iPos] == '>' )
			{
				cType = XML_ELEMENT_NULL;
			}

			continue;
		}

		if( pszText[iPos] == ' ' || pszText[iPos] == '\t' || pszText[iPos] == '/' || pszText[iPos] == '\r' || pszText[iPos] == '\n' )
		{
			if( cType == XML_ELEMENT_NAME )
			{
				m_strName.append( pszText + iStartPos, iPos - iStartPos );
				cType = XML_ELEMENT_ATTR;
				iStartPos = -1;
				strName.clear();
				strValue.clear();
			}
		}
		else if( cType == XML_ELEMENT_CDATA )
		{
			if( pszText[iPos] == ']' )
			{
				if( iPos + 3 < iTextLen && !strncmp( pszText + iPos + 1, "]>", 2 ) )
				{
					m_strData.append( pszText + iStartPos, iPos - iStartPos );
					cType = XML_ELEMENT_DATA;
					iPos += 2;
					iStartPos = iPos + 1;
				}
			}
		}
		else if( pszText[iPos] == '<' )
		{
			if( iPos + 4 < iTextLen && !strncmp( pszText + iPos + 1, "!--", 3 ) )
			{
				cTypeOld = cType;
				cType = XML_ELEMENT_COMMENT;
			}
			else if( iPos + 9 < iTextLen && !strncmp( pszText + iPos + 1, "![CDATA[", 8 ) )
			{
				if( cType == XML_ELEMENT_DATA )
				{
					m_strData.append( pszText + iStartPos, iPos - iStartPos );
				}

				cTypeOld = cType;
				cType = XML_ELEMENT_CDATA;
				iStartPos = iPos + 9;
			}
			else if( iPos + 1 < iTextLen && ( pszText[iPos+1] == '?' || pszText[iPos+1] == '!' ) )
			{
				cTypeOld = cType;
				cType = XML_ELEMENT_DECLARATION;
			}
			else if( cType == XML_ELEMENT_NULL )
			{
				cType = XML_ELEMENT_NAME;
				iStartPos = iPos + 1;
			}
			else if( pszText[iPos+1] == '/' )
			{
				if( cType == XML_ELEMENT_DATA )
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
					cType = XML_ELEMENT_NAME_END;
					iPos += iLen + 1;
				}
				else
				{
					CLog::Print( LOG_ERROR, "end tag(%.*s) is not correct for start tag(%s)", iLen, pszText + iPos + 2, m_strName.c_str() );
					return -1;
				}
			}
			else
			{
				CXmlElement clsElement;

				iLen = clsElement.Parse( pszText + iPos, iTextLen - iPos );
				if( iLen == -1 ) return -1;

				m_clsElementList.push_back( clsElement );

				cType = XML_ELEMENT_DATA_PARSE;
				iPos += iLen - 1;
			}
		}
		else if( pszText[iPos] == '>' )
		{
			if( cType == XML_ELEMENT_NAME )
			{
				m_strName.append( pszText + iStartPos, iPos - iStartPos );
			}
			else if( cType == XML_ELEMENT_NAME_END )
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

			cType = XML_ELEMENT_DATA;
			iStartPos = iPos + 1;
		}
		else if( cType == XML_ELEMENT_ATTR )
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

				m_clsAttributeMap.insert( XML_ATTRIBUTE_MAP::value_type( strName, strValue ) );

				strName.clear();
				strValue.clear();
				iStartPos = -1;
			}
		}
	}

	return iPos;
}

/**
 * @ingroup XmlParser
 * @brief XML ���ڿ��� �Ľ��Ͽ��� ��� ������ �����Ѵ�.
 * @param strText XML ���ڿ�
 * @returns �����ϸ� �Ľ��� XML ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CXmlElement::Parse( std::string & strText )
{
	return Parse( strText.c_str(), (int)strText.length() );
}

/**
 * @ingroup XmlParser
 * @brief ��� ������ ����� ���� �̿��Ͽ��� XML ���ڿ��� �����Ѵ�.
 * @param pszText			XML ���ڿ��� ������ ����
 * @param iTextSize		XML ���ڿ��� ������ ������ ũ��
 * @param bUseTab			TAB ���ڸ� ����ϴ°�? ���� element ���۽� TAB ���ڸ� �־��ְ� ������ true �� �Է��ϰ� �׷��� ������ false �� �Է��Ѵ�.
 * @param	iDepth			XML ���� element ����. �� ���� XML element �� 0 �̰� �� ���� element �� 1 �̴�.
 * @returns ������ XML ���ڿ��� ���̸� �����Ѵ�.
 */
int CXmlElement::ToString( char * pszText, int iTextSize, bool bUseTab, int iDepth )
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
		XML_ATTRIBUTE_MAP::iterator	itAM;

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
		XML_ELEMENT_LIST::iterator	itEL;

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
 * @ingroup XmlParser
 * @brief ��� ������ ����� ���� �̿��Ͽ��� XML ���ڿ��� �����Ѵ�.
 * @param strText XML ���ڿ��� ������ ����
 * @param bUseTab			TAB ���ڸ� ����ϴ°�? ���� element ���۽� TAB ���ڸ� �־��ְ� ������ true �� �Է��ϰ� �׷��� ������ false �� �Է��Ѵ�.
 * @param	iDepth			XML ���� element ����. �� ���� XML element �� 0 �̰� �� ���� element �� 1 �̴�.
 */
void CXmlElement::ToString( std::string & strText, bool bUseTab, int iDepth )
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
		XML_ATTRIBUTE_MAP::iterator	itAM;

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
		XML_ELEMENT_LIST::iterator	itEL;

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
 * @ingroup XmlParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CXmlElement::Clear( )
{
	m_strName.clear();
	m_strData.clear();
	m_clsAttributeMap.clear();
	m_clsElementList.clear();
}

/**
 * @ingroup XmlParser
 * @brief XML ������ �о ��� ������ �����Ѵ�.
 * @param pszFileName		XML ���� �̸�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::ParseFile( const char * pszFileName )
{
	FILE * fd;
	char	szBuf[1024];
	std::string	strBuf;

	fd = fopen( pszFileName, "r" );
	if( fd == NULL ) return false;

	memset( szBuf, 0, sizeof(szBuf) );
	while( fgets( szBuf, sizeof(szBuf)-1, fd ) )
	{
		strBuf.append( szBuf );
	}
	fclose( fd );

	if( Parse( strBuf.c_str(), (int)strBuf.length() ) == -1 ) return false;

	return true;
}

/**
 * @ingroup XmlParser
 * @brief XML �� ���Ͽ� �����Ѵ�.
 * @param pszFileName		XML ���� �̸�
 * @param bUseTab			TAB ���ڸ� ����ϴ°�? ���� element ���۽� TAB ���ڸ� �־��ְ� ������ true �� �Է��ϰ� �׷��� ������ false �� �Է��Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::WriteFile( const char * pszFileName, bool bUseTab )
{
	FILE * fd;
	std::string strXml;

	fd = fopen( pszFileName, "w" );
	if( fd == NULL ) return false;

	ToString( strXml, bUseTab );
	fwrite( strXml.c_str(), 1, strXml.length(), fd );

	fclose( fd );

	return true;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ѵ�.
 * @param pszName ��Ʈ����Ʈ �̸�
 * @returns �����ϸ� ��Ʈ����Ʈ�� ���� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
const char * CXmlElement::SelectAttribute( const char * pszName )
{
	XML_ATTRIBUTE_MAP::iterator	itAM;

	itAM = m_clsAttributeMap.find( pszName );
	if( itAM != m_clsAttributeMap.end() )
	{
		return itAM->second.c_str();
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ѵ�. ���� ��, �� ������ ������ ���ڿ��� �����͸� �����Ѵ�.
 * @param pszName ��Ʈ����Ʈ �̸�
 * @returns �����ϸ� ��Ʈ����Ʈ�� ���� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
const char * CXmlElement::SelectAttributeTrim( const char * pszName )
{
	XML_ATTRIBUTE_MAP::iterator	itAM;

	itAM = m_clsAttributeMap.find( pszName );
	if( itAM != m_clsAttributeMap.end() )
	{
		TrimString( itAM->second );
		return itAM->second.c_str();
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param strValue	��Ʈ����Ʈ ��
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectAttribute( const char * pszName, std::string & strValue )
{
	XML_ATTRIBUTE_MAP::iterator	itAM;

	strValue.clear();

	itAM = m_clsAttributeMap.find( pszName );
	if( itAM != m_clsAttributeMap.end() )
	{
		strValue = itAM->second;
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ѵ�. �˻��� �����ϸ� ���� ����, ������ ������ �����Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param strValue	��Ʈ����Ʈ ��
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectAttributeTrim( const char * pszName, std::string & strValue )
{
	XML_ATTRIBUTE_MAP::iterator	itAM;

	strValue.clear();

	itAM = m_clsAttributeMap.find( pszName );
	if( itAM != m_clsAttributeMap.end() )
	{
		strValue = itAM->second;
		TrimString( strValue );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ͽ� int ������ �����Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param iValue		��Ʈ����Ʈ ��
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectAttribute( const char * pszName, int & iValue )
{
	iValue = 0;

	const char * pszValue = SelectAttributeTrim( pszName );
	if( pszValue )
	{
		iValue = atoi( pszValue );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ͽ� int64_t ������ �����Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param iValue		��Ʈ����Ʈ ��
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectAttribute( const char * pszName, int64_t & iValue )
{
	iValue = 0;

	const char * pszValue = SelectAttributeTrim( pszName );
	if( pszValue )
	{
		iValue = atoll( pszValue );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ͽ� bool ������ �����Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param bValue		��Ʈ����Ʈ ��
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectAttribute( const char * pszName, bool & bValue )
{
	bValue = false;

	const char * pszValue = SelectAttributeTrim( pszName );
	if( pszValue )
	{
		bValue = GetBoolean( pszValue );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ͽ� double ������ �����Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param dbValue		��Ʈ����Ʈ ��
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectAttribute( const char * pszName, double & dbValue )
{
	dbValue = 0.0;

	const char * pszValue = SelectAttributeTrim( pszName );
	if( pszValue )
	{
		dbValue = atof( pszValue );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �˻��Ѵ�.
 * @param pszName		���� Element �̸�
 * @param iIndex		���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� ���� Element ��ü�� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CXmlElement * CXmlElement::SelectElement( const char * pszName, const int iIndex )
{
	XML_ELEMENT_LIST::iterator	itEL;
	int iCount = 0;

	if( iIndex < 0 ) return NULL;

	for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
	{
		if( !strcmp( pszName, itEL->m_strName.c_str() ) )
		{
			if( iCount == iIndex )
			{
				return &(*itEL);
			}

			++iCount;
		}
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �˻��Ѵ�.
 * @param iIndex ���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� ���� Element ��ü�� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CXmlElement * CXmlElement::SelectElement( const int iIndex )
{
	XML_ELEMENT_LIST::iterator	itEL;
	int iCount = 0;

	if( iIndex < 0 ) return NULL;

	for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
	{
		if( iCount == iIndex )
		{
			return &(*itEL);
		}

		++iCount;
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �˻��Ͽ��� Element ����Ʈ�� �����Ѵ�.
 * @param pszName		���� Element �̸�
 * @param clsList		���� Element �� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectElementList( const char * pszName, XML_ELEMENT_LIST & clsList )
{
	XML_ELEMENT_LIST::iterator	itEL;

	clsList.clear();

	for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
	{
		if( !strcmp( pszName, itEL->m_strName.c_str() ) )
		{
			clsList.push_back( *itEL );
		}
	}

	if( clsList.empty() == false ) return true;

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �� ���ڿ��� �����͸� �����Ѵ�.
 * @param pszName ���� Element �̸�
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns ���� Element �̸��� �����ϸ� �ش� Element �� �� ���ڿ��� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�. 
 */
const char * CXmlElement::GetElementData( const char * pszName, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		return pclsElement->m_strData.c_str();
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �� ���ڿ��� �����͸� �����Ѵ�.
 * @param pszName ���� Element �̸�
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns ���� Element �̸��� �����ϸ� �ش� Element �� �� ���ڿ��� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�. 
 */
const char * CXmlElement::GetElementDataTrim( const char * pszName, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		TrimString( pclsElement->m_strData );
		return pclsElement->m_strData.c_str();
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �˻��Ͽ��� ������ �����Ѵ�.
 * @param pszName		���� Element �̸�
 * @param strData		���� Elemnet �� ������ ������ ����
 * @param iIndex		���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectElementData( const char * pszName, std::string & strData, const int iIndex )
{
	strData.clear();

	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		strData = pclsElement->m_strData;
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �˻��Ͽ��� ������ �����Ѵ�. �˻��� �����ϸ� ������ ����, ������ ������ �����Ѵ�.
 * @param pszName		���� Element �̸�
 * @param strData		���� Elemnet �� ������ ������ ����
 * @param iIndex		���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectElementTrimData( const char * pszName, std::string & strData, const int iIndex )
{
	strData.clear();

	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		strData = pclsElement->m_strData;
		TrimString( strData );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �˻��Ͽ��� ���� ������ �����´�.
 * @param pszName ���� Element �̸�
 * @param iData		���� Element �� ���� �����ϴ� ����
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectElementData( const char * pszName, int & iData, const int iIndex )
{
	iData = 0;

	const char * pszValue = GetElementDataTrim( pszName, iIndex );
	if( pszValue )
	{
		iData = atoi( pszValue );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �˻��Ͽ��� 64bit ���� ������ �����´�.
 * @param pszName ���� Element �̸�
 * @param iData		���� Element �� ���� �����ϴ� ����
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectElementData( const char * pszName, int64_t & iData, const int iIndex )
{
	iData = 0;

	const char * pszValue = GetElementDataTrim( pszName, iIndex );
	if( pszValue )
	{
		iData = atoll( pszValue );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �˻��Ͽ��� bool ������ �����´�.
 * @param pszName ���� Element �̸�
 * @param bData		���� Element �� ���� �����ϴ� ����
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectElementData( const char * pszName, bool & bData, const int iIndex )
{
	bData = false;

	const char * pszValue = GetElementDataTrim( pszName, iIndex );
	if( pszValue )
	{
		bData = GetBoolean( pszValue );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �˻��Ͽ��� double ������ �����´�.
 * @param pszName ���� Element �̸�
 * @param dbData	���� Element �� ���� �����ϴ� ����
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectElementData( const char * pszName, double & dbData, const int iIndex )
{
	dbData = 0.0;

	const char * pszValue = GetElementDataTrim( pszName, iIndex );
	if( pszValue )
	{
		dbData = atof( pszValue );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief Element �̸��� �����Ѵ�.
 * @returns Element �̸��� �����Ѵ�.
 */
const char * CXmlElement::GetName()
{
	return m_strName.c_str();
}

/**
 * @ingroup XmlParser
 * @brief Element ������ �����Ѵ�.
 * @returns Element ������ �����Ѵ�.
 */
const char * CXmlElement::GetData()
{
	return m_strData.c_str();
}

/**
 * @ingroup XmlParser
 * @brief Element ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 * @returns 
 */
bool CXmlElement::IsDataEmpty()
{
	return m_strData.empty();
}

/**
 * @ingroup XmlParser
 * @brief Element �̸��� �����Ѵ�.
 * @param pszName Element �̸�
 */
void CXmlElement::SetName( const char * pszName )
{
	m_strName = pszName;
}

/**
 * @ingroup XmlParser
 * @brief Element ������ �����Ѵ�.
 * @param pszData Element ����
 */
void CXmlElement::SetData( const char * pszData )
{
	m_strData = pszData;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �߰��Ѵ�.
 * @param pszName			���� Element �̸�
 * @param pclsElement ���� Element �� element;
 */
void CXmlElement::InsertElementData( const char * pszName, CXmlElement * pclsElement )
{
	CXmlElement clsElement( pszName );

	clsElement.InsertElement( pclsElement );

	m_clsElementList.push_back( clsElement );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �߰��Ѵ�.
 * @param pszName ���� Element �̸�
 * @param pszData ���� Element �� data ��
 */
void CXmlElement::InsertElementData( const char * pszName, const char * pszData )
{
	CXmlElement clsElement( pszName );

	if( pszData )
	{
		if( strstr( pszData, "<" ) || strstr( pszData, ">" ) )
		{
			clsElement.m_strData = "<![CDATA[";
			clsElement.m_strData.append( pszData );
			clsElement.m_strData.append( "]]>" );
		}
		else
		{
			clsElement.m_strData = pszData;
		}
	}

	m_clsElementList.push_back( clsElement );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �߰��Ѵ�.
 * @param pszName ���� Element �̸�
 * @param strData ���� Element �� data ��
 */
void CXmlElement::InsertElementData( const char * pszName, std::string & strData )
{
	InsertElementData( pszName, strData.c_str() );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �߰��Ѵ�.
 * @param pszName ���� Element �̸�
 * @param iData		���� Element �� data ��
 */
void CXmlElement::InsertElementData( const char * pszName, int iData )
{
	char szData[11];

	snprintf( szData, sizeof(szData), "%d", iData );

	InsertElementData( pszName, szData );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �߰��Ѵ�.
 * @param pszName ���� Element �̸�
 * @param iData		���� Element �� data ��
 */
void CXmlElement::InsertElementData( const char * pszName, int64_t iData )
{
	char szData[21];

	snprintf( szData, sizeof(szData), LONG_LONG_FORMAT, iData );

	InsertElementData( pszName, szData );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �߰��Ѵ�.
 * @param pszName ���� Element �̸�
 * @param bData		���� Element �� data ��
 */
void CXmlElement::InsertElementData( const char * pszName, bool bData )
{
	InsertElementData( pszName, bData ? "true" : "false" );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �߰��Ѵ�.
 * @param pszName ���� Element �̸�
 * @param dbData	���� Element �� data ��
 */
void CXmlElement::InsertElementData( const char * pszName, double dbData )
{
	char szData[51];

	snprintf( szData, sizeof(szData), "%f", dbData );

	InsertElementData( pszName, szData );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �߰��Ѵ�.
 * @param pclsElement ���� Element
 */
void CXmlElement::InsertElement( CXmlElement * pclsElement )
{
	m_clsElementList.push_back( *pclsElement );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element ���� �Էµ� �̸��� ������ Element �� ������ �����Ѵ�.
 * @param pszName ���� Element �̸�
 * @param pszData ���� Element �� data ��
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns ���� Element ���� ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CXmlElement::UpdateElementData( const char * pszName, const char * pszData, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		pclsElement->SetData( pszData );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element ���� �Էµ� �̸��� ������ Element �� ������ �����Ѵ�.
 * @param pszName ���� Element �̸�
 * @param strData ���� Element �� data ��
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns ���� Element ���� ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CXmlElement::UpdateElementData( const char * pszName, std::string & strData, const int iIndex )
{
	return UpdateElementData( pszName, strData.c_str(), iIndex );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element ���� �Էµ� �̸��� ������ Element �� ������ �����Ѵ�.
 * @param pszName ���� Element �̸�
 * @param iData 
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns ���� Element ���� ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CXmlElement::UpdateElementData( const char * pszName, int iData, const int iIndex )
{
	char szData[11];

	snprintf( szData, sizeof(szData), "%d", iData );

	return UpdateElementData( pszName, szData, iIndex );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element ���� �Էµ� �̸��� ������ Element �� ������ �����Ѵ�.
 * @param pszName ���� Element �̸�
 * @param iData		���� Element �� data ��
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns ���� Element ���� ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CXmlElement::UpdateElementData( const char * pszName, int64_t iData, const int iIndex )
{
	char szData[21];

	snprintf( szData, sizeof(szData), LONG_LONG_FORMAT, iData );

	return UpdateElementData( pszName, szData, iIndex );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element ���� �Էµ� �̸��� ������ Element �� ������ �����Ѵ�.
 * @param pszName ���� Element �̸�
 * @param bData		���� Element �� data ��
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns ���� Element ���� ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CXmlElement::UpdateElementData( const char * pszName, bool bData, const int iIndex )
{
	return UpdateElementData( pszName, bData ? "true" : "false", iIndex );
}

/**
 * @ingroup XmlParser
 * @brief ���� Element ���� �Էµ� �̸��� ������ Element �� ������ �����Ѵ�.
 * @param pszName ���� Element �̸�
 * @param dbData	���� Element �� data ��
 * @param iIndex	���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns ���� Element ���� ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CXmlElement::UpdateElementData( const char * pszName, double dbData, const int iIndex )
{
	char szData[51];

	snprintf( szData, sizeof(szData), "%f", dbData );

	return UpdateElementData( pszName, szData, iIndex );
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ �̸��� �������� ������ ��Ʈ����Ʈ�� �߰��ϰ� ��Ʈ����Ʈ �̸��� �����ϸ� �ش� ���� �����Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param pszValue	��Ʈ����Ʈ ��
 */
void CXmlElement::InsertAttribute( const char * pszName, const char * pszValue )
{
	XML_ATTRIBUTE_MAP::iterator itMap;

	itMap = m_clsAttributeMap.find( pszName );
	if( itMap == m_clsAttributeMap.end() )
	{
		m_clsAttributeMap.insert( XML_ATTRIBUTE_MAP::value_type( pszName, pszValue ) );
	}
	else
	{
		itMap->second = pszValue;
	}
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �߰��Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param strValue	��Ʈ����Ʈ ��
 */
void CXmlElement::InsertAttribute( const char * pszName, std::string & strValue )
{
	InsertAttribute( pszName, strValue.c_str() );
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �߰��Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param iValue		��Ʈ����Ʈ ��
 */
void CXmlElement::InsertAttribute( const char * pszName, int iValue )
{
	char szValue[11];

	snprintf( szValue, sizeof(szValue), "%d", iValue );

	InsertAttribute( pszName, szValue );
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �߰��Ѵ�.
 * @param pszName ��Ʈ����Ʈ �̸�
 * @param iValue	��Ʈ����Ʈ ��
 */
void CXmlElement::InsertAttribute( const char * pszName, int64_t iValue )
{
	char szValue[21];

	snprintf( szValue, sizeof(szValue), LONG_LONG_FORMAT, iValue );

	InsertAttribute( pszName, szValue );
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �߰��Ѵ�.
 * @param pszName ��Ʈ����Ʈ �̸�
 * @param bValue	��Ʈ����Ʈ ��
 */
void CXmlElement::InsertAttribute( const char * pszName, bool bValue )
{
	InsertAttribute( pszName, bValue ? "true" : "false" );
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �߰��Ѵ�.
 * @param pszName ��Ʈ����Ʈ �̸�
 * @param dbValue	��Ʈ����Ʈ ��
 */
void CXmlElement::InsertAttribute( const char * pszName, double dbValue )
{
	char szValue[51];

	snprintf( szValue, sizeof(szValue), "%f", dbValue );

	InsertAttribute( pszName, szValue );
}

/**
 * @ingroup XmlParser
 * @brief element list �� �����͸� �����Ѵ�.
 * @returns element list �� �����͸� �����Ѵ�.
 */
XML_ELEMENT_LIST * CXmlElement::GetElementList()
{
	return &m_clsElementList;
}

/**
 * @brief �Էµ� ���ڿ��� bool �� ��ȯ�Ͽ��� �����Ѵ�.
 * @param pszData ���ڿ�
 * @returns �Էµ� ���ڿ��� true �̰ų� yes �̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CXmlElement::GetBoolean( const char * pszData )
{
	if( !strcasecmp( pszData, "true" ) || !strcasecmp( pszData, "yes" ) )
	{
		return true;
	}

	return false;
}
