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

#include "HttpNoCacheProxy.h"
#include "HttpStatusCode.h"
#include "HttpClient.h"
#include "Log.h"
#include "MemoryDebug.h"

extern CHttpStack gclsStack;

CHttpNoCacheProxy::CHttpNoCacheProxy() : m_iServerPort(80), m_bStop(false)
{
}

CHttpNoCacheProxy::~CHttpNoCacheProxy()
{
}

/**
 * @ingroup HttpNoCacheProxy
 * @brief HTTP ��û ���� �̺�Ʈ callback
 * @param pclsRequest		HTTP ��û �޽���
 * @param pclsResponse	HTTP ���� �޽��� - ���뿡�� �����Ѵ�.
 * @returns ���뿡�� HTTP ���� �޽����� ���������� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CHttpNoCacheProxy::RecvHttpRequest( CHttpMessage * pclsRequest, CHttpMessage * pclsResponse )
{
	CLog::Print( LOG_DEBUG, "req uri[%s]", pclsRequest->m_strReqUri.c_str() );

	CHttpClient clsClient;
	const char * pszMethod = pclsRequest->m_strHttpMethod.c_str();
	const char * pszUrl = pclsRequest->m_strReqUri.c_str();
	const char * pszPath;
	std::string strUrl, strContentType, strContent;
	HTTP_HEADER_LIST clsHeaderList;

	pclsResponse->AddHeader( "Cache-Control", "no-cache, no-store, must-revalidate" );

	if( !strncmp( pszUrl, "http://", 7 ) )
	{
		pszPath = strstr( pszUrl + 7, "/" );
	}
	else
	{
		pszPath = pszUrl;
	}

	strUrl = "http://";
	strUrl.append( m_strServerIp );

	if( m_iServerPort != 80 )
	{
		char szPort[12];

		snprintf( szPort, sizeof(szPort), ":%d", m_iServerPort );
		strUrl.append( szPort );
	}

	strUrl.append( pszPath );

	CHttpHeader * pclsHeader = pclsRequest->GetHeader( "Cookie" );
	if( pclsHeader )
	{
		clsHeaderList.push_back( *pclsHeader );
	}

	if( !strcmp( pszMethod, "GET" ) )
	{
		if( clsClient.DoGet( strUrl.c_str(), &clsHeaderList, strContentType, strContent ) )
		{
			pclsResponse->m_iStatusCode = HTTP_OK;

			pclsResponse->m_strContentType = strContentType;
			pclsResponse->m_strBody = strContent;
		}
		else
		{
			pclsResponse->m_iStatusCode = HTTP_INTERNAL_SERVER_ERROR;
		}
	}
	else if( !strcmp( pszMethod, "POST" ) )
	{
		if( clsClient.DoPost( strUrl.c_str(), &clsHeaderList, pclsRequest->m_strContentType.c_str(), pclsRequest->m_strBody.c_str(), pclsRequest->m_strBody.length(), strContentType, strContent ) )
		{
			pclsResponse->m_iStatusCode = HTTP_OK;

			pclsResponse->m_strContentType = strContentType;
			pclsResponse->m_strBody = strContent;
		}
		else
		{
			pclsResponse->m_iStatusCode = HTTP_INTERNAL_SERVER_ERROR;
		}
	}
	else
	{
		pclsResponse->m_iStatusCode = HTTP_NOT_IMPLEMENTED;
	}

	return true;
}

/**
 * @ingroup HttpNoCacheProxy
 * @brief WebSocket Ŭ���̾�Ʈ TCP ���� ���� �̺�Ʈ callback
 * @param pszClientIp WebSocket Ŭ���̾�Ʈ IP �ּ�
 * @param iClientPort WebSocket Ŭ���̾�Ʈ ��Ʈ ��ȣ
 */
void CHttpNoCacheProxy::WebSocketConnected( const char * pszClientIp, int iClientPort )
{
	printf( "WebSocket[%s:%d] connected\n", pszClientIp, iClientPort );
}

/**
 * @ingroup HttpNoCacheProxy
 * @brief WebSocket Ŭ���̾�Ʈ TCP ���� ���� �̺�Ʈ callback
 * @param pszClientIp WebSocket Ŭ���̾�Ʈ IP �ּ�
 * @param iClientPort WebSocket Ŭ���̾�Ʈ ��Ʈ ��ȣ
 */
void CHttpNoCacheProxy::WebSocketClosed( const char * pszClientIp, int iClientPort )
{
	printf( "WebSocket[%s:%d] closed\n", pszClientIp, iClientPort );
}

/**
 * @ingroup HttpNoCacheProxy
 * @brief WebSocket Ŭ���̾�Ʈ ������ ���� �̺�Ʈ callback
 * @param pszClientIp WebSocket Ŭ���̾�Ʈ IP �ּ�
 * @param iClientPort WebSocket Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @param strData			WebSocket Ŭ���̾�Ʈ�� ������ ������
 * @returns WebSocket Ŭ���̾�Ʈ ������ �����Ϸ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CHttpNoCacheProxy::WebSocketData( const char * pszClientIp, int iClientPort, std::string & strData )
{
	printf( "WebSocket[%s:%d] recv[%s]\n", pszClientIp, iClientPort, strData.c_str() );

	gclsStack.SendWebSocketPacket( pszClientIp, iClientPort, strData.c_str(), strData.length() );

	return true;
}
