/*
The MIT License (MIT)

Copyright (c) wenjie.zhao

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef TIMESTAMPBULIDER_H
#define TIMESTAMPBULIDER_H
#include "Active_Object/CriticalSection.h"
#include "Receive_handler.h"
#include<string>
class BleTimestampBulider: public Receive_handler
{
public:
    BleTimestampBulider(Receive_handler * videoCaptureHandler,Receive_handler * RtmpSendTaskHandler);

    void setVideoCaptureInternal(float internal);
    void setAudioCaptureInternal(float internal);

    inline float videoInternal() { return m_videoInternal; }

    double addAudioFrame();
    double addVideoFrame();

    virtual bool receive_handler(const char *data,int size);
private:
    double next_video_pts();
    double next_audio_pts();
private:
    Receive_handler    *m_pVideoCaptureHandler; //视频采集接口
    Receive_handler    *m_pRtmpSendTaskHandler; //rtmp发送接口
    CCriticalSection   m_poCriSection;

    float m_videoInternal;
    float m_audioInternal;

    double m_videoTimestamp;
    double m_audioTimestamp;
};

#endif // TIMESTAMPBULIDER_H
