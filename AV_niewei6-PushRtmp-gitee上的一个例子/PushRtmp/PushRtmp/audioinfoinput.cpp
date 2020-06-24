#include "audioinfoinput.h"
#include "Receive_handler.h"
AudioInfoInPut::AudioInfoInPut(Receive_handler* phandler,int nFrameSize)
    :m_handler(phandler)
    ,m_nFrameSize(nFrameSize)
{

}

AudioInfoInPut::~AudioInfoInPut()
{

}

void AudioInfoInPut::start()
{
    open(QIODevice::WriteOnly);
}

void AudioInfoInPut::stop()
{
    close();
}

qint64 AudioInfoInPut::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
    Q_UNUSED(maxlen)

    return 0;
}

qint64 AudioInfoInPut::writeData(const char *data, qint64 len)
{
    m_InPut_buf.append(std::string(data,len));//添加到后面 PCM 数据

    while(m_InPut_buf.size()>=m_nFrameSize) // 16位的PCM  就是2K 大小
    {
        if(m_handler)
        {
            m_handler->receive_handler(const_cast <char*>(m_InPut_buf.data()),m_nFrameSize);

            m_InPut_buf = std::string(m_InPut_buf.begin()+ m_nFrameSize,m_InPut_buf.end());
        }
    }

    return len;
}

