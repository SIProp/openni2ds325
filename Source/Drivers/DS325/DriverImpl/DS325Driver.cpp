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
#include "DS325Driver.h"
#include "DS325Device.h"
#include "DS325Stream.h"
#include "DS325Config.h"

DS325Driver::DS325Driver(OniDriverServices* pDriverServices) : DriverBase(pDriverServices){}

OniStatus DS325Driver::initialize(
	oni::driver::DeviceConnectedCallback connectedCallback,
	oni::driver::DeviceDisconnectedCallback disconnectedCallback,
	oni::driver::DeviceStateChangedCallback deviceStateChangedCallback,
	void* pCookie) {

	oni::driver::DriverBase::initialize(connectedCallback, disconnectedCallback, deviceStateChangedCallback, pCookie);

	OniDeviceInfo* pInfo = XN_NEW(OniDeviceInfo);
	xnOSStrCopy(pInfo->uri, "DS325", ONI_MAX_STR);
	xnOSStrCopy(pInfo->name, "DS325", ONI_MAX_STR);
	xnOSStrCopy(pInfo->vendor, "SoftKinetic", ONI_MAX_STR);
	m_devices[pInfo] = NULL;

	deviceConnected(pInfo);
	deviceStateChanged(pInfo, 0);

	return ONI_STATUS_OK;
}

oni::driver::DeviceBase* DS325Driver::deviceOpen(const char* uri, const char* /*mode*/) {
	for(xnl::Hash<OniDeviceInfo*, oni::driver::DeviceBase*>::Iterator iter = m_devices.Begin(); iter != m_devices.End(); ++iter) {
		if(xnOSStrCmp(iter->Key()->uri, uri) == 0) {
			// Found
			if(iter->Value() != NULL) {
				// already using
				return iter->Value();
			}

			DS325Device* pDevice = XN_NEW(DS325Device, iter->Key(), getServices());
			iter->Value() = pDevice;

			return pDevice;
		}
	}

	getServices().errorLoggerAppend("Looking for '%s'", uri);
	return NULL;
}

void DS325Driver::deviceClose(oni::driver::DeviceBase* pDevice) {
	for(xnl::Hash<OniDeviceInfo*, oni::driver::DeviceBase*>::Iterator iter = m_devices.Begin(); iter != m_devices.End(); ++iter) {
		if(iter->Value() == pDevice) {
			iter->Value() = NULL;
			XN_DELETE(pDevice);
			return;
		}
	}

	// not our device?!
	XN_ASSERT(FALSE);
}

OniStatus DS325Driver::tryDevice(const char* uri) {
	return ONI_STATUS_OK;
}

void DS325Driver::shutdown() {}

ONI_EXPORT_DRIVER(DS325Driver);
