
#include "sys_cache.h"
// #include "sys_types.h"
// #include "sys_uart.h"
#include "arm32.h"

//extern void sysSetupCP15(unsigned int);
//extern void sys_flush_and_clean_dcache(void);

//BOOL volatile _sys_IsCacheOn = FALSE;
//INT32 volatile _sys_CacheMode;
//extern void sys_flush_and_clean_dcache(void);
//extern void sysDisableDCache(void);
//extern void sysDisableICache(void);
////extern int sysInitMMUTable(int);
//BOOL sysGetCacheState(void);
////
////__align(0x4000) unsigned int _mmuSectionTable[4096];
////__align(1024) static _CTable _mmuCoarsePageTable[_CoarsePageSize]; 			// maximum 64MB for coarse pages
////__align(1024) static _CTable _mmuCoarsePageTable_NonCache[_CoarsePageSize];	// Shadow SDRAM area for non-cacheable

////
//static int _MMUMappingMode = MMU_DIRECT_MAPPING;
//static BOOL _IsInitMMUTable = FALSE;
////

//INT32 sysGetSdramSizebyMB()
//{
//  return 32;
//}

/////*
////初始化二级页表 粗表
////vaddr 虚拟地睛
////phy_addr 物理地址
////size 内存大小
////CACHE 标志
////fev_flag 物理地址的方向 顺向与反向 ，反向为高地址先存入
////*/
////int sysInitPageTable(unsigned int vaddr, unsigned int phy_addr, int size, int cache_flag, int rev_flag)
////{
////	int i, cnt, table_num, page_num, cache_mode, addr_offset;
////	unsigned volatile int phy_base_addr, vbase_addr, temp;
////	volatile _CTable *PageTabPtr;
////
////
////	//sysprintf("v=0x%08x--0x%08x\r\n",vaddr,phy_addr);
////
////
////	//--------------------------------------得二级页表地址-------------------------------------
////		PageTabPtr = (_CTable *) _mmuCoarsePageTable;	//cache-able virtual address
////
////	vaddr &= 0x7FFFFFFF;
////	//--------------------------------------错误返回-------------------------------------
////	if (vaddr & 0xFFFFF) 	return -1;  /* MUST 1M Boundary */
////	if (size % 4096)	    return -2;  /* MUST 4K multiple size */
////

////	/* Pages count 得到页的数量*/
////	cnt = size / 4096;
//////	sysprintf("v=%d\r\n",cnt);
////
////
////  //得CACHE 模式
////	if (cache_flag == CACHE_WRITE_BACK) /* write back mode */
////		cache_mode = 0x0C;
////	else if (cache_flag == CACHE_WRITE_THROUGH) /* write through mode */
////		cache_mode = 0x08;
////	else
////		cache_mode = 0; /* Non-cacheable, non-buffered */
////
////
////
////	//映射模式
////	if (rev_flag == MMU_DIRECT_MAPPING)
////		phy_base_addr = phy_addr; //直接
////	else
////		phy_base_addr = phy_addr + size - 4096;		//返相
////
////
////	//4K每页大小 小页模式
////	addr_offset = 4096;
////
////
////
////	//复制参数到二级页表
////	for (i=0; i<cnt; i++)
////	{
////    //计算虚拟在址的页计数值 page_num
////		vbase_addr = vaddr + i * 4096; //得虚拟地址
////		table_num = vbase_addr / 0x100000;//得相应断表对应地址 指向下一段的页表
////		page_num =  (vbase_addr & 0xFF000) >> 12; /*得到页表的索引，计数值*/
////
////
////		//计算参数值
////		temp = phy_base_addr & 0xFFFFF000;//页地址 高20位
////		temp |= 0xFF0; /* access permission, 11 for read/write 存取权限 11读写 两个位控制1/4大小的权限*/
////		temp |= cache_mode; /* cache mode Cache 模式*/
////		temp |= 0x02;  /* small page 小页模式 4k大小*/
////
////
////		//写入参数值
////		(*(PageTabPtr+table_num)).page[page_num] = temp;
////
////
////		//地址偏移方式
////		if (rev_flag == MMU_DIRECT_MAPPING)
////			phy_base_addr += addr_offset; //顺向
////		else
////			phy_base_addr -= addr_offset; //返向
////	}
////
////	return 0;
////
////} /* end sysInitPageTable */

/////*
////MMU初始化
////物理 0x80000000-0x82000000映射到0x80000000-0x82000000
////*/
////int sysInitMMUTable(int cache_mode)
////{
////#define RAM_SIZE 32	 //设置32MB内存
////
////	unsigned volatile int temp;
////	int i, size;
////  size=RAM_SIZE;
////	ramsize=RAM_SIZE;
////
////	if (_IsInitMMUTable == FALSE)
////	{
//////-----------------------------------------------初始化4G空间段表 0-4G 射到 0-4G 4096个1MB----------------------------------------
////		temp=0;
////		temp |= (0x3 << 10);//权限
////		temp |= (0x0 << 5);/* (8:5) domain 15 */
////		temp |= (0x1 << 4);/* bit 4 must be 1 */
////		temp |= (0x0 << 2);/* bit 3:2 for cache control bits, cache disabled */
////		temp |= (0x2 << 0);   /* set as 1Mb section */
////
////		for (i=0; i<4096; i++)
////		{
////			_mmuSectionTable[i] = (unsigned int)(temp | (i << 20));//【存放0-4096个1M表】
////		}
////
//////--------------------------------------印射物理 0x80000000 到虚拟 0x80000000 共32M，没有CACHE,粗表模式 2级映射-------------------------------------------------
////		for (i=0; i< size; i++)
////		{
////			temp = (((unsigned int)_mmuCoarsePageTable + (unsigned int)(i*1024)) & 0xFFFFFC00); /* 粗表地址 256*4=1024 */
////			temp |= (0x0 << 5);/* (8:5) domain 15 */
////			temp |= (0x1 << 4);/* bit 4 must be 1 */
////			temp |= (0x1 << 0);/* 设置为粗表*/

////			sysInitPageTable((0x80000000+(i << 20)),(0x80000000+(i << 20)), 0x100000, cache_mode, MMU_DIRECT_MAPPING); /* 直接映射 */
////			_mmuSectionTable[(0x80000000+(i << 20))>>20] = temp;

////		}
////
//// 	}
//// 	//set CP15 registers
////	sysSetupCP15((unsigned int)_mmuSectionTable);
//// 	_IsInitMMUTable = TRUE;
//// 	return 0;
////
////} /* end sysInitMMUTable */

////INT32 sysEnableCache(UINT32 uCacheOpMode)
////{
////	sysInitMMUTable(uCacheOpMode);
////	_sys_IsCacheOn = TRUE;
////	_sys_CacheMode = uCacheOpMode;
////
////	return 0;
////}

void sysFlushCache(int32_t nCacheType)
{
	int temp;

	switch (nCacheType)
	{
	case I_CACHE:
		// __asm__ __volatile__(
		// 	/*----- flush I-cache -----*/
		// 	"MOV temp, 0x0\n"
		// 	"MCR p15, 0, temp, c7, c5, 0" /* invalidate I cache */
		// );
		temp = 0;
		__asm__ __volatile__("mcr p15, 0, %0, c7, c5, 0"
												 :
												 : "r"(temp));
		break;

	case D_CACHE:
		sys_flush_and_clean_dcache();
		// __asm__ __volatile__(
		// 	/*----- flush D-cache & write buffer -----*/
		// 	"MOV temp, 0x0\n"
		// 	"MCR p15, 0, temp, c7, c10, 4" /* drain write buffer */
		// );
		temp = 0;
		__asm__ __volatile__("mcr p15, 0, %0, c7, c10, 4"
												 :
												 : "r"(temp));
		break;

	case I_D_CACHE:
		sys_flush_and_clean_dcache();
		// __asm__ __volatile__(
		// 	/*----- flush I, D cache & write buffer -----*/
		// 	"MOV temp, 0x0\n"
		// 	"MCR p15, 0, temp, c7, c5, 0" /* invalidate I cache */
		// 	"MCR p15, 0, temp, c7, c10, 4" /* drain write buffer */
		// );
		temp = 0;
		__asm__ __volatile__("mcr p15, 0, %0, c7, c5, 0"
												 :
												 : "r"(temp));
		__asm__ __volatile__("mcr p15, 0, %0, c7, c10, 4"
												 :
												 : "r"(temp));
		break;

	default:;
	}
}

void sysInvalidCache()
{
	int temp;

	// __asm__ __volatile__(
	// 	MOV temp, 0x0
	// 	MCR p15, 0, temp, c7, c7, 0 /* invalidate I and D cache */
	// }
	temp = 0;
	__asm__ __volatile__("mcr p15, 0, %0, c7, c7, 0"
											 :
											 : "r"(temp));
}

//BOOL sysGetCacheState()
//{
//	return _sys_IsCacheOn;
//}

//INT32 sysGetCacheMode()
//{
//	return _sys_CacheMode;
//}

//INT32 _sysLockCode(UINT32 addr, INT32 size)
//{
//	int i, cnt, temp;
//
//	__asm
//	{
//	    /* use way3 to lock instructions */
//		MRC p15, 0, temp, c9, c0, 1 ;
//		ORR temp, temp, 0x07 ;
//		MCR p15, 0, temp, c9, c0, 1 ;
//	}
//
//	if (size % 16)  cnt = (size/16) + 1;
//	else            cnt = size / 16;
//
//	for (i=0; i<cnt; i++)
//	{
//		__asm
//		{
//			MCR p15, 0, addr, c7, c13, 1;
//		}
//
//		addr += 16;
//	}
//
//
//	__asm
//	{
//	    /* use way3 to lock instructions */
//		MRC p15, 0, temp, c9, c0, 1 ;
//		BIC temp, temp, 0x07 ;
//		ORR temp, temp, 0x08 ;
//		MCR p15, 0, temp, c9, c0, 1 ;
//	}
//
//	return Successful;

//}

//INT32 _sysUnLockCode()
//{
//	int temp;
//
//	/* unlock I-cache way 3 */
//	__asm
//	{
//		MRC p15, 0, temp, c9, c0, 1;
//		BIC temp, temp, 0x08 ;
//		MCR p15, 0, temp, c9, c0, 1;
//
//	}
//
//	return Successful;
//}

////INT32 Copy_vector()
////{
////	__asm
////	{
////		/* Copy vector to the correct address */

////	}
////}
