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

	clsObject.ToString( strOutput );
	int iOutputLen = clsObject.GetStringLen();

	if( strcmp( strOutput.c_str(), pszOutput ) )
	{
		printf( "%s input(%s) output(%s) != output_want(%s) error\n", __FUNCTION__, pszInput, strOutput.c_str(), pszOutput );
		return false;
	}

	int iWantLen = (int)strlen( pszOutput );
	if( iOutputLen != iWantLen )
	{
		printf( "%s input(%s) output(%s) outputLen(%d) wantLen(%d) error\n", __FUNCTION__, pszInput, pszOutput, iOutputLen, iWantLen );
		return false;
	}

	return true;
}

bool TestJsonObject( )
{
	Check( TestJsonObject( "{ \"name\" : \"value\" }", "{ \"name\" : \"value\" }" ) );
	Check( TestJsonObject( "{\"name\":\"value\"}", "{ \"name\" : \"value\" }" ) );
	Check( TestJsonObject( "{\"name\":{\"value1\":\"value2\"}}", "{ \"name\" : { \"value1\" : \"value2\" } }" ) );
	Check( TestJsonObject( "{\"name\":[{\"value1\":\"value2\"},{\"value3\":\"value4\"}]}", "{ \"name\" : [ { \"value1\" : \"value2\" }, { \"value3\" : \"value4\" } ] }" ) );
	Check( TestJsonObject( "{ \"name\" : \"value\", \"name1\" : \"value1\" }", "{ \"name\" : \"value\", \"name1\" : \"value1\" }" ) );
	Check( TestJsonObject( "{ \"name\"  : \"value\"  , \"name1\"  : \"value1\" }", "{ \"name\" : \"value\", \"name1\" : \"value1\" }" ) );
	Check( TestJsonObject( "{ \"name\" : \"value\", \"object\" : { \"name1\" : \"value1\" } }", "{ \"name\" : \"value\", \"object\" : { \"name1\" : \"value1\" } }" ) );
	Check( TestJsonObject( "{ \"name\" : \"value\" , \"object\" : { \"name1\" : \"value1\" } }", "{ \"name\" : \"value\", \"object\" : { \"name1\" : \"value1\" } }" ) );
	Check( TestJsonObject( "{ \"name\" : \"value\", \"object\" : { \"object1\" : { \"name1\" : \"value1\" } } }", "{ \"name\" : \"value\", \"object\" : { \"object1\" : { \"name1\" : \"value1\" } } }" ) );

	Check( TestJsonObject( "{ \"name\" : 1234 }", "{ \"name\" : 1234 }" ) );
	Check( TestJsonObject( "{\"name\":1234}", "{ \"name\" : 1234 }" ) );
	Check( TestJsonObject( "{ \"name\" : -1 }", "{ \"name\" : -1 }" ) );
	Check( TestJsonObject( "{ \"name\" : 12e31 }", "{ \"name\" : 12e31 }" ) );

	Check( TestJsonObject( "{ \"name\" : [ \"value\", \"value2\" ] }", "{ \"name\" : [ \"value\", \"value2\" ] }" ) );

	Check( TestJsonObject( "{ \"name\" : true }", "{ \"name\" : true }" ) );
	Check( TestJsonObject( "{ \"name\" : false }", "{ \"name\" : false }" ) );
	Check( TestJsonObject( "{\"name\":true}", "{ \"name\" : true }" ) );
	Check( TestJsonObject( "{\"name\":false}", "{ \"name\" : false }" ) );
	Check( TestJsonObject( "{ \"name\" : [ \"value\", \"value2\", true, false ] }", "{ \"name\" : [ \"value\", \"value2\", true, false ] }" ) );

	Check( TestJsonObject( "{ \"name\" : null }", "{ \"name\" : null }" ) );
	Check( TestJsonObject( "{\"name\":null}", "{ \"name\" : null }" ) );

	Check( TestJsonObject( "{ \"list\" : [ { \"UserId\" : \"Test\" } ] }", "{ \"list\" : [ { \"UserId\" : \"Test\" } ] }" ) );
	Check( TestJsonObject( "{ \"a\" : \"1\", \"b\" : \"2\", \"list\" : [ ], \"q\" : \"3\", \"z\" : \"4\" }", "{ \"a\" : \"1\", \"b\" : \"2\", \"list\" : [ ], \"q\" : \"3\", \"z\" : \"4\" }" ) );
	Check( TestJsonObject( "{ \"list\" : [ { \"A\" : [ 1, 2 ], \"UserId\" : \"Test\" } ] }", "{ \"list\" : [ { \"A\" : [ 1, 2 ], \"UserId\" : \"Test\" } ] }" ) );

	CJsonObject clsObject, clsNewObject;
	std::string strValue, strText;
	int64_t iValue = 0;
	bool bValue = false;
	CJsonArray * pclsArray = NULL;
	CJsonObject * pclsObject = NULL;
	const char * pszText = "{ \"strName\" : \"strValue\", \"iName\" : 1234, \"bName\" : true, \"arrName\" : [ 1, 2, 3 ], \"clsName\" : { \"childName\" : \"childValue\" } }";

	Check( clsObject.Parse( pszText, (int)strlen(pszText) ) );
	Check( clsObject.SelectData( "strName", strValue ) );
	Check( !strcmp( strValue.c_str(), "strValue" ) );
	Check( clsObject.SelectData( "iName", iValue ) );
	Check( iValue == 1234 );
	Check( clsObject.SelectData( "bName", bValue ) );
	Check( bValue == true );
	Check( clsObject.SelectData( "arrName", &pclsArray ) );
	Check( pclsArray->SelectData( 0, iValue ) );
	Check( iValue == 1 );
	Check( pclsArray->SelectData( 1, iValue ) );
	Check( iValue == 2 );
	Check( pclsArray->SelectData( 2, iValue ) );
	Check( iValue == 3 );
	Check( clsObject.SelectData( "clsName", &pclsObject ) );
	Check( pclsObject->SelectData( "childName", strValue ) );
	Check( !strcmp( strValue.c_str(), "childValue" ) );

	clsObject.Clear();
	Check( clsObject.InsertData( "name", "value" ) )
	clsObject.ToString( strText );
	Check( !strcmp( strText.c_str(), "{ \"name\" : \"value\" }" ) );

	clsObject.Clear();
	Check( clsObject.InsertData( "name", true ) )
	clsObject.MakeString( strText );
	Check( !strcmp( strText.c_str(), "{ \"name\" : true }" ) );

	clsObject.Clear();
	Check( clsObject.InsertData( "name", 1234 ) )
	clsObject.MakeString( strText );
	Check( !strcmp( strText.c_str(), "{ \"name\" : 1234 }" ) );

	clsObject.Clear();
	clsNewObject.Clear();
	Check( clsNewObject.InsertData( "name", "value" ) );
	Check( clsObject.InsertData( "a1", &clsNewObject ) );
	clsObject.MakeString( strText );
	Check( !strcmp( strText.c_str(), "{ \"a1\" : { \"name\" : \"value\" } }" ) );

	clsObject.Clear();
	Check( clsObject.InsertData( "a1", "v1" ) );
	Check( clsObject.InsertData( "a2", "v2" ) );
	clsObject.MakeString( strText );
	Check( !strcmp( strText.c_str(), "{ \"a1\" : \"v1\", \"a2\" : \"v2\" }" ) );
	Check( clsObject.DeleteData( "a2" ) );
	clsObject.MakeString( strText );
	Check( !strcmp( strText.c_str(), "{ \"a1\" : \"v1\" }" ) );
	Check( clsObject.DeleteData( "a1" ) );
	clsObject.MakeString( strText );
	Check( !strcmp( strText.c_str(), "{ }" ) );

	clsObject.Clear();
	Check( clsObject.InsertData( "a1", "v1" ) );
	Check( clsObject.InsertData( "a2", "v2" ) );
	clsObject.MakeString( strText );
	Check( !strcmp( strText.c_str(), "{ \"a1\" : \"v1\", \"a2\" : \"v2\" }" ) );
	Check( clsObject.UpdateData( "a1", "v11" ) );
	clsObject.MakeString( strText );
	Check( !strcmp( strText.c_str(), "{ \"a1\" : \"v11\", \"a2\" : \"v2\" }" ) );
	Check( clsObject.UpdateData( "a1" ) );
	clsObject.MakeString( strText );
	Check( !strcmp( strText.c_str(), "{ \"a1\" : null, \"a2\" : \"v2\" }" ) );
	Check( clsObject.UpdateData( "a1", 100 ) );
	clsObject.MakeString( strText );
	Check( !strcmp( strText.c_str(), "{ \"a1\" : 100, \"a2\" : \"v2\" }" ) );
	Check( clsObject.UpdateData( "a1", &clsNewObject ) );
	clsObject.MakeString( strText );
	Check( !strcmp( strText.c_str(), "{ \"a1\" : { \"name\" : \"value\" }, \"a2\" : \"v2\" }" ) );

	return true;
}
