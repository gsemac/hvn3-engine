#pragma once

#include "core/window.h"
#include "services/di_service_container.h"

namespace hvn3::core {

	class IApp {

	public:
		virtual ~IApp() = default;

		virtual void Run(const Window& window) = 0;

	};

}