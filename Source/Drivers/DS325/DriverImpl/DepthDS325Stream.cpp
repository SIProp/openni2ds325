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
#include "DepthDS325Stream.h"
#include "DS325Config.h"

DepthDS325Stream::DepthDS325Stream() {
	data = DepthDS325StreamData::GetInstance();
	isContour = false;
}

DepthDS325Stream::~DepthDS325Stream() {
	stop();
}

OniStatus DepthDS325Stream::SetVideoMode(OniVideoMode*) {return ONI_STATUS_NOT_IMPLEMENTED;}
OniStatus DepthDS325Stream::GetVideoMode(OniVideoMode* pVideoMode) {
	pVideoMode->pixelFormat = ONI_PIXEL_FORMAT_DEPTH_1_MM;
	pVideoMode->fps = 30;
	pVideoMode->resolutionX = DEPTHSENSE_DEPTH_RESOLUTION_X;
	pVideoMode->resolutionY = DEPTHSENSE_DEPTH_RESOLUTION_Y;
	return ONI_STATUS_OK;
}

OniStatus DepthDS325Stream::start() {
	if(data->m_instance_owner == IR) {
		xnOSCreateThread(threadFunc, this, &m_threadHandle);
		return ONI_STATUS_OK;
	}
	OniStatus ret;
	data->m_instance_owner = DEPTH;
	ds325_counter = 0;

	int device_id = DS325Config::GetDeviceID();
	isContour = DS325Config::GetWithContour();

	ret = start_stream(device_id+1, DEPTH_V4L_WIDTH, DEPTH_V4L_HEIGHT);

	return ret;
}

void DepthDS325Stream::stop() {
	if(data->m_instance_owner != DEPTH) {
		m_running = false;
		return;
	}
	data->m_instance_owner = NONE;

	end_cmd();
	stop_stream();
}

int DepthDS325Stream::init_cmd(int send_num) {

    int i;
    int  query[DS325_INIT_MAX]   = {UVC_SET_CUR, UVC_GET_CUR, UVC_SET_CUR, UVC_GET_CUR, UVC_SET_CUR, UVC_GET_CUR, UVC_SET_CUR, UVC_GET_CUR, UVC_SET_CUR, UVC_GET_CUR, UVC_SET_CUR, UVC_GET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_GET_CUR, UVC_SET_CUR, UVC_GET_CUR, UVC_SET_CUR, UVC_GET_CUR, UVC_SET_CUR, UVC_GET_CUR };

    unsigned char data[DS325_INIT_MAX][7];

    i=0;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x92; data[i][1] = 0x21; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x92; data[i][1] = 0x21; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1a; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1b; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x13; data[i][2] = 0x00; data[i][3] = 0x04; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x14; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x2c; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x15; data[i][2] = 0x00; data[i][3] = 0x01; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x16; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x17; data[i][2] = 0x00; data[i][3] = 0xef; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x18; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x19; data[i][2] = 0x00; data[i][3] = 0x3f; data[i][4] = 0x01; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1a; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x04; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1b; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x01; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1b; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x05; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1b; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x0d; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1c; data[i][2] = 0x00; data[i][3] = 0x05; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x20; data[i][2] = 0x00; data[i][3] = 0xb0; data[i][4] = 0x04; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x27; data[i][2] = 0x00; data[i][3] = 0x06; data[i][4] = 0x01; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x28; data[i][2] = 0x00; data[i][3] = 0x4d; data[i][4] = 0x01; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x29; data[i][2] = 0x00; data[i][3] = 0xf0; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x2a; data[i][2] = 0x00; data[i][3] = 0x4d; data[i][4] = 0x01; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x30; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x31; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x32; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x3c; data[i][2] = 0x00; data[i][3] = 0x2f; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x3d; data[i][2] = 0x00; data[i][3] = 0xe7; data[i][4] = 0x03; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x3e; data[i][2] = 0x00; data[i][3] = 0x0f; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x3f; data[i][2] = 0x00; data[i][3] = 0x0f; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x40; data[i][2] = 0x00; data[i][3] = 0xe8; data[i][4] = 0x03; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x43; data[i][2] = 0x00; data[i][3] = 0x09; data[i][4] = 0x01; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1e; data[i][2] = 0x00; data[i][3] = 0x09; data[i][4] = 0x82; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1d; data[i][2] = 0x00; data[i][3] = 0x19; data[i][4] = 0x01; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x44; data[i][2] = 0x00; data[i][3] = 0x1e; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1b; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x0d; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1b; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x4d; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x45; data[i][2] = 0x00; data[i][3] = 0x01; data[i][4] = 0x01; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x46; data[i][2] = 0x00; data[i][3] = 0x02; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x47; data[i][2] = 0x00; data[i][3] = 0x32; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x2f; data[i][2] = 0x00; data[i][3] = 0x60; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x0c; data[i][4] = 0x0c; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x01; data[i][2] = 0x00; data[i][3] = 0x0c; data[i][4] = 0x0c; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x2f; data[i][2] = 0x00; data[i][3] = 0x60; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x03; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x04; data[i][2] = 0x00; data[i][3] = 0x30; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x05; data[i][2] = 0x00; data[i][3] = 0x60; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x06; data[i][2] = 0x00; data[i][3] = 0x90; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x07; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x08; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x09; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x0a; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x02; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x0b; data[i][2] = 0x00; data[i][3] = 0x60; data[i][4] = 0xea; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x0c; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x0d; data[i][2] = 0x00; data[i][3] = 0x40; data[i][4] = 0x47; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x0e; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x0f; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x10; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x11; data[i][2] = 0x00; data[i][3] = 0xe0; data[i][4] = 0x01; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x12; data[i][2] = 0x00; data[i][3] = 0x04; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1a; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x14; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x33; data[i][2] = 0x00; data[i][3] = 0xf0; data[i][4] = 0x70; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x4a; data[i][2] = 0x00; data[i][3] = 0x02; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1a; data[i][2] = 0x00; data[i][3] = 0x80; data[i][4] = 0x14; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x12; data[i][1] = 0x1a; data[i][2] = 0x00; data[i][3] = 0xc0; data[i][4] = 0x14; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x92; data[i][1] = 0x21; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
    data[i][0] = 0x92; data[i][1] = 0x21; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
	data[i][0] = 0x92; data[i][1] = 0x35; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
	data[i][0] = 0x92; data[i][1] = 0x35; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
	data[i][0] = 0x92; data[i][1] = 0x36; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
	data[i][0] = 0x92; data[i][1] = 0x36; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
	data[i][0] = 0x92; data[i][1] = 0x35; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
i++;
	data[i][0] = 0x92; data[i][1] = 0x35; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;

    return send_xu(query[send_num], &data[send_num][0]);
}

int DepthDS325Stream::end_cmd() {
    int  i, r;
    int  query[5]   = { UVC_SET_CUR, UVC_GET_CUR, UVC_SET_CUR, UVC_SET_CUR, UVC_SET_CUR};
    unsigned char data[5][7];
    i=0;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
    i++;
    data[i][0] = 0x86; data[i][1] = 0x00; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
    i++;
    data[i][0] = 0x12; data[i][1] = 0x1a; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
    i++;
    data[i][0] = 0x12; data[i][1] = 0x1b; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;
    i++;
    data[i][0] = 0x12; data[i][1] = 0x4b; data[i][2] = 0x00; data[i][3] = 0x00; data[i][4] = 0x00; data[i][5] = 0x00; data[i][6] = 0x00;

    for(i=0; i<5; i++) {
        r = send_xu(query[i], &data[i][0]);
    }

    ds325_counter = 0;
    return r;

}


void DepthDS325Stream::set_image(unsigned char *src, int size) {

	if((!data->depth_map || !data->ir_image)) {
		return;
	}

	unsigned char *s;
	unsigned char c, cs;
	unsigned short *ldepth_map = NULL;
	int *lir_image = NULL;
	unsigned char flag1, flag2, dep, ir;
	unsigned short d;
	int r, g, b;

	ldepth_map = &(data->depth_map)[0];
	lir_image  = &(data->ir_image)[0];
	s = src;

	int step = 8; // Separated Data size
	int read = 0;
	int isDepth = 0;
	unsigned char *t;
	int i;
	for(i=0; i<DEPTH_V4L_WIDTH*DEPTH_V4L_HEIGHT-16;i++) {
		if((i % step) == 0) read++;
		if((read % 2) != 0) {
			// IR image
			ir = *s;
			t = s;
			t = t + (step*2);
			// Depth
			dep = *t;
			s++;

			flag1 = *s;
			t = s;
			t = t + (step*2);
			flag2 = *t;
			s++;

			d = 0x0000;
			// Max Range is 0x07ff. 0x0800>= is out of Range
			if(0x00 == flag1 && 0x00 == flag2) {
				// out of Long range
				d = 0x0f00 | dep;
			} else if(0xff == flag1 && 0x00 == flag2) {
				if(0xf0 > dep) {
					// Longest Range
					d = 0x0700 | dep;
				} else {
					d = 0x07ff;
				}
			} else if(0xff == flag1 && 0xff == flag2){
				// Middle Range
				if(0xc0 < dep) {
					d = 0x0600 | dep;
				} else if(0x80 < dep) {
					d = 0x0500 | dep;
				} else if(0x40 < dep) {
					d = 0x0400 | dep;
				} else if(0x00 <= dep) {
					d = 0x0300 | dep;
				}
			} else if(0x00 == flag1 && 0xff == flag2 && 0xf0 <= dep) {
				// Out of range
				d = 0x0e00 | dep;
			} else if(0x00 <= flag1 && flag1 < 0x08){
				// Short Range
				if(flag1 <= 4) {
					dep = (unsigned char)(255 - (255 * (dep+(255*flag1)) / 0x3ff));
					d = 0x0200 | dep;
				} else {
					flag1 = flag1 - 4;
					dep = (unsigned char)(255 - (255 * (dep+(255*flag1)) / 0x3ff));
					d = 0x0100 | dep;
				}
				// flag2 is contour ID.
				if(isContour) d = 0x0100 | flag2<<12 | dep;
			} else if(0x08 <= flag1){
				// Out of Short range
				d = 0x0000 | dep;
				// flag2 is contour ID.
				if(isContour) d = 0x0000 | flag2<<12 | dep;
			} else {
				//Out of Range
				d = 0x0d00;
			}

			*ldepth_map++ = d;
			*lir_image++  = 0xff000000 | ir;

		} else {
			flag1 = *s++;
			flag2 = *s++;
		}
	}
}

void DepthDS325Stream::Mainloop() {
	int frameId = 1;
	while (m_running) {
		if(data->m_instance_owner == DEPTH) {
			if(ds325_counter < DS325_INIT_MAX) {
				init_cmd(ds325_counter);
				ds325_counter++;
			}
			read_frame_once();
		}

		OniFrame* pFrame = getServices().acquireFrame();

		if (pFrame == NULL) {printf("Didn't get frame...\n"); continue;}

		// Fill frame
		xnOSMemSet(pFrame->data, 0, pFrame->dataSize);

		// Fill metadata
		pFrame->frameIndex = frameId;

		pFrame->videoMode.pixelFormat = ONI_PIXEL_FORMAT_DEPTH_1_MM;
		pFrame->videoMode.resolutionX = DEPTHSENSE_DEPTH_RESOLUTION_X;
		pFrame->videoMode.resolutionY = DEPTHSENSE_DEPTH_RESOLUTION_Y;
		pFrame->videoMode.fps = 30;

		pFrame->width = DEPTHSENSE_DEPTH_RESOLUTION_X;
		pFrame->height = DEPTHSENSE_DEPTH_RESOLUTION_Y;

		xnOSMemCopy(pFrame->data, &(data->depth_map)[0], DEPTHSENSE_DEPTH_RESOLUTION_X*DEPTHSENSE_DEPTH_RESOLUTION_Y * 2);

		pFrame->cropOriginX = pFrame->cropOriginY = 0;
		pFrame->croppingEnabled = FALSE;

		pFrame->sensorType = ONI_SENSOR_DEPTH;
		pFrame->stride = DEPTHSENSE_DEPTH_RESOLUTION_X*sizeof(OniDepthPixel);
		pFrame->timestamp = frameId*33000;

		raiseNewFrame(pFrame);
		getServices().releaseFrame(pFrame);

		frameId++;

		xnOSSleep(33);
	}
}
