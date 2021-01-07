//
// Created by user on 2021/1/7.
//

#ifndef XPLAY_IOBSERVER_H
#define XPLAY_IOBSERVER_H


#include "XData.h"
#include "XThread.h"
#include <vector>
#include <mutex>

class IObserver: public XThread {
public:
    //观察者接受数据函数
    virtual void Update(XData data) {}

    //主体函数 添加观察者
    void addObs(IObserver *obs);

    //通知所有观察者
    void Notify(XData data);

protected:
    std::vector<IObserver *>obss;
    std::mutex mux;

};


#endif //XPLAY_IOBSERVER_H
