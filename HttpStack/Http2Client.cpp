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
#include "Http2Settings.h"
#include "TimeUtility.h"
#include "Log.h"

CHttp2Client::CHttp2Client() : m_hSocket(INVALID_SOCKET), m_psttSsl(NULL), m_psttCtx(NULL), m_iStreamIdentifier(0)
	, m_iServerPort(0), m_iClientPort(0), m_iStatusCode(0)
{
	InitNetwork();
}

CHttp2Client::~CHttp2Client()
{
	Close();
}

/**
 * @ingroup HttpStack
 * @brief HTTP/2 서버에 연결한다.
 * @param pszIp HTTP/2 서버 IP 주소 또는 도메인 이름
 * @param iPort HTTP/2 서버 포트 번호
 * @param pszClientPemFileName TLS 통신에서 클라이언트 키/인증서를 사용하는 경우, 클라이언트 키/인증서의 full path 를 입력하고 그렇지 않으면 NULL 을 입력한다.
 * @param pszPcapFileName 통신 데이터를 pcap 파일에 저장하고 싶으면 pcap 파일 full path 를 입력하고 그렇지 않으면 NULL 을 입력한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Client::Connect( const char * pszIp, int iPort, const char * pszClientPemFileName, const char * pszPcapFileName )
{
	if( m_hSocket != INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s m_hSocket != INVALID_SOCKET", __FUNCTION__ );
		return false;
	}

	m_strHost = pszIp;

	char		szIp[INET6_ADDRSTRLEN];

	memset( szIp, 0, sizeof(szIp) );
	if( isdigit( pszIp[0] ) == 0 )
	{
		GetIpByName( pszIp, szIp, sizeof(szIp) );
	}
	else
	{
		snprintf( szIp, sizeof(szIp), "%s", pszIp );
	}

	m_hSocket = TcpConnect( szIp, iPort );
	if( m_hSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s TcpConnect(%s:%d) error", __FUNCTION__, szIp, iPort );
		return false;
	}

	CLog::Print( LOG_NETWORK, "TcpConnect(%s:%d) success", szIp, iPort );

	m_psttCtx = SSLClientStart( pszClientPemFileName );
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
		CLog::Print( LOG_ERROR, "%s SSLConnect(%s:%d) error", __FUNCTION__, szIp, iPort );
		Close();
		return false;
	}

	CLog::Print( LOG_NETWORK, "SSLConnect(%s:%d) success", szIp, iPort );

	m_strServerIp = szIp;
	m_iServerPort = iPort;

	if( pszPcapFileName )
	{
#ifdef WIN32
#if _MSC_VER == VC2008_VERSION
		m_clsPcap.Open( pszPcapFileName, HTTP2_FRAME_SIZE + 200 );
		GetLocalIpPort( m_hSocket, m_strClientIp, m_iClientPort );
#endif
#endif
	}

	TcpSetPollIn( m_sttPoll[0], m_hSocket );

	int n, iLen;
	char szPacket[8192];

	iLen = snprintf( szPacket, sizeof(szPacket), "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n" );

	n = Send( szPacket, iLen );
	if( n != iLen )
	{
		CLog::Print( LOG_ERROR, "Send(%s:%d) [%s] error(%d)", m_strServerIp.c_str(), m_iServerPort, szPacket, n );
		Close();
		return false;
	}

	CLog::Print( LOG_NETWORK, "Send(%s:%d) [%s]", m_strServerIp.c_str(), m_iServerPort, szPacket );

	CHttp2Settings clsSettings;

	clsSettings.Add( HTTP2_SETTINGS_MAX_CONCURRENT_STREAMS, 1 );
	clsSettings.Add( HTTP2_SETTINGS_ENABLE_PUSH, 0 );

	m_clsFrame.Set( HTTP2_FRAME_TYPE_SETTINGS, 0, 0, clsSettings.m_pszPacket, clsSettings.m_iPacketLen );
	n = Send( (char *)m_clsFrame.m_pszPacket, m_clsFrame.m_iPacketLen );
	if( n != m_clsFrame.m_iPacketLen )
	{
		CLog::Print( LOG_ERROR, "Send(%s:%d) error(%d)", m_strServerIp.c_str(), m_iServerPort, n );
		Close();
		return false;
	}

	m_clsFrame.PrintLog( LOG_NETWORK, m_strServerIp.c_str(), m_iServerPort, true );

	if( RecvNonBlocking() == false ) return false;

	return true;
}

/**
 * @ingroup HttpStack
 * @brief HTTP/2 서버와의 연결을 종료시킨다.
 * @returns true 를 리턴한다.
 */
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

/**
 * @ingroup HttpStack
 * @brief HTTP GET 명령을 전송한 후, 이에 대한 응답을 수신한다.
 * @param pszPath								path
 * @param strOutputContentType	[out] 수신 content-type
 * @param strOutputBody					[out] 수신 body
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Client::DoGet( const char * pszPath, std::string & strOutputContentType, std::string & strOutputBody )
{
	strOutputContentType.clear();
	strOutputBody.clear();

	if( pszPath == NULL )
	{
		CLog::Print( LOG_ERROR, "%s pszPath is null", __FUNCTION__ );
		return false;
	}

	CHttpMessage clsRequest, clsResponse;

	clsRequest.m_strHttpMethod = "GET";
	clsRequest.m_strReqUri = pszPath;
	clsRequest.AddHeader( "host", m_strHost );

	if( Execute( &clsRequest, &clsResponse ) == false )
	{
		return false;
	}

	strOutputContentType = clsResponse.m_strContentType;
	strOutputBody = clsResponse.m_strBody;

	return true;
}

/**
 * @ingroup HttpStack
 * @brief HTTP POST 명령을 전송한 후, 이에 대한 응답을 수신한다.
 * @param pszPath								path
 * @param pclsHeaderList				HTTP 헤더 리스트
 * @param pszInputContentType		전송 content-type
 * @param pszInputBody					전송 body
 * @param iInputBodyLen					전송 body 길이
 * @param strOutputContentType	[out] 수신 content-type
 * @param strOutputBody					[out] 수신 body
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
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
		iContentLength = (int)strlen( pszInputBody );
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

	return true;
}

/**
 * @ingroup HttpStack
 * @brief HTTP 응답 코드를 리턴한다.
 * @returns HTTP 응답 코드를 리턴한다.
 */
int CHttp2Client::GetStatusCode()
{
	return m_iStatusCode;
}

/**
 * @ingroup HttpStack
 * @brief HTTP 요청 메시지를 전송한 후, 이에 대한 HTTP 응답 메시지를 수신한다.
 * @param pclsRequest		HTTP 요청 메시지
 * @param pclsResponse	HTTP 응답 메시지
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
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

	m_iStatusCode = 0;
	pclsRequest->m_iStreamIdentifier = m_iStreamIdentifier;

	if( m_clsSendConversion.MakeFrameList( *pclsRequest, m_clsFrameList ) == false )
	{
		return false;
	}

	HTTP2_FRAME_LIST::iterator itFL;
	int		n;
	bool	bEnd = false, bRes = false;

	if( RecvNonBlocking() == false ) return false;

	for( itFL = m_clsFrameList.m_clsList.begin(); itFL != m_clsFrameList.m_clsList.end(); ++itFL )
	{
		n = Send( (char *)((*itFL)->m_pszPacket), (*itFL)->m_iPacketLen );
		if( n != (*itFL)->m_iPacketLen )
		{
			CLog::Print( LOG_ERROR, "Send(%s:%d) error(%d)", m_strServerIp.c_str(), m_iServerPort, n );
			return false;
		}

		(*itFL)->PrintLog( LOG_NETWORK, m_strServerIp.c_str(), m_iServerPort, true );
	}

	if( CLog::IsPrintLogLevel( LOG_HTTP_HEADER ) )
	{
		std::string strLog;

		pclsRequest->ToString( strLog, true );
		CLog::Print( LOG_HTTP_HEADER, "Send(%s:%d) [%s]", m_strServerIp.c_str(), m_iServerPort, strLog.c_str() );
	}

	while( bEnd == false )
	{
		n = Recv( (char *)m_szPacket, sizeof(m_szPacket) );
		if( n <= 0 ) break;

		if( m_clsPacket.AddPacket( m_szPacket, n ) )
		{
			while( m_clsPacket.GetFrame( &m_clsFrame ) )
			{
				m_clsFrame.PrintLog( LOG_NETWORK, m_strServerIp.c_str(), m_iServerPort, false );

				switch( m_clsFrame.GetType() )
				{
				case HTTP2_FRAME_TYPE_DATA:
					m_clsRecvConversion.MakeMessage( m_clsFrame, *pclsResponse );
					if( m_clsFrame.GetFlags() & HTTP2_FLAG_END_STREAM )
					{
						bEnd = true;
						bRes = true;
					}
					break;
				case HTTP2_FRAME_TYPE_HEADERS:
					m_clsRecvConversion.MakeMessage( m_clsFrame, *pclsResponse );
					if( m_clsFrame.GetFlags() & HTTP2_FLAG_END_STREAM )
					{
						bEnd = true;
						bRes = true;
					}
					break;
				case HTTP2_FRAME_TYPE_SETTINGS:
					if( m_clsFrame.GetFlags() == 0 )
					{
						if( SendSettingsAck() == false ) return false;
					}
					break;
				case HTTP2_FRAME_TYPE_RST_STREAM:
				case HTTP2_FRAME_TYPE_GOAWAY:
					bEnd = true;
					break;
				case HTTP2_FRAME_TYPE_PING:
					if( m_clsFrame.GetFlags() == 0 )
					{
						if( SendPingAck() == false ) return false;
					}
					break;
				}
			}
		}
	}

	if( CLog::IsPrintLogLevel( LOG_HTTP_HEADER ) )
	{
		std::string strLog;

		pclsResponse->ToString( strLog, true );
		CLog::Print( LOG_HTTP_HEADER, "Recv(%s:%d) [%s]", m_strServerIp.c_str(), m_iServerPort, strLog.c_str() );
	}

	m_iStatusCode = pclsResponse->m_iStatusCode;

	return bRes;
}

/**
 * @ingroup HttpStack
 * @brief 패킷을 전송한다.
 * @param pszPacket		전송할 데이터를 저장한 변수
 * @param iPacketLen	전송 데이터의 길이
 * @returns 전송된 데이터의 길이를 리턴한다.
 */
int CHttp2Client::Send( char * pszPacket, int iPacketLen )
{
	int n = SSLSend( m_psttSsl, pszPacket, iPacketLen );

#ifdef WIN32
#if _MSC_VER == VC2008_VERSION
	if( m_clsPcap.IsOpen() )
	{
		struct timeval sttTime;

		gettimeofday( &sttTime, NULL );
		m_clsPcap.WriteTcp( &sttTime, m_strClientIp.c_str(), m_iClientPort, m_strServerIp.c_str(), 8081, (char *)pszPacket, iPacketLen );
	}
#endif
#endif

	return n;
}

/**
 * @ingroup HttpStack
 * @brief 패킷을 수신한다.
 * @param pszPacket		[out] 수신한 데이터를 저장할 변수
 * @param iPacketSize pszPacket 변수의 크기
 * @returns 수신한 데이터의 길이를 리턴한다.
 */
int CHttp2Client::Recv( char * pszPacket, int iPacketSize )
{
	int n = SSLRecv( m_psttSsl, pszPacket, iPacketSize );

#ifdef WIN32
#if _MSC_VER == VC2008_VERSION
	if( n > 0 )
	{
		struct timeval sttTime;

		gettimeofday( &sttTime, NULL );
		m_clsPcap.WriteTcp( &sttTime, m_strServerIp.c_str(), 8081, m_strClientIp.c_str(), m_iClientPort, (char *)pszPacket, n );
	}
#endif
#endif

	return n;
}

/**
 * @ingroup HttpStack
 * @brief non-blocking mode 로 TCP 수신 처리한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Client::RecvNonBlocking()
{
	while( poll( m_sttPoll, 1, 0 ) > 0 )
	{
		int n = Recv( (char *)m_szPacket, sizeof(m_szPacket) );
		if( n <= 0 )
		{
			CLog::Print( LOG_NETWORK, "TCP(%s:%d) closed", m_strServerIp.c_str(), m_iServerPort );
			Close();
			return false;
		}

		if( m_clsPacket.AddPacket( m_szPacket, n ) )
		{
			while( m_clsPacket.GetFrame( &m_clsFrame ) )
			{
				m_clsFrame.PrintLog( LOG_NETWORK, m_strServerIp.c_str(), m_iServerPort, false );

				switch( m_clsFrame.GetType() )
				{
				case HTTP2_FRAME_TYPE_SETTINGS:
					if( m_clsFrame.GetFlags() == 0 )
					{
						if( SendSettingsAck() == false ) return false;
					}
					break;
				case HTTP2_FRAME_TYPE_PING:
					if( m_clsFrame.GetFlags() == 0 )
					{
						if( SendPingAck() == false ) return false;
					}
					break;
				}
			}
		}
	}

	return true;
}

/**
 * @ingroup HttpStack
 * @brief SETTINGS ACK frame 을 전송한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Client::SendSettingsAck()
{
	m_clsFrame.Set( HTTP2_FRAME_TYPE_SETTINGS, HTTP2_FLAG_ACK, 0, NULL, 0 );
	int n = Send( (char *)m_clsFrame.m_pszPacket, m_clsFrame.m_iPacketLen );
	if( n != m_clsFrame.m_iPacketLen )
	{
		CLog::Print( LOG_ERROR, "Send(%s:%d) error(%d)", m_strServerIp.c_str(), m_iServerPort, n );
		Close();
		return false;
	}

	m_clsFrame.PrintLog( LOG_NETWORK, m_strServerIp.c_str(), m_iServerPort, true );

	return true;
}

/**
 * @ingroup HttpStack
 * @brief PING ACK frame 을 전송한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Client::SendPingAck()
{
	m_clsFrame.SetFlags( HTTP2_FLAG_ACK );
	int n = Send( (char *)m_clsFrame.m_pszPacket, m_clsFrame.m_iPacketLen );
	if( n != m_clsFrame.m_iPacketLen )
	{
		CLog::Print( LOG_ERROR, "Send(%s:%d) error(%d)", m_strServerIp.c_str(), m_iServerPort, n );
		Close();
		return false;
	}

	m_clsFrame.PrintLog( LOG_NETWORK, m_strServerIp.c_str(), m_iServerPort, true );

	return true;
}
