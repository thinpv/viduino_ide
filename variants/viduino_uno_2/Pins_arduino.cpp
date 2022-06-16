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
#include "Arduino.h"

// GPIOPinDescription GPIO_Desc[]=
// {
// 	{GPIOA,0 ,{0}}, //PA0 //16
// 	{GPIOA,1 ,{0}}, //PA1 //17
// 	{GPIOA,2 ,{0}}, //PA2 //0
// 	{GPIOA,3 ,{0}}, //PA3 //1
// 	{GPIOC,0 ,{0}}, //PC0 //16
// 	{GPIOC,1 ,{0}}, //PC1 //17
// 	{GPIOC,2 ,{0}}, //PC2 //0
// 	{GPIOC,3 ,{0}}, //PC3 //1
// 	{GPIOD,0 ,{0}}, //PD0 //48
// 	{GPIOD,1 ,{0}}, //PD1 //49
// 	{GPIOD,2 ,{0}}, //PD2 //46
// 	{GPIOD,3 ,{0}}, //PD3 //47
// 	{GPIOD,4 ,{0}}, //PD4 //44
// 	{GPIOD,5 ,{0}}, //PD5 //45
// 	{GPIOD,6 ,{0}}, //PD6 //42
// 	{GPIOD,7 ,{0}}, //PD7 //43
// 	{GPIOD,8 ,{0}}, //PD8 //40
// 	{GPIOD,9 ,{0}}, //PD9 //41
// 	{GPIOD,10,{0}}, //PD10 //38
// 	{GPIOD,11,{0}}, //PD11 //39
// 	{GPIOD,12,{0}}, //PD12 //36
// 	{GPIOD,13,{0}}, //PD13 //37
// 	{GPIOD,14,{0}}, //PD14 //34
// 	{GPIOD,15,{0}}, //PD15 //35
// 	{GPIOD,16,{0}}, //PD16 //32
// 	{GPIOD,17,{0}}, //PD17 //33
// 	{GPIOD,18,{0}}, //PD18 //30
// 	{GPIOD,19,{0}}, //PD19 //31
// 	{GPIOD,20,{0}}, //PD20 //28
// 	{GPIOD,21,{0}}, //PD21 //29

// 	{NULL ,0 ,{0}}, //2
// 	{GPIOE,0 ,{0}}, //PE0 //3
// 	{GPIOE,1 ,{0}}, //PE1 //4
// 	{GPIOE,2 ,{0}}, //PE2 //5
// 	{GPIOE,3 ,{0}}, //PE3 //6
// 	{GPIOE,4 ,{0}}, //PE4 //7
// 	{GPIOE,5 ,{0}}, //PE5 //8
// 	{GPIOE,6 ,{0}}, //PE6 //9
// 	{GPIOE,7 ,{0}}, //PE7 //10 //spi
// 	{GPIOE,8 ,{0}}, //PE8 //11 //spi
// 	{GPIOE,10,{0}}, //PE10 //12 //spi
// 	{GPIOE,9 ,{0}}, //PE9 //13 //spi
// 	{GPIOE,12,{0}}, //PE12 //14 //i2c
// 	{GPIOE,11,{0}}, //PE11 //15 //i2c
// 	// {NULL ,0 ,{0}}, //16
// 	// {NULL ,0 ,{0}}, //17
// 	{NULL ,0 ,{0}}, //18
// 	{NULL ,0 ,{0}}, //19
// 	{NULL ,0 ,{0}}, //20
// 	{NULL ,0 ,{0}}, //21
// 	{GPIOF,4 ,{0}}, //PF4 //22
// 	{GPIOF,5 ,{0}}, //PF5 //23
// 	{GPIOF,2 ,{0}}, //PF2 //24
// 	{GPIOF,3 ,{0}}, //PF3 //25
// 	{GPIOF,0 ,{0}}, //PF0 //26
// 	{GPIOF,1 ,{0}}, //PF1 //27

// 	// {GPIOC,2 ,{0}}, //PC2 //6
// 	// {GPIOC,3 ,{0}}, //PC3 //7
// 	// {GPIOC,0 ,{0}}, //PC0 //4
// 	// {GPIOC,1 ,{0}}, //PC1 //5
// };

SPIPinDescription SPI_Desc[]={
	// {SPI0, {}, 0, {
	// 	{GPIOC,0 ,2},
	// 	{GPIOC,3 ,2},
	// 	{GPIOC,2 ,2},
	// 	{GPIOC,1 ,2}}
	// },
	{SPI1, {}, 0, {
		{GPIOE9 ,4},
		{GPIOE8 ,4},
		{GPIOE10 ,4},
		{GPIOE7 ,4}}
	},
};

UARTPinDescription UART_Desc[]={
	{UART1, {}, {
		{GPIOA2, 5},
		{GPIOA3, 5}}
	},
	{UART0, {}, {
		{GPIOE0, 5},
		{GPIOE1, 5}}
	},
	{UART2, {}, {
		{GPIOE8, 3},
		{GPIOE7, 3}}
	},
};

I2CPinDescription I2C_Desc[]={
	// {I2C0, {}, {
	// 	{GPIOE11, 3},
	// 	{GPIOE12, 3}}
	// },
	{I2C0, {}, {
		{GPIOD12, 3},
		{GPIOD0, 3}}
	},
	{I2C1, {}, {
		{GPIOD5, 3},
		{GPIOD6, 3}}
	},
	{I2C2, {}, {
		{GPIOE0, 4},
		{GPIOE1, 4}}
	},
	// {I2C2, {}, {
	// 	{GPIOD15, 4},
	// 	{GPIOD16, 4}}
	// },
};
