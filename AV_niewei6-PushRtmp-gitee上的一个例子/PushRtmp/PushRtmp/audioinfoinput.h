#ifndef AUDIOINFOINPUT_H
#define AUDIOINFOINPUT_H

#include <QIODevice>

class Receive_handler;
class AudioInfoInPut : public QIODevice
{
public:
    AudioInfoInPut(Receive_handler*,int nFrameSize);
    ~AudioInfoInPut();
public:
    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);

    qint64 writeData(const char *data, qint64 len);

    std::string  m_InPut_buf;

    Receive_handler* m_handler;

    int  m_nFrameSize;
};

#endif // AUDIOINFOINPUT_H
