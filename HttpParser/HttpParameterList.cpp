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
#include "HttpParameterList.h"
#include "MemoryDebug.h"

CHttpParameterList::CHttpParameterList( char cSep, bool bRemoveFrontSpace ) : m_cSep( cSep ), m_bRemoveFrontSpace( bRemoveFrontSpace )
{
}

CHttpParameterList::~CHttpParameterList()
{
}

/**
 * @ingroup HttpParser
 * @brief parameter ����Ʈ ���ڿ��� �Ľ��Ͽ��� parameter ����Ʈ ��ü�� �����Ѵ�.
 * @param pszText parameter ����Ʈ ���ڿ�
 * @returns �����ϸ� �Ľ��� ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CHttpParameterList::Parse( const char * pszText )
{
	int iLen = (int)strlen( pszText );

	return Parse( pszText, iLen );
}

/**
 * @ingroup HttpParser
 * @brief parameter ����Ʈ ���ڿ��� �Ľ��Ͽ��� parameter ����Ʈ ��ü�� �����Ѵ�.
 * @param strText parameter ����Ʈ ���ڿ�
 * @returns �����ϸ� �Ľ��� ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CHttpParameterList::Parse( const std::string & strText )
{
	return Parse( strText.c_str(), (int)strText.length() );
}

/**
 * @ingroup HttpParser
 * @brief parameter ����Ʈ ���ڿ��� �Ľ��Ͽ��� parameter ����Ʈ ��ü�� �����Ѵ�.
 * @param pszText		parameter ����Ʈ ���ڿ�
 * @param iTextLen	parameter ����Ʈ ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CHttpParameterList::Parse( const char * pszText, int iTextLen )
{
	int iCurPos = 0, iPos;

	m_clsParamList.clear();

	while( iCurPos < iTextLen )
	{
		if( pszText[iCurPos] == m_cSep )
		{
			++iCurPos;
			continue;
		}

		if( m_bRemoveFrontSpace )
		{
			if( pszText[iCurPos] == ' ' )
			{
				++iCurPos;
				continue;
			}
		}

		iPos = ParseOne( pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;
	}

	return iCurPos;
}

/**
 * @ingroup HttpParser
 * @brief URL ���ڿ����� parameter ����Ʈ ���ڿ��� �Ľ��Ͽ��� parameter ����Ʈ ��ü�� �����Ѵ�.
 * @param pszText URL ���ڿ�
 * @returns �����ϸ� �Ľ��� ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CHttpParameterList::ParseUrl( const char * pszText )
{
	int iLen = (int)strlen( pszText );

	return ParseUrl( pszText, iLen );
}

/**
 * @ingroup HttpParser
 * @brief URL ���ڿ����� parameter ����Ʈ ���ڿ��� �Ľ��Ͽ��� parameter ����Ʈ ��ü�� �����Ѵ�.
 * @param strText URL ���ڿ�
 * @returns �����ϸ� �Ľ��� ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CHttpParameterList::ParseUrl( const std::string & strText )
{
	return ParseUrl( strText.c_str(), (int)strText.length() );
}

/**
 * @ingroup HttpParser
 * @brief URL ���ڿ����� parameter ����Ʈ ���ڿ��� �Ľ��Ͽ��� parameter ����Ʈ ��ü�� �����Ѵ�.
 * @param pszText		URL ���ڿ�
 * @param iTextLen	URL ���ڿ� ����
 * @returns �����ϸ� �Ľ��� ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CHttpParameterList::ParseUrl( const char * pszText, int iTextLen )
{
	int iPos = -1;

	for( int i = 0; i < iTextLen; ++i )
	{
		if( pszText[i] == '?' )
		{
			iPos = i + 1;
			break;
		}
	}

	if( iPos == -1 ) return -1;

	return Parse( pszText + iPos, iTextLen - iPos );
}

/**
 * @ingroup HttpParser
 * @brief parameter ����Ʈ ���ڿ��� �Ľ��Ͽ��� parameter ����Ʈ ��ü�� �����Ѵ�.
 * @param pszText		parameter ����Ʈ ���ڿ�
 * @param iTextLen	parameter ����Ʈ ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CHttpParameterList::ParseOne( const char * pszText, int iTextLen )
{
	CHttpParameter clsParam( m_cSep );

	int iPos = clsParam.Parse( pszText, iTextLen );
	if( iPos == -1 ) return -1;

	m_clsParamList.push_back( clsParam );

	return iPos;
}

/**
 * @ingroup HttpParser
 * @brief parameter ����Ʈ ��ü�� parameter ����Ʈ ���ڿ��� �����Ѵ�.
 * @param pszText		parameter ����Ʈ ���ڿ��� ������ ����
 * @param iTextSize parameter ����Ʈ ���ڿ��� ũ��
 * @returns parameter ����Ʈ ���ڿ��� ���̸� �����Ѵ�.
 */
int CHttpParameterList::ToString( char * pszText, int iTextSize )
{
	HTTP_PARAMETER_LIST::iterator	itList;
	int iLen = 0, iPos;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( iLen >= iTextSize ) return -1;

		if( itList != m_clsParamList.begin() )
		{
			pszText[iLen++] = m_cSep;
		}

		iPos = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( iPos == -1 ) return -1;
		iLen += iPos;
	}

	return iLen;
}

/**
 * @ingroup HttpParser
 * @brief parameter list ���� �Էµ� �̸��� ���� �����Ѵ�.
 * @param pszName		parameter �̸�
 * @param pszValue	parameter ��
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CHttpParameterList::Insert( const char * pszName, const char * pszValue )
{
	if( pszName == NULL ) return false;

	CHttpParameter clsParam;

	clsParam.m_strName = pszName;
	if( pszValue ) clsParam.m_strValue = pszValue;

	m_clsParamList.push_back( clsParam );

	return true;
}

/**
 * @ingroup HttpParser
 * @brief parameter list ���� �Էµ� �̸��� ���� ���� �����Ѵ�.
 * @param pszName		parameter �̸�
 * @param pszValue	parameter ��
 * @returns parameter �̸��� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CHttpParameterList::Update( const char * pszName, const char * pszValue )
{
	HTTP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			itList->m_strValue = pszValue;
			return true;
		}
	}

	return false;
}

/**
 * @ingroup HttpParser
 * @brief parameter list ���� �Էµ� �̸��� �˻��Ѵ�.
 * @param pszName		parameter �̸�
 * @param strValue	parameter ���� ������ ����
 * @returns parameter �̸��� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CHttpParameterList::Select( const char * pszName, std::string & strValue )
{
	HTTP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			strValue = itList->m_strValue;
			return true;
		}
	}

	return false;
}

/**
 * @ingroup HttpParser
 * @brief parameter list ���� �Էµ� �̸��� �˻��Ѵ�.
 * @param pszName parameter �̸�
 * @returns parameter �̸��� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CHttpParameterList::Select( const char * pszName )
{
	HTTP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			return true;
		}
	}

	return false;
}

/**
 * @ingroup HttpParser
 * @brief parameter list ���� �Էµ� �̸��� �˻��Ѵ�.
 * @param pszName parameter �̸�
 * @returns parameter �̸��� �����ϸ� �ش� ���� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
const char * CHttpParameterList::SelectValue( const char * pszName )
{
	HTTP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			return itList->m_strValue.c_str();
		}
	}

	return NULL;
}

/**
 * @ingroup HttpParser
 * @brief parameter list �� �����Ѵ�.
 */
void CHttpParameterList::ClearParam()
{
	m_clsParamList.clear();
}
