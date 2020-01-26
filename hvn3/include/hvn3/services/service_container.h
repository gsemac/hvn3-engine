#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <map>

namespace hvn3 {
	namespace services {

		class ServiceContainer final {

		public:
			using size_type = std::size_t;

			ServiceContainer();
			~ServiceContainer();

			template<typename ServiceType, typename ...Args>
			ServiceContainer& AddService(Args&&... args);
			template<typename InterfaceType, typename ServiceType, typename ...Args>
			ServiceContainer& AddService(Args&&... args);

			template <typename ServiceType>
			ServiceType& GetService();
			template <typename ServiceType>
			const ServiceType& GetService() const;

			template<typename ServiceType>
			bool HasService() const;

			size_type size() const;

		private:
			using void_deleter_type = void(*)(void*);
			using service_pointer_type = std::unique_ptr<void, void_deleter_type>;

			struct ServiceInfo {

				ServiceInfo(std::size_t index, service_pointer_type&& service);

				std::size_t index;
				service_pointer_type service;

			};

			std::multimap<std::type_index, ServiceInfo> services;

		};

		// Public members

		template<typename ServiceType, typename ...Args>
		ServiceContainer& ServiceContainer::AddService(Args&&... args) {

			return AddService<ServiceType, ServiceType, Args...>(std::forward<Args>(args)...);

		}
		template<typename InterfaceType, typename ServiceType, typename ...Args>
		ServiceContainer& ServiceContainer::AddService(Args&&... args) {

			using interface_type = std::remove_reference_t<InterfaceType>;
			using service_type = std::remove_reference_t<ServiceType>;

			static_assert(std::is_same_v<interface_type, service_type> || std::is_base_of_v<interface_type, service_type>,
				"Service must implement the given interface");

			static_assert(std::is_constructible_v<service_type, Args &&...>,
				"Service cannot be constructed from the given arguments");

			auto deleter = [](void* ptr) {
				std::default_delete<service_type>()(static_cast<service_type*>(ptr));
			};

			service_type* servicePtr = new service_type(std::forward<Args>(args)...);

			// Map the service type to the service object.

			services.emplace(std::make_pair(
				std::type_index(typeid(service_type)),
				ServiceInfo(services.size(), service_pointer_type(servicePtr, deleter))
			));

			if (!std::is_same_v<InterfaceType, ServiceType>) {

				// Map the interface to the service object.
				// We supply an empty deleter so that we don't delete the service twice.

				services.emplace(std::make_pair(
					std::type_index(typeid(interface_type)),
					ServiceInfo(services.size(), service_pointer_type(static_cast<interface_type*>(servicePtr), [](void*) {}))
				));

			}

			return *this;

		}

		template <typename ServiceType>
		ServiceType& ServiceContainer::GetService() {

			using service_type = std::remove_reference_t<ServiceType>;

			assert(HasService<service_type>());

			auto it = services.find(typeid(service_type));

			return *static_cast<service_type*>(it->second.service.get());

		}
		template <typename ServiceType>
		const ServiceType& ServiceContainer::GetService() const {

			using service_type = std::remove_reference_t<ServiceType>;

			assert(HasService<service_type>());

			auto it = services.find(typeid(service_type));

			return *static_cast<service_type*>(it->second.service.get());

		}

		template<typename ServiceType>
		bool ServiceContainer::HasService() const {

			using service_type = std::remove_reference_t<ServiceType>;

			auto it = services.find(typeid(service_type));

			return it != services.end();

		}

	}
}