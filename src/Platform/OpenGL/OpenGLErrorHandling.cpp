#include"qtpch.h"
#include"OpenGLErrorHandling.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
namespace Quantum {
	bool GLCheckError(const char* function, std::string file, int line)
	{
		bool flag = false;
		while (GLenum error = glGetError())
		{
			switch (error)
			{
			case GL_INVALID_ENUM:
				QT_CORE_ERROR("error: GL_INVALID_ENUM ");
				QT_CORE_TRACE("function: {0}, Line: {1}, File: {2}" ,function,line,file);
				QT_CORE_INFO("hint: an unacceptable value is specified for an enumerated argumentd");
				flag = true;
				break;
			case GL_INVALID_VALUE:
				QT_CORE_ERROR("error: GL_INVALID_VALUE");
				QT_CORE_TRACE("function: {0}, Line: {1}, File: {2}" ,function,line,file);
				QT_CORE_INFO("hint: a numeric argument is out of range");
				flag = true;
				break;
			case GL_INVALID_OPERATION:
				QT_CORE_ERROR("error: GL_INVALID_OPERATION");
				QT_CORE_TRACE("function: {0}, Line: {1}, File: {2}", function, line, file);
				QT_CORE_INFO("hint: the specified operation is not allowed in the current state");
				flag = true;
				break;
			case GL_STACK_OVERFLOW:
				QT_CORE_ERROR("error: GL_STACK_OVERFLOW");
				QT_CORE_TRACE("function: {0}, Line: {1}, File: {2}", function, line, file);
				QT_CORE_INFO("hint : this function would cause a stack overflow");
				flag = true;
				break;
			case GL_STACK_UNDERFLOW:
			QT_CORE_ERROR("error: GL_STACK_UNDERFLOW");
				QT_CORE_TRACE("function: {0}, Line: {1}, File: {2}", function, line, file);
				QT_CORE_INFO("hint : this function would cause a stack underflow");
				flag = true;
				break;
			case GL_OUT_OF_MEMORY:
				QT_CORE_ERROR("error: GL_OUT_OF_MEMORY ");
				QT_CORE_TRACE("function: {0}, Line: {1}, File: {2}", function, line, file);
				QT_CORE_INFO("hint: there is not enough memory left to execute the function");
				flag = true;
				break;
			default:
				QT_CORE_ERROR("error: GL_OUT_OF_MEMORY ");
				QT_CORE_TRACE("function: {0}, Line: {1}, File: {2}", function, line, file);
				QT_CORE_INFO("hint: invalid error code");
				flag = true;
			}
		}
		if (flag)
		{
			glfwTerminate();
		}
		return flag;
	}
	std::string RelativePath(const char* fullPath) {
		// Assuming __FILE__ macro is used to get the current file path
		std::string filePath(fullPath);
		// Find the position of the solution directory name "Quantum"
		size_t pos = filePath.find("Quantum");
		if (pos != std::string::npos) {
			// Extract the part of the path after "Quantum"
			std::string relativePath = filePath.substr(pos + 8); // +7 to skip "Quantum" and the path separatore
			return relativePath;
		}
		else {
			// If "Quantum" is not found, print the full path
			return filePath;
		}

	}
}