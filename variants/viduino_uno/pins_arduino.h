#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

#include <F1C100S.h>

#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13
#define D14 14
#define D15 15
#define D16 16
#define D17 17
#define D18 18
#define D19 19
#define D20 20
#define D21 21
#define D22 22
#define D23 23
#define D24 24
#define D25 25
#define D26 26
#define D27 27
#define D28 28
#define D29 29
#define D30 30
#define D31 31
#define D32 32
#define D33 33
#define D34 34
#define D35 35
#define D36 36
#define D37 37
#define D38 38
#define D39 39
#define D40 40
#define D41 41
#define D42 42
#define D43 43
#define D44 44
#define D45 45
#define D46 46
#define D47 47
#define D48 48
#define D49 49

#define SS 10
#define MOSI 11
#define MISO 12
#define SCK 13

#define LED_BUILTIN 16
#define SDA 14
#define SCL 15

#define USE_BoardToPin

/* Types used for the tables below */
typedef struct _PinType
{
	GPIO_Type *P;
	uint32_t num;
	uint32_t type;
} PinType;

typedef struct _PinDescription
{
	//   uint32_t MFP;
	//   uint32_t Mask;
	uint32_t Type;
} PinDescription;

typedef struct _GPIOPinDescription
{
	GPIO_Type *port;
	uint32_t pin;
	PinDescription Pin;
} GPIOPinDescription;

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

extern GPIOPinDescription GPIO_Desc[];
extern SPIPinDescription SPI_Desc[];
extern UARTPinDescription UART_Desc[];
extern I2CPinDescription I2C_Desc[];

#endif
