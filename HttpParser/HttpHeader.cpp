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
#include "HttpHeader.h"
#include "MemoryDebug.h"

CHttpHeader::CHttpHeader()
{
}

CHttpHeader::CHttpHeader( const char * pszName, const char * pszValue )
{
	Set( pszName, pszValue );
}

CHttpHeader::CHttpHeader( const char * pszName, std::string & strValue )
{
	Set( pszName, strValue );
}

CHttpHeader::~CHttpHeader()
{
}

/**
 * @ingroup HttpParser
 * @brief HTTP ��� ���ڿ��� �Ľ��Ͽ� CHttpHeader Ŭ������ ��� ������ �����Ѵ�.
 * @param pszText		HTTP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CHttpHeader::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iValuePos;
	char	cType = 0;

	// parse header name & skip prefix space of header value
	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( cType == 0 )
		{
			if( pszText[iPos] == ':' || pszText[iPos] == ' ' || pszText[iPos] == '\t' )
			{
				m_strName.append( pszText, iPos );
				++cType;
			}
			else if( pszText[iPos] == '\r' )
			{
				if( iPos + 1 >= iTextLen || pszText[iPos+1] != '\n' ) return -1;
				return iPos + 2;
			}
		}
		else if( pszText[iPos] == ':' || pszText[iPos] == ' ' || pszText[iPos] == '\t' )
		{
			continue;
		}
		else
		{
			break;
		}
	}

	iValuePos = iPos;

	// parse header value
	for( ; iPos < iTextLen; ++iPos )
	{
		if( cType == 10 )
		{
			if( pszText[iPos] != ' ' && pszText[iPos] != '\t' )
			{
				cType = 11;
				--iPos;
				iValuePos = iPos;
			}
		}
		else if( pszText[iPos] == '\r' )
		{
			if( iValuePos != -1 )
			{
				m_strValue.append( pszText + iValuePos, iPos - iValuePos );
				iValuePos = -1;
			}

			++iPos;
			if( iPos == iTextLen || pszText[iPos] != '\n' ) return -1;
			
			++iPos;
			if( iPos == iTextLen ) break;

			if( pszText[iPos] == ' ' || pszText[iPos] == '\t' )
			{
				cType = 10;
				m_strValue.append( " " );
			}
			else
			{
				break;
			}
		}
	}

	return iPos;
}

/**
 * @ingroup HttpParser
 * @brief HTTP �޽����� ���Ե� ���ڿ��� �ۼ��Ѵ�.
 * @param pszText		HTTP ����� ���� ������ ���ڿ� ����
 * @param iTextSize	pszText ������ ũ��
 * @returns �����ϸ� �ۼ��� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CHttpHeader::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	if( m_strValue.empty() )
	{
		return snprintf( pszText, iTextSize, "%s: \r\n", m_strName.c_str() );
	}
	else
	{
		return snprintf( pszText, iTextSize, "%s: %s\r\n", m_strName.c_str(), m_strValue.c_str() );
	}
}

/**
 * @ingroup HttpParser
 * @brief ����� �̸��� ���� �����Ѵ�.
 * @param pszName		��� �̸�
 * @param pszValue	��� ��
 */
void CHttpHeader::Set( const char * pszName, const char * pszValue )
{
	m_strName = pszName;
	m_strValue = pszValue;
}

/**
 * @ingroup HttpParser
 * @brief ����� �̸��� ���� �����Ѵ�.
 * @param pszName		��� �̸�
 * @param strValue	��� ��
 */
void CHttpHeader::Set( const char * pszName, std::string & strValue )
{
	m_strName = pszName;
	m_strValue = strValue;
}

/**
 * @ingroup HttpParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CHttpHeader::Clear()
{
	m_strName.clear();
	m_strValue.clear();
}
