#pragma once
#include <cstddef>
#include <functional>

namespace hvn3 {

	template <class T>
	inline void CombineHash(std::size_t& seed, const T& v) {

		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

	}

	struct PairHasher {

	public:
		template <typename T, typename U>
		std::size_t operator()(const std::pair<T, U> &x) const {
			return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
		}

	};

}