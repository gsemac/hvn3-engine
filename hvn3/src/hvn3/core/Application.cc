#include "hvn3/core/Application.h" 

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
	void Application::Run() {



	}


	void Application::_init(int argc, char* argv[]) {

		// Store all command line arguments passed in.

		if (argc > 0)
			_command_line_arguments.reserve(argc);

		for (int i = 0; i < argc; ++i)
			_command_line_arguments.push_back(std::string(argv[i]));

	}

}