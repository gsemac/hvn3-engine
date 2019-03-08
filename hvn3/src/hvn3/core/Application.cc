#include "hvn3/core/Application.h" 
#include "hvn3/io/DisplayManager.h"
#include "hvn3/rooms/RoomManager.h"

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
	ApplicationContext Application::Context() const {



	}
	void Application::Run() {

		// Register required managers.

		if (!_manager_registry.IsRegistered<DisplayManager>())
			_manager_registry.Register<DisplayManager>();

		if (!_manager_registry.IsRegistered<RoomManager>())
			_manager_registry.Register<RoomManager>();

		// Create the primary display if no display has been created yet.

		if (_manager_registry.GetManager<DisplayManager>()->Count() <= 0)
			_manager_registry.GetManager<DisplayManager>()->CreateDisplay(_properties.DisplaySize);



	}


	void Application::_init(int argc, char* argv[]) {

		// Store all command line arguments passed in.

		if (argc > 0)
			_command_line_arguments.reserve(argc);

		for (int i = 0; i < argc; ++i)
			_command_line_arguments.push_back(std::string(argv[i]));

	}

}