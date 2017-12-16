#pragma once
#include "hvn3/core/ISizeable.h"

namespace hvn3 {

	template <typename T>
	class SizeableBase : public virtual ISizeable<T> {

	public:
		SizeableBase() :
			SizeableBase(0, 0) {
		}
		SizeableBase(T width, T height) {

			_width = width;
			_height = height;

		}
		SizeableBase(const hvn3::Size<T>& size) {

			_width = size.Width();
			_height = size.Height();

		}

		// Returns the width of the object.
		T Width() const override {

			return _width;

		}
		// Returns the height of the object.
		T Height() const override {

			return _height;

		}
		// Sets the width of the object.
		void SetWidth(T value) override {

			Resize(value, _height);

		}
		// Sets the height of the object.
		void SetHeight(T value) override {

			Resize(_width, value);

		}
		// Resizes the object to the specified dimensions.
		void Resize(T width, T height) override {

			_width = width;
			_height = height;

		}
		// Returns the size of the object.
		hvn3::Size<T> Size() const override {

			return hvn3::Size<T>(_width, _height);

		}

	private:
		T _width;
		T _height;

	};

}