#pragma once

#include "io/display_options.h"
#include "io/idisplay.h"

namespace hvn3::io {

	class IDisplayManager {

	public:
		virtual ~IDisplayManager() = default;

		virtual IDisplay& GetDisplay() = 0;
		virtual const IDisplay& GetDisplay() const = 0;
		virtual void SetDisplayOptions(const DisplayOptions& displayOptions) = 0;

	};

}