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

#include "stdafx.h"
#include "SimpleHttpServer.h"
#include "HttpStatusCode.h"
#include "HttpParameterList.h"
#include "HttpMultipart.h"
#include "StringUtility.h"
#include "FileUtility.h"
#include "Directory.h"
#include "Log.h"
#include "MemoryDebug.h"

CSimpleHttpServer::CSimpleHttpServer() : m_bStop(false)
{
}

CSimpleHttpServer::~CSimpleHttpServer()
{
}

/**
 * @ingroup TestHttpStack
 * @brief HTTP ��û ���� �̺�Ʈ callback
 * @param pclsRequest		HTTP ��û �޽���
 * @param pclsResponse	HTTP ���� �޽��� - ���뿡�� �����Ѵ�.
 * @returns ���뿡�� HTTP ���� �޽����� ���������� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSimpleHttpServer::RecvHttpRequest( CHttpMessage * pclsRequest, CHttpMessage * pclsResponse )
{
	std::string strPath = m_strDocumentRoot;
	std::string strExt;

	//CLog::Print( LOG_DEBUG, "req uri[%s]", pclsRequest->m_strReqUri.c_str() );

	// ���Ȼ� .. �� ������ URL �� �����Ѵ�.
	if( strstr( pclsRequest->m_strReqUri.c_str(), ".." ) )
	{
		pclsResponse->m_iStatusCode = HTTP_NOT_FOUND;
		return true;
	}

#ifdef _DEBUG
	// �޸� ���� �˻縦 ���ؼ� exit.html �� �����ϸ� ���α׷��� �����Ѵ�.
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

	// ���� ��ο��� GET parameter �� �����Ѵ�.
	size_t iPos = strPath.find( "?" );
	if( iPos < std::string::npos )
	{
		strPath.erase( iPos );
	}

	if( IsExistFile( strPath.c_str() ) == false )
	{
		pclsResponse->m_iStatusCode = HTTP_NOT_FOUND;
		return true;
	}

	// ���Ϻ� Content-Type �� �����Ѵ�.
	GetFileExt( strPath.c_str(), strExt );
	const char * pszExt = strExt.c_str();
	
	if( !strcmp( pszExt, "html" ) || !strcmp( pszExt, "htm" ) )
	{
		pclsResponse->m_strContentType = "text/html";

		// GET Query parameter list ��������
		CHttpParameterList clsParamList;
		HTTP_PARAMETER_LIST::iterator itPL;

		if( clsParamList.ParseUrl( pclsRequest->m_strReqUri ) != -1 )
		{
			for( itPL = clsParamList.m_clsParamList.begin(); itPL != clsParamList.m_clsParamList.end(); ++itPL )
			{
				printf( "GET name[%s] = value[%s]\n", itPL->m_strName.c_str(), itPL->m_strValue.c_str() );
			}
		}

		if( !strcmp( pclsRequest->m_strHttpMethod.c_str(), "POST" ) )
		{
			if( strstr( pclsRequest->m_strContentType.c_str(), "multipart" ) )
			{
				CHttpMultipart clsMultipart;

				clsMultipart.SetContentType( pclsRequest->m_strContentType.c_str() );
				if( clsMultipart.Parse( pclsRequest->m_strBody ) != -1 )
				{
					HTTP_MULTIPART_DATA_MAP::iterator itMD;

					for( itMD = clsMultipart.m_clsMap.begin(); itMD != clsMultipart.m_clsMap.end(); ++itMD )
					{
						if( itMD->second->m_strFileName.empty() )
						{
							printf( "POST name[%s] = value[%s]\n", itMD->first.c_str(), itMD->second->m_strValue.c_str() );
						}
						else
						{
							// ���ε��� ������ ������ ���� �̸��� ������ �����ؾ� �Ѵ�.
							std::string strFileName = "c:\\temp\\upload.txt";

							FILE * fd = fopen( strFileName.c_str(), "wb" );
							if( fd )
							{
								fwrite( itMD->second->m_strValue.c_str(), 1, itMD->second->m_strValue.length(), fd );
								fclose( fd );
							}
						}
					}
				}
			}
			else
			{
				if( clsParamList.Parse( pclsRequest->m_strBody ) != -1 )
				{
					for( itPL = clsParamList.m_clsParamList.begin(); itPL != clsParamList.m_clsParamList.end(); ++itPL )
					{
						printf( "POST name[%s] = value[%s]\n", itPL->m_strName.c_str(), itPL->m_strValue.c_str() );
					}
				}
			}
		}
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

	// ������ �о HTTP body �� �����Ѵ�.
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
 * @brief WebSocket Ŭ���̾�Ʈ TCP ���� ���� �̺�Ʈ callback
 * @param pszClientIp WebSocket Ŭ���̾�Ʈ IP �ּ�
 * @param iClientPort WebSocket Ŭ���̾�Ʈ ��Ʈ ��ȣ
 */
void CSimpleHttpServer::WebSocketConnected( const char * pszClientIp, int iClientPort )
{
	printf( "WebSocket[%s:%d] connected\n", pszClientIp, iClientPort );
}

/**
 * @ingroup TestHttpStack
 * @brief WebSocket Ŭ���̾�Ʈ TCP ���� ���� �̺�Ʈ callback
 * @param pszClientIp WebSocket Ŭ���̾�Ʈ IP �ּ�
 * @param iClientPort WebSocket Ŭ���̾�Ʈ ��Ʈ ��ȣ
 */
void CSimpleHttpServer::WebSocketClosed( const char * pszClientIp, int iClientPort )
{
	printf( "WebSocket[%s:%d] closed\n", pszClientIp, iClientPort );
}

/**
 * @ingroup TestHttpStack
 * @brief WebSocket Ŭ���̾�Ʈ ������ ���� �̺�Ʈ callback
 * @param pszClientIp WebSocket Ŭ���̾�Ʈ IP �ּ�
 * @param iClientPort WebSocket Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @param strData			WebSocket Ŭ���̾�Ʈ�� ������ ������
 * @returns WebSocket Ŭ���̾�Ʈ ������ �����Ϸ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSimpleHttpServer::WebSocketData( const char * pszClientIp, int iClientPort, std::string & strData )
{
	printf( "WebSocket[%s:%d] recv[%s]\n", pszClientIp, iClientPort, strData.c_str() );

	return true;
}
