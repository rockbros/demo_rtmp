#ifndef VIDEOCAPTURETASK_H
#define VIDEOCAPTURETASK_H
#include <Receive_handler.h>
#include <Active_Object/Task_Base.h>
class VideoCaptureTask: public Task_Base,public Receive_handler
{
public:
    VideoCaptureTask(Receive_handler*);
    ~VideoCaptureTask();
public:
    virtual int close();
    virtual bool receive_handler(const char *data,int size);
protected:
    virtual int svc();//主动工作线程函数
private:
     Receive_handler        *m_pHandler;
};

#endif // VIDEOCAPTURETASK_H
