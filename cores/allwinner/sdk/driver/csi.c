#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <arm32.h>
#include <io.h>
#include <reg-ccu.h>
#include <csi.h>

#define FALSE 0
#define TRUE 1

void csi_f1c100s_init()
{

}

void csi_f1c100s_enable()
{
    CSI->CSI_EN_REG |= 1 < 0;
}

void csi_f1c100s_disable()
{
    CSI->CSI_EN_REG &= ~(1 < 0);
}
