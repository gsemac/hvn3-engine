#pragma once

#include <atomic>
#include <cstddef>

namespace hvn3::core {

	// Used to assign a unique index to types, computed at runtime.
	// The class' "Family" template parameter is not used for anything more than differentiating between different sets of indices.
	// This class is threadsafe.

	template<typename Type, typename ...Family>
	class TypeIndex {

	public:
		typedef int index_type;

		static const index_type Null;

		TypeIndex() :
			TypeIndex(GetIndex<Type>()) {
		}

		// Returns the index of the given type.

		template<typename T>
		static index_type GetIndex() {

			static index_type index = NextIndex();

			return index;

		}

		explicit operator index_type() const {

			return value;

		}

	private:
		index_type value;

		TypeIndex(index_type value) :
			value(value) {
		}

		static index_type NextIndex() {

			static std::atomic<index_type> index = 0;

			return index++;

		}

	};

	template<typename Type, typename ...Family>
	const typename TypeIndex<Type, Family...>::index_type TypeIndex<Type, Family...>::Null = -1;

}