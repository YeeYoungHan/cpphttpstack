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

void PrintGPL( FILE * fd )
{
	fprintf( fd, "/* \r\n"
	" * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)\r\n"
	" *\r\n"
	" * This program is free software; you can redistribute it and/or modify\r\n"
	" * it under the terms of the GNU General Public License as published by\r\n"
	" * the Free Software Foundation; either version 3 of the License, or\r\n"
	" * (at your option) any later version.\r\n"
	" *\r\n"
	" * This program is distributed in the hope that it will be useful,\r\n"
	" * but WITHOUT ANY WARRANTY; without even the implied warranty of\r\n"
	" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\r\n"
	" * GNU General Public License for more details.\r\n"
	" *\r\n"
	" * You should have received a copy of the GNU General Public License\r\n"
	" * along with this program; if not, write to the Free Software\r\n"
	" * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA \r\n"
	" */\r\n\r\n" );
}

void GetDefineName( const char * pszName, std::string & strDefine )
{
	int iLen = strlen( pszName );
	char szOne[2];

	memset( szOne, 0, sizeof(szOne) );
	strDefine.clear();

	for( int i = 0; i < iLen; ++i )
	{
		if( isupper( pszName[i] ) )
		{
			strDefine.append( "_" );
			strDefine.append( pszName + i, 1 );
		}
		else if( islower( pszName[i] ) )
		{
			szOne[0] = pszName[i] - 'A';
			strDefine.append( szOne, 1 );
		}
		else
		{
			strDefine.append( pszName + i, 1 );
		}
	}

	strDefine.append( "_H_" );
}

bool MakeHeaderFile( const char * pszFileName, const char * pszClassName )
{
	std::string strDefine, strFileName;

	GetDefineName( pszFileName, strDefine );

	strFileName = pszFileName;
	strFileName.append( ".h" );

	if( IsExistFile( strFileName.c_str() ) )
	{
		printf( "file(%s) already exist\n", strFileName.c_str() );
		return false;
	}

	FILE * fd = fopen( strFileName.c_str(), "w" );
	if( fd == NULL )
	{
		printf( "file(%s) open error\n", strFileName.c_str() );
		return false;
	}

	PrintGPL( fd );

	fprintf( fd, "#ifndef %s\r\n", strDefine.c_str() );
	fprintf( fd, "#define %s\r\n", strDefine.c_str() );
	fprintf( fd, "\r\n" );

	fprintf( fd, "class %s\r\n", pszClassName );
	fprintf( fd, "\r\n" );

	fprintf( fd, "{\r\n" );
	fprintf( fd, "public:\r\n" );
	fprintf( fd, "\t%s();\r\n", pszClassName );
	fprintf( fd, "\t~%s();\r\n", pszClassName );
	fprintf( fd, "}\r\n" );

	fprintf( fd, "#endif\r\n" );

	fclose( fd );

	return true;
}
