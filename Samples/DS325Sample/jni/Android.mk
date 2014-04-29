LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := DS325Camera
LOCAL_C_INCLUDES += ./jni/include/
LOCAL_SRC_FILES  := DS325Camera.cpp
LOCAL_LDLIBS     := -L./jni/libs/ -llog -ljnigraphics -lDS325 -lOpenNI2

include $(BUILD_SHARED_LIBRARY)
