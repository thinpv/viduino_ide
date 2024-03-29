USR_MODE		=	0x10
FIQ_MODE		=	0x11
IRQ_MODE		=	0x12
SVC_MODE		=	0x13
ABT_MODE		=	0x17
UDF_MODE		=	0x1B
SYS_MODE		=	0x1F
       
I_BIT				=	0x80        /* Disables IRQ when I bit is set               */
F_BIT				=	0x40        /* Disables FIQ when F bit is set               */

RAM_Limit				=	0x80000000+0x2000000            // For unexpanded hardware board

USR_Stack				= RAM_Limit
SVC_Stack				= (USR_Stack-1024*50)      /* SVC stack at top of memory */
FIQ_Stack				= (SVC_Stack-1024*50)      /* followed by IRQ stack */
IRQ_Stack				= (FIQ_Stack-1024*50)
Abort_Stack			= (IRQ_Stack-1024*100)
UND_Stack				= (Abort_Stack-1024*50)

.global _vector
.arm
.section .vectors, "ax"
.extern	 vPreemptiveTick
.extern  vPortYieldProcessor

_vector:
	ldr pc, _reset
	ldr pc, _undefined_instruction
	ldr pc, _software_interrupt
	ldr pc, _prefetch_abort
	ldr pc, _data_abort
	ldr pc, _not_used
	ldr pc, _irq
	ldr pc, _fiq

_reset:
	.word reset
_undefined_instruction:
	.word undefined_instruction
_software_interrupt:
	.word vPortYieldProcessor
_prefetch_abort:
	.word prefetch_abort
_data_abort:
	.word data_abort
_not_used:
	.word not_used
_irq:
	.word vPreemptiveTick
_fiq:
	.word fiq

reset:
	/* Enter svc mode and mask interrupts */
	mrs r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0xd3
	msr cpsr, r0

	/* Set vector to the low address */
	mrc p15, 0, r0, c1, c0, 0
	bic r0, #(1 << 13)
	mcr p15, 0, r0, c1, c0, 0

	/* Copy vector to the correct address */
	adr r0, _vector
	mrc p15, 0, r2, c1, c0, 0
	ands r2, r2, #(1 << 13)
	ldreq r1, =0x00000000
	ldrne r1, =0xffff0000	
	ldmia r0!, {r2-r8, r10}
	stmia r1!, {r2-r8, r10}
	ldmia r0!, {r2-r8, r10}
	stmia r1!, {r2-r8, r10}

	MSR    CPSR_c, #UDF_MODE | I_BIT | F_BIT
	LDR    SP, =UND_Stack

	MSR    CPSR_c, #ABT_MODE | I_BIT | F_BIT
	LDR    SP, =Abort_Stack

	MSR    CPSR_c, #IRQ_MODE | I_BIT | F_BIT
	LDR    SP, =IRQ_Stack

	MSR    CPSR_c, #FIQ_MODE | I_BIT | F_BIT
	LDR    SP, =FIQ_Stack

	MSR    CPSR_c, #SYS_MODE | I_BIT | F_BIT
	LDR    SP, =USR_Stack

	MSR    CPSR_c, #SVC_MODE | I_BIT | F_BIT
	LDR    SP, =SVC_Stack

	/* Clear bss section */
	ldr r0, _bss_start
	ldr r2, _bss_end
	sub r2, r2, r0
	mov r1, #0
	bl memset

	/* Call _main */
	bl boot_main

/*
 * Exception handlers
 */
	.align 5
undefined_instruction:
	b .

	.align 5
software_interrupt:
	b vPortYieldProcessor

	.align 5
prefetch_abort:
	b .

	.align 5
data_abort:
	b .

	.align 5
not_used:
	b .

	.align 5
irq:
	b vPreemptiveTick

	.align 5
fiq:
	b .

/*
 * The location of section
 */
 	.align 4
_image_start:
	.long __image_start
_image_end:
	.long __image_end
_data_start:
	.long __data_start
_data_end:
	.long __data_end
_bss_start:
	.long __bss_start
_bss_end:
	.long __bss_end
_stack_und_end:
	.long __stack_und_end
_stack_abt_end:
	.long __stack_abt_end
_stack_irq_end:
	.long __stack_irq_end
_stack_fiq_end:
	.long __stack_fiq_end
_stack_srv_end:
	.long __stack_srv_end
