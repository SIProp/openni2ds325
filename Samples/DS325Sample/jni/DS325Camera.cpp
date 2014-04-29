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
#include "DS325Camera.h"

jint
Java_org_siprop_openni_ds325_DS325CameraPreview_prepareCamera(JNIEnv* env, jobject thiz, jint camera_type){

	openni::Status ret;

	openni::OpenNI::initialize();

	ret = device.open("DS325");
	if(ret != openni::STATUS_OK) {
		LOGE("openni::Device::open() failed.");
		return -1;
	}
	if(camera_type == 0) {
		// Color Camera
		videoStream.create(device, openni::SENSOR_COLOR);
	} else if(camera_type == 1) {
		// Depth Camera
		videoStream.create(device, openni::SENSOR_DEPTH);
	} else if(camera_type == 2) {
		// IR Camera
		videoStream.create(device, openni::SENSOR_IR);
	}

	videoStream.start();

	return 0;
}

void
Java_org_siprop_openni_ds325_DS325CameraPreview_readCameraImage(JNIEnv* env, jobject thiz, jobject bitmap) {

	openni::VideoFrameRef videoFrame;
	videoStream.readFrame(&videoFrame);

	AndroidBitmapInfo info;
	void* pixels = NULL;
	int ret, i;
	int *colors = NULL;

	int width=0;
	int height=0;

	if((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
		LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}

	width = info.width;
	height = info.height;

	if(info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
		LOGE("Bitmap format is not RGBA_8888 !");
		return;
	}

	if((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
		LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
		return;
	}

	colors = (int *)pixels;
	if(videoStream.getSensorInfo().getSensorType() == openni::SENSOR_DEPTH) {
		//DepthPixel* lcamera_image = (DepthPixel *)videoFrame.getData();
		unsigned short* lcamera_image = (unsigned short *)videoFrame.getData();
		unsigned short depth = 0;
		unsigned char gray = 0;
		float ratio = 0.0f;
		for(i=0 ; i<width*height ; i++) {
			depth = *lcamera_image++;
			ratio = (float)depth/2048.0f;
			gray = ratio * 0xff;
			*colors++ = 0xff000000 | gray<<16 | gray<<8 | gray;
		}
	} else {
		//RGB888Pixel* lcamera_image = (RGB888Pixel *)videoFrame.getData();
		unsigned char* lcamera_image = (unsigned char *)videoFrame.getData();
		unsigned char r, g, b;
		for(i=0 ; i<width*height ; i++) {
			//OpenNI=1:R, 2:G, 3:B
			r = *lcamera_image++;
			g = *lcamera_image++;
			b = *lcamera_image++;
			//Android=1:B, 2:G, 3:R
			*colors++ = 0xff000000 | b<<16 | g<<8 | r;
		}
	}

	AndroidBitmap_unlockPixels(env, bitmap);

}

void
Java_org_siprop_openni_ds325_DS325CameraPreview_stopCamera(JNIEnv* env, jobject thiz){
	videoStream.stop();
	device.close();
}

