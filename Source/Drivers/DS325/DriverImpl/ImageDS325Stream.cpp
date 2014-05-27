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
#include "ImageDS325Stream.h"
#include "DS325Config.h"

ImageDS325Stream::ImageDS325Stream() {
	resID = DS325Config::GetImageResolutionID();
	resX = DS325Config::GetImageResolutionX(resID);
	resY = DS325Config::GetImageResolutionY(resID);
}


ImageDS325Stream::~ImageDS325Stream() {
	stop();
}

OniStatus ImageDS325Stream::SetVideoMode(OniVideoMode*) {return ONI_STATUS_NOT_IMPLEMENTED;}
OniStatus ImageDS325Stream::GetVideoMode(OniVideoMode* pVideoMode) {
	pVideoMode->pixelFormat = ONI_PIXEL_FORMAT_RGB888;
	pVideoMode->fps = 30;
	pVideoMode->resolutionX = resX;
	pVideoMode->resolutionY = resY;
	return ONI_STATUS_OK;
}

OniStatus ImageDS325Stream::start() {
	OniStatus ret;
	int device_id = DS325Config::GetDeviceID();

	ret = start_stream(device_id, resX, resY);

	if(ret != ONI_STATUS_ERROR) {
		rgb_image = (unsigned char *)malloc(sizeof(unsigned char) * (resX * resY * 3));
	}
	return ret;
}

void ImageDS325Stream::stop() {
	stop_stream();
	if(rgb_image) free(rgb_image);
	rgb_image = NULL;
}

#define SAT(c) \
        if (c & (~255)) { if (c < 0) c = 0; else c = 255; }
void ImageDS325Stream::set_image(unsigned char *src, int size) {
	if(!rgb_image) {
		return;
	}
	unsigned char *s;
	unsigned char *lrgb_image = NULL;
	int l, c;
	int r, g, b, cr, cg, cb, y1, y2;

	l = resY;
	s = src;
	lrgb_image = &rgb_image[0];
	while(l--) {
		c = resX >> 1;
		while(c--) {
			y1 = *s++;
			cb = ((*s - 128) * 454) >> 8;
			cg = (*s++ - 128) * 88;
			y2 = *s++;
			cr = ((*s - 128) * 359) >> 8;
			cg = (cg + (*s++ - 128) * 183) >> 8;

			r = y1 + cr;
			b = y1 + cb;
			g = y1 - cg;
			SAT(r);
			SAT(g);
			SAT(b);

			*lrgb_image++ = r;
			*lrgb_image++ = g;
			*lrgb_image++ = b;

			r = y2 + cr;
			b = y2 + cb;
			g = y2 - cg;
			SAT(r);
			SAT(g);
			SAT(b);

			*lrgb_image++ = r;
			*lrgb_image++ = g;
			*lrgb_image++ = b;
		}
	}
}

void ImageDS325Stream::Mainloop() {
	int frameId = 1;
	while(m_running) {
		read_frame_once();

		OniFrame* pFrame = getServices().acquireFrame();

		if(pFrame == NULL) {printf("Didn't get frame...\n"); continue;}

		// Fill frame
		xnOSMemSet(pFrame->data, 0, pFrame->dataSize);

		// Fill metadata
		pFrame->frameIndex = frameId;

		pFrame->videoMode.pixelFormat = ONI_PIXEL_FORMAT_RGB888;
		pFrame->videoMode.resolutionX = resX;
		pFrame->videoMode.resolutionY = resY;
		pFrame->videoMode.fps = 30;

		pFrame->width = resX;
		pFrame->height = resY;

		xnOSMemCopy(pFrame->data, &rgb_image[0], resX * resY * 3);

		pFrame->cropOriginX = pFrame->cropOriginY = 0;
		pFrame->croppingEnabled = FALSE;

		pFrame->sensorType = ONI_SENSOR_COLOR;
		pFrame->stride = resX*3;
		pFrame->timestamp = frameId*33000;

		raiseNewFrame(pFrame);
		getServices().releaseFrame(pFrame);

		frameId++;

		xnOSSleep(33);
	}
}
