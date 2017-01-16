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

#include "HttpStack.h"
#include "Log.h"
#include "MemoryDebug.h"

CHttpStack::CHttpStack() : m_pclsCallBack(NULL)
{
}

CHttpStack::~CHttpStack()
{
}

/**
 * @ingroup HttpStack
 * @brief HTTP 서버를 시작한다.
 * @param pclsSetup			설정 객체
 * @param pclsCallBack	callback 객체
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttpStack::Start( CTcpStackSetup * pclsSetup, IHttpStackCallBack * pclsCallBack )
{
	if( pclsCallBack == NULL )
	{
		CLog::Print( LOG_ERROR, "%s IHttpStackCallBack is NULL", __FUNCTION__ );
		return false;
	}

	m_pclsCallBack = pclsCallBack;

	return m_clsTcpStack.Start( pclsSetup, this );
}

/**
 * @ingroup HttpStack
 * @brief 
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttpStack::Stop( )
{
	return m_clsTcpStack.Stop();
}

bool CHttpStack::InComingConnected( CTcpSessionInfo * pclsSessionInfo )
{
	return true;
}

void CHttpStack::SessionClosed( CTcpSessionInfo * pclsSessionInfo )
{

}

bool CHttpStack::RecvPacket( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo )
{
	if( pclsSessionInfo->m_pclsApp == NULL )
	{
		pclsSessionInfo->m_pclsApp = new CHttpStackSession();
		if( pclsSessionInfo->m_pclsApp == NULL )
		{
			CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
			return false;
		}
	}

	CHttpStackSession * pclsApp = (CHttpStackSession *)pclsSessionInfo->m_pclsApp;

	if( pclsApp->m_clsPacket.AddPacket( pszPacket, iPacketLen ) == false )
	{
		CLog::Print( LOG_ERROR, "%s m_clsPacket.AddPacket error", __FUNCTION__ );
		return false;
	}

	if( pclsApp->m_clsPacket.IsCompleted() )
	{
		CHttpMessage * pclsRecv = pclsApp->m_clsPacket.GetHttpMessage();
		if( pclsRecv->IsRequest() )
		{
			CHttpMessage clsSend;
			bool bClose = false;

			CHttpHeader * pclsHeader = pclsRecv->GetHeader( "Connection" );
			if( pclsHeader && !strcmp( pclsHeader->m_strValue.c_str(), "close" ) )
			{
				bClose = true;
			}

			if( m_pclsCallBack->RecvHttpRequest( pclsRecv, &clsSend ) == false )
			{
				CLog::Print( LOG_ERROR, "%s RecvHttpRequest error", __FUNCTION__ );
				return false;
			}

			int iNewBufLen = 8192 + clsSend.m_strBody.length();
			char * pszBuf = (char *)malloc( iNewBufLen );
			if( pszBuf == NULL )
			{
				CLog::Print( LOG_ERROR, "%s malloc error", __FUNCTION__ );
				return false;
			}
			
			int iBufLen = clsSend.ToString( pszBuf, iNewBufLen );
			if( iBufLen == -1 )
			{
				CLog::Print( LOG_ERROR, "%s ToString error", __FUNCTION__ );
				free( pszBuf );
				return false;
			}

			if( pclsSessionInfo->Send( pszBuf, iBufLen ) == false )
			{
				CLog::Print( LOG_ERROR, "%s Send error", __FUNCTION__ );
				free( pszBuf );
				return false;
			}

			free( pszBuf );

			if( bClose )
			{
				return false;
			}
		}
		else
		{
			CLog::Print( LOG_ERROR, "%s http request error", __FUNCTION__ );
			return false;
		}
	}

	return true;
}
