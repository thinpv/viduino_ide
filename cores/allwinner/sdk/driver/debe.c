#include <F1C100S.h>
#include <debe.h>
#include <io.h>
#include <ccu.h>

const uint32_t bpp[] = {1, 2, 4, 8, 16, 16, 16, 16, 16, 32, 32, 24};
// RGB to YUV coefficient
unsigned int csc_tab2[4][12] =
		{
				{0x0204, 0x0107, 0x0064, 0x0100, 0x1ed6, 0x1f68, 0x01c1, 0x0800, 0x1e87, 0x01c1, 0x1fb7, 0x0800}, //bt601 rgb2yuv
				{0x0274, 0x00bb, 0x003f, 0x0100, 0x1ea5, 0x1f98, 0x01c1, 0x0800, 0x1e67, 0x01c1, 0x1fd7, 0x0800}, //bt709 rgb2yuv
				{0x0258, 0x0132, 0x0075, 0x0000, 0x1eac, 0x1f53, 0x0200, 0x0800, 0x1e53, 0x0200, 0x1fac, 0x0800}, //DISP_YCC rgb2yuv
				{0x0274, 0x00bb, 0x003f, 0x0100, 0x1ea5, 0x1f98, 0x01c1, 0x0800, 0x1e67, 0x01c1, 0x1fd7, 0x0800}	//xvYCC rgb2yuv
};

void debe_enable()
{
	ccu_reset(RESET_DEBE, true);
	S_Bit(DEBE->DEBE_MODE_CTRL_REG, 0);
}

void debe_disable()
{
	ccu_reset(RESET_DEBE, false);
	C_Bit(DEBE->DEBE_MODE_CTRL_REG, 0);
}

void debe_init(uint32_t width, uint32_t height, uint32_t *buffer)
{
	for (int i = 0x0800; i < 0x1000; i += 4)
		write32(0x01e60000 + i, 0);
	DEBE->disp_size = ((height - 1) << 16) | ((width - 1) << 0);
	debe_layer_init(0, width, height, 0, 0, FB_FORMAT_COLOR_16BPP_R5G6B5);
	debe_layer_set_buffer_addr(0, (uint32_t)buffer);
	debe_layer_enable(0);
	S_Bit(DEBE->DEBE_MODE_CTRL_REG, 1);
}

void debe_layer_enable(uint32_t index)
{
	S_Bit(DEBE->DEBE_MODE_CTRL_REG, 8 + index);
}

void debe_layer_disable(uint32_t index)
{
	C_Bit(DEBE->DEBE_MODE_CTRL_REG, 8 + index);
}

void debe_layer_init(uint32_t index, uint32_t width, uint32_t height, uint32_t pwidth, uint32_t pheight, uint32_t fb_format)
{
	write32((virtual_addr_t)(&DEBE->DEBE_LAY0_SIZE_REG + index), ((height - 1) << 16) | ((width - 1) << 0));
	write32((virtual_addr_t)(&DEBE->DEBE_LAY0_CODNT_REG + index), (pheight << 16) | (pwidth << 0));
	write32((virtual_addr_t)(&DEBE->DEBE_LAY0_ATT_CTRL_REG1 + index), fb_format << 8);
	write32((virtual_addr_t)(&DEBE->DEBE_LAY0_LINEWIDTH_REG + index), width * bpp[fb_format]);
}

void debe_layer_set_buffer_addr(uint32_t index, virtual_addr_t buffer_addr)
{
	write32((virtual_addr_t)(&DEBE->DEBE_LAY0_FB_ADDR_REG + index), buffer_addr << 3);
	write32((virtual_addr_t)(&DEBE->layer0_addr_high4b + index), buffer_addr >> 29);
}

/*
YUV enable output
mode mode
*/
void debe_yuv_out_enable(char mode)
{
	for (int i = 0; i < sizeof(csc_tab2[0]) / 4; i++)
	{
		write32((virtual_addr_t)(&DEBE->DEBE_COEF00_REG + i), csc_tab2[mode][i]);
	}
	S_Bit(DEBE->DEBE_MODE_CTRL_REG, 5);
	// int i;
	// for (i = 0x950; i <= 0x97c; i += 4)
	// 	write32(DEBE_Base_Address + i, csc_tab2[mode][(i - 0x950) / 4] << 16);
	// S_BIT(DEBE_Base_Address + 0x800, 5);
}