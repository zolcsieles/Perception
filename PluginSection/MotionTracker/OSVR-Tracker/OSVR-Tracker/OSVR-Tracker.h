﻿/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OSVR Tracker - Open Source Virtual Reality Tracking Node
Copyright (C) 2016 Denis Reischl

File <OSVR-Tracker.h> and
Class <OSVR-Tracker> :
Copyright (C) 2015 Denis Reischl

The stub class <AQU_Nodus> is the only public class from the Aquilinus 
repository and permitted to be used for open source plugins of any kind. 
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include"AQU_Nodus.h"
#include"Resources.h"
#include<stdio.h>
#include<sstream>

#include <osvr/ClientKit/ContextC.h>
#include <osvr/ClientKit/InterfaceC.h>
#include <osvr/ClientKit/InterfaceStateC.h>
#pragma comment(lib, "osvrClientKit.lib")

#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112

#define NUMBER_OF_COMMANDERS                          10

#define FLOAT_PI                            (3.1415926f)

/**
* Node Commander Enumeration.
***/
enum OSVR_Commanders
{
	Pitch,
	Yaw,
	Roll,
	OrientationW,
	OrientationX,
	OrientationY,
	OrientationZ,
	PositionX,
	PositionY,
	PositionZ,
};

/**
* Vireio Open Source VR Tracker Node Plugin.
***/
class OSVR_Tracker : public AQU_Nodus
{
public:
	OSVR_Tracker();
	virtual ~OSVR_Tracker();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4+256+4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetCommandersNumber() { return NUMBER_OF_COMMANDERS; }
	virtual LPWSTR          GetCommanderName(DWORD dwCommanderIndex);
	virtual DWORD           GetCommanderType(DWORD dwCommanderIndex);
	virtual void*           GetOutputPointer(DWORD dwCommanderIndex);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);
private:
	/**
	* The OSVR client context handle.
	***/
	OSVR_ClientContext m_psOSVR_ClientContext;
	/**
	* The OSVR client interface handle.
	***/
	OSVR_ClientInterface m_psOSVR_ClientInterface;
	/**
	* The OSVR pose state.
	* Contains both position and orientation.
	***/
	OSVR_PoseState m_sState;
	/**
	* The OSVR pose state time stamp.
	***/
	OSVR_TimeValue m_sTimestamp;
	/**
	* The control bitmap.
	***/
	HBITMAP m_hBitmapControl;
	/**
	* The control update bool.
	***/
	bool m_bControlUpdate;
	/**
	* The font used.
	***/
	HFONT m_hFont;
	/**
	* Euler angles.
	***/
	float m_fYaw, m_fRoll, m_fPitch;
	/**
	* Position (float).
	***/
	float m_afTranslation[3];
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	OSVR_Tracker* pOSVR_Tracker = new OSVR_Tracker();
	return static_cast<AQU_Nodus*>(pOSVR_Tracker);
}
