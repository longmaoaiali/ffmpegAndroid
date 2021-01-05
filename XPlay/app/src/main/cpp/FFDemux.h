//
// Created by user on 2021/1/5.
//

#ifndef XPLAY_FFDEMUX_H
#define XPLAY_FFDEMUX_H


#include "IDemux.h"

struct AVFormatContext;

class FFDemux : public IDemux{
public:
    FFDemux();

    //打开文件 或者流媒体 rmtp http rtsp
    virtual bool Open(const char *url);
    //读取一帧数据 ，数据由调用者清理
    virtual XData Read();

private:
    AVFormatContext *avFormatContext = 0;
};


#endif //XPLAY_FFDEMUX_H
