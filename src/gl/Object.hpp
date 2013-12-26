#pragma once

#include <GL3/gl3w.h>

namespace gl
{
	template <class T = GLuint>
	class Object
	{
	public:
		inline const T& identifier() const;
	protected:
		T _id;
	};

	template <class T>
	inline const T& Object<T>::identifier() const
	{
		return _id;
	}
}