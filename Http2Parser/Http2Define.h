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

#endif
