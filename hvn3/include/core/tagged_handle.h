#pragma once

#include <cassert>
#include <typeinfo>

namespace hvn3 {

	class TaggedHandle {

		template<typename T>
		using pointer_type = std::add_pointer_t<std::remove_reference_t<T>>;

		template<typename T>
		friend TaggedHandle make_tagged_handle(T* data);

	public:
		template<typename T, std::enable_if_t<std::is_same_v<void*, pointer_type<T>>, int> = 0>
		pointer_type<T> GetHandle() {

			return handle;

		}
		template<typename T, std::enable_if_t<std::is_same_v<void*, pointer_type<T>>, int> = 0>
		const pointer_type<T> GetHandle() const {

			return handle;

		}
		template<typename T, std::enable_if_t<!std::is_same_v<void*, pointer_type<T>>, int> = 0>
		pointer_type<T> GetHandle() {

			assert(typeid(pointer_type<T>) == tag);

			return (pointer_type<T>)(handle);

		}
		template<typename T, std::enable_if_t<!std::is_same_v<void*, pointer_type<T>>, int> = 0>
		const pointer_type<T> GetHandle() const {

			assert(typeid(pointer_type<T>) == tag);

			return (pointer_type<T>)(handle);

		}
		void* GetHandle();
		const void* GetHandle() const;

		template<typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
		operator T();
		template<typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
		operator T() const;
		explicit operator bool() const;

	private:
		const std::type_info& tag;
		void* handle;

		TaggedHandle(const std::type_info& tag, void* handle);

	};

	template<typename T, typename>
	TaggedHandle::operator T () {

		return GetHandle<std::remove_pointer_t<T>>();

	}
	template<typename T, typename>
	TaggedHandle::operator T () const {

		return GetHandle<std::remove_pointer_t<T>>();

	}

	bool operator==(const TaggedHandle& lhs, const TaggedHandle& rhs);
	bool operator!=(const TaggedHandle& lhs, const TaggedHandle& rhs);

	template<typename T>
	TaggedHandle make_tagged_handle(T* handle) {

		return TaggedHandle(typeid(TaggedHandle::pointer_type<T>), handle);

	}

}