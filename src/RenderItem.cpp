#include "RenderItem.h"
#include <iostream>
using namespace std;

void RenderItem::Draw(unsigned int programID) const {
  
  Shader::SetUniform(programID, "model", mModelMatrix);
  Shader::SetUniform(programID, "model_T_inv", glm::transpose(glm::inverse(mModelMatrix)));
  mModel->Draw(programID);
}

RenderItem::RenderItem(std::shared_ptr<Model> model, const glm::mat4x4& matrix)
{
    mModel = model;
    mModelMatrix = matrix;
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