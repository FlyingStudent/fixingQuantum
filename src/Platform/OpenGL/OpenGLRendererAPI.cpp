#include"qtpch.h"
#include"OpenGLRendererAPI.h"

#include<glad/glad.h>
namespace Quantum {
void OpenGLMessageCallback(
unsigned source,
unsigned type,
unsigned id,
unsigned severity,
int length,
const char* message,
const void* userParam)
{
switch(severity)
{
case GL_DEBUG_SEVERITY_HIGH: QT_CORE_CRITICAL(message); return;
case GL_DEBUG_SEVERITY_MEDIUM: QT_CORE_ERROR(message); return;
case GL_DEBUG_SEVERITY_LOW: QT_CORE_WARN(message); return;
case GL_DEBUG_SEVERITY_NOTIFICATION: QT_CORE_TRACE(message);return;
}

QT_CORE_ASSERT(false, "Unknown severity level!");
}

	void OpenGLRendererAPI::Init()
	{
		QT_PROFILE_FUNCTION();
#ifdef QT_DEBUG
GLValidate(glEnable(GL_DEBUG_OUTPUT));
GLValidate(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
GLValidate(glDebugMessageCallback(OpenGLMessageCallback,nullptr));

GLValidate(glDebugMessageControl(GL_DONT_CARE,GL_DONT_CARE,GL_DEBUG_SEVERITY_NOTIFICATION,0,NULL,GL_FALSE));                  
#endif
		GLValidate(glEnable(GL_BLEND));
		GLValidate(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLValidate(glEnable(GL_DEPTH_TEST));
	}
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		GLValidate(glViewport(x, y, width, height));
	}
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		GLValidate(glClearColor(color.r, color.g, color.b, color.a));
	}
	void OpenGLRendererAPI::Clear()
	{
		GLValidate(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount: vertexArray->GetIndexBuffer()->GetCount();
		GLValidate(glDrawElements(GL_TRIANGLES,count, GL_UNSIGNED_INT, nullptr));
GLValidate(glBindTexture(GL_TEXTURE_2D,0));
	}
}
