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

void SampleJsonArray()
{
	{
		// [ "a", 2, true, null, { "n", "v2" } ] 积己规过
		CJsonArray clsArray;
		CJsonObject clsChild;
		std::string strJson;

		clsArray.InsertData( "a" );
		clsArray.InsertData( 2 );
		clsArray.InsertData( true );
		clsArray.InsertData( );

		clsChild.InsertData( "n", "v2" );
		clsArray.InsertData( &clsChild );

		clsArray.MakeString( strJson );

		printf( "#200 [%s]\n", strJson.c_str() );

		clsArray.MakeString( strJson, E_JNL_ALL );

		printf( "#201 [%s]\n", strJson.c_str() );
	}
}
