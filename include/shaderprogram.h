#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "rotator.h"
/// Adapted from http://stackoverflow.com/questions/2795044/easy-framework-for-opengl-shaders-in-c-c

class ShaderProgram {
public:
	/// Constructs a GLSL shader program with V/TC/TE/G/F-shaders located in the specified files
	ShaderProgram(std::string vertex_shader_filename = "",
		std::string tessellation_control_shader_filename = "",
		std::string tessellation_eval_shader_filename = "",
		std::string geometry_shader_filename = "",
		std::string fragment_shader_filename = "");

	static const std::string ReadFromFile(std::string fileName);

	/// Get the GLuint corresponding to the OpenGL shader program
	inline operator GLuint() {
		return prog;
	}

	/// Activate the shader program
	inline void operator()() {
		glUseProgram(prog);
    }
    
	static const std::string getShaderType(GLuint type);

	~ShaderProgram();

	void updateCommonUniforms(MouseRotator rotator, float width, float height, float time);

protected:
	GLuint AttachShader(GLuint shaderType, std::string source);

	void ConfigureShaderProgram();

private:
	std::vector<GLuint> shader_programs_;
	GLuint prog;

	GLuint compile(GLuint type, GLchar const *source);
};