#include "MassStorage.h"
#include <sdc.h>

MassStorage::MassStorage()
{
}

// ~MassStorage();
int MassStorage::begin(int memoryType)
{
    MemoryType = memoryType;
    sdc_init(SDC0);
    Memory_init();
    set_usb_descriptor_request_func(usb_mass_descriptor_request);
    set_usb_in_ep_callback_func(usb_mass_in_ep_callback);
    set_usb_out_ep_callback_func(usb_mass_out_ep_callback);
    usb_device_init(USB_TYPE_USB_MASS);
}

int MassStorage::getStatus()
{
}
