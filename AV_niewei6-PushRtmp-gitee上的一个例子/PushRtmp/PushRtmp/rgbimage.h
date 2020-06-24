#ifndef RGBIMAGE_H
#define RGBIMAGE_H


class RGBImage
{
public:
    RGBImage(const char *buf,int w,int h);
    RGBImage(const RGBImage & );
    RGBImage & operator = (const RGBImage & );
public:
    char * Buf();
    int width();
    int height();
public:
    ~RGBImage();
private:
    int     m_width;
    int     m_height;
    char    *dataBuf;
};

#endif // RGBIMAGE_H
