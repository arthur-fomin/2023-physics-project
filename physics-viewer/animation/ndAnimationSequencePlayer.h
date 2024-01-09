#pragma once 
#include "ndAnimationBlendTreeNode.h"

class ndAnimPose;
class ndAnimationSequenceBase;

class ndAnimationSequencePlayer: public ndAnimationBlendTreeNode
{
	public:
	ndAnimationSequencePlayer(ndAnimationSequenceBase* const sequence);
	virtual ~ndAnimationSequencePlayer();

	virtual void Evaluate(ndAnimationPose& output);

	ndFloat32 GetParam() const;
	void SetParam(ndFloat32 param);
	ndAnimationSequenceBase* GetSequence();

	private:
	ndAnimationSequenceBase* m_sequence;
	ndFloat32 m_param;
};
