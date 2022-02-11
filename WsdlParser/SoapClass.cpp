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
#include "SoapClass.h"
#include "FileUtility.h"
#include "Directory.h"
#include "Log.h"
#include "MemoryDebug.h"

CSoapClass::CSoapClass()
{
}

CSoapClass::~CSoapClass()
{
}

/**
 * @ingroup WsdlParser
 * @brief SOAP Ŭ���̾�Ʈ �ҽ� �ڵ带 �����Ѵ�.
 * @param pszFolder �ҽ� ������ ������ ���� ���
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSoapClass::MakeSource( const char * pszFolder )
{
	std::string strClassName = "CSoap";
	strClassName.append( m_strName );

	std::string strFileName = "Soap";
	strFileName.append( m_strName );

	std::string strHeaderPath, strSourcePath;

	if( pszFolder )
	{
		strHeaderPath = pszFolder;
		CDirectory::AppendName( strHeaderPath, strFileName.c_str() );
	}
	else
	{
		strHeaderPath = strFileName;
	}

	strHeaderPath.append( ".h" );

	if( pszFolder )
	{
		strSourcePath = pszFolder;
		CDirectory::AppendName( strSourcePath, strFileName.c_str() );
	}
	else
	{
		strSourcePath = strFileName;
	}

	strSourcePath.append( ".cpp" );

	if( MakeHeaderFile( strClassName.c_str(), strFileName.c_str(), strHeaderPath.c_str() ) == false )
	{
		return false;
	}

	if( MakeSourceFile( strClassName.c_str(), strFileName.c_str(), strSourcePath.c_str() ) == false )
	{
		return false;
	}

	return true;
}

/**
 * @ingroup WsdlParser
 * @brief ��� ���Ͽ� ����� ���� �̸��� �����Ѵ�.
 * @param pszName		���� �̸�
 * @param strDefine ���� �̸� ����� ����
 */
void CSoapClass::GetDefineName( const char * pszName, std::string & strDefine )
{
	int iLen = (int)strlen( pszName );
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
 * @ingroup WsdlParser
 * @brief SOAP ������ �����ϴ� C++ ��� ������ �����Ѵ�.
 * @param pszClassName	Ŭ���� �̸�
 * @param pszFileName		Ȯ���ڸ� ������ ���� �̸�
 * @param pszFilePath		��� ���� ���
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSoapClass::MakeHeaderFile( const char * pszClassName, const char * pszFileName, const char * pszFilePath )
{
	std::string strDefine;

	FILE * fd = fopen( pszFilePath, "w" );
	if( fd == NULL )
	{
		CLog::Print( LOG_ERROR, "%s file(%s) open error\n", pszFilePath );
		return false;
	}

	GetDefineName( pszClassName, strDefine );

	fprintf( fd, "#ifndef %s\n", strDefine.c_str() );
	fprintf( fd, "#define %s\n", strDefine.c_str() );
	fprintf( fd, "\n" );
	fprintf( fd, "#include <string>\n" );
	fprintf( fd, "\n" );

	fprintf( fd, "class %s\n", pszClassName );
	fprintf( fd, "{\n" );
	fprintf( fd, "public:\n" );
	fprintf( fd, "\t%s();\n", pszClassName );
	fprintf( fd, "\t~%s();\n", pszClassName );
	fprintf( fd, "\n" );

	SOAP_METHOD_MAP::iterator itMethod;

	for( itMethod = m_clsMethodMap.begin(); itMethod != m_clsMethodMap.end(); ++itMethod )
	{
		std::string strCode;

		itMethod->second.GetPrototypeCode( NULL, strCode );
		fprintf( fd, "\t%s;\n", strCode.c_str() );
	}

	fprintf( fd, "};\n" );
	fprintf( fd, "\n" );

	fprintf( fd, "#endif\n" );

	fclose( fd );

	return true;
}

/**
 * @ingroup WsdlParser
 * @brief SOAP ������ �����ϴ� C++ �ҽ� ������ �����Ѵ�.
 * @param pszClassName	Ŭ���� �̸�
 * @param pszFileName		Ȯ���ڸ� ������ ���� �̸�
 * @param pszFilePath		��� ���� ���
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSoapClass::MakeSourceFile( const char * pszClassName, const char * pszFileName, const char * pszFilePath )
{
	FILE * fd = fopen( pszFilePath, "w" );
	if( fd == NULL )
	{
		CLog::Print( LOG_ERROR, "%s file(%s) open error\n", pszFilePath );
		return false;
	}

	fprintf( fd, "#include \"HttpClient.h\"\n" );
	fprintf( fd, "#include \"XmlElement.h\"\n" );
	fprintf( fd, "#include \"Log.h\"\n" );
	fprintf( fd, "#include \"MemoryDebug.h\"\n" );
	fprintf( fd, "#include \"%s.h\"\n\n", pszFileName );
	fprintf( fd, "%s::%s()\n", pszClassName, pszClassName );
	fprintf( fd, "{\n" );
	fprintf( fd, "}\n" );
	fprintf( fd, "\n" );
	fprintf( fd, "%s::~%s()\n", pszClassName, pszClassName );
	fprintf( fd, "{\n" );
	fprintf( fd, "}\n" );
	fprintf( fd, "\n" );

	SOAP_METHOD_MAP::iterator itMethod;

	for( itMethod = m_clsMethodMap.begin(); itMethod != m_clsMethodMap.end(); ++itMethod )
	{
		std::string strCode;

		itMethod->second.GetCode( pszClassName, this, strCode );
		fprintf( fd, "%s\n", strCode.c_str() );
	}

	fclose( fd );

	return true;
}
