#pragma once

#include "hvn3/core/ApplicationProperties.h"

#include <string>
#include <vector>

namespace hvn3 {

	class Application {

	public:
		Application();
		Application(const ApplicationProperties& properties);
		Application(int argc, char* argv[]);
		Application(int argc, char* argv[], const ApplicationProperties& properties);

		ApplicationProperties& GetProperties();
		const std::vector<std::string>& CommandLineArguments() const;

		void Run();

	private:
		ApplicationProperties _properties;
		std::vector<std::string> _command_line_arguments;

		void _init(int argc, char* argv[]);

	};

}