#ifndef __MP3_PLAYER_H_
#define __MP3_PLAYER_H_

#include "types.h"

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
