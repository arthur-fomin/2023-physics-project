#pragma once 
#include "ndAnimationSequenceBase.h"
#include "ndAnimationKeyframesTrack.h"

class ndAnimationSequence: public ndAnimationSequenceBase
{
	public:
	ndAnimationSequence();
	virtual ~ndAnimationSequence();

	ndAnimationKeyFramesTrack* AddTrack();
	ndList<ndAnimationKeyFramesTrack>& GetTracks();
	virtual void CalculatePose(ndAnimationPose& output, ndFloat32 param) const;

	private:
	ndList<ndAnimationKeyFramesTrack> m_tracks;
};

