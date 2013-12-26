#pragma once

#include "Object.hpp"
#include <set>

namespace std
{
template <class T>
class vector;
}

namespace gl
{
	template <class T>
	class Program : public Object<T>
	{
	public:
		inline Program();
		inline ~Program();

		void attachShader(const T&);
		void detachShader(const T&);
		bool link();
	private:
		std::set<T> _shaders;
	};

	template <class T>
	Program<T>::Program()
		: Object<T>(), _shaders()
	{
		_id = glCreateProgram();
	}

	template <class T>
	Program<T>::~Program()
	{
		glDeleteProgram(_id);
	}

	template <class T>
	void Program<T>::attachShader(const T& shader)
	{
		auto ret = _shaders.insert(shader);
		if (ret.second)
		{
			glAttachShader(_id, shader);
		}
	}

	template <class T>
	void Program<T>::detachShader(const T& shader)
	{
		_shaders.erase(shader);
		glDetachShader(_id, shader);
	}

	template <class T>
	bool Program<T>::link()
	{
		glLinkProgram(_id);
		GLint status;
		glGetProgramiv(_id, GL_LINK_STATUS, &status);
		return status != GL_FALSE;
	}
}