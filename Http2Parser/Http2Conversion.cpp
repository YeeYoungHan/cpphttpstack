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

CHttp2Conversion::CHttp2Conversion() : m_pclsMessage(NULL), m_pclsFrameList(NULL), m_iHeaderFrameCount(0)
{
}

CHttp2Conversion::~CHttp2Conversion()
{
}

bool CHttp2Conversion::MakeFrameList( CHttpMessage & clsMessage, CHttp2FrameList & clsFrameList )
{
	CHttp2Frame * pclsFrame;

	m_pclsMessage = &clsMessage;
	m_pclsFrameList = &clsFrameList;

	clsFrameList.Clear();
	m_clsHeader.Clear();

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
	}

	//if( clsMessage.m_strContentType.empty() == false )
	//{
	//	AddIndexValue( HTTP2_INDEX_CONTENT_TYPE, clsMessage.m_strContentType.c_str() );
	//}

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
		int iBodyFrameCount = 0;

		while( iBodyLen > 0 )
		{
			pclsFrame = clsFrameList.CreateFrame();
			if( pclsFrame == NULL )
			{
				return false;
			}

			++iBodyFrameCount;

			cType = HTTP2_FRAME_TYPE_DATA;

			if( iBodyFrameCount >= 2 )
			{
				cType = HTTP2_FRAME_TYPE_CONTINUATION;
			}

			if( iBodyLen > HTTP2_FRAME_SIZE )
			{
				if( pclsFrame->Set( cType, 0, m_pclsMessage->m_iStreamIdentifier, (uint8_t *)pszBody, HTTP2_FRAME_SIZE ) == false ) return false;
				pszBody += HTTP2_FRAME_SIZE;
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

bool CHttp2Conversion::MakeMessage( CHttp2Frame & clsFrame, CHttpMessage & clsMessage )
{
	if( clsFrame.GetType() == HTTP2_FRAME_TYPE_HEADERS )
	{
		//uint8_t * pszBody = clsFrame.GetBody() + 9;
		//int iBodyLen = clsFrame.GetBodyLen();

		//for( int i = 0; i < iBodyLen; )
		//{
		//	if( pszBody[i] & 0x80 )
		//	{
		//		if( pszBody[i] != 0xFF )
		//		{
		//			uint8_t cIndex = pszBody[i] &0x7F;
		//		}
		//	}
		//}
	}
	else
	{
		clsMessage.m_strBody.append( (char *)clsFrame.GetBody(), clsFrame.GetBodyLen() );
		clsMessage.m_iContentLength += clsMessage.m_strBody.length();
	}

	return true;
}

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
