#pragma once

#include "Object.hpp"

namespace gl
{
	template <class T = GLuint>
	class BufferObject : public Object<T>
	{
	public:
		BufferObject();
		BufferObject(GLenum usage, const GLvoid* data, int size);
		void upload(GLenum usage, const GLvoid* data, int size);
	};

	template <class T>
	BufferObject<T>::BufferObject()
		: Object<T>()
	{
		glGenBuffers(1, &_id);
	}

	template <class T>
	BufferObject<T>::BufferObject(GLenum usage, const GLvoid* data, int size)
		: BufferObject<T>()
	{
		upload(usage, data, size);
	}

	template <class T>
	void BufferObject<T>::upload(GLenum usage, const GLvoid* data, int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}