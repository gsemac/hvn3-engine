#pragma once
#include "utility/Size.h"

namespace hvn3 {

	template <typename T>
	class SizeableBase {

	public:
		SizeableBase() :
			SizeableBase(0, 0) {}
		SizeableBase(T width, T height) {

			_width = width;
			_height = height;

		}
		SizeableBase(const Size<T>& size) {

			_width = size.Width();
			_height = size.Height();

		}

		// Returns the width of the object.
		virtual T Width() const {

			return _width;

		}
		// Returns the height of the object.
		virtual T Height() const {

			return _height;

		}
		// Sets the width of the object.
		virtual void SetWidth(T value) {

			Resize(value, _height);

		}
		// Sets the height of the object.
		virtual void SetHeight(T value) {

			Resize(_width, value);

		}
		// Resizes the object to the specified dimensions.
		virtual void Resize(T width, T height) {

			_width = width;
			_height = height;

		}

		// Returns the size of the object.
		Size<T> Size() const {

			return hvn3::Size<T>(_width, _height);

		}

	private:
		T _width;
		T _height;

	};

}