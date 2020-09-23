#pragma once

#include <iostream>

namespace hvn3::core {

	template<typename T>
	class Size {

	public:
		T width;
		T height;

		Size() :
			Size(0, 0) {
		}
		Size(T width, T height) {

			this->width = width;
			this->height = height;

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

	};

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

	using SizeF = Size<float>;
	using SizeI = Size<int>;

}