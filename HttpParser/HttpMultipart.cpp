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

CHttpMultipart::CHttpMultipart()
{
}

CHttpMultipart::~CHttpMultipart()
{
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
 * @param pszText		multipart 문자열
 * @param iTextLen	multipart 문자열 길이
 * @returns 성공하면 파싱한 multipart 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CHttpMultipart::Parse( const char * pszText, int iTextLen )
{
	const char * pszBoundary = m_strBoundary.c_str();
	const char * pszPos;
	int iBoundaryLen = (int)m_strBoundary.length();
	int iStartPos = -1, iEndPos;

	Clear();

	for( int i = 0; i < iTextLen; )
	{
		pszPos = strstr( pszText + i, pszBoundary );
		if( pszPos == NULL )
		{
			break;
		}

		if( iStartPos != -1 )
		{
			// \r\n 를 제거하기 위해서 -2 를 추가함
			iEndPos = pszPos - pszText - 2;
			if( ParseData( pszText + iStartPos, iEndPos - iStartPos ) == -1 )
			{
				return -1;
			}
		}

		pszPos += iBoundaryLen;
		i = pszPos - pszText;

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
		//strText.append( *itSL );
		strText.append( "\r\n" );
	}

	strText.append( m_strBoundary );
	strText.append( "--" );
	strText.append( "\r\n" );

	return strText.length();
}

int CHttpMultipart::ParseData( const char * pszText, int iTextLen )
{
	CHttpMultipartData * pclsData = new CHttpMultipartData();
	if( pclsData == NULL ) return -1;

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

		}
		else if( !strcmp( clsHeader.m_strName.c_str(), "Content-Type" ) )
		{
			pclsData->m_strContentType = clsHeader.m_strValue;
		}
	}

	pclsData->m_strValue.append( pszText + iCurPos, iTextLen - iCurPos );

	m_clsMap.insert( HTTP_MULTIPART_DATA_MAP::value_type( "", pclsData ) );

	return iCurPos;
}
