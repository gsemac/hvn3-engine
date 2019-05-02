#pragma once

#include <type_traits>

#include "hvn3/utility/TypeTraits.h"

namespace hvn3 {

	namespace implementation {

		template<typename T, typename U>
		struct TypeListCat;

		template<typename ...Ts> struct TypeListUnion;
		template<typename ...Ts> struct TypeListUnionHelper;
		template<typename ...Ts> struct TypeListFlatten;
		template<typename ...Ts> struct TypeListFlattenHelper;
		template<typename T, typename U> struct TypeListDistinctHelper;

	}

	template<typename... Types>
	struct TypeList {

		template<typename ...Ts>
		using append_type = TypeList<Types..., Ts...>;
		template<typename ...Ts>
		using union_type = typename implementation::TypeListUnion<TypeList<Types...>, Ts...>::type;
		using flatten_type = typename implementation::TypeListFlatten<Types...>::type;
		using distinct_type = typename implementation::TypeListDistinctHelper<TypeList<>, TypeList<Types...>>::type;

		template<typename T>
		static constexpr bool contains = is_any<T, Types...>::value;

	};

	namespace implementation {

		template<typename ...Ts, typename ...Us>
		struct TypeListCat<TypeList<Ts...>, TypeList<Us...>> {
			typedef TypeList<Ts..., Us...> type;
		};

		template<typename T1, typename T2, typename ...Ts>
		struct TypeListUnion<T1, T2, Ts...> {
			typedef typename TypeListUnion<typename TypeListUnionHelper<T1, T2>::type, Ts...>::type type;
		};
		template<typename ...Ts>
		struct TypeListUnion<TypeList<Ts...>> {
			typedef TypeList<Ts...> type;
		};
		template<typename T>
		struct TypeListUnion<T> {
			typedef TypeList<T> type;
		};
		template<>
		struct TypeListUnion<> {
			typedef TypeList<> type;
		};

		template<typename T, typename U>
		struct TypeListUnionHelper<T, U> {
			typedef TypeList<T, U> type;
		};
		template<typename ...Ts, typename U>
		struct TypeListUnionHelper<TypeList<Ts...>, U> {
			typedef TypeList<Ts..., U> type;
		};
		template<typename T, typename ...Us>
		struct TypeListUnionHelper<T, TypeList<Us...>> {
			typedef TypeList<T, Us...> type;
		};
		template<typename ...Ts, typename ...Us>
		struct TypeListUnionHelper<TypeList<Ts...>, TypeList<Us...>> {
			typedef TypeList<Ts..., Us...> type;
		};


		template<typename ...Ts>
		struct TypeListFlatten {
			typedef typename TypeListUnion<typename TypeListFlattenHelper<Ts>::type...>::type type;
		};

		template<typename ...Ts>
		struct TypeListFlattenHelper<TypeList<Ts...>> {
			typedef typename TypeListFlatten<Ts...>::type type;
		};
		template<typename ...Ts>
		struct TypeListFlattenHelper {
			typedef TypeList<Ts...> type;
		};
		template<typename T>
		struct TypeListFlattenHelper<T> {
			typedef T type;
		};

		template <typename ...Ts, typename U, typename ...Us>
		struct TypeListDistinctHelper<TypeList<Ts...>, TypeList<U, Us...>> {

			typedef typename std::conditional<
				is_any<U, Ts...>::value,
				typename TypeListDistinctHelper<TypeList<Ts...>, TypeList<Us...>>::type,
				typename TypeListDistinctHelper<TypeList<Ts..., U>, TypeList<Us...>>::type>::type type;

		};
		template <typename T>
		struct TypeListDistinctHelper<T, TypeList<>> {
			typedef T type;
		};

	}

}