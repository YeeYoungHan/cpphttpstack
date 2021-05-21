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

#include "HttpHuffmanCode.h"

#include "HttpHuffmanCodeEncode.hpp"
#include "HttpHuffmanCodeDecode.hpp"

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

#define HUFFMAN_SEARCH( arrBuf, iMax, iTest, iBit ) for( i = 0; i < iMax; ++i ) \
			{ \
				if( arrBuf[i].iCode == iTest ) \
				{ \
					pszOutput[iOutputPos++] = arrBuf[i].cChar; \
					iCode <<= iBit; \
					iBitPos -= iBit; \
					bFound = true; \
					break; \
				} \
			}

int HuffmanCodeDecode( const uint8_t * pszInput, uint32_t iInputLen, uint8_t * pszOutput, uint32_t iOutputSize )
{
	uint32_t iInputPos = 0, iInputBitPos = 0;
	uint32_t iOutputPos = 0;
	uint32_t iCode = 0, iBitPos = 0, iBitRemain, iTest;
	uint32_t iDecode, i;
	bool bFound;

	while( iInputPos < iInputLen || iBitPos >= 5 )
	{
		if( iBitPos < 32 )
		{
			iBitRemain = 32 - iBitPos;

			if( iInputBitPos )
			{
				if( iBitRemain >= iInputBitPos )
				{
					iCode <<= iInputBitPos;
					iCode |= pszInput[iInputPos++] >> ( 8 - iInputBitPos );
					iBitPos += iInputBitPos;
					iBitRemain -= iInputBitPos;
					iInputBitPos = 0;
				}
				else
				{
					iCode <<= iBitRemain;
					iCode |= pszInput[iInputPos++] >> ( 8 - iBitRemain );
					iBitPos += iBitRemain;
					iInputBitPos += iBitRemain;
					iBitRemain = 0;
				}
			}

			for( ; iBitRemain > 0 && iInputPos < iInputLen; iBitRemain -= 8 )
			{
				if( iBitRemain >= 8 )
				{
					iCode <<= 8;
					iCode |= pszInput[iInputPos++];
					iBitPos += 8;
				}
				else
				{
					iCode <<= iBitRemain;
					iCode |= pszInput[iInputPos++] >> iBitPos;
					iBitPos += iBitRemain;
					iInputBitPos = iBitRemain;
				}
			}
		}

		if( iBitPos < 5 ) break;

		bFound = false;

		for( iDecode = 0; garrHuffmanDecodeInfo[iDecode].psttDecode; ++iDecode )
		{
			if( iBitPos < garrHuffmanDecodeInfo[iDecode].iBit ) break;

			iTest = iCode >> ( iBitPos - garrHuffmanDecodeInfo[iDecode].iBit );

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

		if( bFound == false ) break;
	}

	return iOutputPos;
}
