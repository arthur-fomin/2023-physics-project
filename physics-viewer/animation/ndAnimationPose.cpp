#include <ndNewton.h>
#include "ndAnimationPose.h"

ndAnimationPose::ndAnimationPose()
	:ndArray<ndAnimKeyframe>()
{
}

//ndAnimationPose::ndAnimationPose(const ndAnimationPose& source)
ndAnimationPose::ndAnimationPose(const ndAnimationPose&)
	:ndArray<ndAnimKeyframe>()
{
	ndAssert(0);
}

void ndAnimationPose::Clear() 
{ 
//	RemoveAll(); 
}

//void ndAnimationPose::CopySource(const ndAnimationPose& source)
void ndAnimationPose::CopySource(const ndAnimationPose&)
{
	ndAssert(0);
//	dListNode* destNode = GetFirst();
//	for (dListNode* sourceNode = source.GetFirst(); sourceNode; sourceNode = sourceNode->GetNext()) {
//		const ndAnimKeyframe& srcFrame = sourceNode->GetInfo();
//		ndAnimKeyframe& dstFrame = destNode->GetInfo();
//		ndAssert(srcFrame.m_userData == dstFrame.m_userData);
//		dstFrame.m_rotation = srcFrame.m_rotation;
//		dstFrame.m_posit = srcFrame.m_posit;
//		destNode = destNode->GetNext();
//	}
}

