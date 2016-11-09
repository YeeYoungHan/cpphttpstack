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
#include "JsonType.h"
#include "MemoryDebug.h"
#include "MemoryDebug.h"

CJsonType::CJsonType()
{
}

CJsonType::~CJsonType()
{
}

const char * CJsonType::GetTypeString()
{
	switch( m_cType )
	{
	case JSON_TYPE_STRING: return "string";
	case JSON_TYPE_NUMBER: return "number";
	case JSON_TYPE_INT   : return "int";
	case JSON_TYPE_DOUBLE: return "double";
	case JSON_TYPE_OBJECT: return "object";
	case JSON_TYPE_ARRAY : return "array";
	case JSON_TYPE_BOOL  : return "bool";
	case JSON_TYPE_NULL  : return "null";
	}

	return "";
}
