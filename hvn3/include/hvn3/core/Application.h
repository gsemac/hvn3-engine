#pragma once

#include "hvn3/core/ApplicationContext.h"
#include "hvn3/core/ApplicationProperties.h"
#include "hvn3/core/ManagerRegistry2.h"

#include <string>
#include <utility>
#include <vector>

namespace hvn3 {

	class Application {

		friend class ApplicationContext;

	public:
		Application();
		Application(const ApplicationProperties& properties);
		Application(int argc, char* argv[]);
		Application(int argc, char* argv[], const ApplicationProperties& properties);

		ApplicationProperties& GetProperties();
		const std::vector<std::string>& CommandLineArguments() const;
		ApplicationContext Context() const;

		template<typename ManagerType, typename ...Args>
		void RegisterManager(Args&&... args);

		void Run();

	private:
		ApplicationProperties _properties;
		std::vector<std::string> _command_line_arguments;
		ManagerRegistry2 _manager_registry;

		void _init(int argc, char* argv[]);

	};

	template<typename ManagerType, typename ...Args>
	void Application::RegisterManager(Args&&... args) {

		_manager_registry.Register<ManagerType>(std::forward<Args>(args)...);

	}

}