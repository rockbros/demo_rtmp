#ifndef VIDEOENCODETASK_H
#define VIDEOENCODETASK_H
#include <Receive_handler.h>
#include <Active_Object/Task_Base.h>
class X264EncoderImpl;
class VideoEncodeTask : public Task_Base ,public Receive_handler
{
public:
    VideoEncodeTask();
    ~VideoEncodeTask();
public:
    bool init();

    std::string     getX264Header();
    std::string     getX264SeiInfo();
public:
    float getFrameDuration();
    virtual int close();
    virtual bool receive_handler(const char *data,int size);
protected:
    virtual int svc();//主动工作线程函数
private:
    X264EncoderImpl   *m_videoEncoder;
};

#endif // VIDEOENCODETASK_H
