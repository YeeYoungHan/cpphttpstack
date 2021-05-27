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

#ifndef _HTTP2_CLIENT_H_
#define _HTTP2_CLIENT_H_

#include "HttpMessage.h"
#include "SipTcp.h"
#include "TlsFunction.h"
#include "Http2Conversion.h"
#include "Http2FrameList.h"
#include "PcapSave.h"

class CHttp2Client
{
public:
	CHttp2Client();
	~CHttp2Client();

	bool Connect( const char * pszIp, int iPort, const char * pszClientPemFileName = NULL, const char * pszPcapFileName = NULL );
	bool Close();

	bool DoPost( const char * pszPath, HTTP_HEADER_LIST * pclsHeaderList, const char * pszInputContentType, const char * pszInputBody, int iInputBodyLen, std::string & strOutputContentType, std::string & strOutputBody );

	bool Execute( CHttpMessage * pclsRequest, CHttpMessage * pclsResponse );

private:
	int Send( char * pszPacket, int iPacketLen );
	int Recv( char * pszPacket, int iPacketSize );

	Socket		m_hSocket;
	SSL			* m_psttSsl;
	SSL_CTX * m_psttCtx;
	uint32_t	m_iStreamIdentifier;

	CHttp2Conversion	m_clsSendConversion;
	CHttp2Conversion	m_clsRecvConversion;
	CHttp2FrameList		m_clsFrameList;

	std::string	m_strServerIp;
	int					m_iServerPort;

	std::string	m_strClientIp;
	int					m_iClientPort;

	CPcapSave		m_clsPcap;
};

#endif