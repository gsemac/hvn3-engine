#pragma once

#include "hvn3/services/service_container.h"

#include <type_traits>
#include <utility>

#define HVN3_INJECT(signature)\
using injected_constructor = signature;\
signature

namespace hvn3 {
	namespace services {

		class DIServiceContainer final {

			template <typename T>
			class has_injected_constructor_typedef {

				template <typename T1>
				static typename T1::injected_constructor* test(int);
				template <typename>
				static void test(...);

			public:
				static constexpr bool value = !std::is_void<decltype(test<T>(0))>::value;

			};

		public:
			template<typename ServiceType, std::enable_if_t<has_injected_constructor_typedef<ServiceType>::value, int> = 0>
			DIServiceContainer& AddService();
			template<typename InterfaceType, typename ServiceType, std::enable_if_t<has_injected_constructor_typedef<ServiceType>::value, int> = 0>
			DIServiceContainer& AddService();
			template<typename ServiceType, typename ...Args>
			DIServiceContainer& AddService(Args&&... args);
			template<typename InterfaceType, typename ServiceType, typename ...Args>
			DIServiceContainer& AddService(Args&&... args);

			template <typename ServiceType, std::enable_if_t<!std::is_pointer_v<ServiceType>, int> = 0>
			ServiceType& GetService();
			template <typename ServiceType, std::enable_if_t<!std::is_pointer_v<ServiceType>, int> = 0>
			const ServiceType& GetService() const;
			template <typename ServiceType, std::enable_if_t<std::is_pointer_v<ServiceType>, int> = 0>
			ServiceType GetService();
			template <typename ServiceType, std::enable_if_t<std::is_pointer_v<ServiceType>, int> = 0>
			ServiceType GetService() const;

			template<typename ServiceType>
			bool HasService() const;

			ServiceContainer::size_type size() const;

			template<typename ResolveType, std::enable_if_t<has_injected_constructor_typedef<ResolveType>::value, int> = 0>
			ResolveType Resolve();
			template<typename ResolveType, std::enable_if_t<!has_injected_constructor_typedef<ResolveType>::value && std::is_default_constructible_v<ResolveType>, int> = 0>
			ResolveType Resolve();
			template<typename ResolveType, std::enable_if_t<!has_injected_constructor_typedef<ResolveType>::value && !std::is_default_constructible_v<ResolveType>, int> = 0>
			ResolveType Resolve();

		private:
			ServiceContainer services;

			template<typename InterfaceType, typename ServiceType, typename ...ConstructorArgs>
			DIServiceContainer& AddService(ServiceType(*)(ConstructorArgs...));
			template<typename ...ServiceTypes>
			void EnsureHasServices();
			template<typename ServiceType, std::enable_if_t<std::is_abstract_v<ServiceType>, int> = 0>
			void EnsureHasService();
			template<typename ServiceType, std::enable_if_t<!std::is_abstract_v<ServiceType>, int> = 0>
			void EnsureHasService();
			template<typename ResolveType, typename ...ConstructorArgs>
			ResolveType Resolve(ResolveType(*)(ConstructorArgs...));

		};

		// Public members

		template<typename ServiceType, std::enable_if_t<DIServiceContainer::has_injected_constructor_typedef<ServiceType>::value, int>>
		DIServiceContainer& DIServiceContainer::AddService() {

			return AddService<ServiceType, ServiceType>();

		}
		template<typename InterfaceType, typename ServiceType, std::enable_if_t<DIServiceContainer::has_injected_constructor_typedef<ServiceType>::value, int>>
		DIServiceContainer& DIServiceContainer::AddService() {

			return AddService<InterfaceType, ServiceType>(static_cast<typename ServiceType::injected_constructor*>(nullptr));

		}
		template<typename ServiceType, typename ...Args>
		DIServiceContainer& DIServiceContainer::AddService(Args&&... args) {

			services.AddService<ServiceType>(std::forward<Args>(args)...);

			return *this;

		}
		template<typename InterfaceType, typename ServiceType, typename ...Args>
		DIServiceContainer& DIServiceContainer::AddService(Args&&... args) {

			services.AddService<InterfaceType, ServiceType>(std::forward<Args>(args)...);

			return *this;

		}

		template <typename ServiceType, std::enable_if_t<!std::is_pointer_v<ServiceType>, int>>
		ServiceType& DIServiceContainer::GetService() {

			return services.GetService<ServiceType>();

		}
		template <typename ServiceType, std::enable_if_t<!std::is_pointer_v<ServiceType>, int>>
		const ServiceType& DIServiceContainer::GetService() const {

			return services.GetService<ServiceType>();

		}
		template <typename ServiceType, std::enable_if_t<std::is_pointer_v<ServiceType>, int>>
		ServiceType DIServiceContainer::GetService() {

			using service_type = std::remove_pointer_t<ServiceType>;

			if (services.HasService<service_type>())
				return &services.GetService<service_type>();

			return nullptr;

		}
		template <typename ServiceType, std::enable_if_t<std::is_pointer_v<ServiceType>, int>>
		ServiceType DIServiceContainer::GetService() const {

			using service_type = std::remove_pointer_t<ServiceType>;

			if (services.HasService<service_type>())
				return &services.GetService<service_type>();

			return nullptr;

		}

		template<typename ServiceType>
		bool DIServiceContainer::HasService() const {

			return services.HasService<ServiceType>();

		}

		ServiceContainer::size_type DIServiceContainer::size() const {

			return services.size();

		}

		template<typename ResolveType, std::enable_if_t<DIServiceContainer::has_injected_constructor_typedef<ResolveType>::value, int>>
		ResolveType DIServiceContainer::Resolve() {

			return Resolve<ResolveType>(static_cast<typename ResolveType::injected_constructor*>(nullptr));

		}
		template<typename ResolveType, std::enable_if_t<!DIServiceContainer::has_injected_constructor_typedef<ResolveType>::value && std::is_default_constructible_v<ResolveType>, int>>
		ResolveType DIServiceContainer::Resolve() {

			return ResolveType();

		}
		template<typename ResolveType, std::enable_if_t<!DIServiceContainer::has_injected_constructor_typedef<ResolveType>::value && !std::is_default_constructible_v<ResolveType>, int>>
		ResolveType DIServiceContainer::Resolve() {

			static_assert(false, "Dependencies could not be resolved for this type.");

		}

		// Private members

		template<typename InterfaceType, typename ServiceType, typename ...ConstructorArgs>
		DIServiceContainer& DIServiceContainer::AddService(ServiceType(*)(ConstructorArgs...)) {

			static_assert(std::is_constructible_v<ServiceType, ConstructorArgs...>, "Service does not have an appropriate constructor taking its required services.");

			// Add the required services that don't already exist.

			EnsureHasServices<ConstructorArgs...>();

			// Add the service.

			AddService<InterfaceType, ServiceType>(GetService<ConstructorArgs>()...);

			return *this;

		}
		template<typename ...ServiceTypes>
		void DIServiceContainer::EnsureHasServices() {

			int dummy[] = { 0, (EnsureHasService<std::remove_reference_t<ServiceTypes>>(), 0)... };

		}
		template<typename ServiceType, std::enable_if_t<std::is_abstract_v<ServiceType>, int>>
		void DIServiceContainer::EnsureHasService() {
		}
		template<typename ServiceType, std::enable_if_t<!std::is_abstract_v<ServiceType>, int>>
		void DIServiceContainer::EnsureHasService() {

			if (!HasService<ServiceType>() && !std::is_pointer_v<ServiceType>)
				AddService<ServiceType>();

		}
		template<typename ResolveType, typename ...ConstructorArgs>
		ResolveType DIServiceContainer::Resolve(ResolveType(*)(ConstructorArgs...)) {

			static_assert(std::is_constructible_v<ResolveType, ConstructorArgs...>, "Service does not have an appropriate constructor taking its required services.");

			return ResolveType((HasService<ConstructorArgs>() ? GetService<ConstructorArgs>() : Resolve<ConstructorArgs>())...);

		}

	}
}