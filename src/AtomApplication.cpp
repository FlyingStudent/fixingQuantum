#include<Quantum.h>
#include<Quantum/Core/EntryPoint.h>
#include"EditorLayer.h"
namespace Quantum {
	class Atom : public Application
	{
	public:
		Atom(ApplicationCommandLineArgs args)
			:Application("Atom", args)
		{
			PushLayer(new EditorLayer());
		}
		~Atom()	
		{

		}
	};

	//entry point function implementation
 Application* CreateApplication(ApplicationCommandLineArgs args)
	{

		return new Atom(args);
	}
}
