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
#include "DepthDS325Stream.h"
#include "IRDS325Stream.h"
#include "DS325Device.h"


DS325Device::DS325Device(OniDeviceInfo* pInfo, oni::driver::DriverServices& driverServices) : m_pInfo(pInfo), m_driverServices(driverServices) {
	m_numSensors = 3;

	m_sensors[0].pSupportedVideoModes = XN_NEW_ARR(OniVideoMode, 1);
	m_sensors[0].sensorType = ONI_SENSOR_DEPTH;
	m_sensors[0].numSupportedVideoModes = 1;
	m_sensors[0].pSupportedVideoModes[0].pixelFormat = ONI_PIXEL_FORMAT_DEPTH_1_MM;
	m_sensors[0].pSupportedVideoModes[0].fps = 30;
	m_sensors[0].pSupportedVideoModes[0].resolutionX = DEPTHSENSE_DEPTH_RESOLUTION_X;
	m_sensors[0].pSupportedVideoModes[0].resolutionY = DEPTHSENSE_DEPTH_RESOLUTION_Y;

	m_sensors[1].pSupportedVideoModes = XN_NEW_ARR(OniVideoMode, 1);
	m_sensors[1].sensorType = ONI_SENSOR_COLOR;
	m_sensors[1].numSupportedVideoModes = 1;
	m_sensors[1].pSupportedVideoModes[0].pixelFormat = ONI_PIXEL_FORMAT_RGB888;
	m_sensors[1].pSupportedVideoModes[0].fps = 30;
	m_sensors[1].pSupportedVideoModes[0].resolutionX = DEPTHSENSE_COLOR_RESOLUTION_X;
	m_sensors[1].pSupportedVideoModes[0].resolutionY = DEPTHSENSE_COLOR_RESOLUTION_Y;

	m_sensors[2].pSupportedVideoModes = XN_NEW_ARR(OniVideoMode, 1);
	m_sensors[2].sensorType = ONI_SENSOR_IR;
	m_sensors[2].numSupportedVideoModes = 1;
	m_sensors[2].pSupportedVideoModes[0].pixelFormat = ONI_PIXEL_FORMAT_RGB888;
	m_sensors[2].pSupportedVideoModes[0].fps = 30;
	m_sensors[2].pSupportedVideoModes[0].resolutionX = DEPTHSENSE_DEPTH_RESOLUTION_X;
	m_sensors[2].pSupportedVideoModes[0].resolutionY = DEPTHSENSE_DEPTH_RESOLUTION_Y;
}

OniDeviceInfo* DS325Device::GetInfo() {
	return m_pInfo;
}

OniStatus DS325Device::getSensorInfoList(OniSensorInfo** pSensors, int* numSensors) {
	*numSensors = m_numSensors;
	*pSensors = m_sensors;

	return ONI_STATUS_OK;
}

oni::driver::StreamBase* DS325Device::createStream(OniSensorType sensorType) {
	if(sensorType == ONI_SENSOR_DEPTH) {
		DepthDS325Stream* pDepth = XN_NEW(DepthDS325Stream);
		return pDepth;
	}
	if(sensorType == ONI_SENSOR_IR) {
		IRDS325Stream* pIR = XN_NEW(IRDS325Stream);
		return pIR;
	}
	if(sensorType == ONI_SENSOR_COLOR) {
		ImageDS325Stream* pImage = XN_NEW(ImageDS325Stream);
		return pImage;
	}

	m_driverServices.errorLoggerAppend("DS325Device: Can't create a stream of type %d", sensorType);
	return NULL;
}

void DS325Device::destroyStream(oni::driver::StreamBase* pStream) {
	XN_DELETE(pStream);
}

OniStatus  DS325Device::getProperty(int propertyId, void* data, int* pDataSize) {
	OniStatus rc = ONI_STATUS_OK;

	switch(propertyId) {
	case ONI_DEVICE_PROPERTY_DRIVER_VERSION:
		{
			if(*pDataSize == sizeof(OniVersion)) {
				OniVersion* version = (OniVersion*)data;
				version->major = version->minor = version->maintenance = version->build = 2;
			} else {
				m_driverServices.errorLoggerAppend("Unexpected size: %d != %d\n", *pDataSize, sizeof(OniVersion));
				rc = ONI_STATUS_ERROR;
			}
		}
		break;
	default:
		m_driverServices.errorLoggerAppend("Unknown property: %d\n", propertyId);
		rc = ONI_STATUS_ERROR;
	}
	return rc;
}

