#ifndef OSVRTRACKER_H_INCLUDED
#define OSVRTRACKER_H_INCLUDED

#include "MotionTracker.h"

#include "OSVR_MS_Undef.h" //Beautiful hack

#pragma comment(lib,"osvrClientKit")

#include <osvr/ClientKit/Context.h>
#include <osvr/ClientKit/Interface.h>
#include <osvr/ClientKit/InterfaceStateC.h>

#include "OSVR_MS_Def.h" //Beautiful hack

class OSVRTracker : public MotionTracker
{
public:
	OSVRTracker();
	virtual ~OSVRTracker();

	/*** OSVR public methods ***/
	void init();
	void resetOrientationAndPosition();
	void resetPosition();
	int  getOrientationAndPosition(float* yaw, float* pitch, float* roll, float* x, float* y, float* z);
	void updateOrientationAndPosition();
	MotionTrackerStatus getStatus();
	virtual char* GetTrackerDescription();
	virtual bool SupportsPositionTracking();

private:

	bool supportsPositional; //Have camera

	MotionTrackerStatus status;

 	osvr::clientkit::ClientContext m_Context;
 	osvr::clientkit::Interface m_HeadInterface;
};

#endif