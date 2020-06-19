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
#include "JsonBool.h"
#include "MemoryDebug.h"

CJsonBool::CJsonBool() : m_bValue(false)
{
	m_cType = JSON_TYPE_BOOL;
}

CJsonBool::~CJsonBool()
{
}

/**
 * @ingroup JsonParser
 * @brief JSON boolean ���ڿ� �Ľ��Ͽ��� �ڷᱸ���� �����Ѵ�.
 * @param pszText		JSON boolean ���ڿ�
 * @param iTextLen	JSON boolean ���ڿ� ����
 * @returns JSON boolean ���ڿ� �Ľ̿� �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CJsonBool::Parse( const char * pszText, int iTextLen )
{
	if( !strncmp( pszText, "true", 4 ) )
	{
		m_bValue = true;
		return 4;
	}
	else if( !strncmp( pszText, "false", 5 ) )
	{
		m_bValue = false;
		return 5;
	}

	return -1;
}

/**
 * @ingroup JsonParser
 * @brief �ڷᱸ���� JSON boolean ���ڿ��� ��ȯ�Ѵ�.
 * @param strText		JSON ���ڿ� ���� ����
 * @param eNewLine	�ǹ̾��� ����
 * @param iDepth		�ǹ̾��� ����
 * @returns JSON boolean ���ڿ� ���̸� �����Ѵ�.
 */
int CJsonBool::ToString( std::string & strText, EJsonNewLine eNewLine, int iDepth )
{
	if( m_bValue )
	{
		strText.append( "true" );
		return 4;
	}

	strText.append( "false" );
	return 5;
}

/**
 * @ingroup JsonParser
 * @brief ToString �޼ҵ�� ������ ���ڿ� ���̸� �����Ѵ�.
 * @returns ToString �޼ҵ�� ������ ���ڿ� ���̸� �����Ѵ�.
 */
int CJsonBool::GetStringLen( )
{
	if( m_bValue )
	{
		return 4;
	}

	return 5;
}

/**
 * @ingroup JsonParser
 * @brief �ڽ��� ������ ��ü�� �����Ѵ�.
 * @returns �����ϸ� �ڽ��� ������ ��ü�� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CJsonType * CJsonBool::Copy( )
{
	CJsonBool * pclsBool = new CJsonBool();
	if( pclsBool == NULL ) return NULL;

	pclsBool->m_bValue = m_bValue;

	return pclsBool;
}
