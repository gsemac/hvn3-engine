#include "services/di_service_container.h"

namespace hvn3::services {

	DIServiceContainer::size_type DIServiceContainer::Count() const {

		return services.size();

	}

}