/*****************************************************************************
*                                                                            *
*  OpenNI 2.x                                                                *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*  Copyright (C) 2014 SIProp.org http://www.siprop.org/                      *
*                                                                            *
*  This file is part of OpenNI.                                              *
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
#ifndef __DS325_STREAM_H__
#define __DS325_STREAM_H__

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */

#include <linux/videodev2.h>
#include "uvcvideo.h"
#include "video.h"

#include "Driver/OniDriverAPI.h"
#include "XnLib.h"
#include "XnHash.h"
#include "XnEvent.h"
#include "XnPlatform.h"


#define DEPTHSENSE_COLOR_RESOLUTION_X 640
#define DEPTHSENSE_COLOR_RESOLUTION_Y 480
#define DEPTHSENSE_DEPTH_RESOLUTION_X 320
#define DEPTHSENSE_DEPTH_RESOLUTION_Y 240

#define ERROR_LOCAL -1
#define SUCCESS_LOCAL 0

#define CLEAR(x) memset (&(x), 0, sizeof (x))


class DS325Stream : public oni::driver::StreamBase {
public:
	virtual OniStatus SetVideoMode(OniVideoMode*) = 0;
	virtual OniStatus GetVideoMode(OniVideoMode* pVideoMode) = 0;

	OniStatus getProperty(int propertyId, void* data, int* pDataSize);

	OniStatus setProperty(int propertyId, const void* data, int dataSize);
	virtual void Mainloop() = 0;
	virtual OniStatus start() = 0;
	virtual void stop() = 0;

protected:
	bool m_running;
	XN_THREAD_HANDLE m_threadHandle;

	virtual void set_image(unsigned char *src, int size) = 0;

	// Thread
	static XN_THREAD_PROC threadFunc(XN_THREAD_PARAM pThreadParam);

	/**
	 * For V4L
	 */
	struct buffer {
	        void *                  start;
	        size_t                  length;
	};
	char             dev_name[16];
	int              fd;
	struct buffer   *buffers;
	unsigned int     n_buffers;

	int errno_exit(const char *s);
	int xioctl(int fd, int request, void *arg);
	int send_xu(int query, unsigned char* data);
	OniStatus start_stream(int device_id, int width, int height);
	void stop_stream(void);
	int open_device(int device_id);
	int init_device(int width, int height);
	int init_mmap(void);
	int start_capturing(void);
	int read_frame_once(void);
	int read_frame(void);
	int stop_capturing(void);
	int uninit_device(void);
	int close_device(void);

};

#endif // __DS325_STREAM_H__
