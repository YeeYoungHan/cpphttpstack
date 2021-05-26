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

#ifndef _HTTP_MESSAGE_H_
#define _HTTP_MESSAGE_H_

#include "SipPlatformDefine.h"

#define HTTP_VERSION	"HTTP/1.1"

#include "HttpHeader.h"
#include "HttpUri.h"

/**
 * @defgroup HttpParser HttpParser
 * HTTP �޽��� �ļ�/���� ���̺귯��
 */

/**
 * @ingroup HttpParser
 * @brief HTTP �޽��� Ŭ����
 */
class CHttpMessage
{
public:
	CHttpMessage();
	~CHttpMessage();

	int Parse( const char * pszText, int iTextLen );
	int ParseHeader( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();

	bool AddHeader( const char * pszName, const char * pszValue );
	bool AddHeader( const char * pszName, int iValue );

	bool UpdateHeader( const char * pszName, const char * pszValue );
	bool ReplaceHeader( const char * pszName, const char * pszValue );
	
	CHttpHeader * GetHeader( const char * pszName );

	bool SetRequest( const char * pszMethod, CHttpUri * pclsUri, const char * pszUserAgent = NULL );
	bool IsRequest( );

	/** HTTP �޼ҵ� ( GET, POST ) */
	std::string		m_strHttpMethod;

	/** HTTP request URI */
	std::string		m_strReqUri;

	/** HTTP version ( HTTP/1.1 ) */
	std::string		m_strHttpVersion;

	/** HTTP ���� �ڵ�. HTTP ���� �޽����� ��쿡�� 0 ���� ū ���� ������ �ִ�. */
	int						m_iStatusCode;

	/** HTTP ���� �޽��� */
	std::string		m_strReasonPhrase;

	/** HTTP Content-Type ����� �� */
	std::string		m_strContentType;

	/** HTTP Content-Length ����� �� */
	int							m_iContentLength;

	/** body �� chunked ���� */
	bool						m_bChunked;

	/** HTTP ��� ����Ʈ */
	HTTP_HEADER_LIST		m_clsHeaderList;

	/** HTTP body */
	std::string			m_strBody;

	/** HTTP/2 stream identifier */
	uint32_t				m_iStreamIdentifier;

private:
	int ParseStatusLine( const char * pszText, int iTextLen );
	int ParseRequestLine( const char * pszText, int iTextLen );
};

#endif
