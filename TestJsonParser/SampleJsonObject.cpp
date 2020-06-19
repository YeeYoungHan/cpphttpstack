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

void SampleJsonObject()
{
	{
		// { "a" : "v", "b" : 2, "c" : true, "d" : null, "f" : { "n", "v2" } } 积己规过
		CJsonObject clsObject, clsChild;
		std::string strJson;

		clsObject.InsertData( "a", "v" );
		clsObject.InsertData( "b", 2 );
		clsObject.InsertData( "c", true );
		clsObject.InsertData( "d" );

		clsChild.InsertData( "n", "v2" );
		clsObject.InsertData( "f", &clsChild );

		clsObject.MakeString( strJson );

		printf( "#100 [%s]\n", strJson.c_str() );

		clsObject.MakeString( strJson, E_JNL_ALL );

		printf( "#101 [%s]\n", strJson.c_str() );
	}
}
