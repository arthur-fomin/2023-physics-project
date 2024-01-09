#pragma once 
class ndAnimationPose;

class ndAnimationBlendTreeNode: public ndClassAlloc
{
	public:
	ndAnimationBlendTreeNode(ndAnimationBlendTreeNode* const input);
	virtual ~ndAnimationBlendTreeNode();

	virtual void Evaluate(ndAnimationPose& output)
	{
		if (m_input) 
		{
			m_input->Evaluate(output);
		}
	}

	ndAnimationBlendTreeNode* m_input;
};
