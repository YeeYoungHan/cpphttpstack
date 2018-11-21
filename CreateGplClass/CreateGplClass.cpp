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
#include <stdio.h>
#include "FileUtility.h"

#include "CreateHeaderFile.hpp"
#include "CreateSourceFile.hpp"

int main( int argc, char * argv[] )
{
	if( argc < 2 )
	{
		printf( "[Usage] %s {class name} {use pragma once}\n", argv[0] );
		return 0;
	}

	const char * pszClassName = argv[1];
	if( strlen( pszClassName ) <= 1 )
	{
		printf( "class name is not correct\n" );
		return 0;
	}

	bool bUsePragmaOnce = false;

	if( argc >= 3 )
	{
		bUsePragmaOnce = true;
	}

	const char * pszFileName = pszClassName;

	if( pszClassName[0] == 'C' || pszClassName[0] == 'I' )
	{
		pszFileName = pszClassName + 1;
	}

	if( MakeHeaderFile( pszFileName, pszClassName, bUsePragmaOnce ) == false ) return 0;
	if( MakeSourceFile( pszFileName, pszClassName ) == false ) return 0;

	return 0;
}
