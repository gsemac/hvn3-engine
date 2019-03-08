#include "hvn3/core/ApplicationContext.h"

#include <cassert>

namespace hvn3 {

	ApplicationContext::ApplicationContext() :
		ApplicationContext(nullptr, nullptr) {
	}
	ApplicationContext::ApplicationContext(ManagerRegistry* global, ManagerRegistry* local) {

		_global = global;
		_local = local;

	}
	ApplicationContext::ApplicationContext(const ApplicationContext& copy_global, ManagerRegistry* local) :
		ApplicationContext(copy_global.HasGlobalContext() ? &copy_global.Global() : nullptr, local) {
	}
	ManagerRegistry& ApplicationContext::Local() const {

		assert(_local != nullptr);

		return *_local;

	}
	ManagerRegistry& ApplicationContext::Global() const {

		assert(_global != nullptr);

		return *_global;

	}

	bool ApplicationContext::HasLocalContext() const {
		return _local != nullptr;
	}
	bool ApplicationContext::HasGlobalContext() const {
		return _global != nullptr;
	}
	ApplicationContext::operator bool() const {
		return !(_local == nullptr && _global == nullptr);
	}

}