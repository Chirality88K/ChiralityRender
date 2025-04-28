#ifndef CAMERA_H
#define CAMERA_H
#include "GL/glew.h"
#include "../glm/glm.hpp"
#include "Shader.h"

class Camera
{
public:
	Camera(glm::vec3, float, float);
	glm::mat4x4 GetViewMatrix()const;
	glm::mat4x4 GetProjectMatrix(float)const;
	void TurnLeftRight(float);
	void TurnUpDown(float);
	void Move(const glm::vec3& v)
	{
		mPosition += v;
	}
	void Scale(float);
	void Update(unsigned int)const;
	void SetRaito(float r)
	{
		mRatio = r;
	}
	void GetXYZDirection(glm::vec3& x, glm::vec3& y, glm::vec3& z)const;
	void DebugInfo()const;
private:
	glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 1.0f);
	float mPitch = 0.0f;//high or low 
	float mPaw = 90.0f;//left or right
	float mFieldView = 45.0f;
	float mNear = 0.1f;
	float mFar = 100.0f;
	float mRatio = 1.0f;//width/height
};
#endif
