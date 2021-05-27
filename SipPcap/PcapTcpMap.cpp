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

#include "PcapTcpMap.h"

CPcapTcpInfo::CPcapTcpInfo() : m_iSeqNum(0)
{
}

CPcapTcpMap::CPcapTcpMap()
{
}

CPcapTcpMap::~CPcapTcpMap()
{
}

bool CPcapTcpMap::Select( const char * pszFromIp, u_short iFromPort, const char * pszToIp, u_short iToPort, CPcapTcpInfo ** ppclsInfo )
{
	PCAP_TCP_MAP::iterator itMap;
	std::string strKey;

	GetKey( pszFromIp, iFromPort, pszToIp, iToPort, strKey );

	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		CPcapTcpInfo clsInfo;

		m_clsMap.insert( PCAP_TCP_MAP::value_type( strKey, clsInfo ) );
		itMap = m_clsMap.find( strKey );
		if( itMap == m_clsMap.end() )
		{
			return false;
		}
	}

	*ppclsInfo = &(itMap->second);

	return true;
}

void CPcapTcpMap::GetKey( const char * pszFromIp, u_short iFromPort, const char * pszToIp, u_short iToPort, std::string & strKey )
{
	char szBuf[128];

	snprintf( szBuf, sizeof(szBuf), "%s:%d-%s:%d", pszFromIp, iFromPort, pszToIp, iToPort );
	strKey = szBuf;
}
