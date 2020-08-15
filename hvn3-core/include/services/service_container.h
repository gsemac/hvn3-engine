#pragma once

#include <cassert>
#include <cstddef>
#include <map>
#include <memory>
#include <type_traits>
#include <typeindex>

namespace hvn3::services {

	class ServiceContainer final {

	public:
		using size_type = std::size_t;

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

		struct ServiceDescriptor {

			ServiceDescriptor(std::size_t index, const service_pointer_t& service);

			std::size_t index;
			service_pointer_t service;

		};

		std::multimap<std::type_index, ServiceDescriptor> services;

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

		services.emplace(std::make_pair(
			std::type_index(typeid(service_t)),
			ServiceDescriptor(services.size(), service)
		));

		// Add a pointer to the interface to the service container (if applicable).
		// This service descriptor will have an empty deleter to avoid deleting the service more than once.

		if (!std::is_same_v<interface_t, service_t>) {

			services.emplace(std::make_pair(
				std::type_index(typeid(interface_t)),
				ServiceDescriptor(services.size(), service_pointer_t(static_cast<interface_t*>(service.get()), [](void*) {}))
			));

		}

		return *this;

	}

	template <typename ServiceType>
	ServiceType& ServiceContainer::GetService() {

		using service_t = std::remove_reference_t<ServiceType>;

		assert(IsServiceRegistered<service_t>());

		auto it = services.find(typeid(service_t));

		return *static_cast<service_t*>(it->second.service.get());

	}
	template <typename ServiceType>
	const ServiceType& ServiceContainer::GetService() const {

		using service_t = std::remove_reference_t<ServiceType>;

		assert(IsServiceRegistered<service_t>());

		auto it = services.find(typeid(service_t));

		return *static_cast<service_t*>(it->second.service.get());

	}

	template<typename ServiceType>
	bool ServiceContainer::IsServiceRegistered() const {

		using service_t = std::remove_reference_t<ServiceType>;

		auto it = services.find(typeid(service_t));

		return it != services.end();

	}

}