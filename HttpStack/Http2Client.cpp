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

#include "Http2Client.h"
#include "Log.h"

CHttp2Client::CHttp2Client() : m_hSocket(INVALID_SOCKET), m_psttSsl(NULL)
{
}

CHttp2Client::~CHttp2Client()
{
	Close();
}

bool CHttp2Client::Connect( const char * pszIp, int iPort, const char * pszClientPemFileName )
{
	m_hSocket = TcpConnect( pszIp, iPort );
	if( m_hSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s TcpConnect(%s:%d) error", __FUNCTION__, pszIp, iPort );
		return false;
	}

	CLog::Print( LOG_NETWORK, "TcpConnect(%s:%d) success", pszIp, iPort );

	if( SSLConnect( m_hSocket, &m_psttSsl ) == false )
	{
		CLog::Print( LOG_ERROR, "%s SSLConnect(%s:%d) error", __FUNCTION__, pszIp, iPort );
		Close();
		return false;
	}

	CLog::Print( LOG_NETWORK, "SSLConnect(%s:%d) success", pszIp, iPort );

	return true;
}

bool CHttp2Client::Close()
{
	if( m_psttSsl )
	{
		SSLClose( m_psttSsl );
		m_psttSsl = NULL;
	}

	if( m_hSocket != INVALID_SOCKET )
	{
		closesocket( m_hSocket );
		m_hSocket = INVALID_SOCKET;
	}

	return true;
}

bool CHttp2Client::DoPost( const char * pszPath, HTTP_HEADER_LIST * pclsHeaderList, const char * pszInputContentType, const char * pszInputBody, int iInputBodyLen, std::string & strOutputContentType, std::string & strOutputBody )
{
	return true;
}
