//
// Created by user on 2021/2/1.
//

#include "IResample.h"

void IResample::Update(XData data)
{
    XData d = this->Resample(data);
    if(d.size > 0)
    {
        this->Notify(d);
    }
}