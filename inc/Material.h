#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>
#include "GL/glew.h"
#include "../glm/glm.hpp"
#include "Shader.h"


class PhysicsMaterial
{
public:
	PhysicsMaterial(std::string name, glm::vec3 diffuse, glm::vec3 fresnel, float r)
	{
		mName = name;
		mDiffuseAlbedo = diffuse;
		mFresnelR0 = fresnel;
		mRoughness = std::min(r, 1.0f);
		mRoughness = std::max(0.0f, r);
	}
	void SetMaterial(unsigned int programID) const
	{
		Shader::SetUniform(programID, mName + ".DiffuseAlbedo", mDiffuseAlbedo);
		Shader::SetUniform(programID, mName + ".FresnelR0", mFresnelR0);
		Shader::SetUniform(programID, mName + ".Roughness", mRoughness);
	}
private:
	std::string mName;
	glm::vec3 mDiffuseAlbedo;
	glm::vec3 mFresnelR0;
	float mRoughness;//range 0-1
};

class ADS_Material
{
public:
	ADS_Material(const std::string& name)
	{
		mName = name;
		mDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		mAmbient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		mSpecular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);;
		mShininess = 0.5f;
	}
	void SetMaterial(unsigned int programID) const
	{
		Shader::SetUniform(programID, mName + ".Diffuse", mDiffuse);
		Shader::SetUniform(programID, mName + ".Ambient", mAmbient);
		Shader::SetUniform(programID, mName + ".Specular", mSpecular);
		Shader::SetUniform(programID, mName + ".Shininess", mShininess);
	}
public:
	std::string mName;
	glm::vec4 mDiffuse;
	glm::vec4 mAmbient;
	glm::vec4 mSpecular;
	float mShininess;
};

#endif
