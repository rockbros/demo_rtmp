#if !defined _CONVERT_H
#define _CONVERT_H

#include<stdio.h>

class ColorSpaceConversions
{
public:
    ColorSpaceConversions();

    // Conversion from RGB24 to YUV420

    int  ConvertRGB2YUV(int w,int h, unsigned char *rgbdata, unsigned char *yuv);

    // Conversion from YUV420 to RGB24

    void ConvertYUV2RGB(unsigned char *src0,unsigned char *src2,unsigned char *src1,unsigned char *dst_ori,
                        int width,int height);

    void RGB24toYUV420(unsigned char *lum, unsigned char *cr, unsigned char *cb, unsigned char *src, int width, int height);

private:
    void InitLookupTable();
    void InitConvertTable();
private:
    int RGB2YUV_YR[256], RGB2YUV_YG[256], RGB2YUV_YB[256];
    int RGB2YUV_UR[256], RGB2YUV_UG[256], RGB2YUV_UBVR[256];
    int RGB2YUV_VG[256], RGB2YUV_VB[256];
    // Conversion from YUV420 to RGB24
    long int crv_tab[256];
    long int cbu_tab[256];
    long int cgu_tab[256];
    long int cgv_tab[256];
    long int tab_76309[256];
    unsigned char clp[1024]; //for clip in CCIR601
};


#endif
