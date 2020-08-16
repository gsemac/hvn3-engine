#pragma once

#include <cassert>
#include <cstddef>
#include <map>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <vector>

namespace hvn3::services {

	class ServiceContainer final {

	public:
		using size_type = std::size_t;

		~ServiceContainer();

		template<typename ServiceType>
		ServiceContainer& RegisterService(const std::shared_ptr<ServiceType>& service);
		template<typename InterfaceType, typename ServiceType>
		ServiceContainer& RegisterService(const std::shared_ptr<ServiceType>& service);

		template <typename ServiceType>
		ServiceType& GetService();
		template <typename ServiceType>
		const ServiceType& GetService() const;

		template<typename ServiceType>
		bool IsServiceRegistered() const;

		size_type Count() const;

	private:
		using void_deleter_t = void(*)(void*);
		using service_pointer_t = std::shared_ptr<void>;

		std::multimap<std::type_index, service_pointer_t> services;
		std::vector<service_pointer_t> orderedServices;

	};

	// Public members

	template<typename ServiceType>
	ServiceContainer& ServiceContainer::RegisterService(const std::shared_ptr<ServiceType>& service) {

		return RegisterService<ServiceType, ServiceType>(service);

	}
	template<typename InterfaceType, typename ServiceType>
	ServiceContainer& ServiceContainer::RegisterService(const std::shared_ptr<ServiceType>& service) {

		using interface_t = std::remove_reference_t<InterfaceType>;
		using service_t = std::remove_reference_t<ServiceType>;

		static_assert(std::is_same_v<interface_t, service_t> || std::is_base_of_v<interface_t, service_t>, "The service must implement the given interface.");

		// Add the service pointer to the service container.

		services.emplace(std::make_pair(std::type_index(typeid(service_t)), service));

		orderedServices.push_back(service);

		// If the service is implementing an interface, map the interface to the service as well.

		if (!std::is_same_v<interface_t, service_t>) {

			std::shared_ptr<interface_t> interfacePtr = service;

			services.emplace(std::make_pair(std::type_index(typeid(interface_t)), interfacePtr));

		}

		return *this;

	}

	template <typename ServiceType>
	ServiceType& ServiceContainer::GetService() {

		using service_t = std::remove_reference_t<ServiceType>;

		assert(IsServiceRegistered<service_t>());

		auto it = services.find(typeid(service_t));

		return *static_cast<service_t*>(it->second.get());

	}
	template <typename ServiceType>
	const ServiceType& ServiceContainer::GetService() const {

		using service_t = std::remove_reference_t<ServiceType>;

		assert(IsServiceRegistered<service_t>());

		auto it = services.find(typeid(service_t));

		return *static_cast<service_t*>(it->second.get());

	}

	template<typename ServiceType>
	bool ServiceContainer::IsServiceRegistered() const {

		using service_t = std::remove_reference_t<ServiceType>;

		auto it = services.find(typeid(service_t));

		return it != services.end();

	}

}