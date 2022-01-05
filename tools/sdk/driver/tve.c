#include "tve.h"
#include <io.h>

// __u32 tve_reg_base = 0;
// __u8 TVE_dac_set_de_bounce(__u8 index,__u32 times);
// __u8 TVE_dac_int_disable(__u8 index);
// __s32 TVE_close(void);

// __s32 TVE_set_reg_base( __u32 address)
// {
// 	tve_reg_base = address;
// 	return 0;
// }

// __u32 TVE_get_reg_base(void)
// {
// 	return tve_reg_base;
// }

//tve
// init module
////////////////////////////////////////////////////////////////////////////////
__s32 TVE_init(void)
{
	TVE_close();

	TVE_dac_set_de_bounce(0, 0);
	TVE_dac_int_disable(0);

	return 0;
}

__s32 TVE_exit(void)
{
	TVE_dac_int_disable(0);

	return 0;
}

// open module
////////////////////////////////////////////////////////////////////////////////
__s32 TVE_open(__u32 sel)
{
	// TVE_SET_BIT(TVE_000, 0x1 << 0);
	S_Bit(TVE->TVE_000, 0);

	return 0;
}

__s32 TVE_close(void)
{
	// TVE_CLR_BIT(TVE_000, 0x1 << 0);
	C_Bit(TVE->TVE_000, 0);

	return 0;
}

__s32 TVE_set_tv_mode(__u32 sel, __u8 mode)
{
	// __u32 value_08 = TVE_RUINT32(TVE_008);
	// value_08 &= ~(0x3f << 16); //clear bit 17~25
	// value_08 |= (3 << 20);		 //
	// value_08 |= (3 << 18);		 //
	// value_08 |= (3 << 16);		 //
	// TVE_WUINT32(TVE_008, value_08);

	TVE->TVE_008 &= ~(0x3f << 16);
	TVE->TVE_008 |= (3 << 20); //
	TVE->TVE_008 |= (3 << 18); //
	TVE->TVE_008 |= (3 << 16); //

	switch (mode)
	{
	case DISP_TV_MOD_PAL:
		// TVE_WUINT32(TVE_004, 0x07030001);
		TVE->TVE_004 = 0x07030001;
		// TVE_SET_BIT(TVE_004, sel << 30); //set 0x004 reg first, because write it will change other regs
		S_Value(TVE->TVE_004, 30, sel);
		// TVE_WUINT32(TVE_014, 0x008a0018);
		TVE->TVE_014 = 0x008a0018;
		// TVE_WUINT32(TVE_01C, 0x00160271);
		TVE->TVE_01C = 0x00160271;
		// TVE_WUINT32(TVE_114, 0x0016447e);
		TVE->TVE_114 = 0x0016447e;
		// TVE_WUINT32(TVE_124, 0x000005a0);
		TVE->TVE_124 = 0x000005a0;
		// TVE_WUINT32(TVE_130, 0x800D000C);
		TVE->TVE_130 = 0x800D000C;
		// TVE_WUINT32(TVE_13C, 0x00000000);
		TVE->TVE_13C = 0x00000000;
		// TVE_WUINT32(TVE_00C, 0x00000124);
		TVE->TVE_00C = 0x00000124;
		// TVE_WUINT32(TVE_020, 0x00fc00fc);
		TVE->TVE_020 = 0x00fc00fc;
		// TVE_WUINT32(TVE_10C, 0x00004040);
		TVE->TVE_10C = 0x00004040;
		// TVE_WUINT32(TVE_128, 0x00000000); //
		TVE->TVE_128 = 0x00000000;
		// TVE_WUINT32(TVE_118, 0x0000e0e0);
		TVE->TVE_118 = 0x0000e0e0;
		// TVE_WUINT32(TVE_12C, 0x00000101); //
		TVE->TVE_12C = 0x00000101;
		break;

	case DISP_TV_MOD_NTSC:
		// TVE_WUINT32(TVE_004, 0x07030000);
		TVE->TVE_004 = 0x07030000;
		// TVE_SET_BIT(TVE_004, sel << 30); //set 0x004 reg first, because write it will change other regs
		S_Value(TVE->TVE_004, 30, sel);
		// TVE_WUINT32(TVE_014, 0x00760020);
		TVE->TVE_014 = 0x00760020;
		// TVE_WUINT32(TVE_01C, 0x0016020d);
		TVE->TVE_01C = 0x0016020d;
		// TVE_WUINT32(TVE_114, 0x0016447e);
		TVE->TVE_114 = 0x0016447e;
		// TVE_WUINT32(TVE_124, 0x000005a0);
		TVE->TVE_124 = 0x000005a0;
		// TVE_WUINT32(TVE_130, 0x000e000c);
		TVE->TVE_130 = 0x000e000c;
		// TVE_WUINT32(TVE_13C, 0x00000000);
		TVE->TVE_13C = 0x00000000;
		// TVE_WUINT32(TVE_00C, 0x00000124);
		TVE->TVE_00C = 0x00000124;
		// TVE_WUINT32(TVE_020, 0x00f0011a);
		TVE->TVE_020 = 0x00f0011a;
		// TVE_WUINT32(TVE_10C, 0x0000004f);
		TVE->TVE_10C = 0x0000004f;
		// TVE_WUINT32(TVE_110, 0x00000000);
		TVE->TVE_110 = 0x00000000;
		// TVE_WUINT32(TVE_118, 0x0000a0a0);
		TVE->TVE_118 = 0x0000a0a0;
		// TVE_WUINT32(TVE_11C, 0x001000f0);
		TVE->TVE_11C = 0x001000f0;
		// TVE_WUINT32(TVE_128, 0x00000000); //
		TVE->TVE_128 = 0x00000000;
		// TVE_WUINT32(TVE_12C, 0x00000101); //
		TVE->TVE_12C = 0x00000101;

		break;

	default:
		return 0;
	}

	return 0;
}
__s32 TVE_set_vga_mode(__u32 sel)
{
	// TVE_WUINT32(TVE_004, 0x20000000);
	TVE->TVE_004 = 0x20000000;
	// TVE_SET_BIT(TVE_004, sel << 30);
	S_Value(TVE->TVE_004, 30, sel);
	// TVE_WUINT32(TVE_008, 0x40031ac7);
	TVE->TVE_008 = 0x40031ac7;
	// TVE_WUINT32(TVE_024, 0x00000000);
	TVE->TVE_024 = 0x00000000;

	return 0;
}

__u8 TVE_query_interface(__u8 index)
{
	__u8 sts = 0;
	__u32 readval;

	readval = TVE->TVE_038; //TVE_RUINT32(TVE_038);
	sts = readval & (3 << (index * 8));
	sts >>= (index * 8);

	return sts;
}

__u8 TVE_query_int(void)
{
	__u8 sts = 0;
	__u32 readval;

	readval = TVE->TVE_034; //TVE_RUINT32(TVE_034);
	sts = readval & 0x0f;

	return sts;
}
__s32 tve_low_enhance(__u32 sel, __u32 mode)
{
	if (0 == mode)
	{
		// TVE_CLR_BIT(TVE_000, 0xf << 10); /* deflick off */
		C_Value(TVE->TVE_000, 10, 0xF);
		// TVE_SET_BIT(TVE_000, 0x5 << 10); /* deflick is 5 */
		S_Value(TVE->TVE_000, 10, 0x5);
		// TVE_SET_BIT(TVE_00C, 0x1 << 31); /* lti on */
		S_Bit(TVE->TVE_00C, 31);
		// TVE_SET_BIT(TVE_00C, 0x1 << 16); /* notch off */
		S_Bit(TVE->TVE_00C, 16);
	}
	else if (1 == mode)
	{
		// TVE_CLR_BIT(TVE_000, 0xf << 10);
		C_Value(TVE->TVE_000, 10, 0xF);
		// TVE_SET_BIT(TVE_000, 0x5 << 10);
		S_Value(TVE->TVE_000, 10, 0x5);
		// TVE_SET_BIT(TVE_00C, 0x1 << 31);
		S_Bit(TVE->TVE_00C, 31);
		// TVE_CLR_BIT(TVE_00C, 0x1 << 16);
		C_Bit(TVE->TVE_00C, 16);
	}
	else if (2 == mode)
	{
		// TVE_CLR_BIT(TVE_000, 0xf << 10);
		C_Value(TVE->TVE_000, 10, 0xF);
		// TVE_CLR_BIT(TVE_00C, 0x1 << 31);
		C_Bit(TVE->TVE_00C, 31);
		// TVE_SET_BIT(TVE_00C, 0x1 << 16);
		C_Bit(TVE->TVE_00C, 16);
	}
	return 0;
}

__u8 TVE_clear_int(void)
{
	// __u32 sts = 0;
	// __u32 readval;

	// readval = TVE_RUINT32(TVE_034);
	// sts = readval & 0x0f;
	// TVE_WUINT32(TVE_034, sts);
	TVE->TVE_034 &= 0x0f;

	return 0;
}

//0:unconnected; 1:connected; 3:short to ground
__s32 TVE_get_dac_status(__u32 index)
{
	__u32 readval;

	// readval = TVE_RUINT32(TVE_038);
	readval = TVE->TVE_038;

	if (index == 0)
	{
		readval = (readval & 0x00000003);
	}
	else
	{
		printf("ERR: There is only one dac!\n");
	}

	return readval;
}

__u8 TVE_dac_int_enable(__u8 index)
{
	// __u32 readval;

	// readval = TVE_RUINT32(TVE_030);
	// readval |= (1 << (16 + index));
	// TVE_WUINT32(TVE_030, readval);
	TVE->TVE_030 |= (1 << (16 + index));

	return 0;
}

__u8 TVE_dac_int_disable(__u8 index)
{
	// __u32 readval;

	// readval = TVE_RUINT32(TVE_030);
	// readval &= (~(1 << (16 + index)));
	// TVE_WUINT32(TVE_030, readval);
	TVE->TVE_030 &= (~(1 << (16 + index)));

	return 0;
}

__u8 TVE_dac_autocheck_enable(__u8 index)
{
	// __u32 readval;

	// readval = TVE_RUINT32(TVE_030);
	// readval |= (1 << index);
	// TVE_WUINT32(TVE_030, readval);
	TVE->TVE_030 |= (1 << index);

	return 0;
}

__u8 TVE_dac_autocheck_disable(__u8 index)
{
	// __u32 readval;

	// readval = TVE_RUINT32(TVE_030);
	// readval &= (~(1 << index));
	// TVE_WUINT32(TVE_030, readval);
	TVE->TVE_030 &= (~(1 << index));

	return 0;
}

__u8 TVE_dac_enable(__u8 index)
{
	// __u32 readval;

	if (index == 0)
	{
		// readval = TVE_RUINT32(TVE_008);
		// TVE_SET_BIT(TVE_008, readval | (1 << index));
		TVE->TVE_008 |= 1 << index;
	}
	else
	{
		printf("ERR: There is only one DAC to enable!\n");
	}

	return 0;
}

__u8 TVE_dac_disable(__u8 index)
{
	// __u32 readval;

	if (index == 0)
	{
		// readval = TVE_RUINT32(TVE_008);
		// TVE_WUINT32(TVE_008, readval & (~(1 << index)));
		TVE->TVE_008 &= ~(1 << index);
	}
	else
	{
		printf("ERR: There is only one DAC to disable!\n");
	}

	return 0;
}

__s32 TVE_dac_set_source(__u32 index, __u32 source)
{

	printf("ERR: DAC source can't be set\n");

	return 0;
}

__u8 TVE_dac_set_de_bounce(__u8 index, __u32 times)
{
	__u32 readval;

	readval = TVE->TVE_03C; //TVE_RUINT32(TVE_03C);

	if (index == 0)
	{
		readval = (readval & 0xfffffff0) | (times & 0xf);
	}
	else
	{
		printf("ERR: There is only one DAC to set de-bounce!\n");
		return 0;
	}
	// TVE_WUINT32(TVE_03C, readval);
	TVE->TVE_03C = readval;

	return 0;
}

__u8 TVE_dac_get_de_bounce(__u8 index)
{
	__u8 sts = 0;
	__u32 readval;

	readval = TVE->TVE_03C; //TVE_RUINT32(TVE_03C);

	if (index == 0)
	{
		sts = readval & 0xf;
	}
	else
	{
		printf("ERR: There is only one DAC to get de-bounce!\r\n");
		return 0;
	}

	return sts;
}

/*——————————————————————————————————————————————————————————————————————————————
TCON1时钟初始化
*/
void TVE_Clock_Open(void)
{
	// #define TVE_CLK_REG (u32)(0x01C20000 + 0x120)
	//设置video时钟到270MHZ
	// C_BIT(CCU_Base_Address + 0x010, 31);
	C_Bit(CCU->PLL_VIDEO_CTRL_REG, 31);
	// write32(CCU_Base_Address + 0x010, (0x62 << 8));
	CCU->PLL_VIDEO_CTRL_REG = 0x62 << 8;
	// S_BIT(CCU_Base_Address + 0x010, 31);
	S_Bit(CCU->PLL_VIDEO_CTRL_REG, 31);
	sys_delay_ms(1);
	//使能TVE时钟
	// write32(CCU_BUS_CLK_GATING_REG1, read32(CCU_BUS_CLK_GATING_REG1) | (1) << 10);
	S_Bit(CCU->BUS_CLK_GATING_REG1, 10);
	//设置时钟
	// write32(TVE_CLK_REG, read32(TVE_CLK_REG) | ((u64_t)(1) << 31) | (1) << 15 | (9) << 0 | (1) << 8 | (0) << 24); //270MHZ 十分频 27MHZ时钟
	CCU->TVE_CLK_REG |= ((u64_t)(1) << 31) | (1) << 15 | (9) << 0 | (1) << 8 | (0) << 24; //270MHZ 十分频 27MHZ时钟
	//使能TVE复位
	// write32(CCU_BUS_SOFT_RST_REG1, read32(CCU_BUS_SOFT_RST_REG1) | ((1) << 10));
	S_Bit(CCU->BUS_SOFT_RST_REG1, 10);

	sys_delay_ms(1);
}
/*——————————————————————————————————————————————————————————————————————————————
TCON1初始化
*/
int TCON1_Init(__u8 mode)
{ //int i=0;
	int b_interlace, src_x, src_y, scl_x, scl_y, out_x, out_y;
	//----------------------------------
	int ht, hbp, vt, vbp, vspw, hspw;

	if (mode == DISP_TV_MOD_PAL)
	{
		b_interlace = 1;
		src_x = 720;
		src_y = 288;
		scl_x = 720;
		scl_y = 288;
		out_x = 720;
		out_y = 288;
		//----------------------------------
		ht = 864;
		hbp = 139;
		vt = 625;
		vbp = 22;
		vspw = 2;
		hspw = 2;
	}

	//static struct disp_video_timings video_timing[] =
	//{
	//    /*vic        tv_mode  PCLK       AVI    x      y     HT    HBP   HFP    HST   VT    VBP   VFP  VST*/
	//    {0,   DISP_TV_MOD_NTSC, 27000000,  0,  720,   480,   858,   60,   16,   62,   525,   30,   9,    6,  0,   0,   0,   0,   0},
	//    {0,   DISP_TV_MOD_PAL , 27000000,  0,  720,   576,   864,   68,   12,   64,   625,   39,   5,    5,  0,   0,   0,   0,   0},
	//};

	if (mode == DISP_TV_MOD_NTSC)
	{
		b_interlace = 1;
		src_x = 720;
		src_y = 240;
		scl_x = 720;
		scl_y = 240;
		out_x = 720;
		out_y = 240;
		//----------------------------------
		//	ht       = 858;
		//	hbp      = 60;
		//	hspw     = 16;
		//
		//	vt       = 525;
		//	vbp      = 30;
		//	vspw     = 9;

		ht = 858;
		hbp = 118;
		vt = 525;
		vbp = 18;
		vspw = 2;
		hspw = 2;

		//        	b_interlace   = 0;
		//          src_x       = 720;
		//          src_y       = 480;
		//          scl_x       = 720;
		//          scl_y       = 480;
		//          out_x       = 720;
		//          out_y       = 480;
		//          ht       = 858;
		//          hbp      = 118;
		//          vt       = 1050;
		//          vbp      = 22;
		//          vspw     = 2;
		//          hspw     = 2;
	}

	//使能TCON1模块
	int vblank_len = 0;
	int start_delay = 0;
	vblank_len = vt / 2 - src_y - 2;
	if (vblank_len >= 32)
	{
		start_delay = 30;
	}
	else
	{
		start_delay = vblank_len - 2; //23 modify//old:cfg->start_delay	= vblank_len - 1
	}
	// write32(TCON_Base_Address + 0x090, read32(TCON_Base_Address + 0x090) | ((u64_t)0x1 << 31));
	S_Bit(TCON->TCON1_CTRL_REG, 31);
	// write32(TCON_Base_Address + 0x090, read32(TCON_Base_Address + 0x090) | 0); //0=DE 2=BLUE[1V电压] TCON0与TCON1同一源时将关闭LCD输出
	C_Value(TCON->TCON1_CTRL_REG, 0, 0x3);
	TCON->TCON1_CTRL_REG |= 0; //0=DE 2=BLUE[1V电压] TCON0与TCON1同一源时将关闭LCD输出
	// write32(TCON_Base_Address + 0x090, read32(TCON_Base_Address + 0x090) | ((start_delay & 0x1f) << 4));
	TCON->TCON1_CTRL_REG |= ((start_delay & 0x1f) << 4);
	if (b_interlace == 1)
		// write32(TCON_Base_Address + 0x090, read32(TCON_Base_Address + 0x090) | (0x1 << 20));
		TCON->TCON1_CTRL_REG |= (0x1 << 20);
	//源宽高
	// write32(TCON_Base_Address + 0x094, (src_x - 1) << 16 | (src_y - 1) << 0);
	TCON->TCON1_BASIC_REG0 = (src_x - 1) << 16 | (src_y - 1) << 0;
	// write32(TCON_Base_Address + 0x098, (scl_x - 1) << 16 | (scl_y - 1) << 0);
	TCON->TCON1_BASIC_REG1 = (scl_x - 1) << 16 | (scl_y - 1) << 0;
	// write32(TCON_Base_Address + 0x09c, (out_x - 1) << 16 | (out_y - 1) << 0); //输出宽高
	TCON->TCON1_BASIC_REG2 = (out_x - 1) << 16 | (out_y - 1) << 0; //输出宽高

	// write32(TCON_Base_Address + 0x0a0, ((ht - 1) << 16) | ((hbp - 1) << 0));
	TCON->TCON1_BASIC_REG3 = ((ht - 1) << 16) | ((hbp - 1) << 0);
	// write32(TCON_Base_Address + 0x0a4, ((vt) << 16) | ((vbp - 1) << 0));
	TCON->TCON1_BASIC_REG4 = ((vt) << 16) | ((vbp - 1) << 0);
	// write32(TCON_Base_Address + 0x0a8, ((hspw - 1) << 16) | ((vspw - 1) << 0));
	TCON->TCON1_BASIC_REG5 = ((hspw - 1) << 16) | ((vspw - 1) << 0);
	//
	// write32(TCON_Base_Address + 0x0f0, 0);
	TCON->TCON1_IO_CTRL_REG0 = 0;
	// write32(TCON_Base_Address + 0x0f4, 0x0fffffff);
	TCON->TCON1_IO_CTRL_REG1 = 0x0fffffff;
	return 0;
}
/*——————————————————————————————————————————————————————————————————————————————
TVE配置
*/
int TVE_Config(__u8 mode)
{
	// TVE_set_reg_base(0x01C0A000);
	TVE_Clock_Open(); //打开时钟
	TVE_set_tv_mode(0, mode);
	TVE_init();
	TVE_dac_autocheck_enable(0); //dac检测使能
	TVE_dac_enable(0);
	//	tve_low_enhance(0,0);
	//TVE_SET_BIT(TVE_004,0x1<<8);//输出条色
	// S_Bit(TVE->TVE_004, 8); //tao soc man hinh de test
	TVE_open(0);

	// printf("TVE-%08x\r\n", TVE_RUINT32(TVE_004)); //
	printf("TVE-%08x\r\n", TVE->TVE_004); //
	return 0;
}
/*——————————————————————————————————————————————————————————————————————————————
TV初始化
*/
void TVE_Init(lcd_type_t lcd_type)
{
	printf("TVE_Init\r\n");
	TVE_Clock_Open();
	if (LCD_TYPE_TV_PAL_720_576 == lcd_type)
	{
		TCON1_Init(DISP_TV_MOD_PAL);
		TVE_Config(DISP_TV_MOD_PAL);
	}
	else if (LCD_TYPE_TV_NTSC_720_480 == lcd_type)
	{
		TCON1_Init(DISP_TV_MOD_NTSC);
		TVE_Config(DISP_TV_MOD_NTSC);
	}
	printf("TVE_end\r\n");
}
//测试 ——————————————————————————————————————————————————————————————————————————————
extern unsigned short *BT1;

extern unsigned int LCDbuff1[720 * 480 * 2];
unsigned int *BT2;

void TVE_Demo(void)
{
	int i, l;
	printf("TVE_Demo\r\n");

	BT2 = (unsigned int *)LCDbuff1;

	int R = 0xff, G = 0xff, B = 0xff;
	//	for(l=0;l<480;l++)
	//	for(i=0;i<720;i++)
	//	{
	//		if(i<180){R=0XFF;G=0;B=0;}
	//		else if(i<360){R=0;G=0xFF;B=0;}
	//		else if(i<540){R=0;G=0;B=0XFF;}
	//		else if(i<720){R=0XFF;G=0XFF;B=0XFF;}
	//		else {R=0X0;G=0X0;B=0X0;}
	//
	//		BT1[720*l+i]=(u16)(((R>>3)<<11)|((G>>2)<<5)|(B>>3));
	//	}
	//
	for (l = 0; l < 480; l++)
		for (i = 0; i < 720; i++)
		{
			if (i < 180)
			{
				R = 0XFF;
				G = 0;
				B = 0;
			}
			else if (i < 360)
			{
				R = 0;
				G = 0xFF;
				B = 0;
			}
			else if (i < 540)
			{
				R = 0;
				G = 0;
				B = 0XFF;
			}
			else if (i < 720)
			{
				R = 0XFF;
				G = 0XFF;
				B = 0XFF;
			}
			else
			{
				R = 0X0;
				G = 0X0;
				B = 0X0;
			}

			BT2[720 * l + i] = (u32)((0xff << 24) | (R << 16) | (G << 8) | (B << 0));
		}
	//	for(l=0;l<576;l++)
	//	for(i=0;i<720;i++)
	//	{
	//		BT1[720*l+i]=0xffff;
	//	}
	//	for(l=12;l<564;l++)//552
	//	for(i=30;i<682;i++)//652
	//	{
	//		BT1[720*l+i]=0xff;
	//	}
	//

	while (1)
	{
		sys_delay_ms(999);
		printf("R..%d\r\n", TVE_get_dac_status(0));
	}
}
