#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <io.h>
#include <reg-ccu.h>
#include <F1C100S.h>
#include <ccu.h>
#include <dma.h>
#include <audio.h>

/*DMA-register address*/
#define DMA_Base_Address (0x01C02000)
/*Register address*/
#define F1C100S_AUDIO_BASE (0x01C23C00)
#define AUDIO_BASE F1C100S_AUDIO_BASE
#define codec F1C100S_AUDIO_BASE
//
#define CCU_Base_Address (u32_t)0x01C20000
#define CCU_BUS_CLK_GATING_REG0 (u32_t) CCU_Base_Address + 0x0060
#define CCU_BUS_CLK_GATING_REG1 (u32_t) CCU_Base_Address + 0x0064
#define CCU_BUS_CLK_GATING_REG2 (u32_t) CCU_Base_Address + 0x0068
#define CCU_BUS_SOFT_RST_REG0 (u32_t) CCU_Base_Address + 0x02C0
#define CCU_BUS_SOFT_RST_REG1 (u32_t) CCU_Base_Address + 0x02C4
#define CCU_BUS_SOFT_RST_REG2 (u32_t) CCU_Base_Address + 0x02D0

//AUDIO CLK
// #define PLL_AUDIO_CTRL_REG (u32_t) CCU_Base_Address + 0x0008
// #define AUDIO_CODEC_CLK_REG (u32_t) CCU_Base_Address + 0x0140
// #define PLL_AUDIO_BIAS_REG (u32_t) CCU_Base_Address + 0x0224
// #define PLL_AUDIO_PAT_CTRL_REG (u32_t) CCU_Base_Address + 0x0284

#define AC_DAC_DPC 0x00
#define AC_DAC_FIFOC 0x04
#define AC_DAC_FIFOS 0x08
#define AC_ADC_TXDATA 0x0c
#define AC_ADC_FIFOC 0x10
#define AC_ADC_FIFOS 0x14
#define AC_ADC_RXDATA 0x18
#define DAC_MIXER_CTRL 0x20
#define ADC_MIXER_CTRL 0x24
#define ADDA_TUNE 0x28
#define BIAS_DA16_CAL_CTRL0 0x2C
#define BIAS_DA16_CAL_CTRL1 0x34

#define DAC_DRQ_CLR_CNT 21
#define FIFO_FLUSH 0
#define FIR_VER 28
#define ADC_FIFO_FLUSH 0
#define PH_COM_FC 22
#define PH_COM_PROTEC 21
#define HP_VOL 0
#define EN_DAC 31
#define HP_POWER_EN 15
#define R_MIXER_MUTE_R_DAC 17
#define R_MIXER_MUTE_L_DAC 16
#define L_MIXER_MUTE_R_DAC 9
#define L_MIXER_MUTE_L_DAC 8
#define PH_R_MUTE 27
#define PH_L_MUTE 26
#define DAC_AG_R_EN 31
#define DAC_AG_L_EN 30

void MP3WAVplay(char *path);
int _MP3_Play(char *path);
int _WAV_Play(char *path);
int play_exit = 0; //Play exit sign
// __attribute__ ((aligned(4))) unsigned char wav_buff[2][70000];

wav_st wav;

#define SampleRate48KHZ 0	 //000: 48KHz
#define SampleRate24KHZ 2	 //010: 24KHz
#define SampleRate12KHZ 4	 //100: 12KHz
#define SampleRate192KHZ 6 //110: 192KHz
#define SampleRate32KHZ 1	 //001: 32KHz
#define SampleRate16KHZ 3	 //011: 16KHz
#define SampleRate8KHZ 5	 //101: 8KHz
#define SampleRate96KHZ 7	 //111: 96KHz
//
#define SampleRate44_1KHZ 0		//000: 48KHz
#define SampleRate22_05KHZ 2	//010: 24KHz
#define SampleRate11_025KHZ 4 //100: 12KHz

// #define snd_soc_update_bits(Base_Add, Offset, bit, Clear, Value) write32(Base_Add + Offset, read32(Base_Add + Offset) & (~((u64_t)(Clear) << bit)) | ((u64_t)(Value) << bit))
//#define snd_soc_update_bits(Base_Add,Offset,Clear,Value)	write32(Base_Add+Offset,read32(Base_Add+Offset) & (~((u64_t)(Clear))) | ((u64_t)(Value))    )

/* Audio DMA parameter initialization
dma_ch ：dma channel 0-3
Source ：Target pointer
bcnt	：Count byte
*/
void audio_set_dma_data(uint8_t dma_ch, uint32_t *src, uint32_t bcnt, uint8_t continuous)
{
	NDMA_Conf ndma_conf = {0};
	ndma_conf.src_type = NDMA_SRC_TYPE_SDRAM;
	ndma_conf.src_data_width = NDMA_DATA_WIDTH_16;
	// ndma_conf.src_addr_mode = NDMA_ADDR_MODE_IO;
	// ndma_conf.src_burst_length = NDMA_BURST_LENTH_4;

	ndma_conf.des_type = NDMA_DES_TYPE_AUDIO_CODEC;
	ndma_conf.des_data_width = NDMA_DATA_WIDTH_16;
	ndma_conf.des_addr_mode = NDMA_ADDR_MODE_IO;
	// ndma_conf.des_burst_length = NDMA_BURST_LENTH_4;

	ndma_conf.remain_byte_read_enable = 1;
	ndma_conf.dma_continuous_mode_enable = continuous;

	dma_ndma_set(dma_ch, ndma_conf, src, &AUDIO->AC_DAC_TXDATA_REG, bcnt);
}

// /*dma disable*/
// void dma_disable(int dma_ch)
// {
// 	C_BIT(DMA_Base_Address + (0x100 + dma_ch * 0x20 + 0x0), 31);
// }

void audio_enable()
{
	S_Bit(CCU->AUDIO_CODEC_CLK_REG, 31);
}

void audio_disable()
{
	C_Bit(CCU->AUDIO_CODEC_CLK_REG, 31);
}

/*Audio initialization*/
void audio_init(void)
{
	printf("AUDIO Init...\r\n");
	ccu_reset(RESET_AUDIO_CODEC, true);
	sys_delay_ms(1);

	//PLL enable
	S_Bit(CCU->PLL_AUDIO_CTRL_REG, 31);

	//Wait for the PLL to work
	while (R_Bit(CCU->PLL_AUDIO_CTRL_REG, 28) == 0)
		;

	S_Value(AUDIO->AC_DAC_FIFOC_REG, DAC_DRQ_CLR_CNT, 0x3);
	S_Bit(AUDIO->AC_DAC_FIFOC_REG, FIFO_FLUSH);
	/*
	*	0:64-Tap FIR
	*	1:32-Tap FIR
	*/
	C_Bit(AUDIO->AC_DAC_FIFOC_REG, FIR_VER);
	S_Bit(AUDIO->AC_ADC_FIFOC_REG, ADC_FIFO_FLUSH);

	S_Bit(AUDIO->DAC_MIXER_CTRL_REG, PH_COM_PROTEC);
	C_Value(AUDIO->DAC_MIXER_CTRL_REG, PH_COM_FC, 0x3);
	S_Value(AUDIO->DAC_MIXER_CTRL_REG, PH_COM_FC, 0x3);
	/* set hp volume */
	C_Value(AUDIO->DAC_MIXER_CTRL_REG, HP_VOL, 0x3f);
	S_Value(AUDIO->DAC_MIXER_CTRL_REG, HP_VOL, 0x38);
	S_Bit(AUDIO->AC_DAC_DPC_REG, EN_DAC);

	/* power hp */
	S_Bit(AUDIO->DAC_MIXER_CTRL_REG, HP_POWER_EN);

	S_Bit(AUDIO->DAC_MIXER_CTRL_REG, L_MIXER_MUTE_R_DAC);
	S_Bit(AUDIO->DAC_MIXER_CTRL_REG, L_MIXER_MUTE_L_DAC);

	S_Bit(AUDIO->DAC_MIXER_CTRL_REG, R_MIXER_MUTE_R_DAC);
	S_Bit(AUDIO->DAC_MIXER_CTRL_REG, R_MIXER_MUTE_L_DAC);

	S_Bit(AUDIO->DAC_MIXER_CTRL_REG, PH_L_MUTE);
	S_Bit(AUDIO->DAC_MIXER_CTRL_REG, PH_R_MUTE);

	S_Bit(AUDIO->DAC_MIXER_CTRL_REG, DAC_AG_L_EN);
	S_Bit(AUDIO->DAC_MIXER_CTRL_REG, DAC_AG_R_EN);

	/*Audio amplifier switch control, need to initialize first, 
	then turn on the external amplifier, 
	otherwise there will be noise during initialization*/
	//	sys_delay_ms(500);
	//	GPIO_Congif(GPIOE,GPIO_Pin_9,GPIO_Mode_OUT,GPIO_PuPd_NOPULL);
	//	GPIO_RESET(GPIOE,GPIO_Pin_9);
	//	sys_delay_ms(500);

	//Open the empty interrupt when using DMA
	S_Bit(AUDIO->AC_DAC_FIFOC_REG, 4);
	//Send the last sample
	S_Bit(AUDIO->AC_DAC_FIFOC_REG, 26);
}

/*Play initialization*/
int audio_play_init(wav_st *wav_f)
{
	int s = 0, n = 0, m = 0;

	switch (wav_f->sample_rate)
	{
	case 48000:
		s = SampleRate48KHZ;
		n = 0x55;
		m = 0x14;
		break;
	case 24000:
		s = SampleRate24KHZ;
		n = 0x55;
		m = 0x14;
		break;
	case 12000:
		s = SampleRate12KHZ;
		n = 0x55;
		m = 0x14;
		break;
	case 192000:
		s = SampleRate192KHZ;
		n = 0x55;
		m = 0x14;
		break;
	case 32000:
		s = SampleRate32KHZ;
		n = 0x55;
		m = 0x14;
		break;
	case 16000:
		s = SampleRate16KHZ;
		n = 0x55;
		m = 0x14;
		break;
	case 8000:
		s = SampleRate8KHZ;
		n = 0x55;
		m = 0x14;
		break;
	case 96000:
		s = SampleRate96KHZ;
		n = 0x55;
		m = 0x14;
		break;
	case 44100:
		s = SampleRate44_1KHZ;
		n = 0x4E;
		m = 0x14;
		break;
	case 22050:
		s = SampleRate22_05KHZ;
		n = 0x4E;
		m = 0x14;
		break;
	case 11025:
		s = SampleRate11_025KHZ;
		n = 0x4E;
		m = 0x14;
		break;
	default:
		return -1;
	}
	//PLL off
	C_Bit(CCU->PLL_AUDIO_CTRL_REG, 31);

	//-Set PLL-N
	C_Value(CCU->PLL_AUDIO_CTRL_REG, 8, 0x7f);
	S_Value(CCU->PLL_AUDIO_CTRL_REG, 8, n);

	//-Set PLL-M
	C_Value(CCU->PLL_AUDIO_CTRL_REG, 0, 0x1f);
	S_Value(CCU->PLL_AUDIO_CTRL_REG, 0, m);

	//PLL enable
	S_Bit(CCU->PLL_AUDIO_CTRL_REG, 31);

	//Wait for the PLL to work
	while (R_Bit(CCU->PLL_AUDIO_CTRL_REG, 28) == 0)
		;

	/*Set sampling period*/
	C_Value(AUDIO->AC_DAC_FIFOC_REG, 29, 0xf);
	S_Value(AUDIO->AC_DAC_FIFOC_REG, 29, s);

	/*Set the sampling width*/
	if (wav_f->bits_per_sample == 16)
		C_Bit(AUDIO->AC_DAC_FIFOC_REG, 5); //16 bits
	else if (wav_f->bits_per_sample == 24)
		S_Bit(AUDIO->AC_DAC_FIFOC_REG, 5); //24 bits
	else
		return -1;

	/*Set up sampling*/
	if (wav_f->bits_per_sample == 16)
	{
		/*Set the lower 16 bits of the sample*/
		C_Value(AUDIO->AC_DAC_FIFOC_REG, 24, 0x3);
		S_Value(AUDIO->AC_DAC_FIFOC_REG, 24, 0x3);
	}
	else if (wav_f->bits_per_sample == 24)
	{
		/*Set sampling high 24 bits*/
		C_Value(AUDIO->AC_DAC_FIFOC_REG, 24, 0x3);
	}
	else
		return -1;

	/*Set to mono*/
	if (wav_f->num_channels == 1)
		S_Bit(AUDIO->AC_DAC_FIFOC_REG, 6); //Mono
	else if (wav_f->num_channels == 2)
		C_Bit(AUDIO->AC_DAC_FIFOC_REG, 6); //Dual channel
	else
		return -1;

	//Clear sample count
	AUDIO->AC_DAC_CNT_REG = 0;

	return 0;
}

int audio_play_wav_init(void)
{
	/*Unpack WAV files*/
	wav.audio_format = 1;
	wav.num_channels = 1;
	wav.bits_per_sample = 16;
	wav.sample_rate = 22050;

	/*Output*/
	printf("Sampling format: %d \r\n", wav.audio_format);
	printf("Sampling channel: %d \r\n", wav.num_channels);
	printf("Sampling bit width: %d bit\r\n", wav.bits_per_sample);
	printf("The sampling period: %d Hz\r\n", wav.sample_rate);

	/*Uncompressed format*/
	if (wav.audio_format == 1)
	{
		/*Audio initialization*/
		audio_init();
		// DMA_Init();
		dma_init();
		if (audio_play_init(&wav) == 0)
		{
			//S_BIT(AUDIO_CODEC_CLK_REG,31);//Clock gating
			printf("audio_play_wav_init OK \r\n");
		}
	}
	return 0;
}

//Play wav-
__attribute__((aligned(4))) unsigned short buff[2][0x400];
int Inxa = 0, ns = 0;
int au_f = 0;

void audio_play_wav(unsigned char *wav_buff, int Size)
{
	unsigned short *bul;
	int i;
	bul = (unsigned short *)wav_buff;

	if (ns == 0)
	{
		ns = 1;
		audio_set_dma_data(0, (unsigned int *)buff[0], Size, 0);
		// S_BIT(AUDIO_CODEC_CLK_REG, 31); //Clock gating
		// S_Bit(CCU->AUDIO_CODEC_CLK_REG, 31);
		audio_enable();
		for (i = 0; i < Size; i++)
			buff[0][i] = bul[i];
	}
	else
	{
		if (Inxa == 0)
		{
			Inxa = 1;
			while (1)
				if (read32(DMA_Base_Address + 0x4) & 0x2) //Full transmission
				{
					S_BIT(DMA_Base_Address + 0x4, 1);
					// dma_disable(0);
					dma_ndma_disable(0);
					audio_set_dma_data(0, (unsigned int *)buff[0], Size, 0);
					for (i = 0; i < Size; i++)
						buff[1][i] = bul[i];
					au_f++;
					break;
				}
		}
		else
		{
			Inxa = 0;
			while (1)
				if (read32(DMA_Base_Address + 0x4) & 0x2) //Full transmission
				{
					S_BIT(DMA_Base_Address + 0x4, 1);
					dma_ndma_disable(0);
					// dma_disable(0);
					audio_set_dma_data(0, (unsigned int *)buff[1], Size, 0);
					for (i = 0; i < Size; i++)
						buff[0][i] = bul[i];
					au_f++;
					break;
				}
		}
	}
}
