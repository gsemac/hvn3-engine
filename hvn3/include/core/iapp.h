#pragma once

#include "io/display_options.h"
#include "service_provider.h"

namespace hvn3 {

	class IApp {

	public:
		virtual ~IApp() = default;

		virtual void Show(const io::DisplayOptions& displayOptions) = 0;

		virtual int Run(const io::DisplayOptions& displayOptions) = 0;
		virtual int Run() = 0;

	};

}