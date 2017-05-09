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
#include "JsonArray.h"

bool TestJsonArray()
{
	CJsonObject clsObject;
	CJsonArray clsArray, clsAppend;
	std::string strText;

	Check( clsArray.InsertData( "a" ) );
	Check( clsArray.InsertData( "b" ) );
	Check( clsArray.InsertData( "c" ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ \"a\", \"b\", \"c\" ]" ) );

	clsArray.Clear();
	Check( clsArray.InsertData( 1 ) );
	Check( clsArray.InsertData( 2 ) );
	Check( clsArray.InsertData( 3 ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 1, 2, 3 ]" ) );

	clsArray.Clear();
	Check( clsArray.InsertData( true ) );
	Check( clsArray.InsertData( false ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ true, false ]" ) );

	clsObject.InsertData( "name", "value" );

	clsArray.Clear();
	Check( clsArray.InsertData( "a" ) );
	Check( clsArray.InsertData( &clsObject ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ \"a\", { \"name\" : \"value\" } ]" ) );

	clsArray.Parse( "[ ]", 3 );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ ]" ) );

	clsArray.Clear();
	Check( clsArray.InsertData( 1 ) );
	Check( clsArray.InsertData( 2 ) );
	Check( clsArray.InsertData( 3 ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 1, 2, 3 ]" ) );
	Check( clsArray.DeleteData( 3 ) == false );
	Check( clsArray.DeleteData( -1 ) == false );
	Check( clsArray.DeleteData( 2 ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 1, 2 ]" ) );
	Check( clsArray.DeleteData( 0 ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 2 ]" ) );	

	clsArray.Clear();
	Check( clsArray.InsertData( 1 ) );
	Check( clsArray.InsertData( 2 ) );
	Check( clsArray.InsertData( 3 ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 1, 2, 3 ]" ) );
	Check( clsArray.InsertData( 0, "100" ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ \"100\", 1, 2, 3 ]" ) );
	Check( clsArray.InsertData( 4, "200" ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ \"100\", 1, 2, 3, \"200\" ]" ) );

	clsArray.Clear();
	Check( clsArray.InsertData( 1 ) );
	Check( clsArray.InsertData( 2 ) );
	Check( clsArray.InsertData( 3 ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 1, 2, 3 ]" ) );
	Check( clsArray.InsertData( 0, 100 ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 100, 1, 2, 3 ]" ) );
	Check( clsArray.InsertData( 4, 200 ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 100, 1, 2, 3, 200 ]" ) );
	Check( clsArray.InsertData( ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 100, 1, 2, 3, 200, null ]" ) );
	Check( clsArray.InsertDataNull( 1 ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 100, null, 1, 2, 3, 200, null ]" ) );

	clsArray.Clear();
	Check( clsArray.InsertData( 1 ) );
	Check( clsArray.InsertData( 2 ) );
	Check( clsArray.InsertData( 3 ) );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 1, 2, 3 ]" ) );
	Check( clsAppend.InsertData( 4 ) );
	Check( clsAppend.InsertData( 5 ) );
	Check( clsAppend.InsertData( 6 ) );
	clsArray.InsertArray( &clsAppend );
	clsArray.MakeString( strText );
	Check( !strcmp( strText.c_str(), "[ 1, 2, 3, 4, 5, 6 ]" ) );

	return true;
}
