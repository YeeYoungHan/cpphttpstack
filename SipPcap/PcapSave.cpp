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

#include "PcapSave.h"
#include "PacketHeader.h"

CPcapSave::CPcapSave() : m_pPcap(NULL), m_pPcapDump(NULL), m_pszPacket(NULL), m_iPacketSize(0)
{
}

CPcapSave::~CPcapSave()
{
	Close();
}

/**
 * @brief SIP �޽����� ������ pcap ������ ����.
 * @param pszFileName pcap ���� �̸� ( full path )
 * @param iSnapLen		�ϳ��� ��Ŷ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CPcapSave::Open( const char * pszFileName, int iSnapLen )
{
	if( m_pPcapDump ) return false;
	if( iSnapLen > 1000000000 ) return false;

	char	szError[1024];

	memset( szError, 0, sizeof(szError) );

	m_pszPacket = (uint8_t *)malloc( iSnapLen );
	if( m_pszPacket == NULL ) return false;

	m_iPacketSize = iSnapLen;

	m_pPcap = pcap_open_dead( DLT_EN10MB, iSnapLen );
	if( m_pPcap == NULL ) return false;

	m_pPcapDump = pcap_dump_open( m_pPcap, pszFileName );
	if( m_pPcapDump == NULL )
	{
		pcap_close( m_pPcap );
		m_pPcap = NULL;

		return false;
	}

	return true;
}

/**
 * @brief pcap ���Ͽ� ��Ŷ�� �����Ѵ�.
 * @param psttHeader			ĸó ����
 * @param psttPacketData	��Ŷ 
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CPcapSave::Write( const struct pcap_pkthdr * psttHeader, const u_char * psttPacketData )
{
	if( m_pPcapDump == NULL ) return false;

	pcap_dump( (u_char *)m_pPcapDump, psttHeader, psttPacketData );

	return true;
}

/**
 * @brief pcap ���Ͽ� UDP SIP �޽����� �����Ѵ�.
 * @param psttTime	��Ŷ �ð�
 * @param pszFromIp	source IP �ּ�
 * @param iFromPort source ��Ʈ ��ȣ
 * @param pszToIp		destination IP �ּ�
 * @param iToPort		destination ��Ʈ ��ȣ
 * @param pszData		UDP body
 * @param iDataLen	UDP body ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CPcapSave::WriteUdp( struct timeval * psttTime, const char * pszFromIp, u_short iFromPort, const char * pszToIp, u_short iToPort, const char * pszData, int iDataLen )
{
	struct pcap_pkthdr sttHeader;
	IpHeader	* psttIpHeader = (IpHeader *)(m_pszPacket + 14);
	UdpHeader	* psttUdpHeader = (UdpHeader *)(m_pszPacket + 14 + sizeof(IpHeader));

	memset( m_pszPacket, 0, sizeof(m_pszPacket) );
	m_pszPacket[12] = 0x08;

	sttHeader.caplen = 14 + sizeof(IpHeader) + sizeof(UdpHeader) + iDataLen;
	if( (int)sttHeader.caplen > m_iPacketSize ) return false;

	sttHeader.len = sttHeader.caplen;
	memcpy( &sttHeader.ts, psttTime, sizeof(sttHeader.ts) );

	psttIpHeader->ver_ihl = 0x45;
	psttIpHeader->tlen = htons( sttHeader.caplen - 14 );
	psttIpHeader->ttl = 48;
	psttIpHeader->proto = IP_PROTO_UDP;

	StringToIpAddress( pszFromIp, psttIpHeader->saddr );
	StringToIpAddress( pszToIp, psttIpHeader->daddr );

	psttIpHeader->crc = htons( GetIpHeaderCheckSum( 20, (u_char *)psttIpHeader ) );

	memcpy( m_pszPacket + 8, &psttIpHeader->saddr, 4 );
	memcpy( m_pszPacket + 2, &psttIpHeader->daddr, 4 );

	psttUdpHeader->sport = htons( iFromPort );
	psttUdpHeader->dport = htons( iToPort );
	psttUdpHeader->len = htons( sttHeader.caplen - 14 - sizeof(IpHeader) );

	memcpy( m_pszPacket + 14 + sizeof(IpHeader) + sizeof(UdpHeader), pszData, iDataLen );

	return Write( &sttHeader, (u_char *)m_pszPacket );
}

bool CPcapSave::WriteTcp( struct timeval * psttTime, const char * pszFromIp, u_short iFromPort, const char * pszToIp, u_short iToPort, const char * pszData, int iDataLen )
{
	struct pcap_pkthdr sttHeader;
	IpHeader	* psttIpHeader = (IpHeader *)(m_pszPacket + 14);
	TcpHeader	* psttTcpHeader = (TcpHeader *)(m_pszPacket + 14 + sizeof(IpHeader));

	memset( m_pszPacket, 0, sizeof(m_pszPacket) );
	m_pszPacket[12] = 0x08;

	sttHeader.caplen = 14 + sizeof(IpHeader) + sizeof(TcpHeader) + iDataLen;
	if( (int)sttHeader.caplen > m_iPacketSize ) return false;

	sttHeader.len = sttHeader.caplen;
	memcpy( &sttHeader.ts, psttTime, sizeof(sttHeader.ts) );

	psttIpHeader->ver_ihl = 0x45;
	psttIpHeader->tlen = htons( sttHeader.caplen - 14 );
	psttIpHeader->ttl = 48;
	psttIpHeader->proto = IP_PROTO_TCP;

	StringToIpAddress( pszFromIp, psttIpHeader->saddr );
	StringToIpAddress( pszToIp, psttIpHeader->daddr );

	psttIpHeader->crc = htons( GetIpHeaderCheckSum( 20, (u_char *)psttIpHeader ) );

	memcpy( m_pszPacket + 8, &psttIpHeader->saddr, 4 );
	memcpy( m_pszPacket + 2, &psttIpHeader->daddr, 4 );

	psttTcpHeader->sport = htons( iFromPort );
	psttTcpHeader->dport = htons( iToPort );
	psttTcpHeader->seqnum = 1;
	psttTcpHeader->acknum = 1;
	psttTcpHeader->hlen = 0x50;
	psttTcpHeader->flags = 0;
	psttTcpHeader->win = htons( 8000 );
	psttTcpHeader->crc = 0;
	psttTcpHeader->urgptr = 0;

	memcpy( m_pszPacket + 14 + sizeof(IpHeader) + sizeof(TcpHeader), pszData, iDataLen );

	return Write( &sttHeader, (u_char *)m_pszPacket );
}

/**
 * @brief pcap ������ �ݴ´�.
 */
void CPcapSave::Close( )
{
	if( m_pPcapDump )
	{
		pcap_dump_close( m_pPcapDump );
		m_pPcapDump = NULL;
	}

	if( m_pPcap )
	{
		pcap_close( m_pPcap );
		m_pPcap = NULL;
	}

	if( m_pszPacket )
	{
		free( m_pszPacket );
		m_pszPacket = NULL;
	}

	m_iPacketSize = 0;
}

bool CPcapSave::IsOpen()
{
	if( m_pPcapDump ) return true;

	return false;
}
