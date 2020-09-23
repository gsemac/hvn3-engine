#pragma once

#include "hvn3/objects/IObject.h"

#include <memory>
#include <utility>

namespace hvn3 {

	class ObjectHandle {

	public:
		ObjectHandle() {}
		ObjectHandle(std::weak_ptr<IObject>&& pointerToObject) {
			_object = std::move(pointerToObject);
		}

		std::shared_ptr<IObject> operator->() {
			return _object.lock();
		}
		const std::shared_ptr<IObject> operator->() const {
			return _object.lock();
		}

		explicit operator bool() const {
			return !_object.expired();
		}

	private:
		std::weak_ptr<IObject> _object;

	};

}