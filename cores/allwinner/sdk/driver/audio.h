#ifndef __AUDIO_H__
#define __AUDIO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

	typedef struct
	{
		u16 audio_format;
		u16 num_channels;
		u32 format_block_lenght;
		u32 sample_rate;
		u32 byte_rate; //Data transfer rate
		u16 bits_per_sample;
		u32 data_size;
		u32 play_time_ms;
	} wav_st;

	void audio_enable();
	void audio_disable();
	void audio_init(void);
	int audio_play_init(wav_st *wav_f);
	int audio_play_wav_init(void);
	void audio_play_wav(unsigned char *wav_buff, int Size);
	void audio_set_dma_data(uint8_t dma_ch, uint32_t *src, uint32_t bcnt, uint8_t continuous);

#ifdef __cplusplus
}
#endif

#endif
