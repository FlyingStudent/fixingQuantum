#pragma once
#include "Quantum/Core/Base.h"
#include "Quantum/Core/Application.h"
#ifdef QT_PLATFORM_WINDOWS

extern Quantum::Application* Quantum::CreateApplication(ApplicationCommandLineArgs args);
int  main(int argc, char** argv)
{
	Quantum::Log::Init();

	QT_PROFILE_BEGIN_SESSION("StartUp", "QuantumProfile-Startup.json");
	auto app = Quantum::CreateApplication({argc,argv});
	QT_PROFILE_END_SESSION();

	QT_PROFILE_BEGIN_SESSION("Runtime", "QuantumProfile-Runtime.json");
	app->Run();
	QT_PROFILE_END_SESSION();
	
	QT_PROFILE_BEGIN_SESSION("Shutdown", "QuantumProfile-Shutdown.json");
	delete app;
	QT_PROFILE_END_SESSION();
	
}

#endif
