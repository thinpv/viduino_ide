#include <F1C100S.h>
#include <debe.h>
#include <io.h>
#include <ccu.h>

// typedef struct debe_layer
//     {
//         uint32_t index;
//         uint32_t height;
//         uint32_t width;
//         uint32_t pwidth;
//         uint32_t pheight;
// 		uint32_t bits_per_pixel;
// 		uint32_t bytes_per_pixel;

//         vuint32_t lay_size_reg;
//         vuint32_t lay_codnt_reg;
//         vuint32_t lay_linewidth_reg;
//         vuint32_t lay_fb_addr_reg;
//         vuint32_t lay_att_ctrl_reg0;
//         vuint32_t lay_att_ctrl_reg1;
//     } debe_layer;

// debe_layer layer0 = {
//     .index = 0,
//     .height = }

const uint32_t bpp[] = {1, 2, 4, 8, 16, 16, 16, 16, 16, 32, 32, 24};

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
