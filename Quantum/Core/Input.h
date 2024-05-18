#pragma once
#include <glm/glm.hpp>
#include "Quantum/Core/KeyCodes.h"
#include "Quantum/Core/MouseButtonsCode.h"
namespace Quantum {
	class  Input {
	public :
			static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float  GetMouseY();
	};
}
