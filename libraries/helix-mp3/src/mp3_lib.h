#ifndef _MP3_LIB_H__
#define _MP3_LIB_H__

#define MP3_FATFS

#include <types.h>

#ifdef MP3_FATFS
#include <ff.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	char TIT2[100];
	char TPE1[100];
	char TALB[100];
} ID3v1_t;

typedef struct
{
	uint32_t version;
	char TIT2[100];
	char TPE1[100];
	char TALB[100];
} ID3v2_t;

int play_mp3_init();
void play_mp3_buffer(uint8_t* buffer, uint32_t len);
void play_mp3_file(char* file_path);

#ifdef __cplusplus
}
#endif

#endif