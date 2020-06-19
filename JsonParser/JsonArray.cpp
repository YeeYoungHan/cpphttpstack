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
#include "JsonObject.h"
#include "Log.h"
#include "MemoryDebug.h"

CJsonArray::CJsonArray()
{
	m_cType = JSON_TYPE_ARRAY;
}

CJsonArray::~CJsonArray()
{
	Clear();
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열 문자열 파싱하여서 자료구조에 저장한다.
 * @param pszText		JSON 배열 문자열
 * @param iTextLen	JSON 배열 문자열 길이
 * @returns JSON 배열 문자열 파싱에 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CJsonArray::Parse( const char * pszText, int iTextLen )
{
	int iPos = -1, iParseLen;
	uint8_t	cType = 0;
	std::string	strName;

	Clear();

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
				cType = 1;
				i += iParseLen - 1;
			}
			else if( pszText[i] == ']' )
			{
				iPos = i + 1;
				break;
			}
		}
		else if( pszText[i] == ',' )
		{
			cType = 0;
		}
		else if( pszText[i] == ']' )
		{
			iPos = i + 1;
			break;
		}
	}

	return iPos;
}

/**
 * @ingroup JsonParser
 * @brief 자료구조를 JSON 배열 문자열로 변환한다.
 * @param strText		JSON 배열 문자열 저장 변수
 * @param eNewLine	new line 을 입력하는 방법에 대한 설정값
 * @param	iDepth		하위 항목의 깊이. 맨 위의 항목은 0 이고 그 하위 항목은 1 이다.
 * @returns JSON 배열 문자열 길이를 리턴한다.
 */
int CJsonArray::ToString( std::string & strText, EJsonNewLine eNewLine, int iDepth )
{
	JSON_LIST::iterator itJL;
	std::string strBuf;

	if( m_clsList.empty() )
	{
		strBuf.append( "[ ]" );
	}
	else
	{
		strBuf.append( "[" );

		if( eNewLine != E_JNL_NULL ) strBuf.append( m_strNewLine );

		for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
		{
			if( itJL == m_clsList.begin() )
			{
				if( eNewLine == E_JNL_NULL ) strBuf.append( " " );
			}
			else
			{
				if( eNewLine != E_JNL_NULL ) 
				{
					strBuf.append( "," );
					strBuf.append( m_strNewLine );
				}
				else
				{
					strBuf.append( ", " );
				}
			}

			if( eNewLine != E_JNL_NULL ) 
			{
				CJsonObject::AddTab( strBuf, iDepth + 1 );
			}

			CJsonObject::JsonToString( *itJL, strBuf, eNewLine == E_JNL_TOP ? E_JNL_NULL : eNewLine, iDepth + 1 );
		}

		if( eNewLine != E_JNL_NULL )
		{
			strBuf.append( m_strNewLine );
			CJsonObject::AddTab( strBuf, iDepth );
			strBuf.append( "]" );
		}
		else
		{
			strBuf.append( " ]" );
		}
	}

	strText.append( strBuf );

	return (int)strBuf.length();
}

/**
 * @ingroup JsonParser
 * @brief ToString 메소드로 생성될 문자열 길이를 리턴한다.
 * @returns ToString 메소드로 생성될 문자열 길이를 리턴한다.
 */
int CJsonArray::GetStringLen( )
{
	JSON_LIST::iterator itJL;
	int iLen = 3;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		if( itJL == m_clsList.begin() )
		{
			iLen += 1;
		}
		else
		{
			iLen += 2;
		}

		iLen += (*itJL)->GetStringLen();
	}

	return iLen;
}

/**
 * @ingroup JsonParser
 * @brief 자신을 복제한 객체를 생성한다.
 * @returns 성공하면 자신을 복제한 객체를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CJsonType * CJsonArray::Copy( )
{
	CJsonArray * pclsArray = new CJsonArray();
	if( pclsArray == NULL ) return NULL;

	JSON_LIST::iterator itJL;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		CJsonType * pclsType = (*itJL)->Copy();
		if( pclsType == NULL )
		{
			delete pclsArray;
			return NULL;
		}

		pclsArray->m_clsList.push_back( pclsType );
	}

	return pclsArray;
}

/**
 * @ingroup JsonParser
 * @brief 자료구조를 JSON array 문자열로 변환한다. 본 메소드는 입력된 strText 를 초기화시킨 후, ToString 메소드를 호출한다.
 * @param strText		JSON array 문자열 저장 변수
 * @param eNewLine	new line 을 입력하는 방법에 대한 설정값
 * @returns JSON object 문자열 길이를 리턴한다.
 */
int CJsonArray::MakeString( std::string & strText, EJsonNewLine eNewLine )
{
	strText.clear();

	return ToString( strText, eNewLine );
}

/**
 * @ingroup JsonParser
 * @brief 자료구조를 초기화시킨다.
 */
void CJsonArray::Clear()
{
	JSON_LIST::iterator itJL;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		delete *itJL;
	}

	m_clsList.clear();
}

/**
 * @ingroup JsonParser
 * @brief 배열 자료구조에서 Element 인덱스에 해당하는 문자열 값을 검색한다.
 * @param iIndex		Element 인덱스
 * @param strValue	Element 값
 * @returns 검색에 성공하고 해당 값이 문자열 타입인 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::SelectData( int iIndex, std::string & strValue )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_STRING )
	{
		if( pclsType->m_cType != JSON_TYPE_NULL )
		{
			CLog::Print( LOG_ERROR, "%s type(%d) is not string (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		}

		return false;
	}

	strValue = ((CJsonString *)pclsType)->m_strValue;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief 배열 자료구조에서 Element 인덱스에 해당하는 정수 값을 검색한다.
 * @param iIndex Element 인덱스
 * @param iValue Element 값
 * @returns 검색에 성공하고 해당 값이 정수 타입인 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::SelectData( int iIndex, int64_t & iValue )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_INT )
	{
		if( pclsType->m_cType != JSON_TYPE_NULL )
		{
			CLog::Print( LOG_ERROR, "%s type(%d) is not int (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		}

		return false;
	}

	iValue = ((CJsonInt *)pclsType)->m_iValue;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief 배열 자료구조에서 Element 인덱스에 해당하는 boolean 값을 검색한다.
 * @param iIndex Element 인덱스
 * @param bValue Element 값
 * @returns 검색에 성공하고 해당 값이 boolean 타입인 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::SelectData( int iIndex, bool & bValue )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_BOOL )
	{
		if( pclsType->m_cType != JSON_TYPE_NULL )
		{
			CLog::Print( LOG_ERROR, "%s type(%d) is not bool (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		}

		return false;
	}

	bValue = ((CJsonBool *)pclsType)->m_bValue;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief 배열 자료구조에서 Element 인덱스에 해당하는 object 값을 검색한다.
 * @param iIndex			Element 인덱스
 * @param ppclsObject Element 값
 * @returns 검색에 성공하고 해당 값이 object 타입인 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::SelectData( int iIndex, CJsonObject ** ppclsObject )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_OBJECT )
	{
		if( pclsType->m_cType != JSON_TYPE_NULL )
		{
			CLog::Print( LOG_ERROR, "%s type(%d) is not object (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		}

		return false;
	}

	*ppclsObject = (CJsonObject *)pclsType;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief 배열 자료구조에서 Element 인덱스에 해당하는 배열 값을 검색한다.
 * @param iIndex			Element 인덱스
 * @param ppclsArray	Element 값
 * @returns 검색에 성공하고 해당 값이 배열 타입인 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::SelectData( int iIndex, CJsonArray ** ppclsArray )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_ARRAY )
	{
		if( pclsType->m_cType != JSON_TYPE_NULL )
		{
			CLog::Print( LOG_ERROR, "%s type(%d) is not array (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		}

		return false;
	}

	*ppclsArray = (CJsonArray *)pclsType;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief 배열 자료구조에서 Element 인덱스에 해당하는 값을 검색한다.
 * @param iIndex		Element 인덱스
 * @param ppclsType Element 값
 * @returns 검색에 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::SelectData( int iIndex, CJsonType ** ppclsType )
{
	if( iIndex >= (int)m_clsList.size() ) 
	{
		CLog::Print( LOG_ERROR, "%s iIndex(%d) >= m_clsList.size(%d)", __FUNCTION__, iIndex, m_clsList.size() );
		return false;
	}

	JSON_LIST::iterator itJL;
	int iCount = 0;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		if( iIndex == iCount )
		{
			*ppclsType = *itJL;
			return true;
		}

		++iCount;
	}

	return false;
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열에 문자열 Element 값을 추가한다.
 * @param strValue Element 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( std::string & strValue )
{
	return InsertData( strValue.c_str() );
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열에 문자열 Element 값을 추가한다.
 * @param pszValue Element 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( const char * pszValue )
{
	CJsonString * pclsNew = new CJsonString();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_strValue = pszValue;
	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열에 정수 Element 값을 추가한다.
 * @param iValue Element 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( int32_t iValue )
{
	return InsertData( (int64_t)iValue );
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열에 정수 Element 값을 추가한다.
 * @param iValue Element 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( int64_t iValue )
{
	CJsonInt * pclsNew = new CJsonInt();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_iValue = iValue;
	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열에 boolean Element 값을 추가한다.
 * @param bValue Element 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( bool bValue )
{
	CJsonBool * pclsNew = new CJsonBool();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_bValue = bValue;
	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열에 Element 값을 추가한다.
 * @param pclsType Element 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( CJsonType * pclsType )
{
	CJsonType * pclsNew = pclsType->Copy();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열에 null Element 값을 추가한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( )
{
	CJsonNull * pclsNew = new CJsonNull();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열의 지정된 위치에 문자열 Element 를 추가한다. 지정된 위치부터 element 는 한칸씩 뒤로 밀린다.
 * @param iIndex		문자열 Element 를 저장할 인덱스
 * @param strValue	문자열
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( int iIndex, std::string & strValue )
{
	return InsertData( iIndex, strValue.c_str() );
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열의 지정된 위치에 문자열 Element 를 추가한다. 지정된 위치부터 element 는 한칸씩 뒤로 밀린다.
 * @param iIndex		문자열 Element 를 저장할 인덱스
 * @param pszValue	문자열
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( int iIndex, const char * pszValue )
{
	CJsonString * pclsNew = new CJsonString();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_strValue = pszValue;

	if( InsertDataNoCopy( iIndex, pclsNew ) == false )
	{
		delete pclsNew;
		return false;
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열의 지정된 위치에 정수 Element 를 추가한다. 지정된 위치부터 element 는 한칸씩 뒤로 밀린다.
 * @param iIndex		정수 Element 를 저장할 인덱스
 * @param iValue		정수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( int iIndex, int32_t iValue )
{
	return InsertData( iIndex, (int64_t)iValue );
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열의 지정된 위치에 정수 Element 를 추가한다. 지정된 위치부터 element 는 한칸씩 뒤로 밀린다.
 * @param iIndex		정수 Element 를 저장할 인덱스
 * @param iValue		정수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( int iIndex, int64_t iValue )
{
	CJsonInt * pclsNew = new CJsonInt();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_iValue = iValue;

	if( InsertDataNoCopy( iIndex, pclsNew ) == false )
	{
		delete pclsNew;
		return false;
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열의 지정된 위치에 bool Element 를 추가한다. 지정된 위치부터 element 는 한칸씩 뒤로 밀린다.
 * @param iIndex		bool Element 를 저장할 인덱스
 * @param bValue		bool
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( int iIndex, bool bValue )
{
	CJsonBool * pclsNew = new CJsonBool();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_bValue = bValue;

	if( InsertDataNoCopy( iIndex, pclsNew ) == false )
	{
		delete pclsNew;
		return false;
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열의 지정된 위치에 Element 를 추가한다. 지정된 위치부터 element 는 한칸씩 뒤로 밀린다.
 * @param iIndex		Element 를 저장할 인덱스
 * @param pclsType	Element
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CJsonArray::InsertData( int iIndex, CJsonType * pclsType )
{
	CJsonType * pclsNew = pclsType->Copy();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	if( InsertDataNoCopy( iIndex, pclsNew ) == false )
	{
		delete pclsNew;
		return false;
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열의 지정된 위치에 null Element 를 추가한다. 지정된 위치부터 element 는 한칸씩 뒤로 밀린다.
 * @param iIndex		Element 를 저장할 인덱스
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CJsonArray::InsertDataNull( int iIndex )
{
	CJsonNull * pclsNew = new CJsonNull();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	if( InsertDataNoCopy( iIndex, pclsNew ) == false )
	{
		delete pclsNew;
		return false;
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief 입력한 JSON 배열의 element 들을 추가한다.
 * @param pclsArray JSON 배열
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CJsonArray::InsertArray( CJsonArray * pclsArray )
{
	int iCount = pclsArray->GetCount();
	CJsonType * pclsData;
		
	for( int i = 0; i < iCount; ++i )
	{
		if( pclsArray->SelectData( i, &pclsData ) )
		{
			InsertData( pclsData );
		}
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief 배열 자료구조에서 Element 인덱스에 해당하는 값을 삭제한다.
 * @param iIndex Element 인덱스
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CJsonArray::DeleteData( int iIndex )
{
	if( iIndex >= (int)m_clsList.size() ) 
	{
		CLog::Print( LOG_ERROR, "%s iIndex(%d) >= m_clsList.size(%d)", __FUNCTION__, iIndex, m_clsList.size() );
		return false;
	}

	JSON_LIST::iterator itJL;
	int iCount = 0;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		if( iIndex == iCount )
		{
			delete *itJL;
			m_clsList.erase( itJL );
			return true;
		}

		++iCount;
	}

	return false;
}

/**
 * @ingroup JsonParser
 * @brief JSON 배열에 Element 값을 추가한다.
 * @returns JSON 배열의 Element 개수를 리턴한다.
 */
int CJsonArray::GetCount()
{
	return (int)m_clsList.size();
}

/**
 * @ingroup JsonParser
 * @brief 입력된 값을 copy 하지 않고 JSON 배열의 지정된 위치에 Element 를 추가한다. 지정된 위치부터 element 는 한칸씩 뒤로 밀린다.
 * @param iIndex		Element 를 저장할 인덱스
 * @param pszValue	Element
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CJsonArray::InsertDataNoCopy( int iIndex, CJsonType * pclsType )
{
	int iSize = (int)m_clsList.size();

	if( iIndex > iSize )
	{
		CLog::Print( LOG_ERROR, "%s iIndex(%d) > m_clsList.size(%d)", __FUNCTION__, iIndex, iSize );
		return false;
	}

	if( iIndex == iSize )
	{
		m_clsList.push_back( pclsType );
		return true;
	}

	JSON_LIST::iterator itJL;
	int iCount = 0;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		if( iIndex == iCount )
		{
			m_clsList.insert( itJL, pclsType );
			return true;
		}

		++iCount;
	}

	return false;
}
