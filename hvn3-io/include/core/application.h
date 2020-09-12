#pragma once

#include "io/window.h"
#include "services/di_service_container.h"

#include <vector>

namespace hvn3 {

	class Application {

	public:
		void Run(const io::Window& window);

	private:
		class WindowInfo {

		public:
			io::Window window;
			services::DIServiceContainer services;

			WindowInfo(const io::Window& window);

		};

		std::vector<WindowInfo> windows;

		void InitializeWindow(const io::Window& window);

	};

}