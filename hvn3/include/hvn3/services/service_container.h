#pragma once

#include <cassert>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <map>

namespace hvn3 {
	namespace services {

		class ServiceContainer final {

		public:
			ServiceContainer();

			template<typename ServiceType>
			ServiceType& AddService(const ServiceType& service);
			template<typename InterfaceType, typename ServiceType>
			ServiceType& AddService(const ServiceType& service);
			template<typename ServiceType, typename ...Args>
			ServiceType& AddService(Args&&... args);
			template<typename InterfaceType, typename ServiceType, typename ...Args>
			ServiceType& AddService(Args&&... args);

			template <typename ServiceType>
			ServiceType& GetService();
			template <typename ServiceType>
			const ServiceType& GetService() const;

			template<typename ServiceType>
			bool HasService() const;

		private:
			using void_deleter = void(*)(void*);

			std::multimap<std::type_index, std::unique_ptr<void, void_deleter>> services;

		};

		// Public members

		template<typename ServiceType>
		ServiceType& ServiceContainer::AddService(const ServiceType& service) {

			return AddService<ServiceType, ServiceType>(service);

		}
		template<typename InterfaceType, typename ServiceType>
		ServiceType& ServiceContainer::AddService(const ServiceType& service) {

			return AddService<ServiceType, ServiceType, const ServiceType&>(service);

		}
		template<typename ServiceType, typename ...Args>
		ServiceType& ServiceContainer::AddService(Args&&... args) {

			return AddService<ServiceType, ServiceType, Args...>(std::forward<Args>(args)...);

		}
		template<typename InterfaceType, typename ServiceType, typename ...Args>
		ServiceType& ServiceContainer::AddService(Args&&... args) {

			using interface_type = InterfaceType;
			using service_type = ServiceType;

			static_assert(std::is_same_v<InterfaceType, ServiceType> || std::is_base_of_v<interface_type, service_type>,
				"Service must implement the given interface");

			auto deleter = [](void* ptr) {
				std::default_delete<service_type>()(static_cast<service_type*>(ptr));
			};

			service_type* servicePtr = new service_type(std::forward<Args>(args)...);

			// Map the service type to the service object.

			services.emplace(std::make_pair(
				std::type_index(typeid(service_type)),
				std::unique_ptr<service_type, void_deleter>(servicePtr, deleter)
			));

			if (!std::is_same_v<InterfaceType, ServiceType>) {

				// Map the interface to the service object.
				// We supply an empty deleter so that we don't delete the service twice.

				services.emplace(std::make_pair(
					std::type_index(typeid(interface_type)),
					std::unique_ptr<interface_type, void_deleter>(servicePtr, [](void*) {})
				));

			}

			return *servicePtr;

		}

		template <typename ServiceType>
		ServiceType& ServiceContainer::GetService() {

			assert(HasService<ServiceType>());

			auto it = services.find(typeid(ServiceType));

			return *static_cast<ServiceType*>(it->second.get());

		}
		template <typename ServiceType>
		const ServiceType& ServiceContainer::GetService() const {

			assert(HasService<ServiceType>());

			auto it = services.find(typeid(ServiceType));

			return *static_cast<ServiceType*>(it->second.get());

		}

		template<typename ServiceType>
		bool ServiceContainer::HasService() const {

			auto it = services.find(typeid(ServiceType));

			return it != services.end();

		}

	}
}