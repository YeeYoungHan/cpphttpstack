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

#ifndef _HTML_ELEMENT_URL_H_
#define _HTML_ELEMENT_URL_H_

#include "HtmlElement.h"
#include "StringUtility.h"

enum EHtmlUrlType
{
	E_HUT_NULL		= 0x00,
	E_HUT_LINK		= 0x01,
	E_HUT_SCRIPT	= 0x02,
	E_HUT_IMG			= 0x04
};

class CHtmlElementUrl : public CHtmlElement
{
public:
	CHtmlElementUrl();
	~CHtmlElementUrl();

	bool GetUrlList( STRING_LIST & clsUrlList );
	void SetType( int iType );

private:
	bool GetUrlList( HTML_ELEMENT_LIST & clsElementList, STRING_LIST & clsUrlList );

	int m_iType;
};

#endif
