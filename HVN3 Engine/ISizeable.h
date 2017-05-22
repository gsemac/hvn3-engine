#pragma once
#include "Size.h"

namespace hvn3 {

	class ISizeable {

	private:
		float __width, __height;

	public:
		ISizeable(float width = 0, float height = 0);

		// Returns the width of the object.
		virtual float Width() const;
		// Returns the height of the object.
		virtual float Height() const;
		// Sets the width of the object.
		virtual void SetWidth(float value);
		// Sets the height of the object.
		virtual void SetHeight(float value);
		// Resizes the object to the specified dimensions.
		virtual void Resize(float width, float height);

		// Returns the size of the object.
		Size Dimensions() const;

	};

}