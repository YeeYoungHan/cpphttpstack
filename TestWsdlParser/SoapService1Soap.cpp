#include "HttpClient.h"
#include "XmlElement.h"
#include "Log.h"
#include "MemoryDebug.h"
#include "SoapService1Soap.h"

CSoapService1Soap::CSoapService1Soap()
{
}

CSoapService1Soap::~CSoapService1Soap()
{
}

bool CSoapService1Soap::HelloWorld( std::string & strHelloWorldResultOut )
{
	std::string strSendBody, strRecvBody;
	CHttpClient clsClient;
	CXmlElement clsXml;

	clsXml.SetName( "soapenv:Envelope" );
	clsXml.InsertAttribute( "xmlns:soapenv", "http://schemas.xmlsoap.org/soap/envelope/" );
	clsXml.InsertAttribute( "xmlns:tns", "http://tempuri.org/" );
	clsXml.InsertElementData( "soapenv:Header", "" );

	CXmlElement clsBody, clsMethod;

	clsBody.SetName( "soapenv:Body" );
	clsMethod.SetName( "tns:HelloWorld" );
	clsBody.InsertElement( &clsMethod );
	clsXml.InsertElement( &clsBody );
	clsXml.ToString( strSendBody );

	if( clsClient.DoSoap( "http://localhost:2584/Service1.asmx", "http://tempuri.org/HelloWorld", strSendBody.c_str(), strRecvBody ) == false )
	{
		CLog::Print( LOG_ERROR, "%s DoSoap error", __FUNCTION__ );
		return false;
	}

	if( clsXml.Parse( strRecvBody ) == -1 )
	{
		CLog::Print( LOG_ERROR, "%s Parse error", __FUNCTION__ );
		return false;
	}

	std::string strPrefix, strBodyName;
	const char * pszName = clsXml.GetName();
	const char * pszPos = strstr( pszName, ":" );
	if( pszPos )
	{
		strPrefix.append( pszName, pszPos - pszName );
	}

	strBodyName = strPrefix + ":" + "Body";
	CXmlElement * pclsBody = clsXml.SelectElement( strBodyName.c_str() );
	if( pclsBody == NULL )
	{
		CLog::Print( LOG_ERROR, "%s Body not found", __FUNCTION__ );
		return false;
	}

	CXmlElement * pclsArgList = pclsBody->SelectElement( 0 );
	if( pclsArgList == NULL )
	{
		CLog::Print( LOG_ERROR, "%s Body ArgList not found", __FUNCTION__ );
		return false;
	}

	for( int iArg = 0; ; ++iArg )
	{
		CXmlElement * pclsArg = pclsArgList->SelectElement( iArg );
		if( pclsArg == NULL ) break;

		if( iArg == 0 )
		{
			strHelloWorldResultOut = pclsArg->GetData();
		}
	}

	return true;
}

bool CSoapService1Soap::PutString( std::string & strname, std::string & strPutStringResultOut )
{
	std::string strSendBody, strRecvBody;
	CHttpClient clsClient;
	CXmlElement clsXml;

	clsXml.SetName( "soapenv:Envelope" );
	clsXml.InsertAttribute( "xmlns:soapenv", "http://schemas.xmlsoap.org/soap/envelope/" );
	clsXml.InsertAttribute( "xmlns:tns", "http://tempuri.org/" );
	clsXml.InsertElementData( "soapenv:Header", "" );

	CXmlElement clsBody, clsMethod;

	clsBody.SetName( "soapenv:Body" );
	clsMethod.SetName( "tns:PutString" );
	clsMethod.InsertElementData( "tns:name", strname );
	clsBody.InsertElement( &clsMethod );
	clsXml.InsertElement( &clsBody );
	clsXml.ToString( strSendBody );

	if( clsClient.DoSoap( "http://localhost:2584/Service1.asmx", "http://tempuri.org/PutString", strSendBody.c_str(), strRecvBody ) == false )
	{
		CLog::Print( LOG_ERROR, "%s DoSoap error", __FUNCTION__ );
		return false;
	}

	if( clsXml.Parse( strRecvBody ) == -1 )
	{
		CLog::Print( LOG_ERROR, "%s Parse error", __FUNCTION__ );
		return false;
	}

	std::string strPrefix, strBodyName;
	const char * pszName = clsXml.GetName();
	const char * pszPos = strstr( pszName, ":" );
	if( pszPos )
	{
		strPrefix.append( pszName, pszPos - pszName );
	}

	strBodyName = strPrefix + ":" + "Body";
	CXmlElement * pclsBody = clsXml.SelectElement( strBodyName.c_str() );
	if( pclsBody == NULL )
	{
		CLog::Print( LOG_ERROR, "%s Body not found", __FUNCTION__ );
		return false;
	}

	CXmlElement * pclsArgList = pclsBody->SelectElement( 0 );
	if( pclsArgList == NULL )
	{
		CLog::Print( LOG_ERROR, "%s Body ArgList not found", __FUNCTION__ );
		return false;
	}

	for( int iArg = 0; ; ++iArg )
	{
		CXmlElement * pclsArg = pclsArgList->SelectElement( iArg );
		if( pclsArg == NULL ) break;

		if( iArg == 0 )
		{
			strPutStringResultOut = pclsArg->GetData();
		}
	}

	return true;
}

bool CSoapService1Soap::PutStringInt( std::string & strname, int & ivalue, std::string & strPutStringIntResultOut )
{
	std::string strSendBody, strRecvBody;
	CHttpClient clsClient;
	CXmlElement clsXml;

	clsXml.SetName( "soapenv:Envelope" );
	clsXml.InsertAttribute( "xmlns:soapenv", "http://schemas.xmlsoap.org/soap/envelope/" );
	clsXml.InsertAttribute( "xmlns:tns", "http://tempuri.org/" );
	clsXml.InsertElementData( "soapenv:Header", "" );

	CXmlElement clsBody, clsMethod;

	clsBody.SetName( "soapenv:Body" );
	clsMethod.SetName( "tns:PutStringInt" );
	clsMethod.InsertElementData( "tns:name", strname );
	clsMethod.InsertElementData( "tns:value", ivalue );
	clsBody.InsertElement( &clsMethod );
	clsXml.InsertElement( &clsBody );
	clsXml.ToString( strSendBody );

	if( clsClient.DoSoap( "http://localhost:2584/Service1.asmx", "http://tempuri.org/PutStringInt", strSendBody.c_str(), strRecvBody ) == false )
	{
		CLog::Print( LOG_ERROR, "%s DoSoap error", __FUNCTION__ );
		return false;
	}

	if( clsXml.Parse( strRecvBody ) == -1 )
	{
		CLog::Print( LOG_ERROR, "%s Parse error", __FUNCTION__ );
		return false;
	}

	std::string strPrefix, strBodyName;
	const char * pszName = clsXml.GetName();
	const char * pszPos = strstr( pszName, ":" );
	if( pszPos )
	{
		strPrefix.append( pszName, pszPos - pszName );
	}

	strBodyName = strPrefix + ":" + "Body";
	CXmlElement * pclsBody = clsXml.SelectElement( strBodyName.c_str() );
	if( pclsBody == NULL )
	{
		CLog::Print( LOG_ERROR, "%s Body not found", __FUNCTION__ );
		return false;
	}

	CXmlElement * pclsArgList = pclsBody->SelectElement( 0 );
	if( pclsArgList == NULL )
	{
		CLog::Print( LOG_ERROR, "%s Body ArgList not found", __FUNCTION__ );
		return false;
	}

	for( int iArg = 0; ; ++iArg )
	{
		CXmlElement * pclsArg = pclsArgList->SelectElement( iArg );
		if( pclsArg == NULL ) break;

		if( iArg == 0 )
		{
			strPutStringIntResultOut = pclsArg->GetData();
		}
	}

	return true;
}

