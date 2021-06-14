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

#ifndef _HTTP2_FRAME_H_
#define _HTTP2_FRAME_H_

#include "SipPlatformDefine.h"
#include "Log.h"

/**
 * @ingroup Http2Parser
 * @brief HTTP/2 frame Å¬·¡½º
 */
class CHttp2Frame
{
public:
	CHttp2Frame();
	~CHttp2Frame();

	bool Set( uint8_t cType, uint8_t cFlag, uint32_t iStreamIdentifier, uint8_t * pszBody, int iBodyLen );
	bool Set( uint8_t * pszPacket, int iPacketLen );
	bool SetFlags( uint8_t cFlag );
	void Clear();
	void Delete();

	uint8_t GetType();
	uint8_t GetFlags();
	uint32_t GetStreamIdentifier();
	uint8_t * GetBody();
	int GetBodyLen();

	void PrintLog( EnumLogLevel eLogLevel, const char * pszIp, int iPort, bool bSend );

	uint8_t	* m_pszPacket;
	int m_iPacketLen;

private:
	int m_iPacketSize;
};

#endif
