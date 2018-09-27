#include "hvn3/core/Context.h"

#include <cassert>

namespace hvn3 {

	Context::Context() :
		Context(nullptr, nullptr) {
	}
	Context::Context(ManagerRegistry* global, ManagerRegistry* local) {

		_global = global;
		_local = local;

	}
	Context::Context(const Context& copy_global, ManagerRegistry* local) :
		Context(copy_global.HasGlobalContext() ? &copy_global.Global() : nullptr, local) {
	}
	ManagerRegistry& Context::Local() const {

		assert(_local != nullptr);

		return *_local;

	}
	ManagerRegistry& Context::Global() const {

		assert(_global != nullptr);

		return *_global;

	}

	bool Context::HasLocalContext() const {
		return _local != nullptr;
	}
	bool Context::HasGlobalContext() const {
		return _global != nullptr;
	}
	Context::operator bool() const {
		return !(_local == nullptr && _global == nullptr);
	}

}