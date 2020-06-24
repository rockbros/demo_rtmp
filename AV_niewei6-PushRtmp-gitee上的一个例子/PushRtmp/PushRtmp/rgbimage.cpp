#include "rgbimage.h"
#include <string.h>
#define NULL 0
RGBImage::RGBImage(const char *buf, int w, int h)
{
    dataBuf = NULL;
    m_width   = NULL;
    m_height  = NULL;
    if((w >0) && ( h > 0) && buf)
    {
        m_width = w;
        m_height = h;
        int lenBuf = m_width * m_height * 3 ;
        dataBuf =new char[ lenBuf ];
        memset(dataBuf,0,lenBuf);
        memcpy(dataBuf,buf,lenBuf);
    }
}

RGBImage::RGBImage(const RGBImage & Buf)
{
    if( dataBuf )
    {
        delete []dataBuf;
        dataBuf = NULL;
        m_width   = NULL;
        m_height  = NULL;
    }

    if ((Buf.m_width >0 ) && (Buf.m_height >0 ))
    {
        m_width =  Buf.m_width;
        m_height = Buf.m_height;
        int lenBuf = m_width * m_height * 3 ;
        dataBuf =new char[ lenBuf ];
        memset(dataBuf,0,lenBuf);
        memcpy(dataBuf,Buf.dataBuf,lenBuf);
    }
}

RGBImage &RGBImage::operator =(const RGBImage &Buf)
{
    if(this == &Buf)
        return    *this;

    if( dataBuf )
    {
        delete []dataBuf;
        dataBuf = NULL;
        m_width   = NULL;
        m_height  = NULL;
    }
    //    赋值操作...
    if ((Buf.m_height >0 ) && (Buf.m_width >0 ))
    {
        m_width =  Buf.m_width;
        m_height = Buf.m_height;
        int lenBuf = m_width * m_height * 3 ;
        dataBuf =new char[ lenBuf ];
        memset(dataBuf,0,lenBuf);
        memcpy(dataBuf,Buf.dataBuf,lenBuf);
    }
    return    *this  ;
}

char *RGBImage::Buf()
{
    return dataBuf;
}

int RGBImage::width()
{
    return m_width;
}

int RGBImage::height()
{
    return m_height;
}

RGBImage::~RGBImage()
{
    if( dataBuf )
    {
        delete []dataBuf;
        dataBuf = NULL;
        m_width   = NULL;
        m_height  = NULL;
    }
}

