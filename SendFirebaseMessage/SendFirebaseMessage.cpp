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

#include "SipTcp.h"
#include "HttpClient.h"
#include "Log.h"

int main( int argc, char * argv[] )
{
	CHttpClient clsApi;
	CHttpHeader clsHeader;
	HTTP_HEADER_LIST clsHeaderList;
	std::string strSendBody, strRecvContentType, strRecvBody;

	CLog::SetLevel( LOG_DEBUG | LOG_NETWORK | LOG_INFO );

	InitNetwork();

	clsHeader.m_strName = "Authorization";

	// QQQ: FCM 클라우드 메시징에서 서버키를 가져와서 아래의 {server key} 를 수정한다.
	clsHeader.m_strValue = "key={server key}";
	clsHeaderList.push_back( clsHeader );

	// QQQ: 안드로이드 앱의 FCM token 으로 아래의 {device token} 을 수정한다.
	strSendBody = "{ "
		"  \"registration_ids\" : [\"{device token}\"], "
		"  \"data\" : { \"dName\" : \"dValue\" } "
		"}";

	if( clsApi.DoPost( "https://fcm.googleapis.com/fcm/send", &clsHeaderList, "application/json", strSendBody.c_str(), strSendBody.length(), strRecvContentType, strRecvBody ) )
	{
		printf( "recv[%s]\n", strRecvBody.c_str() );
	}
	else
	{
		printf( "DoPost error\n" );
	}

	return 0;
}
