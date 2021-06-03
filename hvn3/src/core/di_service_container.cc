#include "core/di_service_container.h"

namespace hvn3::core {

	DIServiceContainer::DIServiceContainer() :
		currentId(0) {
	}

	DIServiceContainer::~DIServiceContainer() {

		// Services are likely to depend on one another. Therefore, it is safest to free them in the reverse order that they were added.
		// This allows services to reference the services they depend on in their destructors safely.

		services.clear();

		for (auto i = orderedServiceIds.begin(); i != orderedServiceIds.end(); ++i)
			idServiceMap.erase(idServiceMap.find(*i));

		idServiceMap.clear();
		orderedServiceIds.clear();

	}

	DIServiceContainer::size_type DIServiceContainer::Count() const {

		return services.size();

	}

}