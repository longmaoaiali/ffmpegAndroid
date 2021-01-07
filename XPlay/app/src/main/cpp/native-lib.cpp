#include <jni.h>
#include <string>
#include "FFDemux.h"
#include "XLog.h"


class TestObs:public IObserver{
public:
    void Update(XData d){
        XLOGI("TestObs Update data size is %d",d.size);
    }
};

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_cvte_xplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    TestObs *testObs = new TestObs();
    IDemux *de = new FFDemux();
    //添加观察者
    de->addObs(testObs);

    de->Open("/sdcard/1080.mp4");

    de->Start();
    XSleep(3000);
    de->Stop();
    //测试 FFDemux::Read函数
    /*for(;;)
    {
        XData d = de->Read();
        XLOGI("Read data size is %d",d.size);
    }*/

    return env->NewStringUTF(hello.c_str());
}
