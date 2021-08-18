#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <io.h>
#include <reg-ccu.h>
#include <i2c.h>
#include <ccu.h>

enum
{
	I2C_ADDR = 0x000,
	I2C_XADDR = 0x004,
	I2C_DATA = 0x008,
	I2C_CNTR = 0x00c,
	I2C_STAT = 0x010,
	I2C_CCR = 0x014,
	I2C_SRST = 0x018,
	I2C_EFR = 0x01c,
	I2C_LCR = 0x020,
};

enum
{
	I2C_STAT_BUS_ERROR = 0x00,
	I2C_STAT_TX_START = 0x08,
	I2C_STAT_TX_RSTART = 0x10,
	I2C_STAT_TX_AW_ACK = 0x18,
	I2C_STAT_TX_AW_NAK = 0x20,
	I2C_STAT_TXD_ACK = 0x28,
	I2C_STAT_TXD_NAK = 0x30,
	I2C_STAT_LOST_ARB = 0x38,
	I2C_STAT_TX_AR_ACK = 0x40,
	I2C_STAT_TX_AR_NAK = 0x48,
	I2C_STAT_RXD_ACK = 0x50,
	I2C_STAT_RXD_NAK = 0x58,
	I2C_STAT_IDLE = 0xf8,
};

void i2c_set_frequency(I2C_Type *i2c, u64_t rate)
{
	u64_t pclk = 600000000;
	s64_t freq, delta, best = 0x7fffffffffffffffLL;
	int tm = 5, tn = 0;
	int m, n;

	for (n = 0; n <= 7; n++)
	{
		for (m = 0; m <= 15; m++)
		{
			freq = pclk / (10 * (m + 1) * (1 << n));
			delta = rate - freq;
			if (delta >= 0 && delta < best)
			{
				tm = m;
				tn = n;
				best = delta;
			}
			if (best == 0)
				break;
		}
	}
	i2c->TWI_CCR_REG = ((tm & 0xf) << 3) | ((tn & 0x7) << 0);
}

int i2c_init(I2C_Type *i2c, uint32_t rate)
{
	if (i2c == I2C0)
		ccu_reset(RESET_I2C0, true);
	else if (i2c == I2C1)
		ccu_reset(RESET_I2C1, true);
	else if (i2c == I2C2)
		ccu_reset(RESET_I2C2, true);

	i2c->TWI_SRST_REG = 1 << 0;
	i2c->TWI_SRST_REG = 0 << 0;
	i2c_set_frequency(i2c, rate);
	i2c->TWI_ADDR_REG = 0;
	i2c->TWI_XADDR_REG = 0;
	i2c->TWI_CNTR_REG = (1 << 6) | (1 << 4);

	unsigned long timeout = millis() + 100;
	do
	{
		if (!(i2c->TWI_CNTR_REG & (1 << 4)))
		{
			break;
		}
	} while (timeout > millis());
	if (i2c->TWI_CNTR_REG & (1 << 4))
	{
		// printf("stop error\r\n");
	}
	return 1;
}

int i2c_wait_status(I2C_Type *i2c)
{
	unsigned long timeout = millis() + 100;
	do
	{
		if (i2c->TWI_CNTR_REG & (1 << 3))
		{
			//printf("i2c->TWI_STAT_REG: 0x%02X\r\n", i2c->TWI_STAT_REG);
			return i2c->TWI_STAT_REG;
		}
	} while (timeout > millis());
	// printf("i2c_wait_status timeout\r\n");
	return I2C_STAT_BUS_ERROR;
}

int i2c_start(I2C_Type *i2c)
{
	S_Bit(i2c->TWI_CNTR_REG, 5);
	C_Bit(i2c->TWI_CNTR_REG, 3);
	unsigned long timeout = millis() + 100;
	do
	{
		if (!R_Bit(i2c->TWI_CNTR_REG, 5))
		{
			break;
		}
	} while (timeout > millis());
	if (R_Bit(i2c->TWI_CNTR_REG, 5))
	{
		// printf("i2c_start error\r\n");
	}
	return i2c_wait_status(i2c);
}

int i2c_stop(I2C_Type *i2c)
{
	S_Bit(i2c->TWI_CNTR_REG, 4);
	C_Bit(i2c->TWI_CNTR_REG, 3);
	unsigned long timeout = millis() + 100;
	do
	{
		if (!R_Bit(i2c->TWI_CNTR_REG, 4))
			return 1;
	} while (timeout > millis());
	if (R_Bit(i2c->TWI_CNTR_REG, 4))
	{
		// printf("i2c_stop error\r\n");
	}
	return 0;
}

int i2c_send_data(I2C_Type *i2c, u8_t dat)
{
	i2c->TWI_DATA_REG = dat;
	C_Bit(i2c->TWI_CNTR_REG, 3);
	return i2c_wait_status(i2c);
}

uint8_t i2c_read(I2C_Type *i2c, uint8_t addr, uint8_t *buf, size_t len, uint8_t sendStop)
{
	uint8_t ret = len;
	i2c_start(i2c);
	int rs = i2c_send_data(i2c, (u8_t)(addr << 1 | 1));
	if (rs != I2C_STAT_TX_AR_ACK)
	{
		// printf("i2c_read addr 0x%02X error %d\r\n", addr, rs);
		if (sendStop)
		{
			i2c_stop(i2c);
		}
		return 0;
	}

	while (len > 0)
	{
		switch (i2c_wait_status(i2c))
		{
		case I2C_STAT_TX_AR_ACK:
			if (len == 1)
				i2c->TWI_CNTR_REG &= ~((1 << 3) | (1 << 2));
			else
				i2c->TWI_CNTR_REG &= ~(1 << 3);
			break;
		case I2C_STAT_RXD_ACK:
			*buf++ = i2c->TWI_DATA_REG;
			len--;
			if (len == 1)
				i2c->TWI_CNTR_REG &= ~((1 << 3) | (1 << 2));
			else
				i2c->TWI_CNTR_REG &= ~(1 << 3);
			break;
		case I2C_STAT_RXD_NAK:
			*buf++ = i2c->TWI_DATA_REG;
			len--;
			break;
		default:
			if (sendStop)
			{
				i2c_stop(i2c);
			}
			return ret - len;
		}
	}
	if (sendStop)
	{
		i2c_stop(i2c);
	}
	return ret - len;
}

uint8_t i2c_write(I2C_Type *i2c, uint8_t addr, uint8_t *buf, size_t len, uint8_t sendStop)
{
	uint8_t ret = len;
	i2c_start(i2c);
	uint8_t rs = i2c_send_data(i2c, (u8_t)(addr << 1));
	if (rs != I2C_STAT_TX_AW_ACK)
	{
		// printf("i2c_write error 0x%02X addr 0x%02X\r\n", rs, addr);
		if (sendStop)
		{
			i2c_stop(i2c);
		}
		return 2;
	}
	while (len > 0)
	{
		if (i2c_send_data(i2c, *buf++) != I2C_STAT_TXD_ACK)
			return 3;
		len--;
	}
	if (sendStop)
	{
		i2c_stop(i2c);
	}
	return 0;
}
