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

/**
 * @ingroup WsdlParser
 * @brief WSDL ���ڿ��� �Ľ��Ͽ��� SOAP Ŭ���� �� �޼ҵ带 �����Ѵ�.
 * @param pszText		WSDL ���ڿ�
 * @param iTextLen	WSDL ���ڿ� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CWsdlMessage::Parse( const char * pszText, int iTextLen )
{
	if( m_clsRoot.Parse( pszText, iTextLen ) == -1 )
	{
		return false;
	}

	return ParseXml( );
}

/**
 * @ingroup WsdlParser
 * @brief WSDL ���ڿ��� �Ľ��Ͽ��� SOAP Ŭ���� �� �޼ҵ带 �����Ѵ�.
 * @param strText WSDL ���ڿ�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CWsdlMessage::Parse( std::string & strText )
{
	return Parse( strText.c_str(), (int)strText.length() );
}

/**
 * @ingroup WsdlParser
 * @brief WSDL ������ �о SOAP Ŭ���� �� �޼ҵ带 �����Ѵ�.
 * @param pszFileName WSDL ���� �̸�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CWsdlMessage::ParseFile( const char * pszFileName )
{
	if( m_clsRoot.ParseFile( pszFileName ) == false )
	{
		return false;
	}

	return ParseXml( );
}

/**
 * @ingroup WsdlParser
 * @brief SOAP Ŭ������ �����Ѵ�.
 * @returns SOAP Ŭ������ �����Ѵ�.
 */
CSoapClass * CWsdlMessage::GetSoap( )
{
	return &m_clsSoap;
}

/**
 * @ingroup WsdlParser
 * @brief XML ��ü�� ����� WSDL ������ �Ľ��Ͽ��� SOAP ��ü�� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CWsdlMessage::ParseXml( )
{
	std::string strName;

	if( !strcmp( m_clsRoot.GetName(), "wsdl:definitions" ) )
	{
		m_clsRoot.SelectAttribute( "targetNamespace", m_clsSoap.m_strNameSpace );

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
						GetMessageArgList( strMessage.c_str(), itMethod->second.m_clsInputList );
					}

					CXmlElement * pclsOutput = pclsOperation->SelectElement( "wsdl:output" );
					if( pclsOutput )
					{
						pclsOutput->SelectAttribute( "message", strMessage );
						GetMessageArgList( strMessage.c_str(), itMethod->second.m_clsOutputList );
					}
				}

				break;
			}
		}
	}
	else
	{
		// WSDL 2.0 ������ �������� �ʴ´�.
		return false;
	}

	return true;
}

/**
 * @ingroup WsdlParser
 * @brief wsdl:message ����Ʈ���� �Էµ� �̸��� ������ �׸� ���� ���� ����Ʈ�� �����´�.
 * @param pszName			wsdl:message �̸�
 * @param clsArgList	���� ����Ʈ�� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CWsdlMessage::GetMessageArgList( const char * pszName, SOAP_ARG_LIST & clsArgList )
{
	std::string strName, strElement;
	bool bRes = false;
	const char * pszPos = strstr( pszName, ":" );

	if( pszPos )
	{
		pszName = pszPos + 1;
	}

	for( int iMessage = 0; ; ++iMessage )
	{
		CXmlElement * pclsMessage = m_clsRoot.SelectElement( "wsdl:message", iMessage );
		if( pclsMessage == NULL ) break;

		pclsMessage->SelectAttribute( "name", strName );
		if( !strcmp( strName.c_str(), pszName ) )
		{
			CXmlElement * pclsPart = pclsMessage->SelectElement( "wsdl:part" );
			if( pclsPart )
			{
				pclsPart->SelectAttribute( "element", strElement );
				if( strElement.empty() == false )
				{
					GetTypeArgList( strElement.c_str(), clsArgList );
					bRes = true;
				}
			}

			break;
		}
	}

	return bRes;
}

/**
 * @ingroup WsdlParser
 * @brief wsdl:types ���� �Է��� �̸��� Ÿ�Կ� ���� ���� ����Ʈ�� �����´�.
 * @param pszName			Ÿ�� �̸�
 * @param clsArgList	���� ����Ʈ�� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CWsdlMessage::GetTypeArgList( const char * pszName, SOAP_ARG_LIST & clsArgList )
{
	std::string strName, strType;
	bool bRes = false;
	const char * pszPos = strstr( pszName, ":" );

	if( pszPos )
	{
		pszName = pszPos + 1;
	}

	CXmlElement * pclsType = m_clsRoot.SelectElement( "wsdl:types" );
	if( pclsType == NULL ) return false;

	CXmlElement * pclsSchema = pclsType->SelectElement( "s:schema" );
	if( pclsSchema == NULL ) return false;

	for( int iElement = 0; ; ++iElement )
	{
		CXmlElement * pclsElement = pclsSchema->SelectElement( "s:element", iElement );
		if( pclsElement == NULL ) break;

		pclsElement->SelectAttribute( "name", strName );
		if( !strcmp( strName.c_str(), pszName ) )
		{
			CXmlElement * pclsComplexType = pclsElement->SelectElement( "s:complexType" );
			if( pclsComplexType == NULL ) break;

			CXmlElement * pclsSequence = pclsComplexType->SelectElement( "s:sequence" );
			if( pclsSequence == NULL ) break;

			for( int i = 0; ; ++i )
			{
				CXmlElement * pclsArg = pclsSequence->SelectElement( "s:element", i );
				if( pclsArg == NULL ) break;

				CSoapArg clsArg;

				pclsArg->SelectAttribute( "name", clsArg.m_strName );
				pclsArg->SelectAttribute( "type", strType );

				clsArg.SetType( strType.c_str() );

				clsArgList.push_back( clsArg );

				bRes = true;
			}

			break;
		}
	}

	return bRes;
}
