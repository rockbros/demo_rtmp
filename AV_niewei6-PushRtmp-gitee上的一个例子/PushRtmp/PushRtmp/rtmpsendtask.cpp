#include "RtmpMuxerImpl.h"
#include "avpacket.h"
#include "rtmpsendtask.h"
#include <QDateTime>
#include <windows.h>
#include "TimestampBulider.h"
#include "avpacket.h"
RtmpSendTask::RtmpSendTask()
    : m_audio_send_bytes(0)
    , m_video_send_bytes(0)
    , m_data_send_bytes(0)
    , videoSH(0)
    , audioSH(0)
    , m_fps(0)
{

}

RtmpSendTask::~RtmpSendTask()
{
    close();
}

int RtmpSendTask::close()
{
    std::string str("");

    int nthread  =thr_count();

    for (int i =0 ;i<nthread ;i++)
        putq(str);

    wait();
    return true;
}

bool RtmpSendTask::receive_handler(const char *data, int size)
{
    putq(std::string(data,size));
    return true;
}

int RtmpSendTask::svc()
{
    WORD versionRequested;
    WSADATA wsaData;
    versionRequested = MAKEWORD(1, 1);

    if(WSAStartup(versionRequested, &wsaData))
        return false;

    m_rtmp_muxer =  new RtmpMuxerImpl;
    m_rtmp_muxer->setRtmpUrl("rtmp://182.92.177.100/live/livestream");
    int ret = m_rtmp_muxer->start();
    if (ret != TRUE)
    {

        m_rtmp_muxer->stop();
        delete m_rtmp_muxer;
        return false;
    }

    ret = service(*m_rtmp_muxer);

    m_rtmp_muxer->stop();
    delete m_rtmp_muxer;
    WSACleanup();
    return true;
}

int RtmpSendTask::service(RtmpMuxerImpl &rtmp_muxer)
{
    int ret = 0;
    MStream metadata_body;
    if ((ret = sendMetadata(rtmp_muxer, metadata_body)) != 0) {
        return ret;
    }

    if (videoSH)
    {
        MStream &data = videoSH->data;
        if (rtmp_muxer.addH264(data, videoSH->dts) != TRUE )
            return 100;
    }else
        return 100;


    if (audioSH)
    {
        MStream &data = audioSH->data;
        if (rtmp_muxer.addAAC(data, audioSH->dts) != TRUE )
            return 100;
    }else
        return 100;


    std::string str ="";
    while(getq(str))
    {
        if (str=="") break;

        AVPacket  **task_pack = (AVPacket **)str.data();
        AVPacket  * avPacket = (*task_pack);
        while (!avPacket->has_encoded)
        {
            Sleep(20);
        }

        MStream &data = avPacket->data;

        if ((avPacket->pktType == Packet_Video)&&(!data.empty()))
        {
            if (rtmp_muxer.addH264(data, avPacket->dts) != TRUE )
            {
                 ret = 100;
                 break;
            }

             m_video_send_bytes += data.size();
             m_fps += 1;
        } else if ((avPacket->pktType == Packet_Audio)&&(!data.empty()))
        {
            if (rtmp_muxer.addAAC(data, avPacket->dts) != TRUE )
            {
                ret = 100;
                break;
            }

            m_audio_send_bytes += data.size();
        }

        m_data_send_bytes += data.size();

        delete avPacket;//释放 包内存
    }
    return ret;
}

void RtmpSendTask::setVideoSh(std::string &strBuf)
{
    if(videoSH)
        delete videoSH;

    videoSH = new VideoPacket;
    videoSH->dts = 0;
    videoSH->has_encoded = true;
    videoSH->data.writeString(strBuf.data(),strBuf.size() );
}

void RtmpSendTask::setVideoSeiInfo(std::string &strBuf)
{
    VideoPacket *videoSei = new VideoPacket;
    videoSei->dts = 0;
    videoSei->has_encoded = true;
    videoSei->data.writeString(strBuf.data(),strBuf.size() );
    int A = sizeof(videoSei);
    putq(string((char*)&videoSei,sizeof(videoSei)));
}

void RtmpSendTask::setAudioSh(std::string &strBuf)
{
    if(audioSH)
        delete audioSH;

    audioSH = new AudioPacket;
    audioSH->dts = 0;
    audioSH->has_encoded = true;
    audioSH->data.writeString(strBuf.data(),strBuf.size() );
}

int RtmpSendTask::sendMetadata(RtmpMuxerImpl &muxer, MStream &body)
{
    FlvMetaData *meta = new FlvMetaData;
    meta->audiocodecid = CODECID_AAC;
    meta->audiodatarate = 22050;
    meta->audiodelay = 0;
    meta->audiosamplerate = 1000;
    meta->audiosamplesize = 1024;
    meta->canSeekToEnd = false;
    meta->duration = 0;
    meta->filesize = 0;
    meta->framerate = 10;
    meta->height = 480;
    meta->stereo = false;
    meta->videocodecid = CODECID_H264;
    meta->videodatarate = 800;
    meta->width = 640;
    meta->creationdate = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();

    if ((muxer.setMetaData(*meta, body)) != TRUE)
    {
        delete meta;
        return 100;
    }
    delete meta;
    return 0;
}

