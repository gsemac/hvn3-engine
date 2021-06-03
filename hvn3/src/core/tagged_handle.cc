#include "core/tagged_handle.h"

namespace hvn3 {

	// Public members

	void* TaggedHandle::GetHandle() {

		return handle;

	}
	const void* TaggedHandle::GetHandle() const {

		return handle;

	}

	TaggedHandle::operator bool() const {

		return handle != nullptr;

	}

	// Private members

	TaggedHandle::TaggedHandle(const std::type_info& tag, void* handle) :
		tag(tag),
		handle(handle) {
	}

	// Other

	bool operator==(const TaggedHandle& lhs, const TaggedHandle& rhs) {

		return lhs.GetHandle() == rhs.GetHandle();

	}
	bool operator!=(const TaggedHandle& lhs, const TaggedHandle& rhs) {

		return !(lhs == rhs);

	}

}