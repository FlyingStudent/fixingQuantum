#include"qtpch.h"
#include"OpenGLBuffer.h"

#include<glad/glad.h>
namespace Quantum {

	 
	////////////////////////////////////////////////
	////VertexBuffer///////////////////////////////
	//////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glCreateBuffers(1, &m_RendererID));
		GLValidate(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLValidate(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glCreateBuffers(1, &m_RendererID));
		GLValidate(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLValidate(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glDeleteBuffers(1, &m_RendererID));
	}
	void OpenGLVertexBuffer::Bind() const
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glBindBuffer(GL_ARRAY_BUFFER,0));
	}

	void OpenGLVertexBuffer::SetData( void* data, uint32_t size)
	{
		GLValidate(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLValidate(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}

	////////////////////////////////////////////////
	////IndexBuffer///////////////////////////////
	//////////////////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indecies, uint32_t count)
		:m_Count(count)
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glCreateBuffers(1, &m_RendererID));
		GLValidate(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLValidate(glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indecies, GL_STATIC_DRAW));
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glDeleteBuffers(1, &m_RendererID));
	}
	void OpenGLIndexBuffer::Bind() const
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

}
