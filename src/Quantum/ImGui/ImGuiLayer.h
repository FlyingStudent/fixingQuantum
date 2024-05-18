#pragma once
#include "Quantum/Core/Layer.h"

#include "Quantum/Events/ApplicationEvent.h"
#include "Quantum/Events/KeyEvent.h"
#include "Quantum/Events/MouseEvent.h"
namespace Quantum {
	class  ImGuiLayer : public Layer
	{
	public:

		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override ;
		virtual void OnDetach() override ;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;

	};
}
