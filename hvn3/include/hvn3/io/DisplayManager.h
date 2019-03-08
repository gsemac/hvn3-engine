#pragma once

#include "hvn3/core/IManager.h"
#include "hvn3/io/Display.h"

#include <cstddef>
#include <utility>
#include <vector>

namespace hvn3 {

	class DisplayManager :
		public IManager {

	public:
		template<typename ...Args>
		void CreateDisplay(Args&&... args) {

			_displays.push_back(std::move(Display(std::forward<Args>(args)...)));

		}

		size_t Count() const {

			return _displays.size();

		}

	private:
		std::vector<Display> _displays;

	};

}