#include "aacaudioencoderimpl.h"
#include <qDebug>
AACAudioEncoderImpl::AACAudioEncoderImpl()
{
    nPCMBitSize = 16;      // 单样本位数
    nInputSamples = 0;
    nMaxOutputBytes = 0;
}

AACAudioEncoderImpl::~AACAudioEncoderImpl()
{

}

std::string AACAudioEncoderImpl::getAudioHeader()
{
     return m_header;
}

bool AACAudioEncoderImpl::init(int samplerate, int channel, int bitrate)
{
    m_samplerate = samplerate;
    m_channels = channel;
    m_bitrate = bitrate;

    hEncoder = faacEncOpen(m_samplerate, m_channels, &nInputSamples, &nMaxOutputBytes);

    if(hEncoder == NULL)
    {
        qDebug(QString("[ERROR] Failed to call faacEncOpen()").toLocal8Bit().data());
        return  false;
    }

    nPCMBufferSize = nInputSamples * nPCMBitSize / 8;

    pbPCMBuffer = new unsigned char [nPCMBufferSize]; //输入PCM buf长度

    pbAACBuffer = new unsigned char [nMaxOutputBytes];//输出AAC buf长度

    pConfiguration = faacEncGetCurrentConfiguration(hEncoder);
    if (pConfiguration->version != FAAC_CFG_VERSION)
    {
        faacEncClose(hEncoder);//log_error("faacEncGetCurrentConfiguration failed");
        return false;
    }

    pConfiguration->aacObjectType = LOW; // MAIN
    pConfiguration->mpegVersion = MPEG4;
    pConfiguration->useTns = 1;
    pConfiguration->useLfe = 1;
    pConfiguration->allowMidside = 1;
    pConfiguration->bitRate = m_bitrate / channel;
    pConfiguration->bandWidth = 0;
    pConfiguration->quantqual = 100;
    pConfiguration->outputFormat = 0;
    pConfiguration->inputFormat = FAAC_INPUT_16BIT;
    pConfiguration->shortctl = SHORTCTL_NORMAL;

    int nRet = faacEncSetConfiguration(hEncoder, pConfiguration);
    if( 0 == nRet)
    {
         return false;
    }

    unsigned long extradata_size = 0;
    unsigned char *buffer;
    unsigned long decoder_specific_info_size;

    if (!faacEncGetDecoderSpecificInfo(hEncoder, &buffer, &decoder_specific_info_size)) {
            extradata_size = decoder_specific_info_size;
    }

    m_header.clear();

    unsigned char af[2] = {0xaf, 0x00};

    m_header.append((char *)af, 2);
    m_header.append((char*)buffer, extradata_size);

    free(buffer);

    return true;
}

void AACAudioEncoderImpl::fini()
{
    if (hEncoder)
        faacEncClose(hEncoder);

    delete []pbPCMBuffer;

    delete []pbAACBuffer;
}

bool AACAudioEncoderImpl::encode(const std::string &data, std::string &output)
{
    if (data.size() != getFrameSize()) {
        return false; //log_error("aac data is too not equal to %d", getFrameSize());
    }

    // A negative value to indicate a failure, the number of vaid bytes
    // in the output buffer otherwise. A return value of zero does not
    // indicate failure.
    int encodedBytes = faacEncEncode(hEncoder, (int32_t*)data.data(), nInputSamples, pbAACBuffer, nMaxOutputBytes);
    if (encodedBytes < 0) {
        //log_error("faacEncEncode failed, aac ori data %d , encode data %d", data.size(), encodedBytes);
        return false;
    } else if (encodedBytes == 0) {
        //log_trace("audio frame delayed in encoder.");
        return true;
    }

    static unsigned char af[2] = {0xaf, 0x01};

    output.append((char *)af, 2);
    output.append((char*)pbAACBuffer, encodedBytes);

    return true;
}

int AACAudioEncoderImpl::getFrameSize()
{
    return nInputSamples * nPCMBitSize / 8;
}

float AACAudioEncoderImpl::getFrameDuration()
{
   return  (float)nInputSamples * 1000.00 / (float)m_samplerate / m_channels;
}

