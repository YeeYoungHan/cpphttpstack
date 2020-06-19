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
#include "JsonNumber.h"
#include "MemoryDebug.h"

CJsonNumber::CJsonNumber()
{
	m_cType = JSON_TYPE_NUMBER;
}

CJsonNumber::~CJsonNumber()
{
}

/**
 * @ingroup JsonParser
 * @brief JSON ���� ���ڿ� �Ľ��Ͽ��� �ڷᱸ���� �����Ѵ�.
 * @param pszText		JSON ���� ���ڿ�
 * @param iTextLen	JSON ���� ���ڿ� ����
 * @returns JSON ���� ���ڿ� �Ľ̿� �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CJsonNumber::Parse( const char * pszText, int iTextLen )
{
	m_strValue.clear();

	for( int i = 0; i < iTextLen; ++i )
	{
		if( i == 0 && pszText[i] == '-' )
		{
			// ù��° - �� �����̹Ƿ� �Ѿ��.
		}
		else if( isspace( pszText[i] ) || pszText[i] == ',' || pszText[i] == '}' || pszText[i] == ']' )
		{
			m_strValue.append( pszText, i );
			return i;
		}
	}

	return -1;
}

/**
 * @ingroup JsonParser
 * @brief �ڷᱸ���� JSON ���� ���ڿ��� ��ȯ�Ѵ�.
 * @param strText		JSON ���ڿ� ���� ����
 * @param eNewLine	�ǹ̾��� ����
 * @param iDepth		�ǹ̾��� ����
 * @returns JSON ���� ���ڿ� ���̸� �����Ѵ�.
 */
int CJsonNumber::ToString( std::string & strText, EJsonNewLine eNewLine, int iDepth )
{
	strText.append( m_strValue );

	return (int)m_strValue.length();
}

/**
 * @ingroup JsonParser
 * @brief ToString �޼ҵ�� ������ ���ڿ� ���̸� �����Ѵ�.
 * @returns ToString �޼ҵ�� ������ ���ڿ� ���̸� �����Ѵ�.
 */
int CJsonNumber::GetStringLen( )
{
	return (int)m_strValue.length();
}

/**
 * @ingroup JsonParser
 * @brief �ڽ��� ������ ��ü�� �����Ѵ�.
 * @returns �����ϸ� �ڽ��� ������ ��ü�� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CJsonType * CJsonNumber::Copy( )
{
	CJsonNumber * pclsNum = new CJsonNumber();
	if( pclsNum == NULL ) return NULL;

	pclsNum->m_strValue = m_strValue;

	return pclsNum;
}

/**
 * @ingroup JsonParser
 * @brief double ���� �˻��Ѵ�.
 * @returns double �̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonNumber::IsDouble( )
{
	const char * pszValue = m_strValue.c_str();
	int iLen = (int)m_strValue.length();

	for( int i = 0; i < iLen; ++i )
	{
		if( pszValue[i] == 'e' ) return true;
	}

	return false;
}
