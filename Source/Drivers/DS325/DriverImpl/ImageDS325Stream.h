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
#ifndef __IMAGE_DS325_STREAM_H__
#define __IMAGE_DS325_STREAM_H__

#include "DS325Stream.h"

class ImageDS325Stream : public DS325Stream {
public:
	~ImageDS325Stream();

	virtual OniStatus SetVideoMode(OniVideoMode*);
	virtual OniStatus GetVideoMode(OniVideoMode* pVideoMode);

	virtual OniStatus start();
	virtual void stop();


protected:
	virtual void set_image(unsigned char *src, int size);


private:
	unsigned char *rgb_image;

	virtual void Mainloop();

};

#endif // __IMAGE_DS325_STREAM_H__
