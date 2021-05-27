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

#define IP_PROTO_TCP	6
#define IP_PROTO_UDP	17

/* 4 bytes IP address */
typedef struct _IpAddress_
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
} IpAddress;

/* IPv4 header */
typedef struct _IpHeader_
{
	u_char	ver_ihl;				// Version (4 bits) + Internet header length (4 bits)
	u_char	tos;						// Type of service 
	u_short tlen;						// Total length 
	u_short identification; // Identification
	u_short flags_fo;				// Flags (3 bits) + Fragment offset (13 bits)
	u_char	ttl;						// Time to live
	u_char	proto;					// Protocol
	u_short crc;						// Header checksum
	IpAddress	saddr;				// Source address
	IpAddress	daddr;				// Destination address
} IpHeader;

/* UDP header*/
typedef struct _UdpHeader_
{
	u_short sport;		// Source port
	u_short dport;		// Destination port
	u_short len;			// Datagram length
	u_short crc;			// Checksum
} UdpHeader;

/* TCP header */
typedef struct _TcpHeader_ 
{
	u_short sport;		// Source port
	u_short dport;		// Destination port
	u_int		seqnum;		// Sequence Number
	u_int		acknum;		// Acknowledgement number
	u_char	hlen;			// Header length - 상위 4bit 만 header 길이이다.
	u_char	flags;		// packet flags
	u_short win;			// Window size
	u_short crc;			// Header Checksum
	u_short urgptr;		// Urgent pointer
} TcpHeader;

bool StringToIpAddress( const char * pszIp, IpAddress & sttIpAddress );
u_short GetIpHeaderCheckSum( u_short sIpHeaderLen, u_char * pszIpHeader );

#endif
