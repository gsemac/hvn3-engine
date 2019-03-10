#include "hvn3/core/Application.h" 
#include "hvn3/core/ApplicationContext.h"
#include "hvn3/io/DisplayManager.h"
#include "hvn3/rooms/SceneManager.h"

namespace hvn3 {

	Application::Application() {

		_init(0, nullptr);

	}
	Application::Application(const ApplicationProperties& properties) :
		_properties(properties) {

		_init(0, nullptr);

	}
	Application::Application(int argc, char* argv[]) {

		_init(argc, argv);

	}
	Application::Application(int argc, char* argv[], const ApplicationProperties& properties) :
		_properties(properties) {

		_init(argc, argv);

	}

	ApplicationProperties& Application::GetProperties() {

		return _properties;

	}
	const std::vector<std::string>& Application::CommandLineArguments() const {

		return _command_line_arguments;

	}
	ApplicationContext Application::Context() {

		return ApplicationContext(this);

	}
	void Application::Run() {

		// Create the primary display if no display has been created yet.

		if (_manager_registry.GetManager<DisplayManager>()->Count() <= 0)
			_manager_registry.GetManager<DisplayManager>()->CreateDisplay(_properties.DisplaySize, _properties.ApplicationName);

		while (true) {

			UpdateEventArgs e(0.0001);
			DrawEventArgs draw_args(Graphics::Graphics(_manager_registry.GetManager<DisplayManager>()->GetDisplay().BackBuffer()));


			_manager_registry.GetManager<ISceneManager>()->OnUpdate(e);
			_manager_registry.GetManager<ISceneManager>()->OnDraw(draw_args);

			_manager_registry.GetManager<DisplayManager>()->GetDisplay().Refresh();

		}

	}


	void Application::_init(int argc, char* argv[]) {

		// Store all command line arguments passed in.

		if (argc > 0)
			_command_line_arguments.reserve(argc);

		for (int i = 0; i < argc; ++i)
			_command_line_arguments.push_back(std::string(argv[i]));

		// Register required managers (the user should be allowed to replace them later).
		_registerCoreManagers();

	}
	void Application::_registerCoreManagers() {

		if (!_manager_registry.IsRegistered<DisplayManager>())
			RegisterManager<DisplayManager>();

		if (!_manager_registry.IsRegistered<SceneManager>())
			RegisterManager<SceneManager>();

	}

}