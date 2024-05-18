#pragma once
#include"Quantum/Core/Base.h"
#include"Quantum/Events/Event.h"
#include"Quantum/Core/TimeStep.h"
namespace Quantum {
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const {
			return m_DebugName;
		}
	protected:
		std::string m_DebugName;
	};
}   
