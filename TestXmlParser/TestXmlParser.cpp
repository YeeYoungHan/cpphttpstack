#include "XmlElement.h"

int main( int argc, char * argv[] )
{
	CXmlElement clsElement;
	std::string strXml;

	clsElement.SetName( "Class" );
	clsElement.InsertElementData( "Arg1", "Value1" );

	clsElement.ToString( strXml );

	clsElement.Clear();
	clsElement.SetName( "soapenv:Envelope" );
	clsElement.InsertAttribute( "xmlns:soapenv", "http://schemas.xmlsoap.org/soap/envelope/" );
	clsElement.InsertAttribute( "xmlns:web", "http://www.webserviceX.NET" );
	clsElement.InsertElementData( "soapenv:Header", "" );

	CXmlElement clsBody, clsGetWeather;
	clsBody.SetName( "soapenv:Body" );
	clsGetWeather.SetName( "web:GetWeather" );
	clsGetWeather.InsertElementData( "web:CityName", "seoul" );
	clsGetWeather.InsertElementData( "web:CountryName", "kr" );

	clsBody.InsertElement( &clsGetWeather );
	clsElement.InsertElement( &clsBody );

	strXml.clear();
	clsElement.ToString( strXml );
	
	return 0;
}
