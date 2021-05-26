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

#include "Http2Conversion.h"
#include "Log.h"

CHttp2Conversion::CHttp2Conversion()
{
}

CHttp2Conversion::~CHttp2Conversion()
{
}

bool CHttp2Conversion::MakeFrameList( CHttpMessage & clsMessage, CHttp2FrameList & clsFrameList )
{
	CHttp2Frame * pclsFrame;

	pclsFrame = clsFrameList.CreateFrame();
	if( pclsFrame == NULL )
	{
		return false;
	}

	if( clsMessage.m_strHttpMethod.empty() == false )
	{
		const char * pszMethod = clsMessage.m_strHttpMethod.c_str();
		if( !strcasecmp( pszMethod, "GET" ) )
		{
			
		}
		else if( !strcasecmp( pszMethod, "POST" ) )
		{

		}
	}

	return true;
}

bool CHttp2Conversion::MakeMessage( CHttp2FrameList & clsFrameList, CHttpMessage & clsMessage )
{

	return true;
}
