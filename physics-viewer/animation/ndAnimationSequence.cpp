#include "ndCommon.h"
#include "ndAnimationPose.h"
#include "ndAnimationSequence.h"

ndAnimationSequence::ndAnimationSequence()
	:ndAnimationSequenceBase()
	,m_tracks()
{
}

ndAnimationSequence::~ndAnimationSequence()
{
}

ndList<ndAnimationKeyFramesTrack>& ndAnimationSequence::GetTracks()
{
	return m_tracks;
}

ndAnimationKeyFramesTrack* ndAnimationSequence::AddTrack()
{
	ndList<ndAnimationKeyFramesTrack>::ndNode* const node = m_tracks.Append();
	return &node->GetInfo();
}

void ndAnimationSequence::CalculatePose(ndAnimationPose& output, ndFloat32 param) const
{
	if (output.GetCount())
	{
		ndInt32 index = 0;
		ndAnimKeyframe* const keyFrames = &output[0];
		for (ndList<ndAnimationKeyFramesTrack>::ndNode* srcNode = m_tracks.GetFirst(); srcNode; srcNode = srcNode->GetNext())
		{
			const ndAnimationKeyFramesTrack& track = srcNode->GetInfo();
			ndAnimKeyframe& keyFrame = keyFrames[index];
			track.InterpolatePosition(param, keyFrame.m_posit);
			track.InterpolateRotation(param, keyFrame.m_rotation);
			ndAssert(keyFrame.m_rotation.DotProduct(keyFrame.m_rotation).GetScalar() > 0.999f);
			ndAssert(keyFrame.m_rotation.DotProduct(keyFrame.m_rotation).GetScalar() < 1.001f);

			index++;
		}
	}
}
