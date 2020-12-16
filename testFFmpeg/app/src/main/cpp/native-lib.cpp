#include <jni.h>
#include <string>
#include <android/log.h>

#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,"testff",__VA_ARGS__)

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavcodec/jni.h>
}

#include <iostream>
static double  r2d(AVRational r)
{
    return r.num==0||r.den==0?0:(double)r.num/(double)r.den;
}

//得到当前的时间戳
long long GetNowMs()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long long sec = tv.tv_sec%360000;
    //得到ms数
    long long t = sec*1000+tv.tv_usec/1000;
    return t;
}

//java加载库时会自动执行这里，使得硬解码时C++能够调用java的接口
extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm,void *res)
{
    av_jni_set_java_vm(vm,0);
    return JNI_VERSION_1_4;

}

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_cvte_testffmpeg_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    hello += avcodec_configuration();
    //初始化解封装，初始化网络
    av_register_all();
    avformat_network_init();
    //注册所有的解码器
    avcodec_register_all();
    //打开文件
    AVFormatContext *ic = NULL;
    char path[] = "/sdcard/1080.mp4";
    int ret = avformat_open_input(&ic,path,0,0);
    if(ret != 0) {
        LOGW("avformat open input %s failed! reason is %s",path,av_err2str(ret));
        return env->NewStringUTF(hello.c_str());
    }
    LOGW("avformat open input %s success!",path);

    //获取时长与 音频视频流的数量
    LOGW("duration = %lld nb_streams = %d",ic->duration,ic->nb_streams);
    //读取流的数据来获取数据流的信息 is useful for file formats with no headers such as MPEG
    //一般open就可以获得数据信息了
    ret = avformat_find_stream_info(ic,0);
    if(ret != 0){
        LOGW("avformat_find_stream_info failed !");
    }

    int fps = 0;
    int width = 0;
    int height = 0;
    int videoStreamId = 0;
    int videoCodecId = 0;
    int audioStreamId = 1;
    int audioCodecId = 0;

    for(int i=0; i< ic->nb_streams;i++){
        AVStream *as = ic->streams[i];
        if(as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            LOGW("视频数据 i=%d",i);
            videoStreamId = i;
            fps = r2d(as->avg_frame_rate);
            width = as->codecpar->width;
            height = as->codecpar->height;
            videoCodecId = as->codecpar->codec_id;
            LOGW("fps = %d,width = %d,height = %d,videoCodecId = %d",fps,width,height,videoCodecId);
            /*fps = 23,width = 1920,height = 1080,videoCodecId = 28 视频编码是AV_CODEC_ID_H264 */
        } else if(as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){
            LOGW("音频数据 i=%d",i);
            audioStreamId = i;
            LOGW("sample_rate=%d,channel=%d,sample_format=%d,audioCodecId=%d",
                as->codecpar->sample_rate,
                as->codecpar->channels,
                as->codecpar->format,
                as->codecpar->codec_id);
            /*sample_rate=44100,channel=2,sample_format=8,audioCodecId=86018 = 0x15002  所以音频编码是AV_CODEC_ID_AAC */

        } else {
            LOGW("codec_type is %d",(int)as->codecpar->codec_type);
        }
    }

    int audioStream = av_find_best_stream(ic,AVMEDIA_TYPE_AUDIO,-1,-1,NULL,0);
    LOGW("av_find_best_stream = %d",audioStream);


    //视频软解码器
    AVCodec *codec = avcodec_find_decoder(ic->streams[videoStreamId]->codecpar->codec_id);
    //视频硬解码器
    codec = avcodec_find_decoder_by_name("h264_mediacodec");
    if(!codec){
        LOGW("video codec find failed !");
        return env->NewStringUTF(hello.c_str());
    }
    //视频解码器初始化
    AVCodecContext *vc = avcodec_alloc_context3(codec);

    //拷贝AVStream参数到视频解码器中
    avcodec_parameters_to_context(vc,ic->streams[videoStreamId]->codecpar);
    vc->thread_count = 1;

    //打开视频解码器上下文
    ret = avcodec_open2(vc,0,0);
    if(ret != 0){
        LOGW("video codec open failed !");
        return env->NewStringUTF(hello.c_str());
    }

    //音频软解码器
    AVCodec *audioCodec = avcodec_find_decoder(ic->streams[audioStreamId]->codecpar->codec_id);

    if(!audioCodec){
        LOGW("audio codec find failed !");
        return env->NewStringUTF(hello.c_str());
    }
    //音频解码器初始化
    AVCodecContext *ac = avcodec_alloc_context3(audioCodec);
    //拷贝AVStream参数到音频解码器中
    avcodec_parameters_to_context(ac,ic->streams[audioStreamId]->codecpar);
    ac->thread_count = 1;

    //打开音频解码器
    ret = avcodec_open2(ac,0,0);
    if(ret != 0){
        LOGW("audio codec open failed !");
        return env->NewStringUTF(hello.c_str());
    }


    //读取帧数据 av_read_frame 注意内存的分配与销毁
    AVPacket *pkt = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    long long start = GetNowMs();
    int frameCount = 0;
    for(;;){
        //LOGW("GetNowMs() = %lld",GetNowMs());
        if(GetNowMs() - start >= 3000)
        {
            LOGW("now decode fps is %d",frameCount/3);
            start = GetNowMs();
            frameCount=0;
        }
        int re = av_read_frame(ic,pkt);
        if(re != 0){
            LOGW("读到结尾处");
            //跳转到20s的位置
            int pos = 20*r2d(ic->streams[videoStreamId]->time_base);
            av_seek_frame(ic,videoStreamId,pos,AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME);
            continue;
        }
        //只解码视频
        AVCodecContext *cc = vc;
        if(pkt->stream_index == audioStreamId)
            cc=ac;

        //将pkt送到线程中解码，并直接丢弃该pkt
        re = avcodec_send_packet(cc,pkt);
        av_packet_unref(pkt);
        if(re != 0){
            //失败就跳过
            LOGW("avcodec_send_packet failed,SKIP the packet");
            continue;
        }

        //读取一个pkt可能对应的多个frame
        for(;;){
            //从线程中接收解码后的frame
            re = avcodec_receive_frame(cc,frame);
            if(re != 0){
                //LOGW("avcodec_receive_frame failed,SKIP the frames");
                break;
            }
            //LOGW("avcodec_receive_frame pts %lld",frame->pts);
            if(cc==vc)
                frameCount++;
        }

//        LOGW("stream=%d size=%d ptd=%lld flag=%d",
//            pkt->stream_index,pkt->size,pkt->pts,pkt->flags);





        //取消pkt对象内部引用的数据buffer unref之后内存136->16
        av_packet_unref(pkt);
    }
    //关闭文件
    avformat_close_input(&ic);

    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_cvte_testffmpeg_MainActivity_open(JNIEnv *env, jobject instance, jstring url_,
                                           jobject handle) {
    const char *url = env->GetStringUTFChars(url_, 0);

    // TODO
    FILE *fp = fopen(url,"rb");
    if(!fp) {
        LOGW("%s open failed !",url);
    } else {
        LOGW("%s open success !",url);
        fclose(fp);
    }

    env->ReleaseStringUTFChars(url_, url);
    return true;
}