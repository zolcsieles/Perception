#include "OSVRTracker.h"
#include "D3DProxyDevice.h"
#include "Extras/OVR_Math.h"

OSVRTracker::OSVRTracker() :
	m_Context("com.vireio.OSVRTracker")
{
	status = MTS_NOTINIT;
	init();
}

OSVRTracker::~OSVRTracker()
{
	//Destroy everything
	if (m_HeadInterface.notEmpty())
	{
		m_HeadInterface.free();
	}
}


//Override virtuals

/**
* OSVR Init
***/
void OSVRTracker::init()
{
	OutputDebugString("!!!OSVRTracker Start");
	status = MTS_INITIALISING;

	const std::time_t waitTime = 5; //wait up to 5 seconds for init
	if (m_HeadInterface.notEmpty())
	{
		m_HeadInterface.free();
	}

	//
 	std::time_t startTime = std::time(nullptr);
 	while (!m_Context.checkStatus())
 	{
 		m_Context.update();
 		if (std::time(nullptr) > startTime + waitTime)
 		{
 			status = MTS_DRIVERFAIL;
 			return;
 		}
 	}

	m_HeadInterface = m_Context.getInterface("/me/head");
	m_HeadInterface.registerCallback(&TrackerCallback, this);

	if (status == MTS_INITIALISING)
		status = MTS_OK;
}

void OSVRTracker::resetOrientationAndPosition()
{
	resetOrientationAndPosition();
}

void OSVRTracker::resetPosition()
{
	offsetX = 0.0f;
	offsetY = 0.0f;
	offsetZ = 0.0f;
}

void OSVRTracker::TrackerCallback(void* userdata, const OSVR_TimeValue* timestamp, const OSVR_PoseReport* report)
{
	double x = osvrVec3GetX(&report->pose.translation);
	double y = osvrVec3GetY(&report->pose.translation);
	double z = osvrVec3GetZ(&report->pose.translation);

	double qw = osvrQuatGetW(&report->pose.rotation);
	double qx = osvrQuatGetX(&report->pose.rotation);
	double qy = osvrQuatGetY(&report->pose.rotation);
	double qz = osvrQuatGetZ(&report->pose.rotation);

	float yaw = RADIANS_TO_DEGREES(asin(-2.0 * (qx*qz - qw*qy)));
	float pitch = RADIANS_TO_DEGREES(atan2(2.0 * (qy*qz + qw*qx), qw*qw - qx*qx - qy*qy + qz*qz));
	float roll = RADIANS_TO_DEGREES(atan2(2.0 * (qx*qy + qw*qz), qw*qw + qx*qx - qy*qy - qz*qz));

	{
		OSVRTracker* thiz = static_cast<OSVRTracker*>(userdata);
		std::lock_guard<std::mutex> lock(thiz->m_mtx);
		thiz->osvr_x = (float)x;
		thiz->osvr_y = (float)y;
		thiz->osvr_z = (float)z;
		thiz->osvr_yaw = yaw;
		thiz->osvr_pitch = pitch;
		thiz->osvr_roll = roll;
	}
}

int  OSVRTracker::getOrientationAndPosition(float* yaw, float* pitch, float* roll, float* x, float* y, float* z)
{
	m_Context.update();

	{
		std::lock_guard<std::mutex> lock(m_mtx);

		primaryYaw = OSVRTracker::osvr_yaw - offsetYaw;
		primaryPitch = OSVRTracker::osvr_pitch - offsetPitch;
		primaryRoll = OSVRTracker::osvr_roll - offsetRoll;

		primaryX = OSVRTracker::osvr_x - offsetX;
		primaryY = OSVRTracker::osvr_y - offsetY;
		primaryZ = OSVRTracker::osvr_z - offsetZ;
	}

	*yaw = -primaryYaw;
	*pitch = primaryPitch;
	*roll = -primaryRoll;
	*x = primaryX;
	*y = primaryY;
	*z = primaryZ;

	return MTS_OK;
}

void OSVRTracker::updateOrientationAndPosition()
{
	SHOW_CALL("OSVRTracker updateOrientation\n");
	
	if (getOrientationAndPosition(&yaw, &pitch, &roll, &x, &y, &z) >= MTS_OK)
	{
		//Convert yaw, pitch to positive degrees
		// (-180.0f...0.0f -> 180.0f...360.0f)
		// (0.0f...180.0f  -> 0.0f...180.0f)
		yaw = fmodf(yaw + 360.0f, 360.0f);
		pitch = -fmodf(pitch + 360.0f, 360.0f);

		//Get difference
		deltaYaw += yaw - currentYaw;
		deltaPitch += pitch - currentPitch;

		// hack to avoid errors while translating over 360/0
		if (fabs(deltaYaw) > 4.0f) deltaYaw = 0.0f;
		if (fabs(deltaPitch) > 4.0f) deltaPitch = 0.0f;

		//Pass to mouse data (long integer).
		mouseData.mi.dx = (long)(deltaYaw*multiplierYaw);
		mouseData.mi.dy = (long)(deltaPitch*multiplierPitch);

		//Keep fractional difference in the delta so it's added to the next update.
		deltaYaw -= ((float)mouseData.mi.dx) / multiplierYaw;
		deltaPitch -= ((float)mouseData.mi.dy) / multiplierPitch;

		// Send to mouse input.
		if (mouseEmulation)
			SendInput(1, &mouseData, sizeof(INPUT));

		// Set current data.
		currentYaw = yaw;
		currentPitch = pitch;
		currentRoll = (float)(roll*(PI / 180.0)*multiplierRoll); //convert from deg to radians then apply multiplier
	}
}

/**
* Returns osvr tracker status as Motion Tracker Status enum
***/
MotionTrackerStatus OSVRTracker::getStatus()
{
	return status;
}

char* OSVRTracker::GetTrackerDescription()
{
	return "OSVRTracker";
}

bool OSVRTracker::SupportsPositionTracking()
{
	return supportsPositional;
}
