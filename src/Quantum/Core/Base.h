#pragma once
#include<memory>

#include"Quantum/Core/PlatformDetection.h"
#ifdef QT_DEBUG
#if defined(QT_PLATFORM_WINDOWS)
#define QT_DEBUGBREAK() __debugbreak()
#elif defined(QT_PLATFORM_LINUX)
#include <signal.h>
#define QT_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif

#define QT_ENABLE_ASSERTS
#else 
#define QT_DEBUGBREAK()
#endif 

#define QT_EXPAND_MACRO(x) x
#define QT_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define QT_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Quantum {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "Quantum/Core/Log.h"
#include "Quantum/Core/Assert.h"
