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

#ifndef _HTTP2_DEFINE_H_
#define _HTTP2_DEFINE_H_

#define HTTP2_FRAME_SIZE								16384
#define HTTP2_HEADER_SIZE								HTTP2_FRAME_SIZE
#define HTTP2_HEADER_TABLE_SIZE					4096

// HTTP frame type
#define HTTP2_FRAME_TYPE_DATA						0x00
#define HTTP2_FRAME_TYPE_HEADERS				0x01
#define HTTP2_FRAME_TYPE_PRIORITY				0x02
#define HTTP2_FRAME_TYPE_RST_STREAM			0x03
#define HTTP2_FRAME_TYPE_SETTINGS				0x04
#define HTTP2_FRAME_TYPE_PUSH_PROMISE		0x05
#define HTTP2_FRAME_TYPE_PING						0x06
#define HTTP2_FRAME_TYPE_GOAWAY					0x07
#define HTTP2_FRAME_TYPE_WINDOW_UPDATE	0x08
#define HTTP2_FRAME_TYPE_CONTINUATION		0x09

// HTTP frame flag
#define HTTP2_FLAG_END_STREAM						0x01
#define HTTP2_FLAG_ACK									0x01
#define HTTP2_FLAG_END_HEADER						0x04
#define HTTP2_FLAG_PADDED								0x08
#define HTTP2_FLAG_PRIORITY							0x20

// SETTINGS
#define HTTP2_SETTINGS_HEADER_TABLE_SIZE				0x01
#define HTTP2_SETTINGS_ENABLE_PUSH							0x02
#define HTTP2_SETTINGS_MAX_CONCURRENT_STREAMS		0x03
#define HTTP2_SETTINGS_INITIAL_WINDOW_SIZE			0x04
#define HTTP2_SETTINGS_MAX_FRAME_SIZE						0x05
#define HTTP2_SETTINGS_MAX_HEADER_LIST_SIZE			0x06
#define HTTP2_SETTINGS_ACCEPT_CACHE_DIGEST			0x07
#define HTTP2_SETTINGS_ENABLE_CONNECT_PROTOCOL	0x08

// HPACK index
#define HTTP2_INDEX_AUTHORITY						1
#define HTTP2_INDEX_METHOD_GET					2
#define HTTP2_INDEX_METHOD_POST					3
#define HTTP2_INDEX_PATH								4
#define HTTP2_INDEX_PATH_INDEX_HTML			5
#define HTTP2_INDEX_SCHEME_HTTP					6
#define HTTP2_INDEX_SCHEME_HTTPS				7
#define HTTP2_INDEX_STATUS_200					8
#define HTTP2_INDEX_STATUS_204					9
#define HTTP2_INDEX_STATUS_206					10
#define HTTP2_INDEX_STATUS_304					11
#define HTTP2_INDEX_STATUS_400					12
#define HTTP2_INDEX_STATUS_404					13
#define HTTP2_INDEX_STATUS_500					14
#define HTTP2_INDEX_ACCEPT_CHARSET			15
#define HTTP2_INDEX_ACCEPT_ENCODING			16
#define HTTP2_INDEX_ACCEPT_LANGUAGE			17
#define HTTP2_INDEX_ACCEPT_RANGES				18
#define HTTP2_INDEX_ACCEPT							19
#define HTTP2_INDEX_ACCESS_CONTROL_ALLOW_ORIGIN	20
#define HTTP2_INDEX_AGE									21
#define HTTP2_INDEX_ALLOW								22
#define HTTP2_INDEX_AUTHORIZATION				23
#define HTTP2_INDEX_CACHE_CONTROL				24
#define HTTP2_INDEX_CONTENT_DISPOSITION	25
#define HTTP2_INDEX_CONTENT_ENCODING		26
#define HTTP2_INDEX_CONTENT_LANGUAGE		27
#define HTTP2_INDEX_CONTENT_LENGTH			28
#define HTTP2_INDEX_CONTENT_LOCATION		29
#define HTTP2_INDEX_CONTENT_RANGE				30
#define HTTP2_INDEX_CONTENT_TYPE				31
#define HTTP2_INDEX_COOKIE							32
#define HTTP2_INDEX_DATE								33
#define HTTP2_INDEX_ETAG								34
#define HTTP2_INDEX_EXPECT							35
#define HTTP2_INDEX_EXPIRES							36
#define HTTP2_INDEX_FROM								37
#define HTTP2_INDEX_HOST								38
#define HTTP2_INDEX_IF_MATCH						39
#define HTTP2_INDEX_IF_MODIFIED_SINCE		40
#define HTTP2_INDEX_IF_NONE_MATCH				41
#define HTTP2_INDEX_IF_RANGE						42
#define HTTP2_INDEX_IF_UNMODIFIED_SINCE	43
#define HTTP2_INDEX_LAST_MODIFIED				44
#define HTTP2_INDEX_LINK								45
#define HTTP2_INDEX_LOCATION						46
#define HTTP2_INDEX_MAX_FORWARDS				47
#define HTTP2_INDEX_PROXY_AUTHENTICATE	48
#define HTTP2_INDEX_PROXY_AUTHORIZATION	49
#define HTTP2_INDEX_RANGE								50
#define HTTP2_INDEX_REFERER							51
#define HTTP2_INDEX_REFRESH							52
#define HTTP2_INDEX_RETRY_AFTER					53
#define HTTP2_INDEX_SERVER							54
#define HTTP2_INDEX_SET_COOKIE					55
#define HTTP2_INDEX_STRICT_TRANSPORT_SECURITY	56
#define HTTP2_INDEX_TRANSFER_ENCODING		57
#define HTTP2_INDEX_USER_AGENT					58
#define HTTP2_INDEX_VARY								59
#define HTTP2_INDEX_VIA									60
#define HTTP2_INDEX_WWW_AUTHENTICATE		61

// HPACK 이름에 저장할 문자열
#define HTTP2_METHOD_TYPE					"Method"
#define HTTP2_PATH_TYPE						"Path"
#define HTTP2_STATUS_CODE_TYPE		"StatusCode"
#define HTTP2_CONTENT_LENGTH_TYPE	"Content-Length"
#define HTTP2_CONTENT_TYPE_TYPE		"Content-Type"

#endif
