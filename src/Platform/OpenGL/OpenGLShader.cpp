#include<qtpch.h>
#include"Platform/OpenGL/OpenGLShader.h"
#include<fstream>
#include<glad/glad.h>
#include<glm/gtc/type_ptr.hpp>
#include<shaderc/shaderc.hpp>
#include<spirv_cross/spirv_cross.hpp>
#include<spirv_cross/spirv_glsl.hpp>

#include"Quantum/Core/Timer.h"

namespace Quantum {
	namespace Utils {
		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;
			QT_CORE_ASSERT(false, "Unkown shader type!");
			return 0;
		}
		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			QT_CORE_ASSERT(false);
			return (shaderc_shader_kind)0;
		}
		static const char* GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			QT_CORE_ASSERT(false);
			return nullptr;
		}
		static const char* GetCacheDirectory()
		{
			//TODO: make sure the assets  directory is valid
			return "assets/cache/shader/opengl";
		}
		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}
		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER:return ".cached_opengl.frag";
			}
			QT_CORE_ASSERT(false);
			return "";
		}
		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER:return ".cached_vulkan.frag";
			}
			QT_CORE_ASSERT(false);
			return "";
		}
	static const bool IsAmdGpu()
	{
	GLValidate(	const char* vendor = (char*)glGetString(GL_VENDOR));
	return strstr(vendor, "ATI") != nullptr;
	}

	}
	OpenGLShader::OpenGLShader(const std::string& filepath)
		:m_FilePath(filepath)

	{
		QT_PROFILE_FUNCTION();

		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSources);
			if (Utils::IsAmdGpu())
			{
				CreateProgramForAmd();
			}
			else
			{
				CompileOrGetOpenGLBinaries();
				CreateProgram();
			}
			QT_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}
		//Texture name form filepath 
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind(".");
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}
		OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		QT_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		CompileOrGetVulkanBinaries(sources);
		if(Utils::IsAmdGpu())
		{
		CreateProgramForAmd();
		}
		else
		{
		CompileOrGetOpenGLBinaries();
		CreateProgram();
		}
		}
	OpenGLShader::~OpenGLShader()
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glDeleteProgram(m_RendererID));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		QT_PROFILE_FUNCTION();
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
size_t size = in.tellg();
if(size != -1){
	result.resize(size);
in.seekg(0,std::ios::beg);
in.read(&result[0],size);

}
else
{
	QT_CORE_ERROR("Could not open file '{0}'", filepath);
	}

		}
		else
		{
			QT_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		QT_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			QT_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			QT_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid Shader type Specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
QT_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);
			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;
	}
	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		

		GLValidate(GLuint program = glCreateProgram());

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));
			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
				if (in.is_open())
				{
					in.seekg(0, std::ios::end);
					auto size = in.tellg();
					in.seekg(0, std::ios::beg);
					auto& data = shaderData[stage];
					data.resize(size / sizeof(uint32_t));
					in.read((char*)data.data(), size);
				}
				else
				{
					shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
					if (module.GetCompilationStatus() != shaderc_compilation_status_success)
					{
						QT_CORE_ERROR(module.GetErrorMessage());
						QT_CORE_ASSERT(false);
					}
					shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				
					std::ofstream out(cachedPath, std::ios::out | std::ios::binary);//fix problem
					if (out.is_open())
					{
						auto& data = shaderData[stage];
						out.write((char*)data.data(), data.size() * sizeof(uint32_t));
						out.flush();
						out.close();
					}
				}
		}
		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{

		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;

		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();
		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_FilePath;//fix problem
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					QT_CORE_ERROR(module.GetErrorMessage());
					QT_CORE_ASSERT(false);
				}
				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);

				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();

				}
			}
		}
	} 
	void OpenGLShader::CreateProgram()
	{

		GLValidate(GLuint program = glCreateProgram());

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRV)
		{

			GLValidate(GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage)));//fix problem
 			GLValidate(glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t)));
			GLValidate(glSpecializeShader(shaderID, "main", 0, nullptr, nullptr));
			GLValidate(glAttachShader(program, shaderID));
		}


		//link our program
		glLinkProgram(program);

		GLint isLinked;
		GLValidate(glGetProgramiv(program, GL_LINK_STATUS, &isLinked));

		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			GLValidate(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));
			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GLValidate(glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data()));//fix problem
			QT_CORE_ERROR("Shader linking filed ({0}):\n{1}", m_FilePath, infoLog.data());
			glDeleteProgram(program);
			for (auto id : shaderIDs)
			{
				GLValidate(glDeleteShader(id));
			}
			for (auto id : shaderIDs)
			{
				GLValidate(glDetachShader(program, id));
				GLValidate(glDeleteShader(id));
			}
			m_RendererID = program;
		}
}
	static bool VerifyProgramLink(GLenum& program)
	{
		int isLinked = 0;
		GLValidate(glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked));
		if (isLinked == GL_FALSE)
		{
			int maxLength = 0;
			GLValidate(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));

			std::vector<char> infoLog(maxLength);
			GLValidate(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));

			GLValidate(glDeleteProgram(program));

			QT_CORE_ASSERT("{0}", infoLog.data());
			QT_CORE_ASSERT(false, "[OpenGL] Shader Link failure!");

			return false;
		}
		return true;
	}
	void OpenGLShader::CreateProgramForAmd()
	{
		GLValidate(GLuint program = glCreateProgram());

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();
		std::filesystem::path shaderFilePath = m_FilePath;
		std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + ".cached_opengl.pgr");
		std::ifstream in(cachedPath, std::ios::ate | std::ios::binary);

		if (in.is_open())
		{
			auto size = in.tellg();
			in.seekg(0);

			auto data = std::vector<char>(size);
			uint32_t format = 0;
			in.read((char*)&format, sizeof(uint32_t));
			in.read((char*)data.data(), size);
			GLValidate(glProgramBinary(program, format, data.data(), data.size()));

			bool linked = VerifyProgramLink(program);

			if (!linked)
				return;
		}
		else
		{
			std::array<uint32_t, 2> glShadersIDs;
			CompileOpenGLBinariesForAmd(program, glShadersIDs);
			GLValidate(glLinkProgram(program));

			bool linked = VerifyProgramLink(program);

			if (linked)
			{
				//Save program data
				GLint formats = 0;

				GLValidate(glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats));
				QT_CORE_ASSERT(formats > 0, "Driver does not support binary format");
				Utils::CreateCacheDirectoryIfNeeded();
				GLint length = 0;
				GLValidate(glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &length));
				auto shaderData = std::vector<char>(length);
				uint32_t format = 0;
				GLValidate(glGetProgramBinary(program, length, nullptr, &format, shaderData.data()));
				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					out.write((char*)&format, sizeof(uint32_t));
					out.write(shaderData.data(), shaderData.size());
					out.flush();
					out.close();
				}
			}
			for (auto& id : glShadersIDs) {
				GLValidate(glDetachShader(program, id));
			}
		}
		m_RendererID = program;
	}
	void OpenGLShader::CompileOpenGLBinariesForAmd(GLenum& program, std::array<uint32_t, 2>& glShadersIDs)
	{
		int glShaderIDIndex = 0;
		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			
			spirv_cross::CompilerGLSL glslCompiler(spirv);
			std::string source = glslCompiler.compile();

			uint32_t shader;

			GLValidate(shader = glCreateShader(stage));

			const GLchar* sourceCStr = source.c_str();
			GLValidate(glShaderSource(shader, 1, &sourceCStr, 0));

			GLValidate(glCompileShader(shader));

			int isCompiled = 0;;
			GLValidate(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
			if (isCompiled == GL_FALSE)
			{
				int maxLength = 0;
				GLValidate(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));

				std::vector<char> infoLog(maxLength);
				GLValidate(glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]));

				GLValidate(glDeleteShader(shader));

				QT_CORE_ERROR("{0}", infoLog.data());
				QT_CORE_ASSERT(false, "[OpenGL] shader compilation failure!");
				return;
			}
			GLValidate(glAttachShader(program, shader));
			glShadersIDs[glShaderIDIndex++] = shader;
		}
	}
	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		QT_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
		QT_CORE_TRACE("		{0} uniform buffers,", resources.uniform_buffers.size());
		QT_CORE_TRACE("		{0} resources", resources.sampled_images.size());

		QT_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			QT_CORE_TRACE("	   {0}",resource.name);
			QT_CORE_TRACE("	   Size = {0}",bufferSize);
			QT_CORE_TRACE("	   Binding = {0}",binding);
			QT_CORE_TRACE("	   Members = {0}",memberCount);
	
		}
	}
	void OpenGLShader::Bind() const
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glUseProgram(m_RendererID));
	}
	void OpenGLShader::UnBind() const
	{
		QT_PROFILE_FUNCTION();
		GLValidate(glUseProgram(0));
	}
	void OpenGLShader::SetInt(const std::string& name, int  value)
	{
		QT_PROFILE_FUNCTION();
		UploadUniformInt(name, value);
	}
	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);

	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
QT_PROFILE_FUNCTION();
		UploadUniformFloat(name, value);
	}
	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		QT_PROFILE_FUNCTION();

		UploadUniformFloat2(name, value);
	}
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
QT_PROFILE_FUNCTION();
		UploadUniformFloat3(name, value);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		QT_PROFILE_FUNCTION();
		UploadUniformFloat4(name, value);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		QT_PROFILE_FUNCTION();
		UploadUniformMat4(name, value);
	}
	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLValidate(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
		GLValidate(glUniform1i(location, value));
	}
	 void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		 GLValidate(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
		 GLValidate(glUniform1iv(location, count,values));
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLValidate(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
		GLValidate(glUniform1f(location, value));
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
	
		GLValidate(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
		GLValidate(glUniform2f(location, values.x, values.y));
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLValidate(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
		GLValidate(glUniform3f(location, values.x, values.y, values.z));
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLValidate(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
		GLValidate(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLValidate(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
		GLValidate(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLValidate(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
		GLValidate(glUniform4f(location, values.x, values.y, values.z, values.w));
	}
}
