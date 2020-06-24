#include "videocapturetask.h"
#include <QPixmap>
#include<QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include "capturetaskpack.h"
#include "rgbimage.h"

VideoCaptureTask::VideoCaptureTask(Receive_handler *pHandler):m_pHandler(pHandler)
{

}

VideoCaptureTask::~VideoCaptureTask()
{
    close();
}

int VideoCaptureTask::close()
{
    std::string str("");

    int nthread  =thr_count();

    for (int i =0 ;i<nthread ;i++)
        putq(str);

    wait();
    return true;
}

bool VideoCaptureTask::receive_handler(const char *data, int size)
{
    putq(std::string(data,size));
    return true;
}

int VideoCaptureTask::svc()
{
    std::string str ="";
    QRect rcBuf =QRect(0,0,640,480);
    QImage *m_image = new QImage(rcBuf.size(), QImage::Format_RGB888);
    QPainter p(m_image);

    QPixmap pixmap;

    WId A = QApplication::desktop()->winId();

     while(getq(str))
     {
         if (str=="") break;


         pixmap=QPixmap::grabWindow(A);
         QPixmap pix = pixmap.scaled(rcBuf.size(),Qt::KeepAspectRatioByExpanding);
         p.drawPixmap(rcBuf, pix);

         CAPTURE_TASK_PACK task_pack;
         memset(&task_pack,0,sizeof(task_pack));
         task_pack.image = new RGBImage((char *)m_image->bits(),m_image->width(),m_image->height());
         task_pack.packet = *((AVPacket **)str.data());

         if(m_pHandler)
             m_pHandler->receive_handler((const char *)&task_pack,sizeof(task_pack));
     }
     return 0;
}

