#pragma once

#include "core/iapplication.h"

#include <vector>

namespace hvn3::core {

	class Application :
		public IApplication {

	public:
		void Run(const Window& window) override;

	protected:
		void ConfigureServices(services::DIServiceContainer& services) override;

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