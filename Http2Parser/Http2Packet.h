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

#ifndef _HTTP2_PACKET_H_
#define _HTTP2_PACKET_H_

#include "SipPlatformDefine.h"
#include "Http2Frame.h"
#include "HttpPacket.h"
#include <string>

/**
 * @ingroup Http2Parser
 * @brief HTTP/2 수신 패킷 저장 클래스
 */
class CHttp2Packet
{
public:
	CHttp2Packet();
	~CHttp2Packet();

	bool AddPacket( const uint8_t * pszPacket, int iPacketLen );
	bool AddPacket( CHttpPacket * pclsPacket );
	bool GetFrame( CHttp2Frame * pclsFrame );

private:
	std::string m_strBuf;
};

#endif
