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

bool CHtmlElementUrl::GetUrlList( STRING_LIST & clsUrlList )
{
	clsUrlList.clear();

	GetUrlList( m_clsElementList, clsUrlList );

	if( clsUrlList.empty() ) return false;

	return true;
}

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
				if( itEL->SelectAttributeTrim( "href", strUrl ) )
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
				if( itEL->SelectAttributeTrim( "src", strUrl ) )
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
				if( itEL->SelectAttributeTrim( "src", strUrl ) )
				{
					clsUrlList.push_back( strUrl );
				}

				continue;
			}
		}
	}

	return true;
}

void CHtmlElementUrl::SetType( int iType )
{
	m_iType = iType;
}
