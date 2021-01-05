//
// Created by user on 2021/1/5.
//

#ifndef XPLAY_IDEMUX_H
#define XPLAY_IDEMUX_H

#include "XData.h"

//定义解封装接口
class IDemux {
public:
    //读取一帧数据 ，数据由调用者清理
    virtual XData Read() = 0;
    //打开文件 或者流媒体 rmtp http rtsp
    virtual bool Open(const char *url) = 0;

    //总时长 ms
    int totalMs = 0;
};


#endif //XPLAY_IDEMUX_H
