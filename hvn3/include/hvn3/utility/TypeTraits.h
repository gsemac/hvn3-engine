#pragma once

#include <type_traits>

namespace hvn3 {

	template <typename...>
	struct is_any {
		static constexpr bool value = false;
	};

	template <typename CheckingType, typename CompareToType1, typename... CompareToTypeN>
	struct is_any<CheckingType, CompareToType1, CompareToTypeN...> {
		static constexpr bool value = std::is_same<CheckingType, CompareToType1>::value || is_any<CheckingType, CompareToTypeN...>::value;
	};

}