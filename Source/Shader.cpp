#include <Classes/Shader.h>

Shader* Shader::currentShader = nullptr;

Shader::~Shader() {
}

GLint Shader::Attribute(const GLchar* name) {
	return glGetAttribLocation(program, name);
}

GLint Shader::Uniform(const GLchar * name) {
	return glGetUniformLocation(program, name);
}

