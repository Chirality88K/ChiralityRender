#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Log.h"

#define THROWERROR(a,b) if(a==-1){LOG_ERROR("Fail to set uniform: "+b);\
throw("Fail to set uniform: " + b); }

using namespace std;

Shader::Shader(ShaderType type, const std::string &path) {
  mShaderType = type;
  string shadercode;
  stringstream shaderstream;
  ifstream shaderfile;
  shaderfile.exceptions(ifstream::failbit | ifstream::badbit);
  try {
    shaderfile.open(path.c_str());
    shaderstream << shaderfile.rdbuf();
    shaderfile.close();
    shadercode = shaderstream.str();
  } catch (ifstream::failure e) {
      LOG_ERROR("Cannot find the file of shaders");
  }
  unsigned int shaderID = 0;
  int success;
  char infoLog[512];
  switch (type) {
  case ShaderType::VERTEX_SHADER: {
    shaderID = glCreateShader(GL_VERTEX_SHADER);
    const char *sc = shadercode.c_str();
    glShaderSource(shaderID, 1, &sc, nullptr);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
      LOG_ERROR("Fail to create vertex shader\n"+std::string(infoLog));
      break;
    };
    LOG_INFO("Succeed to compile vertex shader");
    break;
  }
  case ShaderType::FRAGMENT_SHADER: {
    shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    const char *sc = shadercode.c_str();
    glShaderSource(shaderID, 1, &sc, nullptr);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
      LOG_ERROR("Fail to create fragment shader\n" + std::string(infoLog));
      break;
    };
    LOG_INFO("Succeed to compile fragment shader");
    break;
  }
  case ShaderType::NONE: {
    LOG_ERROR("The type of shader is none");
    break;
  }
  }
  mShaderID = shaderID;
}

void Shader::AttachShader(unsigned int programID) const{
    glAttachShader(programID,mShaderID);
    LOG_INFO("Succeed to attach shader");
}

void Shader::SetUniform(unsigned int programID, std::string name, glm::mat4x4 val)
{
    int location = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));
    THROWERROR(location, name);
}

void Shader::SetUniform(unsigned int programID, std::string name, glm::vec3 val)
{
    int location = glGetUniformLocation(programID, name.c_str());
    glUniform3f(location, val.x, val.y, val.z);
    THROWERROR(location, name);
}

void Shader::SetUniform(unsigned int programID, std::string name, glm::vec4 val)
{
    int location = glGetUniformLocation(programID, name.c_str());
    glUniform4f(location, val.x, val.y, val.z, val.w);
    THROWERROR(location, name);
}

void Shader::SetUniform(unsigned int programID, std::string name, float val)
{
    int location = glGetUniformLocation(programID, name.c_str());
    glUniform1f(location, val);
    THROWERROR(location, name);
}
