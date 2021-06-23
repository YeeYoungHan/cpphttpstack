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

#include "SipPlatformDefine.h"
#include "SipTcp.h"
#include "Http2Client.h"
#include "Log.h"

int main( int argc, char * argv[] )
{
	CLog::SetLevel( LOG_DEBUG | LOG_HTTP_HEADER | LOG_INFO );

	if( argc != 4 )
	{
		printf( "[Usage] %s {pem path} {device id} {apns-topic}\n", argv[0] );
		return 0;
	}

	// TLS 클라이언트 KEY + 인증서를 저장한 pem 파일의 full path
	const char * pszPemFilePath = argv[1];
	const char * pszDeviceId = argv[2];
	const char * pszApnsTopic = argv[3];

	CHttp2Client	clsClient;
	CHttpHeader		clsHeader;
	HTTP_HEADER_LIST clsHeaderList;

	std::string strOutputContentType, strOutputBody;
	char szPath[255];
	char szInputBody[255];

	snprintf( szPath, sizeof(szPath), "/3/device/%s", pszDeviceId );
	//snprintf( szInputBody, sizeof(szInputBody), "{ \"aps\" : { \"alert\" : \"Hello\" } }" );
	snprintf( szInputBody, sizeof(szInputBody), "{ \"aps\" : { \"from\" : \"%s\", \"type\" : \"%d\" } }", "01012345678", 1 );

	//clsHeader.Set( "apns-push-type", "alert" );
	clsHeader.Set( "apns-push-type", "voip" );
	clsHeaderList.push_back( clsHeader );

	if( clsClient.Connect( "api.sandbox.push.apple.com", 443, pszPemFilePath, "c:\\temp\\h2.pcap" ) )
	{
		clsClient.DoPost( szPath, &clsHeaderList, "application/json", szInputBody, strlen(szInputBody), strOutputContentType, strOutputBody );
		clsClient.Close();

		printf( "output body[%s]\n", strOutputBody.c_str() );
	}

	return 0;
}
