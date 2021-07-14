#include "core/service_provider.h"

namespace hvn3 {

	ServiceProvider::ServiceProvider() :
		currentId(0) {
	}

	ServiceProvider::~ServiceProvider() {

		// Services are likely to depend on one another. Therefore, it is safest to free them in the reverse order that they were added.
		// This allows services to reference the services they depend on in their destructors safely.

		services.clear();

		for (auto i = orderedServiceIds.begin(); i != orderedServiceIds.end(); ++i)
			idServiceMap.erase(idServiceMap.find(*i));

		idServiceMap.clear();
		orderedServiceIds.clear();

	}

	ServiceProvider::size_type ServiceProvider::Count() const {

		return services.size();

	}

}