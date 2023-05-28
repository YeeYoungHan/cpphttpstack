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
#include "ServerUtility.h"
#include "FileUtility.h"
#include "TestWhisper.h"
#include "MemoryDebug.h"

class CWhisperThreadArg
{
public:
	std::string m_strClientIp;
	int					m_iClientPort;
	std::string m_strFileName;
	std::string m_strModel;
};

THREAD_API WhisperThread( LPVOID lpParameter )
{
	CWhisperThreadArg * pclsArg = (CWhisperThreadArg *)lpParameter;
	char szCommand[1024], szBuf[8192];
	time_t iStartTime, iEndTime;

	time( &iStartTime );

	snprintf( szCommand, sizeof(szCommand), "whisper %s --language Korean --model %s --output_dir /tmp --output_format txt", pclsArg->m_strFileName.c_str(), pclsArg->m_strModel.c_str() );

	FILE * fd = popen( szCommand, "r" );
	if( fd )
	{
		snprintf( szBuf, sizeof(szBuf), "###START###\n" );
		gclsStack.SendWebSocketPacket( pclsArg->m_strClientIp.c_str(), pclsArg->m_iClientPort, szBuf, (int)strlen(szBuf) );

		memset( szBuf, 0, sizeof(szBuf) );
		while( fgets( szBuf, sizeof(szBuf), fd ) )
		{
			gclsStack.SendWebSocketPacket( pclsArg->m_strClientIp.c_str(), pclsArg->m_iClientPort, szBuf, (int)strlen(szBuf) );

			memset( szBuf, 0, sizeof(szBuf) );
		}

		pclose( fd );
	}
	else
	{
		snprintf( szBuf, sizeof(szBuf), "ERROR!!!!\n" );
		gclsStack.SendWebSocketPacket( pclsArg->m_strClientIp.c_str(), pclsArg->m_iClientPort, szBuf, (int)strlen(szBuf) );
	}

	time( &iEndTime );

	snprintf( szBuf, sizeof(szBuf), "time = %d sec\n", (int)(iEndTime - iStartTime) );
	gclsStack.SendWebSocketPacket( pclsArg->m_strClientIp.c_str(), pclsArg->m_iClientPort, szBuf, (int)strlen(szBuf) );

	gclsStack.SendWebSocketPacket( pclsArg->m_strClientIp.c_str(), pclsArg->m_iClientPort, "###END###", 9 );
	DelFile( pclsArg->m_strFileName.c_str() );

	delete pclsArg;

	return 0;
}

bool StartWhisperThread( const char * pszClientIp, int iClientPort, const char * pszFileName, const char * pszModel )
{
	CWhisperThreadArg * pclsArg = new CWhisperThreadArg();
	if( pclsArg == NULL )
	{
		return false;
	}

	pclsArg->m_strClientIp = pszClientIp;
	pclsArg->m_iClientPort = iClientPort;
	pclsArg->m_strFileName = pszFileName;
	pclsArg->m_strModel = pszModel;

	if( StartThread( "WhisperThread", WhisperThread, pclsArg ) == false )
	{
		delete pclsArg;
		return false;
	}

	return true;
}
