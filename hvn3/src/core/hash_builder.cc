#include "core/hash_builder.h"

namespace hvn3::core {

	HashBuilder::HashBuilder() :
		HashBuilder(0) {
	}
	HashBuilder::HashBuilder(hash_t seed) {

		this->hash = seed;

	}

	HashBuilder::hash_t HashBuilder::Build() {

		return hash;

	}

}