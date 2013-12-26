#pragma once

#include <string>
#include <fstream>
#include <memory>

#include "Object.hpp"

namespace gl
{
	template <class T = GLuint>
	class Shader : public Object<T>
	{
	public:
		inline Shader(GLenum type);
		inline Shader(GLenum type, const char* filename);
		~Shader();
		bool compile();
		void source(const char* filename);
		std::auto_ptr<GLchar> infoLog();
	};

	template <class T>
	Shader<T>::Shader(GLenum type)
		: Object<T>()
	{
		_id = glCreateShader(type);
	}

	template <class T>
	Shader<T>::Shader(GLenum type, const char* filename)
		: Shader(type)
	{
		source(filename);
	}

	template <class T>
	Shader<T>::~Shader()
	{

	}

	template <class T>
	bool Shader<T>::compile()
	{
		glCompileShader(_id);
		GLint status;
		glGetShaderiv(_id, GL_COMPILE_STATUS, &status);
		return status != GL_FALSE;
	}

	template <class T>
	void Shader<T>::source(const char* filename)
	{
		std::ifstream file(filename);
		std::string content(
			(std::istreambuf_iterator<char>(file)),
			(std::istreambuf_iterator<char>())
			);
		const char* rawContent = content.c_str();
		glShaderSource(_id, 1, &rawContent, nullptr);
	}

	template <class T>
	std::auto_ptr<GLchar> Shader<T>::infoLog()
	{
		GLint infoLength;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &infoLength);
		GLchar* logMsg = new GLchar[infoLength + 1];
		glGetShaderInfoLog(_id, infoLength, nullptr, logMsg);
		return std::auto_ptr<GLchar>(logMsg);
	}
}