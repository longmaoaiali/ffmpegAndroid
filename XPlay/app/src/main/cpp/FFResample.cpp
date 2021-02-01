//
// Created by user on 2021/2/1.
//

extern "C"
{
#include <libswresample/swresample.h>
}

#include "XLog.h"
#include <libavcodec/avcodec.h>
#include "FFResample.h"

bool FFResample::Open(XParameter in,XParameter out)
{

    //音频重采样上下文初始化
    actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(2),
                              AV_SAMPLE_FMT_S16,in.para->sample_rate,
                              av_get_default_channel_layout(in.para->channels),
                              (AVSampleFormat)in.para->format,in.para->sample_rate,
                              0,0 );

    int re = swr_init(actx);
    if(re != 0)
    {
        XLOGE("swr_init failed!");
        return false;
    }
    else
    {
        XLOGI("swr_init success!");
    }
    return true;

}