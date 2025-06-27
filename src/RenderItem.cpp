#include "../inc/RenderItem.h"
#include <iostream>
using namespace std;
static const float PI = acos(-1);

void RenderItem::Draw(unsigned int programID) const
{

	Shader::SetUniform(programID, "model", mModelMatrix);
	Shader::SetUniform(programID, "model_T_inv", glm::transpose(glm::inverse(mModelMatrix)));
	mModel->Draw(programID);
}

RenderItem::RenderItem(std::shared_ptr<Model> model) noexcept
{
	mModel = model;
	double center[3];
	model->GetBox().GetCenter(center);
	glm::mat4x4 matrix = glm::translate(glm::mat4x4(1.0), glm::vec3(-center[0], -center[1], -center[2]));
	double ratio = (std::min)(1.0 / model->GetBox().GetWidth(), 1.0 / model->GetBox().GetHeight());
	ratio = (std::min)(ratio, 1.0 / model->GetBox().GetLength());
	mModelMatrix = glm::mat4x4(ratio * 10.0) * matrix;
	mModelMatrix = glm::rotate(mModelMatrix, PI / 2, glm::vec3(-1.0, 0.0, 0.0));
}

/*
void RenderItem::LinkShader(unsigned int programID,const std::vector<Shader> & ShaderArray)const{
  for (const Shader &it : ShaderArray) {
	it.AttachShader(programID);
  }
  glLinkProgram(programID);
  int success;
  char infoLog[512];
  glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}
}
*/