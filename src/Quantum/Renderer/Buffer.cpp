#include"qtpch.h"
#include"Buffer.h"
#include"Renderer.h"
#include"Platform/OpenGL/OpenGLBuffer.h"
namespace Quantum {


	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: QT_CORE_ASSERT(false, "Renderer API None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
		}
		QT_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}


	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: QT_CORE_ASSERT(false, "Renderer API None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL :
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		QT_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indecies, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: QT_CORE_ASSERT(false, "Renderer API None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indecies, size);
		}
		QT_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
}
