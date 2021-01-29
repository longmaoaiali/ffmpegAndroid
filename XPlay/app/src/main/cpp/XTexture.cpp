//
// Created by user on 2021/1/14.
//

#include "XTexture.h"
#include "XLog.h"
#include "XShader.h"
#include "XEGL.h"

class CXTexture:public XTexture
{
public:
    XShader shader;
    bool Init(void* win){
        if(!win){
            XLOGE("XTexture Init failed!");
            return false;
        }
        if(!XEGL::Get()->Init(win))return false;
        shader.Init();
        return true;
    }

    void Draw(unsigned char *data[],int width,int height)
    {
        shader.GetTexture(0,width,height,data[0]); //Y
        shader.GetTexture(1,width/2,height/2,data[1]); //Y
        shader.GetTexture(2,width/2,height/2,data[2]); //Y

        shader.Draw();
        XEGL::Get()->Draw();
    }
};

XTexture *XTexture::Create()
{
    return new CXTexture();
}