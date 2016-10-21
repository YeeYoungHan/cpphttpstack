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
#include "SoapGlobalWeatherSoap.h"
#include "WsdlMessage.h"

bool TestGlobalWeatherSoap( )
{
	CSoapGlobalWeatherSoap clsSoap;
	std::string strInput, strOutput;

	strInput = "korea";

	if( clsSoap.GetCitiesByCountry( strInput, strOutput ) == false ) return false;

	printf( "[%s]\n", strOutput.c_str() );

	return true;
}

// http://www.webservicex.net/ws/WSDetails.aspx?CATID=12&WSID=56

bool MakeGlobalWeatherSoap( )
{
	FILE * fd;
	char szBuf[8192];
	std::string strXml;
	CWsdlMessage clsWsdl;

	fd = fopen( "globalweather.xml", "r" );
	if( fd == NULL )
	{
		printf( "fopen error\n" );
		return false;
	}

	memset( szBuf, 0, sizeof(szBuf) );

	while( fgets( szBuf, sizeof(szBuf)-1, fd ) )
	{
		strXml.append( szBuf );
		memset( szBuf, 0, sizeof(szBuf) );
	}

	fclose( fd );

	if( clsWsdl.Parse( strXml.c_str(), strXml.length() ) == false )
	{
		printf( "clsWsdl.Parse error\n" );
		return 0;
	}

	clsWsdl.GetSoap()->MakeSource( NULL );

	return true;
}
