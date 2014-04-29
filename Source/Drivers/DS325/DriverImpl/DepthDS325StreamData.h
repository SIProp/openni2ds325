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
#ifndef __DEPTH_DS325_STREAM_DATA_H__
#define __DEPTH_DS325_STREAM_DATA_H__

#include <malloc.h>

#define DEPTHSENSE_DEPTH_RESOLUTION_X 320
#define DEPTHSENSE_DEPTH_RESOLUTION_Y 240

#define DEPTH_V4L_WIDTH 640
#define DEPTH_V4L_HEIGHT 240

#define DS325_INIT_MAX 82

enum OWNER { NONE , DEPTH , IR };

class DepthDS325StreamData {
public:
	 static DepthDS325StreamData* GetInstance() {
	    	static DepthDS325StreamData instance;
	    	return &instance;
	}

	unsigned short *depth_map;
	unsigned char  *ir_image;
	enum OWNER m_instance_owner;

private:
	DepthDS325StreamData() {
		depth_map = (unsigned short *)malloc(sizeof(unsigned short) * (DEPTHSENSE_DEPTH_RESOLUTION_X*DEPTHSENSE_DEPTH_RESOLUTION_Y));
		ir_image  = (unsigned char  *)malloc(sizeof(unsigned char)  * (DEPTHSENSE_DEPTH_RESOLUTION_X*DEPTHSENSE_DEPTH_RESOLUTION_Y * 3));
		m_instance_owner = NONE;
	}
	~DepthDS325StreamData() {
		if(depth_map) free(depth_map);
		if(ir_image) free(ir_image);
	}
	DepthDS325StreamData(const DepthDS325StreamData& rhs);
	DepthDS325StreamData& operator=(const DepthDS325StreamData& rhs);
};

#endif // __DEPTH_DS325_STREAM_DATA_H__

