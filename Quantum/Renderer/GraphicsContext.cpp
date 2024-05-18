#include"qtpch.h"
#include"GraphicsContext.h"

#include "Quantum/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Quantum {
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    QT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		QT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
