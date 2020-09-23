#pragma once

#include "graphics/window.h"
#include "services/di_service_container.h"

#include <vector>

namespace hvn3 {

	class Application {

	public:
		void Run(const graphics::Window& window);

	private:
		class WindowInfo {

		public:
			graphics::Window window;
			services::DIServiceContainer services;

			WindowInfo(const graphics::Window& window);

		};

		std::vector<WindowInfo> windows;

		void InitializeWindow(const graphics::Window& window);
		void Run();

	};

}