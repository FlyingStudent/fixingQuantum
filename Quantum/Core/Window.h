#pragma once
#include <sstream>

#include "Quantum/Core/Base.h"
#include "Quantum/Events/Event.h"
namespace Quantum {
	struct WindowProps {
		std::string m_Title;
		uint32_t  m_Width;
		uint32_t  m_Height;
		WindowProps(const std::string& title = "Quantum Engine",
			uint32_t width = 800,
			uint32_t height = 600)
			:m_Title(title),m_Width(width),m_Height(height){}
	};

//Interface representing a desktop system based Window
	class  Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() = default;
		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() const = 0;
		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}
