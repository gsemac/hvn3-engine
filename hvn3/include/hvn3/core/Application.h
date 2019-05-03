#pragma once

#include "hvn3/core/ApplicationProperties.h"
#include "hvn3/core/ApplicationContextProvider.h"
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/ManagerRegistry.h"
#include "hvn3/events/Timer.h"

#include <string>
#include <utility>
#include <vector>

namespace hvn3 {

	class ApplicationContext;

	class Application :
		public ApplicationContextProvider {

		friend class ApplicationContext;

	public:
		Application();
		Application(const ApplicationProperties& properties);
		Application(int argc, char* argv[]);
		Application(int argc, char* argv[], const ApplicationProperties& properties);

		ApplicationProperties& GetProperties();
		const std::vector<std::string>& CommandLineArguments() const;
		ApplicationContext Context();

		void Run();

	protected:
		template<typename ManagerType, typename ...Args>
		void RegisterManager(Args&&... args);

	private:
		ApplicationProperties _properties;
		std::vector<std::string> _command_line_arguments;

		ManagerRegistry _manager_registry;
		std::vector<IUpdatable*> _updatable_managers;
		std::vector<IDrawable*> _drawable_managers;

		Timer _update_event_source;

		void _init(int argc, char* argv[]);
		void _registerCoreManagers();
		void _setUpCoreManagers();

	};

	template<typename ManagerType, typename ...Args>
	void Application::RegisterManager(Args&&... args) {

		_manager_registry.Register<ManagerType>(std::forward<Args>(args)...);

		auto registered_manager = _manager_registry.GetManager<ManagerType>();

		ApplicationContext context = Context();

		IManager::StartEventArgs args(&context);

		registered_manager->OnStart(args);

		/*auto* event_manager = _manager_registry.GetManager<EventManager>();

		if (event_manager != nullptr) {

			event_manager->RegisterEventListener(registered_manager);

		}*/

	}

}