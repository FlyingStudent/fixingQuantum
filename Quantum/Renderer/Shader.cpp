#include"qtpch.h"
#include"Shader.h"

#include"Renderer.h"
#include"Platform/OpenGL/OpenGLShader.h"
namespace Quantum {
	Ref<Shader> Shader::Create(const std::string& filepath)
	{

		switch (Renderer::GetAPI())
		{

		case RendererAPI::API::None: QT_CORE_ASSERT(false, "Renderer API None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(filepath);
		}
		QT_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& name,const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{

		case RendererAPI::API::None: QT_CORE_ASSERT(false, "Renderer API None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name,vertexSrc, fragmentSrc);
		}
		QT_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{

		QT_CORE_ASSERT(!Exists(name), "Shader already exists! ");
		m_Shader[name] = shader;
	}
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name,const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name,shader);
		return shader;

	}
	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shader.find(name) != m_Shader.end();
	}
	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		QT_CORE_ASSERT(Exists(name), "Shader not found! ");
		return m_Shader[name];
	}
}
