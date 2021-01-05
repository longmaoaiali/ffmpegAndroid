//
// Created by user on 2021/1/5.
//

#include "FFDemux.h"
#include "XLog.h"
extern "C"{
#include <libavformat/avformat.h>
}

bool FFDemux::Open(const char *url) {
    XLOGI("open file %s begin",url);
    int ret = avformat_open_input(&avFormatContext,url,0,0);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf, sizeof(buf));
        XLOGE("open failed : %s",url);
        return false;
    }

    //读取文件信息
    ret = avformat_find_stream_info(avFormatContext,0);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf, sizeof(buf));
        XLOGE("avformat find stream info failed!");
        return false;
    }

    this->totalMs = avFormatContext->duration/(AV_TIME_BASE/1000);
    XLOGI("total ms = %d!",totalMs);

    return true;
}


XData FFDemux::Read() {
    XData d;

    return d;
}

FFDemux::FFDemux(){
    static bool isFirst = true;
    if(isFirst){
        isFirst = false;
        //注册所有封装器 解码器
        av_register_all();
        avcodec_register_all();
        //初始化网络
        avformat_network_init();
        XLOGI("register ffmpeg just once");
    }
}