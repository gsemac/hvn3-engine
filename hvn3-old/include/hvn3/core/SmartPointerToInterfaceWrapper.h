#pragma once

namespace hvn3 {
	namespace system {

		template<typename PointerType, typename ObjectType>
		class SmartPointerToInterfaceWrapper {

		public:
			typedef PointerType pointer_type;

			SmartPointerToInterfaceWrapper() {}
			SmartPointerToInterfaceWrapper(pointer_type&& pointer) {
				_ptr = std::move(pointer);
			}

			ObjectType* operator->() {
				return static_cast<ObjectType*>(_ptr.get());
			}
			const ObjectType* operator->() const {
				return static_cast<ObjectType*>(_ptr.get());
			}

			explicit operator bool() const {
				return static_cast<bool>(_ptr);
			}
			operator const pointer_type&() const {
				return _ptr;
			}
			operator pointer_type&() {
				return _ptr;
			}

		private:
			pointer_type _ptr;

		};

	}
}