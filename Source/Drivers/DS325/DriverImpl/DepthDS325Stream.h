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
#ifndef __DEPTH_DS325_STREAM_H__
#define __DEPTH_DS325_STREAM_H__

#include "DS325Stream.h"
#include "DepthDS325StreamData.h"

class DepthDS325Stream : public DS325Stream {
public:
	DepthDS325Stream();
	~DepthDS325Stream();

	virtual OniStatus SetVideoMode(OniVideoMode*);
	virtual OniStatus GetVideoMode(OniVideoMode* pVideoMode);

	virtual OniStatus start();
	virtual void stop();

protected:
	int ds325_counter;
	bool isContour;
	DepthDS325StreamData *data;

	virtual int init_cmd(int send_num);
	virtual int end_cmd();
	virtual void set_image(unsigned char *src, int size);

private:
	virtual void Mainloop();

};

#endif // __DEPTH_DS325_STREAM_H__

