#include "qtpch.h"
#include "Quantum/Core/Window.h"

#ifdef QT_PLATFORM_WINDOWS
#include"Platform/Windows/WindowsWindow.h"
#endif
namespace Quantum
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef QT_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		QT_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}
