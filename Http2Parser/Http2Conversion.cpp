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
#include "Http2Define.h"
#include "Http2Conversion.h"
#include "Log.h"
#include <stdlib.h>
#include "MemoryDebug.h"

CHttp2Conversion::CHttp2Conversion() : m_pclsMessage(NULL), m_pclsFrameList(NULL), m_iHeaderFrameCount(0)
{
}

CHttp2Conversion::~CHttp2Conversion()
{
}

/**
 * @ingroup Http2Parser
 * @brief HTTP/1.1 을 HTTP/2 로 전환한다.
 * @param clsMessage		HTTP/1.1 메시지
 * @param clsFrameList	[out] HTTP/2 frame 리스트
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Conversion::MakeFrameList( CHttpMessage & clsMessage, CHttp2FrameList & clsFrameList )
{
	CHttp2Frame * pclsFrame;

	m_pclsMessage = &clsMessage;
	m_pclsFrameList = &clsFrameList;

	clsFrameList.Clear();
	m_clsHeader.Clear();
	m_iHeaderFrameCount = 0;

	if( clsMessage.m_iStatusCode > 0 )
	{
		switch( clsMessage.m_iStatusCode )
		{
		case 200:
			m_clsHeader.AddIndex( HTTP2_INDEX_STATUS_200 );
			break;
		case 204:
			m_clsHeader.AddIndex( HTTP2_INDEX_STATUS_204 );
			break;
		case 206:
			m_clsHeader.AddIndex( HTTP2_INDEX_STATUS_206 );
			break;
		case 304:
			m_clsHeader.AddIndex( HTTP2_INDEX_STATUS_304 );
			break;
		case 400:
			m_clsHeader.AddIndex( HTTP2_INDEX_STATUS_400 );
			break;
		case 404:
			m_clsHeader.AddIndex( HTTP2_INDEX_STATUS_404 );
			break;
		case 500:
			m_clsHeader.AddIndex( HTTP2_INDEX_STATUS_500 );
			break;
		default:
			{
				char szStatus[11];

				snprintf( szStatus, sizeof(szStatus), "%d", clsMessage.m_iStatusCode );
				m_clsHeader.AddIndexValue( HTTP2_INDEX_STATUS_200, szStatus );
			}
			break;
		}
	}
	else 
	{
		if( clsMessage.m_strHttpMethod.empty() )
		{
			CLog::Print( LOG_ERROR, "%s clsMessage.m_strHttpMethod is empty", __FUNCTION__ );
			return false;
		}

		const char * pszMethod = clsMessage.m_strHttpMethod.c_str();
		if( !strcasecmp( pszMethod, "GET" ) )
		{
			m_clsHeader.AddIndex( HTTP2_INDEX_METHOD_GET );
		}
		else if( !strcasecmp( pszMethod, "POST" ) )
		{
			m_clsHeader.AddIndex( HTTP2_INDEX_METHOD_POST );
		}
		else
		{
			AddIndexValue( HTTP2_INDEX_METHOD_GET, pszMethod );
		}

		AddIndexValue( HTTP2_INDEX_PATH, clsMessage.m_strReqUri.c_str() );
		AddIndex( HTTP2_INDEX_SCHEME_HTTPS );
	}

	if( clsMessage.m_strContentType.empty() == false )
	{
		AddIndexValue( HTTP2_INDEX_CONTENT_TYPE, clsMessage.m_strContentType.c_str() );
	}

	const char * pszName;

	for( HTTP_HEADER_LIST::iterator itList = clsMessage.m_clsHeaderList.begin(); itList != clsMessage.m_clsHeaderList.end(); ++itList )
	{
		pszName = itList->m_strName.c_str();

		if( !strcasecmp( pszName, "host" ) )
		{
			AddIndexValue( HTTP2_INDEX_AUTHORITY, itList->m_strValue.c_str() );
		}
		else if( !strcasecmp( pszName, "content-type" ) )
		{
			AddIndexValue( HTTP2_INDEX_CONTENT_TYPE, itList->m_strValue.c_str() );
		}
		else if( !strcasecmp( pszName, "cookie" ) )
		{
			AddIndexValue( HTTP2_INDEX_COOKIE, itList->m_strValue.c_str() );
		}
		else
		{
			AddNameValue( itList->m_strName.c_str(), itList->m_strValue.c_str() );
		}
	}

	uint8_t cType, cFlag;

	if( m_clsHeader.m_iPacketLen > 0 )
	{
		pclsFrame = clsFrameList.CreateFrame();
		if( pclsFrame == NULL )
		{
			return false;
		}

		++m_iHeaderFrameCount;

		cType = HTTP2_FRAME_TYPE_HEADERS;

		if( m_iHeaderFrameCount >= 2 )
		{
			cType = HTTP2_FRAME_TYPE_CONTINUATION;
		}

		cFlag = HTTP2_FLAG_END_HEADER;

		if( clsMessage.m_strBody.empty() )
		{
			cFlag |= HTTP2_FLAG_END_STREAM;
		}

		if( pclsFrame->Set( cType, cFlag, m_pclsMessage->m_iStreamIdentifier, m_clsHeader.m_pszPacket, m_clsHeader.m_iPacketLen ) == false ) return false;
	}

	if( clsMessage.m_strBody.empty() == false )
	{
		const char * pszBody = clsMessage.m_strBody.c_str();
		int iBodyLen = (int)clsMessage.m_strBody.length();

		while( iBodyLen > 0 )
		{
			pclsFrame = clsFrameList.CreateFrame();
			if( pclsFrame == NULL )
			{
				return false;
			}

			cType = HTTP2_FRAME_TYPE_DATA;

			if( iBodyLen > HTTP2_FRAME_SIZE )
			{
				if( pclsFrame->Set( cType, 0, m_pclsMessage->m_iStreamIdentifier, (uint8_t *)pszBody, HTTP2_FRAME_SIZE ) == false ) return false;
			}
			else
			{
				if( pclsFrame->Set( cType, HTTP2_FLAG_END_STREAM, m_pclsMessage->m_iStreamIdentifier, (uint8_t *)pszBody, iBodyLen ) == false ) return false;
				break;
			}

			pszBody += HTTP2_FRAME_SIZE;
			iBodyLen -= HTTP2_FRAME_SIZE;
		}
	}

	return true;
}

/**
 * @ingroup Http2Parser
 * @brief HTTP/2 frame 을 HTTP/1.1 메시지에 저장한다.
 * @param clsFrame		HTTP/2 frame
 * @param clsMessage	[out] HTTP/1.1 메시지
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Conversion::MakeMessage( CHttp2Frame & clsFrame, CHttpMessage & clsMessage )
{
	if( clsFrame.GetType() == HTTP2_FRAME_TYPE_HEADERS )
	{
		if( clsMessage.m_strHttpVersion.empty() ) clsMessage.m_strHttpVersion = "HTTP/2";

		uint8_t * pszBody = clsFrame.GetBody();
		if( pszBody == NULL )
		{
			CLog::Print( LOG_ERROR, "%s GetBody() return NULL", __FUNCTION__ );
			return false;
		}

		int iBodyLen = clsFrame.GetBodyLen();
		int iBodyPos = 0, n;
		CHttp2HpackHeader clsHpack;

		while( iBodyLen > iBodyPos )
		{
			n = clsHpack.Parse( pszBody + iBodyPos, iBodyLen - iBodyPos );
			if( n == -1 ) return false;

			iBodyPos += n;

			HpackToMessage( clsHpack, clsMessage );
		}
	}
	else
	{
		clsMessage.m_strBody.append( (char *)clsFrame.GetBody(), clsFrame.GetBodyLen() );
		clsMessage.m_iContentLength += (int)clsMessage.m_strBody.length();
	}

	return true;
}

/**
 * @ingroup Http2Parser
 * @brief 인덱스만 존재하는 HTTP/2 헤더를 추가한다.
 * @param iIndex 인덱스 번호
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Conversion::AddIndex( uint32_t iIndex )
{
	if( m_clsHeader.AddIndex( iIndex ) == false )
	{
		CHttp2Frame * pclsFrame = m_pclsFrameList->CreateFrame();
		if( pclsFrame == NULL ) return false;

		++m_iHeaderFrameCount;

		uint8_t cType = HTTP2_FRAME_TYPE_HEADERS;

		if( m_iHeaderFrameCount >= 2 )
		{
			cType = HTTP2_FRAME_TYPE_CONTINUATION;
		}

		if( pclsFrame->Set( cType, 0, m_pclsMessage->m_iStreamIdentifier, m_clsHeader.m_pszPacket, m_clsHeader.m_iPacketLen ) == false ) return false;

		m_clsHeader.Clear();
		if( m_clsHeader.AddIndex( iIndex ) == false )
		{
			return false;
		}
	}

	return true;
}


/**
 * @ingroup Http2Parser
 * @brief 인덱스 & 값이 존재하는 HTTP/2 헤더를 추가한다.
 * @param iIndex		인덱스 번호
 * @param pszValue	헤더 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Conversion::AddIndexValue( uint32_t iIndex, const char * pszValue )
{
	if( m_clsHeader.AddIndexValue( iIndex, pszValue ) == false )
	{
		CHttp2Frame * pclsFrame = m_pclsFrameList->CreateFrame();
		if( pclsFrame == NULL ) return false;

		++m_iHeaderFrameCount;

		uint8_t cType = HTTP2_FRAME_TYPE_HEADERS;

		if( m_iHeaderFrameCount >= 2 )
		{
			cType = HTTP2_FRAME_TYPE_CONTINUATION;
		}

		if( pclsFrame->Set( cType, 0, m_pclsMessage->m_iStreamIdentifier, m_clsHeader.m_pszPacket, m_clsHeader.m_iPacketLen ) == false ) return false;

		m_clsHeader.Clear();
		if( m_clsHeader.AddIndexValue( iIndex, pszValue ) == false )
		{
			return false;
		}
	}

	return true;
}

/**
 * @ingroup Http2Parser
 * @brief 이름 & 값이 존재하는 HTTP/2 헤더를 추가한다.
 * @param pszName		헤더 이름
 * @param pszValue	헤더 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Conversion::AddNameValue( const char * pszName, const char * pszValue )
{
	if( m_clsHeader.AddNameValue( pszName, pszValue ) == false )
	{
		CHttp2Frame * pclsFrame = m_pclsFrameList->CreateFrame();
		if( pclsFrame == NULL ) return false;

		++m_iHeaderFrameCount;

		uint8_t cType = HTTP2_FRAME_TYPE_HEADERS;

		if( m_iHeaderFrameCount >= 2 )
		{
			cType = HTTP2_FRAME_TYPE_CONTINUATION;
		}

		if( pclsFrame->Set( cType, 0, m_pclsMessage->m_iStreamIdentifier, m_clsHeader.m_pszPacket, m_clsHeader.m_iPacketLen ) == false ) return false;

		m_clsHeader.Clear();
		if( m_clsHeader.AddNameValue( pszName, pszValue ) == false )
		{
			return false;
		}
	}

	return true;
}

/**
 * @ingroup Http2Parser
 * @brief HPACK 헤더를 HTTP/1.1 메시지에 저장한다.
 * @param clsHpack		HPACK 헤더
 * @param clsMessage	[out] HTTP/1.1 메시지
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttp2Conversion::HpackToMessage( CHttp2HpackHeader & clsHpack, CHttpMessage & clsMessage )
{
	bool bRes = false;

	if( clsHpack.m_iIndex == 0 )
	{
		if( clsHpack.m_strName.empty() == false && clsHpack.m_strValue.empty() == false )
		{
			bRes = clsMessage.AddHeader( clsHpack.m_strName.c_str(), clsHpack.m_strValue );
		}
	}
	else if( clsHpack.m_iIndex <= 61 )
	{
		switch( clsHpack.m_iIndex )
		{
		case HTTP2_INDEX_AUTHORITY:
			HpackAddHeader( clsHpack, clsMessage, "Authority" );
			break;
		case HTTP2_INDEX_METHOD_GET:
			HpackToString( clsHpack, clsMessage.m_strHttpMethod, "GET", HTTP2_METHOD_TYPE );
			break;
		case HTTP2_INDEX_METHOD_POST:
			HpackToString( clsHpack, clsMessage.m_strHttpMethod, "POST", HTTP2_METHOD_TYPE );
			break;
		case HTTP2_INDEX_PATH:
			HpackToString( clsHpack, clsMessage.m_strReqUri, "/", HTTP2_PATH_TYPE );
			break;
		case HTTP2_INDEX_PATH_INDEX_HTML:
			HpackToString( clsHpack, clsMessage.m_strReqUri, "/index.html", HTTP2_PATH_TYPE );
			break;
		case HTTP2_INDEX_SCHEME_HTTP:
			break;
		case HTTP2_INDEX_SCHEME_HTTPS:
			break;
		case HTTP2_INDEX_STATUS_200:
			HpackToInt( clsHpack, clsMessage.m_iStatusCode, 200, HTTP2_STATUS_CODE_TYPE );
			break;
		case HTTP2_INDEX_STATUS_204:
			HpackToInt( clsHpack, clsMessage.m_iStatusCode, 204, HTTP2_STATUS_CODE_TYPE );
			break;
		case HTTP2_INDEX_STATUS_206:
			HpackToInt( clsHpack, clsMessage.m_iStatusCode, 206, HTTP2_STATUS_CODE_TYPE );
			break;
		case HTTP2_INDEX_STATUS_304:
			HpackToInt( clsHpack, clsMessage.m_iStatusCode, 304, HTTP2_STATUS_CODE_TYPE );
			break;
		case HTTP2_INDEX_STATUS_400:
			HpackToInt( clsHpack, clsMessage.m_iStatusCode, 400, HTTP2_STATUS_CODE_TYPE );
			break;
		case HTTP2_INDEX_STATUS_404:
			HpackToInt( clsHpack, clsMessage.m_iStatusCode, 404, HTTP2_STATUS_CODE_TYPE );
			break;
		case HTTP2_INDEX_STATUS_500:
			HpackToInt( clsHpack, clsMessage.m_iStatusCode, 500, HTTP2_STATUS_CODE_TYPE );
			break;
		case HTTP2_INDEX_ACCEPT_CHARSET:
			HpackAddHeader( clsHpack, clsMessage, "Accept-Charset" );
			break;
		case HTTP2_INDEX_ACCEPT_ENCODING:
			HpackAddHeader( clsHpack, clsMessage, "Accept-Encoding" );
			break;
		case HTTP2_INDEX_ACCEPT_LANGUAGE:
			HpackAddHeader( clsHpack, clsMessage, "Accept-Language" );
			break;
		case HTTP2_INDEX_ACCEPT_RANGES:
			HpackAddHeader( clsHpack, clsMessage, "Accept-Ranges" );
			break;
		case HTTP2_INDEX_ACCEPT:
			HpackAddHeader( clsHpack, clsMessage, "Accept" );
			break;
		case HTTP2_INDEX_ACCESS_CONTROL_ALLOW_ORIGIN:
			HpackAddHeader( clsHpack, clsMessage, "Access-Control-Allow-Origin" );
			break;
		case HTTP2_INDEX_AGE:
			HpackAddHeader( clsHpack, clsMessage, "Age" );
			break;
		case HTTP2_INDEX_ALLOW:
			HpackAddHeader( clsHpack, clsMessage, "Allow" );
			break;
		case HTTP2_INDEX_AUTHORIZATION:
			HpackAddHeader( clsHpack, clsMessage, "Authorization" );
			break;
		case HTTP2_INDEX_CACHE_CONTROL:
			HpackAddHeader( clsHpack, clsMessage, "Cache-Control" );
			break;
		case HTTP2_INDEX_CONTENT_DISPOSITION:
			HpackAddHeader( clsHpack, clsMessage, "Content-Disposition" );
			break;
		case HTTP2_INDEX_CONTENT_ENCODING:
			HpackAddHeader( clsHpack, clsMessage, "Content-Encoding" );
			break;
		case HTTP2_INDEX_CONTENT_LANGUAGE:
			HpackAddHeader( clsHpack, clsMessage, "Content-Language" );
			break;
		case HTTP2_INDEX_CONTENT_LENGTH:
			HpackToInt( clsHpack, clsMessage.m_iContentLength, 0, HTTP2_CONTENT_LENGTH_TYPE );
			break;
		case HTTP2_INDEX_CONTENT_LOCATION:
			HpackAddHeader( clsHpack, clsMessage, "Content-Location" );
			break;
		case HTTP2_INDEX_CONTENT_RANGE:
			HpackAddHeader( clsHpack, clsMessage, "Content-Range" );
			break;
		case HTTP2_INDEX_CONTENT_TYPE:
			HpackToString( clsHpack, clsMessage.m_strContentType, "", HTTP2_CONTENT_TYPE_TYPE );
			break;
		case HTTP2_INDEX_COOKIE:
			HpackAddHeader( clsHpack, clsMessage, "Cookie" );
			break;
		case HTTP2_INDEX_DATE:
			HpackAddHeader( clsHpack, clsMessage, "Date" );
			break;
		case HTTP2_INDEX_ETAG:
			HpackAddHeader( clsHpack, clsMessage, "ETag" );
			break;
		case HTTP2_INDEX_EXPECT:
			HpackAddHeader( clsHpack, clsMessage, "Expect" );
			break;
		case HTTP2_INDEX_EXPIRES:
			HpackAddHeader( clsHpack, clsMessage, "Expires" );
			break;
		case HTTP2_INDEX_FROM:
			HpackAddHeader( clsHpack, clsMessage, "From" );
			break;
		case HTTP2_INDEX_HOST:
			HpackAddHeader( clsHpack, clsMessage, "Host" );
			break;
		case HTTP2_INDEX_IF_MATCH:
			HpackAddHeader( clsHpack, clsMessage, "If-Match" );
			break;
		case HTTP2_INDEX_IF_MODIFIED_SINCE:
			HpackAddHeader( clsHpack, clsMessage, "If-Modified-Since" );
			break;
		case HTTP2_INDEX_IF_NONE_MATCH:
			HpackAddHeader( clsHpack, clsMessage, "If-None-Match" );
			break;
		case HTTP2_INDEX_IF_RANGE:
			HpackAddHeader( clsHpack, clsMessage, "If-Range" );
			break;
		case HTTP2_INDEX_IF_UNMODIFIED_SINCE:
			HpackAddHeader( clsHpack, clsMessage, "If-Unmodified-Since" );
			break;
		case HTTP2_INDEX_LAST_MODIFIED:
			HpackAddHeader( clsHpack, clsMessage, "Last-Modified" );
			break;
		case HTTP2_INDEX_LINK:
			HpackAddHeader( clsHpack, clsMessage, "Link" );
			break;
		case HTTP2_INDEX_LOCATION:
			HpackAddHeader( clsHpack, clsMessage, "Location" );
			break;
		case HTTP2_INDEX_MAX_FORWARDS:
			HpackAddHeader( clsHpack, clsMessage, "Max-Forwards" );
			break;
		case HTTP2_INDEX_PROXY_AUTHENTICATE:
			HpackAddHeader( clsHpack, clsMessage, "ProxyAuthenticate" );
			break;
		case HTTP2_INDEX_PROXY_AUTHORIZATION:
			HpackAddHeader( clsHpack, clsMessage, "ProxyAuthorization" );
			break;
		case HTTP2_INDEX_RANGE:
			HpackAddHeader( clsHpack, clsMessage, "Range" );
			break;
		case HTTP2_INDEX_REFERER:
			HpackAddHeader( clsHpack, clsMessage, "Referer" );
			break;
		case HTTP2_INDEX_REFRESH:
			HpackAddHeader( clsHpack, clsMessage, "Refresh" );
			break;
		case HTTP2_INDEX_RETRY_AFTER:
			HpackAddHeader( clsHpack, clsMessage, "Retry-After" );
			break;
		case HTTP2_INDEX_SERVER:
			HpackAddHeader( clsHpack, clsMessage, "Server" );
			break;
		case HTTP2_INDEX_SET_COOKIE:
			HpackAddHeader( clsHpack, clsMessage, "Set-Cookie" );
			break;
		case HTTP2_INDEX_STRICT_TRANSPORT_SECURITY:
			HpackAddHeader( clsHpack, clsMessage, "Strict-Transport-Security" );
			break;
		case HTTP2_INDEX_TRANSFER_ENCODING:
			HpackAddHeader( clsHpack, clsMessage, "Transfer-Encoding" );
			break;
		case HTTP2_INDEX_USER_AGENT:
			HpackAddHeader( clsHpack, clsMessage, "UserAgent" );
			break;
		case HTTP2_INDEX_VARY:
			HpackAddHeader( clsHpack, clsMessage, "Vary" );
			break;
		case HTTP2_INDEX_VIA:
			HpackAddHeader( clsHpack, clsMessage, "Via" );
			break;
		case HTTP2_INDEX_WWW_AUTHENTICATE:
			HpackAddHeader( clsHpack, clsMessage, "WWW-Authenticate" );
			break;
		}

		bRes = true;
	}
	else
	{
		CHttp2HpackHeader * pclsHpack;

		if( m_clsHpackList.Select( clsHpack.m_iIndex, &pclsHpack ) == false )
		{
			CLog::Print( LOG_ERROR, "%s hpack index(%d) is not found in hpack list", __FUNCTION__, clsHpack.m_iIndex );
		}
		else
		{
			const char * pszName = pclsHpack->m_strName.c_str();

			if( !strcmp( pszName, HTTP2_METHOD_TYPE ) )
			{
				HpackToString( clsHpack, clsMessage.m_strHttpMethod, pclsHpack->m_strValue.c_str(), HTTP2_METHOD_TYPE );
			}
			else if( !strcmp( pszName, HTTP2_PATH_TYPE ) )
			{
				HpackToString( clsHpack, clsMessage.m_strReqUri, pclsHpack->m_strValue.c_str(), HTTP2_PATH_TYPE );
			}
			else if( !strcmp( pszName, HTTP2_STATUS_CODE_TYPE ) )
			{
				int iStatusCode = atoi( pclsHpack->m_strValue.c_str() );
				HpackToInt( clsHpack, clsMessage.m_iStatusCode, iStatusCode, HTTP2_STATUS_CODE_TYPE );
			}
			else if( !strcmp( pszName, HTTP2_CONTENT_LENGTH_TYPE ) )
			{
				int iContentLength = atoi( pclsHpack->m_strValue.c_str() );
				HpackToInt( clsHpack, clsMessage.m_iContentLength, iContentLength, HTTP2_CONTENT_LENGTH_TYPE );
			}
			else if( !strcmp( pszName, HTTP2_CONTENT_TYPE_TYPE ) )
			{
				HpackToString( clsHpack, clsMessage.m_strContentType, pclsHpack->m_strValue.c_str(), HTTP2_CONTENT_TYPE_TYPE );
			}
			else
			{
				if( clsHpack.m_strValue.empty() )
				{
					HpackAddHeader( *pclsHpack, clsMessage, pclsHpack->m_strName.c_str() );
				}
				else
				{
					HpackAddHeader( clsHpack, clsMessage, pclsHpack->m_strName.c_str() );
				}

				if( clsHpack.m_bIncrementalIndexing )
				{
					if( clsHpack.m_strName.empty() )
					{
						clsHpack.m_strName = pclsHpack->m_strName;
					}

					if( clsHpack.m_strValue.empty() )
					{
						clsHpack.m_strValue = pclsHpack->m_strValue;
					}
				}
			}
		}
	}

	if( clsHpack.m_bIncrementalIndexing )
	{
		m_clsHpackList.Insert( &clsHpack );
	}

	return bRes;
}

/**
 * @ingroup Http2Parser
 * @brief HPACK 헤더의 값을 문자열로 저장한다.
 * @param clsHpack		HPACK 헤더
 * @param strOutput		[out] 문자열 저장 변수
 * @param pszDefault	HPACK 헤더에 값이 존재하지 않는 경우, 기본값 문자열
 * @param pszType			IncrementalIndexing 인 경우, HPACK 이름으로 저장할 문자열
 */
void CHttp2Conversion::HpackToString( CHttp2HpackHeader & clsHpack, std::string & strOutput, const char * pszDefault, const char * pszType )
{
	if( clsHpack.m_strValue.empty() )
	{
		strOutput = pszDefault;
	}
	else
	{
		strOutput = clsHpack.m_strValue;
	}

	if( clsHpack.m_bIncrementalIndexing )
	{
		clsHpack.m_strName = pszType;
		clsHpack.m_strValue = strOutput;
	}
}

/**
 * @ingroup Http2Parser
 * @brief HPACK 헤더의 값을 숫자로 저장한다.
 * @param clsHpack	HPACK 헤더
 * @param iOutput		[out] 숫자 저장 변수
 * @param iDefault	HPACK 헤더에 숫자가 존재하지 않는 경우, 기본 숫자
 * @param pszType			IncrementalIndexing 인 경우, HPACK 이름으로 저장할 문자열
 */
void CHttp2Conversion::HpackToInt( CHttp2HpackHeader & clsHpack, int & iOutput, int iDefault, const char * pszType )
{
	if( clsHpack.m_strValue.empty() )
	{
		iOutput = iDefault;
	}
	else
	{
		iOutput = atoi( clsHpack.m_strValue.c_str() );
	}

	if( clsHpack.m_bIncrementalIndexing )
	{
		char szOutput[11];

		snprintf( szOutput, sizeof(szOutput), "%d", iOutput );

		clsHpack.m_strName = pszType;
		clsHpack.m_strValue = szOutput;
	}
}

/**
 * @ingroup Http2Parser
 * @brief HPACK 헤더를 HTTP/1.1 메시지에 저장한다.
 * @param clsHpack		HPACK 헤더
 * @param clsMessage	[out] HTTP/1.1 메시지
 * @param pszName			HTTP/1.1 메시지에 저장할 헤더 이름
 */
void CHttp2Conversion::HpackAddHeader( CHttp2HpackHeader & clsHpack, CHttpMessage & clsMessage, const char * pszName )
{
	if( clsHpack.m_strValue.empty() == false )
	{
		clsMessage.AddHeader( pszName, clsHpack.m_strValue );
		
		if( clsHpack.m_bIncrementalIndexing )
		{
			clsHpack.m_strName = pszName;
		}
	}
}
