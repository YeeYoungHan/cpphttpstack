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

#include "JsonBool.h"

CJsonBool::CJsonBool() : m_bValue(false)
{
	m_cType = JSON_TYPE_BOOL;
}

CJsonBool::~CJsonBool()
{
}

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

int CJsonBool::ToString( std::string & strText )
{
	if( m_bValue )
	{
		strText.append( "true" );
		return 4;
	}

	strText.append( "false" );
	return 5;
}
