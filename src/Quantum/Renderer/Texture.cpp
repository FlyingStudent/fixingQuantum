#include"qtpch.h"
#include"Texture.h"

#include"Renderer.h"
#include"Platform/OpenGL/OpenGLTexture.h"
namespace Quantum {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{

		case RendererAPI::API::None: QT_CORE_ASSERT(false, "Renderer API None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width,height);
		}
		QT_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
       Ref<Texture2D> Texture2D::Create(const std::string& path)
	{

		   switch (Renderer::GetAPI())
		   {

		   case RendererAPI::API::None: QT_CORE_ASSERT(false, "Renderer API None is currently not supported!");
			   return nullptr;
		   case RendererAPI::API::OpenGL:
			   return CreateRef<OpenGLTexture2D>(path);
		   }
		   QT_CORE_ASSERT(false, "Unkown RendererAPI!");
		   return nullptr;
	   }

	}
