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

#include "HttpSimulatorSetup.h"
#include "XmlElement.h"
#include "Log.h"

CHttpSimulatorSetup::CHttpSimulatorSetup() : m_iLoopCount(1)
{
}

CHttpSimulatorSetup::~CHttpSimulatorSetup()
{
}

/**
 * @ingroup HttpSimulator
 * @brief ���� ������ �о ��� ������ �����Ѵ�.
 * @param pszFileName ���� ���� (full path)
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CHttpSimulatorSetup::Read( const char * pszFileName )
{
	CXmlElement clsXml, *pclsElement;
	XML_ELEMENT_LIST clsList;
	XML_ELEMENT_LIST::iterator itEL;
	std::string strTemp;

	if( clsXml.ParseFile( pszFileName ) == false )
	{
		CLog::Print( LOG_ERROR, "%s clsXml.ParseFile(%s) error", __FUNCTION__, pszFileName );
		return false;
	}

	// ����
	pclsElement = clsXml.SelectElement( "Setup" );
	if( pclsElement )
	{
		pclsElement->SelectElementData( "LoopCount", m_iLoopCount );
		if( m_iLoopCount <= 0 ) m_iLoopCount = 1;
	}

	// ���
	pclsElement = clsXml.SelectElement( "CommandList" );
	if( pclsElement )
	{
		pclsElement->SelectElementList( "Command", clsList );

		for( itEL = clsList.begin(); itEL != clsList.end(); ++itEL )
		{
			CHttpSimulatorCommand clsCommand;

			itEL->SelectElementTrimData( "Method", clsCommand.m_strMethod );
			itEL->SelectElementTrimData( "Url", clsCommand.m_strUrl );
			itEL->SelectElementTrimData( "Body", clsCommand.m_strBody );
			itEL->SelectElementTrimData( "RecvFileName", clsCommand.m_strRecvFileName );
			itEL->SelectElementTrimData( "BodyExecute", clsCommand.m_strBodyExecute );
			itEL->SelectElementData( "DownloadAll", clsCommand.m_bDownloadAll );

			if( !strcasecmp( clsCommand.m_strMethod.c_str(), "post" ) )
			{
				clsCommand.m_strMethod = "POST";

				if( clsCommand.m_strBody.empty() && clsCommand.m_strBodyExecute.empty() )
				{
					CLog::Print( LOG_ERROR, "%s Command(%s) POST body not found", __FUNCTION__, clsCommand.m_strUrl.c_str() );
				}
			}
			else
			{
				clsCommand.m_strMethod = "GET";
			}

			m_clsCommandList.push_back( clsCommand );
		}
	}

	if( m_clsCommandList.empty() )
	{
		printf( "[SetupFile] CommandList is empty\n" );
		return false;
	}

	// HTTP ��������
	pclsElement = clsXml.SelectElement( "Http" );
	if( pclsElement )
	{
		pclsElement->SelectElementTrimData( "UserAgent", m_strUserAgent );
	}

	// �α�
	pclsElement = clsXml.SelectElement( "Log" );
	if( pclsElement )
	{
		if( pclsElement->SelectElementTrimData( "Folder", strTemp ) )
		{
			if( CLog::SetDirectory( strTemp.c_str() ) == false )
			{
				printf( "[SetupFile] CLog::SetDirectory(%s) error\n", strTemp.c_str() );
				return false;
			}
		}

		int iLogLevel = 0;

		CXmlElement * pclsClient = pclsElement->SelectElement( "Level" );
		if( pclsClient )
		{
			bool bTemp;

			pclsClient->SelectAttribute( "Debug", bTemp );
			if( bTemp ) iLogLevel |= LOG_DEBUG;

			pclsClient->SelectAttribute( "Info", bTemp );
			if( bTemp ) iLogLevel |= LOG_INFO;

			pclsClient->SelectAttribute( "Network", bTemp );
			if( bTemp ) iLogLevel |= LOG_NETWORK;

			pclsClient->SelectAttribute( "HttpHeader", bTemp );
			if( bTemp ) iLogLevel |= LOG_HTTP_HEADER;
		}

		int			iLogMaxSize = 0;
		int64_t iLogMaxFolderSize = 0;

		pclsElement->SelectElementData( "MaxSize", iLogMaxSize );
		pclsElement->SelectElementData( "MaxFolderSize", iLogMaxFolderSize );

		CLog::SetLevel( iLogLevel );
		CLog::SetMaxLogSize( iLogMaxSize );
		CLog::SetMaxFolderSize( iLogMaxFolderSize );
	}

	return true;
}
