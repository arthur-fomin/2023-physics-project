#pragma once 
class ndAnimationKeyFramesTrack
{
	public:
	template<class OBJECT>
	class dKeyFramesArray: public ndArray<OBJECT>
	{
		public:
		dKeyFramesArray()
			:ndArray<OBJECT>()
		{
		}
		ndInt32 GetIndex(ndFloat32 param) const;

		private:
		ndInt32 GetIndexDebug(ndFloat32 param) const;

		public:
		ndArray<ndFloat32> m_param;
	};

	ndAnimationKeyFramesTrack()
		:m_position()
		,m_rotation()
	{
	}

	~ndAnimationKeyFramesTrack()
	{
	}

	const ndString& GetName () const
	{
		return m_name;
	}

	void SetName (const ndString& name)
	{
		m_name = name; 
	}

	void InterpolatePosition(ndFloat32 param, ndVector &positOut) const;
	void InterpolateRotation(ndFloat32 param, ndQuaternion& rotationOut) const;

	ndString m_name;
	dKeyFramesArray<ndVector> m_position;
	dKeyFramesArray<ndQuaternion> m_rotation;
};

