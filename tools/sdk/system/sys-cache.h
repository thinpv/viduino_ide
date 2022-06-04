#include "types.h"


/* Define constants for use Cache in service parameters.  */
#define CACHE_4M		2
#define CACHE_8M		3
#define CACHE_16M		4
#define CACHE_32M		5
#define I_CACHE		6
#define D_CACHE		7
#define I_D_CACHE		8


typedef struct _coarse_table
{
	unsigned int page[256];
} _CTable;

#define  _CoarsePageSize 	64  //MB


/* Define Cache type  */
#define CACHE_WRITE_BACK		0
#define CACHE_WRITE_THROUGH	1
#define CACHE_DISABLE			-1

#define MMU_DIRECT_MAPPING	0
#define MMU_INVERSE_MAPPING	1


void  sysFlushCache(int32_t nCacheType);
int32_t sysEnableCache(uint32_t uCacheOpMode);
bool  sysGetCacheState(void);
void sysInvalidCache();
