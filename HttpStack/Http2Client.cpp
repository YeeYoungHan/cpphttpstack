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

#include "Http2Define.h"
#include "Http2Client.h"
#include "Log.h"

CHttp2Client::CHttp2Client() : m_hSocket(INVALID_SOCKET), m_psttSsl(NULL), m_psttCtx(NULL), m_iStreamIdentifier(0), m_iPort(0)
{
	InitNetwork();
}

CHttp2Client::~CHttp2Client()
{
	Close();
}

bool CHttp2Client::Connect( const char * pszIp, int iPort, const char * pszClientPemFileName )
{
	if( m_hSocket != INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s m_hSocket != INVALID_SOCKET", __FUNCTION__ );
		return false;
	}

	m_hSocket = TcpConnect( pszIp, iPort );
	if( m_hSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s TcpConnect(%s:%d) error", __FUNCTION__, pszIp, iPort );
		return false;
	}

	CLog::Print( LOG_NETWORK, "TcpConnect(%s:%d) success", pszIp, iPort );

	if( pszClientPemFileName )
	{
		m_psttCtx = SSLClientStart( pszClientPemFileName, E_TLS_1_2 );
		if( m_psttCtx == NULL )
		{
			CLog::Print( LOG_ERROR, "%s SSLClientStart(%s) error", __FUNCTION__, pszClientPemFileName );
			Close();
			return false;
		}

		uint8_t szProto[] = { 2, 'h', '2' };

		SSL_CTX_set_alpn_protos( m_psttCtx, szProto, sizeof(szProto) );

		if( SSLConnect( m_psttCtx, m_hSocket, &m_psttSsl ) == false )
		{
			CLog::Print( LOG_ERROR, "%s SSLConnect(%s:%d) error", __FUNCTION__, pszIp, iPort );
			Close();
			return false;
		}
	}
	else
	{
		if( SSLConnect( m_hSocket, &m_psttSsl ) == false )
		{
			CLog::Print( LOG_ERROR, "%s SSLConnect(%s:%d) error", __FUNCTION__, pszIp, iPort );
			Close();
			return false;
		}
	}

	CLog::Print( LOG_NETWORK, "SSLConnect(%s:%d) success", pszIp, iPort );

	m_strIp = pszIp;
	m_iPort = iPort;

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

	if( m_psttCtx )
	{
		SSL_CTX_free( m_psttCtx );
		m_psttCtx = NULL;
	}

	return true;
}

bool CHttp2Client::DoPost( const char * pszPath, HTTP_HEADER_LIST * pclsHeaderList, const char * pszInputContentType, const char * pszInputBody, int iInputBodyLen, std::string & strOutputContentType, std::string & strOutputBody )
{
	strOutputContentType.clear();
	strOutputBody.clear();

	if( pszPath == NULL || pszInputContentType == NULL || pszInputBody == NULL )
	{
		CLog::Print( LOG_ERROR, "%s pszPath or input content type or input body is null", __FUNCTION__ );
		return false;
	}

	int iContentLength = 0;

	if( iInputBodyLen > 0 )
	{
		iContentLength = iInputBodyLen;
	}
	else
	{
		iContentLength = strlen( pszInputBody );
	}

	if( iContentLength <= 0 )
	{
		CLog::Print( LOG_ERROR, "%s pszInputBody's length(%d) error", __FUNCTION__, iContentLength );
		return false;
	}

	CHttpMessage clsRequest, clsResponse;

	clsRequest.m_strHttpMethod = "POST";
	clsRequest.m_strReqUri = pszPath;
	clsRequest.m_strContentType = pszInputContentType;
	clsRequest.m_iContentLength = iContentLength;
	clsRequest.m_strBody.append( pszInputBody, iContentLength );

	if( pclsHeaderList )
	{
		clsRequest.m_clsHeaderList.insert( clsRequest.m_clsHeaderList.end(), pclsHeaderList->begin(), pclsHeaderList->end() );
	}

	if( Execute( &clsRequest, &clsResponse ) == false )
	{
		return false;
	}

	strOutputContentType = clsResponse.m_strContentType;
	strOutputBody = clsResponse.m_strBody;

	return false;
}

bool CHttp2Client::Execute( CHttpMessage * pclsRequest, CHttpMessage * pclsResponse )
{
	if( m_iStreamIdentifier == 0 )
	{
		m_iStreamIdentifier = 1;
	}
	else
	{
		m_iStreamIdentifier += 2;
	}

	if( m_clsSendConversion.MakeFrameList( *pclsRequest, m_clsFrameList ) == false )
	{
		return false;
	}

	HTTP2_FRAME_LIST::iterator itFL;
	int n;
	char szPacket[8192];
	//CHttp2Packet clsPacket;

	n = SSLRecv( m_psttSsl, szPacket, sizeof(szPacket) );
	if( n >= 9 && szPacket[3] == HTTP2_FRAME_TYPE_SETTINGS )
	{
		SSLSend( m_psttSsl, szPacket, n );

		CHttp2Frame clsFrame;

		clsFrame.Set( HTTP2_FRAME_TYPE_SETTINGS, HTTP2_FLAG_ACK, 0, NULL, 0 );
		n = SSLSend( m_psttSsl, (char *)clsFrame.m_pszPacket, clsFrame.m_iPacketLen );
		if( n != clsFrame.m_iPacketLen )
		{
			CLog::Print( LOG_ERROR, "SSLSend(%s:%d) error(%d)", m_strIp.c_str(), m_iPort, n );
			return false;
		}
	}

	for( itFL = m_clsFrameList.m_clsList.begin(); itFL != m_clsFrameList.m_clsList.end(); ++itFL )
	{
		n = SSLSend( m_psttSsl, (char *)((*itFL)->m_pszPacket), (*itFL)->m_iPacketLen );
		if( n != (*itFL)->m_iPacketLen )
		{
			CLog::Print( LOG_ERROR, "SSLSend(%s:%d) error(%d)", m_strIp.c_str(), m_iPort, n );
			return false;
		}
	}

	while( 1 )
	{
		n = SSLRecv( m_psttSsl, szPacket, sizeof(szPacket) );
		if( n <= 0 ) break;
	}

	return true;
}
