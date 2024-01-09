#pragma once
#include "ndAnimationKeyframesTrack.h"

class ndAnimationPose;

class ndAnimationSequenceBase : public ndClassAlloc
{
	public:
	ndAnimationSequenceBase();
	virtual ~ndAnimationSequenceBase();

	const ndString& GetName() const;
	void SetName(const char* const name);
	virtual void CalculatePose(ndAnimationPose& output, ndFloat32 param) const = 0;

	protected:
	ndString m_name;
};
