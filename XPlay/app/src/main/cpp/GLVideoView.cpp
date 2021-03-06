//
// Created by user on 2021/1/14.
//

#include "GLVideoView.h"
#include "XTexture.h"

void GLVideoView::SetRender(void *win)
{
    view = win;
}

void GLVideoView::Render(XData data)
{
    if(!view) return;

    if(!txt)
    {
        txt = XTexture::Create();
        txt->Init(view);
    }
    txt->Draw(data.datas,data.width,data.height);

}