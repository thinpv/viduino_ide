#ifndef __ARM32_H__
#define __ARM32_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

	static inline uint32_t arm32_read_p15_c1(void)
	{
		uint32_t value;

		__asm__ __volatile__(
				"mrc p15, 0, %0, c1, c0, 0"
				: "=r"(value)
				:
				: "memory");

		return value;
	}

	static inline void arm32_write_p15_c1(uint32_t value)
	{
		__asm__ __volatile__(
				"mcr p15, 0, %0, c1, c0, 0"
				:
				: "r"(value)
				: "memory");
		arm32_read_p15_c1();
	}

	static inline void arm32_interrupt_enable(void)
	{
#if defined(__GNUC__) && !(__CC_ARM)

#else
	INT32 temp;
#endif
#if defined(__GNUC__) && !(__CC_ARM)
		asm(
				"mrs    r0, CPSR  \n"
				"bic    r0, r0, #0x80  \n"
				"msr    CPSR_c, r0  \n");
#else
	__asm
	{
               MRS    temp, CPSR
               AND    temp, temp, nIntState
               MSR    CPSR_c, temp
	}
#endif
	}

	static inline void arm32_interrupt_disable(void)
	{
#if defined(__GNUC__) && !(__CC_ARM)

#else
	INT32 temp;
#endif
#if defined(__GNUC__) && !(__CC_ARM)
		asm(
				"MRS    r0, CPSR  \n"
				"ORR    r0, r0, #0x80  \n"
				"MSR    CPSR_c, r0  \n");
#else
	__asm
	{
               MRS    temp, CPSR
               ORR    temp, temp, nIntState
               MSR    CPSR_c, temp
	}
#endif
	}

	static inline void arm32_mmu_enable(void)
	{
		uint32_t value = arm32_read_p15_c1();
		arm32_write_p15_c1(value | (1 << 0));
	}

	static inline void arm32_mmu_disable(void)
	{
		uint32_t value = arm32_read_p15_c1();
		arm32_write_p15_c1(value & ~(1 << 0));
	}

	static inline void arm32_dcache_enable(void)
	{
		uint32_t value = arm32_read_p15_c1();
		arm32_write_p15_c1(value | (1 << 2));
	}

	static inline void arm32_dcache_disable(void)
	{
		uint32_t value = arm32_read_p15_c1();
		arm32_write_p15_c1(value & ~(1 << 2));
	}

	static inline void arm32_icache_enable(void)
	{
		uint32_t value = arm32_read_p15_c1();
		arm32_write_p15_c1(value | (1 << 12));
	}

	static inline void arm32_icache_disable(void)
	{
		uint32_t value = arm32_read_p15_c1();
		arm32_write_p15_c1(value & ~(1 << 12));
	}

	static inline uint32_t arm32_smp_processor_id(void)
	{
		uint32_t tmp;

		__asm__ __volatile__(
				"mrc p15,0,%0,c0,c0,5\n"
				"and %0,%0,#0x3\n"
				: "=r"(tmp)
				:
				: "memory");
		return tmp;
	}

	static inline void arm32_ttb_set(uint32_t base)
	{
		__asm__ __volatile__(
				"mcr p15, 0, %0, c2, c0, 0"
				:
				: "r"(base)
				: "memory");
	}

	static inline void arm32_domain_set(uint32_t domain)
	{
		__asm__ __volatile__(
				"mcr p15, 0, %0, c3, c0, 0"
				:
				: "r"(domain)
				: "memory");
	}

	static inline void arm32_tlb_invalidate(void)
	{
		__asm__ __volatile__(
				"mov r0, #0\n"
				"mcr p15, 0, r0, c7, c10, 4\n"
				"mcr p15, 0, r0, c8, c6, 0\n"
				"mcr p15, 0, r0, c8, c5, 0\n"
				:
				:
				: "r0");
	}

#ifdef __cplusplus
}
#endif

#endif /* __ARM32_H__ */
