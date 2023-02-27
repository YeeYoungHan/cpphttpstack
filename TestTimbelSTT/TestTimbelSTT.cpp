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

// API ¹®¼­: https://ailab.sorizava.co.kr:3443/category/stt---online

#include "SipPlatformDefine.h"
#include "WebSocketClient.h"
#include "TimeUtility.h"
#include "StringUtility.h"
#include "Log.h"

class CCallBack : public IWebSocketClientCallBack
{
public:
	CCallBack() : m_bRecv(false){};
	virtual ~CCallBack(){};

	virtual bool RecvWebSocket( const char * pszPacket, int iPacketLen )
	{
		std::string strText;

		Utf8ToAnsi( pszPacket, strText );

		printf( "%s\n", strText.c_str() );
		m_bRecv = true;

		return true;
	}

	bool m_bRecv;
};

int main( int argc, char * argv[] )
{
	CWebSocketClient clsClient;
	CCallBack clsCallBack;

#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	//CLog::SetLevel( LOG_NETWORK | LOG_INFO | LOG_DEBUG );

	if( clsClient.Connect( "ws://ailab.sorizava.co.kr:40000/client/ws/speech?single=false&model=KOREAN_ONLINE_16K&verbosity=final&content-type=audio%2Fx-raw%2C+layout%3D%28string%29interleaved%2C+rate%3D%28int%298000%2C+format%3D%28string%29S16LE%2C+channels%3D%28int%291", &clsCallBack ) )
	{
		// Send 8k 16bit pcm file
		FILE * fd = fopen( "c:\\temp\\out.pcm", "rb" );
		if( fd == NULL )
		{
			printf( "file open error\n" );
		}
		else
		{
			char szBuf[4000];

			while( 1 )
			{
				int n = fread( szBuf, 1, 4000, fd );
				if( n <= 0 ) break;

				clsClient.Send( E_WST_BINARY, szBuf, n );
			}

			clsClient.Send( E_WST_TEXT, "EOS", 3 );

			while( clsClient.IsClosed() == false )
			{
				MiliSleep(20);
			}
		}
	}

	clsClient.Close();

	SSLClientStop();
	SSLFinal();

	return 0;
}
