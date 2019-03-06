#pragma once

#include <atomic>
#include <cstddef>

namespace hvn3 {

	// Used to assign a unique index to types, computed at runtime.
	// The class' template parameter is not used for anything more than differentiating between different sets of indices.
	// This class is threadsafe.

	template<typename ...Family>
	class TypeIndexer {

	public:
		typedef size_t index_type;

		// Returns the index of the given type.
		template<typename T>
		static index_type GetIndex() {

			static index_type index = _nextIndex();

			return index;

		}

	private:
		static index_type _nextIndex() {

			static std::atomic<index_type> index = 0;

			return index++;

		}

	};

}