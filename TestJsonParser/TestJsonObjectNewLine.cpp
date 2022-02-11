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

#include "Main.h"
#include "JsonObject.h"

static bool TestJsonObject( const char * pszInput, const char * pszOutput )
{
	CJsonObject clsObject;
	std::string strOutput;

	if( clsObject.Parse( pszInput, (int)strlen(pszInput) ) == -1 )
	{
		printf( "%s parse(%s) error\n", __FUNCTION__, pszInput );
		return false;
	}

	clsObject.ToString( strOutput, E_JNL_ALL );

	if( strcmp( strOutput.c_str(), pszOutput ) )
	{
		printf( "%s input(%s) output(%s) != output_want(%s) error\n", __FUNCTION__, pszInput, strOutput.c_str(), pszOutput );
		return false;
	}

	return true;
}

bool TestJsonObjectNewLine( )
{
	Check( TestJsonObject( "{ \"name\" : \"value\" }", 
		"{\n"
		"	\"name\" : \"value\"\n"
		"}" ) );
	Check( TestJsonObject( "{\"name\":\"value\"}", 
		"{\n"
		"	\"name\" : \"value\"\n"
		"}" ) );
	Check( TestJsonObject( "{\"name\":{\"value1\":\"value2\"}}", 
		"{\n"
		"	\"name\" : {\n"
		"		\"value1\" : \"value2\"\n"
		"	}\n"
		"}" ) );
	Check( TestJsonObject( "{\"name\":[{\"value1\":\"value2\"},{\"value3\":\"value4\"}]}", 
		"{\n"
		"	\"name\" : [\n"
		"		{\n"
		"			\"value1\" : \"value2\"\n"
		"		},\n"
		"		{\n"
		"			\"value3\" : \"value4\"\n"
		"		}\n"
		"	]\n"
		"}" ) );
	Check( TestJsonObject( "{ \"name\" : \"value\", \"name1\" : \"value1\" }", 
		"{\n"
		"	\"name\" : \"value\",\n"
		"	\"name1\" : \"value1\"\n"
		"}" ) );
	Check( TestJsonObject( "{ \"name\"  : \"value\"  , \"name1\"  : \"value1\" }", 
		"{\n"
		"	\"name\" : \"value\",\n"
		"	\"name1\" : \"value1\"\n"
		"}" ) );
	Check( TestJsonObject( "{ \"name\" : \"value\", \"object\" : { \"name1\" : \"value1\" } }", 
		"{\n"
		"	\"name\" : \"value\",\n"
		"	\"object\" : {\n"
		"		\"name1\" : \"value1\"\n"
		"	}\n"
		"}" ) );
	Check( TestJsonObject( "{ \"name\" : \"value\" , \"object\" : { \"name1\" : \"value1\" } }"
		, "{\n"
		"	\"name\" : \"value\",\n"
		"	\"object\" : {\n"
		"		\"name1\" : \"value1\"\n"
		"	}\n"
		"}" ) );
	Check( TestJsonObject( "{ \"name\" : \"value\", \"object\" : { \"object1\" : { \"name1\" : \"value1\" } } }", 
		"{\n"
		"	\"name\" : \"value\",\n"
		"	\"object\" : {\n"
		"		\"object1\" : {\n"
		"			\"name1\" : \"value1\"\n"
		"		}\n"
		"	}\n"
		"}" ) );

	Check( TestJsonObject( "{ \"name\" : 1234 }", 
		"{\n"
		"	\"name\" : 1234\n"
		"}" ) );
	Check( TestJsonObject( "{\"name\":1234}", 
		"{\n"
		"	\"name\" : 1234\n"
		"}" ) );
	Check( TestJsonObject( "{ \"name\" : -1 }", 
		"{\n"
		"	\"name\" : -1\n"
		"}" ) );
	Check( TestJsonObject( "{ \"name\" : 12e31 }", 
		"{\n"
		"	\"name\" : 12e31\n"
		"}" ) );

	Check( TestJsonObject( "{ \"name\" : [ \"value\", \"value2\" ] }", 
		"{\n"
		"	\"name\" : [\n"
		"		\"value\",\n"
		"		\"value2\"\n"
		"	]\n"
		"}" ) );

	Check( TestJsonObject( "{ \"name\" : true }", 
		"{\n"
		"	\"name\" : true\n"
		"}" ) );
	Check( TestJsonObject( "{ \"name\" : false }", 
		"{\n"
		"	\"name\" : false\n"
		"}" ) );
	Check( TestJsonObject( "{\"name\":true}", 
		"{\n"
		"	\"name\" : true\n"
		"}" ) );
	Check( TestJsonObject( "{\"name\":false}", 
		"{\n"
		"	\"name\" : false\n"
		"}" ) );
	Check( TestJsonObject( "{ \"name\" : [ \"value\", \"value2\", true, false ] }", 
		"{\n"
		"	\"name\" : [\n"
		"		\"value\",\n"
		"		\"value2\",\n"
		"		true,\n"
		"		false\n"
		"	]\n"
		"}" ) );

	Check( TestJsonObject( "{ \"name\" : null }", 
		"{\n"
		"	\"name\" : null\n"
		"}" ) );
	Check( TestJsonObject( "{\"name\":null}", 
		"{\n"
		"	\"name\" : null\n"
		"}" ) );

	Check( TestJsonObject( "{ \"list\" : [ { \"UserId\" : \"Test\" } ] }", 
		"{\n"
		"	\"list\" : [\n"
		"		{\n"
		"			\"UserId\" : \"Test\"\n"
		"		}\n"
		"	]\n"
		"}" ) );
	Check( TestJsonObject( "{ \"a\" : \"1\", \"b\" : \"2\", \"list\" : [ ], \"q\" : \"3\", \"z\" : \"4\" }", 
		"{\n"
		"	\"a\" : \"1\",\n"
		"	\"b\" : \"2\",\n"
		"	\"list\" : [ ],\n"
		"	\"q\" : \"3\",\n"
		"	\"z\" : \"4\"\n"
		"}" ) );
	Check( TestJsonObject( "{ \"list\" : [ { \"A\" : [ 1, 2 ], \"UserId\" : \"Test\" } ] }", 
		"{\n"
		"	\"list\" : [\n"
		"		{\n"
		"			\"A\" : [\n"
		"				1,\n"
		"				2\n"
		"			],\n"
		"			\"UserId\" : \"Test\"\n"
		"		}\n"
		"	]\n"
		"}" ) );

	return true;
}
