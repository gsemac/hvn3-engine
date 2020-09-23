#pragma once

#include "hvn3/exceptions/Exception.h"

#include <cassert>
#include <deque>
#include <limits>

namespace hvn3 {

	template<typename IntegerType>
	class UniqueIntegerGenerator {

	public:
		typedef IntegerType integer_type;

		UniqueIntegerGenerator() = default;
		UniqueIntegerGenerator(integer_type startingValue);

		integer_type Next();
		void Free(integer_type value);

	private:
		integer_type _current_value;
		std::deque<integer_type> _freed_values;

	};

	template<typename IntegerType>
	UniqueIntegerGenerator<IntegerType>::UniqueIntegerGenerator(typename UniqueIntegerGenerator<IntegerType>::integer_type startingValue) {
		_current_value = startingValue;
	}
	template<typename IntegerType>
	typename UniqueIntegerGenerator<IntegerType>::integer_type UniqueIntegerGenerator<IntegerType>::Next() {

		if (_freed_values.size() > 0) {

			// If there are values we can recycle from the pool, return one of those.

			integer_type value = _freed_values.front();

			_freed_values.pop_front();

			return value;

		}
		else {

			// Make sure we haven't exhausted the entire unique range.

			if (_current_value >= std::numeric_limits<integer_type>::max())
				throw Exception("Unique range has been exhausted.");

			// Return the next value.
			return _current_value++;

		}

	}
	template<typename IntegerType>
	void UniqueIntegerGenerator<IntegerType>::Free(integer_type value) {

		// The value we're freeing should be one that was returned previously.
		assert(value < _current_value);

		// The value we're freeing should not have been freed previously.
		assert(std::find(_freed_values.begin(), _freed_values.end(), value) == _freed_values.end());

		_freed_values.push_back(value);

	}

}