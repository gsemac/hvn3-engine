#pragma once

#include "core/type_traits.h"

#include <type_traits>

namespace hvn3 {

	namespace {

		template<typename T, typename U>
		struct TypeListCat;

		template<typename ...Ts> struct TypeListUnion;
		template<typename ...Ts> struct TypeListUnionHelper;
		template<typename ...Ts> struct TypeListFlatten;
		template<typename ...Ts> struct TypeListFlattenHelper;
		template<typename T, typename U> struct TypeListDistinctHelper;

	}

	namespace core {

		template<typename... Types>
		struct TypeList {

			template<typename ...Ts>
			using append_type = TypeList<Types..., Ts...>;
			template<typename ...Ts>
			using union_type = typename TypeListUnion<TypeList<Types...>, Ts...>::type;
			using flatten_type = typename TypeListFlatten<Types...>::type;
			using distinct_type = typename TypeListDistinctHelper<TypeList<>, TypeList<Types...>>::type;

			template<typename T>
			struct contains {
				static constexpr bool value = is_any<T, Types...>::value;
			};

		};

	}

	namespace {

		template<typename ...Ts, typename ...Us>
		struct TypeListCat<core::TypeList<Ts...>, core::TypeList<Us...>> {
			typedef core::TypeList<Ts..., Us...> type;
		};

		template<typename T1, typename T2, typename ...Ts>
		struct TypeListUnion<T1, T2, Ts...> {
			typedef typename TypeListUnion<typename TypeListUnionHelper<T1, T2>::type, Ts...>::type type;
		};
		template<typename ...Ts>
		struct TypeListUnion<core::TypeList<Ts...>> {
			typedef core::TypeList<Ts...> type;
		};
		template<typename T>
		struct TypeListUnion<T> {
			typedef core::TypeList<T> type;
		};
		template<>
		struct TypeListUnion<> {
			typedef core::TypeList<> type;
		};

		template<typename T, typename U>
		struct TypeListUnionHelper<T, U> {
			typedef core::TypeList<T, U> type;
		};
		template<typename ...Ts, typename U>
		struct TypeListUnionHelper<core::TypeList<Ts...>, U> {
			typedef core::TypeList<Ts..., U> type;
		};
		template<typename T, typename ...Us>
		struct TypeListUnionHelper<T, core::TypeList<Us...>> {
			typedef core::TypeList<T, Us...> type;
		};
		template<typename ...Ts, typename ...Us>
		struct TypeListUnionHelper<core::TypeList<Ts...>, core::TypeList<Us...>> {
			typedef core::TypeList<Ts..., Us...> type;
		};


		template<typename ...Ts>
		struct TypeListFlatten {
			typedef typename TypeListUnion<typename TypeListFlattenHelper<Ts>::type...>::type type;
		};

		template<typename ...Ts>
		struct TypeListFlattenHelper<core::TypeList<Ts...>> {
			typedef typename TypeListFlatten<Ts...>::type type;
		};
		template<typename ...Ts>
		struct TypeListFlattenHelper {
			typedef core::TypeList<Ts...> type;
		};
		template<typename T>
		struct TypeListFlattenHelper<T> {
			typedef T type;
		};

		template <typename ...Ts, typename U, typename ...Us>
		struct TypeListDistinctHelper<core::TypeList<Ts...>, core::TypeList<U, Us...>> {

			typedef typename std::conditional<
				core::is_any<U, Ts...>::value,
				typename TypeListDistinctHelper<core::TypeList<Ts...>, core::TypeList<Us...>>::type,
				typename TypeListDistinctHelper<core::TypeList<Ts..., U>, core::TypeList<Us...>>::type>::type type;

		};
		template <typename T>
		struct TypeListDistinctHelper<T, core::TypeList<>> {
			typedef T type;
		};

	}

}