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

#include "Http2HuffmanCode.h"
#include <stdio.h>

#include "Http2HuffmanCodeEncode.hpp"
#include "Http2HuffmanCodeDecode.hpp"

/**
 * @ingroup HttpParser
 * @brief Huffman code 인코딩 함수
 * @param pszInput		[in] 평문
 * @param iInputLen		[in] pszInput 변수의 길이
 * @param pszOutput		[out] Huffman code 인코딩된 데이터를 저장할 버퍼
 * @param iOutputSize [in] pszOutput 변수의 크기
 * @returns 성공하면 Huffman code 인코딩된 데이터의 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int HuffmanCodeEncode( const uint8_t * pszInput, uint32_t iInputLen, uint8_t * pszOutput, uint32_t iOutputSize )
{
	HuffmanEncode * psttCode;
	uint32_t	iOutput = 0;
	uint32_t	iBitPos = 0, iBitRemain, iOutputPos = 0;

	for( uint32_t i = 0; i < iInputLen; ++i )
	{
		psttCode = &garrHuffmanEncode[pszInput[i]];
		if( ( psttCode->iBit + iBitPos ) < 32 )
		{
			iOutput <<= psttCode->iBit;
			iOutput |= psttCode->iCode;
			iBitPos += psttCode->iBit;
			continue;
		}

		if( iOutputPos + 4 > iOutputSize )
		{
			return -1;
		}

		iBitRemain = 32 - iBitPos;
		iOutput <<= iBitRemain;
		iBitPos = psttCode->iBit - iBitRemain;
		iOutput |= psttCode->iCode >> iBitPos;

		pszOutput[iOutputPos++] = iOutput >> 24;
		pszOutput[iOutputPos++] = iOutput >> 16;
		pszOutput[iOutputPos++] = iOutput >> 8;
		pszOutput[iOutputPos++] = iOutput;

		iOutput = psttCode->iCode;
	}

	if( iBitPos )
	{
		iBitRemain = 32 - iBitPos;
		iOutput <<= iBitRemain;
		iOutput |= 0xFFFFFFFF >> iBitPos;

		pszOutput[iOutputPos++] = iOutput >> 24;
		if( iBitPos > 8 ) pszOutput[iOutputPos++] = iOutput >> 16;
		if( iBitPos > 16 ) pszOutput[iOutputPos++] = iOutput >> 8;
		if( iBitPos > 24 ) pszOutput[iOutputPos++] = iOutput;
	}

	return iOutputPos;
}

static void PrintDebug( const char * pszName, uint32_t iCode, int iBitPos, int iBitRemain, uint8_t cInput, int iInputBitRemain )
{
	printf( "%s code[", pszName );

	for( int i = 0; i < iBitPos; ++i )
	{
		printf( "%d", ( iCode >> ( 32 - i - 1 ) ) & 0x01 );
	}

	printf( "] pos[%d] remain[%d] input[", iBitPos, iBitRemain );

	for( int i = 0; i < 8; ++i )
	{
		printf( "%d", ( cInput >> ( 8 - i - 1 ) ) & 0x01 );
	}
	
	printf( "] input remain[%d]\n", iInputBitRemain );
}

/**
 * @ingroup HttpParser
 * @brief Huffman code 디코더 함수
 * @param pszInput		[in] Huffman code 로 인코딩된 데이터를 저장하는 버퍼
 * @param iInputLen		[in] pszInput 변수의 길이
 * @param pszOutput		[out] Huffman code 로 디코딩된 문자열을 저장할 버퍼
 * @param iOutputSize [in] pszOutput 변수의 크기
 * @returns 성공하면 Huffman code 로 디코딩된 문자열의 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int HuffmanCodeDecode( const uint8_t * pszInput, uint32_t iInputLen, uint8_t * pszOutput, uint32_t iOutputSize )
{
	int iInputPos = 0, iInputBitRemain = 0;
	int iOutputPos = 0;
	int iBitPos = 0, iBitRemain, iDecode, i;
	uint32_t iCode = 0, iCodeInput = 0, iTest;
	bool bFound;

	while( iInputPos < (int)iInputLen || iBitPos >= 5 )
	{
		if( iBitPos < 32 )
		{
			iBitRemain = 32 - iBitPos;

			if( iInputBitRemain )
			{
				// 입력 문자에서 남은 값만 변수에 저장한다.
				switch( iInputBitRemain )
				{
				case 1: iCodeInput = pszInput[iInputPos] & 0x01; break;
				case 2: iCodeInput = pszInput[iInputPos] & 0x03; break;
				case 3: iCodeInput = pszInput[iInputPos] & 0x07; break;
				case 4: iCodeInput = pszInput[iInputPos] & 0x0F; break;
				case 5: iCodeInput = pszInput[iInputPos] & 0x1F; break;
				case 6: iCodeInput = pszInput[iInputPos] & 0x3F; break;
				case 7: iCodeInput = pszInput[iInputPos] & 0x7F; break;
				}

				if( iBitRemain >= iInputBitRemain )
				{
					// 남은 값을 모두 저장한다.
					iCode |= iCodeInput << ( iBitRemain - iInputBitRemain );
					iBitPos += iInputBitRemain;
					iBitRemain -= iInputBitRemain;
					iInputBitRemain = 0;
					++iInputPos;
				}
				else
				{
					// 남은 값의 일부만 저장한다.
					iCode |= iCodeInput >> ( iInputBitRemain - iBitRemain );
					iBitPos += iBitRemain;
					iInputBitRemain -= iBitRemain;
					iBitRemain = 0;
				}
			}

			for( ; iBitRemain > 0 && iInputPos < (int)iInputLen; iBitRemain -= 8 )
			{
				if( iBitRemain >= 8 )
				{
					// 8bit 저장한다.
					iCode |= pszInput[iInputPos++] << ( iBitRemain - 8 );
					iBitPos += 8;
				}
				else
				{
					// 일부만 저장한다.
					iCode |= pszInput[iInputPos] >> ( 8 - iBitRemain );
					iBitPos += iBitRemain;
					iInputBitRemain = 8 - iBitRemain;
				}
			}
		}

		if( iBitPos < 5 ) 
		{
			break;
		}

		bFound = false;

		// 코드 테이블에서 검색한다.
		for( iDecode = 0; garrHuffmanDecodeInfo[iDecode].psttDecode; ++iDecode )
		{
			if( iBitPos < garrHuffmanDecodeInfo[iDecode].iBit ) break;

			iTest = iCode >> ( 32 - garrHuffmanDecodeInfo[iDecode].iBit );

			for( i = 0; i < garrHuffmanDecodeInfo[iDecode].iCount; ++i )
			{
				if( garrHuffmanDecodeInfo[iDecode].psttDecode[i].iCode == iTest )
				{
					pszOutput[iOutputPos++] = garrHuffmanDecodeInfo[iDecode].psttDecode[i].cChar;
					iCode <<= garrHuffmanDecodeInfo[iDecode].iBit;
					iBitPos -= garrHuffmanDecodeInfo[iDecode].iBit;
					bFound = true;
					break;
				}
			}

			if( bFound ) break;
		}

		if( bFound == false )
		{
			break;
		}

		if( iOutputPos >= (int)iOutputSize )
		{
			return -1;
		}
	}

	return iOutputPos;
}
