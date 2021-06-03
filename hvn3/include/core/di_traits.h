#pragma once

#include <type_traits>

namespace hvn3::core {

	template <typename T>
	class has_injected_constructor {

		template <typename T1>
		static typename T1::injected_constructor* test(int);
		template <typename>
		static void test(...);

	public:
		static constexpr bool value = !std::is_void<decltype(test<T>(0))>::value;

	};

	template<typename T>
	constexpr bool has_injected_constructor_v = has_injected_constructor<T>::value;

	template<typename T>
	constexpr bool is_inject_constructible_v = has_injected_constructor_v<T> || std::is_default_constructible_v<T>;

}