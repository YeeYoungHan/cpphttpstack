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
	Clear();
}

/**
 * @ingroup JsonParser
 * @brief JSON object 문자열 파싱하여서 자료구조에 저장한다.
 * @param pszText		JSON object 문자열
 * @param iTextLen	JSON object 문자열 길이
 * @returns JSON object 문자열 파싱에 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
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
			CJsonType * pclsType = GetJsonType( pszText, iTextLen, i );

			if( pclsType )
			{
				iParseLen = pclsType->Parse( pszText + i, iTextLen - i );
				if( iParseLen == -1 )
				{
					CLog::Print( LOG_ERROR, "%s json parse error", __FUNCTION__ );
					delete pclsType;
					return -1;
				}

				m_clsMap.insert( JSON_OBJECT_MAP::value_type( strName, pclsType ) );
				cType = 0;
				i += iParseLen;
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
		else if( pszText[i] == '}' )
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

/**
 * @ingroup JsonParser
 * @brief 자료구조를 JSON object 문자열로 변환한다.
 * @param strText JSON object 문자열 저장 변수
 * @returns JSON object 문자열 길이를 리턴한다.
 */
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

		JsonToString( itMap->second, strText );
	}

	strText.append( " }" );

	return strText.length();
}

/**
 * @ingroup JsonParser
 * @brief 자료구조를 초기화시킨다.
 */
void CJsonObject::Clear()
{
	JSON_OBJECT_MAP::iterator itMap;

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		delete itMap->second;
	}

	m_clsMap.clear();
}

/**
 * @ingroup JsonParser
 * @brief JSON 문자열에 대한 CJsonType 을 생성한다.
 * @param pszText		JSON 문자열
 * @param iTextLen	JSON 문자열 길이
 * @param iPos			JSON 문자열 파싱 위치
 * @returns 성공하면 CJsonType 객체의 포인터를 리턴하고 실패하면 NULL 을 리턴한다.
 */
CJsonType * CJsonObject::GetJsonType( const char * pszText, int iTextLen, int iPos )
{
	CJsonType * pclsType = NULL;

	if( pszText[iPos] == '"' )
	{
		pclsType = new CJsonString();
		if( pclsType == NULL )
		{
			CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
			return NULL;
		}
	}
	else if( pszText[iPos] == '{' )
	{
		pclsType = new CJsonObject();
		if( pclsType == NULL )
		{
			CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
			return NULL;
		}
	}
	else if( pszText[iPos] == '[' )
	{
		pclsType = new CJsonArray();
		if( pclsType == NULL )
		{
			CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
			return NULL;
		}
	}
	else if( isdigit(pszText[iPos]) )
	{
		CJsonNumber clsNumber;

		int iParseLen = clsNumber.Parse( pszText + iPos, iTextLen - iPos );
		if( iParseLen == -1 )
		{
			CLog::Print( LOG_ERROR, "%s json number parse error", __FUNCTION__ );
			return NULL;
		}

		if( clsNumber.IsDouble() )
		{
			pclsType = new CJsonNumber();
			if( pclsType == NULL )
			{
				CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
				return NULL;
			}
		}
		else
		{
			pclsType = new CJsonInt();
			if( pclsType == NULL )
			{
				CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
				return NULL;
			}
		}
	}

	return pclsType;
}

/**
 * @ingroup JsonParser
 * @brief CJsonType 을 문자열에 저장한다.
 * @param pclsType	CJsonType 객체
 * @param strText		JSON 문자열 저장 변수
 */
void CJsonObject::JsonToString( CJsonType * pclsType, std::string & strText )
{
	switch( pclsType->m_cType )
	{
	case JSON_TYPE_STRING:
		((CJsonString *)pclsType)->ToString( strText );
		break;
	case JSON_TYPE_NUMBER:
		((CJsonNumber *)pclsType)->ToString( strText );
		break;
	case JSON_TYPE_INT:
		((CJsonInt *)pclsType)->ToString( strText );
		break;
	case JSON_TYPE_OBJECT:
		((CJsonObject *)pclsType)->ToString( strText );
		break;
	case JSON_TYPE_ARRAY:
		((CJsonArray *)pclsType)->ToString( strText );
		break;
	}
}
