#ifndef AACAUDIOENCODERIMPL_H
#define AACAUDIOENCODERIMPL_H

#include "audioencoderabstract.h"
#include <faac.h>
class AACAudioEncoderImpl : public AudioEncoderAbstract
{
public:
    AACAudioEncoderImpl();
    ~AACAudioEncoderImpl();

    virtual std::string  getAudioHeader();

    virtual bool    init(int samplerate, int channel, int bitrate);
    virtual void    fini();
    virtual bool    encode(const std::string &data, std::string &output);
    virtual int     getFrameSize();
    virtual float   getFrameDuration();
private:
    faacEncHandle hEncoder;

    faacEncConfigurationPtr pConfiguration;

    int nPCMBufferSize;

    unsigned int nPCMBitSize;

    unsigned long nInputSamples;

    unsigned long nMaxOutputBytes;

    unsigned char * pbPCMBuffer;

    unsigned char * pbAACBuffer;
    std::string     m_header;
};

#endif // AACAUDIOENCODERIMPL_H
