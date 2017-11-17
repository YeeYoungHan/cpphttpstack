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

#include "WebRtcServer.h"
#include "HttpStatusCode.h"
#include "StringUtility.h"
#include "FileUtility.h"
#include "Directory.h"
#include "Log.h"
#include "UserMap.h"
#include "MemoryDebug.h"

extern CHttpStack gclsStack;

CWebRtcServer::CWebRtcServer() : m_bStop(false)
{
}

CWebRtcServer::~CWebRtcServer()
{
}

/**
 * @ingroup TestHttpStack
 * @brief HTTP 요청 수신 이벤트 callback
 * @param pclsRequest		HTTP 요청 메시지
 * @param pclsResponse	HTTP 응답 메시지 - 응용에서 저장한다.
 * @returns 응용에서 HTTP 응답 메시지를 정상적으로 생성하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWebRtcServer::RecvHttpRequest( CHttpMessage * pclsRequest, CHttpMessage * pclsResponse )
{
	std::string strPath = m_strDocumentRoot;
	std::string strExt;

	//CLog::Print( LOG_DEBUG, "req uri[%s]", pclsRequest->m_strReqUri.c_str() );

	// 보안상 .. 을 포함한 URL 을 무시한다.
	if( strstr( pclsRequest->m_strReqUri.c_str(), ".." ) )
	{
		pclsResponse->m_iStatusCode = HTTP_NOT_FOUND;
		return true;
	}

#ifdef _DEBUG
	// 메모리 누수 검사를 위해서 exit.html 을 수신하면 프로그램을 종료한다.
	if( !strcmp( pclsRequest->m_strReqUri.c_str(), "/exit.html" ) )
	{
		pclsResponse->m_iStatusCode = HTTP_NOT_FOUND;
		m_bStop = true;
		return true;
	}
#endif

	if( !strcmp( pclsRequest->m_strReqUri.c_str(), "/" ) )
	{
		CDirectory::AppendName( strPath, "index.html" );
	}
	else
	{
#ifdef WIN32
		ReplaceString( pclsRequest->m_strReqUri, "/", "\\" );
#endif

		strPath.append( pclsRequest->m_strReqUri );
	}

	if( IsExistFile( strPath.c_str() ) == false )
	{
		pclsResponse->m_iStatusCode = HTTP_NOT_FOUND;
		return true;
	}

	// 파일별 Content-Type 을 설정한다.
	GetFileExt( strPath.c_str(), strExt );
	const char * pszExt = strExt.c_str();
	
	if( !strcmp( pszExt, "html" ) || !strcmp( pszExt, "htm" ) )
	{
		pclsResponse->m_strContentType = "text/html";
	}
	else if( !strcmp( pszExt, "css" ) )
	{
		pclsResponse->m_strContentType = "text/css";
	}
	else if( !strcmp( pszExt, "js" ) )
	{
		pclsResponse->m_strContentType = "text/javascript";
	}
	else if( !strcmp( pszExt, "png" ) || !strcmp( pszExt, "gif" ) )
	{
		pclsResponse->m_strContentType = "image/";
		pclsResponse->m_strContentType.append( pszExt );
	}
	else if( !strcmp( pszExt, "jpg" ) || !strcmp( pszExt, "jpeg" ) )
	{
		pclsResponse->m_strContentType = "image/jpeg";
	}
	else
	{
		pclsResponse->m_iStatusCode = HTTP_NOT_FOUND;
		return true;
	}

	// 파일을 읽어서 HTTP body 에 저장한다.
	FILE * fd = fopen( strPath.c_str(), "rb" );
	if( fd == NULL )
	{
		pclsResponse->m_iStatusCode = HTTP_NOT_FOUND;
		return true;
	}

	int n;
	char szBuf[8192];

	while( ( n = fread( szBuf, 1, sizeof(szBuf), fd ) ) > 0 )
	{
		pclsResponse->m_strBody.append( szBuf, n );
	}

	fclose( fd );

	pclsResponse->m_iStatusCode = HTTP_OK;

	return true;
}

/**
 * @ingroup TestHttpStack
 * @brief WebSocket 클라이언트 TCP 연결 시작 이벤트 callback
 * @param pszClientIp WebSocket 클라이언트 IP 주소
 * @param iClientPort WebSocket 클라이언트 포트 번호
 */
void CWebRtcServer::WebSocketConnected( const char * pszClientIp, int iClientPort )
{
	printf( "WebSocket[%s:%d] connected\n", pszClientIp, iClientPort );
}

/**
 * @ingroup TestHttpStack
 * @brief WebSocket 클라이언트 TCP 연결 종료 이벤트 callback
 * @param pszClientIp WebSocket 클라이언트 IP 주소
 * @param iClientPort WebSocket 클라이언트 포트 번호
 */
void CWebRtcServer::WebSocketClosed( const char * pszClientIp, int iClientPort )
{
	printf( "WebSocket[%s:%d] closed\n", pszClientIp, iClientPort );

	gclsUserMap.Delete( pszClientIp, iClientPort );
}

/**
 * @ingroup TestHttpStack
 * @brief WebSocket 클라이언트 데이터 수신 이벤트 callback
 * @param pszClientIp WebSocket 클라이언트 IP 주소
 * @param iClientPort WebSocket 클라이언트 포트 번호
 * @param strData			WebSocket 클라이언트가 전송한 데이터
 * @returns WebSocket 클라이언트 연결을 유지하려면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWebRtcServer::WebSocketData( const char * pszClientIp, int iClientPort, std::string & strData )
{
	printf( "WebSocket[%s:%d] recv[%s]\n", pszClientIp, iClientPort, strData.c_str() );

	STRING_LIST clsList;
	STRING_LIST::iterator itList;
	int iIndex = 0;
	const char * pszCol;
	bool bReq = true;
	int iCommand = 0, iStatusCode = 0;
	std::string strTo, strSdp;

	SplitString( strData.c_str(), clsList, '|' );

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		pszCol = itList->c_str();

		switch( iIndex )
		{
		case 0:
			if( !strcmp( pszCol, "req" ) )
			{
				bReq = true;
			}
			else
			{
				bReq = false;
			}
			break;
		case 1:
			if( !strcmp( pszCol, "register" ) )
			{
				iCommand = 1;
			}
			else if( !strcmp( pszCol, "invite" ) )
			{
				iCommand = 2;
			}
			else if( !strcmp( pszCol, "bye" ) )
			{
				iCommand = 3;
			}
			break;
		case 2:
			if( bReq )
			{
				switch( iCommand )
				{
				case 1:
					if( gclsUserMap.Insert( pszCol, pszClientIp, iClientPort ) == false )
					{
						Send( pszClientIp, iClientPort, "res|register|500" );
					}
					else
					{
						Send( pszClientIp, iClientPort, "res|register|200" );
					}
					break;
				case 2:
					strTo = pszCol;
					break;
				}
			}
			else
			{
				switch( iCommand )
				{
				case 2:
					iStatusCode = atoi( pszCol );
					if( iStatusCode > 200 )
					{

					}
					break;
				}
			}
			break;
		case 3:
			if( bReq )
			{
				switch( iCommand )
				{
				case 2:
					strSdp = pszCol;

					{
						CUserInfo clsToUser;
						std::string strUserId;

						if( gclsUserMap.Select( strTo.c_str(), clsToUser ) == false )
						{
							Send( pszClientIp, iClientPort, "res|invite|404" );
						}
						else if( gclsUserMap.SelectUserId( pszClientIp, iClientPort, strUserId ) == false )
						{
							Send( pszClientIp, iClientPort, "res|invite|403" );
						}
						else
						{
							if( Send( clsToUser.m_strIp.c_str(), clsToUser.m_iPort, "req|invite|%s|%s", strUserId.c_str(), strSdp.c_str() ) == false )
							{
								Send( pszClientIp, iClientPort, "res|invite|500" );
							}
							else
							{
								Send( pszClientIp, iClientPort, "res|invite|180" );
							}
						}
					}
					break;
				}
			}
			else
			{
				switch( iCommand )
				{
				case 2:
					break;
				}
			}
			break;
		}
		
		++iIndex;
	}

	//gclsStack.SendWebSocketPacket( pszClientIp, iClientPort, strData.c_str(), strData.length() );

	return true;
}

bool CWebRtcServer::Send( const char * pszClientIp, int iClientPort, const char * fmt, ... )
{
	va_list	ap;
	char		szBuf[8192];
	int			iBufLen;

	va_start( ap, fmt );
	iBufLen = vsnprintf( szBuf, sizeof(szBuf)-1, fmt, ap );
	va_end( ap );

	if( gclsStack.SendWebSocketPacket( pszClientIp, iClientPort, szBuf, iBufLen ) )
	{
		printf( "WebSocket[%s:%d] send[%s]\n", pszClientIp, iClientPort, szBuf );
		return true;
	}

	return false;
}
