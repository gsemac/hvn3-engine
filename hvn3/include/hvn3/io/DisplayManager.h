#pragma once

#include "hvn3/core/ManagerBase.h"
#include "hvn3/io/Display.h"
#include "hvn3/io/IDisplayManager.h"

#include <cstddef>
#include <utility>
#include <vector>

namespace hvn3 {

	class DisplayManager :
		public ManagerBase<IDisplayManager> {

	public:
		template<typename ...Args>
		void CreateDisplay(Args&&... args) {

			_displays.emplace_back(std::forward<Args>(args)...);

		}

		// Returns the primary display.
		Display& GetDisplay();

		void RefreshAll();

		size_t Count() const;

	private:
		std::vector<Display> _displays;

	};

}