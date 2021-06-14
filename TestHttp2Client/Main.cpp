/* 
 * Copyright (C) 2021 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
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

#include "TestHttp2Client.h"

int main( int argc, char * argv[] )
{
	bool bPrintHelp = false;

#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	if( argc >= 2 )
	{
		const char * pszCommand = argv[1];

		if( !strcmp( pszCommand, "get" ) )
		{
			TestHttp2ClientGet( argc, argv );
		}
		else if( !strcmp( pszCommand, "loop" ) )
		{
			TestHttp2ClientLoop( argc, argv );
		}
		else
		{
			bPrintHelp = true;
		}
	}
	else
	{
		bPrintHelp = true;
	}

	if( bPrintHelp )
	{
		argc = 1;

		TestHttp2ClientGet( argc, argv );
		TestHttp2ClientLoop( argc, argv );
	}

#ifdef WIN32
	SSLClientStop();
	SSLFinal();
#endif

	return 0;
}
