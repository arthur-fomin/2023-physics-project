#include <ndNewton.h>
#include "ndAnimationPose.h"
#include "ndAnimationSequenceBase.h"

ndAnimationSequenceBase::ndAnimationSequenceBase()
	:m_name("")
{
}

ndAnimationSequenceBase::~ndAnimationSequenceBase()
{
}

const ndString& ndAnimationSequenceBase::GetName() const
{
	return m_name;
}

void ndAnimationSequenceBase::SetName(const char* const name)
{
	m_name = name;
}

