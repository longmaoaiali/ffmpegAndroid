//
// Created by user on 2021/1/6.
//

#ifndef XPLAY_XTHREAD_H
#define XPLAY_XTHREAD_H

void XSleep(int mis);
//C++ std11
class XThread {
public:
    //开启 退出线程
    virtual void Start();
    virtual void Stop();

    //入口主函数
    virtual void Main(){}

protected:
    bool isExit = false;
    bool isRunning = false;
private:
    void ThreadMain();
};


#endif //XPLAY_XTHREAD_H
