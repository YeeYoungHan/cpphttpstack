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

#ifndef _WHISPER_MAP_H_
#define _WHISPER_MAP_H_

#include <string>
#include <map>
#include "SipMutex.h"

class CWhisperInfo
{
public:
	CWhisperInfo();

	std::string m_strClientIp;
	std::string m_strFileName;
	std::string m_strExt;
	std::string m_strModel;

	int					m_iClientPort;
	int					m_iFileSize;
	int					m_iRecvSize;

	FILE				* m_psttFd;
};

typedef std::map< std::string, CWhisperInfo > WHISPER_MAP;

class CWhisperMap
{
public:
	CWhisperMap();
	~CWhisperMap();

	bool Insert( const char * pszClientIp, int iClientPort, const char * pszModel, const char * pszExt, int iFileSize );
	bool Delete( const char * pszClientIp, int iClientPort );

	bool SaveFile( const char * pszClientIp, int iClientPort, std::string & strData );

private:
	void GetKey( const char * pszClientIp, int iClientPort, std::string & strKey );

	WHISPER_MAP m_clsMap;
	CSipMutex		m_clsMutex;
	int					m_iFileIndex;
};

extern CWhisperMap gclsWhisperMap;

#endif
