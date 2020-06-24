#ifndef AUDIOENCODETASK_H
#define AUDIOENCODETASK_H

#include <Receive_handler.h>
#include <Active_Object//Task_Base.h>
class AudioEncoderAbstract;
class AudioEncodeTask: public Task_Base ,public Receive_handler
{
public:
    AudioEncodeTask(Receive_handler*);
    ~AudioEncodeTask();
public:
    std::string  getAudioHeader();
    float   getFrameDuration();     //获取音频编码 间隔时间
    int     getFrameSize();
    virtual int  close();
    virtual bool init(int samplerate, int channel, int bitrate);
    virtual bool receive_handler(const char *data,int size);
protected:
    virtual int svc();//主动工作线程函数
private:
    Receive_handler        *m_pHandler;
    FILE* outfile;

    int m_pack_num1;
    int m_pack_num2;
    AudioEncoderAbstract   *m_audioEncoder;
};

#endif // AUDIOENCODETASK_H
