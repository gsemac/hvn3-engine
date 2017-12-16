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
		// Resizes the object to the specified dimensions.
		virtual void Resize(T width, T height) = 0;
		// Returns the size of the object.
		virtual Size<T> Size() const = 0;

	};

}