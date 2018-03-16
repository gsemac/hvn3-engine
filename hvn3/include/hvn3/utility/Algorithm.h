#pragma once
#include <functional>
#include <iterator>
#include <unordered_map>
#include <utility>
#include <vector>
namespace hvn3 {

	// Moves matching elements to the ends of both containers and returns a pair of iterators one past the last elements. Not guaranteed to preserve element ordering.
	template<typename Iter1, typename Iter2>
	std::pair<Iter1, Iter2> RemoveSame(Iter1 begin_1, Iter1 end_1, Iter2 begin_2, Iter2 end_2, std::function<bool(typename const Iter1::value_type&, typename const Iter2::value_type&)> comparer) {

		bool found = false;

		for (auto i = begin_1; i != end_1;) {

			for (auto j = begin_2; j != end_2; ++j)
				if (comparer(*i, *j)) {
					*i = std::move(*--end_1);
					*j = std::move(*--end_2);
					found = true;
					break;
				}

			if (begin_2 == end_2)
				break;

			if (found) {
				found = false;
				continue;
			}

			++i;

		}

		return std::make_pair(end_1, end_2);

	}

	// Moves matching elements to the ends of both containers and returns a pair of iterators one past the last elements. Not guaranteed to preserve element ordering.
	template<typename Iter1, typename Iter2>
	std::pair<Iter1, Iter2> RemoveSame(Iter1 begin_1, Iter1 end_1, Iter2 begin_2, Iter2 end_2) {

		return RemoveSame(begin_1, end_1, begin_2, end_2, [](typename const Iter1::value_type& x, typename const Iter2::value_type& y) {return x == y; });
		
	}

	// Sorts a collection according to the order of elements in the second collection.
	template <typename IterBegin, typename IterEnd, typename IterOrderBegin, typename IterOrderEnd>
	void RelativeSort(IterBegin in_begin, IterEnd in_end, IterOrderBegin order_begin, IterOrderEnd order_end) {

		using key_type = typename IterBegin::value_type;

		std::unordered_map<key_type, size_t> order_map;
		size_t i = 0;

		for (auto it = order_begin; it != order_end; ++it, ++i)
			order_map[*it] = i;

		std::sort(in_begin, in_end, [&](key_type lhs, key_type rhs) {
			auto it_lhs = order_map.find(lhs);
			if (it_lhs == order_map.end())
				it_lhs = order_map.emplace(std::make_pair(lhs, ++i)).first;
			auto it_rhs = order_map.find(rhs);
			if (it_rhs == order_map.end())
				it_rhs = order_map.emplace(std::make_pair(rhs, ++i)).first;
			return it_lhs->second < it_rhs->second;
		});

	}
	
}