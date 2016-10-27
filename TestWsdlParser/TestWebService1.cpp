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
#include "SoapService1Soap.h"
#include "WsdlMessage.h"

// C# 으로 생성한 WebService 테스트

bool TestWebService1Soap( )
{
	CSoapService1Soap clsSoap;
	std::string strName, strOutput;
	int iValue;

	if( clsSoap.HelloWorld( strOutput ) == false ) return false;

	printf( "[%s]\n", strOutput.c_str() );

	strName = "Su";
	iValue = 102;

	if( clsSoap.PutString( strName, strOutput ) == false ) return false;

	printf( "[%s]\n", strOutput.c_str() );

	if( clsSoap.PutStringInt( strName, iValue, strOutput ) == false ) return false;

	printf( "[%s]\n", strOutput.c_str() );

	return true;
}

bool MakeWebService1Soap( )
{
	CWsdlMessage clsWsdl;

	if( clsWsdl.ParseFile( "WebService1.xml" ) == false )
	{
		printf( "clsWsdl.Parse error\n" );
		return 0;
	}

	clsWsdl.GetSoap()->MakeSource( NULL );

	return true;
}
