#pragma once

#include "iterators/reverse_iterator.h"

#include <algorithm>
#include <utility>

namespace hvn3::core {

	constexpr ptrdiff_t npos = -1;

	template<typename IteratorT, typename ValueT>
	typename ptrdiff_t index_of(IteratorT begin, IteratorT end, ValueT value) {

		auto it = std::find(begin, end, std::forward<ValueT>(value));

		if (it == end)
			return static_cast<ptrdiff_t>(npos);

		return std::distance(begin, it);

	}
	template<typename IteratorT, typename ValueT>
	typename ptrdiff_t last_index_of(IteratorT begin, IteratorT end, ValueT value) {

		auto rBegin = iterators::make_reverse_iterator(end);
		auto rEnd = iterators::make_reverse_iterator(begin);

		auto index = index_of(rBegin, rEnd, std::forward<ValueT>(value));
		auto totalDistance = std::distance(begin, end);

		return index == npos ?
			npos :
			totalDistance - index;

	}
	template<typename IteratorT, typename PredicateT>
	typename ptrdiff_t index_of_any(IteratorT begin, IteratorT end, PredicateT predicate) {

		auto it = std::find_if(begin, end, std::forward<PredicateT>(predicate));

		if (it == end)
			return static_cast<ptrdiff_t>(npos);

		return std::distance(begin, it);

	}
	template<typename IteratorT, typename PredicateT>
	typename ptrdiff_t last_index_of_any(IteratorT begin, IteratorT end, PredicateT predicate) {

		auto rBegin = iterators::make_reverse_iterator(end);
		auto rEnd = iterators::make_reverse_iterator(begin);

		auto index = index_of_any(rBegin, rEnd, std::forward<PredicateT>(predicate));
		auto totalDistance = std::distance(begin, end);

		return index == npos ?
			npos :
			totalDistance - index;

	}
	template<typename SearchIteratorT, typename FindIteratorT>
	typename ptrdiff_t index_of_any(SearchIteratorT searchBegin, SearchIteratorT searchEnd, FindIteratorT findBegin, FindIteratorT findEnd) {

		return index_of_any(searchBegin, searchEnd,
			[=](const typename std::iterator_traits<SearchIteratorT>::value_type& i) {
				return std::any_of(findBegin, findEnd, [&](const typename std::iterator_traits<FindIteratorT>::value_type& j) {return i == j; });
			});

	}
	template<typename SearchIteratorT, typename FindIteratorT>
	typename ptrdiff_t last_index_of_any(SearchIteratorT searchBegin, SearchIteratorT searchEnd, FindIteratorT findBegin, FindIteratorT findEnd) {

		auto rBegin = iterators::make_reverse_iterator(searchEnd);
		auto rEnd = iterators::make_reverse_iterator(searchBegin);

		auto index = index_of_any(rBegin, rEnd, findBegin, findEnd);
		auto totalDistance = std::distance(searchBegin, searchEnd);

		return index == npos ?
			npos :
			totalDistance - index;

	}

	template<typename SearchIteratorT, typename FindIteratorT>
	bool starts_with(SearchIteratorT searchBegin, SearchIteratorT searchEnd, FindIteratorT findBegin, FindIteratorT findEnd) {

		auto i1 = searchBegin;
		auto i2 = findBegin;

		for (; i1 != searchEnd && i2 != findEnd; ++i1, ++i2) {

			if (*i1 != *i2)
				return false;

		}

		return true;

	}
	template<typename SearchIteratorT, typename FindIteratorT>
	bool ends_with(SearchIteratorT searchBegin, SearchIteratorT searchEnd, FindIteratorT findBegin, FindIteratorT findEnd) {

		auto searchRBegin = iterators::make_reverse_iterator(searchEnd);
		auto searchREnd = iterators::make_reverse_iterator(searchBegin);
		auto findRBegin = iterators::make_reverse_iterator(findEnd);
		auto findREnd = iterators::make_reverse_iterator(findBegin);

		return starts_with(searchRBegin, searchREnd, findRBegin, findREnd);

	}

}