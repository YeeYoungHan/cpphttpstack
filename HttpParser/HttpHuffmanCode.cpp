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

#include "HttpHuffmanCode.hpp"

int HuffmanCodeEncode( const uint8_t * pszInput, uint32_t iInputLen, uint8_t * pszOutput, uint32_t iOutputSize )
{
	HuffmanCode * psttCode;
	uint32_t	iOutput = 0;
	uint32_t	iBitPos = 0, iBitRemain, iOutputPos = 0;

	for( uint32_t i = 0; i < iInputLen; ++i )
	{
		psttCode = &garrHuffmanCode[pszInput[i]];
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
