#ifndef __FRAME_BUFFER_H_
#define __FRAME_BUFFER_H_

#include "fb.h"

#ifdef __cplusplus
class FrameBuffer{
public :
    FrameBuffer();
    // ~FrameBuffer();
    int begin(int width = 800, int height = 480, int bright = 100);
    void setBright(int bright);
    void displayFlush(int x1, int x2, int y1, int y2, int* color_p);
private:
    framebuffer_t framebuffer;
};
#endif

#endif
