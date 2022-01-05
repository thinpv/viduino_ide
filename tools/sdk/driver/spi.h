#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

	void spi_init(SPI_Type *spi);
	void spi_exit(SPI_Type *spi);
	void spi_select(SPI_Type *spi);
	void spi_deselect(SPI_Type *spi);
	void spi_write_txbuf(SPI_Type *spi, uint8_t *buf, int len);
	int spi_transfer(SPI_Type *spi, void *txbuf, void *rxbuf, int len);

#ifdef __cplusplus
}
#endif

#endif
