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
#include "HtmlElementUrl.h"
#include "HttpClient2.h"
#include "FileUtility.h"
#include "StringUtility.h"
#include "Log.h"

/**
 * @ingroup HttpSimulator
 * @brief ���α׷��� ������Ѽ� stdout ���� ��µ� ������ �����Ѵ�.
 * @param strExecute	���α׷� full path
 * @param strPostBody	[out] ���α׷��� ��� ������ �����ϴ� ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief ������ HTTP body �� ���Ͽ� �����Ѵ�.
 * @param strRecvFileName				���� �̸�
 * @param strOutputContentType	HTTP Content-Type
 * @param strOutputBody					HTTP body
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief HTML ���ڿ����� URL ����Ʈ�� �����´�.
 * @param strHtml			HTML ���ڿ�
 * @param clsUrlList	[out] URL ����Ʈ
 * @returns URL ����Ʈ�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool GetHtmlUrlList( std::string & strHtml, STRING_LIST & clsUrlList )
{
	CHtmlElementUrl clsHtml;
	
	if( clsHtml.Parse( strHtml, E_HEO_NOT_CHECK_END_TAG ) == -1 )
	{
		return false;
	}

	return clsHtml.GetUrlList( clsUrlList );
}

/**
 * @ingroup HttpSimulator
 * @brief URL ���� �ֻ��� URL �� ���� URL �� �θ� URL �� �����´�.
 * @param strUrl				URL	(��: http://127.0.0.1/test/index.html )
 * @param strRootUrl		[out] �ֻ��� URL (��: http://127.0.0.1 )
 * @param strParentUrl	[out] �θ� URL (��: http://127.0.0.1/test )
 * @returns true �� �����Ѵ�.
 */
bool GetRootParentUrl( std::string & strUrl, std::string & strRootUrl, std::string & strParentUrl )
{
	const char * pszUrl = strUrl.c_str();
	int iLen = (int)strUrl.length();
	int iParentUrlEnd = 0;
	char cType = 0;

	for( int i = 4; i < iLen; ++i )
	{
		if( cType == 0 )
		{
			if( pszUrl[i-2] == ':' && pszUrl[i-1] == '/' && pszUrl[i] == '/' )
			{
				cType = 1;
			}
		}
		else if( cType == 1 )
		{
			if( pszUrl[i] == '/' )
			{
				strRootUrl.append( pszUrl, i );
				cType = 2;
			}
		}
		else if( cType == 2 )
		{
			if( pszUrl[i] == '/' )
			{
				iParentUrlEnd = i;
			}
			else if( pszUrl[i] == '?' )
			{
				break;
			}
		}
	}

	if( iParentUrlEnd )
	{
		strParentUrl.append( pszUrl, iParentUrlEnd );
	}

	if( strRootUrl.empty() )
	{
		strRootUrl = strUrl;
	}

	if( strParentUrl.empty() )
	{
		strParentUrl = strUrl;
	}

	return true;
}

/**
 * @ingroup HttpSimulator
 * @brief HTTP �ùķ��̼� ����� ��� �����Ѵ�.
 * @param clsCommandList HTTP �ùķ��̼� ��ɵ��� �����ϴ� �ڷᱸ��
 * @returns true �� �����Ѵ�.
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

		if( bRes == false )
		{
			CLog::Print( LOG_ERROR, "url[%s] error - status code(%d)", itCL->m_strUrl.c_str(), clsClient.GetStatusCode() );
			break;
		}

		CLog::Print( LOG_INFO, "url[%s] ok", itCL->m_strUrl.c_str() );

		if( itCL->m_bDownloadAll )
		{
			STRING_LIST clsUrlList;
			STRING_LIST::iterator itSL;

			if( GetHtmlUrlList( strOutputBody, clsUrlList ) )
			{
				std::string strRootUrl, strParentUrl, strNewUrl;

				GetRootParentUrl( itCL->m_strUrl, strRootUrl, strParentUrl );

				for( itSL = clsUrlList.begin(); itSL != clsUrlList.end(); ++itSL )
				{
					if( (*itSL)[0] == 'h' )
					{
						continue;
					}
					else if( (*itSL)[0] == '/' )
					{
						strNewUrl = strRootUrl + *itSL;
					}
					else
					{
						strNewUrl = strParentUrl + *itSL;
					}

					*itSL = strNewUrl;
				}

				for( itSL = clsUrlList.begin(); itSL != clsUrlList.end(); ++itSL )
				{
					if( clsClient.DoGet( itSL->c_str(), strOutputContentType, strOutputBody ) == false )
					{
						break;
					}
				}
			}
		}
	}

	return true;
}
