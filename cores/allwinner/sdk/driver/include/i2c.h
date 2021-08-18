#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

	enum
	{
		I2C_M_TEN = 0x0010,
		I2C_M_RD = 0x0001,
		I2C_M_STOP = 0x8000,
		I2C_M_NOSTART = 0x4000,
		I2C_M_REV_DIR_ADDR = 0x2000,
		I2C_M_IGNORE_NAK = 0x1000,
		I2C_M_NO_RD_ACK = 0x0800,
		I2C_M_RECV_LEN = 0x0400,
	};

	struct i2c_msg_t
	{
		int addr;
		int flags;
		int len;
		void *buf;
	};

	//from kendryte-standalone-sdk
	//https://github.com/sipeed/kendryte-standalone-sdk/blob/1d52a1aaa05fd10ad6c5968146d408709e0f9c2c/lib/drivers/include/i2c.h
	typedef enum _i2c_bus_speed_mode
	{
		I2C_BS_STANDARD,
		I2C_BS_FAST,
		I2C_BS_HIGHSPEED
	} i2c_bus_speed_mode_t;

	typedef enum _i2c_event
	{
		I2C_EV_START,
		I2C_EV_RESTART,
		I2C_EV_STOP
	} i2c_event_t;
	/////

	int i2c_init(I2C_Type *i2c, uint32_t rate);
	void i2c_set_frequency(I2C_Type *i2c, u64_t rate);
	int i2c_wait_status(I2C_Type *i2c);
	int i2c_start(I2C_Type *i2c);
	int i2c_stop(I2C_Type *i2c);
	int i2c_send_data(I2C_Type *i2c, u8_t dat);
	uint8_t i2c_read(I2C_Type *i2c, uint8_t addr, uint8_t *buf, size_t len, uint8_t sendStop);
	uint8_t i2c_write(I2C_Type *i2c, uint8_t addr, uint8_t *buf, size_t len, uint8_t sendStop);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */
