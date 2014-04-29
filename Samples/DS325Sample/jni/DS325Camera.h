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
#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <OpenNI.h>

#define  LOG_TAG    "DS325Camera"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)


openni::Device device;
openni::VideoStream videoStream;

#ifdef __cplusplus
extern "C" {
#endif

jint Java_org_siprop_openni_ds325_DS325CameraPreview_prepareCamera(JNIEnv* env, jobject thiz, jint camera_type);
void Java_org_siprop_openni_ds325_DS325CameraPreview_readCameraImage(JNIEnv* env, jobject thiz, jobject bitmap);
void Java_org_siprop_openni_ds325_DS325CameraPreview_stopCamera(JNIEnv* env, jobject thiz);

#ifdef __cplusplus
}
#endif

