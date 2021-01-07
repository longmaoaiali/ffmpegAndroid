//
// Created by user on 2021/1/6.
//

#include "XThread.h"
#include "XLog.h"
#include <thread>

using namespace std;

void XSleep(int mis){
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}

void XThread::Start() {
    isExit = false;
    thread th(&XThread::ThreadMain,this);
    //当前线程放弃对创建线程的控制
    th.detach();
}

void XThread::Stop() {
    isExit = true;
    for(int i=0;i<200;i++){
        if(!isRunning){
            XLOGI("Stop 停止线程成功");
            return;
        }
        XSleep(1);
    }
    XLOGI("Stop 停止线程超时");
}


void XThread::ThreadMain() {
    XLOGI("线程函数进入");
    isRunning = true;
    Main();
    XLOGI("线程函数退出");
    isRunning = false;
}