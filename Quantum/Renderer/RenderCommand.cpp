#include"qtpch.h"
#include"RenderCommand.h"

namespace Quantum
{

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
