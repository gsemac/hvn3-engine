#include "hvn3/core/ApplicationContext.h"

namespace hvn3 {

	ApplicationContext::ApplicationContext() :
		_application(nullptr) {
	}
	ApplicationContext::ApplicationContext(Application* application) :
		_application(application) {

		assert(_application != nullptr);

	}
	const ApplicationProperties& ApplicationContext::ApplicationProperties() const {

		assert(_application != nullptr);

		return _application->Properties();

	}
	const std::vector<std::string>& ApplicationContext::CommandLineArguments() const {

		assert(_application != nullptr);

		return _application->CommandLineArguments();

	}
	ApplicationContext::operator bool() const {

		return _application != nullptr;

	}

}