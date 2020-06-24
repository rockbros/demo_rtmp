#include "X264EncoderImpl.h"
#include "videoencodetask.h"
#include "capturetaskpack.h"
#include "convert.h"
#include "avpacket.h"
#include "rgbimage.h"
#include "string.h"

VideoEncodeTask::VideoEncodeTask()
{
    m_videoEncoder = NULL;
}

VideoEncodeTask::~VideoEncodeTask()
{
    close();
}
bool VideoEncodeTask::init()
{
    if(!m_videoEncoder)
    {
        m_videoEncoder = new X264EncoderImpl;       //X264 编码实现
        if(m_videoEncoder)
        {
            m_videoEncoder->init();
            return true ;
        }
    }
    return false;
}

string VideoEncodeTask::getX264Header()
{
    if(m_videoEncoder)
        return m_videoEncoder->getX264Header();

    return string("");
}

string VideoEncodeTask::getX264SeiInfo()
{
    if(m_videoEncoder)
        return m_videoEncoder->getX264SeiInfo();

    return string("");
}

float VideoEncodeTask::getFrameDuration()
{
    if(m_videoEncoder)
       return m_videoEncoder->getFrameDuration();

    return -1;
}

int VideoEncodeTask::close()
{
    std::string str("");

    int nthread  =thr_count();

    for (int i =0 ;i<nthread ;i++)
        putq(str);

    wait();

    if(m_videoEncoder)
        m_videoEncoder->fini();
     m_videoEncoder = NULL;
    return true;
}

bool VideoEncodeTask::receive_handler(const char *data, int size)
{
    putq(std::string(data,size));
    return true;
}

int VideoEncodeTask::svc()
{
    if(m_videoEncoder)
    {
        std::string str ="";
        ColorSpaceConversions Conversions;
         while(getq(str))
         {
             if (str=="") break;

             CAPTURE_TASK_PACK   *task_pack = (CAPTURE_TASK_PACK   *)str.data();
             RGBImage            *image  = task_pack->image;			//
             AVPacket            *packet = task_pack->packet;			//

             int buflen = image->height() * image->width() * 3 ;

             unsigned char  *YuvBuf=new unsigned char[buflen / 2];
             memset(YuvBuf,0,sizeof(buflen / 2));

             Conversions.ConvertRGB2YUV(image->width(),image->height(),(unsigned char *)image->Buf(),YuvBuf);//RGB 转换YUV格式

             if(m_videoEncoder->encode(YuvBuf,packet->data,5) == 0)//5用在有B帧的时候
             {
                 packet->has_encoded = true ;   //编码视频帧完成
             }
              packet->has_encoded = true ;   //编码视频帧完成
             delete image;  //销毁图像buf
             delete []YuvBuf;
         }
    }
    return 0;
}

