#pragma once
#include <iostream>

namespace hvn3 {

	template <typename T>
	struct Size {

	public:
		Size(T width, T height) {

			_width = width;
			_height = height;

		}

		T Width() const {

			return _width;

		}
		T Height() const {

			return _height;

		}
		void SetWidth(T value) {

			Resize(value, _height);

		}
		void SetHeight(T value) {
			
			Resize(_width, value);

		}
		void Resize(T width, T height) {

			_width = width;
			_height = height;

		}

	private:
		T _width;
		T _height;

	};

	typedef Size<int> SizeI;
	typedef Size<float> SizeF;

	template <typename T>
	bool operator<(const Size<T>& a, const Size<T>& b) {

		return (a.Width() * b.Height() < b.Width() * b.Height());

	}
	template <typename T>
	bool operator>(const Size<T>& a, const Size<T>& b) {

		return (a.Width() * b.Height() > b.Width() * b.Height());

	}
	template <typename T>
	bool operator<=(const Size<T>& a, const Size<T>& b) {

		return (a < b || a == b);

	}
	template <typename T>
	bool operator>=(const Size<T>& a, const Size<T>& b) {

		return (a > b || a == b);

	}
	template <typename T>
	bool operator==(const Size<T>& a, const Size<T>& b) {

		return (a.Width() == b.Width() && a.Height() == b.Height());

	}

	template <typename T>
	std::ostream& operator<< (std::ostream& stream, const Size<T>& size) {

		stream << size.Width() << 'x' << size.Height();

		return stream;

	}

}