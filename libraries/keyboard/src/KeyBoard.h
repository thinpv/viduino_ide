#ifndef __KEY_BOARD_H_
#define __KEY_BOARD_H_

#include "usb_keyboard.h"

#ifdef __cplusplus
class KeyBoard{
public :
    KeyBoard();
    // ~KeyBoard();
    int begin();
    int write(char);
    int getStatus();
private:
};
#endif

#endif
