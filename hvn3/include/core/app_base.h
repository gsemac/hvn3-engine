#pragma once

#include "core/iapp.h"

namespace hvn3::core {

	class AppBase :
		public IApp {

	public:
		void Run(const Window& window) override;

	protected:
		AppBase() = default;

		virtual void ConfigureServices(services::DIServiceContainer& services);

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