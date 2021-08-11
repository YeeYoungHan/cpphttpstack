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

#include "HttpSimulator.h"
#include "HttpSimulatorVersion.h"

void PrintHelp( const char * pszProgramName )
{
	printf( "%s - version %s\n", pszProgramName, HTTP_SIMULATOR_VERSION );
	printf( "[Usage] %s {setup file}\n", pszProgramName );
}

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		PrintHelp( argv[0] );
		return 0;
	}

	if( !strcmp( argv[1], "-v" ) )
	{
		PrintHelp( argv[0] );
		return 0;
	}

	CHttpSimulatorSetup clsSetup;

	if( clsSetup.Read( argv[1] ) == false )
	{
		return 0;
	}

	Execute( clsSetup.m_clsCommandList, clsSetup.m_iLoopCount );

	return 0;
}
