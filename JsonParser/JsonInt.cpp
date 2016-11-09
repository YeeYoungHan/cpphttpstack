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

#include "JsonInt.h"

CJsonInt::CJsonInt() : m_iValue(0)
{
	m_cType = JSON_TYPE_INT;
}

CJsonInt::~CJsonInt()
{
}

int CJsonInt::Parse( const char * pszText, int iTextLen )
{
	m_iValue = 0;

	for( int i = 0; i < iTextLen; ++i )
	{
		if( isdigit( pszText[i] ) == 0 )
		{
			std::string strInt;

			strInt.append( pszText, i );
			m_iValue = atoll( strInt.c_str() );

			return i;
		}
	}

	return -1;
}

int CJsonInt::ToString( std::string & strText )
{
	char szText[22];

	int iLen = snprintf( szText, sizeof(szText), LONG_LONG_FORMAT, m_iValue );
	strText.append( szText );

	return iLen;
}
