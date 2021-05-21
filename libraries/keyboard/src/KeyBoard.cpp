#include "KeyBoard.h"
#include "timer.h"

KeyBoard::KeyBoard()
{
}

// ~KeyBoard();
int KeyBoard::begin()
{
    set_usb_descriptor_request_func(usb_hid_descriptor_request);
    set_usb_in_ep_callback_func(usb_hid_in_ep_callback);
    set_usb_out_ep_callback_func(usb_hid_out_ep_callback);
    usb_device_init(USB_TYPE_USB_HID);
}

int KeyBoard::write(char key)
{
    u8 buff[8]={0,0,0,0,0,0,0,0};	
    if(usb_connect) 
    {
        buff[2]=key;
        usb_device_write_data(1,buff,8);//回送
        delay(1);
        buff[2]=0;
        usb_device_write_data(1,buff,8);//回送
        delay(1);
    }
}

int KeyBoard::getStatus()
{

}
