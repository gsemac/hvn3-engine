#include "hvn3/io/DisplayManager.h"

#include <cassert>

namespace hvn3 {

	// Public methods

	io::Window& DisplayManager::GetDisplay() {

		assert(_displays.size() > 0);

		return _displays[0];

	}

	void DisplayManager::RefreshAll() {

		for (auto i = _displays.begin(); i != _displays.end(); ++i)
			i->Refresh();

	}

	size_t DisplayManager::Count() const {

		return _displays.size();

	}

}