#include "ndNewton.h"
#include "ndAnimationPose.h"
#include "ndAnimationSequenceBase.h"
#include "ndAnimationSequencePlayer.h"

ndAnimationSequencePlayer::ndAnimationSequencePlayer(ndAnimationSequenceBase* const sequence)
	:ndAnimationBlendTreeNode(nullptr)
	,m_sequence(sequence)
	,m_param(ndFloat32 (0.0f))
{
}

ndAnimationSequencePlayer::~ndAnimationSequencePlayer()
{
}

ndFloat32 ndAnimationSequencePlayer::GetParam() const
{
	return m_param;
}

void ndAnimationSequencePlayer::SetParam(ndFloat32 param)
{
	m_param = ndMod(param, ndFloat32 (1.0f));
}

ndAnimationSequenceBase* ndAnimationSequencePlayer::GetSequence()
{
	return m_sequence;
}

void ndAnimationSequencePlayer::Evaluate(ndAnimationPose& output)
{
	m_sequence->CalculatePose(output, m_param);
}


