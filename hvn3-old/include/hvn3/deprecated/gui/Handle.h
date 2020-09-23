#pragma once

namespace hvn3 {

	template <typename T>
	class Handle {

	public:
		Handle() :
			_ptr(nullptr) {
		}
		Handle(T* ptr) :
			_ptr(ptr) {
		}

		explicit operator bool() const {

			return !IsNull();

		}
		operator T*() const {

			return _ptr;

		}

		T* operator->() const {

			return _ptr;

		}
		T& operator*() const {

			return *_ptr;

		}

		bool operator==(const Handle<T> other) const {

			return _ptr == other._ptr;

		}
		bool operator!=(const Handle<T> other) const {

			return !(*this == other);

		}
		bool operator==(T* other) const {

			return _ptr == other;

		}
		bool operator!=(T* other) const {

			return _ptr != other;

		}

		bool IsNull() const {

			return _ptr == nullptr;

		}
		void Invalidate() {

			_ptr = nullptr;

		}

	private:
		T* _ptr;

	};

}