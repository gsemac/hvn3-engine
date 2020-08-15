#pragma once

#include "core/lazy.h"

#include <functional>
#include <map>
#include <type_traits>
#include <typeindex>
#include <utility>

#define HVN3_INJECT(signature)\
using injected_constructor = signature;\
signature

namespace hvn3::services {

	template <typename T>
	class has_injected_constructor {

		template <typename T1>
		static typename T1::injected_constructor* test(int);
		template <typename>
		static void test(...);

	public:
		static constexpr bool value = !std::is_void<decltype(test<T>(0))>::value;

	};

	template<typename T>
	constexpr bool has_injected_constructor_v = has_injected_constructor<T>::value;

	template<typename T>
	constexpr bool is_inject_constructible_v = has_injected_constructor_v<T> || std::is_default_constructible_v<T>;

	class DIServiceContainer final {

	public:
		using size_type = std::size_t;

		template<typename ServiceType, std::enable_if_t<is_inject_constructible_v<ServiceType>, int> = 0>
		DIServiceContainer& RegisterService();
		template<typename InterfaceType, typename ServiceType, std::enable_if_t<is_inject_constructible_v<ServiceType>, int> = 0>
		DIServiceContainer& RegisterService();
		template<typename ServiceType>
		DIServiceContainer& RegisterService(const std::shared_ptr<ServiceType>& service);
		template<typename InterfaceType, typename ServiceType>
		DIServiceContainer& RegisterService(const std::shared_ptr<ServiceType>& service);

		template<typename ServiceType, std::enable_if_t<!std::is_pointer_v<ServiceType>, int> = 0>
		ServiceType& GetService();
		template<typename ServiceType, std::enable_if_t<!std::is_pointer_v<ServiceType>, int> = 0>
		const ServiceType& GetService() const;
		template <typename ServiceType, std::enable_if_t<std::is_pointer_v<ServiceType>, int> = 0>
		ServiceType GetService();
		template <typename ServiceType, std::enable_if_t<std::is_pointer_v<ServiceType>, int> = 0>
		const ServiceType GetService() const;

		template<typename ServiceType>
		bool IsServiceRegistered() const;

		size_type Count() const;

	private:
		class IServiceDescriptor {

		public:
			virtual ~IServiceDescriptor() = default;

			virtual void* GetService() = 0;

		};

		template<typename LazyType>
		class LazyServiceDescriptor :
			public IServiceDescriptor {

		public:
			LazyServiceDescriptor(LazyType&& lazy) :
				lazy(lazy) {
			}

			void* GetService() override {

				return &lazy.Value();

			}

		private:
			LazyType lazy;

		};

		template<typename ServiceType>
		class WrapperServiceDescriptor :
			public IServiceDescriptor {

		public:
			WrapperServiceDescriptor(const std::shared_ptr<ServiceType>& service) :
				service(service) {
			}

			void* GetService() override {

				return service.get();

			}

		private:
			std::shared_ptr<ServiceType> service;

		};

		using service_pointer_t = std::shared_ptr<IServiceDescriptor>;

		std::multimap<std::type_index, service_pointer_t> services;

		template<typename InterfaceType, typename ServiceType, std::enable_if_t<has_injected_constructor_v<ServiceType>, int> = 0>
		service_pointer_t CreateServiceDescriptor();
		template<typename InterfaceType, typename ServiceType, typename ...ConstructorArgs>
		service_pointer_t CreateServiceDescriptor(ServiceType(*)(ConstructorArgs...));
		template<typename InterfaceType, typename ServiceType, std::enable_if_t<!has_injected_constructor_v<ServiceType>&& std::is_default_constructible_v<ServiceType>, int> = 0>
		service_pointer_t CreateServiceDescriptor();
		template<typename InterfaceType, typename ServiceType>
		void RegisterServiceDescriptor(service_pointer_t&& servicePointer);

	};

	// Public members

	template<typename ServiceType, std::enable_if_t<is_inject_constructible_v<ServiceType>, int>>
	DIServiceContainer& DIServiceContainer::RegisterService() {

		return RegisterService<ServiceType, ServiceType>();

	}
	template<typename InterfaceType, typename ServiceType, std::enable_if_t<is_inject_constructible_v<ServiceType>, int>>
	DIServiceContainer& DIServiceContainer::RegisterService() {

		// Services with injectable contructors will be constructed lazily to avoid temporal coupling.

		RegisterServiceDescriptor<InterfaceType, ServiceType>(CreateServiceDescriptor<InterfaceType, ServiceType>());

		return *this;

	}
	template<typename ServiceType>
	DIServiceContainer& DIServiceContainer::RegisterService(const std::shared_ptr<ServiceType>& service) {

		return RegisterService<ServiceType, ServiceType>(service);

	}
	template<typename InterfaceType, typename ServiceType>
	DIServiceContainer& DIServiceContainer::RegisterService(const std::shared_ptr<ServiceType>& service) {

		RegisterServiceDescriptor<InterfaceType, ServiceType>(std::make_shared<WrapperServiceDescriptor<ServiceType>>(service));

		return *this;

	}

	template<typename ServiceType, std::enable_if_t<!std::is_pointer_v<ServiceType>, int>>
	ServiceType& DIServiceContainer::GetService() {

		using service_t = std::remove_reference_t<ServiceType>;

		assert(IsServiceRegistered<service_t>());

		auto it = services.find(typeid(service_t));

		return *static_cast<service_t*>(it->second->GetService());

	}
	template<typename ServiceType, std::enable_if_t<!std::is_pointer_v<ServiceType>, int>>
	const ServiceType& DIServiceContainer::GetService() const {

		using service_t = std::remove_reference_t<ServiceType>;

		assert(IsServiceRegistered<service_t>());

		auto it = services.find(typeid(service_t));

		return *static_cast<service_t*>(it->second->GetService());

	}
	template <typename ServiceType, std::enable_if_t<std::is_pointer_v<ServiceType>, int>>
	ServiceType DIServiceContainer::GetService() {

		using service_t = std::remove_pointer_t<std::remove_reference_t<ServiceType>>;

		auto it = services.find(typeid(service_t));

		return it == services.end() ?
			nullptr :
			static_cast<service_t*>(it->second->GetService());

	}
	template <typename ServiceType, std::enable_if_t<std::is_pointer_v<ServiceType>, int>>
	const ServiceType DIServiceContainer::GetService() const {

		using service_t = std::remove_pointer_t<std::remove_reference_t<ServiceType>>;

		auto it = services.find(typeid(service_t));

		return it == services.end() ?
			nullptr :
			static_cast<service_t*>(it->second->GetService());

	}

	template<typename ServiceType>
	bool DIServiceContainer::IsServiceRegistered() const {

		using service_t = std::remove_reference_t<ServiceType>;

		auto it = services.find(typeid(service_t));

		return it != services.end();

	}

	// Private members

	template<typename InterfaceType, typename ServiceType, std::enable_if_t<has_injected_constructor_v<ServiceType>, int>>
	DIServiceContainer::service_pointer_t DIServiceContainer::CreateServiceDescriptor() {

		return CreateServiceDescriptor<InterfaceType, ServiceType>(static_cast<typename ServiceType::injected_constructor*>(nullptr));

	}
	template<typename InterfaceType, typename ServiceType, typename ...ConstructorArgs>
	DIServiceContainer::service_pointer_t DIServiceContainer::CreateServiceDescriptor(ServiceType(*)(ConstructorArgs...)) {

		auto lazy = core::make_lazy([this]() { return ServiceType(this->GetService<ConstructorArgs>()...); });

		return std::make_shared<LazyServiceDescriptor<decltype(lazy)>>(std::move(lazy));

	}
	template<typename InterfaceType, typename ServiceType, std::enable_if_t<!has_injected_constructor_v<ServiceType>&& std::is_default_constructible_v<ServiceType>, int>>
	DIServiceContainer::service_pointer_t DIServiceContainer::CreateServiceDescriptor() {

		auto lazy = core::make_lazy([]() { return ServiceType(); });

		return std::make_shared<LazyServiceDescriptor<decltype(lazy)>>(std::move(lazy));

	}
	template<typename InterfaceType, typename ServiceType>
	void DIServiceContainer::RegisterServiceDescriptor(service_pointer_t&& servicePtr) {

		using interface_t = std::remove_reference_t<InterfaceType>;
		using service_t = std::remove_reference_t<ServiceType>;

		static_assert(std::is_same_v<interface_t, service_t> || std::is_base_of_v<interface_t, service_t>, "The service must implement the given interface.");

		// Insert the service into the container.

		services.emplace(std::make_pair(std::type_index(typeid(service_t)), servicePtr));

		// If the service is implementing an interface, map the interface to the service as well.

		if (!std::is_same_v<interface_t, service_t>) {

			services.emplace(std::make_pair(std::type_index(typeid(interface_t)), servicePtr));

		}

	}

}