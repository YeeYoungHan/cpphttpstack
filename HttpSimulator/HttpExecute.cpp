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

#include "SipPlatformDefine.h"
#include "HttpSimulator.h"
#include "HttpClient2.h"
#include "FileUtility.h"
#include "StringUtility.h"
#include "Log.h"

/**
 * @ingroup HttpSimulator
 * @brief 프로그램을 실행시켜서 stdout 으로 출력된 내용을 저장한다.
 * @param strExecute	프로그램 full path
 * @param strPostBody	[out] 프로그램의 출력 내용을 저장하는 변수
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
static bool GetPostBody( std::string & strExecute, std::string & strPostBody )
{
	strPostBody.clear();

	FILE * fd = popen( strExecute.c_str(), "rb" );
	if( fd == NULL )
	{
		CLog::Print( LOG_ERROR, "popen(%s) error(%d)", strExecute.c_str(), GetError() );
		return false;
	}

	char szBuf[8192];
	int n;

	while( 1 )
	{
		n = fread( szBuf, 1, sizeof(szBuf), fd );
		if( n <= 0 ) break;

		strPostBody.append( szBuf, n );
	}

	pclose( fd );

	if( strPostBody.empty() ) return false;

	return true;
}

/**
 * @ingroup HttpSimulator
 * @brief 수신한 HTTP body 를 파일에 저장한다.
 * @param strRecvFileName				파일 이름
 * @param strOutputContentType	HTTP Content-Type
 * @param strOutputBody					HTTP body
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
static bool SaveBodyToFile( std::string & strRecvFileName, std::string & strOutputContentType, std::string & strOutputBody )
{
#ifdef WIN32
	const char * pszContentType = strOutputContentType.c_str();

	if( strstr( pszContentType, "UTF-8" ) )
	{
		std::string strOutput;

		if( Utf8ToAnsi( strOutputBody.c_str(), strOutput ) )
		{
			strOutputBody = strOutput;
		}
	}
#endif

	std::string strFolder;

	if( GetFolderPathOfFilePath( strRecvFileName.c_str(), strFolder ) )
	{
		CDirectory::Create( strFolder.c_str() );
	}
			
	FILE * fd = fopen( strRecvFileName.c_str(), "wb" );
	if( fd == NULL )
	{
		CLog::Print( LOG_ERROR, "fopen(%s) error(%d)", strRecvFileName.c_str(), GetError() );
		return false;
	}

	fwrite( strOutputBody.c_str(), 1, strOutputBody.length(), fd );
	fclose( fd );

	return true;
}

/**
 * @ingroup HttpSimulator
 * @brief HTTP 시뮬레이션 명령을 모두 실행한다.
 * @param clsCommandList HTTP 시뮬레이션 명령들을 저장하는 자료구조
 * @returns true 를 리턴한다.
 */
bool Execute( HTTP_SIMULATOR_COMMAND_LIST & clsCommandList )
{
	HTTP_SIMULATOR_COMMAND_LIST::iterator itCL;
	CHttpClient2 clsClient;
	std::string strOutputContentType, strOutputBody;
	bool bRes;

	for( itCL = clsCommandList.begin(); itCL != clsCommandList.end(); ++itCL )
	{
		if( !strcasecmp( itCL->m_strMethod.c_str(), "POST" ) )
		{
			if( itCL->m_strBodyExecute.empty() == false )
			{
				if( GetPostBody( itCL->m_strBodyExecute, itCL->m_strBody ) == false ) break;
			}

			bRes = clsClient.DoPost( itCL->m_strUrl.c_str(), "application/x-www-form-urlencoded", itCL->m_strBody.c_str(), itCL->m_strBody.length(), strOutputContentType, strOutputBody );
		}
		else
		{
			bRes = clsClient.DoGet( itCL->m_strUrl.c_str(), strOutputContentType, strOutputBody );
		}

		if( itCL->m_strRecvFileName.empty() == false )
		{
			if( SaveBodyToFile( itCL->m_strRecvFileName, strOutputContentType, strOutputBody ) == false ) break;
		}

		if( bRes )
		{
			CLog::Print( LOG_INFO, "url[%s] ok", itCL->m_strUrl.c_str() );
		}
		else
		{
			CLog::Print( LOG_ERROR, "url[%s] error - status code(%d)", itCL->m_strUrl.c_str(), clsClient.GetStatusCode() );
			break;
		}
	}

	return true;
}
