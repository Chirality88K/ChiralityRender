#ifndef LIGHT_H
#define LIGHT_H
#include "Shader.h"
#include "../glm/glm.hpp"
#include "Log.h"

class Light {
public:
  // A D S
	Light(const std::string& name,glm::vec4 color)
	{
		mName = name;
		mColor = color;
	}
	Light(const std::string& name)
	{
		mName = name;
		mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
  virtual void AddLight(unsigned int) const = 0;
  virtual std::string Type()const = 0;
  std::string GetName()const
  {
	  return mName;
  }
  virtual void DebugInfo()const = 0;
  glm::vec4 GetRGBA()const {
	  return mColor;
  }
  glm::vec3 GetRGB()const {
	  return glm::vec3(mColor.x, mColor.y, mColor.z);
  }

private:
	glm::vec4 mColor;
	std::string mName;
};

class PointLight :public Light {
public:
	// Position A D S
	PointLight(std::string name,glm::vec4 color, glm::vec3 position)
		:Light(name,color)
	{
		mPosition = position;
	}
	PointLight(std::string name,glm::vec3 position)
		:Light(name)
	{
		mPosition = position;
	}
	void AddLight(unsigned int programID) const override
	{
		Shader::SetUniform(programID, GetName() + ".Position", mPosition);
		Shader::SetUniform(programID, GetName() + ".Color", GetRGB());
		Shader::SetUniform(programID, GetName() + ".Constant", mConstant);
		Shader::SetUniform(programID, GetName() + ".Linear", mLinear);
		Shader::SetUniform(programID, GetName() + ".Quadratic", mQuadratic);
	}
	std::string Type()const override
	{
		return "POINT LIGHT";
	}
	void DebugInfo()const override
	{
		#ifdef _DEBUG
		std::string info;
		info = "Type of the light: POINT LIGHT\n";
		info += "Name of the light: " + GetName() + "\n";
		info += "Position of the light: " + std::to_string(mPosition.x) + " " +
			std::to_string(mPosition.y) + " " + std::to_string(mPosition.z) + "\n";
		glm::vec4 color = GetRGBA();
		info += "Color of the light: " + std::to_string(color.x) + " " +
			std::to_string(color.y) + " " + std::to_string(color.z)
			+ " " + std::to_string(color.w) + "\n";
		info += "Cover distance: " + std::to_string(mCoverDistance) + "\n";
		LOG_INFO(info);
		#endif
	}
private:
	glm::vec3 mPosition;
	float mCoverDistance = 50.0f;
	float mConstant = 1.0f;
	float mLinear = 0.09f;
	float mQuadratic = 0.032f;
};

class ParallelLight :public Light
{
public:
	ParallelLight(const std::string& name, glm::vec4 color, glm::vec3 direction)
		:Light(name,color)
	{
		mDirection = glm::normalize(direction);
	}
	ParallelLight(const std::string& name, glm::vec3 direction)
		:Light(name)
	{
		mDirection = glm::normalize(direction);
	}
	void AddLight(unsigned int programID) const override
	{
		Shader::SetUniform(programID, GetName() + ".Color", GetRGB());
		Shader::SetUniform(programID, GetName() + ".Direction", mDirection);
	}
	std::string Type()const override
	{
		return "PARALLEL LIGHT";
	}
	void DebugInfo()const override
	{
		#ifdef _DEBUG
		std::string info;
		info = "Type of the light: PARALLEL LIGHT\n";
		info += "Name of the light: " + GetName() + "\n";
		info += "Direction of the light: " + std::to_string(mDirection.x) + " " +
			std::to_string(mDirection.y) + " " + std::to_string(mDirection.z) + "\n";
		glm::vec4 color = GetRGBA();
		info += "Color of the light: " + std::to_string(color.x) + " " +
			std::to_string(color.y) + " " + std::to_string(color.z)
			+ " " + std::to_string(color.w);
		LOG_INFO(info);
		#endif
	}
private:
	glm::vec3 mDirection;
};


#endif // !LIGHT_H