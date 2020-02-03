#include "hvn3/core/Application.h" 
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

	// Public members

	Application::Application() :
		updateEventSource(DEFAULT_FPS) {

		Initialize(0, nullptr);

	}
	Application::Application(const ApplicationProperties& properties) :
		properties(properties),
		updateEventSource(DEFAULT_FPS) {

		Initialize(0, nullptr);

	}
	Application::Application(int argc, char* argv[]) :
		updateEventSource(DEFAULT_FPS) {

		Initialize(argc, argv);

	}
	Application::Application(int argc, char* argv[], const ApplicationProperties& properties) :
		properties(properties),
		updateEventSource(1.0 / properties.FrameRate) {

		Initialize(argc, argv);

	}

	const ApplicationProperties& Application::Properties() const {

		return properties;

	}
	const std::vector<std::string>& Application::CommandLineArguments() const {

		return commandLineArguments;

	}
	services::DIServiceContainer& Application::Services() {

		return services;

	}
	const services::DIServiceContainer& Application::Services() const {

		return services;

	}
	void Application::Run() {

		ConfigureServices();

		while (true) {

			auto& event_manager = services.GetService<EventManager>();

			event_manager.DoEvents(true);

			if (event_manager.RedrawRequired()) {

				auto& display_manager = services.GetService<DisplayManager>();

				Graphics::Graphics canvas = display_manager.GetDisplay().Canvas();

				canvas.Clear(properties.OutsideColor);

				auto& render_manager = services.GetService<RenderManager>();

				render_manager.Render(canvas);

				display_manager.RefreshAll();

			}

		}

	}

	// Private members

	void Application::Initialize(int argc, char* argv[]) {

		// Store all command line arguments passed in.

		if (argc > 0)
			commandLineArguments.reserve(argc);

		for (int i = 0; i < argc; ++i)
			commandLineArguments.push_back(std::string(argv[i]));

		// Register required managers (the user should be allowed to replace them later).
		BuildServices();

	}
	void Application::BuildServices() {

		if (!services.HasService<IEventManager>())
			services.AddService<IEventManager, EventManager>();

		if (!services.HasService<DisplayManager>())
			services.AddService<DisplayManager>();

		if (!services.HasService<scenes::SceneManager>())
			services.AddService<scenes::SceneManager>();

		if (!services.HasService<RenderManager>())
			services.AddService<RenderManager>();

		if (!services.HasService<ecs::EntityManager>())
			services.AddService<ecs::EntityManager>();

		if (!services.HasService<ecs::ComponentManager>())
			services.AddService<ecs::ComponentManager>();

	}
	void Application::ConfigureServices() {

		// Create the primary display if no display has been created yet.

		if (services.GetService<DisplayManager>().Count() <= 0)
			services.GetService<DisplayManager>().CreateDisplay(properties.DisplaySize, properties.ApplicationName, properties.DisplayFlags);

		services.GetService<RenderManager>().SetScalingMode(properties.ScalingMode);

		// Set up the event manager with basic event sources.

		updateEventSource.SetSecondsPerTick(1.0 / properties.FrameRate);
		updateEventSource.Start();

		services.GetService<EventManager>().RegisterEventSource(updateEventSource.EventSource());
		services.GetService<EventManager>().RegisterEventSource(io::IOUtils::KeyboardEventSource());
		services.GetService<EventManager>().RegisterEventSource(io::IOUtils::MouseEventSource());
		services.GetService<EventManager>().RegisterEventSource(services.GetService<DisplayManager>().GetDisplay().EventSource());

	}

}