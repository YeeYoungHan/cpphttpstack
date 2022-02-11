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
#include "Http2HuffmanCode.h"
#include "StringUtility.h"
#include "MemoryDebug.h"

bool _TestHuffmanCodeEncode( const char * pszInput, const char * pszOutputHex )
{
	uint8_t szOutput[1000], szResult[1000];
	int n;
	std::string strHex;
	
	n = HuffmanCodeEncode( (uint8_t *)pszInput, (uint32_t)strlen(pszInput), szOutput, (uint32_t)sizeof(szOutput) );
	if( n == -1 )
	{
		printf( "HuffmanCodeEncode(%s) error\n", pszInput );
		return false;
	}
	
	StringToHex( (char *)szOutput, n, strHex );

	if( strcmp( pszOutputHex, strHex.c_str() ) )
	{
		printf( "HuffmanCodeEncode(%s) output [%s] != want [%s]\n", pszInput, strHex.c_str(), pszOutputHex );
		return false;
	}

	memset( szResult, 0, sizeof(szResult) );
	n = HuffmanCodeDecode( szOutput, n, szResult, sizeof(szResult) );
	if( n == -1 )
	{
		printf( "HuffmanCodeDecode(%s) error\n", pszInput );
		return false;
	}

	if( strcmp( pszInput, (char *)szResult ) )
	{
		printf( "HuffmanCodeDecode output [%s] != want [%s]\n", szResult, pszInput );
		return false;
	}

	return true;
}

bool TestHuffmanCodeEncode( )
{
	if( _TestHuffmanCodeEncode( "www.example.com", "f1e3c2e5f23a6ba0ab90f4ff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "a", "1f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ab", "1c7f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abc", "1c64" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcd", "1c6493" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcde", "1c6490bf" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdef", "1c6490b2ff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefg", "1c6490b2cd" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefgh", "1c6490b2cd3f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghi", "1c6490b2cd39bf" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghij", "1c6490b2cd39ba7f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijk", "1c6490b2cd39ba75" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijkl", "1c6490b2cd39ba75a3" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklm", "1c6490b2cd39ba75a29f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmn", "1c6490b2cd39ba75a29abf" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmno", "1c6490b2cd39ba75a29a8f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmnop", "1c6490b2cd39ba75a29a8f5f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmnopq", "1c6490b2cd39ba75a29a8f5f6f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmnopqr", "1c6490b2cd39ba75a29a8f5f6b3f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmnopqrs", "1c6490b2cd39ba75a29a8f5f6b11" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmnopqrst", "1c6490b2cd39ba75a29a8f5f6b109f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmnopqrstu", "1c6490b2cd39ba75a29a8f5f6b109b7f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmnopqrstuv", "1c6490b2cd39ba75a29a8f5f6b109b7bff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmnopqrstuvw", "1c6490b2cd39ba75a29a8f5f6b109b7bf8" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmnopqrstuvwx", "1c6490b2cd39ba75a29a8f5f6b109b7bf8f3" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmnopqrstuvwxy", "1c6490b2cd39ba75a29a8f5f6b109b7bf8f3eb" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "abcdefghijklmnopqrstuvwxyz", "1c6490b2cd39ba75a29a8f5f6b109b7bf8f3ebdf" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "A", "87" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "AB", "86ef" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABC", "86edef" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCD", "86edebff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDE", "86edebf83f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEF", "86edebf830ff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFG", "86edebf830e2" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGH", "86edebf830e2c7" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHI", "86edebf830e2c793" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJ", "86edebf830e2c7932f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJK", "86edebf830e2c7932e6f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKL", "86edebf830e2c7932e6cff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLM", "86edebf830e2c7932e6cfa3f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMN", "86edebf830e2c7932e6cfa34ff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNO", "86edebf830e2c7932e6cfa34ea" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNOP", "86edebf830e2c7932e6cfa34ead7" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNOPQ", "86edebf830e2c7932e6cfa34ead7b3" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNOPQR", "86edebf830e2c7932e6cfa34ead7b36f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNOPQRS", "86edebf830e2c7932e6cfa34ead7b36eef" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNOPQRST", "86edebf830e2c7932e6cfa34ead7b36eedff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNOPQRSTU", "86edebf830e2c7932e6cfa34ead7b36eedfc3f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNOPQRSTUV", "86edebf830e2c7932e6cfa34ead7b36eedfc38ff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNOPQRSTUVW", "86edebf830e2c7932e6cfa34ead7b36eedfc38f2" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNOPQRSTUVWX", "86edebf830e2c7932e6cfa34ead7b36eedfc38f2fc" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNOPQRSTUVWXY", "86edebf830e2c7932e6cfa34ead7b36eedfc38f2fce7" ) == false ) return false;
	if( _TestHuffmanCodeEncode( "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "86edebf830e2c7932e6cfa34ead7b36eedfc38f2fce7fb" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " ", "53" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !", "53f8" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"", "53f8fe7f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#", "53f8fe7feb" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$", "53f8fe7febff3f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%", "53f8fe7febff2aff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&", "53f8fe7febff2afc7f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'", "53f8fe7febff2afc7faf" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'(", "53f8fe7febff2afc7fafeb" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()", "53f8fe7febff2afc7fafebfb" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*", "53f8fe7febff2afc7fafebfbf9" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+", "53f8fe7febff2afc7fafebfbf9ff7f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,", "53f8fe7febff2afc7fafebfbf9ff7f5f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-", "53f8fe7febff2afc7fafebfbf9ff7f4b7f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-.", "53f8fe7febff2afc7fafebfbf9ff7f4b2f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec7" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec03f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./01", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec003" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./012", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec0022f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002267f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./01234", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./012345", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dcf" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./01234567", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc77f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./012345678", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7df" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffcf" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83fef" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffc" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?@", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffcffd7" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffcffd4376f5fc187163c997367d1a756bd9b776fe1c797e73fdffdff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffcffd4376f5fc187163c997367d1a756bd9b776fe1c797e73fdffdffff0f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffcffd4376f5fc187163c997367d1a756bd9b776fe1c797e73fdffdffff0ffe7f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffcffd4376f5fc187163c997367d1a756bd9b776fe1c797e73fdffdffff0ffe7ff9" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffcffd4376f5fc187163c997367d1a756bd9b776fe1c797e73fdffdffff0ffe7ff917" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffcffd4376f5fc187163c997367d1a756bd9b776fe1c797e73fdffdffff0ffe7ff917ffdf" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffcffd4376f5fc187163c997367d1a756bd9b776fe1c797e73fdffdffff0ffe7ff917ffd1c6490b2cd39ba75a29a8f5f6b109b7bf8f3ebdfffe" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffcffd4376f5fc187163c997367d1a756bd9b776fe1c797e73fdffdffff0ffe7ff917ffd1c6490b2cd39ba75a29a8f5f6b109b7bf8f3ebdfffeff9f" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffcffd4376f5fc187163c997367d1a756bd9b776fe1c797e73fdffdffff0ffe7ff917ffd1c6490b2cd39ba75a29a8f5f6b109b7bf8f3ebdfffeff9ffeff" ) == false ) return false;
	if( _TestHuffmanCodeEncode( " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", "53f8fe7febff2afc7fafebfbf9ff7f4b2ec002265a6dc75e7ee7dfffc83feffcffd4376f5fc187163c997367d1a756bd9b776fe1c797e73fdffdffff0ffe7ff917ffd1c6490b2cd39ba75a29a8f5f6b109b7bf8f3ebdfffeff9ffefff7" ) == false ) return false;

	return true;
}

bool _TestHuffmanCodeDecode( const char * pszInputHex, const char * pszOutput )
{
	std::string strInput;
	uint8_t	szOutput[4000];

	memset( szOutput, 0, sizeof(szOutput) );

	HexToString( pszInputHex, strInput );

	int n = HuffmanCodeDecode( (uint8_t *)strInput.c_str(), (uint32_t)strInput.length(), szOutput, (uint32_t)sizeof(szOutput) );
	if( n == -1 )
	{
		printf( "HuffmanCodeDecode(%s) error\n", pszInputHex );
		return false;
	}

	if( strcmp( (char *)szOutput, pszOutput ) )
	{
		printf( "HuffmanCodeDecode(%s) output [%s] != want [%s]\n", pszInputHex, szOutput, pszOutput );
		return false;
	}

	return true;
};

bool _TestHuffmanCodeEncodeDecode( const char * pszInput )
{
	uint8_t szOutput[1000], szResult[1000];
	int n;
	
	n = HuffmanCodeEncode( (uint8_t *)pszInput, (uint32_t)strlen(pszInput), szOutput, (uint32_t)sizeof(szOutput) );
	if( n == -1 )
	{
		printf( "HuffmanCodeEncode(%s) error\n", pszInput );
		return false;
	}
	
	memset( szResult, 0, sizeof(szResult) );
	n = HuffmanCodeDecode( szOutput, n, szResult, sizeof(szResult) );
	if( n == -1 )
	{
		printf( "HuffmanCodeDecode(%s) error\n", pszInput );
		return false;
	}

	if( strcmp( pszInput, (char *)szResult ) )
	{
		printf( "HuffmanCodeDecode output [%s] != want [%s]\n", szResult, pszInput );
		return false;
	}

	return true;
}

bool TestHuffmanCodeDecode( )
{
	if( _TestHuffmanCodeDecode( "27", "c" ) == false ) return false;

	char szInput[1000];

	for( uint8_t c = 1; c < 255; ++c )
	{
		sprintf( szInput, "%c", c );
		if( _TestHuffmanCodeEncodeDecode( szInput ) == false ) return false;
	}

	memset( szInput, 0, sizeof(szInput) );

	for( uint8_t c = 1; c < 255; ++c )
	{
		sprintf( szInput + c - 1, "%c", c );
		if( _TestHuffmanCodeEncodeDecode( szInput ) == false ) return false;
	}

	return true;
}

bool TestHttp2HuffmanCode( )
{
	if( TestHuffmanCodeDecode() == false ) return false;
	if( TestHuffmanCodeEncode() == false ) return false;

	return true;
}
