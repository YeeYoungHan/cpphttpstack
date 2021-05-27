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
#include <stdio.h>
#include <pcap.h>
#include "PacketHeader.h"

/**
 * @brief IP �ּ� ���ڿ��� IpAddress ����ü�� �����Ѵ�.
 * @param pszIp					[in] IP �ּ� ���ڿ�
 * @param sttIpAddress	[out] IpAddress ����ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool StringToIpAddress( const char * pszIp, IpAddress & sttIpAddress )
{
	int iIp1, iIp2, iIp3, iIp4;

	if( sscanf( pszIp, "%d.%d.%d.%d", &iIp1, &iIp2, &iIp3, &iIp4 ) != 4 ) return false;

	sttIpAddress.byte1 = iIp1;
	sttIpAddress.byte2 = iIp2;
	sttIpAddress.byte3 = iIp3;
	sttIpAddress.byte4 = iIp4;

	return true;
}

/**
 * @brief IP ��� checksum �� ����Ͽ� �����Ѵ�.
 * @param sIpHeaderLen	IP ��� ����
 * @param pszIpHeader		IP ���
 * @returns IP ��� checksum �� �����Ѵ�.
 */
uint16_t GetIpHeaderCheckSum( uint16_t sIpHeaderLen, uint8_t * pszIpHeader )
{
	uint16_t sValue;
	uint32_t iSum = 0;
	uint16_t i, sBuf1, sBuf2;
    
	for( i = 0; i < sIpHeaderLen; i = i + 2 )
	{
		if( i == 10 )
		{
			sBuf1 = 0x00;
			sBuf2 = 0x00;
		}
		else
		{
			sBuf1 = pszIpHeader[i];
			sBuf2 = pszIpHeader[i+1];
		}

		sValue = ( ( sBuf1 << 8) & 0xFF00 ) + ( sBuf2 & 0xFF );
		iSum = iSum + ( uint32_t )sValue;	
	}
	
	while( iSum >> 16 )
	  iSum = ( iSum & 0xFFFF ) + ( iSum >> 16 );

	iSum = ~iSum;
	
	return ((uint16_t) iSum);
}
