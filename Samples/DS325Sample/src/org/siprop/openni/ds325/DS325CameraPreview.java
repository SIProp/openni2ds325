/*****************************************************************************
*                                                                            *
*  OpenNI for DS325                                                          *
*  Copyright (C) 2014 SIProp.org http://www.siprop.org/                      *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
package org.siprop.openni.ds325;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;


class DS325CameraPreview extends SurfaceView implements SurfaceHolder.Callback, Runnable {

	private static final String TAG="DS325";
	protected Context context;
	private SurfaceHolder holder;
    Thread mainLoop = null;
	private Bitmap bmp = null;

	private boolean cameraExists = false;
	private boolean shouldStop = false;
	
	static final int DEPTH_IMAGE_WIDTH = 320;
	static final int DEPTH_IMAGE_HEIGHT = 240;
	static final int COLOR_IMAGE_WIDTH = 640;
	static final int COLOR_IMAGE_HEIGHT = 480;

    private Rect rect;
  
    // JNI functions
    public native int prepareCamera(int cameraType);
    public native void readCameraImage(Bitmap bitmap);
    public native void stopCamera();
    static {
        System.loadLibrary("DS325");
        System.loadLibrary("OpenNI2");
        System.loadLibrary("DS325Camera");
    }
    
	public DS325CameraPreview(Context context, CameraType cameraType) {
		super(context);
		this.context = context;
		setFocusable(true);
		
		holder = getHolder();
		holder.addCallback(this);
		holder.setType(SurfaceHolder.SURFACE_TYPE_NORMAL);	
		
		int ret = -1;
		if(bmp == null){
			if(cameraType == CameraType.COLOR) {
				bmp = Bitmap.createBitmap(COLOR_IMAGE_WIDTH, COLOR_IMAGE_HEIGHT, Bitmap.Config.ARGB_8888);
				ret = prepareCamera(0);
			} else if(cameraType == CameraType.DEPTH) {
				bmp = Bitmap.createBitmap(DEPTH_IMAGE_WIDTH, DEPTH_IMAGE_HEIGHT, Bitmap.Config.ARGB_8888);
				ret = prepareCamera(1);
			} else if(cameraType == CameraType.IR) {
				bmp = Bitmap.createBitmap(DEPTH_IMAGE_WIDTH, DEPTH_IMAGE_HEIGHT, Bitmap.Config.ARGB_8888);
				ret = prepareCamera(2);
			}
		}
		if(ret!=-1) cameraExists = true;
		
        mainLoop = new Thread(this);
        mainLoop.start();		
	}
	
    @Override
    public void run() {
        while (true && cameraExists) {
			rect = new Rect(0, 0, 640, 480);
        	
			// Read DS325's Image as Bitmap
        	readCameraImage(bmp);
        	
            Canvas canvas = getHolder().lockCanvas();
            if(canvas != null) {
            	// draw camera bmp on canvas
            	canvas.drawBitmap(bmp,null,rect,null);
            	getHolder().unlockCanvasAndPost(canvas);
            }

            if(shouldStop) {
            	shouldStop = false;  
            	break;
            }	        
        }
    }

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		if(cameraExists) {
			shouldStop = true;
			while(shouldStop){
				try{ 
					Thread.sleep(100); // wait for thread stopping
				}catch(Exception e){}
			}
		}
		stopCamera();
	}   
}
