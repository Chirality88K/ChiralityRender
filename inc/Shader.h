#ifndef SHADER_H
#define SHADER_H
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include "../glm/glm.hpp"
#include "../glm/gtc/type_ptr.hpp"

enum class ShaderType
{
	NONE,
	VERTEX_SHADER,
	FRAGMENT_SHADER
};

class Shader
{
public:
	Shader() { mShaderType = ShaderType::NONE; }
	Shader(ShaderType, const std::string &);
	void AttachShader(unsigned int programID) const;
	void DeleteShader() const;
	static void SetUniform(unsigned int, std::string name, glm::mat4x4 val);
	static void SetUniform(unsigned int, std::string name, glm::vec3 val);
	static void SetUniform(unsigned int, std::string name, glm::vec4 val);
	static void SetUniform(unsigned int, std::string name, float val);

private:
	unsigned int mShaderID = 0;
	ShaderType mShaderType;
};

#endif