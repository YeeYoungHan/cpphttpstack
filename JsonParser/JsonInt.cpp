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
#include "JsonInt.h"
#include <stdlib.h>
#include "MemoryDebug.h"

CJsonInt::CJsonInt() : m_iValue(0)
{
	m_cType = JSON_TYPE_INT;
}

CJsonInt::~CJsonInt()
{
}

/**
 * @ingroup JsonParser
 * @brief JSON ���� ���ڿ� �Ľ��Ͽ��� �ڷᱸ���� �����Ѵ�.
 * @param pszText		JSON ���� ���ڿ�
 * @param iTextLen	JSON ���� ���ڿ� ����
 * @returns JSON ���� ���ڿ� �Ľ̿� �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CJsonInt::Parse( const char * pszText, int iTextLen )
{
	m_iValue = 0;

	for( int i = 0; i < iTextLen; ++i )
	{
		if( i == 0 && pszText[i] == '-' )
		{
			// ù��° - �� �����̹Ƿ� �Ѿ��.
		}
		else if( isdigit( pszText[i] ) == 0 )
		{
			std::string strInt;

			strInt.append( pszText, i );
			m_iValue = atoll( strInt.c_str() );

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
int CJsonInt::ToString( std::string & strText, EJsonNewLine eNewLine, int iDepth )
{
	char szText[22];

	int iLen = snprintf( szText, sizeof(szText), LONG_LONG_FORMAT, m_iValue );
	strText.append( szText );

	return iLen;
}

/**
 * @ingroup JsonParser
 * @brief ToString �޼ҵ�� ������ ���ڿ� ���̸� �����Ѵ�.
 * @returns ToString �޼ҵ�� ������ ���ڿ� ���̸� �����Ѵ�.
 */
int CJsonInt::GetStringLen( )
{
	char szText[22];

	return snprintf( szText, sizeof(szText), LONG_LONG_FORMAT, m_iValue );
}

/**
 * @ingroup JsonParser
 * @brief �ڽ��� ������ ��ü�� �����Ѵ�.
 * @returns �����ϸ� �ڽ��� ������ ��ü�� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CJsonType * CJsonInt::Copy( )
{
	CJsonInt * pclsInt = new CJsonInt();
	if( pclsInt == NULL ) return NULL;

	pclsInt->m_iValue = m_iValue;

	return pclsInt;
}
