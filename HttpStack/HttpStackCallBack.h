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

#ifndef _HTTP_STACK_CALLBACK_H_
#define _HTTP_STACK_CALLBACK_H_

#include "HttpMessage.h"

/**
 * @ingroup HttpStack
 * @brief HTTP ���� callback �������̽�
 */
class IHttpStackCallBack
{
public:
	IHttpStackCallBack(){};
	virtual ~IHttpStackCallBack(){};

	/**
	 * @ingroup HttpStack
	 * @brief HTTP ��û ���� �̺�Ʈ callback
	 * @param pclsRequest		HTTP ��û �޽���
	 * @param pclsResponse	HTTP ���� �޽��� - ���뿡�� �����Ѵ�.
	 * @returns ���뿡�� HTTP ���� �޽����� ���������� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
	 */
	virtual bool RecvHttpRequest( CHttpMessage * pclsRequest, CHttpMessage * pclsResponse ) = 0;

	/**
	 * @ingroup HttpStack
	 * @brief WebSocket Ŭ���̾�Ʈ TCP ���� ���� �̺�Ʈ callback
	 * @param pszClientIp WebSocket Ŭ���̾�Ʈ IP �ּ�
	 * @param iClientPort WebSocket Ŭ���̾�Ʈ ��Ʈ ��ȣ
	 * @param pclsRequest	HTTP ��û �޽���
	 */
	virtual void WebSocketConnected( const char * pszClientIp, int iClientPort, CHttpMessage * pclsRequest ) = 0;

	/**
	 * @ingroup HttpStack
	 * @brief WebSocket Ŭ���̾�Ʈ TCP ���� ���� �̺�Ʈ callback
	 * @param pszClientIp WebSocket Ŭ���̾�Ʈ IP �ּ�
	 * @param iClientPort WebSocket Ŭ���̾�Ʈ ��Ʈ ��ȣ
	 */
	virtual void WebSocketClosed( const char * pszClientIp, int iClientPort ) = 0;

	/**
	 * @ingroup HttpStack
	 * @brief WebSocket Ŭ���̾�Ʈ ������ ���� �̺�Ʈ callback
	 * @param pszClientIp WebSocket Ŭ���̾�Ʈ IP �ּ�
	 * @param iClientPort WebSocket Ŭ���̾�Ʈ ��Ʈ ��ȣ
	 * @param strData			WebSocket Ŭ���̾�Ʈ�� ������ ������
	 * @returns WebSocket Ŭ���̾�Ʈ ������ �����Ϸ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
	 */
	virtual bool WebSocketData( const char * pszClientIp, int iClientPort, std::string & strData ) = 0;
};

#endif
