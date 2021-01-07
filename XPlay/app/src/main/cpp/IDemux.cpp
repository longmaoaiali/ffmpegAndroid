//
// Created by user on 2021/1/5.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::Main() {
    while(!isExit)
    {
        XData d = Read();
        if(d.size>0)
            Notify(d);
        //XLOGI("IDemux Read %d",d.size);
    }

}