#pragma once

#include "Arduino.h"

class Uart : public HardwareSerial {
public:
    Uart(UARTPinDescription* uartDesc);
    Uart() {}

    void begin(unsigned long baudrate);
    void begin(unsigned long baudrate, uint16_t config);
    void end();
    int available(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);

    Uart& operator=(Uart const&) = default; 

    operator bool() { return 1; }

private:
    UARTPinDescription* uartDesc;
};

extern Uart Serial;
extern Uart Serial1;
extern Uart Serial2;