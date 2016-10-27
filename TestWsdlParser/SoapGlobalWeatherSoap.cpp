#include "HttpClient.h"
#include "XmlElement.h"
#include "Log.h"
#include "MemoryDebug.h"
#include "SoapGlobalWeatherSoap.h"

CSoapGlobalWeatherSoap::CSoapGlobalWeatherSoap()
{
}

CSoapGlobalWeatherSoap::~CSoapGlobalWeatherSoap()
{
}

bool CSoapGlobalWeatherSoap::GetCitiesByCountry( std::string & strCountryName, std::string & strGetCitiesByCountryResultOut )
{
	std::string strSendBody, strRecvBody;
	CHttpClient clsClient;
	CXmlElement clsXml;

	clsXml.SetName( "soapenv:Envelope" );
	clsXml.InsertAttribute( "xmlns:soapenv", "http://schemas.xmlsoap.org/soap/envelope/" );
	clsXml.InsertAttribute( "xmlns:tns", "http://www.webserviceX.NET" );
	clsXml.InsertElementData( "soapenv:Header", "" );

	CXmlElement clsBody, clsMethod;

	clsBody.SetName( "soapenv:Body" );
	clsMethod.SetName( "tns:GetCitiesByCountry" );
	clsMethod.InsertElementData( "tns:CountryName", strCountryName );
	clsBody.InsertElement( &clsMethod );
	clsXml.InsertElement( &clsBody );
	clsXml.ToString( strSendBody );

	if( clsClient.DoSoap( "http://www.webservicex.net/globalweather.asmx", "http://www.webserviceX.NET/GetCitiesByCountry", strSendBody.c_str(), strRecvBody ) == false )
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
			strGetCitiesByCountryResultOut = pclsArg->GetData();
		}
	}

	return true;
}

bool CSoapGlobalWeatherSoap::GetWeather( std::string & strCityName, std::string & strCountryName, std::string & strGetWeatherResultOut )
{
	std::string strSendBody, strRecvBody;
	CHttpClient clsClient;
	CXmlElement clsXml;

	clsXml.SetName( "soapenv:Envelope" );
	clsXml.InsertAttribute( "xmlns:soapenv", "http://schemas.xmlsoap.org/soap/envelope/" );
	clsXml.InsertAttribute( "xmlns:tns", "http://www.webserviceX.NET" );
	clsXml.InsertElementData( "soapenv:Header", "" );

	CXmlElement clsBody, clsMethod;

	clsBody.SetName( "soapenv:Body" );
	clsMethod.SetName( "tns:GetWeather" );
	clsMethod.InsertElementData( "tns:CityName", strCityName );
	clsMethod.InsertElementData( "tns:CountryName", strCountryName );
	clsBody.InsertElement( &clsMethod );
	clsXml.InsertElement( &clsBody );
	clsXml.ToString( strSendBody );

	if( clsClient.DoSoap( "http://www.webservicex.net/globalweather.asmx", "http://www.webserviceX.NET/GetWeather", strSendBody.c_str(), strRecvBody ) == false )
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
			strGetWeatherResultOut = pclsArg->GetData();
		}
	}

	return true;
}

