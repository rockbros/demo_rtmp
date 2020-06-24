#ifndef AVPACKET_H
#define AVPACKET_H

#include "mstream.hpp"

#define Packet_Audio 0x08
#define Packet_Video 0x09

#define Audio_AAC 0x01
#define Audio_MP3 0x02

#define Video_H264 0x17

typedef long long qint64;           /* 64 bit signed */

class AVPacket
{
public:
    AVPacket(char type);

    virtual ~AVPacket();
public:
    MStream data;
    char pktType;
    qint64 pts;
    qint64 dts;
    bool has_encoded;
};

class AudioPacket : public AVPacket
{
public:
    AudioPacket(): AVPacket(Packet_Audio)
    {
    }
   virtual ~AudioPacket() {}
};

class VideoPacket : public AVPacket
{
public:
    VideoPacket() : AVPacket(Packet_Video)
    {
    }
   virtual ~VideoPacket() {}
};

#endif // AVPACKET_H
