#include "hvn3/core/Application.h" 
#include "hvn3/core/ApplicationContext.h"
#include "hvn3/ecs/ComponentManager.h"
#include "hvn3/ecs/EntityManager.h"
#include "hvn3/events/EventManager.h"
#include "hvn3/graphics/RenderManager.h"
#include "hvn3/io/DisplayManager.h"
#include "hvn3/io/IOUtils.h"
#include "hvn3/rooms/SceneManager.h"
#include "hvn3/views/IViewManager.h"

constexpr double DEFAULT_FPS = 1.0 / 60.0;

namespace hvn3 {

	Application::Application() :
		_update_event_source(DEFAULT_FPS) {

		_init(0, nullptr);

	}
	Application::Application(const ApplicationProperties& properties) :
		_properties(properties),
		_update_event_source(DEFAULT_FPS) {

		_init(0, nullptr);

	}
	Application::Application(int argc, char* argv[]) :
		_update_event_source(DEFAULT_FPS) {

		_init(argc, argv);

	}
	Application::Application(int argc, char* argv[], const ApplicationProperties& properties) :
		_properties(properties),
		_update_event_source(1.0 / properties.FrameRate) {

		_init(argc, argv);

	}

	const ApplicationProperties& Application::Properties() const {

		return _properties;

	}
	const std::vector<std::string>& Application::CommandLineArguments() const {

		return _command_line_arguments;

	}
	ApplicationContext Application::Context() {

		return ApplicationContext(this);

	}
	void Application::Run() {

		_setUpCoreManagers();

		while (true) {

			auto event_manager = _manager_registry.GetManager<EventManager>();

			event_manager->DoEvents(Context(), true);

			if (event_manager->RedrawRequired()) {

				auto display_manager = _manager_registry.GetManager<DisplayManager>();

				Graphics::Graphics canvas = display_manager->GetDisplay().Canvas();

				canvas.Clear(_properties.OutsideColor);

				auto render_manager = _manager_registry.GetManager<RenderManager>();

				render_manager->Render(Context(), canvas);

				display_manager->RefreshAll();

			}

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

		if (!_manager_registry.IsRegistered<EventManager>())
			RegisterManager<EventManager>();

		if (!_manager_registry.IsRegistered<DisplayManager>())
			RegisterManager<DisplayManager>();

		if (!_manager_registry.IsRegistered<SceneManager>())
			RegisterManager<SceneManager>();

		if (!_manager_registry.IsRegistered<RenderManager>())
			RegisterManager<RenderManager>();

		if (!_manager_registry.IsRegistered<ecs::EntityManager>())
			RegisterManager<ecs::EntityManager>();

		if (!_manager_registry.IsRegistered<ecs::ComponentManager>())
			RegisterManager<ecs::ComponentManager>();

	}
	void Application::_setUpCoreManagers() {

		// Create the primary display if no display has been created yet.

		if (_manager_registry.GetManager<DisplayManager>()->Count() <= 0)
			_manager_registry.GetManager<DisplayManager>()->CreateDisplay(_properties.DisplaySize, _properties.ApplicationName, _properties.DisplayFlags);

		_manager_registry.GetManager<RenderManager>()->SetScalingMode(_properties.ScalingMode);

		// Set up the event manager with basic event sources.

		_update_event_source.SetSecondsPerTick(1.0 / _properties.FrameRate);
		_update_event_source.Start();

		_manager_registry.GetManager<EventManager>()->RegisterEventSource(_update_event_source.EventSource());
		_manager_registry.GetManager<EventManager>()->RegisterEventSource(io::IOUtils::KeyboardEventSource());
		_manager_registry.GetManager<EventManager>()->RegisterEventSource(io::IOUtils::MouseEventSource());
		_manager_registry.GetManager<EventManager>()->RegisterEventSource(_manager_registry.GetManager<DisplayManager>()->GetDisplay().EventSource());

	}

}