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

#ifndef _PACKET_HEADER_H_
#define _PACKET_HEADER_H_

#include "SipPlatformDefine.h"

#define IP_PROTO_TCP	6
#define IP_PROTO_UDP	17

/* 4 bytes IP address */
typedef struct _IpAddress_
{
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
} IpAddress;

/* IPv4 header */
typedef struct _IpHeader_
{
	uint8_t		ver_ihl;				// Version (4 bits) + Internet header length (4 bits)
	uint8_t		tos;						// Type of service 
	uint16_t	tlen;						// Total length 
	uint16_t	identification; // Identification
	uint16_t	flags_fo;				// Flags (3 bits) + Fragment offset (13 bits)
	uint8_t		ttl;						// Time to live
	uint8_t		proto;					// Protocol
	uint16_t	crc;						// Header checksum
	IpAddress	saddr;					// Source address
	IpAddress	daddr;					// Destination address
} IpHeader;

/* UDP header*/
typedef struct _UdpHeader_
{
	uint16_t sport;		// Source port
	uint16_t dport;		// Destination port
	uint16_t len;			// Datagram length
	uint16_t crc;			// Checksum
} UdpHeader;

/* TCP header */
typedef struct _TcpHeader_ 
{
	uint16_t	sport;		// Source port
	uint16_t	dport;		// Destination port
	uint32_t	seqnum;		// Sequence Number
	uint32_t	acknum;		// Acknowledgement number
	uint8_t		hlen;			// Header length - 상위 4bit 만 header 길이이다.
	uint8_t		flags;		// packet flags
	uint16_t	win;			// Window size
	uint16_t	crc;			// Header Checksum
	uint16_t	urgptr;		// Urgent pointer
} TcpHeader;

bool StringToIpAddress( const char * pszIp, IpAddress & sttIpAddress );
uint16_t GetIpHeaderCheckSum( uint16_t sIpHeaderLen, uint8_t * pszIpHeader );

#endif
