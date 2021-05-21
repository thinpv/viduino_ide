#include "Mp3Player.h"
#include "mp3_lib.h"

Mp3Player::Mp3Player()
{
}

// ~Mp3Player();
int Mp3Player::begin()
{
    play_mp3_init();
}

void Mp3Player::playBuffer(uint8_t *buffer, uint32_t len)
{
    play_mp3_buffer(buffer, len);
}

void Mp3Player::playFile(char *file_path)
{
    play_mp3_file(file_path);
}
