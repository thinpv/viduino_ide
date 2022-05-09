#ifndef __NES_MAIN_H
#define __NES_MAIN_H
#include <types.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define NES_SKIP_FRAME 2 //����ģ������֡��,Ĭ����2֡

#define INLINE static inline
#define int8 char
#define int16 short
#define int32 int
#define uint8 unsigned char
#define uint16 unsigned short
#define uint32 unsigned int
#define boolean uint8

#define NES_DW 256
#define NES_DH 240

	typedef void (*video_run_func)(int, int, int, int, unsigned short *);
	extern video_run_func video_run_callback;

	typedef void (*audio_play_func)(unsigned char *, int);
	extern audio_play_func audio_play_callback;

	//nes��Ϣͷ�ṹ��
	typedef struct
	{
		unsigned char id[3];	// 'NES'
		unsigned char ctrl_z; // control-z
		unsigned char num_16k_rom_banks;
		unsigned char num_8k_vrom_banks;
		unsigned char flags_1;
		unsigned char flags_2;
		unsigned char reserved[8];
	} NES_header;

	extern u8 nes_frame_cnt;			//nes֡������
	extern int MapperNo;					//map���
	extern int NES_scanline;			//ɨ����
	extern NES_header *RomHeader; //rom�ļ�ͷ
	extern int VROM_1K_SIZE;
	extern int VROM_8K_SIZE;
	extern u8 cpunmi; //cpu�жϱ�־  �� 6502.s����
	extern u8 cpuirq;
	extern u8 PADdata;		 //�ֱ�1��ֵ
	extern u8 PADdata1;		 //�ֱ�1��ֵ
	extern u8 lianan_biao; //������־
#define CPU_NMI cpunmi = 1;
#define CPU_IRQ cpuirq = 1;
#define NES_RAM_SPEED 0 //1:�ڴ�ռ��С  0:�ٶȿ�

	void cpu6502_init(void); //�� cart.s
	void run6502(u32);			 //�� 6502.s
	u8 nes_load_rom(void);
	void nes_sram_free(void);
	u8 nes_sram_malloc(u32 romsize);
	u8 nes_load_for_fat(s8 *pname);
	u8 nes_load_for_ram(unsigned char *dat, int dat_len);
	void nes_set_window(void);
	void nes_get_gamepadval(void);
	void nes_emulate_frame(void);
	void debug_6502(u16 reg0, u8 reg1);

	void nes_i2s_dma_tx_callback(void);
	int nes_sound_open(int samples_per_sync, int sample_rate);
	void nes_sound_close(void);
	void nes_apu_fill_buffer(int samples, u16 *wavebuf);

	extern u32 get_crc32(u8 *buf, u32 len);

	void mymemset(void *s, u8 c, u32 count);
	void *mymalloc(int s, int size);
	void myfree(int s, void *d);

	u8 nes_load(unsigned char *dat, int dat_len);
	void nes_loop();

	void set_video_callback(video_run_func _video_run_callback);
	void set_audio_callback(audio_play_func _audio_play_callback);

#ifdef __cplusplus
}
#endif

#endif
