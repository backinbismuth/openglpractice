#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // All that OpenGL header stuff

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID; // Used as a Program ID

	Shader(const char* vertexPath, const char* fragmentPath) { // Shader logic
		// First, get the source code from the shader paths (Using C filestreams!!)
		std::string vertexCode;
		std::string fragCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); // Double-checks exceptions
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// Open Files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			// Reading file content into streams with bit-shifts
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// Always close file handlers once done!!!
			vShaderFile.close();
			fShaderFile.close();

			// Convert stream of bits into string
			vertexCode = vShaderStream.str();
			fragCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "Sowwy, fwile not swuccessfwuwwy read :[ -- ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		};

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragCode.c_str();

		// Now that we have the shaders (in vShaderCode/fShaderCode), we need to compile them!

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// Compiling Vertex Shader

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "Vertex comp failed :[ -- ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};
		
		// Compiling Fragment Shader (Very similar)

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// Shader Program

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "skull emoji -- ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		};

		glDeleteShader(vertex); // Delete, as they are now linked!!
		glDeleteShader(fragment);

	};

	void use() { // Activates shader
		glUseProgram(ID);
	}
	void setBool(const std::string& name, bool value) const { // Setter functions for utility
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
};

#endif
