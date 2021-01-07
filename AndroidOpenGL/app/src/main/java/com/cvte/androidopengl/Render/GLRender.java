package com.cvte.androidopengl.Render;

import android.opengl.GLSurfaceView;
import android.opengl.GLU;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by user on 2020/12/30.
 */

public class GLRender implements GLSurfaceView.Renderer{
    private static final String TAG = "GLRender";
    int one = 0x10000;

    //设置用于控制三角形和正方形旋转的角度
    float rotateTri, rotateQuad;

    //三角形的三个顶点
    private int[] mTriggerBuffer = new int[]{
            0,one,0,//上顶点
            -one,-one,0,//左下点
            one,-one,0,//右下点
    };

    //正方形的四个顶点
    private int[] mQuaterBuffer = new int[]{
            one,one,0,
            -one,one,0,
            one,-one,0,
            -one,-one,0
    };

    //三角形的顶点颜色值(r,g,b,a)
    private int[] mColorBuffer = new int[]{
            one,0,0,one,
            0,one,0,one,
            0,0,one,one,
    };

    IntBuffer colorBuffer;
    IntBuffer quaterBuffer;
    IntBuffer triggerBuffer;

    public GLRender(){
        colorBuffer = bufferUtil(mColorBuffer);
        quaterBuffer = bufferUtil(mQuaterBuffer);
        triggerBuffer = bufferUtil(mTriggerBuffer);

    }

    public IntBuffer bufferUtil(int []arr) {
        IntBuffer buffer;

        ByteBuffer qbb = ByteBuffer.allocateDirect(arr.length * 4);
        qbb.order(ByteOrder.nativeOrder());

        buffer = qbb.asIntBuffer();
        buffer.put(arr);
        buffer.position(0);

        return buffer;
    }

    public FloatBuffer bufferUtil(float []arr){
        FloatBuffer buffer;

        ByteBuffer qbb = ByteBuffer.allocateDirect(arr.length * 4);
        qbb.order(ByteOrder.nativeOrder());

        buffer = qbb.asFloatBuffer();
        buffer.put(arr);
        buffer.position(0);

        return buffer;
    }


    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.d(TAG,"onSurfaceCreated");
        //告诉系统需要对透视进行修正
        gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT,GL10.GL_NICEST);
        //设置清理屏幕的颜色
        gl.glClearColor(0,0,0,1);
        //启用深度缓存
        gl.glEnable(GL10.GL_DEPTH_TEST);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.d(TAG,"onSurfaceChanged");
        float ratio = (float)width/height;
        //设置openGL视口的大小
        gl.glViewport(0,0,width,height);
        //重置投影矩阵 (置为单位)
        gl.glLoadIdentity();
        //创建一个透视投影矩阵(设置视口大小)
        gl.glFrustumf(-ratio,ratio,-1,1,1,10);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        Log.d(TAG,"onDrawFrame");

        //首先清理屏幕
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
        //设置模型视图矩阵
        gl.glMatrixMode(GL10.GL_MODELVIEW);
        //重置矩阵
        gl.glLoadIdentity();
        //视点变换
        GLU.gluLookAt(gl,0,0,3,0,0,0,0,1,0);
        //设置模型位置
        gl.glTranslatef(-3.0f,0.0f,-4.0f);
        //设置y轴旋转
        gl.glRotatef(rotateTri,0.0f,1.0f,0.0f);
        //允许设置顶点
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
        //允许设置颜色数组
        gl.glEnableClientState(GL10.GL_COLOR_ARRAY);
        //设置颜色数组与三角形的顶点数组
        gl.glColorPointer(4,GL10.GL_FIXED,0,colorBuffer);
        gl.glVertexPointer(3, GL10.GL_FIXED,0,triggerBuffer);
        //放大三角形
        gl.glScalef(2.0f,2.0f,2.0f);
        //绘制三角形
        gl.glDrawArrays(GL10.GL_TRIANGLES,0,3);
        //关闭颜色数组的设置
        gl.glDisableClientState(GL10.GL_COLOR_ARRAY);

        /*正方形*/
        //设置正方形的颜色RGBA
        gl.glColor4f(0.5f,0.5f,1.0f,1.0f);
        gl.glLoadIdentity();
        gl.glTranslatef(1.0f,0.0f,-4.0f);
        gl.glRotatef(rotateQuad,1.0f,0.0f,0.0f);
        gl.glVertexPointer(3,GL10.GL_FIXED,0,quaterBuffer);
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP,0,4);
        //gl.glDrawArrays(GL10.GL_LINES,0,4);绘制的线框
        //取消顶点设置
        gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);

        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        rotateTri += 0.5f;
        rotateQuad += 0.5f;

    }
}
