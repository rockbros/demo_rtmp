#ifndef AUDIOENCODERABSTRACT_H
#define AUDIOENCODERABSTRACT_H
#include<string>

class AudioEncoderAbstract
{
public:
    AudioEncoderAbstract();
    virtual~AudioEncoderAbstract();
public:
    enum AudioEncoderType
    {
        AAC,
        MP3
    };

public:
    virtual std::string  getAudioHeader()= 0;
    virtual bool    init(int samplerate, int channel, int bitrate) = 0;
    virtual void    fini()  = 0;
    virtual bool    encode(const std::string &data, std::string &output) = 0;
    virtual int     getFrameSize() = 0;
    virtual float   getFrameDuration() = 0;
    inline AudioEncoderType encoderType() { return m_encoderType; }

protected:
    int m_samplerate;
    int m_channels;
    int m_bitrate;
    AudioEncoderType m_encoderType;
};

#endif // AUDIOENCODERABSTRACT_H
