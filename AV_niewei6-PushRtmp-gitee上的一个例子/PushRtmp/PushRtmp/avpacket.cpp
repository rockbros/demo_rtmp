#include "avpacket.h"

AVPacket::AVPacket(char type):pktType(type)
  , pts(0)
  , dts(0)
  , has_encoded(false)
{

}

AVPacket::~AVPacket()
{

}

