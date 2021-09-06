#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <arm32.h>
#include <io.h>
#include <reg-ccu.h>
#include <irq.h>
#include <printf.h>

static void F1C100S_Interrupt_Shell() {} /* Empty function */
sys_pvFunPtr sysIrqHandlerTable[] = {
		0,											 /* 0 */
		F1C100S_Interrupt_Shell, /* 1 */
		F1C100S_Interrupt_Shell, /* 2 */
		F1C100S_Interrupt_Shell, /* 3 */
		F1C100S_Interrupt_Shell, /* 4 */
		F1C100S_Interrupt_Shell, /* 5 */
		F1C100S_Interrupt_Shell, /* 6 */
		F1C100S_Interrupt_Shell, /* 7 */
		F1C100S_Interrupt_Shell, /* 8 */
		F1C100S_Interrupt_Shell, /* 9 */
		F1C100S_Interrupt_Shell, /* 10 */
		F1C100S_Interrupt_Shell, /* 11 */
		F1C100S_Interrupt_Shell, /* 12 */
		F1C100S_Interrupt_Shell, /* 13 */
		F1C100S_Interrupt_Shell, /* 14 */
		F1C100S_Interrupt_Shell, /* 15 */
		F1C100S_Interrupt_Shell, /* 16 */
		F1C100S_Interrupt_Shell, /* 17 */
		F1C100S_Interrupt_Shell, /* 18 */
		F1C100S_Interrupt_Shell, /* 19 */
		F1C100S_Interrupt_Shell, /* 20 */
		F1C100S_Interrupt_Shell, /* 21 */
		F1C100S_Interrupt_Shell, /* 22 */
		F1C100S_Interrupt_Shell, /* 23 */
		F1C100S_Interrupt_Shell, /* 24 */
		F1C100S_Interrupt_Shell, /* 25 */
		F1C100S_Interrupt_Shell, /* 26 */
		F1C100S_Interrupt_Shell, /* 27 */
		F1C100S_Interrupt_Shell, /* 28 */
		F1C100S_Interrupt_Shell, /* 29 */
		F1C100S_Interrupt_Shell, /* 30 */
		F1C100S_Interrupt_Shell, /* 31 */
		F1C100S_Interrupt_Shell, /* 32 */
		F1C100S_Interrupt_Shell, /* 33 */
		F1C100S_Interrupt_Shell, /* 34 */
		F1C100S_Interrupt_Shell, /* 35 */
		F1C100S_Interrupt_Shell, /* 36 */
		F1C100S_Interrupt_Shell, /* 37 */
		F1C100S_Interrupt_Shell, /* 38 */
		F1C100S_Interrupt_Shell, /* 39 */
		F1C100S_Interrupt_Shell	 /* 40 */

};

sys_pvFunPtr sysFiqHandlerTable[] = {
		0,
		F1C100S_Interrupt_Shell, /* 1 */
		F1C100S_Interrupt_Shell, /* 2 */
		F1C100S_Interrupt_Shell, /* 3 */
		F1C100S_Interrupt_Shell, /* 4 */
		F1C100S_Interrupt_Shell, /* 5 */
		F1C100S_Interrupt_Shell, /* 6 */
		F1C100S_Interrupt_Shell, /* 7 */
		F1C100S_Interrupt_Shell, /* 8 */
		F1C100S_Interrupt_Shell, /* 9 */
		F1C100S_Interrupt_Shell, /* 10 */
		F1C100S_Interrupt_Shell, /* 11 */
		F1C100S_Interrupt_Shell, /* 12 */
		F1C100S_Interrupt_Shell, /* 13 */
		F1C100S_Interrupt_Shell, /* 14 */
		F1C100S_Interrupt_Shell, /* 15 */
		F1C100S_Interrupt_Shell, /* 16 */
		F1C100S_Interrupt_Shell, /* 17 */
		F1C100S_Interrupt_Shell, /* 18 */
		F1C100S_Interrupt_Shell, /* 19 */
		F1C100S_Interrupt_Shell, /* 20 */
		F1C100S_Interrupt_Shell, /* 21 */
		F1C100S_Interrupt_Shell, /* 22 */
		F1C100S_Interrupt_Shell, /* 23 */
		F1C100S_Interrupt_Shell, /* 24 */
		F1C100S_Interrupt_Shell, /* 25 */
		F1C100S_Interrupt_Shell, /* 26 */
		F1C100S_Interrupt_Shell, /* 27 */
		F1C100S_Interrupt_Shell, /* 28 */
		F1C100S_Interrupt_Shell, /* 29 */
		F1C100S_Interrupt_Shell, /* 30 */
		F1C100S_Interrupt_Shell, /* 31 */
		F1C100S_Interrupt_Shell, /* 32 */
		F1C100S_Interrupt_Shell, /* 33 */
		F1C100S_Interrupt_Shell, /* 34 */
		F1C100S_Interrupt_Shell, /* 35 */
		F1C100S_Interrupt_Shell, /* 36 */
		F1C100S_Interrupt_Shell, /* 37 */
		F1C100S_Interrupt_Shell, /* 38 */
		F1C100S_Interrupt_Shell, /* 39 */
		F1C100S_Interrupt_Shell	 /* 40 */
};

void irq_enable(int32_t eIntNo)
{
	int en;
	arm32_interrupt_disable();
	if (eIntNo > 63)
	{
		printf("irq number too large");
	}
	else if (eIntNo >= 48)
	{
		en = eIntNo - 32;
		S_Bit(INTC->INTC_EN_REG1, en); //Write interrupt enable
	}
	else if (eIntNo >= 32)
	{
		en = eIntNo - 32;
		S_Bit(INTC->INTC_EN_REG1, en); //Write interrupt enable
	}
	else if (eIntNo >= 16)
	{
		en = eIntNo - 0;
		S_Bit(INTC->INTC_EN_REG0, en); //Write interrupt enable
	}
	else if (eIntNo >= 1)
	{
		en = eIntNo - 0;
		S_Bit(INTC->INTC_EN_REG0, en); //Write interrupt enable
	}
	arm32_interrupt_enable();
}

void irq_disable(int32_t eIntNo)
{
	int en;
	arm32_interrupt_disable();
	if (eIntNo > 63)
	{
		printf("irq number too large");
	}
	else if (eIntNo >= 48)
	{
		en = eIntNo - 32;
		C_Bit(INTC->INTC_EN_REG1, en); //Write interrupt enable
	}
	else if (eIntNo >= 32)
	{
		en = eIntNo - 32;
		C_Bit(INTC->INTC_EN_REG1, en); //Write interrupt enable
	}
	else if (eIntNo >= 16)
	{
		en = eIntNo - 0;
		C_Bit(INTC->INTC_EN_REG0, en); //Write interrupt enable
	}
	else if (eIntNo >= 1)
	{
		en = eIntNo - 0;
		C_Bit(INTC->INTC_EN_REG0, en); //Write interrupt enable
	}
	arm32_interrupt_enable();
}

void irq_register(int32_t nIntTypeLevel, int32_t eIntNo, sys_pvFunPtr pvNewISR, uint8_t Priority)
{
	int prio;
	int en;
	// void *_mOldVect;
	switch (nIntTypeLevel)
	{
	case FIQ_LEVEL_0:
		// _mOldVect = (void *)sysFiqHandlerTable[eIntNo];
		sysFiqHandlerTable[eIntNo] = pvNewISR;
		break;

	case IRQ_LEVEL_1:
	case IRQ_LEVEL_2:
	case IRQ_LEVEL_3:
	case IRQ_LEVEL_4:
	case IRQ_LEVEL_5:
	case IRQ_LEVEL_6:
	case IRQ_LEVEL_7:
		// _mOldVect = (void *)sysIrqHandlerTable[eIntNo];
		sysIrqHandlerTable[eIntNo] = pvNewISR;
		break;

	default:;
	}

	arm32_interrupt_disable();
	if (eIntNo > 63)
	{
		printf("irq number too large");
	}
	else if (eIntNo >= 48)
	{
		prio = eIntNo - 48;
		en = eIntNo - 32;
		C_Value(INTC->INTC_PRIO_REG3, prio * 2, 0x3);			 //Clear priority
		S_Value(INTC->INTC_PRIO_REG3, prio * 2, Priority); //Write priority
		C_Bit(INTC->INTC_PEND_REG1, en);
		C_Bit(INTC->INTC_MASK_REG1, en);
		S_Bit(INTC->INTC_EN_REG1, en); //Write interrupt enable
	}
	else if (eIntNo >= 32)
	{
		prio = eIntNo - 32;
		en = eIntNo - 32;
		C_Value(INTC->INTC_PRIO_REG2, prio * 2, 0x3);			 //Clear priority
		S_Value(INTC->INTC_PRIO_REG2, prio * 2, Priority); //Write priority
		C_Bit(INTC->INTC_PEND_REG1, en);
		C_Bit(INTC->INTC_MASK_REG1, en);
		S_Bit(INTC->INTC_EN_REG1, en); //Write interrupt enable
	}
	else if (eIntNo >= 16)
	{
		prio = eIntNo - 16;
		en = eIntNo - 0;
		C_Value(INTC->INTC_PRIO_REG1, prio * 2, 0x3);			 //Clear priority
		S_Value(INTC->INTC_PRIO_REG1, prio * 2, Priority); //Write priority
		C_Bit(INTC->INTC_PEND_REG0, en);
		C_Bit(INTC->INTC_MASK_REG0, en);
		S_Bit(INTC->INTC_EN_REG0, en); //Write interrupt enable
	}
	else if (eIntNo >= 1)
	{
		prio = eIntNo - 0;
		en = eIntNo - 0;
		C_Value(INTC->INTC_PRIO_REG0, prio * 2, 0x3);			 //Clear priority
		S_Value(INTC->INTC_PRIO_REG0, prio * 2, Priority); //Write priority
		C_Bit(INTC->INTC_PEND_REG0, en);
		C_Bit(INTC->INTC_MASK_REG0, en);
		S_Bit(INTC->INTC_EN_REG0, en); //Write interrupt enable
	}
	arm32_interrupt_enable();
	// return _mOldVect;
}

int32_t irq_handle()
{
	// printf("irq_handle: %d\r\n", INTC->INTC_VECTOR_REG >> 2);
	uint32_t pos = INTC->INTC_VECTOR_REG >> 2;
	if (pos > 0)
	{
		(*sysIrqHandlerTable[pos])();
	}
	return 0;
	// u64_t volatile Iid = 0;
	// int32_t i = 0;
	// //Read interrupt number
	// Iid = INTC->INTC_PEND_REG0 | (u64_t)(INTC->INTC_PEND_REG1 << 32);
	// INTC->INTC_PEND_REG0 = 0;
	// INTC->INTC_PEND_REG1 = 0;
	// // printf("Iid: %d, irq_handle: %d\r\n", Iid, INTC->INTC_VECTOR_REG >> 2);
	// for (i = 1; i < 64; i++)
	//     if (((Iid >> i) & 0x1) == 0x1)
	//     {
	//         //Execution interrupt number
	//         (*sysIrqHandlerTable[i])();
	//         break;
	//         // return i;
	//     }
	// return 0;
}

void irq_init()
{
	INTC->INTC_BASE_ADDR_REG = 0x00000000;
	INTC->INTC_EN_REG0 = 0x00000000;
	INTC->INTC_EN_REG1 = 0x00000000;
	INTC->INTC_MASK_REG0 = 0xFFFFFFFF;
	INTC->INTC_MASK_REG1 = 0xFFFFFFFF;
	INTC->INTC_PEND_REG0 = 0x00000000;
	INTC->INTC_PEND_REG1 = 0x00000000;
	INTC->INTC_RESP_REG0 = 0x00000000;
	INTC->INTC_RESP_REG1 = 0x00000000;
	INTC->INTC_FF_REG0 = 0x00000000;
	INTC->INTC_FF_REG1 = 0x00000000;
}

void arm32_do_irq(struct arm_regs_t *regs)
{
	irq_handle();
	// vPreemptiveTick();
}

void arm32_do_fiq(struct arm_regs_t *regs)
{
	irq_handle();
	// vPreemptiveTick();
}
