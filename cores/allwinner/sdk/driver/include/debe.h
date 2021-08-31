#ifndef __FB_H__
#define __FB_H__

#include <F1C100S.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // typedef struct debe_layer
    // {
    //     uint32_t index;
    //     uint32_t height;
    //     uint32_t width;
    //     uint32_t pwidth;
    //     uint32_t pheight;
    // 	uint32_t bits_per_pixel;
    // 	uint32_t bytes_per_pixel;

    //     vuint32_t lay_size_reg;
    //     vuint32_t lay_codnt_reg;
    //     vuint32_t lay_linewidth_reg;
    //     vuint32_t lay_fb_addr_reg;
    //     vuint32_t lay_att_ctrl_reg0;
    //     vuint32_t lay_att_ctrl_reg1;
    // } debe_layer;

    enum FB_format
    {
        FB_FORMAT_MONO_1BPP = 0,
        FB_FORMAT_MONO_2BPP,
        FB_FORMAT_MONO_4BPP,
        FB_FORMAT_MONO_8BPP,
        FB_FORMAT_COLOR_16BPP_R6G5B5,
        FB_FORMAT_COLOR_16BPP_R5G6B5,
        FB_FORMAT_COLOR_16BPP_R5G5B6,
        FB_FORMAT_COLOR_16BPP_A1R5G5B5,
        FB_FORMAT_COLOR_16BPP_R5G5B5A1,
        FB_FORMAT_COLOR_32BPP_P8R8G8B8,
        FB_FORMAT_COLOR_32BPP_A8R8G8B8,
        FB_FORMAT_COLOR_24BPP_RGBR,
    };

    void debe_init(uint32_t width, uint32_t height, uint32_t *buffer);
    void debe_layer_enable(uint32_t index);
    void debe_layer_disable(uint32_t index);
    void debe_layer_init(uint32_t index, uint32_t width, uint32_t height, uint32_t pwidth, uint32_t pheight, uint32_t fb_format);
    void debe_layer_set_buffer_addr(uint32_t index, virtual_addr_t buffer_addr);

#ifdef __cplusplus
}
#endif

#endif /* __FB_H__ */