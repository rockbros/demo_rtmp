#ifndef RTMPSENDTASK_H
#define RTMPSENDTASK_H

#include <Receive_handler.h>
#include <Active_Object/Task_Base.h>
class RtmpMuxerImpl;
class BleTimestampBulider;
class MStream;
class AVPacket;
class RtmpSendTask: public Task_Base ,public Receive_handler
{
public:
    RtmpSendTask();
    ~RtmpSendTask();
public:
    void setVideoSh(std::string &);
    void setVideoSeiInfo(std::string &);
    void setAudioSh(std::string &);
public:
    virtual int close();
    virtual bool receive_handler(const char *data,int size);
protected:
    virtual int svc();//主动工作线程函数
    int service(RtmpMuxerImpl & muxer);
private:
    int sendMetadata(RtmpMuxerImpl &muxer, MStream &body);
private:
    RtmpMuxerImpl *m_rtmp_muxer;

    int m_audio_send_bytes;
    int m_video_send_bytes;
    long long m_data_send_bytes;

    AVPacket *videoSH;
    AVPacket *audioSH;
    int m_fps;
};

#endif // RTMPSENDTASK_H
