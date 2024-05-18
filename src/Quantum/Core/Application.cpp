#include "qtpch.h"
#include "Quantum/Core/Application.h"

#include "Quantum/Core/Log.h"

#include "Quantum/Renderer/Renderer.h"

#include "Quantum/Core/Input.h"

#include<GLFW/glfw3.h>
namespace Quantum {
Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, ApplicationCommandLineArgs args )
		:m_CommandLineArgs(args)
	{
		
		QT_PROFILE_FUNCTION();

		QT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(QT_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		
		
	}
	 Application::~Application() {
		 QT_PROFILE_FUNCTION();
		 Renderer::Shutdown();
	}
	void Application::Run()
	{
		QT_PROFILE_FUNCTION();
		while (m_Running)
		{
			QT_PROFILE_SCOPE("RunLoop");
				float time = (float)glfwGetTime();
				TimeStep timeStep = time - m_LastFrameTime;
				m_LastFrameTime = time;
		
				if (!m_Minimized)
				{
					{
						QT_PROFILE_SCOPE("LayerStack OnUpdate");
						for (Layer* layer : m_LayerStack)
						{
							layer->OnUpdate(timeStep);
						}
					}
					m_ImGuiLayer->Begin();
					{
						QT_PROFILE_SCOPE("LayerStack OnImGuiRender");
						for (Layer* layer : m_LayerStack)
						{
							layer->OnImGuiRender();
						}
					}
					m_ImGuiLayer->End();
				}
		m_Window->OnUpdate();
		 
		
		}
	}
	void Application::OnEvent(Event& e)
	{
		QT_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(QT_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(QT_BIND_EVENT_FN(Application::OnWindowResize));
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled) 
				break;
			(*it)->OnEvent(e);
		}





	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		QT_PROFILE_FUNCTION();
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		
		
		return false;
	}
	void Application::PushLayer(Layer* layer)
	{
		QT_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		QT_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
	void Application::Close()
	{
		m_Running = false;
	}
}
