#pragma once
namespace Quantum {
#define ASSERT(x) if (x) __debugbreak();
#define GLValidate(x) while(glGetError());\
x;\
ASSERT(GLCheckError(#x,RelativePath(__FILE__),__LINE__))//1-error 
	//fncs
	bool GLCheckError(const char* function, std::string file, int line);
	std::string RelativePath(const char* fullPath);
}