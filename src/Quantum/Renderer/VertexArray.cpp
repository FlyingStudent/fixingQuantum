#include"qtpch.h"
#include"VertexArray.h"
#include"Renderer.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"
namespace Quantum {
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{

		case RendererAPI::API::None: QT_CORE_ASSERT(false, "Renderer API None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		}
		QT_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
		}
	}
