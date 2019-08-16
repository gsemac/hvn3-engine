#pragma once

#include "hvn3/core/Application.h"

#include <cassert>
#include <string>
#include <vector>

namespace hvn3 {

	class Application;

	class ApplicationContext {

	public:
		ApplicationContext();
		ApplicationContext(Application* application);

		const ApplicationProperties& ApplicationProperties() const;
		const std::vector<std::string>& CommandLineArguments() const;

		template<typename ManagerType, typename ...Args>
		void Register(Args&&... args);
		template<typename ManagerType>
		ManagerHandle<ManagerType> Get();
		template<typename ManagerType>
		bool IsRegistered() const;

		explicit operator bool() const;

	private:
		Application* _application;

	};

	template<typename ManagerType, typename ...Args>
	void ApplicationContext::Register(Args&&... args) {

		assert(_application != nullptr);

		_application->RegisterManager<ManagerType>(args...);

	}
	template<typename ManagerType>
	ManagerHandle<ManagerType> ApplicationContext::Get() {

		assert(_application != nullptr);

		ManagerHandle<ManagerType> handle(_application->_manager_registry.GetManager<ManagerType>());

		assert(static_cast<bool>(handle));

		return handle;

	}
	template<typename ManagerType>
	bool ApplicationContext::IsRegistered() const {

		assert(_application != nullptr);

		return _application->_manager_registry.IsRegistered<ManagerType>();

	}

}