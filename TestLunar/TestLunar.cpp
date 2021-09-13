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

#include "SipPlatformDefine.h"
#include "HttpClient.h"
#include "JsonObject.h"
#include <stdlib.h>

typedef struct _MonthDay_
{
	int iMonth;
	int iDay;
} MonthDay;

int main( int argc, char * argv[] )
{
	CHttpClient clsHttpClient;
	std::string strContentType, strContent;
	char szUrl[1024];
	MonthDay arrMonthDay[] = { { 1, 1 }, { 4, 8 }, { 8, 15 }, { 0, 0 } };
	// MonthDay arrMonthDay[] = { { 8, 15 }, { 0, 0 } };

	for( int iYear = 2021; iYear < 2050; ++iYear )
	{
		for( int i = 0; arrMonthDay[i].iMonth; ++i )
		{
			snprintf( szUrl, sizeof(szUrl), "http://astro.kasi.re.kr/life/lunc?yyyy=%04d&mm=%02d&dd=%02d", arrMonthDay[i].iMonth == 12 ? iYear - 1 : iYear, arrMonthDay[i].iMonth, arrMonthDay[i].iDay );

			if( clsHttpClient.DoGet( szUrl, strContentType, strContent ) )
			{
				CJsonObject clsJson;

				if( clsJson.Parse( strContent ) != -1 )
				{
					std::string strYear, strMonth, strDay;

					clsJson.SelectStringData( "SOLC_YYYY", strYear );
					clsJson.SelectStringData( "SOLC_MM", strMonth );
					clsJson.SelectStringData( "SOLC_DD", strDay );

					printf( "%s-%s-%s\n", strYear.c_str(), strMonth.c_str(), strDay.c_str() );
					// printf( "else if( iYear == %s )	strDate = \"%s-%s\";\n", strYear.c_str(), strMonth.c_str(), strDay.c_str() );
				}
			}
		}
	}

	return 0;
}
