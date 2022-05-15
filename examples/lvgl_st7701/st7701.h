#ifndef ST7701_H__
#define ST7701_H__

#define CS_PORT GPIOE
#define CS_PIN 8

#define SCK_PORT GPIOE
#define SCK_PIN 9

#define SDA_PORT GPIOE
#define SDA_PIN 10

void st7701_init(void);

#endif