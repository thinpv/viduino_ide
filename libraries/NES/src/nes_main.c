#include "nes_main.h"
#include "nes_ppu.h"
#include "nes_mapper.h"
#include "nes_apu.h"
#include <stdlib.h>
#include "timer.h"
// #include "sys_audio.h"
#include "sys-print.h"
#include "ff.h"
#include <string.h>
#include <stdio.h>
#include <fb.h>
// #include "sys_keyAdc.h"
// #include "main.h"
#include "defe.h"
#include <nes_key.h>

#define SRAMIN 0

void *mymalloc(int s, int size)
{
	void *o;
	o = malloc(size);
	if ((unsigned int)o & 0x3)
	{
		printf("malloc err...\r\n");
	}
	return o;
}
void myfree(int s, void *d)
{
	free(d);
}
#define vu8 unsigned char

void mymemset(void *s, u8 c, u32 count)
{
	u8 *xs = s;
	while (count--)
		*xs++ = c;
}

extern u8 frame;	//nes帧计数器
u8 nes_frame_cnt; //nes帧计数器
int MapperNo;			//map编号
int NES_scanline; //nes扫描线
int VROM_1K_SIZE;
int VROM_8K_SIZE;
u32 NESrom_crc32;

u8 PADdata0; //手柄1键值 [7:0]右7 左6 下5 上4 Start3 Select2 B1 A0
u8 PADdata1; //手柄2键值 [7:0]右7 左6 下5 上4 Start3 Select2 B1 A0
u8 *NES_RAM; //保持1024字节对齐
u8 *NES_SRAM;
NES_header *RomHeader; //rom文件头
MAPPER *NES_Mapper;
MapperCommRes *MAPx;

u8 *spr_ram;	 //精灵RAM,256字节
ppu_data *ppu; //ppu指针
u8 *VROM_banks;
u8 *VROM_tiles;

apu_t *apu; //apu指针
u16 *wave_buffers;
u16 *i2sbuf1; //音频缓冲帧,占用内存数 367*4 字节@22050Hz
u16 *i2sbuf2; //音频缓冲帧,占用内存数 367*4 字节@22050Hz

u16 num = 0;
u8 *romfile; //nes文件指针,指向整个nes文件的起始地址.

// void NES_Palette_to_argb8888(void);
// extern __attribute__((aligned(1024))) unsigned short NES_BUFF[3][NES_DH][NES_DW];
// extern uint32_t LCDbuff[480*272];

//defe初始化+16位色变32位色
void defe_color_init(void)
{
	//色彩变换
	// NES_Palette_to_argb8888();
	//缩放
	Defe_Init();
	// Defe_Config((u32)(NES_BUFF[0]));
	// Defe_Config_argb8888_to_argb8888(NES_DW, NES_DH, 480, 272, NES_BUFF[0]);
	// Defe_Config_argb8888_to_argb8888(NES_DW,NES_DH,LV_HOR_RES_MAX,LV_VER_RES_MAX,LCDbuff);
	Defe_Start();
}
//////////////////////////////////////////////////////////////////////////////////////

vu8 system_task_return = 0; //任务强制返回标志.
//加载ROM
//返回值:0,成功
//    1,内存错误
//    3,map错误
u8 nes_load_rom(void)
{
	u8 *p;
	u8 i;
	u8 res = 0;
	p = (u8 *)romfile;
	if (strncmp((const char *)p, "NES", 3) == 0)
	{
		RomHeader->ctrl_z = p[3];
		RomHeader->num_16k_rom_banks = p[4];
		RomHeader->num_8k_vrom_banks = p[5];
		RomHeader->flags_1 = p[6];
		RomHeader->flags_2 = p[7];
		if (RomHeader->flags_1 & 0x04)
			p += 512;														//有512字节的trainer:
		if (RomHeader->num_8k_vrom_banks > 0) //存在VROM,进行预解码
		{
			VROM_banks = p + 16 + (RomHeader->num_16k_rom_banks * 0x4000);
#if NES_RAM_SPEED == 1 //1:内存占用小 0:速度快
			VROM_tiles = VROM_banks;
#else
			VROM_tiles = mymalloc(SRAMIN, RomHeader->num_8k_vrom_banks * 8 * 1024); //这里可能申请多达1MB内存!!!
			if (VROM_tiles == 0)
				VROM_tiles = VROM_banks; //内存不够用的情况下,尝试VROM_titles与VROM_banks共用内存
			compile(RomHeader->num_8k_vrom_banks * 8 * 1024 / 16, VROM_banks, VROM_tiles);
#endif
		}
		else
		{
			VROM_banks = mymalloc(SRAMIN, 8 * 1024);
			VROM_tiles = mymalloc(SRAMIN, 8 * 1024);
			if (!VROM_banks || !VROM_tiles)
				res = 1;
		}
		VROM_1K_SIZE = RomHeader->num_8k_vrom_banks * 8;
		VROM_8K_SIZE = RomHeader->num_8k_vrom_banks;
		MapperNo = (RomHeader->flags_1 >> 4) | (RomHeader->flags_2 & 0xf0);
		if (RomHeader->flags_2 & 0x0E)
			MapperNo = RomHeader->flags_1 >> 4; //忽略高四位，如果头看起来很糟糕
		printf("use map:%d\r\n", MapperNo);
		for (i = 0; i < 255; i++) // 查找支持的Mapper号
		{
			if (MapTab[i] == MapperNo)
				break;
			if (MapTab[i] == -1)
				res = 3;
		}
		if (res == 0)
		{
			switch (MapperNo)
			{
			case 1:
				MAP1 = mymalloc(SRAMIN, sizeof(Mapper1Res));
				if (!MAP1)
					res = 1;
				break;
			case 4:
			case 6:
			case 16:
			case 17:
			case 18:
			case 19:
			case 21:
			case 23:
			case 24:
			case 25:
			case 64:
			case 65:
			case 67:
			case 69:
			case 85:
			case 189:
				MAPx = mymalloc(SRAMIN, sizeof(MapperCommRes));
				if (!MAPx)
					res = 1;
				break;
			default:
				break;
			}
		}
	}
	return res; //返回执行结果
}
//释放内存
void nes_sram_free(void)
{
	myfree(SRAMIN, (void *)NES_RAM);
	myfree(SRAMIN, (void *)NES_SRAM);
	myfree(SRAMIN, RomHeader);
	myfree(SRAMIN, NES_Mapper);
	myfree(SRAMIN, (void *)spr_ram);
	myfree(SRAMIN, ppu);
	myfree(SRAMIN, apu);
	myfree(SRAMIN, (void *)wave_buffers);
	myfree(SRAMIN, (void *)i2sbuf1);
	myfree(SRAMIN, (void *)i2sbuf2);
	myfree(SRAMIN, (void *)romfile);
	if ((VROM_tiles != VROM_banks) && VROM_banks && VROM_tiles) //如果分别为VROM_banks和VROM_tiles申请了内存,则释放
	{
		myfree(SRAMIN, (void *)VROM_banks);
		myfree(SRAMIN, (void *)VROM_tiles);
	}
	switch (MapperNo) //释放map内存
	{
	case 1: //释放内存
		myfree(SRAMIN, MAP1);
		break;
	case 4:
	case 6:
	case 16:
	case 17:
	case 18:
	case 19:
	case 21:
	case 23:
	case 24:
	case 25:
	case 64:
	case 65:
	case 67:
	case 69:
	case 85:
	case 189:
		myfree(SRAMIN, MAPx);
		break; //释放内存
	default:
		break;
	}
	NES_RAM = 0;
	NES_SRAM = 0;
	RomHeader = 0;
	NES_Mapper = 0;
	spr_ram = 0;
	ppu = 0;
	apu = 0;
	wave_buffers = 0;
	i2sbuf1 = 0;
	i2sbuf2 = 0;
	romfile = 0;
	VROM_banks = 0;
	VROM_tiles = 0;
	MAP1 = 0;
	MAPx = 0;
}
//为NES运行申请内存
//romsize:nes文件大小
//返回值:0,申请成功
//       1,申请失败
u8 nes_sram_malloc(u32 romsize)
{
	u16 i = 0;
	for (i = 0; i < 256; i++) //为NES_RAM,查找1024对齐的内存
	{
		NES_SRAM = mymalloc(SRAMIN, i * 4);
		NES_RAM = mymalloc(SRAMIN, 0X800); //申请2K字节,必须1024字节对齐
		if ((u32)NES_RAM % 1024)					 //不是1024字节对齐
		{
			myfree(SRAMIN, (void *)NES_RAM); //释放内存,然后重新尝试分配
			myfree(SRAMIN, (void *)NES_SRAM);
		}
		else
		{
			myfree(SRAMIN, (void *)NES_SRAM); //释放内存
			break;
		}
	}
	NES_SRAM = mymalloc(SRAMIN, 0X2000);
	RomHeader = mymalloc(SRAMIN, sizeof(NES_header));
	NES_Mapper = mymalloc(SRAMIN, sizeof(MAPPER));
	spr_ram = mymalloc(SRAMIN, 0X100);
	ppu = mymalloc(SRAMIN, sizeof(ppu_data));
	apu = mymalloc(SRAMIN, sizeof(apu_t)); //sizeof(apu_t)=  12588
	wave_buffers = mymalloc(SRAMIN, APU_PCMBUF_SIZE * 2);
	i2sbuf1 = mymalloc(SRAMIN, APU_PCMBUF_SIZE * 2);
	i2sbuf2 = mymalloc(SRAMIN, APU_PCMBUF_SIZE * 2);
	romfile = mymalloc(SRAMIN, romsize); //申请游戏rom空间,等于nes文件大小
	while (romfile == NULL)							 //内存不够?释放主界面占用内存,再重新申请
	{
		printf("内存不足\r\n");

		romfile = mymalloc(SRAMIN, romsize); //重新申请
	}
	if (i == 64 || !NES_RAM || !NES_SRAM || !RomHeader || !NES_Mapper || !spr_ram || !ppu || !apu || !wave_buffers || !i2sbuf1 || !i2sbuf2 || !romfile)
	{
		printf("err\r\n");
		nes_sram_free();
		return 1;
	}

	memset(NES_SRAM, 0, 0X2000);									//清零
	memset(RomHeader, 0, sizeof(NES_header));			//清零
	memset(NES_Mapper, 0, sizeof(MAPPER));				//清零
	memset(spr_ram, 0, 0X100);										//清零
	memset(ppu, 0, sizeof(ppu_data));							//清零
	memset(apu, 0, sizeof(apu_t));								//清零
	memset(wave_buffers, 0, APU_PCMBUF_SIZE * 2); //清零
	memset(i2sbuf1, 0, APU_PCMBUF_SIZE * 2);			//清零
	memset(i2sbuf2, 0, APU_PCMBUF_SIZE * 2);			//清零
	memset(romfile, 0, romsize);									//清零
	return 0;
}
//开始nes游戏
//pname:nes游戏路径
//返回值:
//0,正常退出
//1,内存错误
//2,文件错误
//3,不支持的map
u8 nes_load_for_fat(s8 *pname)
{
	FIL *file;
	UINT br;
	u8 res = 0;
	//defe初始化+16位色变32位色
	// defe_color_init();
	/////////////////////////////////////////后加
	file = mymalloc(SRAMIN, sizeof(FIL));
	if (file == 0)
		return 1; //内存申请失败.
	res = f_open(file, (char *)pname, FA_READ);
	if (res != FR_OK) //打开文件失败
	{
		myfree(SRAMIN, file);
		printf("文件打开失败 \r\n");
		return 2;
	}
	printf("文件打开成功 \r\n");

	res = nes_sram_malloc(f_size(file)); //申请内存 ?
	if (res == 0)
	{
		printf("初始化中 \r\n");
		f_read(file, romfile, f_size(file), &br);									 //读取nes文件
		NESrom_crc32 = get_crc32(romfile + 16, f_size(file) - 16); //获取CRC32的值
		printf("加载rom\r\n");
		res = nes_load_rom(); //加载ROM
		if (res == 0)
		{
			printf("加载rom成功\r\n");
			cpu6502_init(); //初始化6502,并复位
			printf("a1\r\n");
			Mapper_Init(); //map初始化
			printf("a2\r\n");
			PPU_reset(); //ppu复位
			printf("a3\r\n");
			apu_init(); //apu初始化

			printf("进入模拟器中\r\n");
			nes_emulate_frame(); //进入NES模拟器主循环
			printf("退出模拟器中\r\n");
			nes_sound_close(); //关闭声音输出
		}
	}
	printf("退出\r\n");
	f_close(file);
	myfree(SRAMIN, file); //释放内存
	nes_sram_free();			//释放内存
	return res;
}
u8 nes_xoff = 0; //显示在x轴方向的偏移量(实际显示宽度=256-2*nes_xoff)

//开始nes游戏
//pname:nes游戏路径
//返回值:
//0,正常退出
//1,内存错误
//2,文件错误
//3,不支持的map

u8 nes_load_for_ram(unsigned char *dat, int dat_len)
{
	u8 res = 0;
	printf("Built-in [ROM]\r\n");
	nes_key_init();
	//defe初始化+16位色变32位色
	// defe_color_init();
	/////////////////////////////////////////后加
	res = nes_sram_malloc(dat_len); //申请内存
	memcpy(romfile, dat, dat_len);
	if (res == 0)
	{
		printf("Initializing\r\n");
		NESrom_crc32 = get_crc32(romfile + 16, dat_len - 16); //获取CRC32的值
		printf("Load rom\r\n");
		res = nes_load_rom(); //加载ROM
		if (res == 0)
		{
			printf("Load rom successfully\r\n");
			cpu6502_init(); //初始化6502,并复位
			printf("a1\r\n");
			Mapper_Init(); //map初始化
			printf("a2\r\n");
			PPU_reset(); //ppu复位
			printf("a3\r\n");
			apu_init(); //apu初始化

			printf("Into the simulator\r\n");
			nes_emulate_frame(); //进入NES模拟器主循环
			printf("Exit the simulator\r\n");
			nes_sound_close(); //关闭声音输出
		}
	}
	printf("drop out\r\n");
	nes_sram_free(); //释放内存
	return res;
}

//读取游戏手柄数据
extern unsigned short *BT1;
uint8_t key1, key2;
void get_key(void)
{
	////right-7 left-6 down-5 up-4 Start-3 Select-2 B-1 A-0
	nes_get_key(&key1, &key2);
	PADdata0 = key1;
	PADdata1 = key2;
}
int frame_count = 0, time = 0;
u8 nes_frame = 0;
//nes模拟器主循环
void nes_emulate_frame(void)
{
	int frame_count = 0, time = 0;
	u8 nes_frame = 0;

	time = millis();

	while (1)
	{
		// LINES 0-239
		PPU_start_frame();

		for (NES_scanline = 0; NES_scanline < 240; NES_scanline++)
		{
			run6502(113 * 256);
			NES_Mapper->HSync(NES_scanline);
			//Scan a line
			if (nes_frame == 0)
				scanline_draw(NES_scanline);
			else
				do_scanline_and_dont_draw(NES_scanline);
		}
		NES_scanline = 240;
		run6502(113 * 256); //Run 1 line
		NES_Mapper->HSync(NES_scanline);
		start_vblank();
		if (NMI_enabled())
		{
			cpunmi = 1;
			run6502(7 * 256); //Operation interrupted
		}
		NES_Mapper->VSync();
		// LINES 242-261
		for (NES_scanline = 241; NES_scanline < 262; NES_scanline++)
		{
			run6502(113 * 256);
			NES_Mapper->HSync(NES_scanline);
		}
		end_vblank();
		get_key(); //Query USB every 3 frames

		nes_frame_cnt++;
		nes_frame++;
		frame_count++;

		//
		if (nes_frame > NES_SKIP_FRAME)
			nes_frame = 0; //Skip frame
		if (system_task_return)
			break; //TPAD返回

		//Display frame number
		if ((millis() - time) > 1000)
		{
			printf("Frame=%d-AUDIO=\r\n", frame_count);
			frame_count = 0;
			time = millis();
		}
		apu_soundoutput(); //Output game sound
	}
}

u8 nes_load(unsigned char *dat, int dat_len)
{
	u8 res = 0;
	printf("Built-in [ROM]\r\n");
	nes_key_init();
	res = nes_sram_malloc(dat_len); //申请内存
	memcpy(romfile, dat, dat_len);
	if (res == 0)
	{
		NESrom_crc32 = get_crc32(romfile + 16, dat_len - 16); //获取CRC32的值
		res = nes_load_rom(); //加载ROM
		if (res == 0)
		{
			printf("Load rom successfully\r\n");
			cpu6502_init(); //初始化6502,并复位
			Mapper_Init(); //map初始化
			PPU_reset(); //ppu复位
			apu_init(); //apu初始化
		}
	}
	return res;
}

void nes_loop()
{
	// LINES 0-239
	PPU_start_frame();

	for (NES_scanline = 0; NES_scanline < 240; NES_scanline++)
	{
		run6502(113 * 256);
		NES_Mapper->HSync(NES_scanline);
		//Scan a line
		if (nes_frame == 0)
			scanline_draw(NES_scanline);
		else
			do_scanline_and_dont_draw(NES_scanline);
	}
	NES_scanline = 240;
	run6502(113 * 256); //Run 1 line
	NES_Mapper->HSync(NES_scanline);
	start_vblank();
	if (NMI_enabled())
	{
		cpunmi = 1;
		run6502(7 * 256); //Operation interrupted
	}
	NES_Mapper->VSync();
	// LINES 242-261
	for (NES_scanline = 241; NES_scanline < 262; NES_scanline++)
	{
		run6502(113 * 256);
		NES_Mapper->HSync(NES_scanline);
	}
	end_vblank();
	get_key(); //Query USB every 3 frames

	nes_frame_cnt++;
	nes_frame++;
	frame_count++;

	//
	if (nes_frame > NES_SKIP_FRAME)
		nes_frame = 0; //Skip frame
	if (system_task_return)
		return; //TPAD返回

	//Display frame number
	if ((millis() - time) > 1000)
	{
		printf("Frame=%d-AUDIO=\r\n", frame_count);
		frame_count = 0;
		time = millis();
	}
	apu_soundoutput(); //Output game sound
}
//在6502.s里面被调用
void debug_6502(u16 reg0, u8 reg1)
{
	printf("6502 error:%x,%d\r\n", reg0, reg1);
}
//////////////////////////////////////////////////////////////////////////////////
//nes,音频输出支持部分

//打开dac传送

int nes_sound_open(int samples_per_sync, int sample_rate)
{
	return 1;
}
//NES关闭音频输出
void nes_sound_close(void)
{
}
//NES音频输出到DAC缓存

void nes_apu_fill_buffer(int samples, u16 *wavebuf)
{
	if (audio_play_callback)
		audio_play_callback((u8 *)wavebuf, APU_PCMBUF_SIZE * 2);
}

video_run_func video_run_callback = NULL;
void set_video_callback(video_run_func _video_run_callback)
{
	video_run_callback = _video_run_callback;
}

audio_play_func audio_play_callback = NULL;
void set_audio_callback(audio_play_func _audio_play_callback)
{
	audio_play_callback = _audio_play_callback;
}
