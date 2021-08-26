
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

/*********************************************************************
*
*       Vector table
*
**********************************************************************
*/
.text
.global  __vector
.global  end
.extern  irq_handle
.extern  Reset_Handler
.extern	 vPreemptiveTick
.extern  vPortYieldProcessor

.arm
.section .vectors, "ax"

__vector:
	ldr     pc,Reset_Addr   /* RESET                 vector */
	ldr     pc,Undef_Addr   /* Undefined instruction vector */
	ldr     pc,SWI_Addr     /* Software interrupt    vector */
	ldr     pc,PAbt_Addr    /* Prefetch abort        vector */
	ldr     pc,DAbt_Addr    /* Data abort            vector */
	nop                     /* Reserved              vector */
	ldr     pc,IRQ_Addr     /* Interrupt             vector */
	ldr     pc,FIQ_Addr     /* Fast interrupt        vector */

Reset_Addr:     .word   Reset_Handler
Undef_Addr:     .word   Undef_Handler
SWI_Addr:       .word   SWI_Handler
PAbt_Addr:      .word   PAbt_Handler
DAbt_Addr:      .word   DAbt_Handler
ZeroAddr:       .word   0
IRQ_Addr:       .word   IRQ_Handler
FIQ_Addr:       .word   FIQ_Handler


#ifdef USE_FREERTOS
Undef_Handler:  b       Undef_Handler
SWI_Handler:    b       vPortYieldProcessor
PAbt_Handler:   b       PAbt_Handler
DAbt_Handler:   b       DAbt_Handler
IRQ_Handler:    b       vPreemptiveTick
FIQ_Handler:    b       FIQ_Handler
#else
Undef_Handler:  b       Undef_Handler
SWI_Handler:    b       SWI_Handler
PAbt_Handler:   b       PAbt_Handler
DAbt_Handler:   b       DAbt_Handler
IRQ_Handler:    b       irq
FIQ_Handler:    b       fiq
#endif

Reset_Handler:
	/* --------------------------------
	Initial Stack Pointer register
	--------------------------------
	INIT_STACK */
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

	/* ------------------------------------------------------
	Set the normal exception vector of CP15 control bit
	------------------------------------------------------ */
	MRC p15, 0, r0 , c1, c0
	bic r0, r0, #0x2000
	MCR p15, 0, r0 , c1, c0

	ldr r0, = __vector
	mrc p15, 0, r2, c1, c0, 0
	ands r2, r2, #(1 << 13)
	ldreq r1, =0x00000000
	ldrne r1, =0xFFFF0000
	
	ldmia r0!, {r2-r8, r10}
	stmia r1!, {r2-r8, r10}
	ldmia r0!, {r2-r8, r10}
	stmia r1!, {r2-r8, r10}

	/* Clear bss section */
	ldr r0, _bss_start
	ldr r2, _bss_end
	sub r2, r2, r0
	mov r1, #0
	bl memset

	B     boot_main

	.align 5
irq:
	ldr SP,=IRQ_Stack 
	sub sp, sp, #72
	stmia sp, {r0 - r12}
	add r8, sp, #60
	stmdb r8, {sp, lr}^
	nop
	str lr, [r8, #0]
	mrs r6, spsr
	str r6, [r8, #4]
	str r0, [r8, #8]
	mov r0, sp
	bl arm32_do_irq
	ldmia sp, {r0 - lr}^
	mov r0, r0
	ldr lr, [sp, #60]
	add sp, sp, #72
	subs pc, lr, #4

	.align 5
fiq:
	ldr SP,=FIQ_Stack 
	sub sp, sp, #72
	stmia sp, {r0 - r12}
	add r8, sp, #60
	stmdb r8, {sp, lr}^
	nop
	str lr, [r8, #0]
	mrs r6, spsr
	str r6, [r8, #4]
	str r0, [r8, #8]
	mov r0, sp
	bl arm32_do_fiq
	ldmia sp, {r0 - lr}^
	mov r0, r0
	ldr lr, [sp, #60]
	add sp, sp, #72
	subs pc, lr, #4


_bss_start:
	.long __bss_start
_bss_end:
	.long __bss_end

	.end