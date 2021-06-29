/* 
 * Copyright (C) 2021 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
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

#include "HtmlElementUrl.h"

CHtmlElementUrl::CHtmlElementUrl() : m_iType( E_HUT_LINK | E_HUT_SCRIPT | E_HUT_IMG )
{
}

CHtmlElementUrl::~CHtmlElementUrl()
{
}

/**
 * @ingroup HtmlParser
 * @brief URL 리스트를 가져온다.
 * @param clsUrlList [out] URL 리스트
 * @returns URL 이 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHtmlElementUrl::GetUrlList( STRING_LIST & clsUrlList )
{
	clsUrlList.clear();

	GetUrlList( m_clsElementList, clsUrlList );

	if( clsUrlList.empty() ) return false;

	return true;
}

/**
 * @ingroup HtmlParser
 * @brief elementlist 에 저장된 URL 리스트를 가져온다.
 * @param clsElementList	element list
 * @param clsUrlList			[out] URL 리스트
 * @returns true 를 리턴한다.
 */
bool CHtmlElementUrl::GetUrlList( HTML_ELEMENT_LIST & clsElementList, STRING_LIST & clsUrlList )
{
	HTML_ELEMENT_LIST::iterator itEL;
	const char * pszName;
	std::string strUrl;

	for( itEL = clsElementList.begin(); itEL != clsElementList.end(); ++itEL )
	{
		pszName = itEL->GetName();

		GetUrlList( *(itEL->GetElementList()), clsUrlList );

		if( m_iType & E_HUT_LINK )
		{
			if( !strcasecmp( pszName, "link" ) )
			{
				if( itEL->SelectAttributeTrim( "href", strUrl ) && strUrl.empty() == false )
				{
					clsUrlList.push_back( strUrl );
				}

				continue;
			}
		}

		if( m_iType & E_HUT_SCRIPT )
		{
			if( !strcasecmp( pszName, "script" ) )
			{
				if( itEL->SelectAttributeTrim( "src", strUrl ) && strUrl.empty() == false )
				{
					clsUrlList.push_back( strUrl );
				}

				continue;
			}
		}

		if( m_iType & E_HUT_IMG )
		{
			if( !strcasecmp( pszName, "img" ) )
			{
				if( itEL->SelectAttributeTrim( "src", strUrl ) && strUrl.empty() == false )
				{
					clsUrlList.push_back( strUrl );
				}

				continue;
			}
		}
	}

	return true;
}

/**
 * @ingroup HtmlParser
 * @brief URL 리스트에 저장할 타입을 설정한다.
 * @param iType E_HUT_LINK, E_HUT_SCRIPT, E_HUT_IMG 를 | 연산으로 묶어서 입력한다.
 */
void CHtmlElementUrl::SetType( int iType )
{
	m_iType = iType;
}
