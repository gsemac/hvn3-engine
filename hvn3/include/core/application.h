#pragma once

#include "core/window.h"
#include "services/di_service_container.h"

#include <vector>

namespace hvn3::core {

	class Application {

	public:
		void Run(const Window& window);

	private:
		class WindowInfo {

		public:
			Window window;
			services::DIServiceContainer services;

			WindowInfo(const Window& window);

		};

		std::vector<WindowInfo> windows;

		void InitializeWindow(const Window& window);
		void Run();

	};

}