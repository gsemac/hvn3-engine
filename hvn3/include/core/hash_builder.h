#pragma once

#include <cstddef>
#include <functional>

namespace hvn3::core {

	class HashBuilder {

	public:
		typedef size_t hash_t;

		HashBuilder();
		HashBuilder(hash_t seed);

		template<typename T>
		void Add(const T& toHash);

		hash_t Build();

	private:
		hash_t hash;

	};

	template<typename T>
	void HashBuilder::Add(const T& toHash) {

		hash ^= std::hash<T>(toHash) + 0x9e3779b9 + (hash << 6) + (hash >> 2);

	}

}