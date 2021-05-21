#ifndef __KEY_BOARD_H_
#define __KEY_BOARD_H_

#include "usb_keyboard.h"

#ifdef __cplusplus
class Mp3Player
{
public:
    Mp3Player();
    // ~Mp3Player();
    int begin();
    void playBuffer(uint8_t *buffer, uint32_t len);
    void playFile(char *file_path);

private:
};
#endif

#endif
