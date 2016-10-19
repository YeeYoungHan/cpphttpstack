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

#include "WsdlMessage.h"

CWsdlMessage::CWsdlMessage()
{
}

CWsdlMessage::~CWsdlMessage()
{
}

bool CWsdlMessage::Parse( const char * pszText, int iTextLen )
{
	std::string strName;

	if( m_clsRoot.Parse( pszText, iTextLen ) == -1 )
	{
		return false;
	}

	if( !strcmp( m_clsRoot.GetName(), "wsdl:definitions" ) )
	{
		for( int iBinding = 0; ; ++iBinding )
		{
			CXmlElement * pclsBinding = m_clsRoot.SelectElement( "wsdl:binding", iBinding );
			if( pclsBinding == NULL ) break;

			if( pclsBinding->SelectElement( "soap:binding" ) )
			{
				pclsBinding->SelectAttribute( "name", m_clsSoap.m_strName );

				for( int iOperation = 0; ; ++iOperation )
				{
					CXmlElement * pclsOperation = pclsBinding->SelectElement( "wsdl:operation", iOperation );
					if( pclsOperation == NULL ) break;

					CXmlElement * pclsSoapOperation = pclsOperation->SelectElement( "soap:operation" );
					if( pclsSoapOperation == NULL ) break;

					CSoapMethod clsMethod;

					pclsOperation->SelectAttribute( "name", clsMethod.m_strName );
					pclsSoapOperation->SelectAttribute( "soapAction", clsMethod.m_strSoapAction );

					m_clsSoap.m_clsMethodMap.insert( SOAP_METHOD_MAP::value_type( clsMethod.m_strName, clsMethod ) );
				}

				break;
			}
		}

		CXmlElement * pclsService = m_clsRoot.SelectElement( "wsdl:service" );
		if( pclsService )
		{
			for( int iPort = 0; ; ++iPort )
			{
				CXmlElement * pclsPort = pclsService->SelectElement( "wsdl:port", iPort );
				if( pclsPort == NULL ) break;

				pclsPort->SelectAttribute( "name", strName );
				if( !strcmp( strName.c_str(), m_clsSoap.m_strName.c_str() ) )
				{
					CXmlElement * pclsAddress = pclsPort->SelectElement( "soap:address" );
					if( pclsAddress )
					{
						pclsAddress->SelectAttribute( "location", m_clsSoap.m_strUrl );
					}

					break;
				}
			}
		}

		for( int iPortType = 0; ; ++iPortType )
		{
			CXmlElement * pclsPortType = m_clsRoot.SelectElement( "wsdl:portType", iPortType );
			if( pclsPortType == NULL ) break;

			pclsPortType->SelectAttribute( "name", strName );
			if( !strcmp( strName.c_str(), m_clsSoap.m_strName.c_str() ) )
			{
				std::string strMessage;
				SOAP_METHOD_MAP::iterator itMethod;

				for( int iOperation = 0; ; ++iOperation )
				{
					CXmlElement * pclsOperation = pclsPortType->SelectElement( "wsdl:operation", iOperation );
					if( pclsOperation == NULL ) break;

					pclsOperation->SelectAttribute( "name", strName );
					itMethod = m_clsSoap.m_clsMethodMap.find( strName );
					if( itMethod == m_clsSoap.m_clsMethodMap.end() ) continue;
					
					CXmlElement * pclsInput = pclsOperation->SelectElement( "wsdl:input" );
					if( pclsInput )
					{
						pclsInput->SelectAttribute( "message", strMessage );
					}

					CXmlElement * pclsOutput = pclsOperation->SelectElement( "wsdl:output" );
					if( pclsOutput )
					{
						pclsInput->SelectAttribute( "message", strMessage );
					}
				}

				break;
			}
		}
	}
	else
	{
		// WSDL 2.0 버전은 지원하지 않는다.
		return false;
	}

	return true;
}
