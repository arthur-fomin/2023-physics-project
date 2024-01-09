
#pragma once

class ndAnimKeyframe
{
	public:
	ndAnimKeyframe()
		:m_posit(ndVector::m_wOne)
		,m_rotation()
		,m_userData(nullptr)
	{
	}

	ndAnimKeyframe(const ndVector& posit, const ndQuaternion& rotation)
		:m_posit(posit)
		,m_rotation(rotation)
		,m_userData(nullptr)
	{
	}

	ndVector m_posit;
	ndQuaternion m_rotation;
	void* m_userData;
};

class ndAnimationPose: public ndArray<ndAnimKeyframe>
{
	public:
	ndAnimationPose();
	ndAnimationPose(const ndAnimationPose& source);

	void Clear();
	void CopySource(const ndAnimationPose& source);
};

class ndAnimationLocalPose: public ndAnimationPose
{
	public:
	ndAnimationLocalPose(ndAnimKeyframe* const buffer)
		:ndAnimationPose()
	{
		m_array = buffer;
		m_capacity = 0x7fffffff;
	}

	~ndAnimationLocalPose()
	{
		m_capacity = 0;
		m_array = nullptr;
	}
};

