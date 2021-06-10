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

#ifndef _HTTP2_CONVERSION_H_
#define _HTTP2_CONVERSION_H_

#include "HttpMessage.h"
#include "Http2Header.h"
#include "Http2FrameList.h"
#include "Http2HpackHeader.h"

class CHttp2Conversion
{
public:
	CHttp2Conversion();
	~CHttp2Conversion();

	bool MakeFrameList( CHttpMessage & clsMessage, CHttp2FrameList & clsFrameList );
	bool MakeMessage( CHttp2Frame & clsFrame, CHttpMessage & clsMessage );

private:
	bool AddIndex( uint32_t iIndex );
	bool AddIndexValue( uint32_t iIndex, const char * pszValue );
	bool AddNameValue( const char * pszName, const char * pszValue );
	bool HpackToMessage( CHttp2HpackHeader & clsHpack, CHttpMessage & clsMessage );
	void HpackToString( CHttp2HpackHeader & clsHpack, std::string & strOutput, const char * pszDefault );
	void HpackToInt( CHttp2HpackHeader & clsHpack, int & iOutput, int iDefault );
	void HpackAddHeader( CHttp2HpackHeader & clsHpack, CHttpMessage & clsMessage, const char * pszName );

	CHttpMessage		* m_pclsMessage;
	CHttp2FrameList * m_pclsFrameList;
	CHttp2Header	m_clsHeader;
	int						m_iHeaderFrameCount;
};

#endif
