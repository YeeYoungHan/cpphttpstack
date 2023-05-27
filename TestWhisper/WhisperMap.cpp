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
#include "WhisperMap.h"
#include "SipTcp.h"
#include "Log.h"
#include "TestWhisper.h"
#include "MemoryDebug.h"

CWhisperMap gclsWhisperMap;

CWhisperInfo::CWhisperInfo() : m_iFileSize(0), m_iRecvSize(0), m_psttFd(NULL)
{
}

CWhisperMap::CWhisperMap() : m_iFileIndex(0)
{
}

CWhisperMap::~CWhisperMap()
{
}

bool CWhisperMap::Insert( const char * pszClientIp, int iClientPort, const char * pszModel, const char * pszExt, int iFileSize )
{
	WHISPER_MAP::iterator itMap;
	std::string strKey;

	GetKey( pszClientIp, iClientPort, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		CWhisperInfo clsInfo;

		clsInfo.m_strClientIp = pszClientIp;
		clsInfo.m_iClientPort = iClientPort;
		clsInfo.m_strModel = pszModel;
		clsInfo.m_strExt = pszExt;
		clsInfo.m_iFileSize = iFileSize;

		m_clsMap.insert( WHISPER_MAP::value_type( strKey, clsInfo ) );
	}
	m_clsMutex.release();

	return true;
}

bool CWhisperMap::Delete( const char * pszClientIp, int iClientPort )
{
	WHISPER_MAP::iterator itMap;
	std::string strKey;

	GetKey( pszClientIp, iClientPort, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		if( itMap->second.m_psttFd )
		{
			fclose( itMap->second.m_psttFd );
			itMap->second.m_psttFd = NULL;
		}

		m_clsMap.erase( itMap );
	}
	m_clsMutex.release();

	return true;
}

bool CWhisperMap::SaveFile( const char * pszClientIp, int iClientPort, std::string & strData )
{
	WHISPER_MAP::iterator itMap;
	std::string strKey;

	GetKey( pszClientIp, iClientPort, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		if( itMap->second.m_psttFd == NULL && itMap->second.m_strFileName.empty() )
		{
			char szFileName[255];

			++m_iFileIndex;
			snprintf( szFileName, sizeof(szFileName), "%d.%s", m_iFileIndex, itMap->second.m_strExt.c_str() );

			itMap->second.m_psttFd = fopen( szFileName, "wb" );
			if( itMap->second.m_psttFd == NULL )
			{
				CLog::Print( LOG_ERROR, "fopen(%s) error(%d)", szFileName, GetError() );
			}
			else
			{
				itMap->second.m_strFileName = szFileName;
			}
		}

		if( itMap->second.m_psttFd )
		{
			fwrite( strData.c_str(), 1, strData.length(), itMap->second.m_psttFd );
			itMap->second.m_iRecvSize += strData.length();

			if( itMap->second.m_iRecvSize >= itMap->second.m_iFileSize )
			{
				fclose( itMap->second.m_psttFd );
				itMap->second.m_psttFd = NULL;

				StartWhisperThread( itMap->second.m_strClientIp.c_str(), itMap->second.m_iClientPort, itMap->second.m_strFileName.c_str(), itMap->second.m_strModel.c_str() );
			}
		}
	}
	m_clsMutex.release();

	return true;
}

void CWhisperMap::GetKey( const char * pszClientIp, int iClientPort, std::string & strKey )
{
	char szPort[11];

	snprintf( szPort, sizeof(szPort), "%d", iClientPort );

	strKey = pszClientIp;
	strKey.append( ":" );
	strKey.append( szPort );
}
