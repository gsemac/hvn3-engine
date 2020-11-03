#pragma once

#include "core/display_options.h"
#include "core/idisplay.h"

namespace hvn3::core {

	class IDisplayManager {

	public:
		virtual ~IDisplayManager() = default;

		virtual IDisplay& GetDisplay() = 0;
		virtual const IDisplay& GetDisplay() const = 0;
		virtual void SetDisplayOptions(const DisplayOptions& displayOptions) = 0;

	};

}