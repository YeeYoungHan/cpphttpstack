/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
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
#include "HttpClient2.h"
#include "HttpSetCookie.h"
#include "Log.h"
#include "FileUtility.h"
#include "StringUtility.h"
#include "MemoryDebug.h"

CHttpClient2::CHttpClient2() : m_iRecvTimeout(10), m_iPort(0), m_hSocket(INVALID_SOCKET), m_psttSsl(NULL)
{
	InitNetwork();

	m_strUserAgent = "HttpClent2";
}

CHttpClient2::~CHttpClient2()
{
	Close();
}

/**
 * @ingroup HttpStack
 * @brief	HTTP GET 명령을 실행한다.
 * @param pszUrl								HTTP URL (예:http://www.naver.com)
 * @param strOutputContentType	수신 Content-Type
 * @param strOutputBody					수신 body
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpClient2::DoGet( const char * pszUrl, std::string & strOutputContentType, std::string & strOutputBody )
{
	strOutputContentType.clear();
	strOutputBody.clear();

	if( pszUrl == NULL )
	{
		CLog::Print( LOG_ERROR, "%s pszUrl is null", __FUNCTION__ );
		return false;
	}

	CHttpUri clsUri;
	int iUrlLen = (int)strlen( pszUrl );

	if( clsUri.Parse( pszUrl, iUrlLen ) == -1 )
	{
		CLog::Print( LOG_ERROR, "%s clsUri.Parse(%s) error", __FUNCTION__, pszUrl );
		return false;
	}

	CHttpMessage clsRequest;
	CHttpPacket	clsPacket;

	clsRequest.SetRequest( "GET", &clsUri );
	clsRequest.AddHeader( "Connection", "keep-alive" );

	if( Execute( &clsUri, &clsRequest, &clsPacket ) == false )
	{
		return false;
	}

	CHttpMessage * pclsMessage = clsPacket.GetHttpMessage();

	strOutputContentType = pclsMessage->m_strContentType;
	strOutputBody = pclsMessage->m_strBody;

	return true;
}

/**
 * @ingroup HttpStack
 * @brief	HTTP POST 명령을 실행한다.
 * @param pszUrl								HTTP URL (예:http://wsf.cdyne.com/WeatherWS/Weather.asmx)
 * @param pszInputContentType		전송 Content-Type
 * @param pszInputBody					전송 body
 * @param iInputBodyLen					전송 body 길이
 * @param strOutputContentType	수신 Content-Type
 * @param strOutputBody					수신 body
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpClient2::DoPost( const char * pszUrl, const char * pszInputContentType, const char * pszInputBody, int iInputBodyLen, std::string & strOutputContentType, std::string & strOutputBody )
{
	strOutputContentType.clear();
	strOutputBody.clear();

	if( pszUrl == NULL || pszInputContentType == NULL || pszInputBody == NULL )
	{
		CLog::Print( LOG_ERROR, "%s pszUrl or input content type or input body is null", __FUNCTION__ );
		return false;
	}

	CHttpUri clsUri;
	int iUrlLen = (int)strlen( pszUrl );
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

	if( clsUri.Parse( pszUrl, iUrlLen ) == -1 )
	{
		CLog::Print( LOG_ERROR, "%s clsUri.Parse(%s) error", __FUNCTION__, pszUrl );
		return false;
	}

	CHttpMessage clsRequest;
	CHttpPacket clsPacket;

	clsRequest.SetRequest( "POST", &clsUri );
	clsRequest.AddHeader( "Connection", "keep-alive" );
	clsRequest.m_strContentType = pszInputContentType;
	clsRequest.m_iContentLength = iContentLength;
	clsRequest.m_strBody.append( pszInputBody, iContentLength );

	if( Execute( &clsUri, &clsRequest, &clsPacket ) == false )
	{
		return false;
	}

	CHttpMessage * pclsMessage = clsPacket.GetHttpMessage();

	strOutputContentType = pclsMessage->m_strContentType;
	strOutputBody = pclsMessage->m_strBody;

	return true;
}

/**
 * @ingroup HttpStack
 * @brief 소켓을 종료한다.
 */
void CHttpClient2::Close()
{
	CLog::Print( LOG_NETWORK, "TcpClose(%s:%d)", m_strHost.c_str(), m_iPort );

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

	m_strHost.clear();
	m_iPort = 0;
}

/**
 * @ingroup HttpStack
 * @brief HTTP 응답 메시지 수신 timeout 시간을 설정한다.
 * @param iRecvTimeout HTTP 응답 메시지 수신 timeout 시간 (초단위)
 */
void CHttpClient2::SetRecvTimeout( int iRecvTimeout )
{
	m_iRecvTimeout = iRecvTimeout;
}

/**
 * @ingroup HttpStack
 * @brief HTTP 요청 메시지에 저장할 User-Agent 를 설정한다.
 * @param pszUserAgent 
 */
void CHttpClient2::SetUserAgent( const char * pszUserAgent )
{
	m_strUserAgent = pszUserAgent;
}

/**
 * @ingroup HttpStack
 * @brief HTTP 응답 status code 를 리턴한다.
 * @returns HTTP 응답 status code 를 리턴한다.
 */
int CHttpClient2::GetStatusCode()
{
	return m_iStatusCode;
}

/**
 * @ingroup HttpStack
 * @brief HTTP 서버에 연결하여서 HTTP 요청 메시지를 전송한 후, HTTP 응답 메시지를 수신한다.
 * @param pclsUri				HTTP request URI
 * @param pclsRequest		HTTP request
 * @param pclsPacket		HTTP response 를 저장할 패킷 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpClient2::Execute( CHttpUri * pclsUri, CHttpMessage * pclsRequest, CHttpPacket * pclsPacket )
{
	char * pszBuf = NULL;
	int iBufLen, n;
	bool bRes = false;
	CHttpMessage * pclsResponse = pclsPacket->GetHttpMessage();
	CHttpHeader * pclsHeader;

	int iNewBufLen = 8192 + (int)pclsRequest->m_strBody.length();
	pszBuf = (char *)malloc( iNewBufLen );
	if( pszBuf == NULL )
	{
		CLog::Print( LOG_ERROR, "%s malloc error", __FUNCTION__ );
		return false;
	}

	memset( pszBuf, 0, iNewBufLen );

	pclsRequest->m_strHttpVersion = "HTTP/1.1";
	pclsRequest->AddHeader( "User-Agent", m_strUserAgent );

	AddCookie( pclsUri, pclsRequest );
	
	iBufLen = pclsRequest->ToString( pszBuf, iNewBufLen );
	if( iBufLen <= 0 )
	{
		CLog::Print( LOG_ERROR, "%s clsRequest.ToString() error", __FUNCTION__ );
		goto FUNC_END;
	}

	if( strcmp( m_strHost.c_str(), pclsUri->m_strHost.c_str() ) || m_iPort != pclsUri->m_iPort )
	{
		Close();

		m_hSocket = TcpConnect( pclsUri->m_strHost.c_str(), pclsUri->m_iPort );
		if( m_hSocket == INVALID_SOCKET )
		{
			CLog::Print( LOG_ERROR, "%s TcpConnect(%s:%d) error(%d)", __FUNCTION__, pclsUri->m_strHost.c_str(), pclsUri->m_iPort, GetError() );
			goto FUNC_END;
		}

		CLog::Print( LOG_NETWORK, "TcpConnect(%s:%d) success", pclsUri->m_strHost.c_str(), pclsUri->m_iPort );

		// https 프로토콜이면 TLS 로 연결한다.
		if( !strcmp( pclsUri->m_strProtocol.c_str(), "https" ) )
		{
			if( SSLConnect( m_hSocket, &m_psttSsl ) == false )
			{
				CLog::Print( LOG_ERROR, "%s SSLConnect error", __FUNCTION__ );
				Close();
				goto FUNC_END;
			}

			CLog::Print( LOG_NETWORK, "SSLConnect(%s:%d) success", pclsUri->m_strHost.c_str(), pclsUri->m_iPort );
		}

		m_strHost = pclsUri->m_strHost;
		m_iPort = pclsUri->m_iPort;
	}

	if( m_psttSsl )
	{
		if( SSLSend( m_psttSsl, pszBuf, iBufLen ) != iBufLen )
		{
			CLog::Print( LOG_ERROR, "%s SSLSend error", __FUNCTION__ );
			Close();
			goto FUNC_END;
		}
	}
	else
	{
		if( TcpSend( m_hSocket, pszBuf, iBufLen ) != iBufLen )
		{
			CLog::Print( LOG_ERROR, "%s TcpSend error", __FUNCTION__ );
			Close();
			goto FUNC_END;
		}
	}

	CLog::Print( LOG_NETWORK, "TcpSend(%s:%d) [%s]", pclsUri->m_strHost.c_str(), pclsUri->m_iPort, pszBuf );

	if( CLog::IsPrintLogLevel( LOG_HTTP_HEADER ) )
	{
		std::string strLog;

		pclsRequest->ToString( strLog, true );
		CLog::Print( LOG_HTTP_HEADER, "Send(%s:%d) [%s]", pclsUri->m_strHost.c_str(), pclsUri->m_iPort, strLog.c_str() );
	}

	while( 1 )
	{
		memset( pszBuf, 0, iNewBufLen );

		if( m_psttSsl )
		{
			n = SSLRecv( m_psttSsl, pszBuf, iNewBufLen );
		}
		else
		{
			n = TcpRecv( m_hSocket, pszBuf, iNewBufLen, m_iRecvTimeout );
		}

		if( n <= 0 )
		{
			Close();
			break;
		}

		CLog::Print( LOG_NETWORK, "TcpRecv(%s:%d) [%.*s]", pclsUri->m_strHost.c_str(), pclsUri->m_iPort, n, pszBuf );

		if( pclsPacket->AddPacket( pszBuf, n ) == false )
		{
			CLog::Print( LOG_ERROR, "%s clsPacket.AddPacket error", __FUNCTION__ );
			break;
		}

		if( pclsPacket->IsCompleted() ) break;
	}

	m_iStatusCode = pclsResponse->m_iStatusCode;

	if( pclsResponse->m_iStatusCode / 100 == 2 )
	{
		bRes = true;

		SetCookie( pclsResponse );
	}

	if( CLog::IsPrintLogLevel( LOG_HTTP_HEADER ) )
	{
		std::string strLog;

		pclsResponse->ToString( strLog, true );
		CLog::Print( LOG_HTTP_HEADER, "Recv(%s:%d) [%s]", pclsUri->m_strHost.c_str(), pclsUri->m_iPort, strLog.c_str() );
	}

	pclsHeader = pclsResponse->GetHeader( "Connection" );
	if( pclsHeader )
	{
		if( !strcmp( pclsHeader->m_strValue.c_str(), "close" ) )
		{
			Close();
		}
	}

FUNC_END:
	if( pszBuf )
	{
		free( pszBuf );
		pszBuf = NULL;
	}

	// 3XX 응답에서 Location 헤더가 존재하면 해당 URL 로 다시 시도한다.
	if( pclsResponse->m_iStatusCode / 100 == 3 )
	{
		CHttpHeader * pclsHeader = pclsResponse->GetHeader( "Location" );
		if( pclsHeader && pclsHeader->m_strValue.empty() == false )
		{
			CHttpUri clsUri;

			if( clsUri.Parse( pclsHeader->m_strValue.c_str(), (int)pclsHeader->m_strValue.length() ) )
			{
				if( clsUri.m_strHost.empty() )
				{
					clsUri.m_strHost = pclsUri->m_strHost;
					clsUri.m_iPort = pclsUri->m_iPort;
					clsUri.m_strProtocol = pclsUri->m_strProtocol;
					clsUri.m_strPath = pclsHeader->m_strValue;
				}
				
				pclsRequest->SetRequest( pclsRequest->m_strHttpMethod.c_str(), &clsUri );

				return Execute( &clsUri, pclsRequest, pclsPacket );
			}
		}
	}

	return bRes;
}

void CHttpClient2::AddCookie( CHttpUri * pclsUri, CHttpMessage * pclsMessage )
{
	std::string strCookie;

	if( pclsUri->m_strHost.empty() ) return;
	if( m_clsDomainCookie.IsEmpty() ) return;

	if( isdigit( pclsUri->m_strHost[0] ) == 0 )
	{
		// 도메인 이름 파싱
		STRING_LIST clsList;
		STRING_LIST::reverse_iterator it;
		std::string strDomain, strCookieOne;

		SplitString( pclsUri->m_strHost.c_str(), clsList, '.' );

		for( it = clsList.rbegin(); it != clsList.rend(); ++it )
		{
			if( strDomain.empty() == false ) strDomain.insert( 0, "." );
			strDomain.insert( 0, *it );

			if( m_clsDomainCookie.Select( strDomain.c_str(), pclsMessage->m_strReqUri.c_str(), strCookieOne ) )
			{
				if( strCookie.empty() == false ) strCookie.append( "; " );
				strCookie.append( strCookieOne );
			}
		}
	}
	else
	{
		m_clsDomainCookie.Select( pclsUri->m_strHost.c_str(), pclsMessage->m_strReqUri.c_str(), strCookie );
	}

	if( strCookie.empty() == false )
	{
		pclsMessage->AddHeader( "Cookie", strCookie );
	}
}

void CHttpClient2::SetCookie( CHttpMessage * pclsMessage )
{
	HTTP_HEADER_LIST::iterator itHL;
	CHttpSetCookie clsSetCookie;

	for( itHL = pclsMessage->m_clsHeaderList.begin(); itHL != pclsMessage->m_clsHeaderList.end(); ++itHL )
	{
		if( !strcasecmp( itHL->m_strName.c_str(), "Set-Cookie" ) )
		{
			if( clsSetCookie.Parse( itHL->m_strValue.c_str(), (int)itHL->m_strValue.length() ) != -1 )
			{
				if( clsSetCookie.m_strDomain.empty() ) clsSetCookie.m_strDomain = m_strHost;

				m_clsDomainCookie.Insert( &clsSetCookie );
			}
		}
	}
}
