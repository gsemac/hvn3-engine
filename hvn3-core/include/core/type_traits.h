#pragma once

#include <type_traits>

namespace hvn3::core {

	template <typename...>
	struct is_any {
		static constexpr bool value = false;
	};
	template <typename T, typename U, typename ...Ts>
	struct is_any<T, U, Ts...> {
		static constexpr bool value = std::is_same<T, U>::value || is_any<T, Ts...>::value;
	};

}