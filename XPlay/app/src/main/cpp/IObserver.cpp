//
// Created by user on 2021/1/7.
//

#include "IObserver.h"

//主体函数 添加观察者
void IObserver::addObs(IObserver *obs){
    if(!obs) return;

    mux.lock();
    obss.push_back(obs);
    mux.unlock();

}

//通知所有观察者
void IObserver::Notify(XData data)
{
    mux.lock();
    for(int i=0;i<obss.size();i++)
    {
        obss[i]->Update(data);
    }
    mux.unlock();


}