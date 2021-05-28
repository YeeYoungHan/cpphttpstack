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

typedef struct
{
	uint32_t	iCode;
	int	iBit;
	uint8_t		cChar;
} HuffmanDecode;

static HuffmanDecode garrHuffmanDecode5[] =
{
	{        0x0,  5, 0x30 },
	{        0x1,  5, 0x31 },
	{        0x2,  5, 0x32 },
	{        0x3,  5, 0x61 },
	{        0x4,  5, 0x63 },
	{        0x5,  5, 0x65 },
	{        0x6,  5, 0x69 },
	{        0x7,  5, 0x6f },
	{        0x8,  5, 0x73 },
	{        0x9,  5, 0x74 }
};

static HuffmanDecode garrHuffmanDecode6[] =
{
 	{       0x14,  6, 0x20 },
 	{       0x15,  6, 0x25 },
 	{       0x16,  6, 0x2d },
 	{       0x17,  6, 0x2e },
 	{       0x18,  6, 0x2f },
 	{       0x19,  6, 0x33 },
 	{       0x1a,  6, 0x34 },
 	{       0x1b,  6, 0x35 },
 	{       0x1c,  6, 0x36 },
 	{       0x1d,  6, 0x37 },
 	{       0x1e,  6, 0x38 },
 	{       0x1f,  6, 0x39 },
 	{       0x20,  6, 0x3d },
 	{       0x21,  6, 0x41 },
	{       0x22,  6, 0x5f },
	{       0x23,  6, 0x62 },
	{       0x24,  6, 0x64 },
	{       0x25,  6, 0x66 },
	{       0x26,  6, 0x67 },
	{       0x27,  6, 0x68 },
	{       0x28,  6, 0x6c },
	{       0x29,  6, 0x6d },
	{       0x2a,  6, 0x6e },
	{       0x2b,  6, 0x70 },
	{       0x2c,  6, 0x72 },
	{       0x2d,  6, 0x75 }
};

static HuffmanDecode garrHuffmanDecode7[] =
{
 	{       0x5c,  7, 0x3a },
 	{       0x5d,  7, 0x42 },
 	{       0x5e,  7, 0x43 },
 	{       0x5f,  7, 0x44 },
 	{       0x60,  7, 0x45 },
 	{       0x61,  7, 0x46 },
 	{       0x62,  7, 0x47 },
	{       0x63,  7, 0x48 },
	{       0x64,  7, 0x49 },
	{       0x65,  7, 0x4a },
	{       0x66,  7, 0x4b },
	{       0x67,  7, 0x4c },
	{       0x68,  7, 0x4d },
	{       0x69,  7, 0x4e },
	{       0x6a,  7, 0x4f },
	{       0x6b,  7, 0x50 },
	{       0x6c,  7, 0x51 },
	{       0x6d,  7, 0x52 },
	{       0x6e,  7, 0x53 },
	{       0x6f,  7, 0x54 },
	{       0x70,  7, 0x55 },
	{       0x71,  7, 0x56 },
	{       0x72,  7, 0x57 },
	{       0x73,  7, 0x59 },
	{       0x74,  7, 0x6a },
	{       0x75,  7, 0x6b },
	{       0x76,  7, 0x71 },
	{       0x77,  7, 0x76 },
	{       0x78,  7, 0x77 },
	{       0x79,  7, 0x78 },
	{       0x7a,  7, 0x79 },
	{       0x7b,  7, 0x7a }
};

static HuffmanDecode garrHuffmanDecode8[] =
{
 	{       0xf8,  8, 0x26 },
 	{       0xf9,  8, 0x2a },
 	{       0xfa,  8, 0x2c },
 	{       0xfb,  8, 0x3b },
	{       0xfc,  8, 0x58 },
	{       0xfd,  8, 0x5a }
};

static HuffmanDecode garrHuffmanDecode10[] =
{
 	{      0x3f8, 10, 0x21 },
 	{      0x3f9, 10, 0x22 },
 	{      0x3fa, 10, 0x28 },
 	{      0x3fb, 10, 0x29 },
 	{      0x3fc, 10, 0x3f }
};

static HuffmanDecode garrHuffmanDecode11[] =
{
 	{      0x7fa, 11, 0x27 },
 	{      0x7fb, 11, 0x2b },
	{      0x7fc, 11, 0x7c }
};

static HuffmanDecode garrHuffmanDecode12[] =
{
 	{      0xffa, 12, 0x23 },
 	{      0xffb, 12, 0x3e }
};

static HuffmanDecode garrHuffmanDecode13[] =
{
 	{     0x1ff8, 13, 0x00 },
 	{     0x1ff9, 13, 0x24 },
 	{     0x1ffa, 13, 0x40 },
	{     0x1ffb, 13, 0x5b },
	{     0x1ffc, 13, 0x5d },
	{     0x1ffd, 13, 0x7e }
};

static HuffmanDecode garrHuffmanDecode14[] =
{
	{     0x3ffc, 14, 0x5e },
	{     0x3ffd, 14, 0x7d }
};

static HuffmanDecode garrHuffmanDecode15[] =
{
 	{     0x7ffc, 15, 0x3c },
	{     0x7ffd, 15, 0x60 },
	{     0x7ffe, 15, 0x7b }
};

static HuffmanDecode garrHuffmanDecode19[] =
{
	{    0x7fff0, 19, 0x5c },
	{    0x7fff1, 19, 0xc3 },
	{    0x7fff2, 19, 0xd0 }
};

static HuffmanDecode garrHuffmanDecode20[] =
{
	{    0xfffe6, 20, 0x80 },
	{    0xfffe7, 20, 0x82 },
	{    0xfffe8, 20, 0x83 },
	{    0xfffe9, 20, 0xa2 },
	{    0xfffea, 20, 0xb8 },
	{    0xfffeb, 20, 0xc2 },
	{    0xfffec, 20, 0xe0 },
	{    0xfffed, 20, 0xe2 }
};

static HuffmanDecode garrHuffmanDecode21[] =
{
	{   0x1fffdc, 21, 0x99 },
	{   0x1fffdd, 21, 0xa1 },
	{   0x1fffde, 21, 0xa7 },
	{   0x1fffdf, 21, 0xac },
	{   0x1fffe0, 21, 0xb0 },
	{   0x1fffe1, 21, 0xb1 },
	{   0x1fffe2, 21, 0xb3 },
	{   0x1fffe3, 21, 0xd1 },
	{   0x1fffe4, 21, 0xd8 },
	{   0x1fffe5, 21, 0xd9 },
	{   0x1fffe6, 21, 0xe3 },
	{   0x1fffe7, 21, 0xe5 },
	{   0x1fffe8, 21, 0xe6 }
};

static HuffmanDecode garrHuffmanDecode22[] =
{
	{   0x3fffd2, 22, 0x81 },
	{   0x3fffd3, 22, 0x84 },
	{   0x3fffd4, 22, 0x85 },
	{   0x3fffd5, 22, 0x86 },
	{   0x3fffd6, 22, 0x88 },
	{   0x3fffd7, 22, 0x92 },
	{   0x3fffd8, 22, 0x9a },
	{   0x3fffd9, 22, 0x9c },
	{   0x3fffda, 22, 0xa0 },
	{   0x3fffdb, 22, 0xa3 },
	{   0x3fffdc, 22, 0xa4 },
	{   0x3fffdd, 22, 0xa9 },
	{   0x3fffde, 22, 0xaa },
	{   0x3fffdf, 22, 0xad },
	{   0x3fffe0, 22, 0xb2 },
	{   0x3fffe1, 22, 0xb5 },
	{   0x3fffe2, 22, 0xb9 },
	{   0x3fffe3, 22, 0xba },
	{   0x3fffe4, 22, 0xbb },
	{   0x3fffe5, 22, 0xbd },
	{   0x3fffe6, 22, 0xbe },
	{   0x3fffe7, 22, 0xc4 },
	{   0x3fffe8, 22, 0xc6 },
	{   0x3fffe9, 22, 0xe4 },
	{   0x3fffea, 22, 0xe8 },
	{   0x3fffeb, 22, 0xe9 }
};

static HuffmanDecode garrHuffmanDecode23[] =
{
 	{   0x7fffd8, 23, 0x01 },
	{   0x7fffd9, 23, 0x87 },
	{   0x7fffda, 23, 0x89 },
	{   0x7fffdb, 23, 0x8a },
	{   0x7fffdc, 23, 0x8b },
	{   0x7fffdd, 23, 0x8c },
	{   0x7fffde, 23, 0x8d },
	{   0x7fffdf, 23, 0x8f },
	{   0x7fffe0, 23, 0x93 },
	{   0x7fffe1, 23, 0x95 },
	{   0x7fffe2, 23, 0x96 },
	{   0x7fffe3, 23, 0x97 },
	{   0x7fffe4, 23, 0x98 },
	{   0x7fffe5, 23, 0x9b },
	{   0x7fffe6, 23, 0x9d },
	{   0x7fffe7, 23, 0x9e },
	{   0x7fffe8, 23, 0xa5 },
	{   0x7fffe9, 23, 0xa6 },
	{   0x7fffea, 23, 0xa8 },
	{   0x7fffeb, 23, 0xae },
	{   0x7fffec, 23, 0xaf },
	{   0x7fffed, 23, 0xb4 },
	{   0x7fffee, 23, 0xb6 },
	{   0x7fffef, 23, 0xb7 },
	{   0x7ffff0, 23, 0xbc },
	{   0x7ffff1, 23, 0xbf },
	{   0x7ffff2, 23, 0xc5 },
	{   0x7ffff3, 23, 0xe7 },
	{   0x7ffff4, 23, 0xef }
};

static HuffmanDecode garrHuffmanDecode24[] =
{
 	{   0xffffea, 24, 0x09 },
	{   0xffffeb, 24, 0x8e },
	{   0xffffec, 24, 0x90 },
	{   0xffffed, 24, 0x91 },
	{   0xffffee, 24, 0x94 },
	{   0xffffef, 24, 0x9f },
	{   0xfffff0, 24, 0xab },
	{   0xfffff1, 24, 0xce },
	{   0xfffff2, 24, 0xd7 },
	{   0xfffff3, 24, 0xe1 },
	{   0xfffff4, 24, 0xec },
	{   0xfffff5, 24, 0xed }
};

static HuffmanDecode garrHuffmanDecode25[] =
{
	{  0x1ffffec, 25, 0xc7 },
	{  0x1ffffed, 25, 0xcf },
	{  0x1ffffee, 25, 0xea },
	{  0x1ffffef, 25, 0xeb }
};

static HuffmanDecode garrHuffmanDecode26[] =
{
	{  0x3ffffe0, 26, 0xc0 },
	{  0x3ffffe1, 26, 0xc1 },
	{  0x3ffffe2, 26, 0xc8 },
	{  0x3ffffe3, 26, 0xc9 },
	{  0x3ffffe4, 26, 0xca },
	{  0x3ffffe5, 26, 0xcd },
	{  0x3ffffe6, 26, 0xd2 },
	{  0x3ffffe7, 26, 0xd5 },
	{  0x3ffffe8, 26, 0xda },
	{  0x3ffffe9, 26, 0xdb },
	{  0x3ffffea, 26, 0xee },
	{  0x3ffffeb, 26, 0xf0 },
	{  0x3ffffec, 26, 0xf2 },
	{  0x3ffffed, 26, 0xf3 },
	{  0x3ffffee, 26, 0xff }
};

static HuffmanDecode garrHuffmanDecode27[] =
{
	{  0x7ffffde, 27, 0xcb },
	{  0x7ffffdf, 27, 0xcc },
	{  0x7ffffe0, 27, 0xd3 },
	{  0x7ffffe1, 27, 0xd4 },
	{  0x7ffffe2, 27, 0xd6 },
	{  0x7ffffe3, 27, 0xdd },
	{  0x7ffffe4, 27, 0xde },
	{  0x7ffffe5, 27, 0xdf },
	{  0x7ffffe6, 27, 0xf1 },
	{  0x7ffffe7, 27, 0xf4 },
	{  0x7ffffe8, 27, 0xf5 },
	{  0x7ffffe9, 27, 0xf6 },
	{  0x7ffffea, 27, 0xf7 },
	{  0x7ffffeb, 27, 0xf8 },
	{  0x7ffffec, 27, 0xfa },
	{  0x7ffffed, 27, 0xfb },
	{  0x7ffffee, 27, 0xfc },
	{  0x7ffffef, 27, 0xfd },
	{  0x7fffff0, 27, 0xfe }
};

static HuffmanDecode garrHuffmanDecode28[] =
{
 	{  0xfffffe2, 28, 0x02 },
 	{  0xfffffe3, 28, 0x03 },
 	{  0xfffffe4, 28, 0x04 },
 	{  0xfffffe5, 28, 0x05 },
 	{  0xfffffe6, 28, 0x06 },
 	{  0xfffffe7, 28, 0x07 },
 	{  0xfffffe8, 28, 0x08 },
 	{  0xfffffe9, 28, 0x0b },
 	{  0xfffffea, 28, 0x0c },
 	{  0xfffffeb, 28, 0x0e },
 	{  0xfffffec, 28, 0x0f },
 	{  0xfffffed, 28, 0x10 },
 	{  0xfffffee, 28, 0x11 },
 	{  0xfffffef, 28, 0x12 },
 	{  0xffffff0, 28, 0x13 },
 	{  0xffffff1, 28, 0x14 },
 	{  0xffffff2, 28, 0x15 },
 	{  0xffffff3, 28, 0x17 },
 	{  0xffffff4, 28, 0x18 },
 	{  0xffffff5, 28, 0x19 },
 	{  0xffffff6, 28, 0x1a },
 	{  0xffffff7, 28, 0x1b },
 	{  0xffffff8, 28, 0x1c },
 	{  0xffffff9, 28, 0x1d },
 	{  0xffffffa, 28, 0x1e },
 	{  0xffffffb, 28, 0x1f },
	{  0xffffffc, 28, 0x7f },
	{  0xffffffd, 28, 0xdc },
	{  0xffffffe, 28, 0xf9 }
};

static HuffmanDecode garrHuffmanDecode30[] =
{
 	{ 0x3ffffffc, 30, 0x0a },
 	{ 0x3ffffffd, 30, 0x0d },
 	{ 0x3ffffffe, 30, 0x16 }
};

typedef struct 
{
	HuffmanDecode * psttDecode;
	int		iBit;
	int		iCount;
} HuffmanDecodeInfo;

#define HUFFMAN_DECODE_INFO( arrBuf ) { arrBuf, arrBuf[0].iBit, sizeof(arrBuf) / sizeof(HuffmanDecode) }

static HuffmanDecodeInfo garrHuffmanDecodeInfo[] =
{
	HUFFMAN_DECODE_INFO( garrHuffmanDecode5 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode6 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode7 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode8 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode10 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode11 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode12 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode13 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode14 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode15 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode19 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode20 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode21 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode22 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode23 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode24 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode25 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode26 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode27 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode28 ),
	HUFFMAN_DECODE_INFO( garrHuffmanDecode30 ),
	{ NULL, 0, 0 }
};