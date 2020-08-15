#include "services/service_container.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

namespace hvn3::services {

	// Public members

	ServiceContainer::ServiceDescriptor::ServiceDescriptor(std::size_t index, const service_pointer_t& service) :
		index(index),
		service(service) {
	}

	ServiceContainer::size_type ServiceContainer::Count() const {

		return services.size();

	}

}