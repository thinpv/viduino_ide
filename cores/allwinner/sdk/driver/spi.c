#include <stdint.h>
#include <types.h>
#include <string.h>
#include <io.h>
#include <stddef.h>
#include <spi.h>
#include <gpio.h>
#include <ccu.h>

void spi_init(SPI_Type *spi)
{
	if (spi == SPI0)
		ccu_reset(RESET_SPI0, true);
	else if (spi == SPI1)
		ccu_reset(RESET_SPI1, true);

	spi->SPI_CCR_REG = 0x00001001;
	spi->SPI_GCR_REG |= (1 << 31) | (1 << 7) | (1 << 1) | (1 << 0);
	while (spi->SPI_GCR_REG & (1 << 31))
		;

	spi->SPI_TCR_REG &= ~(0x3 << 0);
	spi->SPI_TCR_REG |= (1 << 6) | (1 << 2);

	spi->SPI_FCR_REG |= (1 << 31);
	while (spi->SPI_FCR_REG & (1 << 31))
		;
	spi->SPI_FCR_REG |= (1 << 15);
	while (spi->SPI_FCR_REG & (1 << 15))
		;
}

void spi_exit(SPI_Type *spi)
{
	spi->SPI_GCR_REG |= (1 << 31);
	while (spi->SPI_GCR_REG & (1 << 31))
		;
	spi->SPI_GCR_REG &= ~((1 << 1) | (1 << 0));
}

void spi_select(SPI_Type *spi)
{
	spi->SPI_TCR_REG &= ~((0x3 << 4) | (0x1 << 7));
	spi->SPI_TCR_REG |= ((0 & 0x3) << 4) | (0x0 << 7);
}

void spi_deselect(SPI_Type *spi)
{
	spi->SPI_TCR_REG &= ~((0x3 << 4) | (0x1 << 7));
	spi->SPI_TCR_REG |= ((0 & 0x3) << 4) | (0x1 << 7);
}

void spi_write_txbuf(SPI_Type *spi, uint8_t *buf, int len)
{
	int i;

	if (!buf)
		len = 0;

	spi->SPI_MTC_REG = len & 0xffffff;
	spi->SPI_BCC_REG = len & 0xffffff;

	for (i = 0; i < len; ++i)
	{
		spi->SPI_TXD_REG = *buf++;
	}
}

int spi_transfer(SPI_Type *spi, void *txbuf, void *rxbuf, int len)
{
	int count = len;
	uint8_t *tx = txbuf;
	uint8_t *rx = rxbuf;
	int n, i;
	uint8_t val;

	while (count > 0)
	{
		n = (count <= 64) ? count : 64;
		spi->SPI_MBC_REG = n;
		spi_write_txbuf(SPI0, tx, n);
		spi->SPI_TCR_REG |= (1 << 31);

		while ((spi->SPI_FSR_REG & 0xff) < n)
			;
		for (i = 0; i < n; i++)
		{
			val = spi->SPI_RXD_REG;
			if (rx)
				*rx++ = val;
		}

		if (tx)
			tx += n;
		count -= n;
	}
	return len;
}
