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

#include "TimestampBulider.h"
#include "avpacket.h"

BleTimestampBulider::BleTimestampBulider(Receive_handler * videoCaptureHandler,Receive_handler * RtmpSendTaskHandler)
    : m_pVideoCaptureHandler(videoCaptureHandler)
    , m_pRtmpSendTaskHandler(RtmpSendTaskHandler)
    , m_videoInternal(66.66666666666667)        // default 15fps
    , m_audioInternal(23.2199546485261)          // default aac 44100Hz
    , m_videoTimestamp(0.00)
    , m_audioTimestamp(0.00)
{

}

void BleTimestampBulider::setVideoCaptureInternal(float internal)
{
    m_videoInternal = internal;
}

void BleTimestampBulider::setAudioCaptureInternal(float internal)
{
    m_audioInternal = internal;
}

double BleTimestampBulider::addVideoFrame()
{
    // never go to here.
    return 0.0;
}

bool BleTimestampBulider::receive_handler(const char *data, int size)
{
    if (size)
    {
        AudioPacket *pkt = new AudioPacket;
        pkt->data.writeString(MString(data, size));

        pkt->dts = addAudioFrame();
        pkt->has_encoded = true;

        if(m_pRtmpSendTaskHandler)
            m_pRtmpSendTaskHandler->receive_handler((const char *)&pkt,sizeof(pkt));
    }
    return true;
}

double BleTimestampBulider::addAudioFrame()
{
    CCriticalAutoLock loAutoLock(m_poCriSection);

    while (next_audio_pts() >= next_video_pts())
    {
        m_videoTimestamp = next_video_pts();

        VideoPacket *pkt = new VideoPacket;
        pkt->dts = m_videoTimestamp;
        pkt->has_encoded = false;

        if(m_pVideoCaptureHandler)
            m_pVideoCaptureHandler->receive_handler((const char *)&pkt,sizeof(pkt));//发送到图像采集队列

        if(m_pRtmpSendTaskHandler)
            m_pRtmpSendTaskHandler->receive_handler((const char *)&pkt,sizeof(pkt));//发送到rtmp发送队列

    }

    m_audioTimestamp = next_audio_pts();
    return m_audioTimestamp;
}

double BleTimestampBulider::next_video_pts()
{
    return m_videoTimestamp + m_videoInternal;
}

double BleTimestampBulider::next_audio_pts()
{
    return m_audioTimestamp + m_audioInternal;
}
