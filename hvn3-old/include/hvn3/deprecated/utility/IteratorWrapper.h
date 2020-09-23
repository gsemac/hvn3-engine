#pragma once

#include "hvn3/utility/IteratorTemplate.h"

#include <functional>

namespace hvn3 {

	template<typename IteratorType, typename ValueType>
	class IteratorWrapper :
		public IteratorTemplate<IteratorWrapper<IteratorType, ValueType>, typename ValueType, typename IteratorType::difference_type> {
		friend class base_type;

	public:
		typedef IteratorType iterator_type;
		typedef std::function<reference(iterator_type&)> dereference_function_type;

		IteratorWrapper(iterator_type& iterator, dereference_function_type&& dereference) :
			_iter(iterator),
			_dereference(std::move(dereference)) {
		}

	private:
		void increment() {
			++_iter;
		}
		void decrement() {
			--_iter;
		}
		reference dereference() {
			return _dereference(_iter);
		}
		bool equal(const derived_type& rhs) const {
			return _iter == rhs._iter;
		}
		void advance(difference_type n) {
			_iter += n;
		}

	private:
		iterator_type _iter;
		dereference_function_type _dereference;

	};

}