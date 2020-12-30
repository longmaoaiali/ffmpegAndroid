package com.cvte.androidopengl;


import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;

import com.cvte.androidopengl.Render.GLRender;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";
    private GLSurfaceView mGlSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //实例化GLSurfaceView 并设置渲染器
        mGlSurfaceView = new GLSurfaceView(this);
        mGlSurfaceView.setRenderer(new GLRender());

        setContentView(mGlSurfaceView);
        Log.d(TAG,"onCreate");
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGlSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGlSurfaceView.onPause();
    }
}
