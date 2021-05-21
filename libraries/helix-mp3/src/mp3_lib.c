#include "mp3_lib.h"
#include "pub/mp3dec.h"
#include "pub/mp3common.h"
#include <audio.h>
#include <string.h>

void print_id3v2(ID3v2_t *id3v2)
{
  printf("TIT2: %s\r\n", id3v2->TIT2);
  printf("TPE1: %s\r\n", id3v2->TPE1);
  printf("TALB: %s\r\n", id3v2->TALB);
}

void print_id3v1(ID3v1_t *id3v1)
{
  printf("TIT2: %s\r\n", id3v1->TIT2);
  printf("TPE1: %s\r\n", id3v1->TPE1);
  printf("TALB: %s\r\n", id3v1->TALB);
}

uint32_t get_id3v2(uint8_t *buffer, uint32_t len, ID3v2_t *id3v2)
{
  uint32_t i, l;
  uint32_t size, s;
  uint8_t TIT2_F = 0;
  uint8_t TPE1_F = 0;
  uint8_t TALB_F = 0;
  for (i = 0; i < 50; i++)
  {
    if (buffer[i + 0] == 'I' && buffer[i + 1] == 'D' && buffer[i + 2] == '3')
    {
      size = ((buffer[i + 9] & 0x7f) | ((buffer[i + 8] & 0x7f) << 7) | ((buffer[i + 7] & 0x7f) << 14) | ((buffer[i + 6] & 0x7f) << 21)) + 10;
      s = size;
      if (id3v2)
      {
        id3v2->version = buffer[i + 3] * 10 + buffer[i + 4];
        printf("ID3V%d.%d \r\n", id3v2->version / 10, id3v2->version % 10);
        printf("label=%d\r\n", size);
        if (size > 5000)
          size = 5000; //Super long ID3 may have pictures
        for (; i < size; i++)
        {
          if (TIT2_F == 0)
            if ((buffer[i + 0] == 'T') && (buffer[i + 1] == 'I') && (buffer[i + 2] == 'T') && (buffer[i + 3] == '2')) //TIT2
            {
              printf("TIT2=%d \r\n", i);
              uint32_t TIT2_size = buffer[i + 4 + 0] << 24 | buffer[i + 4 + 1] << 16 | buffer[i + 4 + 2] << 8 | buffer[i + 4 + 3] << 0;
              for (l = 0; l < (TIT2_size - 3); l++)
                id3v2->TIT2[l] = buffer[i + 10 + 3 + l];
              TIT2_F = 1;
            }
          if (TPE1_F == 0)
            if ((buffer[i + 0] == 'T') && (buffer[i + 1] == 'P') && (buffer[i + 2] == 'E') && (buffer[i + 3] == '1')) //TPE1
            {
              printf("TPE1=%d \r\n", i);
              uint32_t TPE1_size = buffer[i + 4 + 0] << 24 | buffer[i + 4 + 1] << 16 | buffer[i + 4 + 2] << 8 | buffer[i + 4 + 3] << 0;
              for (l = 0; l < (TPE1_size - 3); l++)
                id3v2->TPE1[l] = buffer[i + 10 + 3 + l];
              TPE1_F = 1;
            }
          if (TALB_F == 0)
            if ((buffer[i + 0] == 'T') && (buffer[i + 1] == 'A') && (buffer[i + 2] == 'L') && (buffer[i + 3] == 'B')) //TALB
            {
              printf("TALB=%d \r\n", i);
              uint32_t TALB_size = buffer[i + 4 + 0] << 24 | buffer[i + 4 + 1] << 16 | buffer[i + 4 + 2] << 8 | buffer[i + 4 + 3] << 0;
              for (l = 0; l < (TALB_size - 3); l++)
                id3v2->TALB[l] = buffer[i + 10 + 3 + l];
              TALB_F = 1;
            }
        }
      }
      return s;
    }
  }
  return 0;
}

uint32_t get_id3v1(uint8_t *input_buffer, uint32_t len, ID3v1_t *id3v1)
{
  uint32_t l;
  if ((input_buffer[0] == 'T') && (input_buffer[1] == 'A') && (input_buffer[2] == 'G'))
  {
    if (id3v1)
    {
      printf("ID3V1\r\n");
      for (l = 0; l < 30; l++)
      {
        id3v1->TIT2[l] = input_buffer[3 + l];
        id3v1->TPE1[l] = input_buffer[33 + l];
        id3v1->TALB[l] = input_buffer[63 + l];
      }
    }
    return 128;
  }
  return 0;
}

uint32_t get_mp3_play_time_ms(uint8_t *input_buffer, uint32_t len, MP3FrameInfo *mp3_frame_info)
{
  uint32_t i, time;
  uint32_t total_frame;
  for (i = 0; i < 60; i++)
  {
    if (((input_buffer[i + 0] == 'X') && (input_buffer[i + 1] == 'i') && (input_buffer[i + 2] == 'n') && (input_buffer[i + 3] == 'g')) ||
        ((input_buffer[i + 0] == 'I') && (input_buffer[i + 1] == 'n') && (input_buffer[i + 2] == 'f') && (input_buffer[i + 3] == 'o')))
    {
      /* Read the total number of frames */
      total_frame = input_buffer[i + 8 + 0] << 24 | input_buffer[i + 8 + 1] << 16 | input_buffer[i + 8 + 2] << 8 | input_buffer[i + 8 + 3] << 0;
      printf("Xing=%d \r\n", i);
      /* Calculate the playing time */
      time = (float)total_frame * ((float)1000000 / (float)mp3_frame_info->samprate * (float)samplesPerFrameTab[mp3_frame_info->version][mp3_frame_info->layer - 1]) / (float)1000;
      return time;
    }
    if ((input_buffer[i + 0] == 'V') && (input_buffer[i + 1] == 'B') && (input_buffer[i + 2] == 'R') && (input_buffer[i + 3] == 'I'))
    {
      printf("VBRI=%d \r\n", i);
      /* Read the total number of frames */
      total_frame = input_buffer[i + 14 + 0] << 24 | input_buffer[i + 14 + 1] << 16 | input_buffer[i + 14 + 2] << 8 | input_buffer[i + 14 + 3] << 0;
      /* Calculate the playing time */
      time = (float)total_frame * ((float)1000000 / (float)mp3_frame_info->samprate * (float)samplesPerFrameTab[mp3_frame_info->version][mp3_frame_info->layer - 1]) / (float)1000;
      return time;
    }
  }
  //CBR fixed bit rate

  // Playing time = (file size-ID3 size) × 8 ÷ bit rate (kbit/s)
  time = len * 8 / (mp3_frame_info->bitrate / 1000);
  return time;
}

#define MP3_BUFFER_SIZE (1024 * 2)
__attribute__((aligned(4))) uint8_t mp3_buffer[MP3_BUFFER_SIZE];
// uint32_t mp3_position_read;
uint32_t mp3_byte_left;
uint32_t addr_file_to_read;
uint8_t end_of_file;
uint32_t id3v1, id3v2;

static void mp3_decode_init_param(uint32_t m_id3v1, uint32_t m_id3v2)
{
  id3v1 = m_id3v1;
  id3v2 = m_id3v2;
  addr_file_to_read = id3v2;
  // mp3_position_read = 0;
  mp3_byte_left = 0;
  end_of_file = 0;
}

uint32_t mp3_decode_data(HMP3Decoder mp3_decoder, MP3FrameInfo *mp3_frame_info, uint8_t *output_buffer)
{
  uint8_t *read_ptr;
  uint32_t offset = MP3FindSyncWord(mp3_buffer, mp3_byte_left);
  if (offset >= 0)
  {
    mp3_byte_left -= offset;
    read_ptr = mp3_buffer + offset;
    int res = MP3Decode(mp3_decoder, &read_ptr, (int *)&mp3_byte_left, (short *)output_buffer, 0);
    if (res < 0)
    {
      if (res == ERR_MP3_INVALID_HUFFCODES) //Ignored errors
      {
        printf("Warning: ERR_MP3_INVALID_HUFFCODES\r\n");
        return mp3_frame_info->outputSamps * 2;
      }
      printf("ERR: MP3Decode res %d\r\n", res);
      return -2;
    }
  }
  else
  {
    printf("ERR: Unfound header.\r\n");
    return -3; //No frame header
  }
  return mp3_frame_info->outputSamps * 2;
}

int mp3_decode_buffer(HMP3Decoder mp3_decoder, MP3FrameInfo *mp3_frame_info, uint8_t *input_buffer, uint32_t input_size, uint8_t *output_buffer)
{
  uint32_t end_of_data_file = input_size - id3v1;
  if (mp3_byte_left < MAINBUF_SIZE * 2 && end_of_file == 0)
  {
    memcpy(mp3_buffer, mp3_buffer + (MP3_BUFFER_SIZE - mp3_byte_left), mp3_byte_left);
    if (addr_file_to_read + MP3_BUFFER_SIZE - mp3_byte_left >= end_of_data_file)
    { //File exceeds
      memcpy(mp3_buffer + mp3_byte_left, input_buffer + addr_file_to_read, end_of_data_file - addr_file_to_read);
      mp3_byte_left += end_of_data_file - addr_file_to_read;
      end_of_file = 1;
    }
    else
    { //The file does not exceed
      memcpy(mp3_buffer + mp3_byte_left, input_buffer + addr_file_to_read, MP3_BUFFER_SIZE - mp3_byte_left);
      addr_file_to_read += MP3_BUFFER_SIZE - mp3_byte_left;
      mp3_byte_left = MP3_BUFFER_SIZE;
    }
  }
  return mp3_decode_data(mp3_decoder, mp3_frame_info, output_buffer);
}

int mp3_decode_file(HMP3Decoder mp3_decoder, MP3FrameInfo *mp3_frame_info, FIL *f, uint8_t *output_buffer)
{
  UINT br;
  if (mp3_byte_left < MAINBUF_SIZE * 2 && end_of_file == 0)
  {
    memcpy(mp3_buffer, mp3_buffer + (MP3_BUFFER_SIZE - mp3_byte_left), mp3_byte_left);
    f_lseek(f, addr_file_to_read);
    f_read(f, mp3_buffer + mp3_byte_left, MP3_BUFFER_SIZE - mp3_byte_left, &br);
    if (br != MP3_BUFFER_SIZE - mp3_byte_left)
    {
      mp3_byte_left += br;
      end_of_file = 1;
    }
    else
    {
      addr_file_to_read += MP3_BUFFER_SIZE - mp3_byte_left;
      mp3_byte_left = MP3_BUFFER_SIZE;
    }
  }
  return mp3_decode_data(mp3_decoder, mp3_frame_info, output_buffer);
}

uint8_t mp3_get_frame_info(HMP3Decoder mp3_decoder, MP3FrameInfo *mp3_frame_info, uint8_t *input_buffer, uint32_t byte_left)
{
  uint32_t offset;
  uint32_t frame_size;
  uint32_t padding = 0;
  offset = MP3FindSyncWord(input_buffer, byte_left);
  if (offset < 0)
    printf("MP3FindSyncWord failed!\r\n");

  if (ERR_MP3_NONE == MP3GetNextFrameInfo(mp3_decoder, mp3_frame_info, input_buffer))
  {
    /* Print header information */
    printf("FrameInfo bitrate=%d \r\n", mp3_frame_info->bitrate);
    printf("FrameInfo nChans=%d \r\n", mp3_frame_info->nChans);
    printf("FrameInfo samprate=%d \r\n", mp3_frame_info->samprate);
    printf("FrameInfo bitsPerSample=%d \r\n", mp3_frame_info->bitsPerSample);
    printf("FrameInfo outputSamps=%d \r\n", mp3_frame_info->outputSamps);
    printf("FrameInfo layer=%d \r\n", mp3_frame_info->layer);
    printf("FrameInfo version=%d \r\n", mp3_frame_info->version);
    /* Calculate the data frame size Frame size = (number of samples per frame × bit rate (bit/s) ÷ 8 ÷ sampling rate) + Padding */
    frame_size = (int)samplesPerFrameTab[mp3_frame_info->version][mp3_frame_info->layer - 1] * mp3_frame_info->bitrate / 8 / mp3_frame_info->samprate + padding;
    printf("FrameInfo frame_size=%d \r\n", frame_size);
    // Return to playing time information
    uint32_t t = get_mp3_play_time_ms(input_buffer, byte_left, mp3_frame_info);
    uint32_t mp3_play_time = t / 1000;
    printf("play time=%02d:%02d:%02d \r\n", mp3_play_time / 3600, mp3_play_time % 3600 / 60, mp3_play_time % 3600 % 60);
    // return 0;
  }
  else
  {
    printf("Unfound FrameInfo.\r\n");
    if (get_fw() == 0) //Change the frame flag and re-acquire the frame header information
    {
      set_fw(1);
      return mp3_get_frame_info(mp3_decoder, mp3_frame_info, input_buffer, byte_left);
    }
    else
    {
      return 1;
    }
  }

  if ((mp3_frame_info->layer < 3) || (mp3_frame_info->bitrate <= 0) || (mp3_frame_info->samprate <= 0))
  {
    printf("ERR:It's not in mp3 format.\r\n");
    return 1;
  }
  return 0;
}

static HMP3Decoder mp3_decoder;
static MP3FrameInfo mp3_frame_info;
static wav_st wav;
int play_mp3_init()
{
  audio_init();
  mp3_decoder = MP3InitDecoder();
  if (mp3_decoder == 0)
  {
    printf("ERR: Init Mp3Decoder failed!\r\n");
    return 1;
  }
  return 0;
}

#define WAV_BUFFER_SIZE 10000
__attribute__((aligned(4))) uint8_t wav_buffer[2 * WAV_BUFFER_SIZE];
int wav_size_buffer;
void play_mp3_buffer(uint8_t *input_buffer, uint32_t input_size)
{
  uint32_t id3v1_size;
  uint32_t id3v2_size;
  uint8_t rs;

  printf("play_mp3_buffer len: %d\r\n", input_size);
  id3v1_size = get_id3v1(input_buffer + input_size - 129, 128, 0);
  printf("id3v1_size: %d\r\n", id3v1_size);
  id3v2_size = get_id3v2(input_buffer, input_size, 0);
  printf("id3v2_size: %d\r\n", id3v2_size);

  rs = mp3_get_frame_info(mp3_decoder, &mp3_frame_info, input_buffer + id3v2_size, input_size - id3v2_size - id3v1_size);
  if (rs != 0)
  {
    printf("ERR: Get Frame Info failed!\r\n");
    return;
  }

  wav.audio_format = 1;
  wav.num_channels = mp3_frame_info.nChans;
  wav.bits_per_sample = mp3_frame_info.bitsPerSample;
  wav.sample_rate = mp3_frame_info.samprate;

  if (audio_play_init(&wav) != 0)
  {
    printf("ERR: audio_play_wav_init failed!\r\n");
    return;
  }

  printf("Start decode mp3\r\n");

  mp3_decode_init_param(id3v1_size, id3v2_size);
  wav_size_buffer = mp3_decode_buffer(mp3_decoder, &mp3_frame_info, input_buffer, input_size, wav_buffer);
  printf("wav_size_buffer: %d\r\n", wav_size_buffer);
  wav_size_buffer = mp3_decode_buffer(mp3_decoder, &mp3_frame_info, input_buffer, input_size, wav_buffer + wav_size_buffer);

  uint8_t wait_half_buff = 0;
  dma_init();
  audio_set_dma_data(0, (uint32_t *)wav_buffer, 2 * wav_size_buffer, 1);
  audio_enable();
  while (1)
  {
    if (dma_ndma_get_full_tip(0))
    {
      if (wait_half_buff == 0)
      {
        wav_size_buffer = mp3_decode_buffer(mp3_decoder, &mp3_frame_info, input_buffer, input_size, wav_buffer + wav_size_buffer);
        if (wav_size_buffer > 0)
        {
          // sysFlushCache(D_CACHE);
        }
        else
          wait_half_buff = 1;
      }
      else
      {
        wait_half_buff = 2;
      }
    }
    if (dma_ndma_get_half_tip(0))
    {
      if (wait_half_buff == 0)
      {
        wav_size_buffer = mp3_decode_buffer(mp3_decoder, &mp3_frame_info, input_buffer, input_size, wav_buffer);
        if (wav_size_buffer > 0)
        {
          // sysFlushCache(D_CACHE);
        }
        else
          wait_half_buff = 1;
      }
      else
      {
        wait_half_buff = 2;
      }
    }
    if (wait_half_buff == 2)
    {
      printf("mp3 play end...\r\n");
      break;
    }
  }
  audio_disable();
  dma_exit();
}

void play_mp3_file(char *file_path)
{
  uint32_t id3v1_size;
  uint32_t id3v2_size;
  uint8_t rs;

  FIL f;
  uint32_t input_size;
  uint8_t id3v1_buff[128];
  uint8_t id3v2_buff[10000];
  FRESULT f_rs = f_open(&f, file_path, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
  if (f_rs != FR_OK)
  {
    printf("ERR: f_open file failed!\r\n");
    return;
  }

  input_size = f_size(&f);
  printf("play_mp3_buffer input_size: %d\r\n", input_size);
  f_read(&f, id3v2_buff, sizeof(id3v2_buff), NULL);
  f_lseek(&f, input_size - sizeof(id3v1_buff));
  f_read(&f, id3v1_buff, sizeof(id3v1_buff), NULL);

  id3v1_size = get_id3v1(id3v1_buff, sizeof(id3v1_buff), 0);
  printf("id3v1_size: %d\r\n", id3v1_size);
  id3v2_size = get_id3v2(id3v2_buff, sizeof(id3v2_buff), 0);
  printf("id3v2_size: %d\r\n", id3v2_size);

  f_lseek(&f, id3v2_size);
  f_read(&f, id3v2_buff, sizeof(id3v2_buff), NULL);

  rs = mp3_get_frame_info(mp3_decoder, &mp3_frame_info, id3v2_buff, sizeof(id3v2_buff));
  if (rs != 0)
  {
    printf("ERR: Get Frame Info failed!\r\n");
    return;
  }

  wav.audio_format = 1;
  wav.num_channels = mp3_frame_info.nChans;
  wav.bits_per_sample = mp3_frame_info.bitsPerSample;
  wav.sample_rate = mp3_frame_info.samprate;
  if (audio_play_init(&wav) != 0)
  {
    printf("ERR: audio_play_wav_init failed!\r\n");
    return;
  }

  printf("Start decode mp3\r\n");

  mp3_decode_init_param(id3v1_size, id3v2_size);
  wav_size_buffer = mp3_decode_file(mp3_decoder, &mp3_frame_info, &f, wav_buffer);
  printf("wav_size_buffer: %d\r\n", wav_size_buffer);
  wav_size_buffer = mp3_decode_file(mp3_decoder, &mp3_frame_info, &f, wav_buffer + wav_size_buffer);

  uint8_t wait_half_buff = 0;
  dma_init();
  audio_set_dma_data(0, (uint32_t *)wav_buffer, 2 * wav_size_buffer, 1);
  audio_enable();
  while (1)
  {
    if (dma_ndma_get_full_tip(0))
    {
      if (wait_half_buff == 0)
      {
        wav_size_buffer = mp3_decode_file(mp3_decoder, &mp3_frame_info, &f, wav_buffer + wav_size_buffer);
        if (wav_size_buffer > 0)
        {
          // sysFlushCache(D_CACHE);
        }
        else
          wait_half_buff = 1;
      }
      else
      {
        wait_half_buff = 2;
      }
    }
    if (dma_ndma_get_half_tip(0))
    {
      if (wait_half_buff == 0)
      {
        wav_size_buffer = mp3_decode_file(mp3_decoder, &mp3_frame_info, &f, wav_buffer);
        if (wav_size_buffer > 0)
        {
          // sysFlushCache(D_CACHE);
        }
        else
          wait_half_buff = 1;
      }
      else
      {
        wait_half_buff = 2;
      }
    }
    if (wait_half_buff == 2)
    {
      printf("mp3 play end...\r\n");
      break;
    }
  }
  audio_disable();
  dma_exit();
}
