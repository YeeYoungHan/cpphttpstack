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

#include "TestHttp2Client.h"
#include "MemoryDebug.h"

int TestHttp2ClientLoop( int argc, char * argv[] )
{
	std::string strIp, strPath, strBodyType, strBody;
	int iPort;
	CHttp2Client clsClient;
	char szPcapFileName[255];

	if( argc >= 4 )
	{
		strIp = argv[2];
		iPort = atoi(argv[3]);
	}
	else
	{
		printf( "[Usage] %s loop {ip} {port}\n", argv[0] );
		return 0;
	}

	snprintf( szPcapFileName, sizeof(szPcapFileName), "c:\\temp\\%s.pcap", strIp.c_str() );

	CLog::SetLevel( LOG_DEBUG | LOG_HTTP_HEADER );

	if( clsClient.Connect( strIp.c_str(), iPort, NULL, szPcapFileName ) )
	{
		char	szCommand[1024];
		int iLen;

		memset( szCommand, 0, sizeof(szCommand) );
		while( fgets( szCommand, sizeof(szCommand), stdin ) )
		{
			iLen = (int)strlen( szCommand );
			if( iLen >= 2 && szCommand[iLen-2] == '\r' )
			{
				szCommand[iLen - 2] = '\0';
				iLen -= 2;
			}
			else if (iLen >= 1 && szCommand[iLen - 1] == '\n')
			{
				szCommand[iLen - 1] = '\0';
				--iLen;
			}

			if( szCommand[0] == 'q' ) break;

			if( !strncmp( szCommand, "get ", 4 ) )
			{
				if( clsClient.DoGet( szCommand + 4, strBodyType, strBody ) == false )
				{
					printf( "clsClient.DoGet error\n" );
					break;
				}
			}
			else
			{
				printf( "[Usage] get {path}\n" );
			}

			memset( szCommand, 0, sizeof(szCommand) );
		}		
	}

	return 0;
}
