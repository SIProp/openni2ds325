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
#ifndef __DS325_DEVICE_H__
#define __DS325_DEVICE_H__

#include "Driver/OniDriverAPI.h"
#include "XnLib.h"
#include "XnHash.h"
#include "XnEvent.h"
#include "XnPlatform.h"

class DS325Device : public oni::driver::DeviceBase {
public:
	DS325Device(OniDeviceInfo* pInfo, oni::driver::DriverServices& driverServices);
	OniDeviceInfo* GetInfo();
	OniStatus getSensorInfoList(OniSensorInfo** pSensors, int* numSensors);

	oni::driver::StreamBase* createStream(OniSensorType sensorType);

	void destroyStream(oni::driver::StreamBase* pStream);

	OniStatus  getProperty(int propertyId, void* data, int* pDataSize);

private:
	DS325Device(const DS325Device&);
	void operator=(const DS325Device&);

	OniDeviceInfo* m_pInfo;
	int m_numSensors;
	OniSensorInfo m_sensors[10];
	oni::driver::DriverServices& m_driverServices;
};

#endif // __DS325_DEVICE_H__
