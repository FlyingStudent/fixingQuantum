#pragma once

#include "Quantum/Core/Base.h"

#include "Quantum/Core/Window.h"
#include "Quantum/Core/LayerStack.h"
#include "Quantum/Events/Event.h"
#include "Quantum/Events/ApplicationEvent.h"

#include "Quantum/Core/TimeStep.h"

#include "Quantum/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);
namespace Quantum {
	struct ApplicationCommandLineArgs {
		int Count = 0;
		char** Args = nullptr;
		const char* operator[](int index) const {
			QT_CORE_ASSERT(index < Count);
			return Args[index];
		}
		};
	class  Application
	{
	public:
		Application(const std::string& name = "Quantum Application",ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	    Window& GetWindow() const { return *m_Window; }
		void Close();
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	    static Application& Get() {
			return *s_Instance;
		}
		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		
	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true, m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:  

		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};
	// to be defined in client
	Application* CreateApplication(ApplicationCommandLineArgs args);
}

