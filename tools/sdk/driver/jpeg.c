/*
 * Simple JPEG parser (only basic functions)
 *
 * Copyright (c) 2013 Jens Kuske <jenskuske@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#include <stdio.h>
#include "jpeg.h"
#include <F1C100S.h>
#include <malloc.h>

#define M_SOF0  0xc0
#define M_SOF1  0xc1
#define M_SOF2  0xc2
#define M_SOF3  0xc3
#define M_SOF5  0xc5
#define M_SOF6  0xc6
#define M_SOF7  0xc7
#define M_SOF9  0xc9
#define M_SOF10 0xca
#define M_SOF11 0xcb
#define M_SOF13 0xcd
#define M_SOF14 0xce
#define M_SOF15 0xcf
#define M_SOI   0xd8
#define M_EOI   0xd9
#define M_SOS   0xda
#define M_DQT   0xdb
#define M_DRI   0xdd
#define M_DHT   0xc4
#define M_DAC   0xcc

const char comp_types[5][3] = { "Y", "Cb", "Cr" };

static int process_dqt(struct jpeg_t *jpeg, const uint8_t *data, const int len)
{
	int pos;

	for (pos = 0; pos < len; pos += 65)
	{
		if ((data[pos] >> 4) != 0)
		{
			fprintf(stderr, "Only 8bit Quantization Tables supported\r\n");
			return 0;
		}
		jpeg->quant[data[pos] & 0x03] = (struct quant_t *)&(data[pos + 1]);
	}

	return 1;
}

static int process_dht(struct jpeg_t *jpeg, const uint8_t *data, const int len)
{
	int pos = 0;

	while (pos < len)
	{
		uint8_t id = ((data[pos] & 0x03) << 1) | ((data[pos] & 0x10) >> 4);

		jpeg->huffman[id] = (struct huffman_t *)&(data[pos + 1]);

		int i;
		pos += 17;
		for (i = 0; i < 16; i++)
			pos += jpeg->huffman[id]->num[i];
	}
	return 1;
}

int parse_jpeg(struct jpeg_t *jpeg, const uint8_t *data, const int len)
{
	if (len < 2 || data[0] != 0xff || data[1] != M_SOI)
		return 0;

	int pos = 2;
	int sos = 0;
	while (!sos)
	{
		int i;

		while (pos < len && data[pos] == 0xff)
			pos++;

		if (!(pos + 2 < len))
			return 0;

		uint8_t marker = data[pos++];
		uint16_t seg_len = ((uint16_t)data[pos]) << 8 | (uint16_t)data[pos + 1];

		if (!(pos + seg_len < len))
			return 0;

		switch (marker)
		{
		case M_DQT:
			if (!process_dqt(jpeg, &data[pos + 2], seg_len - 2))
				return 0;

			break;

		case M_DHT:
			if (!process_dht(jpeg, &data[pos + 2], seg_len - 2))
				return 0;

			break;

		case M_SOF0:
			jpeg->bits = data[pos + 2];
			jpeg->width = ((uint16_t)data[pos + 5]) << 8 | (uint16_t)data[pos + 6];
			jpeg->height = ((uint16_t)data[pos + 3]) << 8 | (uint16_t)data[pos + 4];
			for (i = 0; i < data[pos + 7]; i++)
			{
				uint8_t id = data[pos + 8 + 3 * i] - 1;
				if (id > 2)
				{
					fprintf(stderr, "only YCbCr supported\r\n");
					return 0;
				}
				jpeg->comp[id].samp_h = data[pos + 9 + 3 * i] >> 4;
				jpeg->comp[id].samp_v = data[pos + 9 + 3 * i] & 0x0f;
				jpeg->comp[id].qt = data[pos + 10 + 3 * i];
			}
			break;

		case M_DRI:
			jpeg->restart_interval = ((uint16_t)data[pos + 2]) << 8 | (uint16_t)data[pos + 3];
			break;

		case M_SOS:
			for (i = 0; i < data[pos + 2]; i++)
			{
				uint8_t id = data[pos + 3 + 2 * i] - 1;
				if (id > 2)
				{
					fprintf(stderr, "only YCbCr supported\r\n");
					return 0;
				}
				jpeg->comp[id].ht_dc = data[pos + 4 + 2 * i] >> 4;
				jpeg->comp[id].ht_ac = data[pos + 4 + 2 * i] & 0x0f;
			}
			sos = 1;
			break;

		case M_DAC:
			fprintf(stderr, "Arithmetic Coding unsupported\r\n");
			return 0;

		case M_SOF1:
		case M_SOF2:
		case M_SOF3:
		case M_SOF5:
		case M_SOF6:
		case M_SOF7:
		case M_SOF9:
		case M_SOF10:
		case M_SOF11:
		case M_SOF13:
		case M_SOF14:
		case M_SOF15:
			fprintf(stderr, "only Baseline DCT supported (yet?)\r\n");
			return 0;

		case M_SOI:
		case M_EOI:
			fprintf(stderr, "corrupted file\r\n");
			return 0;

		default:
			//fprintf(stderr, "unknown marker: 0x%02x len: %u\r\n", marker, seg_len);
			break;
		}
		pos += seg_len;
	}

	jpeg->data = (uint8_t *)&(data[pos]);
	jpeg->data_len = len - pos;

	return 1;
}

void dump_jpeg(const struct jpeg_t *jpeg)
{
	int i, j, k;
	printf("Width: %u  Height: %u  Bits: %u\r\nRestart interval: %u\r\nComponents:\r\n", jpeg->width, jpeg->height, jpeg->bits, jpeg->restart_interval);
	for (i = 0; i < 3; i++)
	{
		if (jpeg->comp[i].samp_h && jpeg->comp[i].samp_v)
			printf("\tType: %s  Sampling: %u:%u  QT: %u  HT: %u/%u\r\n", comp_types[i], jpeg->comp[i].samp_h, jpeg->comp[i].samp_v, jpeg->comp[i].qt, jpeg->comp[i].ht_dc, jpeg->comp[i].ht_dc);
	}
	printf("Quantization Tables:\r\n");
	for (i = 0; i < 4; i++)
	{
		if (jpeg->quant[i])
		{
			printf("\tID: %u\r\n", i);
			for (j = 0; j < 64 / 8; j++)
			{
				printf("\t\t");
				for (k = 0; k < 8; k++)
				{
					printf("0x%02x ", jpeg->quant[i]->coeff[j * 8 + k]);
				}
				printf("\r\n");
			}
		}
	}
	printf("Huffman Tables:\r\n");
	for (i = 0; i < 8; i++)
	{
		if (jpeg->huffman[i])
		{
			printf("\tID: %u (%cC)\r\n", (i & 0x06) >> 1, i & 0x01 ? 'A' : 'D');
			int sum = 0;
			for (j = 0; j < 16; j++)
			{
				if (jpeg->huffman[i]->num[j])
				{
					printf("\t\t%u bits:", j + 1);
					for (k = 0; k < jpeg->huffman[i]->num[j]; k++)
					{
						printf(" 0x%02x", jpeg->huffman[i]->codes[sum + k]);
					}
					printf("\r\n");
					sum += jpeg->huffman[i]->num[j];
				}
			}
		}
	}
	printf("Data length: %u\r\n", jpeg->data_len);
}

#if 0
// http://elm-chan.org/junk/32bit/binclude.html
/* Import a binary file */
#define IMPORT_BIN(sect, file, sym) asm (\
    ".section " #sect "\r\n"                  /* Change section */\
    ".balign 4\r\n"                           /* Word alignment */\
    ".global " #sym "\r\n"                    /* Export the object address */\
    #sym ":\r\n"                              /* Define the object label */\
    ".incbin \"" file "\"\r\n"                /* Import the file */\
    ".global _sizeof_" #sym "\r\n"            /* Export the object size */\
    ".set _sizeof_" #sym ", . - " #sym "\r\n" /* Define the object size */\
    ".balign 4\r\n"                           /* Word alignment */\
    ".section \".text\"\r\n")                 /* Restore section */

IMPORT_BIN(".rodata", "Avatar.mjpg", mjpg);
//IMPORT_BIN(".rodata", "NextGen.mjpg", mjpg);
#endif

/******************************************************************************/
/*                                   JPG                                      */
/******************************************************************************/
void set_quantization_tables (struct jpeg_t *jpeg)
{
	int i;
	for (i = 0; i < 64; i++)
    MPEG->IQ_MIN_INPUT = (uint32_t)(64 + i) << 8 | jpeg->quant[jpeg->comp[0].qt]->coeff[i];
	for (i = 0; i < 64; i++)
    MPEG->IQ_MIN_INPUT = (uint32_t)(i) << 8 | jpeg->quant[jpeg->comp[1].qt]->coeff[i];
}

void set_huffman_tables(struct jpeg_t *jpeg)
{
	uint32_t buffer[512];
	memset(buffer, 0, 4*512);
	int i;
	for (i = 0; i < 4; i++)
	{
		if (jpeg->huffman[i])
		{
			int j, sum, last;

			last = 0;
			sum = 0;
			for (j = 0; j < 16; j++)
			{
				((uint8_t *)buffer)[i * 64 + 32 + j] = sum;
				sum += jpeg->huffman[i]->num[j];
				if (jpeg->huffman[i]->num[j] != 0)
					last = j;
			}
			memcpy(&(buffer[256 + 64 * i]), jpeg->huffman[i]->codes, sum);
			sum = 0;
			for (j = 0; j <= last; j++)
			{
				((uint16_t *)buffer)[i * 32 + j] = sum;
				sum += jpeg->huffman[i]->num[j];
				sum *= 2;
			}
			for (j = last + 1; j < 16; j++)
			{
				((uint16_t *)buffer)[i * 32 + j] = 0xffff;
			}
		}
	}
	for (i = 0; i < 512; i++) MPEG->RAM_WRITE_DATA = buffer[i];
}

void set_format(struct jpeg_t *jpeg)
{
  int fmt = (jpeg->comp[0].samp_h << 4) | jpeg->comp[0].samp_v;
  if(fmt == 0x11) fmt = 0x1B000000;
  else if(fmt == 0x21) fmt = 0x13000000;
  else if(fmt == 0x12) fmt = 0x23000000;
  else if(fmt == 0x22) fmt = 0x03000000;
  else fmt = 0;
  MPEG->TRIG = fmt;
  //printf("fmt:%x\r\n", fmt);
}

void set_size(struct jpeg_t *jpeg)
{
  uint16_t h = (jpeg->height - 1) / (8 * jpeg->comp[0].samp_v);
  uint16_t w = (jpeg->width - 1) / (8 * jpeg->comp[0].samp_h);
  //uint16_t h = jpeg->height / (8 * jpeg->comp[0].samp_v) - 1;
  //uint16_t w = jpeg->width  / (8 * jpeg->comp[0].samp_h) - 1;
  MPEG->JPEG_SIZE = (uint32_t)h << 16 | w;
}

/* Tables for YCbCr->RGB and BG_YCC->RGB colorspace conversion */
static const s16 crrtab[] = {
  -179, -178, -176, -175, -173, -172, -171, -169, 
  -168, -166, -165, -164, -162, -161, -159, -158, 
  -157, -155, -154, -152, -151, -150, -148, -147, 
  -145, -144, -143, -141, -140, -138, -137, -136, 
  -134, -133, -131, -130, -129, -127, -126, -124, 
  -123, -122, -120, -119, -117, -116, -114, -113, 
  -112, -110, -109, -107, -106, -105, -103, -102, 
  -100, -99, -98, -96, -95, -93, -92, -91, 
  -89, -88, -86, -85, -84, -82, -81, -79, 
  -78, -77, -75, -74, -72, -71, -70, -68, 
  -67, -65, -64, -63, -61, -60, -58, -57, 
  -56, -54, -53, -51, -50, -49, -47, -46, 
  -44, -43, -42, -40, -39, -37, -36, -35, 
  -33, -32, -30, -29, -28, -26, -25, -23, 
  -22, -21, -19, -18, -16, -15, -14, -12, 
  -11, -9, -8, -7, -5, -4, -2, -1, 
  0, 1, 2, 4, 5, 7, 8, 9, 
  11, 12, 14, 15, 16, 18, 19, 21, 
  22, 23, 25, 26, 28, 29, 30, 32, 
  33, 35, 36, 37, 39, 40, 42, 43, 
  44, 46, 47, 49, 50, 51, 53, 54, 
  56, 57, 58, 60, 61, 63, 64, 65, 
  67, 68, 70, 71, 72, 74, 75, 77, 
  78, 79, 81, 82, 84, 85, 86, 88, 
  89, 91, 92, 93, 95, 96, 98, 99, 
  100, 102, 103, 105, 106, 107, 109, 110, 
  112, 113, 114, 116, 117, 119, 120, 122, 
  123, 124, 126, 127, 129, 130, 131, 133, 
  134, 136, 137, 138, 140, 141, 143, 144, 
  145, 147, 148, 150, 151, 152, 154, 155, 
  157, 158, 159, 161, 162, 164, 165, 166, 
  168, 169, 171, 172, 173, 175, 176, 178
};

static const s16 crgtab[] = {
  -91, -90, -89, -89, -88, -87, -87, -86, 
  -85, -84, -84, -83, -82, -82, -81, -80, 
  -79, -79, -78, -77, -77, -76, -75, -74, 
  -74, -73, -72, -72, -71, -70, -69, -69, 
  -68, -67, -67, -66, -65, -64, -64, -63, 
  -62, -62, -61, -60, -59, -59, -58, -57, 
  -57, -56, -55, -54, -54, -53, -52, -52, 
  -51, -50, -49, -49, -48, -47, -47, -46, 
  -45, -44, -44, -43, -42, -42, -41, -40, 
  -39, -39, -38, -37, -37, -36, -35, -34, 
  -34, -33, -32, -32, -31, -30, -29, -29, 
  -28, -27, -27, -26, -25, -24, -24, -23, 
  -22, -22, -21, -20, -19, -19, -18, -17, 
  -17, -16, -15, -14, -14, -13, -12, -12, 
  -11, -10, -9, -9, -8, -7, -7, -6, 
  -5, -4, -4, -3, -2, -2, -1, 0, 
  0, 0, 1, 2, 2, 3, 4, 4, 
  5, 6, 7, 7, 8, 9, 9, 10, 
  11, 12, 12, 13, 14, 14, 15, 16, 
  17, 17, 18, 19, 19, 20, 21, 22, 
  22, 23, 24, 24, 25, 26, 27, 27, 
  28, 29, 29, 30, 31, 32, 32, 33, 
  34, 34, 35, 36, 37, 37, 38, 39, 
  39, 40, 41, 42, 42, 43, 44, 44, 
  45, 46, 47, 47, 48, 49, 49, 50, 
  51, 52, 52, 53, 54, 54, 55, 56, 
  57, 57, 58, 59, 59, 60, 61, 62, 
  62, 63, 64, 64, 65, 66, 67, 67, 
  68, 69, 69, 70, 71, 72, 72, 73, 
  74, 74, 75, 76, 77, 77, 78, 79, 
  79, 80, 81, 82, 82, 83, 84, 84, 
  85, 86, 87, 87, 88, 89, 89, 90
};

static const s16 cbbtab[] = {
  -226, -225, -223, -221, -219, -217, -216, -214, 
  -212, -210, -209, -207, -205, -203, -202, -200, 
  -198, -196, -194, -193, -191, -189, -187, -186, 
  -184, -182, -180, -178, -177, -175, -173, -171, 
  -170, -168, -166, -164, -163, -161, -159, -157, 
  -155, -154, -152, -150, -148, -147, -145, -143, 
  -141, -139, -138, -136, -134, -132, -131, -129, 
  -127, -125, -124, -122, -120, -118, -116, -115, 
  -113, -111, -109, -108, -106, -104, -102, -101, 
  -99, -97, -95, -93, -92, -90, -88, -86, 
  -85, -83, -81, -79, -77, -76, -74, -72, 
  -70, -69, -67, -65, -63, -62, -60, -58, 
  -56, -54, -53, -51, -49, -47, -46, -44, 
  -42, -40, -38, -37, -35, -33, -31, -30, 
  -28, -26, -24, -23, -21, -19, -17, -15, 
  -14, -12, -10, -8, -7, -5, -3, -1, 
  0, 1, 3, 5, 7, 8, 10, 12, 
  14, 15, 17, 19, 21, 23, 24, 26, 
  28, 30, 31, 33, 35, 37, 38, 40, 
  42, 44, 46, 47, 49, 51, 53, 54, 
  56, 58, 60, 62, 63, 65, 67, 69, 
  70, 72, 74, 76, 77, 79, 81, 83, 
  85, 86, 88, 90, 92, 93, 95, 97, 
  99, 101, 102, 104, 106, 108, 109, 111, 
  113, 115, 116, 118, 120, 122, 124, 125, 
  127, 129, 131, 132, 134, 136, 138, 139, 
  141, 143, 145, 147, 148, 150, 152, 154, 
  155, 157, 159, 161, 163, 164, 166, 168, 
  170, 171, 173, 175, 177, 178, 180, 182, 
  184, 186, 187, 189, 191, 193, 194, 196, 
  198, 200, 202, 203, 205, 207, 209, 210, 
  212, 214, 216, 217, 219, 221, 223, 225
};

static const s16 cbgtab[] = {
  -44, -43, -43, -42, -42, -42, -41, -41, 
  -41, -40, -40, -40, -39, -39, -39, -38, 
  -38, -38, -37, -37, -37, -36, -36, -36, 
  -35, -35, -35, -34, -34, -34, -33, -33, 
  -33, -32, -32, -31, -31, -31, -30, -30, 
  -30, -29, -29, -29, -28, -28, -28, -27, 
  -27, -27, -26, -26, -26, -25, -25, -25, 
  -24, -24, -24, -23, -23, -23, -22, -22, 
  -22, -21, -21, -20, -20, -20, -19, -19, 
  -19, -18, -18, -18, -17, -17, -17, -16, 
  -16, -16, -15, -15, -15, -14, -14, -14, 
  -13, -13, -13, -12, -12, -12, -11, -11, 
  -11, -10, -10, -9, -9, -9, -8, -8, 
  -8, -7, -7, -7, -6, -6, -6, -5, 
  -5, -5, -4, -4, -4, -3, -3, -3, 
  -2, -2, -2, -1, -1, -1, 0, 0, 
  0, 0, 0, 1, 1, 1, 2, 2, 
  2, 3, 3, 3, 4, 4, 4, 5, 
  5, 5, 6, 6, 6, 7, 7, 7, 
  8, 8, 8, 9, 9, 9, 10, 10, 
  11, 11, 11, 12, 12, 12, 13, 13, 
  13, 14, 14, 14, 15, 15, 15, 16, 
  16, 16, 17, 17, 17, 18, 18, 18, 
  19, 19, 19, 20, 20, 20, 21, 21, 
  22, 22, 22, 23, 23, 23, 24, 24, 
  24, 25, 25, 25, 26, 26, 26, 27, 
  27, 27, 28, 28, 28, 29, 29, 29, 
  30, 30, 30, 31, 31, 31, 32, 32, 
  33, 33, 33, 34, 34, 34, 35, 35, 
  35, 36, 36, 36, 37, 37, 37, 38, 
  38, 38, 39, 39, 39, 40, 40, 40, 
  41, 41, 41, 42, 42, 42, 43, 43
};

int yuv2rgb565 (uint8_t Y, uint8_t Cb, uint8_t Cr)
{
  int color, rgb;
  color = Y + crrtab[Cr];
  if(color < 0) color = 0; else if(color > 255) color = 255;
  rgb = (color & 0xF8) << 8;
  color = Y - cbgtab[Cb] - crgtab[Cr];
  if(color < 0) color = 0; else if(color > 255) color = 255;
  rgb |= (color & 0xFC) << 3;
  color = Y +cbbtab[Cb];
  if(color < 0) color = 0; else if(color > 255) color = 255;
  return rgb | (color >> 3);
}

void yuv2rgb(uint16_t* out, int width, int height, uint8_t* luma, uint8_t* chroma, int samp)
{
  int x, y, cy;
  for(y = 0; y < height; y++)
  {
    cy = y / samp;
    cy = ((cy % 32) * 32) + ((cy / 32) * (((width + 31) / 32) * 1024));
    for(x = 0; x < width; x += 4)
    {
      uint32_t Y = *((uint32_t*)(luma + (x / 32) * 1024 + (x % 32) + ((y % 32) * 32) + ((y / 32) * (((width + 31) / 32) * 1024))));
      uint32_t C = *((uint32_t*)(chroma + (x / 32) * 1024 + ((x % 32) / 2 * 2) + cy));
      *out++ = yuv2rgb565(Y, C, C >> 8);
      *out++ = yuv2rgb565(Y >> 8, C, C >> 8);
      *out++ = yuv2rgb565(Y >> 16, C >> 16, C >> 24);
      *out++ = yuv2rgb565(Y >> 24, C >> 16, C >> 24);
    }
  }
}

void decode_jpeg(struct jpeg_t *jpeg, uint16_t *frame)
{
  int input_size =(jpeg->data_len + 65535) & ~65535;
  int output_size = ((jpeg->width + 31) & ~31) * ((jpeg->height + 31) & ~31);
  uint8_t *input_buffer = memalign(1024, input_size);
  uint8_t *luma_output = memalign(1024, output_size);
  uint8_t *chroma_output = memalign(1024, output_size);
  int ctr_ms = millis();
  VE->CTRL = 0x00000000;                        // activate MPEG engine
  memcpy(input_buffer, jpeg->data, jpeg->data_len);
  MPEG->JPEG_RES_INT = jpeg->restart_interval;
  set_format(jpeg);                             // set JPEG format
  MPEG->ROT_LUMA = (uint32_t)luma_output;       // set output buffers (Luma / Croma)
  MPEG->ROT_CHROMA = (uint32_t)chroma_output;
  set_size(jpeg);                               // set size
  MPEG->ROT_SCALE_CTRL = 0;                     // scale 1:1
  MPEG->VLD_END = (uint32_t)input_buffer + input_size - 1;  // input end
  MPEG->VLD_OFFSET = 0;// * 8;                     // set input offset in bits
  MPEG->VLD_LEN = jpeg->data_len * 8;           // set input length in bits
  MPEG->VLD_ADDR = 0x70000000 | ((uint32_t)input_buffer & 0x0ffffff0) | (uint32_t)input_buffer >> 28;
  set_quantization_tables(jpeg);                // set Quantisation Table
  MPEG->RAM_WRITE_PTR = 0;                      // set Huffman Table
  set_huffman_tables(jpeg);
  MPEG->TRIG |= 0x8000000E;                             // Start MPEGs: 0xe, JPEG: 0xd ..
  for(uint32_t tm = ctr_ms + 1000; millis() < tm; )
  {
    if(MPEG->STATUS & 3) break;
  }
  MPEG->STATUS = 0x0000c00f;                    // clean interrupt flag
  VE->CTRL = 7;
  printf("JPGDEC: %umS\r\r\n", millis() - ctr_ms);
  /* Make rgb565 */
  ctr_ms = 0;
  yuv2rgb(frame, jpeg->width, jpeg->height, luma_output, chroma_output, jpeg->comp[0].samp_v);
  printf("yuv2rgb done\r\r\n");
  free(chroma_output);
  free(luma_output);
  free(input_buffer);
  printf("free done\r\r\n");
  printf("YUV2RGB: %umS\r\r\n", ctr_ms);
}
