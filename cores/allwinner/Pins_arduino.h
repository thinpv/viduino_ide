/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

#define D0	0
#define D1	1
#define D2	2
#define D3	3
#define D4	4
#define D5	5
#define D6	6
#define D7	7
#define D8	8
#define D9	9
#define D10	10
#define D11	11
#define D12	12
#define D13	13
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

#define SS   10
#define MOSI 11
#define MISO 12
#define SCK  13

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
#if defined(__M451__) | defined(__NANO100__) | defined(__NANO1X2__)
#elif defined(__NUC240__)
#elif defined(__NUC131__)
#endif
} PinType;

typedef struct _PinDescription
{
#if defined(__VIDUINO_UNO__)
//   uint32_t MFP;  
//   uint32_t Mask;
	uint32_t Type;
#elif defined(__M451__) | defined(__NANO100__) | defined(__NANO1X2__)
#elif defined(__NUC240__)
#elif defined(__NUC131__)
#endif
} PinDescription;

typedef struct _GPIOPinDescription
{
	GPIO_Type* port;
	uint32_t pin;
	PinDescription Pin;
} GPIOPinDescription;

typedef struct _ADCPinDescription
{
 #if defined(__M451__) 
	EADC_T *A;	
 #elif defined(__NUC131__) | defined(__NUC240__) | defined(__NANO100__) | defined(__NANO1X2__)
        ADC_T *A;
 #endif
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
}SPIPinDescription;

typedef struct _UARTPinDescription
{
	UART_Type *U;	
	// uint32_t module;
	IRQn_Type irq;
	PinType pintype[2];
}UARTPinDescription;

#if defined(__M451__) | defined(__NUC240__)
typedef struct _CANPinDescription
{
	CAN_T *C;	
	uint32_t module;
	IRQn_Type irq;
  PinType pintype[2];
}CANPinDescription;
#endif

typedef struct _I2CPinDescription
{
	I2C_Type *I;	
	// uint32_t module;
	IRQn_Type irq;
	PinType pintype[2];
}I2CPinDescription;


typedef struct _BoardToPin
{
	int32_t pin;
	uint8_t type;
  uint8_t num;
}BoardToPin;

extern GPIOPinDescription GPIO_Desc[];

#ifdef USE_BoardToPin
#define GPIO_TYPE   0
#define UART_TYPE  	1
#define I2C_TYPE   	2
#define SPI_TYPE		3
#define ADC_TYPE 		4
#define PWM_TYPE 		5
#define CAN_TYPE 		6
#if defined(__NUC131__)
#define BoardToPin_MAX_COUNT 44
#elif defined(__NANO1X2__)
	#if defined(__IOT_EVB__)
	  #define BoardToPin_MAX_COUNT 18
	#endif
#else
#define BoardToPin_MAX_COUNT 81
#endif
extern BoardToPin BoardToPinInfo[];
#endif


#if defined(__M451__)
	#define GPIO_MAX_COUNT 101
	extern GPIOPinDescription GPIO_Desc[];
	#define GPIO_Config(Desc) outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type)
#elif defined(__NUC240__)
	#define GPIO_MAX_COUNT 101
	extern GPIOPinDescription GPIO_Desc[];
	#define GPIO_Config(Desc) \
	do { \
		outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type); \
		outp32(&SYS->ALT_MFP, (inp32(&SYS->ALT_MFP)  & ~Desc.Pin.ALTMsk));  \
		outp32(&SYS->ALT_MFP1,(inp32(&SYS->ALT_MFP1) & ~Desc.Pin.ALT1Msk)); \
		outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~Desc.Pin.ALT2Msk)); \
	}while(0); 
#elif defined(__NANO100__)
	#define GPIO_MAX_COUNT 128
	extern GPIOPinDescription GPIO_Desc[];
	#define GPIO_Config(Desc) outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type)
#elif defined(__NUC131__)
#define GPIO_MAX_COUNT 64
extern const GPIOPinDescription GPIO_Desc[];
#define GPIO_Config(Desc) \
do { \
	outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type); \
	outp32(&SYS->ALT_MFP, (inp32(&SYS->ALT_MFP)  & ~Desc.Pin.ALTMsk));  \
	outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~Desc.Pin.ALT2Msk)); \
	outp32(&SYS->ALT_MFP3,(inp32(&SYS->ALT_MFP3) & ~Desc.Pin.ALT3Msk)); \
	outp32(&SYS->ALT_MFP4,(inp32(&SYS->ALT_MFP4) & ~Desc.Pin.ALT4Msk)); \
}while(0); 
#elif defined(__NANO1X2__) 
     #if defined(__NANO102SC2AN__)
	      #define GPIO_MAX_COUNT 64
	   #elif defined(__NANO102ZC2AN__)
	      #define GPIO_MAX_COUNT 32
     #elif defined(__NANO112__)
        #define GPIO_MAX_COUNT 100
     #endif

	extern GPIOPinDescription GPIO_Desc[];
	#define GPIO_Config(Desc) outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type)
#endif

#if defined(__M451__)
#define PWM_MAX_COUNT 8
	extern PWMPinDescription PWM_Desc[];
	#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#elif defined(__NUC240__)
	#define PWM_MAX_COUNT 8
	extern PWMPinDescription PWM_Desc[];
	//#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
	#define PWM_Config(Desc) \
	do{ \
		outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type); \
		if(GPIO_Desc[Desc.pintype.num].Pin.ALTMsk!=NULL) \
			outp32(&SYS->ALT_MFP,(inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.ALTMsk) | Desc.pintype.AMsk); \
		if(GPIO_Desc[Desc.pintype.num].Pin.ALT1Msk!=NULL) \
			outp32(&SYS->ALT_MFP1,(inp32(&SYS->ALT_MFP1) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT1Msk) | Desc.pintype.AMsk1); \
		if(GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk!=NULL) \
			outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk) | Desc.pintype.AMsk2); \
	}while(0);
#elif defined(__NANO100__)
	#define PWM_MAX_COUNT 8
	extern PWMPinDescription PWM_Desc[];
	#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#elif defined(__NANO1X2__)
   #if defined(__IOT_EVB__)
    #define PWM_MAX_COUNT 1
   #endif
  extern PWMPinDescription PWM_Desc[];
	#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);	
#elif defined(__NUC131__)
#define PWM_MAX_COUNT 10
extern PWMPinDescription PWM_Desc[];
//#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#define PWM_Config(Desc) \
do{ \
	outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type); \
	if(GPIO_Desc[Desc.pintype.num].Pin.ALTMsk!=NULL) \
		outp32(&SYS->ALT_MFP,(inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.ALTMsk) | Desc.pintype.AMsk); \
	if(GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk!=NULL) \
		outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk) | Desc.pintype.AMsk2); \
	if(GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk!=NULL) \
		outp32(&SYS->ALT_MFP3,(inp32(&SYS->ALT_MFP3) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk) | Desc.pintype.AMsk3); \
	if(GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk!=NULL) \
		outp32(&SYS->ALT_MFP4,(inp32(&SYS->ALT_MFP4) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk) | Desc.pintype.AMsk4); \
}while(0);
#endif

#if defined(__M451__)
	#define ADC_MAX_COUNT 11
	extern ADCPinDescription ADC_Desc[];
	#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#elif defined(__NUC240__)
	#define ADC_MAX_COUNT 8
	extern ADCPinDescription ADC_Desc[];
	//#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
	#define ADC_Config(Desc) \
	do { \
		outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type); \
		if(GPIO_Desc[Desc.pintype.num].Pin.ALTMsk!=NULL) \
			outp32(&SYS->ALT_MFP,(inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.ALTMsk) | Desc.pintype.AMsk); \
		if(GPIO_Desc[Desc.pintype.num].Pin.ALT1Msk!=NULL) \
			outp32(&SYS->ALT_MFP1,(inp32(&SYS->ALT_MFP1) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT1Msk) | Desc.pintype.AMsk1); \
		if(GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk!=NULL) \
			outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk) | Desc.pintype.AMsk2); \
	}while(0);
#elif defined(__NANO100__)
	#define ADC_MAX_COUNT 10
	extern ADCPinDescription ADC_Desc[];
	#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#elif defined(__NANO1X2__)
  #if defined(__IOT_EVB__)
	  #define ADC_MAX_COUNT 2
	#endif
	extern ADCPinDescription ADC_Desc[];
	#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);	
#elif defined(__NUC131__)
#define ADC_MAX_COUNT 6
extern ADCPinDescription ADC_Desc[];
//#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#define ADC_Config(Desc) \
do { \
	outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type); \
	if(GPIO_Desc[Desc.pintype.num].Pin.ALTMsk!=NULL) \
		outp32(&SYS->ALT_MFP,(inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.ALTMsk) | Desc.pintype.AMsk); \
	if(GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk!=NULL) \
		outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk) | Desc.pintype.AMsk2); \
	if(GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk!=NULL) \
		outp32(&SYS->ALT_MFP3,(inp32(&SYS->ALT_MFP3) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk) | Desc.pintype.AMsk3); \
	if(GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk!=NULL) \
		outp32(&SYS->ALT_MFP4,(inp32(&SYS->ALT_MFP4) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk) | Desc.pintype.AMsk4); \
}while(0);
#endif


#if defined(__M451__)
	#define SPI_MAX_COUNT 1
	#define SPI_CHANNELS_NUM 1
#elif defined(__NUC240__)
	#define SPI_MAX_COUNT 1
	#define SPI_CHANNELS_NUM 1
#elif defined(__NANO100__)
	#define SPI_MAX_COUNT 3
#elif defined(__NANO1X2__)
  #if defined(__IOT_EVB__)
    #define SPI_MAX_COUNT 0
  #endif
#elif defined(__NUC131__)
#define SPI_MAX_COUNT 1
#define SPI_CHANNELS_NUM 1
#endif
extern SPIPinDescription SPI_Desc[];
#define SPI_SCK   0
#define SPI_MOSI  1
#define SPI_MISO  2
#define SPI_SS    3
#define SPI_Config(Desc) \
do { \
	uint8_t i; \
	for(i=0;i<4;i++) \
	 if(Desc.pintype[i].num != 0) \
	 	outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0); 

#if defined(__VIDUINO_UNO__)
	#define UART_MAX_COUNT 3
#elif defined(__M451__)
#elif defined(__NUC240__)
#elif defined(__NANO100__)  
#elif defined(__NANO1X2__)
#elif defined(__NUC131__)
#endif
extern UARTPinDescription UART_Desc[];
#define UART_RX 0
#define UART_TX 1
#define UART_Config(Desc) \
do { \
	uint8_t i; \
	for(i=0;i<2;i++) \
		outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);

#if defined(__M451__) | defined(__NUC240__)
#define CAN_MAX_COUNT 1
extern CANPinDescription CAN_Desc[];
#define CAN_RX 0
#define CAN_TX 1
#define CAN_Config(Desc) \
do { \
	uint8_t i; \
	for(i=0;i<2;i++) \
		outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);
#endif

#if defined(__M451__) | defined(__NUC240__) | defined(__NANO100__)
#define I2C_MAX_COUNT 2
#elif defined(__NANO1X2__)
	#if defined(__IOT_EVB__)
	  #define I2C_MAX_COUNT 1
	#endif
#elif defined(__NUC131__)
#define I2C_MAX_COUNT 1
#endif
extern I2CPinDescription I2C_Desc[];
#define I2C_SCL 0
#define I2C_SDA 1
#define I2C_Config(Desc) \
do { \
	uint8_t i; \
	for(i=0;i<2;i++) \
		outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);

#if defined(__M451__)
static const uint8_t SS   = 43;
static const uint8_t MOSI = 42;
static const uint8_t MISO = 40;
static const uint8_t SCK  = 41;

static const uint8_t SDA = 14;
static const uint8_t SCL = 15;

static const uint8_t A0  = 0;
static const uint8_t A1  = 1;
static const uint8_t A2  = 2;
static const uint8_t A3  = 3;
static const uint8_t A4  = 4;
static const uint8_t A5  = 5;
static const uint8_t A6  = 6;
static const uint8_t A7  = 7;
static const uint8_t A8  = 8;
static const uint8_t A9  = 9;
//static const uint8_t A10 = 10;
//static const uint8_t A11 = 11;
#elif defined(__NUC240__)
static const uint8_t SS   = 43;
static const uint8_t MOSI = 42;
static const uint8_t MISO = 40;
static const uint8_t SCK  = 41;

static const uint8_t SDA = 14;
static const uint8_t SCL = 15;

static const uint8_t A0 = 0;
static const uint8_t A1 = 1;
static const uint8_t A2 = 2;
static const uint8_t A3 = 3;
static const uint8_t A4 = 4;
static const uint8_t A5 = 5;
static const uint8_t A6 = 6;
static const uint8_t A7 = 7;
//static const uint8_t A8 = 8;
//static const uint8_t A9 = 9;
#elif defined(__NANO100__)
static const uint8_t SS   = 43;
static const uint8_t MOSI = 42;
static const uint8_t MISO = 40;
static const uint8_t SCK  = 41;

static const uint8_t SDA = 14;
static const uint8_t SCL = 15;

static const uint8_t A0 = 0;
static const uint8_t A1 = 1;
static const uint8_t A2 = 2;
static const uint8_t A3 = 3;
static const uint8_t A4 = 4;
static const uint8_t A5 = 5;
static const uint8_t A6 = 6;
static const uint8_t A7 = 7;
static const uint8_t A8 = 8;
static const uint8_t A9 = 9;
#elif defined(__NUC131__)
static const uint8_t SS   = 43;
static const uint8_t MOSI = 42;
static const uint8_t MISO = 40;
static const uint8_t SCK  = 41;

static const uint8_t LED_BUILTIN = 13;
static const uint8_t SDA = 14;
static const uint8_t SCL = 15;

static const uint8_t A0 = 0;
static const uint8_t A1 = 1;
static const uint8_t A2 = 2;
static const uint8_t A3 = 3;
static const uint8_t A4 = 4;
static const uint8_t A5 = 5;
//static const uint8_t A6 = 6;
//static const uint8_t A7 = 7;
//static const uint8_t A8 = 8;
//static const uint8_t A9 = 9;

#elif defined(__NANO1X2__)
  #if defined(__IOT_EVB__)
  	static const uint8_t A0 = 0;
		static const uint8_t A1 = 1;
		static const uint8_t SDA = 12;
    static const uint8_t SCL = 13;
  #endif
#endif  

#endif
