#include "hvn3/services/di_service_container.h"

namespace hvn3 {
	namespace services {

		ServiceContainer::size_type DIServiceContainer::size() const {

			return services.size();

		}

	}
}