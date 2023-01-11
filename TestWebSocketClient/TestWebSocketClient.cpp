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

#include "WebSocketClient.h"
#include "TimeUtility.h"
#include "Log.h"

class CCallBack : public IWebSocketClientCallBack
{
public:
	CCallBack() : m_bRecv(false){};
	virtual ~CCallBack(){};

	virtual bool RecvWebSocket( const char * pszPacket, int iPacketLen )
	{
		printf( "recv[%.*s]\n", iPacketLen, pszPacket );
		m_bRecv = true;

		return true;
	}

	bool m_bRecv;
};

int main( int argc, char * argv[] )
{
	CWebSocketClient clsClient;
	CCallBack clsCallBack;
	std::string strData = "Hello";

	CLog::SetLevel( LOG_NETWORK | LOG_INFO | LOG_DEBUG );

	//if( clsClient.Connect( "wss://demo.piesocket.com/v3/channel_123?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_self", &clsCallBack ) )
	if( clsClient.Connect( "wss://ws.postman-echo.com/raw", &clsCallBack ) )
	{
		if( clsClient.Send( strData ) )
		{
			printf( "send[%s]\n", strData.c_str() );
		}

		while( clsClient.IsClosed() == false )
		{
			MiliSleep(20);
		}
	}

	return 0;
}
