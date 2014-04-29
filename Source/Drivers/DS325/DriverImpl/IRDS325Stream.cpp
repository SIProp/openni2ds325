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
#include "IRDS325Stream.h"
#include "DS325Config.h"

IRDS325Stream::IRDS325Stream() {
	data = DepthDS325StreamData::GetInstance();
	isContour = false;
}

IRDS325Stream::~IRDS325Stream() {
	stop();
}

OniStatus IRDS325Stream::SetVideoMode(OniVideoMode*) {return ONI_STATUS_NOT_IMPLEMENTED;}
OniStatus IRDS325Stream::GetVideoMode(OniVideoMode* pVideoMode) {
	pVideoMode->pixelFormat = ONI_PIXEL_FORMAT_RGB888;
	pVideoMode->fps = 30;
	pVideoMode->resolutionX = DEPTHSENSE_DEPTH_RESOLUTION_X;
	pVideoMode->resolutionY = DEPTHSENSE_DEPTH_RESOLUTION_Y;
	return ONI_STATUS_OK;
}

OniStatus IRDS325Stream::start() {
	if(data->m_instance_owner == DEPTH) {
		xnOSCreateThread(threadFunc, this, &m_threadHandle);
		return ONI_STATUS_OK;
	}
	data->m_instance_owner = IR;
	ds325_counter = 0;

	OniStatus ret;
	int device_id = DS325Config::GetDeviceID();
	isContour = DS325Config::GetWithContour();

	ret = start_stream(device_id+1, DEPTH_V4L_WIDTH, DEPTH_V4L_HEIGHT);

	return ret;
}

void IRDS325Stream::stop() {
	if(data->m_instance_owner != IR) {
		m_running = false;
		return;
	}
	data->m_instance_owner = NONE;
	
	end_cmd();
	stop_stream();
}

void IRDS325Stream::Mainloop() {
	int frameId = 1;
	while(m_running) {
		if(data->m_instance_owner == IR) {
			if(ds325_counter < DS325_INIT_MAX) {
				init_cmd(ds325_counter);
				ds325_counter++;
			}
			read_frame_once();
		}

		OniFrame* pFrame = getServices().acquireFrame();

		if(pFrame == NULL) {printf("Didn't get frame...\n"); continue;}

		// Fill frame
		xnOSMemSet(pFrame->data, 0, pFrame->dataSize);

		// Fill metadata
		pFrame->frameIndex = frameId;

		pFrame->videoMode.pixelFormat = ONI_PIXEL_FORMAT_RGB888;
		pFrame->videoMode.resolutionX = DEPTHSENSE_DEPTH_RESOLUTION_X;
		pFrame->videoMode.resolutionY = DEPTHSENSE_DEPTH_RESOLUTION_Y;
		pFrame->videoMode.fps = 30;

		pFrame->width = DEPTHSENSE_DEPTH_RESOLUTION_X;
		pFrame->height = DEPTHSENSE_DEPTH_RESOLUTION_Y;

		xnOSMemCopy(pFrame->data, &(data->ir_image)[0], DEPTHSENSE_DEPTH_RESOLUTION_X*DEPTHSENSE_DEPTH_RESOLUTION_Y * 3);

		pFrame->cropOriginX = pFrame->cropOriginY = 0;
		pFrame->croppingEnabled = FALSE;

		pFrame->sensorType = ONI_SENSOR_IR;
		pFrame->stride = DEPTHSENSE_DEPTH_RESOLUTION_X*3;
		pFrame->timestamp = frameId*33000;

		raiseNewFrame(pFrame);
		getServices().releaseFrame(pFrame);

		frameId++;

		xnOSSleep(33);
	}
}
