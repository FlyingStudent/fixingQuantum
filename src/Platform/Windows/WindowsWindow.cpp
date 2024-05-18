#include"qtpch.h"
#include"WindowsWindow.h"
#include"Quantum/Core/Input.h"
#include"Quantum/Events/ApplicationEvent.h"
#include"Quantum/Events/KeyEvent.h"
#include"Quantum/Events/MouseEvent.h"
#include"Platform/OpenGL/OpenGLContext.h"

#include"Quantum/Renderer/Renderer.h"
namespace Quantum {
	static uint8_t s_GLFWWindowCount = 0;
	static void  GLFWErrorCallback(int error, const char* description)
	{
		QT_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		QT_PROFILE_FUNCTION();
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		QT_PROFILE_FUNCTION();
		ShutDown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		QT_PROFILE_FUNCTION();
		m_Data.title = props.m_Title;
		m_Data.width = props.m_Width;
		m_Data.height = props.m_Height;
		QT_CORE_INFO("Creatig Window {0} ({1} , {2})", props.m_Title, props.m_Width, props.m_Height);

		if (s_GLFWWindowCount ==0)
		{
			//ToDo: glfwTerminate on system shutdown
			glfwSetErrorCallback([](int  error, const char* description) {
				QT_CORE_ERROR("Error: {0}, Description: {1}", error, description);
				});
			{
				QT_PROFILE_SCOPE("glfwInit");
				int success = glfwInit();
				QT_CORE_ASSERT(success, "Could not initialize GLFW!");
				glfwSetErrorCallback(GLFWErrorCallback);
			}
		}
		{
			QT_PROFILE_SCOPE("glfwCreateWindow");
#if defined(QT_DEBUG)
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
			m_Window = glfwCreateWindow((int)props.m_Width, (int)props.m_Height, m_Data.title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);


	//set glfw callbacks
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.width = width;
		data.height = height;
		WindowResizeEvent event(width, height);
		data.EventCallback(event);
		});
	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.EventCallback(event);
		});
	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent event(key, 0);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent event(key);
			data.EventCallback(event);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent event(key, 1);
			data.EventCallback(event);
			break;
		}
		}
		});
glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
{
WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

KeyTypedEvent event(keycode);
data.EventCallback(event);
});
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}

			}
		});
	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);

		});
	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}
	void WindowsWindow::ShutDown()
	{
		QT_PROFILE_FUNCTION();
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}
	void WindowsWindow::OnUpdate()
	{
		QT_PROFILE_FUNCTION();
		glfwPollEvents();
		m_Context->SwapBuffers();
		
	}
	void WindowsWindow::SetVSync(bool enabled)
	{
		QT_PROFILE_FUNCTION();
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;
	}
	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}
