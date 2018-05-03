#pragma once
#include "hvn3/utility/Size.h"

namespace hvn3 {

	template <typename T>
	class ISizeable {

	public:
		// Returns the width of the object.
		virtual T Width() const = 0;
		// Returns the height of the object.
		virtual T Height() const = 0;
		// Sets the width of the object.
		virtual void SetWidth(T value) = 0;
		// Sets the height of the object.
		virtual void SetHeight(T value) = 0;
		// Sets the size of the object according to the given dimensions.
		virtual void SetSize(T width, T height) = 0;
		// Sets the size of the object according to the given dimensions.
		virtual void SetSize(const hvn3::Size<T>& value) = 0;
		// Returns the size of the object.
		virtual Size<T> Size() const = 0;

	};

}