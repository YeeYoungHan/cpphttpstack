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
#include "JsonObject.h"
#include "Log.h"
#include "MemoryDebug.h"

CJsonArray::CJsonArray()
{
	m_cType = JSON_TYPE_ARRAY;
}

CJsonArray::~CJsonArray()
{
	Clear();
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭 ���ڿ� �Ľ��Ͽ��� �ڷᱸ���� �����Ѵ�.
 * @param pszText		JSON �迭 ���ڿ�
 * @param iTextLen	JSON �迭 ���ڿ� ����
 * @returns JSON �迭 ���ڿ� �Ľ̿� �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CJsonArray::Parse( const char * pszText, int iTextLen )
{
	int iPos = -1, iParseLen;
	uint8_t	cType = 0;
	std::string	strName;

	Clear();

	for( int i = 0; i < iTextLen; ++i )
	{
		if( pszText[i] == '[' )
		{
			iPos = i + 1;
			break;
		}
	}

	if( iPos == -1 ) 
	{
		CLog::Print( LOG_ERROR, "%s [ is not found", __FUNCTION__ );
		return -1;
	}

	for( int i = iPos; i < iTextLen; ++i )
	{
		if( cType == 0 )
		{
			CJsonType * pclsType = CJsonObject::GetJsonType( pszText, iTextLen, i );

			if( pclsType )
			{
				iParseLen = pclsType->Parse( pszText + i, iTextLen - i );
				if( iParseLen == -1 )
				{
					CLog::Print( LOG_ERROR, "%s json parse error", __FUNCTION__ );
					delete pclsType;
					return -1;
				}

				m_clsList.push_back( pclsType );
				cType = 1;
				i += iParseLen - 1;
			}
			else if( pszText[i] == ']' )
			{
				iPos = i + 1;
				break;
			}
		}
		else if( pszText[i] == ',' )
		{
			cType = 0;
		}
		else if( pszText[i] == ']' )
		{
			iPos = i + 1;
			break;
		}
	}

	return iPos;
}

/**
 * @ingroup JsonParser
 * @brief �ڷᱸ���� JSON �迭 ���ڿ��� ��ȯ�Ѵ�.
 * @param strText		JSON �迭 ���ڿ� ���� ����
 * @param eNewLine	new line �� �Է��ϴ� ����� ���� ������
 * @param	iDepth		���� �׸��� ����. �� ���� �׸��� 0 �̰� �� ���� �׸��� 1 �̴�.
 * @returns JSON �迭 ���ڿ� ���̸� �����Ѵ�.
 */
int CJsonArray::ToString( std::string & strText, EJsonNewLine eNewLine, int iDepth )
{
	JSON_LIST::iterator itJL;
	std::string strBuf;

	if( m_clsList.empty() )
	{
		strBuf.append( "[ ]" );
	}
	else
	{
		strBuf.append( "[" );

		if( eNewLine != E_JNL_NULL ) strBuf.append( m_strNewLine );

		for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
		{
			if( itJL == m_clsList.begin() )
			{
				if( eNewLine == E_JNL_NULL ) strBuf.append( " " );
			}
			else
			{
				if( eNewLine != E_JNL_NULL ) 
				{
					strBuf.append( "," );
					strBuf.append( m_strNewLine );
				}
				else
				{
					strBuf.append( ", " );
				}
			}

			if( eNewLine != E_JNL_NULL ) 
			{
				CJsonObject::AddTab( strBuf, iDepth + 1 );
			}

			CJsonObject::JsonToString( *itJL, strBuf, eNewLine == E_JNL_TOP ? E_JNL_NULL : eNewLine, iDepth + 1 );
		}

		if( eNewLine != E_JNL_NULL )
		{
			strBuf.append( m_strNewLine );
			CJsonObject::AddTab( strBuf, iDepth );
			strBuf.append( "]" );
		}
		else
		{
			strBuf.append( " ]" );
		}
	}

	strText.append( strBuf );

	return (int)strBuf.length();
}

/**
 * @ingroup JsonParser
 * @brief ToString �޼ҵ�� ������ ���ڿ� ���̸� �����Ѵ�.
 * @returns ToString �޼ҵ�� ������ ���ڿ� ���̸� �����Ѵ�.
 */
int CJsonArray::GetStringLen( )
{
	JSON_LIST::iterator itJL;
	int iLen = 3;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		if( itJL == m_clsList.begin() )
		{
			iLen += 1;
		}
		else
		{
			iLen += 2;
		}

		iLen += (*itJL)->GetStringLen();
	}

	return iLen;
}

/**
 * @ingroup JsonParser
 * @brief �ڽ��� ������ ��ü�� �����Ѵ�.
 * @returns �����ϸ� �ڽ��� ������ ��ü�� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CJsonType * CJsonArray::Copy( )
{
	CJsonArray * pclsArray = new CJsonArray();
	if( pclsArray == NULL ) return NULL;

	JSON_LIST::iterator itJL;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		CJsonType * pclsType = (*itJL)->Copy();
		if( pclsType == NULL )
		{
			delete pclsArray;
			return NULL;
		}

		pclsArray->m_clsList.push_back( pclsType );
	}

	return pclsArray;
}

/**
 * @ingroup JsonParser
 * @brief �ڷᱸ���� JSON array ���ڿ��� ��ȯ�Ѵ�. �� �޼ҵ�� �Էµ� strText �� �ʱ�ȭ��Ų ��, ToString �޼ҵ带 ȣ���Ѵ�.
 * @param strText		JSON array ���ڿ� ���� ����
 * @param eNewLine	new line �� �Է��ϴ� ����� ���� ������
 * @returns JSON object ���ڿ� ���̸� �����Ѵ�.
 */
int CJsonArray::MakeString( std::string & strText, EJsonNewLine eNewLine )
{
	strText.clear();

	return ToString( strText, eNewLine );
}

/**
 * @ingroup JsonParser
 * @brief �ڷᱸ���� �ʱ�ȭ��Ų��.
 */
void CJsonArray::Clear()
{
	JSON_LIST::iterator itJL;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		delete *itJL;
	}

	m_clsList.clear();
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� ���ڿ� ���� �˻��Ѵ�.
 * @param iIndex		Element �ε���
 * @param strValue	Element ��
 * @returns �˻��� �����ϰ� �ش� ���� ���ڿ� Ÿ���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, std::string & strValue )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_STRING )
	{
		if( pclsType->m_cType != JSON_TYPE_NULL )
		{
			CLog::Print( LOG_ERROR, "%s type(%d) is not string (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		}

		return false;
	}

	strValue = ((CJsonString *)pclsType)->m_strValue;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� ���� ���� �˻��Ѵ�.
 * @param iIndex Element �ε���
 * @param iValue Element ��
 * @returns �˻��� �����ϰ� �ش� ���� ���� Ÿ���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, int64_t & iValue )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_INT )
	{
		if( pclsType->m_cType != JSON_TYPE_NULL )
		{
			CLog::Print( LOG_ERROR, "%s type(%d) is not int (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		}

		return false;
	}

	iValue = ((CJsonInt *)pclsType)->m_iValue;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� boolean ���� �˻��Ѵ�.
 * @param iIndex Element �ε���
 * @param bValue Element ��
 * @returns �˻��� �����ϰ� �ش� ���� boolean Ÿ���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, bool & bValue )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_BOOL )
	{
		if( pclsType->m_cType != JSON_TYPE_NULL )
		{
			CLog::Print( LOG_ERROR, "%s type(%d) is not bool (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		}

		return false;
	}

	bValue = ((CJsonBool *)pclsType)->m_bValue;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� object ���� �˻��Ѵ�.
 * @param iIndex			Element �ε���
 * @param ppclsObject Element ��
 * @returns �˻��� �����ϰ� �ش� ���� object Ÿ���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, CJsonObject ** ppclsObject )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_OBJECT )
	{
		if( pclsType->m_cType != JSON_TYPE_NULL )
		{
			CLog::Print( LOG_ERROR, "%s type(%d) is not object (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		}

		return false;
	}

	*ppclsObject = (CJsonObject *)pclsType;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� �迭 ���� �˻��Ѵ�.
 * @param iIndex			Element �ε���
 * @param ppclsArray	Element ��
 * @returns �˻��� �����ϰ� �ش� ���� �迭 Ÿ���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, CJsonArray ** ppclsArray )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_ARRAY )
	{
		if( pclsType->m_cType != JSON_TYPE_NULL )
		{
			CLog::Print( LOG_ERROR, "%s type(%d) is not array (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		}

		return false;
	}

	*ppclsArray = (CJsonArray *)pclsType;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� ���� �˻��Ѵ�.
 * @param iIndex		Element �ε���
 * @param ppclsType Element ��
 * @returns �˻��� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, CJsonType ** ppclsType )
{
	if( iIndex >= (int)m_clsList.size() ) 
	{
		CLog::Print( LOG_ERROR, "%s iIndex(%d) >= m_clsList.size(%d)", __FUNCTION__, iIndex, m_clsList.size() );
		return false;
	}

	JSON_LIST::iterator itJL;
	int iCount = 0;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		if( iIndex == iCount )
		{
			*ppclsType = *itJL;
			return true;
		}

		++iCount;
	}

	return false;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ���ڿ� Element ���� �߰��Ѵ�.
 * @param strValue Element ��
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( std::string & strValue )
{
	return InsertData( strValue.c_str() );
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ���ڿ� Element ���� �߰��Ѵ�.
 * @param pszValue Element ��
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( const char * pszValue )
{
	CJsonString * pclsNew = new CJsonString();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_strValue = pszValue;
	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ���� Element ���� �߰��Ѵ�.
 * @param iValue Element ��
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( int32_t iValue )
{
	return InsertData( (int64_t)iValue );
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ���� Element ���� �߰��Ѵ�.
 * @param iValue Element ��
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( int64_t iValue )
{
	CJsonInt * pclsNew = new CJsonInt();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_iValue = iValue;
	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� boolean Element ���� �߰��Ѵ�.
 * @param bValue Element ��
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( bool bValue )
{
	CJsonBool * pclsNew = new CJsonBool();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_bValue = bValue;
	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� Element ���� �߰��Ѵ�.
 * @param pclsType Element ��
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( CJsonType * pclsType )
{
	CJsonType * pclsNew = pclsType->Copy();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� null Element ���� �߰��Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( )
{
	CJsonNull * pclsNew = new CJsonNull();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ������ ��ġ�� ���ڿ� Element �� �߰��Ѵ�. ������ ��ġ���� element �� ��ĭ�� �ڷ� �и���.
 * @param iIndex		���ڿ� Element �� ������ �ε���
 * @param strValue	���ڿ�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( int iIndex, std::string & strValue )
{
	return InsertData( iIndex, strValue.c_str() );
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ������ ��ġ�� ���ڿ� Element �� �߰��Ѵ�. ������ ��ġ���� element �� ��ĭ�� �ڷ� �и���.
 * @param iIndex		���ڿ� Element �� ������ �ε���
 * @param pszValue	���ڿ�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( int iIndex, const char * pszValue )
{
	CJsonString * pclsNew = new CJsonString();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_strValue = pszValue;

	if( InsertDataNoCopy( iIndex, pclsNew ) == false )
	{
		delete pclsNew;
		return false;
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ������ ��ġ�� ���� Element �� �߰��Ѵ�. ������ ��ġ���� element �� ��ĭ�� �ڷ� �и���.
 * @param iIndex		���� Element �� ������ �ε���
 * @param iValue		����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( int iIndex, int32_t iValue )
{
	return InsertData( iIndex, (int64_t)iValue );
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ������ ��ġ�� ���� Element �� �߰��Ѵ�. ������ ��ġ���� element �� ��ĭ�� �ڷ� �и���.
 * @param iIndex		���� Element �� ������ �ε���
 * @param iValue		����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( int iIndex, int64_t iValue )
{
	CJsonInt * pclsNew = new CJsonInt();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_iValue = iValue;

	if( InsertDataNoCopy( iIndex, pclsNew ) == false )
	{
		delete pclsNew;
		return false;
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ������ ��ġ�� bool Element �� �߰��Ѵ�. ������ ��ġ���� element �� ��ĭ�� �ڷ� �и���.
 * @param iIndex		bool Element �� ������ �ε���
 * @param bValue		bool
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( int iIndex, bool bValue )
{
	CJsonBool * pclsNew = new CJsonBool();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_bValue = bValue;

	if( InsertDataNoCopy( iIndex, pclsNew ) == false )
	{
		delete pclsNew;
		return false;
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ������ ��ġ�� Element �� �߰��Ѵ�. ������ ��ġ���� element �� ��ĭ�� �ڷ� �и���.
 * @param iIndex		Element �� ������ �ε���
 * @param pclsType	Element
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( int iIndex, CJsonType * pclsType )
{
	CJsonType * pclsNew = pclsType->Copy();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	if( InsertDataNoCopy( iIndex, pclsNew ) == false )
	{
		delete pclsNew;
		return false;
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ������ ��ġ�� null Element �� �߰��Ѵ�. ������ ��ġ���� element �� ��ĭ�� �ڷ� �и���.
 * @param iIndex		Element �� ������ �ε���
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CJsonArray::InsertDataNull( int iIndex )
{
	CJsonNull * pclsNew = new CJsonNull();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	if( InsertDataNoCopy( iIndex, pclsNew ) == false )
	{
		delete pclsNew;
		return false;
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �Է��� JSON �迭�� element ���� �߰��Ѵ�.
 * @param pclsArray JSON �迭
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CJsonArray::InsertArray( CJsonArray * pclsArray )
{
	int iCount = pclsArray->GetCount();
	CJsonType * pclsData;
		
	for( int i = 0; i < iCount; ++i )
	{
		if( pclsArray->SelectData( i, &pclsData ) )
		{
			InsertData( pclsData );
		}
	}

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� ���� �����Ѵ�.
 * @param iIndex Element �ε���
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::DeleteData( int iIndex )
{
	if( iIndex >= (int)m_clsList.size() ) 
	{
		CLog::Print( LOG_ERROR, "%s iIndex(%d) >= m_clsList.size(%d)", __FUNCTION__, iIndex, m_clsList.size() );
		return false;
	}

	JSON_LIST::iterator itJL;
	int iCount = 0;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		if( iIndex == iCount )
		{
			delete *itJL;
			m_clsList.erase( itJL );
			return true;
		}

		++iCount;
	}

	return false;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� Element ���� �߰��Ѵ�.
 * @returns JSON �迭�� Element ������ �����Ѵ�.
 */
int CJsonArray::GetCount()
{
	return (int)m_clsList.size();
}

/**
 * @ingroup JsonParser
 * @brief �Էµ� ���� copy ���� �ʰ� JSON �迭�� ������ ��ġ�� Element �� �߰��Ѵ�. ������ ��ġ���� element �� ��ĭ�� �ڷ� �и���.
 * @param iIndex		Element �� ������ �ε���
 * @param pszValue	Element
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CJsonArray::InsertDataNoCopy( int iIndex, CJsonType * pclsType )
{
	int iSize = (int)m_clsList.size();

	if( iIndex > iSize )
	{
		CLog::Print( LOG_ERROR, "%s iIndex(%d) > m_clsList.size(%d)", __FUNCTION__, iIndex, iSize );
		return false;
	}

	if( iIndex == iSize )
	{
		m_clsList.push_back( pclsType );
		return true;
	}

	JSON_LIST::iterator itJL;
	int iCount = 0;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		if( iIndex == iCount )
		{
			m_clsList.insert( itJL, pclsType );
			return true;
		}

		++iCount;
	}

	return false;
}
