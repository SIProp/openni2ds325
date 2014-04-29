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
#ifndef __IR_DS325_STREAM_H__
#define __IR_DS325_STREAM_H__

#include "DepthDS325Stream.h"

class IRDS325Stream : public DepthDS325Stream {
public:
	IRDS325Stream();
	~IRDS325Stream();

	virtual OniStatus SetVideoMode(OniVideoMode*);
	virtual OniStatus GetVideoMode(OniVideoMode* pVideoMode);

	virtual OniStatus start();
	virtual void stop();


private:
	virtual void Mainloop();
};

#endif // __IR_DS325_STREAM_H__

