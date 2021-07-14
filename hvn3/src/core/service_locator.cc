#include "core/service_locator.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

namespace hvn3 {

	// Public members

	ServiceLocator::~ServiceLocator() {

		// It's possible that services depend on one another. Therefore, it is safest to free them in the reverse order that they were added.

		services.clear();

		for (auto i = orderedServices.rbegin(); i != orderedServices.rend(); ++i)
			i->reset();

		orderedServices.clear();

	}

	ServiceLocator::size_type ServiceLocator::Count() const {

		return services.size();

	}

}