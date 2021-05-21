#ifndef __MASS_STORAGE_H_
#define __MASS_STORAGE_H_

#include "usb_dev.h"
#include "usb_memory.h"
#include "usb_Mass_storage.h"

#ifdef __cplusplus
class MassStorage{
public :
    MassStorage();
    // ~MassStorage();
    int begin(int memoryType = MemoryTypeSD);
    int getStatus();
private:
};
#endif

#endif
