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

#ifndef _PCAP_TCP_MAP_H_
#define _PCAP_TCP_MAP_H_

#include "SipPlatformDefine.h"
#include <map>
#include <string>

class CPcapTcpInfo
{
public:
	CPcapTcpInfo();

	uint32_t	m_iSeqNum;
};

typedef std::map< std::string, CPcapTcpInfo > PCAP_TCP_MAP;

class CPcapTcpMap
{
public:
	CPcapTcpMap();
	~CPcapTcpMap();

	bool Select( const char * pszFromIp, u_short iFromPort, const char * pszToIp, u_short iToPort, CPcapTcpInfo ** ppclsInfo );

private:
	void GetKey( const char * pszFromIp, u_short iFromPort, const char * pszToIp, u_short iToPort, std::string & strKey );

	PCAP_TCP_MAP m_clsMap;
};

#endif
