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

#include "TestNaverCafeSearch.h"

// https://search.naver.com/search.naver?sm=tab_hty.top&where=articlec&tqi=inTAHwp0JXVssQmO%2FywssssssPV-505980&nso=so%3Ar%2Cp%3A1w&stnm=rel&query=NT930XED-KC51S

bool ParseHtml( std::string & strHtml )
{
	CHtmlSearch clsHtml;
	HTML_ELEMENT_LIST clsTAList;
	HTML_ELEMENT_LIST::iterator itTA;

	if( clsHtml.Parse( strHtml.c_str(), (int)strHtml.length() ) == -1 )
	{
		printf( "html parse error\n" );
		return false;
	}

	clsHtml.SelectClassElementList( "total_area", clsTAList );

	for( itTA = clsTAList.begin(); itTA != clsTAList.end(); ++itTA )
	{
		CHtmlElement * pclsA = itTA->SelectElement( 1 );
		if( pclsA == NULL ) continue;

		if( !strcasecmp( pclsA->GetName(), "a" ) )
		{
			printf( "%s\n", pclsA->GetData() );
		}
	}

	return true;
}

bool GetHtml( const char * pszQuery, std::string & strHtml )
{
	CHttpClient clsClient;
	std::string strBodyType, strBody;
	std::string strUrl = "https://search.naver.com/search.naver?sm=tab_hty.top&where=articlec&tqi=inTAHwp0JXVssQmO%2FywssssssPV-505980&nso=so%3Ar%2Cp%3A1w&stnm=rel&query=";

	strUrl.append( pszQuery );

	if( clsClient.DoGet( strUrl.c_str(), strBodyType, strBody ) )
	{
		strHtml = strBody;

		return true;
	}

	return false;
}

int main( int argc, char * argv[] )
{
	std::string strHtml;

	if( GetHtml( "NT930XED-KC51S", strHtml ) )
	{
#ifdef WIN32
		Utf8ToAnsi( strHtml.c_str(), strHtml );
#endif

		ParseHtml( strHtml );
	}

	return 0;
}