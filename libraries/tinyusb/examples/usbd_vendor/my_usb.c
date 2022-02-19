#include "tusb.h"
#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// WebUSB use vendor class
//--------------------------------------------------------------------+

// Invoked when a control transfer occurred on an interface of this class
// Driver response accordingly to the request and the transfer stage (setup/data/ack)
// return false to stall control endpoint (e.g unsupported request)
bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
  // nothing to with DATA & ACK stage
  if (stage != CONTROL_STAGE_SETUP) return true;

  switch (request->bmRequestType_bit.type)
  {
    case TUSB_REQ_TYPE_VENDOR:
    //   switch (request->bRequest)
    //   {
    //     case VENDOR_REQUEST_WEBUSB:
    //       // match vendor request in BOS descriptor
    //       // Get landing page url
    //       return tud_control_xfer(rhport, request, (void*)(uintptr_t) &desc_url, desc_url.bLength);

    //     case VENDOR_REQUEST_MICROSOFT:
    //       if ( request->wIndex == 7 )
    //       {
    //         // Get Microsoft OS 2.0 compatible descriptor
    //         uint16_t total_len;
    //         memcpy(&total_len, desc_ms_os_20+8, 2);

    //         return tud_control_xfer(rhport, request, (void*)(uintptr_t) desc_ms_os_20, total_len);
    //       }else
    //       {
    //         return false;
    //       }

    //     default: break;
    //   }
    break;

    case TUSB_REQ_TYPE_CLASS:
    //   if (request->bRequest == 0x22)
    //   {
    //     // Webserial simulate the CDC_REQUEST_SET_CONTROL_LINE_STATE (0x22) to connect and disconnect.
    //     web_serial_connected = (request->wValue != 0);

    //     // Always lit LED if connected
    //     if ( web_serial_connected )
    //     {
    //       board_led_write(true);
    //       blink_interval_ms = BLINK_ALWAYS_ON;

    //       tud_vendor_write_str("\r\nTinyUSB WebUSB device example\r\n");
    //     }else
    //     {
    //       blink_interval_ms = BLINK_MOUNTED;
    //     }

    //     // response with status OK
    //     return tud_control_status(rhport, request);
    //   }
    break;

    default: break;
  }

  // stall unknown request
  return false;
}