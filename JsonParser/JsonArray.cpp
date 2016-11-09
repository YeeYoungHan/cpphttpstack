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

#include "JsonObject.h"
#include "Log.h"

CJsonArray::CJsonArray()
{
	m_cType = JSON_TYPE_ARRAY;
}

CJsonArray::~CJsonArray()
{
	Clear();
}

int CJsonArray::Parse( const char * pszText, int iTextLen )
{
	int iPos = -1, iParseLen;
	uint8_t	cType = 0;
	std::string	strName;

	for( int i = 0; i < iTextLen; ++i )
	{
		if( pszText[i] == '[' )
		{
			iPos = i + 1;
			break;
		}
	}

	if( iPos == -1 ) 
	{
		CLog::Print( LOG_ERROR, "%s [ is not found", __FUNCTION__ );
		return -1;
	}

	for( int i = iPos; i < iTextLen; ++i )
	{
		if( cType == 0 )
		{
			CJsonType * pclsType = CJsonObject::GetJsonType( pszText, iTextLen, i );

			if( pclsType )
			{
				iParseLen = pclsType->Parse( pszText + i, iTextLen - i );
				if( iParseLen == -1 )
				{
					CLog::Print( LOG_ERROR, "%s json parse error", __FUNCTION__ );
					delete pclsType;
					return -1;
				}

				m_clsList.push_back( pclsType );
				cType = 0;
				i += iParseLen;
			}
		}
		else if( pszText[i] == ']' )
		{
			if( cType == 0 )
			{
				iPos = i + 1;
				break;
			}
		}
	}

	return iPos;
}

int CJsonArray::ToString( std::string & strText )
{
	JSON_LIST::iterator itJL;

	strText.append( "[" );

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		if( itJL == m_clsList.begin() )
		{
			strText.append( " " );
		}
		else
		{
			strText.append( ", " );
		}

		CJsonObject::JsonToString( *itJL, strText );
	}

	strText.append( " ]" );

	return strText.length();
}

void CJsonArray::Clear()
{
	JSON_LIST::iterator itJL;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		delete *itJL;
	}

	m_clsList.clear();
}
