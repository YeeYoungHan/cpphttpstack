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

#include "Http2Packet.h"
#include "SipTcp.h"

CHttp2Packet::CHttp2Packet()
{
}

CHttp2Packet::~CHttp2Packet()
{
}

/**
 * @ingroup HttpParser
 * @brief HTTP/2 프로토콜 기반으로 수신한 패킷을 저장한다.
 * @param pszPacket		패킷
 * @param iPacketLen	패킷 길이
 * @returns true 를 리턴한다.
 */
bool CHttp2Packet::AddPacket( const uint8_t * pszPacket, int iPacketLen )
{
	if( pszPacket && iPacketLen > 0 )
	{
		m_strBuf.append( (char *)pszPacket, iPacketLen );
	}

	return true;
}

/**
 * @ingroup Http2Parser
 * @brief HTTP/2 프로토콜 기반으로 수신한 패킷을 저장한다.
 * @param pclsPacket HTTP/1.1 프로토콜 기반으로 수신한 패킷 저장 객체
 * @returns true 를 리턴한다.
 */
bool CHttp2Packet::AddPacket( CHttpPacket * pclsPacket )
{
	std::string * pstrBuf = pclsPacket->GetBuf();

	if( pstrBuf->empty() == false )
	{
		m_strBuf.append( *pstrBuf );
		pstrBuf->clear();
	}

	return true;
}

/**
 * @ingroup Http2Parser
 * @brief HTTP/2 프로토콜 기반으로 수신한 패킷에서 하나의 frame 이 모두 수신되었는지 검사한다.
 * @param pclsFrame [out] 하나의 frame 저장 변수
 * @returns 하나의 frame 이 수신되었다면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Packet::GetFrame( CHttp2Frame * pclsFrame )
{
	int iBufLen = (int)m_strBuf.length();

	if( iBufLen < 9 ) return false;

	int iFrameLen = 0;

	memcpy( (char *)&iFrameLen + 1, m_strBuf.c_str(), 3 );
	iFrameLen = ntohl( iFrameLen );

	if( ( iFrameLen + 9 ) > iBufLen ) return false;
	
	bool bRes = pclsFrame->Set( (uint8_t *)m_strBuf.c_str(), iFrameLen + 9 );
	m_strBuf.erase( 0, iFrameLen + 9 );

	return bRes;
}
