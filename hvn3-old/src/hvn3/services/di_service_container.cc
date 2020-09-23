#include "hvn3/services/di_service_container.h"

namespace hvn3::services {

	ServiceContainer::size_type DIServiceContainer::Count() const {

		return services.Count();

	}

}