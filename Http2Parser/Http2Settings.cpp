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

#include "Http2Settings.h"

CHttp2Settings::CHttp2Settings() : m_pszPacket(NULL), m_iPacketLen(0), m_iPacketSize(0)
{
}

CHttp2Settings::~CHttp2Settings()
{
	Delete();
}

bool CHttp2Settings::Add( uint16_t iName, uint32_t iValue )
{
	if( m_pszPacket == NULL )
	{
		m_pszPacket = (uint8_t *)malloc( 6 * 10 );
		if( m_pszPacket == NULL ) return false;
	}

	if( m_iPacketLen + 6 > m_iPacketSize ) return false;

	iName = htons( iName );
	iValue = htonl( iValue );

	memcpy( m_pszPacket + m_iPacketLen, &iName, 2 );
	memcpy( m_pszPacket + m_iPacketLen + 2, &iValue, 4 );
	m_iPacketLen += 6;

	return true;
}

void CHttp2Settings::Clear()
{
	m_iPacketLen = 0;
}

void CHttp2Settings::Delete()
{
	if( m_pszPacket )
	{
		free( m_pszPacket );
		m_pszPacket = NULL;
	}

	m_iPacketSize = 0;
	m_iPacketLen = 0;
}
