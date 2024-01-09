#include "ndCommon.h"
#include "ndOpenGlUtil.h"
#include "ndHighResolutionTimer.h"

static ndUnsigned64 m_prevTime = 0;

void dResetTimer()
{
	m_prevTime = ndGetTimeInMicroseconds();
}

ndFloat32 dGetElapsedSeconds()
{
	const ndFloat64 TICKS2SEC = 1.0e-6f;
	ndUnsigned64 microseconds = ndGetTimeInMicroseconds();

	ndFloat32 timeStep = ndFloat32 ((ndFloat64)(microseconds - m_prevTime) * TICKS2SEC);
	m_prevTime = microseconds;

	return timeStep;
} 

