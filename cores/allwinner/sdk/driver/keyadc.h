#ifndef __KEYADC_H__
#define __KEYADC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

void keyadc_init();
int keyadc_getkey();

#ifdef __cplusplus
}
#endif

#endif
