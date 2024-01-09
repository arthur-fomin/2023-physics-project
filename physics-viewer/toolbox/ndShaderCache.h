#pragma once
#include "ndOpenGlUtil.h"

class ndShaderCache
{
	public:
	ndShaderCache();
	~ndShaderCache();

	void Cleanup();
	bool CreateAllEffects();
	
	private:
	void LoadShaderCode (const char* const name, char* const buffer);
	GLuint CreateShaderEffect (const char* const vertexShader, const char* const pixelShader, const char* const geometryShader = nullptr);

	public:
	union
	{
		struct
		{
			GLuint m_skyBox;
			GLuint m_wireFrame;
			GLuint m_flatShaded;
			GLuint m_thickPoints;
			GLuint m_texturedDecal;
			GLuint m_diffuseEffect;
			GLuint m_spriteSpheres;
			GLuint m_diffuseDebrisEffect;
			GLuint m_diffuseIntanceEffect;
			GLuint m_skinningDiffuseEffect;
		};
		GLuint m_shaders[128];
	};
};


