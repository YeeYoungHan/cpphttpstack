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
#include "JsonString.h"
#include "MemoryDebug.h"

CJsonString::CJsonString()
{
	m_cType = JSON_TYPE_STRING;
}

CJsonString::~CJsonString()
{
}

/**
 * @ingroup JsonParser
 * @brief JSON ���ڿ��� �Ľ��Ͽ��� �ڷᱸ���� �����Ѵ�.
 * @param pszText		JSON ���ڿ�
 * @param iTextLen	JSON ���ڿ� ����
 * @returns JSON ���ڿ��� �Ľ̿� �����ϸ� �Ľ��� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CJsonString::Parse( const char * pszText, int iTextLen )
{
	char cSep = '\0';
	int iPos = 0;

	m_strValue.clear();

	for( int i = 0; i < iTextLen; ++i )
	{
		if( cSep == '\0' )
		{
			if( pszText[i] == '"' || pszText[i] == '\'' )
			{
				cSep = pszText[i];
				iPos = i + 1;
			}
		}
		else if( pszText[i] == cSep )
		{
			m_strValue.append( pszText + iPos, i - iPos );
			return i + 1;
		}
	}

	return -1;
}

/**
 * @ingroup JsonParser
 * @brief �ڷᱸ���� JSON ���ڿ��� �����Ѵ�.
 * @param strText JSON ���ڿ� ���� ����
 * @param bUseNewLine	�ǹ̾��� ����
 * @param iDepth			�ǹ̾��� ����
 * @returns JSON ���ڿ� ���̸� �����Ѵ�.
 */
int CJsonString::ToString( std::string & strText, bool bUseNewLine, int iDepth )
{
	strText.append( "\"" );
	strText.append( m_strValue );
	strText.append( "\"" );

	return (int)m_strValue.length() + 2;
}

/**
 * @ingroup JsonParser
 * @brief ToString �޼ҵ�� ������ ���ڿ� ���̸� �����Ѵ�.
 * @returns ToString �޼ҵ�� ������ ���ڿ� ���̸� �����Ѵ�.
 */
int CJsonString::GetStringLen( )
{
	return (int)m_strValue.length() + 2;
}

/**
 * @ingroup JsonParser
 * @brief �ڽ��� ������ ��ü�� �����Ѵ�.
 * @returns �����ϸ� �ڽ��� ������ ��ü�� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CJsonType * CJsonString::Copy( )
{
	CJsonString * pclsString = new CJsonString();
	if( pclsString == NULL ) return NULL;

	pclsString->m_strValue = m_strValue;

	return pclsString;
}
