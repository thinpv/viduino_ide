extern "C"
{
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <i2c.h>
#include <gpio.h>
}

#include "Wire.h"

// Initialize Class Variables //////////////////////////////////////////////////

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire()
{
	i2c = NULL;
}

TwoWire::TwoWire(I2CPinDescription *i2c) : i2c(i2c)
{
}

// Public Methods //////////////////////////////////////////////////////////////

void TwoWire::begin(void)
{
	begin(100000);
}

void TwoWire::begin(uint32_t frequency)
{
	rxBufferIndex = 0;
	rxBufferLength = 0;

	txBufferIndex = 0;
	txBufferLength = 0;

	// twi_init();
	// twi_attachSlaveTxEvent(onRequestService); // default callback must exist
	// twi_attachSlaveRxEvent(onReceiveService); // default callback must exist

	if (i2c != NULL)
	{
		gpio_set_cfg(i2c->pintype[0].P, i2c->pintype[0].num, i2c->pintype[0].type);
		gpio_set_cfg(i2c->pintype[1].P, i2c->pintype[1].num, i2c->pintype[1].type);
		gpio_set_pull(i2c->pintype[0].P, i2c->pintype[0].num, GPIO_PULL_UP);
		gpio_set_pull(i2c->pintype[1].P, i2c->pintype[1].num, GPIO_PULL_UP);
		i2c_init(i2c->I);
		i2c_set_frequency(i2c->I, frequency);
	}
}

void TwoWire::begin(int frequency)
{
	begin((uint32_t)frequency);
}

void TwoWire::end(void)
{
	// twi_disable();
}

void TwoWire::setClock(uint32_t clock)
{
	// twi_setFrequency(clock);
	i2c_set_frequency(i2c->I, clock);
}

void TwoWire::setWireTimeout(uint32_t timeout, bool reset_with_timeout)
{
	// twi_setTimeoutInMicros(timeout, reset_with_timeout);
}

/***
 * Returns the TWI timeout flag.
 *
 * @return true if timeout has occurred since the flag was last cleared.
 */
bool TwoWire::getWireTimeoutFlag(void)
{
	// return (twi_manageTimeoutFlag(false));
	return true;
}

/***
 * Clears the TWI timeout flag.
 */
void TwoWire::clearWireTimeoutFlag(void)
{
	// twi_manageTimeoutFlag(true);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
	if (isize > 0)
	{
		// send internal address; this mode allows sending a repeated start to access
		// some devices' internal registers. This function is executed by the hardware
		// TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

		beginTransmission(address);

		// the maximum size of internal address is 3 bytes
		if (isize > 3)
		{
			isize = 3;
		}

		// write internal register address - most significant byte first
		while (isize-- > 0)
			write((uint8_t)(iaddress >> (isize * 8)));
		endTransmission(false);
	}

	// clamp to buffer length
	if (quantity > BUFFER_LENGTH)
	{
		quantity = BUFFER_LENGTH;
	}
	// perform blocking read into buffer
	// uint8_t read = twi_readFrom(address, rxBuffer, quantity, sendStop);
	uint8_t read = i2c_read(i2c->I, address, rxBuffer, quantity, sendStop);
	//printf("requestFrom rt %d\r\n", read);
	// set rx buffer iterator vars
	rxBufferIndex = 0;
	rxBufferLength = read;

	return read;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

void TwoWire::beginTransmission(uint8_t address)
{
	// indicate that we are transmitting
	transmitting = 1;
	// set address of targeted slave
	txAddress = address;
	// reset tx buffer iterator vars
	txBufferIndex = 0;
	txBufferLength = 0;

	// printf("beginTransmission address %d txAddress %d\r\n", address, txAddress);

	// i2c_start(i2c->I);
}

void TwoWire::beginTransmission(int address)
{
	// printf("beginTransmission1 address %d\r\n", address);
	beginTransmission((uint8_t)address);
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to
//	perform a repeated start.
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
	//printf("endTransmission stop %d\r\n", sendStop);
	// transmit buffer (blocking)
	// uint8_t ret = twi_writeTo(txAddress, txBuffer, txBufferLength, 1, sendStop);
	// printf("endTransmission txAddress %d\r\n", txAddress);
	uint8_t ret = i2c_write(i2c->I, txAddress, txBuffer, txBufferLength, sendStop);
	// reset tx buffer iterator vars
	txBufferIndex = 0;
	txBufferLength = 0;
	// indicate that we are done transmitting
	transmitting = 0;
	return ret;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
	return endTransmission(true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
	//printf("write data: %d -- 0x%02X\r\n", data, data);
	if (transmitting)
	{
		// in master transmitter mode
		// don't bother if buffer is full
		if (txBufferLength >= BUFFER_LENGTH)
		{
			setWriteError();
			return 0;
		}
		// put byte in tx buffer
		txBuffer[txBufferIndex] = data;
		++txBufferIndex;
		// update amount in buffer
		txBufferLength = txBufferIndex;
	}
	else
	{
		// in slave send mode
		// reply to master
		// twi_transmit(&data, 1);
	}
	return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
	if (transmitting)
	{
		// in master transmitter mode
		for (size_t i = 0; i < quantity; ++i)
		{
			write(data[i]);
		}
	}
	else
	{
		// in slave send mode
		// reply to master
		// twi_transmit(data, quantity);
	}
	return quantity;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
	return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
	int value = -1;

	// get each successive byte on each call
	if (rxBufferIndex < rxBufferLength)
	{
		value = rxBuffer[rxBufferIndex];
		++rxBufferIndex;
	}

	return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
	int value = -1;

	if (rxBufferIndex < rxBufferLength)
	{
		value = rxBuffer[rxBufferIndex];
	}

	return value;
}

void TwoWire::flush(void)
{
	// XXX: to be implemented.
}

// behind the scenes function that is called when data is received
void TwoWire::onReceiveService(uint8_t *inBytes, int numBytes)
{
	// don't bother if user hasn't registered a callback
	if (!user_onReceive)
	{
		return;
	}
	// don't bother if rx buffer is in use by a master requestFrom() op
	// i know this drops data, but it allows for slight stupidity
	// meaning, they may not have read all the master requestFrom() data yet
	if (rxBufferIndex < rxBufferLength)
	{
		return;
	}
	// copy twi rx buffer into local read buffer
	// this enables new reads to happen in parallel
	for (uint8_t i = 0; i < numBytes; ++i)
	{
		rxBuffer[i] = inBytes[i];
	}
	// set rx iterator vars
	rxBufferIndex = 0;
	rxBufferLength = numBytes;
	// alert user program
	user_onReceive(numBytes);
}

// behind the scenes function that is called when data is requested
void TwoWire::onRequestService(void)
{
	// don't bother if user hasn't registered a callback
	if (!user_onRequest)
	{
		return;
	}
	// reset tx buffer iterator vars
	// !!! this will kill any pending pre-master sendTo() activity
	txBufferIndex = 0;
	txBufferLength = 0;
	// alert user program
	user_onRequest();
}

// sets function called on slave write
void TwoWire::onReceive(void (*function)(int))
{
	user_onReceive = function;
}

// sets function called on slave read
void TwoWire::onRequest(void (*function)(void))
{
	user_onRequest = function;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

void TwoWire::scan(Print &print, int start, int end)
{
	uint8_t temp;
	print.println("I2C start scan");
	for (int addr = start; addr < end; ++addr)
	{
		beginTransmission(addr);
		int ret = endTransmission(true);
		if (ret == 0)
		{
			print.print("Find I2C device addr: 0x");
			print.println(addr, HEX);
		}
	}
	print.println("I2C scan done");
}

TwoWire Wire(&I2C_Desc[0]);
TwoWire Wire1(&I2C_Desc[2]);
TwoWire Wire2(&I2C_Desc[3]);
