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

#include "HttpMultipart.h"
#include "HttpHeader.h"
#include "HttpParameterList.h"
#include "MemoryDebug.h"

CHttpMultipart::CHttpMultipart()
{
}

CHttpMultipart::~CHttpMultipart()
{
	Clear();
}

/**
 * @ingroup HttpParser
 * @brief multipart 의 boundary 문자열을 저장한다.
 * @param pszBoundary multipart 의 boundary 문자열
 */
void CHttpMultipart::SetBoundary( const char * pszBoundary )
{
	m_strBoundary = "--";
	m_strBoundary.append( pszBoundary );
}

/**
 * @ingroup HttpParser
 * @brief Content-Type 에서 boundary 문자열을 찾아서 boundary 문자열을 저장한다.
 * @param pszContentType Content-Type 문자열
 */
void CHttpMultipart::SetContentType( const char * pszContentType )
{
	CHttpParameterList clsParamList( ';', true );

	if( clsParamList.Parse( pszContentType ) != -1 )
	{
		std::string strBoundary;

		if( clsParamList.Select( "boundary", strBoundary ) )
		{
			SetBoundary( strBoundary.c_str() );
		}
	}
}

/**
 * @ingroup HttpParser
 * @brief 자료구조를 초기화시킨다.
 */
void CHttpMultipart::Clear()
{
	HTTP_MULTIPART_DATA_MAP::iterator itMap;

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		delete itMap->second;
	}

	m_clsMap.clear();
}

/**
 * @ingroup HttpParser
 * @brief multipart 문자열을 파싱한다.
 * @param strText multipart 문자열
 * @returns 성공하면 파싱한 multipart 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CHttpMultipart::Parse( const std::string & strText )
{
	return Parse( strText.c_str(), (int)strText.length() );
}

/**
 * @ingroup HttpParser
 * @brief multipart 문자열을 파싱한다.
 * @param pszText		multipart 문자열
 * @param iTextLen	multipart 문자열 길이
 * @returns 성공하면 파싱한 multipart 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CHttpMultipart::Parse( const char * pszText, int iTextLen )
{
	const char * pszBoundary = m_strBoundary.c_str();
	int iBoundaryLen = (int)m_strBoundary.length();
	int iStartPos = -1, iEndPos;

	Clear();

	for( int i = 0; i < iTextLen; )
	{
		if( pszText[i] != '-' || strncmp( pszText + i, pszBoundary, iBoundaryLen ) )
		{
			++i;
			continue;
		}

		if( iStartPos != -1 )
		{
			// \r\n 를 제거하기 위해서 -2 를 추가함
			iEndPos = i - 2;
			if( ParseData( pszText + iStartPos, iEndPos - iStartPos ) == -1 )
			{
				return -1;
			}
		}

		i += iBoundaryLen;

		if( ( i + 4 ) > iTextLen )
		{
			break;
		}

		if( pszText[i] == '-' && pszText[i+1] == '-' && pszText[i+2] == '\r' && pszText[i+3] == '\n' )
		{
			return i + 4;
		}
		
		if( pszText[i] != '\r' || pszText[i+1] != '\n' )
		{
			break;
		}

		iStartPos = i + 2;
	}

	return -1;
}

/**
 * @ingroup HttpParser
 * @brief multipart 문자열을 생성한다.
 * @param strText [out] multipart 문자열
 * @returns multipart 문자열 길이를 리턴한다.
 */
int CHttpMultipart::ToString( std::string & strText )
{
	HTTP_MULTIPART_DATA_MAP::iterator itMap;

	strText.clear();

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strText.append( m_strBoundary );
		strText.append( "\r\n" );
		strText.append( "Content-Disposition: form-data; name=\"" );
		strText.append( itMap->first );
		strText.append( "\"" );

		if( itMap->second->m_strFileName.empty() == false )
		{
			strText.append( "; filename=\"" );
			strText.append( itMap->second->m_strFileName );
			strText.append( "\"" );
		}

		strText.append( "\r\n" );

		if( itMap->second->m_strContentType.empty() == false )
		{
			strText.append( "Content-Type: " );
			strText.append( itMap->second->m_strContentType );
			strText.append( "\r\n" );
		}

		strText.append( "\r\n" );
		strText.append( itMap->second->m_strValue );
		strText.append( "\r\n" );
	}

	strText.append( m_strBoundary );
	strText.append( "--" );
	strText.append( "\r\n" );

	return (int)strText.length();
}

/**
 * @ingroup HttpParser
 * @brief multipart 에 포함된 하나의 항목을 form-data 로 파싱한다.
 * @param pszText		문자열
 * @param iTextLen	문자열 길이
 * @returns 정상적으로 파싱되면 파싱한 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CHttpMultipart::ParseData( const char * pszText, int iTextLen )
{
	CHttpMultipartData * pclsData = new CHttpMultipartData();
	if( pclsData == NULL ) return -1;

	std::string strName;
	CHttpHeader clsHeader;
	int iPos, iCurPos = 0;

	while( 1 )
	{
		iPos = clsHeader.Parse( pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) break;

		iCurPos += iPos;
		if( iPos == 2 ) break;

		if( !strcmp( clsHeader.m_strName.c_str(), "Content-Disposition" ) )
		{
			CHttpParameterList clsParamList( ';', true );

			if( clsParamList.Parse( clsHeader.m_strValue ) != -1 )
			{
				std::string strTemp;

				if( clsParamList.Select( "name", strTemp ) )
				{
					DeQuoteString( strTemp, strName );
				}

				if( clsParamList.Select( "filename", strTemp ) )
				{
					DeQuoteString( strTemp, pclsData->m_strFileName );
				}
			}
		}
		else if( !strcmp( clsHeader.m_strName.c_str(), "Content-Type" ) )
		{
			pclsData->m_strContentType = clsHeader.m_strValue;
		}
	}

	if( strName.empty() ) return -1;

	pclsData->m_strValue.append( pszText + iCurPos, iTextLen - iCurPos );

	m_clsMap.insert( HTTP_MULTIPART_DATA_MAP::value_type( strName, pclsData ) );

	return iCurPos;
}
