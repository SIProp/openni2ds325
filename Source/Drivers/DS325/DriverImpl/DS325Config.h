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
#ifndef __DS325_CONFIG_H__
#define __DS325_CONFIG_H__

#include "XnLib.h"
#include "XnHash.h"
#include "XnEvent.h"
#include "XnPlatform.h"

#define DS325_DEVICE_ID 0
#define RESOLUTION_ID   1

// Dummy function used only for taking its address for the sake of xnOSGetModulePathForProcAddress.
static void dummyFunctionToTakeAddress() {}
static const char* DS325_CONFIGURATION_FILE = "DS325.ini";
#define XN_FILE_MAX_PATH 256

class DS325Config {
public:
	 static int GetDeviceID() {
		int device_id = DS325_DEVICE_ID;
		XnStatus rc;
		XnChar strModulePath[XN_FILE_MAX_PATH];
		XnChar strOniConfigurationFile[XN_FILE_MAX_PATH];
		XnBool configurationFileExists = FALSE;
		XnChar strBaseDir[XN_FILE_MAX_PATH];
		rc = xnOSGetModulePathForProcAddress(reinterpret_cast<void*>(&dummyFunctionToTakeAddress), strModulePath);
		if(rc != XN_STATUS_OK) return device_id;

		rc = xnOSGetDirName(strModulePath, strBaseDir, XN_FILE_MAX_PATH);
		if(rc != XN_STATUS_OK) return device_id;

		xnOSStrCopy(strOniConfigurationFile, strBaseDir, XN_FILE_MAX_PATH);
		rc = xnOSAppendFilePath(strOniConfigurationFile, DS325_CONFIGURATION_FILE, XN_FILE_MAX_PATH);
		if(rc == XN_STATUS_OK) {
			xnOSDoesFileExist(strOniConfigurationFile, &configurationFileExists);
		}
		if(configurationFileExists) {
			XnInt32 deviceID = 0;
			rc = xnOSReadIntFromINI(strOniConfigurationFile, "Device", "deviceID", &deviceID);
			if (rc == XN_STATUS_OK) device_id = (int)deviceID;
		}
		return device_id;
	 }
	 static bool GetWithContour() {
		bool isContour = false;
		XnStatus rc;
		XnChar strModulePath[XN_FILE_MAX_PATH];
		XnChar strOniConfigurationFile[XN_FILE_MAX_PATH];
		XnBool configurationFileExists = FALSE;
		XnChar strBaseDir[XN_FILE_MAX_PATH];
		rc = xnOSGetModulePathForProcAddress(reinterpret_cast<void*>(&dummyFunctionToTakeAddress), strModulePath);
		if(rc != XN_STATUS_OK) return isContour;

		rc = xnOSGetDirName(strModulePath, strBaseDir, XN_FILE_MAX_PATH);
		if(rc != XN_STATUS_OK) return isContour;

		xnOSStrCopy(strOniConfigurationFile, strBaseDir, XN_FILE_MAX_PATH);
		rc = xnOSAppendFilePath(strOniConfigurationFile, DS325_CONFIGURATION_FILE, XN_FILE_MAX_PATH);
		if(rc == XN_STATUS_OK) {
			xnOSDoesFileExist(strOniConfigurationFile, &configurationFileExists);
		}
		if(configurationFileExists) {
			XnChar withContour[XN_FILE_MAX_PATH] = {0};
			rc = xnOSReadStringFromINI(strOniConfigurationFile, "Depth", "withContour", withContour, XN_FILE_MAX_PATH);
			if(xnOSStrCmp(withContour, "true") == 0 || xnOSStrCmp(withContour, "TRUE") == 0) {
				isContour = true;
			}
		}
		return isContour;
	 }
	 static int GetImageResolutionID() {
		int resolution_id = RESOLUTION_ID;
		XnStatus rc;
		XnChar strModulePath[XN_FILE_MAX_PATH];
		XnChar strOniConfigurationFile[XN_FILE_MAX_PATH];
		XnBool configurationFileExists = FALSE;
		XnChar strBaseDir[XN_FILE_MAX_PATH];
		rc = xnOSGetModulePathForProcAddress(reinterpret_cast<void*>(&dummyFunctionToTakeAddress), strModulePath);
		if(rc != XN_STATUS_OK) return resolution_id;

		rc = xnOSGetDirName(strModulePath, strBaseDir, XN_FILE_MAX_PATH);
		if(rc != XN_STATUS_OK) return resolution_id;

		xnOSStrCopy(strOniConfigurationFile, strBaseDir, XN_FILE_MAX_PATH);
		rc = xnOSAppendFilePath(strOniConfigurationFile, DS325_CONFIGURATION_FILE, XN_FILE_MAX_PATH);
		if(rc == XN_STATUS_OK) {
			xnOSDoesFileExist(strOniConfigurationFile, &configurationFileExists);
		}
		if(configurationFileExists) {
			XnInt32 resID = 0;
			rc = xnOSReadIntFromINI(strOniConfigurationFile, "Device", "imageResolution", &resID);
			if (rc == XN_STATUS_OK) resolution_id = (int)resID;
		}
		return resolution_id;
	 }
	 static int GetImageResolutionX(int ID) {
		 switch(ID) {
		 case 0:
			 return 160;
		 case 1:
			 return 320;
		 case 2:
			 return 640;
		 default:
			 return 320;
		 }
	 }
	 static int GetImageResolutionY(int ID) {
		 switch(ID) {
		 case 0:
			 return 120;
		 case 1:
			 return 240;
		 case 2:
			 return 480;
		 default:
			 return 240;
		 }
	 }

};

#endif /* __DS325_CONFIG_H__ */
