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

/**
 * @ingroup CreateGplClass
 * @brief GPL 라이선스 문구를 출력한다.
 * @param fd 파일 핸들
 */
void PrintGPL( FILE * fd )
{
	fprintf( fd, "/* \n"
	" * Copyright (C) 2021 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)\n"
	" *\n"
	" * This program is free software; you can redistribute it and/or modify\n"
	" * it under the terms of the GNU General Public License as published by\n"
	" * the Free Software Foundation; either version 3 of the License, or\n"
	" * (at your option) any later version.\n"
	" *\n"
	" * This program is distributed in the hope that it will be useful,\n"
	" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	" * GNU General Public License for more details.\n"
	" *\n"
	" * You should have received a copy of the GNU General Public License\n"
	" * along with this program; if not, write to the Free Software\n"
	" * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA \n"
	" */\n\n" );
}

/**
 * @ingroup CreateGplClass
 * @brief 헤더 파일에 사용할 선언문 이름을 생성한다.
 * @param pszName		파일 이름
 * @param strDefine 선언문 이름 저장용 변수
 */
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
			szOne[0] = pszName[i] - 'a' + 'A';
			strDefine.append( szOne, 1 );
		}
		else
		{
			strDefine.append( pszName + i, 1 );
		}
	}

	strDefine.append( "_H_" );
}

/**
 * @ingroup CreateGplClass
 * @brief 헤더 파일을 생성한다.
 * @param pszFileName		확장자를 제외한 헤더 파일 이름
 * @param pszClassName	클래스 이름
 * @param bUsePragmaOnce	pragma once 를 사용한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool MakeHeaderFile( const char * pszFileName, const char * pszClassName, bool bUsePragmaOnce )
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

	if( bUsePragmaOnce )
	{
		fprintf( fd, "#pragma once\n" );
	}
	else
	{
		fprintf( fd, "#ifndef %s\n", strDefine.c_str() );
		fprintf( fd, "#define %s\n", strDefine.c_str() );
	}

	fprintf( fd, "\n" );
	fprintf( fd, "class %s\n", pszClassName );
	fprintf( fd, "{\n" );
	fprintf( fd, "public:\n" );
	fprintf( fd, "\t%s();\n", pszClassName );
	fprintf( fd, "\t~%s();\n", pszClassName );
	fprintf( fd, "};\n" );
	fprintf( fd, "\n" );

	if( bUsePragmaOnce == false )
	{
		fprintf( fd, "#endif\n" );
	}

	fclose( fd );

	return true;
}
