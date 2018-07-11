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

#include "XmlElement.h"

int main( int argc, char * argv[] )
{
	CXmlElement clsXml;
	std::string strXml;

	clsXml.SetName( "Class" );
	clsXml.InsertElementData( "Arg1", "Value1" );

	clsXml.ToString( strXml );

	clsXml.Clear();
	clsXml.SetName( "soapenv:Envelope" );
	clsXml.InsertAttribute( "xmlns:soapenv", "http://schemas.xmlsoap.org/soap/envelope/" );
	clsXml.InsertAttribute( "xmlns:web", "http://www.webserviceX.NET" );
	clsXml.InsertElementData( "soapenv:Header", "" );

	CXmlElement clsBody, clsGetWeather;
	clsBody.SetName( "soapenv:Body" );
	clsGetWeather.SetName( "web:GetWeather" );
	clsGetWeather.InsertElementData( "web:CityName", "seoul" );
	clsGetWeather.InsertElementData( "web:CountryName", "kr" );

	clsBody.InsertElement( &clsGetWeather );
	clsXml.InsertElement( &clsBody );

	strXml.clear();
	clsXml.ToString( strXml );

	std::string strPrefix, strBodyName;
	const char * pszName = clsXml.GetName();
	const char * pszPos = strstr( pszName, ":" );
	if( pszPos )
	{
		strPrefix.append( pszName, pszPos - pszName );
	}

	strBodyName = strPrefix + ":" + "Body";
	
	CXmlElement * pclsBody = clsXml.SelectElement( strBodyName.c_str() );
	if( pclsBody )
	{
		CXmlElement * pclsArgList = pclsBody->SelectElement( 0 );
		if( pclsArgList )
		{
			for( int iArg = 0; ; ++iArg )
			{
				CXmlElement * pclsArg = pclsArgList->SelectElement( iArg );
				if( pclsArg == NULL ) break;

				printf( "[%s]\n", pclsArg->GetName() );
			}

			pclsArgList->UpdateElementData( "web:CountryName", "us" );
		}
	}

	strXml.clear();
	clsXml.ToString( strXml );

	return 0;
}
