#ifndef __TVE_H__
#define __TVE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>
#include "fb.h"

#define __u8 unsigned char
#define __u16 unsigned short
#define __u32 unsigned int

#define __s8 char
#define __s32 int
#define __s64 long long

	typedef enum
	{
		DISP_TV_MOD_PAL_SVIDEO = 0,
		DISP_TV_MOD_PAL,
		DISP_TV_MOD_PAL_M,
		DISP_TV_MOD_PAL_M_SVIDEO,
		DISP_TV_MOD_PAL_NC,
		DISP_TV_MOD_PAL_NC_SVIDEO,
		DISP_TV_MOD_NTSC,
		DISP_TV_MOD_NTSC_SVIDEO,
		DISP_TV_MOD_480I,
		DISP_TV_MOD_576I,
		DISP_TV_MOD_480P,
		DISP_TV_MOD_576P,
		DISP_TV_MOD_720P_50HZ,
		DISP_TV_MOD_720P_60HZ,
		DISP_TV_MOD_1080I_50HZ,
		DISP_TV_MOD_1080I_60HZ,
		DISP_TV_MOD_1080P_50HZ,
		DISP_TV_MOD_1080P_60HZ,
	} __tve_mode_t;

	void TVE_Init(lcd_type_t lcd_type);
	__u8 TVE_dac_int_disable(__u8 index);
	__u8 TVE_dac_set_de_bounce(__u8 index, __u32 times);

#ifdef __cplusplus
}
#endif

#endif