#pragma once
#include <functional>
#include "hvn3/exceptions/Exception.h"

namespace hvn3 {

	template <typename T>
	class Handle {

	public:
		Handle(std::function<T*()>&& on_access) :
			_on_access(on_access) {
		}
		Handle(std::function<T*()>&& on_access, std::function<void(T*)>&& on_destroy) :
			_on_access(on_access),
			_on_destroy(on_destroy) {
		}

		~Handle() {

			if (_on_destroy)
				_on_destroy(_on_access());

		}

		T* operator->() {
			return static_cast<T*>(*this);
		}
		T& operator*() {
			return *static_cast<T*>(*this);
		}
		explicit operator T*() {

			T* ptr = _on_access();

			if (ptr == nullptr)
				throw System::NullPointerException("The object pointed to by this handle is null.");

			return ptr;

		}

	private:
		std::function<T*()> _on_access;
		std::function<void(T*)> _on_destroy;

	};

}