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

#include "WhisperHttpServer.h"
#include "HttpStatusCode.h"
#include "HttpParameterList.h"
#include "HttpMultipart.h"
#include "StringUtility.h"
#include "FileUtility.h"
#include "Directory.h"
#include "Log.h"
#include "WhisperMap.h"
#include "MemoryDebug.h"

extern CHttpStack gclsStack;

CWhisperHttpServer::CWhisperHttpServer() : m_bStop(false)
{
}

CWhisperHttpServer::~CWhisperHttpServer()
{
}

/**
 * @ingroup TestHttpStack
 * @brief HTTP 요청 수신 이벤트 callback
 * @param pclsRequest		HTTP 요청 메시지
 * @param pclsResponse	HTTP 응답 메시지 - 응용에서 저장한다.
 * @returns 응용에서 HTTP 응답 메시지를 정상적으로 생성하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWhisperHttpServer::RecvHttpRequest( CHttpMessage * pclsRequest, CHttpMessage * pclsResponse )
{
	const char * pszUri = pclsRequest->m_strReqUri.c_str();
	if( strcmp( pszUri, "/" ) && strcmp( pszUri, "/index.html" ) )
	{
		pclsResponse->m_iStatusCode = HTTP_NOT_FOUND;
		return true;
	}

	std::string strPath = "index.html";

	if( IsExistFile( strPath.c_str() ) == false )
	{
		pclsResponse->m_iStatusCode = HTTP_NOT_FOUND;
		return true;
	}

	pclsResponse->m_strContentType = "text/html";

	// 파일을 읽어서 HTTP body 에 저장한다.
	FILE * fd = fopen( strPath.c_str(), "rb" );
	if( fd == NULL )
	{
		pclsResponse->m_iStatusCode = HTTP_NOT_FOUND;
		return true;
	}

	int n;
	char szBuf[8192];

	while( ( n = (int)fread( szBuf, 1, sizeof(szBuf), fd ) ) > 0 )
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
 * @param pclsRequest	HTTP 요청 메시지
 */
void CWhisperHttpServer::WebSocketConnected( const char * pszClientIp, int iClientPort, CHttpMessage * pclsRequest )
{
	printf( "WebSocket[%s:%d] connected\n", pszClientIp, iClientPort );

	// GET Query parameter list 가져오기
	CHttpParameterList clsParamList;
	HTTP_PARAMETER_LIST::iterator itPL;
	std::string strModel, strExt;
	int iFileSize = 0;

	if( clsParamList.ParseUrl( pclsRequest->m_strReqUri ) != -1 )
	{
		for( itPL = clsParamList.m_clsParamList.begin(); itPL != clsParamList.m_clsParamList.end(); ++itPL )
		{
			const char * pszName = itPL->m_strName.c_str();
			const char * pszValue = itPL->m_strValue.c_str();

			printf( "GET name[%s] = value[%s]\n", pszName, pszValue );

			if( !strcmp( pszName, "model" ) )
			{
				strModel = pszValue;
			}
			else if( !strcmp( pszName, "ext" ) )
			{
				strExt = pszValue;
			}
			else if( !strcmp( pszName, "size" ) )
			{
				iFileSize = atoi( pszValue );
			}
		}
	}

	gclsWhisperMap.Insert( pszClientIp, iClientPort, strModel.c_str(), strExt.c_str(), iFileSize );
}

/**
 * @ingroup TestHttpStack
 * @brief WebSocket 클라이언트 TCP 연결 종료 이벤트 callback
 * @param pszClientIp WebSocket 클라이언트 IP 주소
 * @param iClientPort WebSocket 클라이언트 포트 번호
 */
void CWhisperHttpServer::WebSocketClosed( const char * pszClientIp, int iClientPort )
{
	printf( "WebSocket[%s:%d] closed\n", pszClientIp, iClientPort );

	gclsWhisperMap.Delete( pszClientIp, iClientPort );
}

/**
 * @ingroup TestHttpStack
 * @brief WebSocket 클라이언트 데이터 수신 이벤트 callback
 * @param pszClientIp WebSocket 클라이언트 IP 주소
 * @param iClientPort WebSocket 클라이언트 포트 번호
 * @param strData			WebSocket 클라이언트가 전송한 데이터
 * @returns WebSocket 클라이언트 연결을 유지하려면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWhisperHttpServer::WebSocketData( const char * pszClientIp, int iClientPort, std::string & strData )
{
	//printf( "WebSocket[%s:%d] recv[%s]\n", pszClientIp, iClientPort, strData.c_str() );

	gclsWhisperMap.SaveFile( pszClientIp, iClientPort, strData );

	return true;
}
