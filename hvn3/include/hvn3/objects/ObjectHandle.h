#pragma once

/*
This class is designed to be a potential alternative to returning raw pointers from IObjectManager's methods,
but is currently not in use until its use cases can be more thoroughly established.

The key difference would be being able to check that the handle is valid before accessing the underlying object,
like using a weak_ptr. At the time of writing, ObjectManager uses shared_ptr instances for managing the lifetime
of objects, so encapsulating a weak_ptr through this interface would be an option. However, I may move away from
using shared_ptr instances for this purpose in the future.
*/

namespace hvn3 {

	template <typename object_type>
	class ObjectHandle {

	public:
		ObjectHandle(object_type* ptr) {
			_ptr = ptr;
		}

		object_type* operator->() {
			return _ptr;
		}
		object_type& operator*() {
			return *_ptr;
		}
		const object_type& operator*() const {
			return *_ptr;
		}

		explicit operator bool() const {
			// #todo Provide some means of checking the object is still valid with as little overhead as possible? Maybe without weak_ptr?
			return _ptr != nullptr;
		}
		explicit operator object_type*() {
			return _ptr;
		}
		explicit operator const object_type*() const {
			return _ptr;
		}

	private:
		object_type* _ptr; // Allows access to the object without locking every time

	};

}