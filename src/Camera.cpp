#include "../inc/Camera.h"
static const double PI = 3.141592653589793238;

Camera::Camera(glm::vec3 p, float a1, float a2)
{
	mPosition = p;
	while (a1 < 0)
	{
		a1 += 360.0f;
	}
	while (a1 > 360.0f)
	{
		a1 -= 360.0f;
	}
	mPaw = a1;
	if (a2 < -88.0f)
	{
		a2 = -88.0f;
	}
	if (a2 > 88.0f)
	{
		a2 = 88.0f;
	}
	mPitch = a2;
}

glm::mat4x4 Camera::GetViewMatrix() const
{
	glm::mat4x4 mat;
	double theta = mPaw / 180.0 * PI;
	double phi = mPitch / 180.0 * PI;
	glm::vec3 front_z = glm::vec3(cos(theta) * cos(phi), sin(phi), sin(theta) * cos(phi));
	front_z = glm::normalize(front_z);
	glm::vec3 up_y = -(glm::dot(front_z, glm::vec3(0, 1, 0))) * front_z + glm::vec3(0, 1, 0);
	up_y = glm::normalize(up_y);
	glm::vec3 right_x = glm::cross(up_y, front_z);
	mat[0][0] = right_x.x; mat[0][1] = right_x.y; mat[0][2] = right_x.z;
	mat[1][0] = up_y.x; mat[1][1] = up_y.y; mat[1][2] = up_y.z;
	mat[2][0] = front_z.x; mat[2][1] = front_z.y; mat[2][2] = front_z.z;
	mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;
	mat[0][3] = -glm::dot(mPosition, right_x);
	mat[1][3] = -glm::dot(mPosition, up_y);
	mat[2][3] = -glm::dot(mPosition, front_z);
	return glm::transpose(mat);
}

glm::mat4x4 Camera::GetProjectMatrix(float ratio) const
{
	glm::mat4x4 mat(0.0f);
	mat[0][0] = 1.0f / (ratio * tan(mFieldView / 360.0f * PI));
	mat[1][1] = mat[0][0] * ratio;
	mat[2][2] = (mNear + mFar) / (mNear - mFar);
	mat[3][2] = 2 * mNear * mFar / (mNear - mFar);
	mat[2][3] = -1.0f;
	return mat;
}

void Camera::TurnLeftRight(float a)
{
	mPaw += a;
}

void Camera::TurnUpDown(float a)
{
	mPitch += a;
	if (mPitch > 88.0f)
	{
		mPitch = 88.0f;
	}
	if (mPitch < -88.0f)
	{
		mPitch = -88.0f;
	}
}

void Camera::Scale(float a)
{
	mFieldView += a;
	if (mFieldView > 120.0f)
	{
		mFieldView = 120.0f;
	}
	if (mFieldView < 10.0f)
	{
		mFieldView = 10.0f;
	}
}

void Camera::Update(unsigned int programID) const
{
	Shader::SetUniform(programID, "view", GetViewMatrix());
	Shader::SetUniform(programID, "project", GetProjectMatrix(mRatio));
	Shader::SetUniform(programID, "CameraPos", mPosition);
}

void Camera::GetXYZDirection(glm::vec3& x, glm::vec3& y, glm::vec3& z) const
{
	double theta = mPaw / 180.0 * PI;
	double phi = mPitch / 180.0 * PI;
	glm::vec3 front_z = glm::vec3(cos(theta) * cos(phi), sin(phi), sin(theta) * cos(phi));
	z = glm::normalize(front_z);
	glm::vec3 up_y = -(glm::dot(front_z, glm::vec3(0, 1, 0))) * front_z + glm::vec3(0, 1, 0);
	y = glm::normalize(up_y);
	glm::vec3 right_x = glm::cross(up_y, front_z);
	x = glm::normalize(right_x);
}

void Camera::DebugInfo() const
{
	std::string info;
}
