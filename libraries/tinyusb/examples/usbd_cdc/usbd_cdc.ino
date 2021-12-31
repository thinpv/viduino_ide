#include "tusb.h"

void setup() {
  // put your setup code here, to run once:
  tusb_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  tud_task();
}

// echo to either Serial0 or Serial1
// with Serial0 as all lower case, Serial1 as all upper case
static void echo_serial_port(uint8_t itf, uint8_t buf[], uint32_t count)
{
  for(uint32_t i=0; i<count; i++)
  {
    if (itf == 0)
    {
      // echo back 1st port as lower case
      if (isupper(buf[i])) buf[i] += 'a' - 'A';
    }
    else
    {
      // echo back 2nd port as upper case
      if (islower(buf[i])) buf[i] -= 'a' - 'A';
    }

    tud_cdc_n_write_char(itf, buf[i]);
  }
  tud_cdc_n_write_flush(itf);
}

uint8_t buf[512];
void tud_cdc_rx_cb(uint8_t itf) {
	uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

	// echo back to both serial ports
	echo_serial_port(itf, buf, count);
}