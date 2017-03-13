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
	CJsonArray clsArray;
	std::string strText;

	Check( clsArray.InsertData( "a" ) );
	Check( clsArray.InsertData( "b" ) );
	Check( clsArray.InsertData( "c" ) );
	clsArray.ToString( strText );
	Check( !strcmp( strText.c_str(), "[ \"a\", \"b\", \"c\" ]" ) );

	clsArray.Clear();
	strText.clear();

	Check( clsArray.InsertData( 1 ) );
	Check( clsArray.InsertData( 2 ) );
	Check( clsArray.InsertData( 3 ) );
	clsArray.ToString( strText );
	Check( !strcmp( strText.c_str(), "[ 1, 2, 3 ]" ) );

	clsArray.Clear();
	strText.clear();

	Check( clsArray.InsertData( true ) );
	Check( clsArray.InsertData( false ) );
	clsArray.ToString( strText );
	Check( !strcmp( strText.c_str(), "[ true, false ]" ) );

	clsObject.InsertData( "name", "value" );

	clsArray.Clear();
	strText.clear();

	Check( clsArray.InsertData( "a" ) );
	Check( clsArray.InsertData( &clsObject ) );
	clsArray.ToString( strText );
	Check( !strcmp( strText.c_str(), "[ \"a\", { \"name\" : \"value\" } ]" ) );

	strText.clear();

	clsArray.Parse( "[ ]", 3 );
	clsArray.ToString( strText );
	Check( !strcmp( strText.c_str(), "[ ]" ) );

	return true;
}
