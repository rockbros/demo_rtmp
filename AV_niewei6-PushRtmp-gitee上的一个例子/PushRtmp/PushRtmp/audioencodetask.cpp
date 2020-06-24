#include "audioencodetask.h"
#include "aacaudioencoderimpl.h"
#include "TimestampBulider.h"
#include <qDebug>
AudioEncodeTask::AudioEncodeTask(Receive_handler *pHandler):m_pHandler(pHandler)
{
    outfile = NULL;
    m_audioEncoder = NULL;
    m_pack_num1 = 0;
    m_pack_num2 = 0;
}

AudioEncodeTask::~AudioEncodeTask()
{
    close();

    if(m_audioEncoder)
        delete m_audioEncoder;

    m_audioEncoder = NULL;
//    if(outfile)
    //        fclose(outfile);
}

string AudioEncodeTask::getAudioHeader()
{
    if(m_audioEncoder)
        return m_audioEncoder->getAudioHeader();

    return string("");
}

float AudioEncodeTask::getFrameDuration()
{

    if(m_audioEncoder)
        return m_audioEncoder->getFrameDuration();
    return -1;
}

int AudioEncodeTask::getFrameSize()
{
    if(m_audioEncoder)
        return m_audioEncoder->getFrameSize();
    return 0;
}

bool AudioEncodeTask::init(int samplerate, int channel, int bitrate)
{
    if(!m_audioEncoder)
    {
        m_audioEncoder = new AACAudioEncoderImpl;       //aac 编码实现
        if(m_audioEncoder)
        {
            return m_audioEncoder->init(samplerate,channel,bitrate);
        }

    }
    return false;
}

int AudioEncodeTask::close()
{
    std::string str("");

    int nthread  =thr_count();

    for (int i =0 ;i<nthread ;i++)
        putq(str);

    wait();

    if(m_audioEncoder)
        m_audioEncoder->fini();
    m_audioEncoder = NULL;
    return 0;
}

bool AudioEncodeTask::receive_handler(const char *data, int size)
{
    m_pack_num1 ++ ;
    putq(std::string(data,size));
    return true;
}

int AudioEncodeTask::svc()
{
    std::string str ="";

    //BleTimestampBulider *pTimestampBulider = new BleTimestampBulider;
     while(getq(str))
     {
         if (str=="") break;

         m_pack_num2 ++;

         if(m_audioEncoder)
         {
             std::string outbuf ="";
             if(m_audioEncoder->encode(str,outbuf))
             {
                 if(m_pHandler && (!outbuf.empty()))
                     m_pHandler->receive_handler(outbuf.data(),outbuf.size());//发送到接口
             }else
             {
                 return false;
             }
         }
     }
     return true;
}

