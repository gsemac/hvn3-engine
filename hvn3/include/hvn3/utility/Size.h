#pragma once
#include <iostream>

namespace hvn3 {

	template <typename T>
	class Size {

	public:
		Size(T width, T height) {
			SetSize(width, height);
		}

		T Width() const {
			return width;
		}
		T Height() const {
			return height;
		}
		void SetWidth(T value) {
			SetSize(value, height);
		}
		void SetHeight(T value) {
			SetSize(width, value);
		}
		void SetSize(T width, T height) {

			this->width = width;
			this->height = height;

		}
		void SetSize(const Size<T>& value) {
			SetSize(value.width, value.height);
		}

		template <typename U>
		explicit operator Size<U>() {
			return Size<U>(static_cast<U>(Width()), static_cast<U>(Height()));
		}

		T width;
		T height;

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
	bool operator!=(const Size<T>& lhs, const Size<T>& rhs) {
		return !(lhs == rhs);
	}

	template <typename T>
	std::ostream& operator<< (std::ostream& stream, const Size<T>& size) {

		stream << size.Width() << 'x' << size.Height();

		return stream;

	}

}