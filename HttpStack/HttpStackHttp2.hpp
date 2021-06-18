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

bool CHttpStack::RecvPacketHttp2Pri( CTcpSessionInfo * pclsSessionInfo, CHttpStackSession * pclsApp )
{
	pclsApp->m_eType = E_HST_HTTP_2;
	pclsApp->m_clsHttp2Packet.AddPacket( &pclsApp->m_clsHttpPacket );

	CHttp2Settings clsSettings;
	CHttp2Frame clsFrame;

	clsSettings.Add( HTTP2_SETTINGS_MAX_CONCURRENT_STREAMS, 10 );

	clsFrame.Set( HTTP2_FRAME_TYPE_SETTINGS, 0, 0, clsSettings.m_pszPacket, clsSettings.m_iPacketLen );
	if( pclsSessionInfo->Send( (char *)clsFrame.m_pszPacket, clsFrame.m_iPacketLen ) == false )
	{
		CLog::Print( LOG_ERROR, "Send(%s:%d) error", pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort );
		return false;
	}

	clsFrame.PrintLog( LOG_HTTP2, pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort, true );

	return RecvPacketHttp2( NULL, 0, pclsSessionInfo, pclsApp );
}

bool CHttpStack::RecvPacketHttp2( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo, CHttpStackSession * pclsApp )
{
	pclsApp->m_clsHttp2Packet.AddPacket( (uint8_t *)pszPacket, iPacketLen );

	CHttp2Frame clsFrame;
	CHttpMessageSendRecv * pclsData;

	while( pclsApp->m_clsHttp2Packet.GetFrame( &clsFrame ) )
	{
		clsFrame.PrintLog( LOG_HTTP2, pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort, false );
		pclsData = pclsApp->GetMessage( clsFrame.GetStreamIdentifier() );

		switch( clsFrame.GetType() )
		{
		case HTTP2_FRAME_TYPE_HEADERS:
		case HTTP2_FRAME_TYPE_DATA:
			pclsApp->m_clsRecvConversion.MakeMessage( clsFrame, pclsData->m_clsRecv );
			if( clsFrame.GetFlags() & HTTP2_FLAG_END_STREAM )
			{
				if( m_clsTcpStack.m_clsSetup.m_bUseHttp2HeaderLog )
				{
					std::string strLog;

					pclsData->m_clsRecv.ToString( strLog, true );
					CLog::Print( LOG_HTTP2, "Recv(%s:%d) [%s]", pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort, strLog.c_str() );
				}

				if( m_pclsCallBack->RecvHttpRequest( &pclsData->m_clsRecv, &pclsData->m_clsSend ) == false )
				{
					CLog::Print( LOG_ERROR, "%s RecvHttpRequest error", __FUNCTION__ );
					return false;
				}

				if( m_clsTcpStack.m_clsSetup.m_bUseHttp2HeaderLog )
				{
					std::string strLog;

					pclsData->m_clsSend.ToString( strLog, true );
					CLog::Print( LOG_HTTP2, "Send(%s:%d) [%s]", pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort, strLog.c_str() );
				}

				pclsData->m_clsSend.m_iStreamIdentifier = clsFrame.GetStreamIdentifier();

				if( pclsApp->m_clsSendConversion.MakeFrameList( pclsData->m_clsSend, pclsApp->m_clsFrameList ) == false )
				{
					CLog::Print( LOG_ERROR, "%s MakeFrameList error", __FUNCTION__ );
					return false;
				}

				HTTP2_FRAME_LIST::iterator itFL;

				for( itFL = pclsApp->m_clsFrameList.m_clsList.begin(); itFL != pclsApp->m_clsFrameList.m_clsList.end(); ++itFL )
				{
					if( pclsSessionInfo->Send( (char *)((*itFL)->m_pszPacket), (*itFL)->m_iPacketLen ) == false )
					{
						CLog::Print( LOG_ERROR, "Send(%s:%d) error", pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort );
						return false;
					}

					(*itFL)->PrintLog( LOG_HTTP2, pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort, true );
				}
			}
			break;
		case HTTP2_FRAME_TYPE_SETTINGS:
			if( clsFrame.GetFlags() == 0 )
			{
				clsFrame.Set( HTTP2_FRAME_TYPE_SETTINGS, HTTP2_FLAG_ACK, 0, NULL, 0 );
				if( pclsSessionInfo->Send( (char *)clsFrame.m_pszPacket, clsFrame.m_iPacketLen ) == false )
				{
					CLog::Print( LOG_ERROR, "%s Send(%s:%d) error", __FUNCTION__, pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort );
					return false;
				}

				clsFrame.PrintLog( LOG_HTTP2, pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort, true );
			}
			break;
		case HTTP2_FRAME_TYPE_RST_STREAM:
		case HTTP2_FRAME_TYPE_GOAWAY:
			break;
		case HTTP2_FRAME_TYPE_PING:
			if( clsFrame.GetFlags() == 0 )
			{
				clsFrame.SetFlags( HTTP2_FLAG_ACK );
				if( pclsSessionInfo->Send( (char *)clsFrame.m_pszPacket, clsFrame.m_iPacketLen ) == false )
				{
					CLog::Print( LOG_ERROR, "%s Send(%s:%d) error", __FUNCTION__, pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort );
					return false;
				}

				clsFrame.PrintLog( LOG_HTTP2, pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort, true );
			}
			break;
		}
	}

	return true;
}