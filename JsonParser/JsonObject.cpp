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

CJsonObject::CJsonObject()
{
	m_cType = JSON_TYPE_OBJECT;
}

CJsonObject::~CJsonObject()
{
}

int CJsonObject::Parse( const char * pszText, int iTextLen )
{
	int iPos = -1, iParseLen;
	uint8_t	cType = 0;
	std::string	strName;

	for( int i = 0; i < iTextLen; ++i )
	{
		if( pszText[i] == '{' )
		{
			iPos = i + 1;
			break;
		}
	}

	if( iPos == -1 ) 
	{
		CLog::Print( LOG_ERROR, "%s { is not found", __FUNCTION__ );
		return -1;
	}

	for( int i = iPos; i < iTextLen; ++i )
	{
		if( cType == 3 )
		{
			if( pszText[i] == '"' )
			{
				CJsonString * pclsString = new CJsonString();
				if( pclsString == NULL )
				{
					CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
					return -1;
				}

				iParseLen = pclsString->Parse( pszText + i, iTextLen - i );
				if( iParseLen == -1 )
				{
					CLog::Print( LOG_ERROR, "%s json string parse error", __FUNCTION__ );
					delete pclsString;
					return -1;
				}

				m_clsMap.insert( JSON_OBJECT_MAP::value_type( strName, pclsString ) );
				i += iParseLen;
			}
			else if( pszText[i] == '{' )
			{
				CJsonObject * pclsObject = new CJsonObject();
				if( pclsObject == NULL )
				{
					CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
					return -1;
				}

				iParseLen = pclsObject->Parse( pszText + i, iTextLen - i );
				if( iParseLen == -1 )
				{
					CLog::Print( LOG_ERROR, "%s json object parse error", __FUNCTION__ );
					delete pclsObject;
					return -1;
				}

				m_clsMap.insert( JSON_OBJECT_MAP::value_type( strName, pclsObject ) );
				i += iParseLen;
			}
			else if( pszText[i] == ',' )
			{
				cType = 0;
			}
		}
		else if( pszText[i] == '"' )
		{
			if( cType == 0 )
			{
				iPos = i + 1;
				++cType;
			}
			else if( cType == 1 )
			{
				strName.clear();
				strName.append( pszText + iPos, i - iPos );
				++cType;
			}
		}
		else if( pszText[i] == ':' )
		{
			if( cType == 2 )
			{
				++cType;
			}
		}
	}

	return 0;
}

int CJsonObject::ToString( std::string & strText )
{
	JSON_OBJECT_MAP::iterator itMap;

	strText.append( "{" );

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		if( itMap != m_clsMap.begin() )
		{
			strText.append( "," );
		}

		strText.append( " \"" );
		strText.append( itMap->first );
		strText.append( "\" : " );

		switch( itMap->second->m_cType )
		{
		case JSON_TYPE_STRING:
			((CJsonString *)itMap->second)->ToString( strText );
			break;
		case JSON_TYPE_OBJECT:
			((CJsonObject *)itMap->second)->ToString( strText );
			break;
		}
	}

	strText.append( "}" );

	return strText.length();
}

void CJsonObject::Clear()
{
	JSON_OBJECT_MAP::iterator itMap;

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		delete itMap->second;
	}

	m_clsMap.clear();
}
