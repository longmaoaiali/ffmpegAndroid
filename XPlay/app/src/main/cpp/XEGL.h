//
// Created by user on 2021/1/14.
//

#ifndef XPLAY_XEGL_H
#define XPLAY_XEGL_H


class XEGL {
public:
    virtual bool Init(void *win)=0;
    virtual void Draw() = 0;
    static XEGL *Get();
protected:
    XEGL(){}
};


#endif //XPLAY_XEGL_H
