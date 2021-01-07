//
// Created by user on 2021/1/5.
//

#include "XData.h"

extern "C"{
#include <libavformat/avformat.h>
}

void XData::Drop() {
    if(!data) return;
    av_packet_free((AVPacket**)&data);
    data = 0;
    size = 0;
}