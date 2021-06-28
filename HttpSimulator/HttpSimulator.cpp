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

#include "HttpSimulatorSetup.h"
#include "HttpClient2.h"
#include "StringUtility.h"
#include "Log.h"

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {setup file}\n", argv[0] );
		return 0;
	}

	CHttpSimulatorSetup clsSetup;

	if( clsSetup.Read( argv[1] ) == false )
	{
		return 0;
	}

	HTTP_SIMULATOR_COMMAND_LIST::iterator itCL;
	CHttpClient2 clsClient;
	std::string strOutputContentType, strOutputBody;
	bool bRes;

	for( itCL = clsSetup.m_clsCommandList.begin(); itCL != clsSetup.m_clsCommandList.end(); ++itCL )
	{
		if( !strcasecmp( itCL->m_strMethod.c_str(), "POST" ) )
		{
			bRes = clsClient.DoPost( itCL->m_strUrl.c_str(), "application/x-www-form-urlencoded", itCL->m_strBody.c_str(), itCL->m_strBody.length(), strOutputContentType, strOutputBody );
		}
		else
		{
			bRes = clsClient.DoGet( itCL->m_strUrl.c_str(), strOutputContentType, strOutputBody );
		}

#ifdef WIN32
		const char * pszContentType = strOutputContentType.c_str();

		if( strstr( pszContentType, "UTF-8" ) )
		{
			std::string strOutput;

			if( Utf8ToAnsi( strOutputBody.c_str(), strOutput ) )
			{
				CLog::Print( LOG_DEBUG, "body(%s)", strOutput.c_str() );
			}
		}
#endif

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

	return 0;
}
