#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <arm32.h>
#include <types.h>
#include <io.h>
#include <reg-ccu.h>
#include <irq.h>
//#include <sys_print.h>

#define DEBUG(...) // printf(##__VA_ARGS__)

#ifdef __UVISION_VERSION
void arm32_interrupt_enable()
{
	INT32 temp;
	__asm
	{
		 MRS    temp, CPSR
		 AND    temp, temp, ENABLE_IRQ
		 MSR    CPSR_c, temp
	}
}

void arm32_interrupt_disable()
{
	INT32 temp;
	__asm
	{
		 MRS    temp, CPSR
		 ORR    temp, temp, DISABLE_IRQ
		 MSR    CPSR_c, temp
	}
}
#endif

static void F1C100S_Interrupt_Shell()
{
} /* Empty function */
sys_pvFunPtr irqHandlerTable[] = {
	0,						 /* 0 */
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

sys_pvFunPtr fiqHandlerTable[] = {
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
	// arm32_interrupt_disable();
	if (eIntNo > 63)
	{
		DEBUG("irq number too large");
	}
	else if (eIntNo >= 48)
	{
		en = eIntNo - 32;
		S_Bit(INTC->INTC_EN_REG1, en); // Write interrupt enable
	}
	else if (eIntNo >= 32)
	{
		en = eIntNo - 32;
		S_Bit(INTC->INTC_EN_REG1, en); // Write interrupt enable
	}
	else if (eIntNo >= 16)
	{
		en = eIntNo - 0;
		S_Bit(INTC->INTC_EN_REG0, en); // Write interrupt enable
	}
	else if (eIntNo >= 1)
	{
		en = eIntNo - 0;
		S_Bit(INTC->INTC_EN_REG0, en); // Write interrupt enable
	}
	// arm32_interrupt_enable();
}

void irq_disable(int32_t eIntNo)
{
	int en;
	// arm32_interrupt_disable();
	if (eIntNo > 63)
	{
		DEBUG("irq number too large");
	}
	else if (eIntNo >= 48)
	{
		en = eIntNo - 32;
		C_Bit(INTC->INTC_EN_REG1, en); // Write interrupt enable
	}
	else if (eIntNo >= 32)
	{
		en = eIntNo - 32;
		C_Bit(INTC->INTC_EN_REG1, en); // Write interrupt enable
	}
	else if (eIntNo >= 16)
	{
		en = eIntNo - 0;
		C_Bit(INTC->INTC_EN_REG0, en); // Write interrupt enable
	}
	else if (eIntNo >= 1)
	{
		en = eIntNo - 0;
		C_Bit(INTC->INTC_EN_REG0, en); // Write interrupt enable
	}
	// arm32_interrupt_enable();
}

void irq_register(int32_t nIntTypeLevel, int32_t eIntNo, sys_pvFunPtr pvNewISR, uint8_t Priority)
{
	int prio;
	int en;
	switch (nIntTypeLevel)
	{
	case FIQ_LEVEL_0:
		fiqHandlerTable[eIntNo] = pvNewISR;
		break;

	case IRQ_LEVEL_1:
	case IRQ_LEVEL_2:
	case IRQ_LEVEL_3:
	case IRQ_LEVEL_4:
	case IRQ_LEVEL_5:
	case IRQ_LEVEL_6:
	case IRQ_LEVEL_7:
		irqHandlerTable[eIntNo] = pvNewISR;
		break;

	default:;
	}

	// arm32_interrupt_disable();
	if (eIntNo > 63)
	{
		DEBUG("irq number too large");
	}
	else if (eIntNo >= 48)
	{
		prio = eIntNo - 48;
		en = eIntNo - 32;
		C_Value(INTC->INTC_PRIO_REG3, prio * 2, 0x3);	   // Clear priority
		S_Value(INTC->INTC_PRIO_REG3, prio * 2, Priority); // Write priority
		C_Bit(INTC->INTC_PEND_REG1, en);
		C_Bit(INTC->INTC_MASK_REG1, en);
		S_Bit(INTC->INTC_EN_REG1, en); // Write interrupt enable
	}
	else if (eIntNo >= 32)
	{
		prio = eIntNo - 32;
		en = eIntNo - 32;
		C_Value(INTC->INTC_PRIO_REG2, prio * 2, 0x3);	   // Clear priority
		S_Value(INTC->INTC_PRIO_REG2, prio * 2, Priority); // Write priority
		C_Bit(INTC->INTC_PEND_REG1, en);
		C_Bit(INTC->INTC_MASK_REG1, en);
		S_Bit(INTC->INTC_EN_REG1, en); // Write interrupt enable
	}
	else if (eIntNo >= 16)
	{
		prio = eIntNo - 16;
		en = eIntNo - 0;
		C_Value(INTC->INTC_PRIO_REG1, prio * 2, 0x3);	   // Clear priority
		S_Value(INTC->INTC_PRIO_REG1, prio * 2, Priority); // Write priority
		C_Bit(INTC->INTC_PEND_REG0, en);
		C_Bit(INTC->INTC_MASK_REG0, en);
		S_Bit(INTC->INTC_EN_REG0, en); // Write interrupt enable
	}
	else if (eIntNo >= 1)
	{
		prio = eIntNo - 0;
		en = eIntNo - 0;
		C_Value(INTC->INTC_PRIO_REG0, prio * 2, 0x3);	   // Clear priority
		S_Value(INTC->INTC_PRIO_REG0, prio * 2, Priority); // Write priority
		C_Bit(INTC->INTC_PEND_REG0, en);
		C_Bit(INTC->INTC_MASK_REG0, en);
		S_Bit(INTC->INTC_EN_REG0, en); // Write interrupt enable
	}
	// arm32_interrupt_enable();
}

int32_t irq_handle()
{
	uint32_t pos = ((INTC->INTC_VECTOR_REG - INTC->INTC_BASE_ADDR_REG) >> 2) & 0x3f;
	if (pos > 0)
	{
		(*irqHandlerTable[pos])();
		// f1c100s_intc_clear_fast_forcing(pos);
		f1c100s_intc_clear_pend(pos);
	}
	return 0;
}

int32_t swi_handle()
{
	return 0;
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

void f1c100s_intc_mask_irq(uint8_t nIRQ)
{
	if (nIRQ > 31)
	{
		S_Bit(INTC->INTC_MASK_REG1, nIRQ % 32);
	}
	else
	{
		S_Bit(INTC->INTC_MASK_REG0, nIRQ % 32);
	}
}

void f1c100s_intc_unmask_irq(uint8_t nIRQ)
{
	if (nIRQ > 31)
	{
		C_Bit(INTC->INTC_MASK_REG1, nIRQ % 32);
	}
	else
	{
		C_Bit(INTC->INTC_MASK_REG0, nIRQ % 32);
	}
}

void f1c100s_intc_clear_pend(uint8_t nIRQ)
{
	if (nIRQ > 31)
	{
		C_Bit(INTC->INTC_PEND_REG1, nIRQ % 32);
		// INTC->INTC_PEND_REG1 = (1 << (nIRQ % 32));
	}
	else
	{
		C_Bit(INTC->INTC_PEND_REG0, nIRQ % 32);
		// INTC->INTC_PEND_REG0 = (1 << (nIRQ % 32));
	}
}

void f1c100s_intc_clear_fast_forcing(uint8_t nIRQ)
{
	if (nIRQ > 31)
	{
		S_Bit(INTC->INTC_FF_REG1, nIRQ % 32);
		// INTC->INTC_FF_REG1 = (1 << (nIRQ % 32));
	}
	else
	{
		S_Bit(INTC->INTC_FF_REG0, nIRQ % 32);
		// INTC->INTC_FF_REG0 = (1 << (nIRQ % 32));
	}
}
