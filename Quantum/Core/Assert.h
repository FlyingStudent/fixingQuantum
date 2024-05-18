#pragma once

#include "Quantum/Core/Base.h"
#include "Quantum/Core/Log.h"
#include <filesystem>

#ifdef QT_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define QT_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { QT##type##ERROR(msg, __VA_ARGS__); QT_DEBUGBREAK(); } }
#define QT_INTERNAL_ASSERT_WITH_MSG(type, check, ...) QT_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define QT_INTERNAL_ASSERT_NO_MSG(type, check) QT_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", QT_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define QT_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define QT_INTERNAL_ASSERT_GET_MACRO(...) QT_EXPAND_MACRO( QT_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, QT_INTERNAL_ASSERT_WITH_MSG, QT_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define QT_ASSERT(...) QT_EXPAND_MACRO( QT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define QT_CORE_ASSERT(...) QT_EXPAND_MACRO( QT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define QT_ASSERT(...)
#define QT_CORE_ASSERT(...)
#endif
