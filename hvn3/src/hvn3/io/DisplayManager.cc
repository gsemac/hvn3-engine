#include "hvn3/io/DisplayManager.h"

#include <cassert>

namespace hvn3 {

	// Public methods

	Display& DisplayManager::GetDisplay() {

		assert(_displays.size() > 0);

		return _displays[0];

	}

	size_t DisplayManager::Count() const {

		return _displays.size();

	}

}