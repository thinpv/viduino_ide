#ifndef ST7701_H__
#define ST7701_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CS_PORT GPIOE
#define CS_PIN 8

#define SCK_PORT GPIOE
#define SCK_PIN 9

#define SDA_PORT GPIOE
#define SDA_PIN 10

// #define RST_PORT GPIOE
// #define RST_PIN 7

void st7701_init(void);

#ifdef __cplusplus
}
#endif
#endif