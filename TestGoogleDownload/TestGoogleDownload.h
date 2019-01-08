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

#ifndef _TEST_GOOGLE_DOWNLOAD_H_
#define _TEST_GOOGLE_DOWNLOAD_H_

#include "SipPlatformDefine.h"
#include "HttpClient.h"
#include "StringUtility.h"
#include "FileUtility.h"
#include "StringMap.h"
#include "Directory.h"

#define URL_PRINT_FILEPATH "c:\\temp\\url.txt"

extern bool gbStop;

// GetFileName.cpp
bool GetFileName( std::string & strUrl, std::string & strFileName );

// PrintUrl.cpp
bool PrintUrl( const char * pszExt, const char * pszSearch );

#endif
