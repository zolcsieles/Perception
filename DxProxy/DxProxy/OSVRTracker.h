#ifndef OSVRTRACKER_H_INCLUDED
#define OSVRTRACKER_H_INCLUDED

#include "MotionTracker.h"

#include <mutex>

#include "OSVR_MS_Undef.h" //Beautiful hack

#pragma comment(lib,"osvrClientKit")

#include <osvr/ClientKit/Context.h>
#include <osvr/ClientKit/Interface.h>

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

	std::mutex m_mtx;

	float osvr_x, osvr_y, osvr_z; //Position
	float osvr_yaw, osvr_pitch, osvr_roll; //Rotation

 	osvr::clientkit::ClientContext m_Context;
 	osvr::clientkit::Interface m_HeadInterface;

	static void TrackerCallback(void* userdata, const OSVR_TimeValue* timestamp, const OSVR_PoseReport* report);
};

#endif