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

#include "SipUdp.h"
#include "Setup.h"
#include "XmlElement.h"
#include "Log.h"

CSetup gclsSetup;

CSetup::CSetup()
{
}

CSetup::~CSetup()
{
}

/**
 * @ingroup TestJsonServer
 * @brief 설정 파일을 읽어서 멤버 변수에 저장한다.
 * @param pszFileName 설정 파일 full path
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSetup::Read( const char * pszFileName )
{
	CXmlElement clsXml;

	if( clsXml.ParseFile( pszFileName ) == false )
	{
		CLog::Print( LOG_ERROR, "%s clsXml.ParseFile(%s) error", __FUNCTION__, pszFileName );
		return false;
	}

	CXmlElement * pclsElement = clsXml.SelectElement( "ReqUriPathList" );
	if( pclsElement == NULL )
	{
		CLog::Print( LOG_ERROR, "%s ReqUriPathList is not found", __FUNCTION__ );
		return false;
	}

	XML_ELEMENT_LIST clsList;
	XML_ELEMENT_LIST::iterator itList;

	pclsElement->SelectElementList( "ReqUriPath", clsList );

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		std::string strReqUri, strPath;

		itList->SelectElementTrimData( "ReqUri", strReqUri );
		itList->SelectElementTrimData( "Path", strPath );

		if( strReqUri.empty() == false && strPath.empty() == false )
		{
			m_clsPathMap.Insert( strReqUri.c_str(), strPath.c_str() );
		}
	}

	return true;
}
