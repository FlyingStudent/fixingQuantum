#include<qtpch.h>
#include"Framebuffer.h"
#include"Quantum/Renderer/Renderer.h"
#include"Platform/OpenGL/OpenGLFramebuffer.h"

namespace Quantum {
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: QT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}
		QT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
