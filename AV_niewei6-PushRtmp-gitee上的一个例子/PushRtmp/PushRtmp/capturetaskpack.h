#ifndef CAPTURETASKPACK
#define CAPTURETASKPACK
class AVPacket;
class RGBImage;
typedef struct
{
    RGBImage            *image ;			//
    AVPacket            *packet;			//
}CAPTURE_TASK_PACK;
#endif // CAPTURETASKPACK

