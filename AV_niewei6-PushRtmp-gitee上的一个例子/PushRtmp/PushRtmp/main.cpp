#include "mainwindow.h"
#include <QAudioInput>

#include <QApplication>
#include <QPixmap>
#include<QDesktopWidget>
#include <QPainter>

#include <stdint.h>
#include "audioinfoinput.h"
#include "Convert.h"
#include "audioencodetask.h"
#include "TimestampBulider.h"
#include "videocapturetask.h"
#include "videoencodetask.h"
#include "rtmpsendtask.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     WId A = QApplication::desktop()->winId();
    //ColorSpaceConversions	convert;
    QAudioFormat    m_format;
    m_format.setSampleRate(44100);//
    m_format.setChannelCount(2);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::UnSignedInt);
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setCodec("audio/pcm");

    RtmpSendTask    oRtmpSendTask;

    VideoEncodeTask  oVideoEncodeTask;//视频编码任务
    oVideoEncodeTask.init();    //视频编码器初始化
    float vdeioFrameDuration =  oVideoEncodeTask.getFrameDuration();//编码器初始化后获取间隔时间
    std::string   strX264Header =  oVideoEncodeTask.getX264Header();
    std::string   strSeiInfo  = oVideoEncodeTask.getX264SeiInfo();


    VideoCaptureTask oVideoCaptureTask(&oVideoEncodeTask);//视频捕获任务


    BleTimestampBulider TimestampBulider(&oVideoCaptureTask,&oRtmpSendTask);//音视频时间戳同步


    AudioEncodeTask oAudioEncodeTask(&TimestampBulider);//音频编码任务
    oAudioEncodeTask.init(44100,2,96000);   //96000音频编码器初始化
    float audioFrameDuration = oAudioEncodeTask.getFrameDuration();//编码器初始化后获取间隔时间
    int audioFrameSize = oAudioEncodeTask.getFrameSize(); //编码器初始化后 音频帧大小
    std::string   strAudioHeader = oAudioEncodeTask.getAudioHeader();//

    oRtmpSendTask.setAudioSh(strAudioHeader);
    oRtmpSendTask.setVideoSh(strX264Header);
    oRtmpSendTask.setVideoSeiInfo(strSeiInfo);
    oRtmpSendTask.activate();//启动rtmp发送任务

    oVideoCaptureTask.activate();//启动图像采集任务

    oVideoEncodeTask.activate();//启动x264编码任务

    oAudioEncodeTask.activate();//启动音频编码任务

    TimestampBulider.setAudioCaptureInternal(audioFrameDuration);
    TimestampBulider.setVideoCaptureInternal(vdeioFrameDuration);


    AudioInfoInPut  m_audioInInfo(&oAudioEncodeTask,audioFrameSize);//音频采集   pAdioEncodeTask
    m_audioInInfo.start();

    QAudioInput   m_audioInput(m_format);
    m_audioInput.start(&m_audioInInfo);

    MainWindow w;

    w.show();
    int ret = a.exec();



    return ret;
}
