#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

#include <F1C100S.h>

#define A0 GPIOA3
#define A1 GPIOA2
#define A2 GPIOA1
#define A3 GPIOA0

#define D0 GPIOA2
#define D1 GPIOA3
#define D2 GPIOE0
#define D3 GPIOE1
#define D4 GPIOE2
#define D5 GPIOE3
#define D6 GPIOE4
#define D7 GPIOE5
#define D8 GPIOE12
#define D9 GPIOE6
#define D10 GPIOE7
#define D11 GPIOE8
#define D12 GPIOE10
#define D13 GPIOE9
#define D14 GPIOD0
#define D15 GPIOD12

#define D16 GPIOD19
#define D17 GPIOD21
#define D18 GPIOD20
#define D19 GPIOD18
#define D20 GPIOD17
#define D21 GPIOD16
#define D22 GPIOD15
#define D23 GPIOD14
#define D24 GPIOD13
#define D25 GPIOD11
#define D26 GPIOD10
#define D27 GPIOD9
#define D28 GPIOD8
#define D29 GPIOD7
#define D30 GPIOD6
#define D31 GPIOD5
#define D32 GPIOD4
#define D33 GPIOD3
#define D34 GPIOD2
#define D35 GPIOD1

#define PIN_SPI_SS		D10
#define PIN_SPI_MOSI	D11
#define PIN_SPI_MISO	D12
#define PIN_SPI_SCK		D13

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

#define PIN_WIRE_SDA	D14
#define PIN_WIRE_SCL	D15

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN		GPIOA0

#define USE_BoardToPin

/* Types used for the tables below */
typedef struct _PinType
{
	uint32_t num;
	uint32_t type;
} PinType;

typedef struct _PinDescription
{
	//   uint32_t MFP;
	//   uint32_t Mask;
	uint32_t Type;
} PinDescription;

// typedef struct _GPIOPinDescription
// {
// 	GPIO_Type *port;
// 	uint32_t pin;
// 	PinDescription Pin;
// } GPIOPinDescription;

typedef struct _ADCPinDescription
{
	uint32_t module;
	uint32_t ch;
	PinType pintype;
} ADCPinDescription;

typedef struct _PWMPinDescription
{
	// PWM_T *P;
	uint32_t module;
	IRQn_Type irq;
	uint32_t ch;
	uint32_t freq;
	PinType pintype;
} PWMPinDescription;

typedef struct _SPIPinDescription
{
	SPI_Type *S;
	// uint32_t module;
	IRQn_Type irq;
	uint32_t clksel;
	PinType pintype[4];
} SPIPinDescription;

typedef struct _UARTPinDescription
{
	UART_Type *U;
	// uint32_t module;
	IRQn_Type irq;
	PinType pintype[2];
} UARTPinDescription;

typedef struct _I2CPinDescription
{
	I2C_Type *I;
	// uint32_t module;
	IRQn_Type irq;
	PinType pintype[2];
} I2CPinDescription;

typedef struct _BoardToPin
{
	int32_t pin;
	uint8_t type;
	uint8_t num;
} BoardToPin;

// extern GPIOPinDescription GPIO_Desc[];
extern SPIPinDescription SPI_Desc[];
extern UARTPinDescription UART_Desc[];
extern I2CPinDescription I2C_Desc[];

#endif
