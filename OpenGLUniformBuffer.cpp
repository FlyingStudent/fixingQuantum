#include<qtpch.h>
#include"OpenGLUniformBuffer.h"
#include<glad/glad.h>

namespace Quantum {
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
	{
		GLValidate(glCreateBuffers(1, &m_RendererID));
		GLValidate(glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW));//TODO : investigate usage hint
		GLValidate(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID));
	}
	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		GLValidate(glDeleteBuffers(1, &m_RendererID));
	}
	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		GLValidate(glNamedBufferSubData(m_RendererID, offset, size, data));
	}
}