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

#include "FileUtility.h"
#include <sys/stat.h>

#ifndef WIN32
#include <unistd.h>
#endif

#include "MemoryDebug.h"

/**
 * @ingroup SipPlatform
 * @brief ������ �����ϴ��� �˻��Ѵ�.
 * @param pszFileName ���� �̸�
 * @returns ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsExistFile( const char * pszFileName )
{
	struct stat sttStat;

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return false;
	}

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief ���� ũ�⸦ �����Ѵ�.
 * @param pszFileName ���� �̸�
 * @returns ������ �����ϸ� ���� ũ�⸦ �����ϰ� �׷��� ������ 0 �� �����Ѵ�.
 */
int64_t GetFileSize( const char * pszFileName )
{
	struct stat sttStat;

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return 0;
	}

	return sttStat.st_size;
}

/**
 * @ingroup SipPlatform
 * @brief ���� �̸����� ���� Ȯ���ڸ� �����´�.
 * @param pszFileName ���� �̸�
 * @param strExt			���� Ȯ���� ���� ����
 * @returns ���� Ȯ���ڰ� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool GetFileExt( const char * pszFileName, std::string & strExt )
{
	int iLen = (int)strlen( pszFileName );

	strExt.clear();

	for( int i = iLen - 1; i >= 0; --i )
	{
		if( pszFileName[i] == '.' )
		{
			strExt = pszFileName + i + 1;
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief ���� ��ο��� ���� ��� ��������
 * @param pszFilePath	���� ���
 * @param strFolder		���� ��� ���� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool GetFolderPathOfFilePath( const char * pszFilePath, std::string & strFolder )
{
	strFolder.clear();

	if( pszFilePath == NULL ) return false;

	int iLen = (int)strlen( pszFilePath );
	if( iLen < 2 ) return false;

#ifdef WIN32
	if( pszFilePath[iLen-1] == '\\' ) return false;
#else
	if( pszFilePath[iLen-1] == '/' ) return false;
#endif

	for( int i = iLen - 2; i >= 0; --i )
	{
#ifdef WIN32
		if( pszFilePath[i] == '\\' )
#else
		if( pszFilePath[i] == '/' )
#endif
		{

#ifndef WIN32
			if( i == 0 )
			{
				strFolder = "/";
				return true;
			}
#endif

			strFolder.append( pszFilePath, i );
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief ���� ��ο��� ���� �̸� ��������
 * @param pszFilePath ���� ���
 * @param strFileName ���� �̸� ���� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool GetFileNameOfFilePath( const char * pszFilePath, std::string & strFileName )
{
	strFileName.clear();

	if( pszFilePath == NULL ) return false;

	int iLen = (int)strlen( pszFilePath );
	if( iLen < 2 ) return false;

#ifdef WIN32
	if( pszFilePath[iLen-1] == '\\' ) return false;
#else
	if( pszFilePath[iLen-1] == '/' ) return false;
#endif

	for( int i = iLen - 2; i >= 0; --i )
	{
#ifdef WIN32
		if( pszFilePath[i] == '\\' )
#else
		if( pszFilePath[i] == '/' )
#endif
		{
			strFileName.append( pszFilePath + i + 1 );
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief ������ �����Ѵ�.
 * @param pszFileName ���� �̸�
 */
void DelFile( const char * pszFileName )
{
#ifdef WIN32
	DeleteFile( pszFileName );
#else
	unlink( pszFileName );
#endif
}
