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
#include "HtmlElement.h"
#include "MemoryDebug.h"

static bool TestHtmlElement( int iLine, const char * pszInput, const char * pszOutput )
{
	CHtmlElement clsHtml;
	std::string strOutput;

	if( clsHtml.Parse( pszInput, (int)strlen(pszInput) ) == -1 )
	{
		printf( "%s line(%d) Parse error - input(%s)\n", __FUNCTION__, iLine, pszInput );
		return false;
	}

	clsHtml.ToString( strOutput );

	if( strcmp( pszOutput, strOutput.c_str() ) )
	{
		printf( "%s line(%d) Parse error - input(%s) output(%s) != result(%s)\n", __FUNCTION__, iLine, pszInput, pszOutput, strOutput.c_str() );
		return false;
	}

	return true;
}

bool TestHtmlElement( )
{
	if( TestHtmlElement( __LINE__, "<html><head><title>TITLE</title></head><body>BODY</body></html>"
		, "<html>\n<head>\n<title>TITLE</title>\n</head>\n<body>BODY</body>\n</html>\n" ) == false ) return false;

	if( TestHtmlElement( __LINE__, " <html> <head> <title>TITLE</title> </head> <body>BODY</body> </html>"
		, "<html>\n<head>\n<title>TITLE</title>\n</head>\n<body>BODY</body>\n</html>\n" ) == false ) return false;

	if( TestHtmlElement( __LINE__, " <html>\r\n\r\n<head>\r\n\r\n<title>TITLE</title>\r\n\r\n</head>\r\n\r\n<body>BODY</body>\r\n\r\n</html>\r\n\r\n"
		, "<html>\n<head>\n<title>TITLE</title>\n</head>\n<body>BODY</body>\n</html>\n" ) == false ) return false;

	if( TestHtmlElement( __LINE__, " <html> <head> <title> TITLE </title> </head> <body> BODY </body> </html>"
		, "<html>\n<head>\n<title> TITLE </title>\n</head>\n<body> BODY </body>\n</html>\n" ) == false ) return false;

	if( TestHtmlElement( __LINE__, "<html><head><title>TITLE</Title></HEAD><body>BODY</BOdy></HTML>"
		, "<html>\n<head>\n<title>TITLE</title>\n</head>\n<body>BODY</body>\n</html>\n" ) == false ) return false;

	// script ÆÄ½Ì Å×½ºÆ®
	if( TestHtmlElement( __LINE__, "<html><head><title>TITLE</title><script>function a(){\n\tif( c > a )\n\t\treturn false;\n\treturn true;\n\t}\n</script></head><body>BODY</body></html>"
		, "<html>\n<head>\n<title>TITLE</title>\n<script>function a(){\n\tif( c > a )\n\t\treturn false;\n\treturn true;\n\t}\n</script>\n</head>\n<body>BODY</body>\n</html>\n" ) == false ) return false;

	if( TestHtmlElement( __LINE__, "<html><head><title>TITLE</title><script>function a(){\n\tif( c < a )\n\t\treturn false;\n\treturn true;\n\t}\n</script></head><body>BODY</body></html>"
		, "<html>\n<head>\n<title>TITLE</title>\n<script>function a(){\n\tif( c < a )\n\t\treturn false;\n\treturn true;\n\t}\n</script>\n</head>\n<body>BODY</body>\n</html>\n" ) == false ) return false;

	// style ÆÄ½Ì Å×½ºÆ®
	if( TestHtmlElement( __LINE__, "<html><head><title>TITLE</title><style>.div > div { float:left; width: 50% }</style></head><body>BODY</body></html>"
		, "<html>\n<head>\n<title>TITLE</title>\n<style>.div > div { float:left; width: 50% }</style>\n</head>\n<body>BODY</body>\n</html>\n" ) == false ) return false;

	// meta tag ÆÄ½Ì Å×½ºÆ®
	if( TestHtmlElement( __LINE__, "<html><head><meta charset=\"utf-8\"><title>TITLE</title></head><body>BODY</body></html>"
		, "<html>\n<head>\n<meta charset=\"utf-8\">\n<title>TITLE</title>\n</head>\n<body>BODY</body>\n</html>\n" ) == false ) return false;

	if( TestHtmlElement( __LINE__, "<html><head><meta charset=\"utf-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><title>TITLE</title></head><body>BODY</body></html>"
		, "<html>\n<head>\n<meta charset=\"utf-8\">\n<meta content=\"IE=edge\" http-equiv=\"X-UA-Compatible\">\n<title>TITLE</title>\n</head>\n<body>BODY</body>\n</html>\n" ) == false ) return false;

	if( TestHtmlElement( __LINE__, "<html><head><meta charset=\"utf-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>TITLE</title></head><body>BODY</body></html>"
		, "<html>\n<head>\n<meta charset=\"utf-8\">\n<meta content=\"IE=edge\" http-equiv=\"X-UA-Compatible\">\n<meta content=\"width=device-width, initial-scale=1\" name=\"viewport\">\n<title>TITLE</title>\n</head>\n<body>BODY</body>\n</html>\n" ) == false ) return false;

	// link tag ÆÄ½Ì Å×½ºÆ®
	if( TestHtmlElement( __LINE__, "<html><head><link href=\"test.css\" rel=\"stylesheet\"><title>TITLE</title></head><body>BODY</body></html>"
		, "<html>\n<head>\n<link href=\"test.css\" rel=\"stylesheet\">\n<title>TITLE</title>\n</head>\n<body>BODY</body>\n</html>\n" ) == false ) return false;

	// br tag ÆÄ½Ì Å×½ºÆ®
	if( TestHtmlElement( __LINE__, "<html><head><title>TITLE</title></head><body>BODY<br>BODY2<br>BODY3<br></body></html>"
		, "<html>\n<head>\n<title>TITLE</title>\n</head>\n<body>\nBODY<br>\nBODY2<br>\nBODY3<br>\n</body>\n</html>\n" ) == false ) return false;

	if( TestHtmlElement( __LINE__, "<html><head><title>TITLE</title></head><body>BODY<br />BODY2<br />BODY3<br /></body></html>"
		, "<html>\n<head>\n<title>TITLE</title>\n</head>\n<body>\nBODY<br>\nBODY2<br>\nBODY3<br>\n</body>\n</html>\n" ) == false ) return false;

	// hr tag ÆÄ½Ì Å×½ºÆ®
	if( TestHtmlElement( __LINE__, "<html><head><title>TITLE</title></head><body>BODY<hr>BODY2<hr>BODY3<hr></body></html>"
		, "<html>\n<head>\n<title>TITLE</title>\n</head>\n<body>\nBODY<hr>\nBODY2<hr>\nBODY3<hr>\n</body>\n</html>\n" ) == false ) return false;

	if( TestHtmlElement( __LINE__, "<html><head><title>TITLE</title></head><body>BODY<hr class='hr1'>BODY2<hr class='hr2'>BODY3<hr class='hr3'></body></html>"
		, "<html>\n<head>\n<title>TITLE</title>\n</head>\n<body>\nBODY<hr class='hr1'>\nBODY2<hr class='hr2'>\nBODY3<hr class='hr3'>\n</body>\n</html>\n" ) == false ) return false;

	return true;
}
