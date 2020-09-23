#pragma once

#include "hvn3/core/ApplicationProperties.h"
#include "hvn3/events/Timer.h"
#include "hvn3/services/di_service_container.h"

#include <string>
#include <utility>
#include <vector>

namespace hvn3 {

	class Application final {

	public:
		Application();
		Application(const ApplicationProperties& properties);
		Application(int argc, char* argv[]);
		Application(int argc, char* argv[], const ApplicationProperties& properties);

		const ApplicationProperties& Properties() const;
		const std::vector<std::string>& CommandLineArguments() const;
		services::DIServiceContainer& Services();
		const services::DIServiceContainer& Services() const;

		void Run();

	private:
		ApplicationProperties properties;
		std::vector<std::string> commandLineArguments;
		services::DIServiceContainer services;
		Timer updateEventSource;

		void Initialize(int argc, char* argv[]);
		void BuildServices();
		void ConfigureServices();

	};

}