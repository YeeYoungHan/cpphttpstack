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

#include "HtmlSearch.h"

CHtmlSearch::CHtmlSearch()
{
}

CHtmlSearch::~CHtmlSearch()
{
}

/**
 * @ingroup HtmlParser
 * @brief 클래스 이름으로 저장된 HTML element 리스트를 생성한다.
 * @param pszClassName	클래스 이름
 * @param clsList				[out] 클래스 이름으로 저장된 HTML element 리스트
 * @returns 클래스 이름으로 저장된 HTML element 가 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHtmlSearch::SelectClassElementList( const char * pszClassName, HTML_ELEMENT_LIST & clsList )
{
	clsList.clear();

	SelectClassElementList( *this, pszClassName, clsList );

	if( clsList.empty() == false ) return true;

	return false;
}

/**
 * @ingroup HtmlParser
 * @brief 클래스 이름으로 저장된 HTML element 리스트를 생성한다.
 * @param clsHtml				HTML element
 * @param pszClassName	클래스 이름
 * @param clsList				[out] 클래스 이름으로 저장된 HTML element 리스트
 */
void CHtmlSearch::SelectClassElementList( CHtmlElement & clsHtml, const char * pszClassName, HTML_ELEMENT_LIST & clsList )
{
	HTML_ELEMENT_LIST::iterator	itEL;
	HTML_ELEMENT_LIST * pclsElementList = clsHtml.GetElementList();

	for( itEL = pclsElementList->begin(); itEL != pclsElementList->end(); ++itEL )
	{
		if( itEL->IsClass( pszClassName ) )
		{
			clsList.push_back( *itEL );
		}

		SelectClassElementList( *itEL, pszClassName, clsList );
	}
}
