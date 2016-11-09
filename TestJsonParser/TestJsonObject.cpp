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

#include "JsonObject.h"

static bool TestJsonObject( const char * pszInput, const char * pszOutput )
{
	CJsonObject clsObject;
	std::string strOutput;

	if( clsObject.Parse( pszInput, strlen(pszInput) ) == -1 )
	{
		printf( "%s parse(%s) error\n", __FUNCTION__, pszInput );
		return false;
	}

	clsObject.ToString( strOutput );

	if( strcmp( strOutput.c_str(), pszOutput ) )
	{
		printf( "%s input(%s) output(%s) != output_want(%s) error\n", __FUNCTION__, pszInput, strOutput.c_str(), pszOutput );
		return false;
	}

	return true;
}

bool TestJsonObject( )
{
	if( TestJsonObject( "{ \"name\" : \"value\" }", "{ \"name\" : \"value\" }" ) == false ) return false;
	if( TestJsonObject( "{ \"name\" : \"value\", \"name1\" : \"value1\" }", "{ \"name\" : \"value\", \"name1\" : \"value1\" }" ) == false ) return false;
	if( TestJsonObject( "{ \"name\"  : \"value\"  , \"name1\"  : \"value1\" }", "{ \"name\" : \"value\", \"name1\" : \"value1\" }" ) == false ) return false;
	if( TestJsonObject( "{ \"name\" : \"value\", \"object\" : { \"name1\" : \"value1\" } }", "{ \"name\" : \"value\", \"object\" : { \"name1\" : \"value1\" } }" ) == false ) return false;
	if( TestJsonObject( "{ \"name\" : \"value\" , \"object\" : { \"name1\" : \"value1\" } }", "{ \"name\" : \"value\", \"object\" : { \"name1\" : \"value1\" } }" ) == false ) return false;
	if( TestJsonObject( "{ \"name\" : \"value\", \"object\" : { \"object1\" : { \"name1\" : \"value1\" } } }", "{ \"name\" : \"value\", \"object\" : { \"object1\" : { \"name1\" : \"value1\" } } }" ) == false ) return false;

	return true;
}
