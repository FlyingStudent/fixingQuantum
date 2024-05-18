#pragma once
#include"Quantum/Core/Window.h"
#include"Quantum/Renderer/GraphicsContext.h"
#include<GLFW/glfw3.h>
namespace Quantum {

	class WindowsWindow :public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		
		virtual ~WindowsWindow() ;
		
		void OnUpdate() override;
		 unsigned int GetWidth() const override { return m_Data.width; }
		 unsigned int GetHeight() const override { return m_Data.height; }

		//Window attributes
		 void SetEventCallback(const EventCallbackFn& callback) override {
			m_Data.EventCallback = callback;
		}
		void SetVSync(bool enabled)override;
		bool IsVSync() const override ;
			virtual   void* GetNativeWindow() const  {
			return m_Window;
	}
	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;
		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
		
	};
}
