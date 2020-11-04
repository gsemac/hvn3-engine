#pragma once

#include "core/display_options.h"
#include "services/di_service_container.h"

namespace hvn3::core {

	class IApp {

	public:
		virtual ~IApp() = default;

		virtual void Show(const DisplayOptions& displayOptions) = 0;
		virtual int Run() = 0;
		virtual int Run(const DisplayOptions& displayOptions) = 0;

	};

}