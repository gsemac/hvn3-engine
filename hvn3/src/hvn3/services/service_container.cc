#include "hvn3/services/service_container.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

namespace hvn3::services {

	// Public members

	ServiceContainer::ServiceInfo::ServiceInfo(std::size_t index, service_pointer_type&& service) :
		index(index),
		service(std::move(service)) {
	}

	ServiceContainer::ServiceContainer() {
	}
	ServiceContainer::~ServiceContainer() {

		// Services need to be destructed in the reverse order that they were added.
		// There are no guarantees about this ordering for STL containers.

		// Move the services into a vector.

		std::vector<ServiceInfo> orderedServices;
		orderedServices.reserve(services.size());

		for (auto keyIt = services.begin(); keyIt != services.end(); ++keyIt) {

			auto valuesRange = services.equal_range(keyIt->first);

			for (auto valueIt = valuesRange.first; valueIt != valuesRange.second; ++valueIt)
				orderedServices.emplace_back(std::move(valueIt->second));

		}

		// Sort the vector in descending order according to the order of construction.

		std::sort(orderedServices.begin(), orderedServices.end(),
			[](const ServiceInfo& lhs, const ServiceInfo& rhs) {
				return lhs.index > rhs.index;
			}
		);

		// Free the services.

		for (auto i = orderedServices.begin(); i != orderedServices.end(); ++i)
			i->service.release();

	}

	ServiceContainer::size_type ServiceContainer::Count() const {

		return services.size();

	}

}