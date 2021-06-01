#include "sys_lcd.h"
#include "reg-debe.h"
#include "reg-tcon.h"
// #include "sys_interrupt.h"
// #include "sys_uart.h"
// #include "sys_gpio.h"
// #include "sys_delay.h"
// #include "sys_io.h"
#include <stdlib.h>
// #include "main.h"

#include <irq.h>
#include <io.h>

/*---------------------------------------------------
TCON 中断
----------------------------------------------------*/
volatile int LCD_TE = 0;
void TCON_ISR(void)
{
	C_BIT(TCON_Base_Address + 0x04, 13); //Clera TCON0 line trigger interrupt
	LCD_TE = 1;
}
/*---------------------------------------------------
等待TCON line trigger中断
----------------------------------------------------*/
void Wait_tcon_te(void)
{
	int tcon_timeout = 0; //超时退出
	LCD_TE = 0;
	while (1)
	{
		delay_ms(1); //延时可做其它任务
		tcon_timeout++;
		if ((LCD_TE == 1) || (tcon_timeout > 30))
			break;
	}
}
//RGB转YUV系数
unsigned int csc_tab2[4][12] =
		{
				{0x0204, 0x0107, 0x0064, 0x0100, 0x1ed6, 0x1f68, 0x01c1, 0x0800, 0x1e87, 0x01c1, 0x1fb7, 0x0800}, //bt601 rgb2yuv
				{0x0274, 0x00bb, 0x003f, 0x0100, 0x1ea5, 0x1f98, 0x01c1, 0x0800, 0x1e67, 0x01c1, 0x1fd7, 0x0800}, //bt709 rgb2yuv
				{0x0258, 0x0132, 0x0075, 0x0000, 0x1eac, 0x1f53, 0x0200, 0x0800, 0x1e53, 0x0200, 0x1fac, 0x0800}, //DISP_YCC rgb2yuv
				{0x0274, 0x00bb, 0x003f, 0x0100, 0x1ea5, 0x1f98, 0x01c1, 0x0800, 0x1e67, 0x01c1, 0x1fd7, 0x0800}	//xvYCC rgb2yuv
};
/*
YUV使能输出
mode模式
*/
void YUV_OUT_Enable(char mode)
{
	int i;
	for (i = 0x950; i <= 0x97c; i += 4)
		write32(DEBE_Base_Address + i, csc_tab2[mode][(i - 0x950) / 4] << 16);

	S_BIT(DEBE_Base_Address + 0x800, 5);
}
/*
TCON设置
*/
void Tcon_Init(struct fb_f1c100s_pdata_t *pdat)
{
	u8 F;

#ifdef LCD_TYPE_Vga_1024_768
	int ht = 1344;	//HT-1=1344-1
	int hbp = 296;	//HS+HBP-1=136+160-1
	int vt = 1612;	//VT*2=806*2
	int vbp = 35;		//VS+VBP-1=6+29-1
	int vspw = 6;		//VS-1=6-1
	int hspw = 136; //HS-1=136-1
#endif

#ifdef LCD_TYPE_Vga_640_480_75HZ //75HZ
	int ht = 840;									 //HT-1=-1
	int hbp = 120 + 64;						 //HS+HBP-1=+-1 【同步脉冲+后沿】
	int vt = 500 * 2;							 //VT*2=*2
	int vbp = 16 + 3;							 //VS+VBP-1=+-1【同步脉冲+后沿】

	int vspw = 3;	 //VS-1=-1
	int hspw = 64; //HS-1=-1
#endif

#ifdef LCD_TYPE_Vga_640_480_60HZ //25090909    , 1      ,	25090909,  		276000000
	int ht = 800;									 //HT-1=-1
	int hbp = 48 + 96;						 //HS+HBP-1=+-1 【同步脉冲+后沿】
	int vt = 525 * 2;							 //VT*2=*2
	int vbp = 33 + 2;							 //VS+VBP-1=+-1【同步脉冲+后沿】

	int vspw = 2;	 //VS-1=-1
	int hspw = 96; //HS-1=-1
#endif

#if (defined LCD_TYPE_RGB43_480_272) || (defined LCD_TYPE_RGB43_800_480)
	int bp, total;
#endif
	struct f1c100s_tcon_reg_t *tcon = ((struct f1c100s_tcon_reg_t *)pdat->virttcon);
	u32_t val;

	//-----------------------------------------计算除数-------------------------------------------------
	//-----------------------------------------计算除数-------------------------------------------------
//	float N1,M1,F1;
//	for(N1=1;N1<=128;N1++)
//	for(M1=1;M1<=16;M1++)
//	for(F1=6;F1<=95;F1++)

//if((24.0*N1/M1)==276.0)sysprintf("N=%d M=%d F=%d\r\n",(int)N1,(int)M1,(int)F1);

//-----------------------------------------VGA 60HZ 设置Vedio到65MHZ-------------------------------------------------
#ifdef LCD_TYPE_Vga_1024_768
	u8 N = 65, M = 4;
	F = 6; //(24MHz*N)/M/F=65MHZ
	C_BIT(CCU_Base_Address + 0x010, 31);
	write32(CCU_Base_Address + 0x010, ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24));
	S_BIT(CCU_Base_Address + 0x010, 31);
	delay_ms(1);
#endif
//-----------------------------------------VGA 75HZ 设置Vedio到31.5MHZ-------------------------------------------------
#ifdef LCD_TYPE_Vga_640_480_75HZ //75HZ
	u8 N = 63, M = 8;
	F = 6;
	C_BIT(CCU_Base_Address + 0x010, 31);
	write32(CCU_Base_Address + 0x010, ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24));
	S_BIT(CCU_Base_Address + 0x010, 31);
	delay_ms(1);
#endif

#ifdef LCD_TYPE_Vga_640_480_60HZ //60HZ
	u8 N = 23, M = 2;
	F = 11; //LCD=276000000/11=25090909
	C_BIT(CCU_Base_Address + 0x010, 31);
	write32(CCU_Base_Address + 0x010, ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24));
	S_BIT(CCU_Base_Address + 0x010, 31);
	delay_ms(1);
#endif

#ifdef LCD_TYPE_RGB43_480_272
	//设置video时钟到390MHZ
	u8 N = 65, M = 4;
	F = 25; //(24MHz*N)/M/F=15.6MHZ
	C_BIT(CCU_Base_Address + 0x010, 31);
	write32(CCU_Base_Address + 0x010, ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24));
	S_BIT(CCU_Base_Address + 0x010, 31);
	delay_ms(1);
#endif

#ifdef LCD_TYPE_RGB43_800_480
	//设置video时钟到390MHZ
	u8 N = 65, M = 4;
	F = 9; //(24MHz*N)/M/F=43.33MHZ
	C_BIT(CCU_Base_Address + 0x010, 31);
	write32(CCU_Base_Address + 0x010, ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24));
	S_BIT(CCU_Base_Address + 0x010, 31);
	delay_ms(1);
#endif

	//-----------------------------------------TCON时钟与复位-------------------------------------------------
	//	sysprintf("VEdoi=%08x\r\n",read32(CCU_Base_Address+0x10));
	//使能LCD时钟
	write32(CCU_BUS_CLK_GATING_REG1, read32(CCU_BUS_CLK_GATING_REG1) | (1) << 4);
	//	//使能TCON时钟
	write32(CCU_TCON_CLK_REG, read32(CCU_TCON_CLK_REG) | (u64_t)(1) << 31);
	//使能LCD复位
	write32(CCU_BUS_SOFT_RST_REG1, read32(CCU_BUS_SOFT_RST_REG1) | ((1) << 4));
	delay_ms(1);

	//-----------------------------------------TCON相关设置-------------------------------------------------
	//设置为TCON0
	val = read32((virtual_addr_t)&tcon->ctrl);
	val &= ~(0x1 << 0);
	write32((virtual_addr_t)&tcon->ctrl, val);
	//使能TCON 31
	val = (pdat->timing.v_front_porch + pdat->timing.v_back_porch + pdat->timing.v_sync_len);
	write32((virtual_addr_t)&tcon->tcon0_ctrl, ((u64_t)0x1 << 31) | (val & 0x1f) << 4);
	//设置时钟
	val = F; // 5< DCLKDIV <96
	write32((virtual_addr_t)&tcon->tcon0_dclk, ((u64_t)0xf << 28) | (val << 0));

#if (defined LCD_TYPE_Vga_640_480_60HZ) || (defined LCD_TYPE_Vga_640_480_75HZ) || (defined LCD_TYPE_Vga_1024_768)
	//设置宽高
	write32((virtual_addr_t)&tcon->tcon0_timing_active, ((pdat->width - 1) << 16) | ((pdat->height - 1) << 0));
	//设置HT+HBP
	write32((virtual_addr_t)&tcon->tcon0_timing_h, ((ht - 1) << 16) | ((hbp - 1) << 0));
	//设置VT+VBp
	write32((virtual_addr_t)&tcon->tcon0_timing_v, ((vt) << 16) | ((vbp - 1) << 0));

	write32((virtual_addr_t)&tcon->tcon0_timing_sync, ((hspw - 1) << 16) | ((vspw - 1) << 0));
#endif

#if (defined LCD_TYPE_RGB43_480_272) || (defined LCD_TYPE_RGB43_800_480)
	//设置宽高
	write32((virtual_addr_t)&tcon->tcon0_timing_active, ((pdat->width - 1) << 16) | ((pdat->height - 1) << 0));
	//设置HT+HBP
	bp = pdat->timing.h_sync_len + pdat->timing.h_back_porch;
	total = pdat->width + pdat->timing.h_front_porch + bp;
	write32((virtual_addr_t)&tcon->tcon0_timing_h, ((total - 1) << 16) | ((bp - 1) << 0));
	//设置VT+VBP
	bp = pdat->timing.v_sync_len + pdat->timing.v_back_porch;
	total = pdat->height + pdat->timing.v_front_porch + bp;
	write32((virtual_addr_t)&tcon->tcon0_timing_v, ((total * 2) << 16) | ((bp - 1) << 0));
	//设置时钟宽度
	write32((virtual_addr_t)&tcon->tcon0_timing_sync, ((pdat->timing.h_sync_len - 1) << 16) | ((pdat->timing.v_sync_len - 1) << 0));
#endif

	//设置模式
	if (pdat->mode == 1) //rgb
	{
		write32((virtual_addr_t)&tcon->tcon0_hv_intf, 0);
		write32((virtual_addr_t)&tcon->tcon0_cpu_intf, 0);
	}
	else //cpu
	{
		//设置为8080模式
		write32(TCON_Base_Address + 0x40, read32(TCON_Base_Address + 0x40) | (1) << 24);
		//设置输入源
		write32(TCON_Base_Address + 0x40, read32(TCON_Base_Address + 0x40) | (0) << 0); //[3-白色数据][2-DMA][0-DE]
		//设置为16位模式+自动
		write32(TCON_Base_Address + 0x60, (u64_t)(4) << 29 | (u64_t)(1) << 28);
	}
	//FRM
	if (pdat->bits_per_pixel == 18 || pdat->bits_per_pixel == 16)
	{
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[0], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[1], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[2], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[3], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[4], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[5], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[0], 0x01010000);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[1], 0x15151111);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[2], 0x57575555);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[3], 0x7f7f7777);
		write32((virtual_addr_t)&tcon->tcon0_frm_ctrl, (pdat->bits_per_pixel == 18) ? (((u64_t)1 << 31) | (0 << 4)) : (((u64_t)1 << 31) | (5 << 4)));
	}
	//极性控制
	val = (1 << 28);
	if (!pdat->timing.h_sync_active)
		val |= (1 << 25);
	if (!pdat->timing.v_sync_active)
		val |= (1 << 24);
	if (!pdat->timing.den_active)
		val |= (1 << 27);
	if (!pdat->timing.clk_active)
		val |= (1 << 26);
	write32((virtual_addr_t)&tcon->tcon0_io_polarity, val);
	//触发控制关
	write32((virtual_addr_t)&tcon->tcon0_io_tristate, 0);
	//
	irq_register(IRQ_LEVEL_1, F1C100S_IRQ_LCD, TCON_ISR, 3);
	// IRQ_Init(IRQ_LEVEL_1,IRQ_TCON,TCON_ISR ,3);
	write32(TCON_Base_Address + 0x08, (pdat->height) << 16); //set line
	S_BIT(TCON_Base_Address + 0x04, 29);										 //TCON0 line trigger interrupt enable
																													 // sysSetLocalInterrupt(ENABLE_IRQ);												 //开IRQ中断
}

void Debe_Init(struct fb_f1c100s_pdata_t *pdat)
{
	struct f1c100s_debe_reg_t *debe = (struct f1c100s_debe_reg_t *)pdat->virtdebe;
	//	u32_t val;
	//-----------------------------------------DEBE时钟与复位-------------------------------------------------
	//使能DEBE时钟
	write32(CCU_BUS_CLK_GATING_REG1, read32(CCU_BUS_CLK_GATING_REG1) | (1) << 12);
	//使能DEBE复位
	write32(CCU_BUS_SOFT_RST_REG1, read32(CCU_BUS_SOFT_RST_REG1) | ((1) << 12));
	delay_ms(1);

	//使能DEBE
	S_BIT((virtual_addr_t)&debe->mode, 0);
	//设置层0参数
	write32((virtual_addr_t)&debe->disp_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
	write32((virtual_addr_t)&debe->layer0_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
	write32((virtual_addr_t)&debe->layer0_stride, ((pdat->width) << 4));
	write32((virtual_addr_t)&debe->layer0_addr_low32b, (u32)(pdat->vram[0]) << 3);
	write32((virtual_addr_t)&debe->layer0_addr_high4b, (u32)(pdat->vram[0]) >> 29);
	write32((virtual_addr_t)&debe->layer0_attr1_ctrl, 0x05 << 8); //层0属性 5=RGB565

	S_BIT((virtual_addr_t)&debe->mode, 8); //层0使能
																				 ////设置层1参数
																				 //	write32((virtual_addr_t)&debe->disp_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
																				 //	write32((virtual_addr_t)&debe->layer1_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
																				 //	write32((virtual_addr_t)&debe->layer1_stride, ((pdat->width) << 4));
																				 //	write32((virtual_addr_t)&debe->layer1_addr_low32b, (u32)(pdat->vram[1]) << 3);
																				 //	write32((virtual_addr_t)&debe->layer1_addr_high4b, (u32)(pdat->vram[1]) >> 29);
																				 //	write32((virtual_addr_t)&debe->layer1_attr1_ctrl, 0x0A << 8);//层0属性 5=RGB565
																				 //	//
																				 //	S_BIT((virtual_addr_t)&debe->layer1_attr0_ctrl,10);//优先级
																				 //  S_BIT((virtual_addr_t)&debe->layer1_attr0_ctrl,15);//1: select Pipe 1
																				 //	//
																				 //	S_BIT((virtual_addr_t)&debe->mode,9);//层1使能

	//加载
	S_BIT((virtual_addr_t)&debe->reg_ctrl, 0);
	//DEBE 开始
	S_BIT((virtual_addr_t)&debe->mode, 1);
}

void f1c100s_debe_set_address(struct fb_f1c100s_pdata_t *pdat, void *vram)
{
	struct f1c100s_debe_reg_t *debe = (struct f1c100s_debe_reg_t *)pdat->virtdebe;

	write32((virtual_addr_t)&debe->layer0_addr_low32b, (u32_t)vram << 3);
	write32((virtual_addr_t)&debe->layer0_addr_high4b, (u32_t)vram >> 29);
}

void f1c100s_tcon_enable(struct fb_f1c100s_pdata_t *pdat)
{
	struct f1c100s_tcon_reg_t *tcon = (struct f1c100s_tcon_reg_t *)pdat->virttcon;
	u32_t val;

	val = read32((virtual_addr_t)&tcon->ctrl);
	val |= ((u64_t)1 << 31);
	write32((virtual_addr_t)&tcon->ctrl, val);
}

void f1c100s_tcon_disable(struct fb_f1c100s_pdata_t *pdat)
{
	struct f1c100s_tcon_reg_t *tcon = (struct f1c100s_tcon_reg_t *)pdat->virttcon;
	u32_t val;

	write32((virtual_addr_t)&tcon->ctrl, 0);
	write32((virtual_addr_t)&tcon->int0, 0);

	val = read32((virtual_addr_t)&tcon->tcon0_dclk);
	val &= ~((u64_t)0xf << 28);
	write32((virtual_addr_t)&tcon->tcon0_dclk, val);

	write32((virtual_addr_t)&tcon->tcon0_io_tristate, 0xffffffff);
	write32((virtual_addr_t)&tcon->tcon1_io_tristate, 0xffffffff);
}

//全局结构体
struct fb_f1c100s_pdata_t *lcd_pdat;
struct fb_f1c100s_pdata_t f1;
//
void F1C100S_LCD_Init(int width, int height, unsigned int *buff1, unsigned int *buff2)
{
	int i;
	lcd_pdat = &f1;
	//	memset(&f1,0,sizeof(struct fb_f1c100s_pdata_t));
	//
	lcd_pdat->virtdebe = F1C100S_DEBE_BASE;
	lcd_pdat->virttcon = F1C100S_TCON_BASE;
	/*********************************TCON SET**************************/
	//LCD模式 0-cpu 1-rgb
	lcd_pdat->mode = 1;
	//LCD宽高
	lcd_pdat->width = width;
	lcd_pdat->height = height;
	//像素位宽
	lcd_pdat->bits_per_pixel = 18;

	//时序
	lcd_pdat->timing.h_front_porch = 40; //
	lcd_pdat->timing.h_back_porch = 87;	 //
	lcd_pdat->timing.h_sync_len = 1;		 //HSPW
	lcd_pdat->timing.v_front_porch = 13; //
	lcd_pdat->timing.v_back_porch = 31;	 //
	lcd_pdat->timing.v_sync_len = 1;		 //VSPW
	//极性设置 0反相
	lcd_pdat->timing.h_sync_active = 0;
	lcd_pdat->timing.v_sync_active = 0;
	lcd_pdat->timing.den_active = 1; //反相
	lcd_pdat->timing.clk_active = 0;
	/*********************************DEBE SET**************************/
	lcd_pdat->index = 0;					 //FB索引
	lcd_pdat->bytes_per_pixel = 4; //buff 色位宽
	lcd_pdat->vram[0] = buff1;
	lcd_pdat->vram[1] = buff2;

	/*********************************初始化**************************/
	for (i = 0x0800; i < 0x1000; i += 4)
		write32(F1C100S_DEBE_BASE + i, 0);
	//
	f1c100s_tcon_disable(lcd_pdat);
	Debe_Init(lcd_pdat);

	Tcon_Init(lcd_pdat);

#ifdef LCD_TYPE_TV_PAL_720_576
	TVE_Init();
	YUV_OUT_Enable(0);
#endif

	f1c100s_tcon_enable(lcd_pdat);
}

//LCD IO初始化
void LCD_IO_Init(void)
{
	int i = 0;
	for (i = 0; i <= 21; i++)
	{
		gpio_set_cfg(GPIOD, i, GPIO_FUNC_010);
		gpio_set_pull(GPIOD, i, GPIO_PULL_NONE);
		gpio_set_drv(GPIOD, i, GPIO_DRV_STRONG);
	}
	// GPIO_Congif(GPIOD, i, GPIO_Mode_010, GPIO_PuPd_NOPULL);
}

/*
LCD初始化
*/

void Sys_LCD_Init(int width, int height, unsigned int *buff1, unsigned int *buff2)
{
	LCD_IO_Init();																 //IO初始化
	F1C100S_LCD_Init(width, height, buff1, buff2); //TCON DEBE 初始化
}
