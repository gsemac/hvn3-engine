#pragma once

#include "core/window.h"
#include "services/di_service_container.h"

namespace hvn3::core {

	class IApplication {

	public:
		virtual ~IApplication() = default;

		virtual void Run(const Window& window) = 0;

	protected:
		virtual void ConfigureServices(services::DIServiceContainer& services) = 0;

	};

}