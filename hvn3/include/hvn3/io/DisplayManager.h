#pragma once

#include "hvn3/core/ManagerBase.h"
#include "hvn3/io/Display.h"

#include <cstddef>
#include <utility>
#include <vector>

namespace hvn3 {

	class DisplayManager :
		public ManagerBase {

	public:
		template<typename ...Args>
		void CreateDisplay(Args&&... args) {

			_displays.emplace_back(std::forward<Args>(args)...);

		}

		Display& GetDisplay() {

			assert(_displays.size() > 0);

			return _displays[0];

		}

		size_t Count() const {

			return _displays.size();

		}

	private:
		std::vector<Display> _displays;

	};

}