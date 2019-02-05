#pragma once
#include "hvn3/core/CoreDefs.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/core/ManagerRegistry.h"

#include <exception>
#include <type_traits>

namespace hvn3 {

	class ManagerRegistry;

	class Context {

	public:
		Context();
		Context(ManagerRegistry* global, ManagerRegistry* local);
		Context(const Context& copy_global, ManagerRegistry* local);

		ManagerRegistry& Local() const;
		ManagerRegistry& Global() const;

		bool HasLocalContext() const;
		bool HasGlobalContext() const;

		explicit operator bool() const;

		template <ManagerId MANAGER_ID>
		typename ManagerIdTraits<MANAGER_ID>::type& Get() const {

			using interface_type = typename ManagerIdTraits<MANAGER_ID>::type;

			return Get<interface_type>();

		}
		template <typename InterfaceType>
		InterfaceType& Get() const {

			if (HasLocalContext() && _local->IsRegistered<InterfaceType>())
				return _local->Get<InterfaceType>();

			if (HasGlobalContext() && _global->IsRegistered<InterfaceType>())
				return _global->Get<InterfaceType>();

			HVN3_ASSERT(false, "The requested manager was not registered in any context.");

			// Prevents warning about no return value
			throw std::logic_error("this should never be reached");

		}

		template <typename InterfaceType>
		bool IsRegistered() const {

			if (HasLocalContext() && _local->IsRegistered<InterfaceType>())
				return true;

			if (HasGlobalContext() && _global->IsRegistered<InterfaceType>())
				return true;

			return false;

		}
		template <ManagerId MANAGER_ID>
		bool IsRegistered() const {

			using interface_type = typename ManagerIdTraits<MANAGER_ID>::type;

			return IsRegistered<interface_type>();

		}

	private:
		ManagerRegistry* _global;
		ManagerRegistry* _local;

	};

	class ContextChangedEventArgs :
		public EventArgs {

	public:
		ContextChangedEventArgs(const hvn3::Context& context) :
			_context(context) {
		}

		const hvn3::Context& Context() const {
			return _context;
		}

	private:
		hvn3::Context _context;

	};

}