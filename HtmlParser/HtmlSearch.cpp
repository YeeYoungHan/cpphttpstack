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

bool CHtmlSearch::SelectClassElementList( const char * pszClassName, HTML_ELEMENT_LIST & clsList )
{
	clsList.clear();

	SelectClassElementList( *this, pszClassName, clsList );

	if( clsList.empty() == false ) return true;

	return false;
}

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
