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

#include "SipPlatformDefine.h"
#include "SoapClass.h"

CSoapMethod::CSoapMethod()
{
}

CSoapMethod::~CSoapMethod()
{
}

/**
 * @ingroup WsdlParser
 * @brief 헤더 파일과 소스 파일에 저장할 메소드 선언문을 생성한다.
 * @param pszClassName	클래스 이름
 * @param strCode				소스 코드 출력용 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSoapMethod::GetPrototypeCode( const char * pszClassName, std::string & strCode )
{
	SOAP_ARG_LIST::iterator itAL;
	std::string strArg;
	int iArgCount = 0;

	strCode = "bool ";

	if( pszClassName && strlen( pszClassName ) > 0 )
	{
		strCode.append( pszClassName );
		strCode.append( "::" );
	}

	strCode.append( m_strName );
	strCode.append( "( " );

	for( itAL = m_clsInputList.begin(); itAL != m_clsInputList.end(); ++itAL )
	{
		if( itAL->GetCode( strArg, true ) == false ) return false;

		if( iArgCount > 0 ) strCode.append( ", " );
		strCode.append( strArg );
		++iArgCount;
	}

	for( itAL = m_clsOutputList.begin(); itAL != m_clsOutputList.end(); ++itAL )
	{
		if( itAL->GetCode( strArg, false ) == false ) return false;

		if( iArgCount > 0 ) strCode.append( ", " );
		strCode.append( strArg );
		++iArgCount;
	}

	if( iArgCount > 0 ) strCode.append( " " );
	strCode.append( ")" );

	return true;
}

/**
 * @ingroup WsdlParser
 * @brief 소스 파일에 메소드 소스 코드를 저장한다.
 * @param pszClassName	클래스 이름
 * @param pclsSoap			메소드를 포함한 SoapClass 객체
 * @param strCode				소스 코드 출력용 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSoapMethod::GetCode( const char * pszClassName, CSoapClass * pclsSoap, std::string & strCode )
{
	SOAP_ARG_LIST::iterator itAL;
	std::string strVar;

	if( GetPrototypeCode( pszClassName, strCode ) == false ) return false;

	strCode.append( "\n" );
	strCode.append( "{\n" );

	strCode.append( "\tstd::string strSendBody, strRecvBody;\n" );
	strCode.append( "\tCHttpClient clsClient;\n" );
	strCode.append( "\tCXmlElement clsXml;\n" );
	strCode.append( "\n" );

	strCode.append( "\tclsXml.SetName( \"soapenv:Envelope\" );\n" );
	strCode.append( "\tclsXml.InsertAttribute( \"xmlns:soapenv\", \"http://schemas.xmlsoap.org/soap/envelope/\" );\n" );
	
	strCode.append( "\tclsXml.InsertAttribute( \"xmlns:tns\", \"" );
	strCode.append( pclsSoap->m_strNameSpace );
	strCode.append( "\" );\n" );
	
	strCode.append( "\tclsXml.InsertElementData( \"soapenv:Header\", \"\" );\n" );
	strCode.append( "\n" );

	strCode.append( "\tCXmlElement clsBody, clsMethod;\n" );
	strCode.append( "\n" );
	strCode.append( "\tclsBody.SetName( \"soapenv:Body\" );\n" );

	strCode.append( "\tclsMethod.SetName( \"tns:" );
	strCode.append( m_strName );
	strCode.append( "\" );\n" );

	for( itAL = m_clsInputList.begin(); itAL != m_clsInputList.end(); ++itAL )
	{
		strCode.append( "\tclsMethod.InsertElementData( \"tns:" );
		strCode.append( itAL->m_strName );
		strCode.append( "\", " );
		
		itAL->GetVariable( strVar, true );
		strCode.append( strVar );
		strCode.append( " );\n" );
	}

	strCode.append( "\tclsBody.InsertElement( &clsMethod );\n" );
	strCode.append( "\tclsXml.InsertElement( &clsBody );\n" );
	strCode.append( "\tclsXml.ToString( strSendBody );\n" );
	strCode.append( "\n" );

	strCode.append( "\tif( clsClient.DoSoap( \"" );
	strCode.append( pclsSoap->m_strUrl );
	strCode.append( "\", \"" );
	strCode.append( m_strSoapAction );
	strCode.append( "\", strSendBody.c_str(), strRecvBody ) == false )\n" );
	strCode.append( "\t{\n" );
	strCode.append( "\t\tCLog::Print( LOG_ERROR, \"%s DoSoap error\", __FUNCTION__ );\n" );
	strCode.append( "\t\treturn false;\n" );
	strCode.append( "\t}\n" );
	strCode.append( "\n" );

	strCode.append( "\tif( clsXml.Parse( strRecvBody ) == -1 )\n" );
	strCode.append( "\t{\n" );
	strCode.append( "\t\tCLog::Print( LOG_ERROR, \"%s Parse error\", __FUNCTION__ );\n" );
	strCode.append( "\t\treturn false;\n" );
	strCode.append( "\t}\n" );
	strCode.append( "\n" );

	strCode.append( "\tstd::string strPrefix, strBodyName;\n" );
	strCode.append( "\tconst char * pszName = clsXml.GetName();\n" );
	strCode.append( "\tconst char * pszPos = strstr( pszName, \":\" );\n" );
	strCode.append( "\tif( pszPos )\n" );
	strCode.append( "\t{\n" );
	strCode.append( "\t\tstrPrefix.append( pszName, pszPos - pszName );\n" );
	strCode.append( "\t}\n" );
	strCode.append( "\n" );

	strCode.append( "\tstrBodyName = strPrefix + \":\" + \"Body\";\n" );
	
	strCode.append( "\tCXmlElement * pclsBody = clsXml.SelectElement( strBodyName.c_str() );\n" );
	strCode.append( "\tif( pclsBody == NULL )\n" );
	strCode.append( "\t{\n" );
	strCode.append( "\t\tCLog::Print( LOG_ERROR, \"%s Body not found\", __FUNCTION__ );\n" );
	strCode.append( "\t\treturn false;\n" );
	strCode.append( "\t}\n" );
	strCode.append( "\n" );

	strCode.append( "\tCXmlElement * pclsArgList = pclsBody->SelectElement( 0 );\n" );
	strCode.append( "\tif( pclsArgList == NULL )\n" );
	strCode.append( "\t{\n" );
	strCode.append( "\t\tCLog::Print( LOG_ERROR, \"%s Body ArgList not found\", __FUNCTION__ );\n" );
	strCode.append( "\t\treturn false;\n" );
	strCode.append( "\t}\n" );
	strCode.append( "\n" );

	strCode.append( "\tfor( int iArg = 0; ; ++iArg )\n" );
	strCode.append( "\t{\n" );
	strCode.append( "\t\tCXmlElement * pclsArg = pclsArgList->SelectElement( iArg );\n" );
	strCode.append( "\t\tif( pclsArg == NULL ) break;\n" );
	strCode.append( "\n" );

	int iArg = 0;
	char szIf[255];

	for( itAL = m_clsOutputList.begin(); itAL != m_clsOutputList.end(); ++iArg, ++itAL )
	{
		itAL->GetVariable( strVar, false );
		
		if( iArg == 0 )
		{
			snprintf( szIf, sizeof(szIf), "\t\tif( iArg == %d )\n", iArg );
		}
		else
		{
			snprintf( szIf, sizeof(szIf), "\t\telse if( iArg == %d )\n", iArg );
		}

		strCode.append( szIf );
		strCode.append( "\t\t{\n" );
		strCode.append( "\t\t\t" );
		strCode.append( strVar );
		strCode.append( " = pclsArg->GetData();\n" );
		strCode.append( "\t\t}\n" );
	}

	strCode.append( "\t}\n" );
	strCode.append( "\n" );
	strCode.append( "\treturn true;\n" );
	strCode.append( "}\n" );

	return true;
}
