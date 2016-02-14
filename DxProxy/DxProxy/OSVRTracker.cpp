#include "OSVRTracker.h"
#include "D3DProxyDevice.h"

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

	if (status == MTS_INITIALISING)
		status = MTS_OK;
}

void OSVRTracker::resetOrientationAndPosition()
{

}

void OSVRTracker::resetPosition()
{

}

int  OSVRTracker::getOrientationAndPosition(float* yaw, float* pitch, float* roll, float* x, float* y, float* z)
{
	m_Context.update();
	
	OSVR_PoseState state;
	OSVR_TimeValue timestamp;
	OSVR_ReturnCode ret = osvrGetPoseState(m_HeadInterface.get(), &timestamp, &state);
	
	if (ret == OSVR_RETURN_SUCCESS)
	{
		*x = osvrVec3GetX(&state.translation);
		*y = osvrVec3GetY(&state.translation);
		*z = osvrVec3GetZ(&state.translation);

		float qw = osvrQuatGetW(&state.rotation);
		float qx = osvrQuatGetX(&state.rotation);
		float qy = osvrQuatGetY(&state.rotation);
		float qz = osvrQuatGetZ(&state.rotation);

		*pitch= atan2(2.0 * (qy*qz + qw*qx), qw*qw - qx*qx - qy*qy + qz*qz) * (180.0 / PI);
		*yaw  =-asin(-2.0 * (qx*qz - qw*qy)) * (180.0 / PI);
		*roll = atan2(2.0 * (qx*qy + qw*qz), qw*qw + qx*qx - qy*qy - qz*qz) * (180.0 / PI);

		//vireio::debugf("x: %8.4f, y:%8.4f, z:%8.4f, yaw: %8.4f, pitch: %8.4f, roll: %8.4f");
		return MTS_OK;
	}
	else
	{
		return MTS_LOSTPOSITIONAL;
	}
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
