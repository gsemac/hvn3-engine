#pragma once

#include "hvn3/iterators/reverse_iterator.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iterator>
#include <iostream>
#include <type_traits>

#include <allegro5/utf8.h>

namespace hvn3 {
	namespace allegro {

		template<bool IsConst = false>
		class AllegroUstrIterator {

			using ustr_pointer_type = typename std::conditional<IsConst, const ALLEGRO_USTR*, ALLEGRO_USTR*>::type;
			using iterator = AllegroUstrIterator<false>;
			using const_iterator = AllegroUstrIterator<true>;

		public:
			struct CharProxy {

				friend class AllegroUstrIterator;

				using value_type = int32_t;

				CharProxy(ustr_pointer_type ustr, int index) {

					this->ustr = ustr;
					this->index = index;

				}

				template<typename T = ustr_pointer_type>
				typename std::enable_if<!std::IsConst<T>::value, CharProxy&>::type operator=(value_type value) {

					assert(ustr != nullptr);
					assert(index >= 0);

					al_ustr_set_chr(ustr, index, value);

					return *this;

				}

				operator value_type() const {

					assert(ustr != nullptr);
					assert(index >= 0);

					return al_ustr_get(ustr, index);

				}

				bool operator==(const CharProxy& other) const {

					return static_cast<value_type>(*this) == static_cast<value_type>(other);

				}
				bool operator!=(const CharProxy& other) const {

					return !(*this == other);

				}

				friend std::ostream& operator<<(std::ostream& lhs, const CharProxy& rhs) {

					lhs << static_cast<value_type>(rhs);

					return lhs;

				}

			private:
				ustr_pointer_type ustr;
				int index;

			};

			using difference_type = int;
			using value_type = CharProxy;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::bidirectional_iterator_tag;

			friend class iterator;
			friend class const_iterator;

			AllegroUstrIterator(ustr_pointer_type ustr, int index) :
				_proxy(ustr, index) {
			}
			AllegroUstrIterator(const AllegroUstrIterator& other) :
				_proxy(other._proxy.ustr, other._proxy.index) {
			}

			bool operator==(const AllegroUstrIterator& other) const {

				assert(_proxy.ustr == other._proxy.ustr);

				return _proxy.index == other._proxy.index;

			}
			bool operator!=(const AllegroUstrIterator& other) const {

				return !(*this == other);

			}

			AllegroUstrIterator& operator++() {

				increment();

				return *this;

			}
			AllegroUstrIterator operator++(int) {

				AllegroUstrIterator before = *this;

				++* this;

				return before;

			}
			AllegroUstrIterator& operator--() {

				decrement();

				return *this;

			}
			AllegroUstrIterator operator--(int) {

				AllegroUstrIterator before = *this;

				--* this;

				return before;

			}
			AllegroUstrIterator operator+(difference_type value) const {

				AllegroUstrIterator iter = *this;

				iter += value;

				return iter;

			}
			AllegroUstrIterator& operator+=(difference_type value) {

				advance(value);

				return *this;

			}
			AllegroUstrIterator operator-(difference_type value) const {

				AllegroUstrIterator iter = *this;

				iter -= value;

				return iter;

			}
			AllegroUstrIterator& operator-=(difference_type value) {

				advance(-value);

				return *this;

			}

			pointer operator->() {

				return &_proxy;

			}
			const value_type* operator->() const {

				return &_proxy;

			}
			reference operator*() {

				return _proxy;

			}
			const CharProxy& operator*() const {

				return _proxy;

			}
			reference operator[](difference_type index) {

				return *(*this + index);

			}
			const CharProxy& operator[](difference_type index) const {

				return *(*this + index);

			}

		private:
			CharProxy _proxy;

			void increment() {

				al_ustr_next(_proxy.ustr, &_proxy.index);

			}
			void decrement() {

				al_ustr_prev(_proxy.ustr, &_proxy.index);

			}
			void advance(difference_type value) {

				if (value > 0) {

					for (auto i = 0; i < value; ++i)
						increment();

				}
				else if (value < 0) {

					for (auto i = 0; i < (std::abs)(value); ++i)
						decrement();

				}

			}

		};

		using al_ustr_iterator = AllegroUstrIterator<false>;
		using al_ustr_const_iterator = AllegroUstrIterator<true>;
		using al_ustr_reverse_iterator = ReverseIterator<AllegroUstrIterator<false>>;
		using al_ustr_const_reverse_iterator = ReverseIterator<AllegroUstrIterator<true>>;

		al_ustr_iterator al_ustr_begin(ALLEGRO_USTR* ustr);
		al_ustr_iterator al_ustr_end(ALLEGRO_USTR* ustr);
		al_ustr_const_iterator al_ustr_begin(const ALLEGRO_USTR* ustr);
		al_ustr_const_iterator al_ustr_end(const ALLEGRO_USTR* ustr);
		al_ustr_const_iterator al_ustr_cbegin(const ALLEGRO_USTR* ustr);
		al_ustr_const_iterator al_ustr_cend(const ALLEGRO_USTR* ustr);
		al_ustr_reverse_iterator al_ustr_rbegin(ALLEGRO_USTR* ustr);
		al_ustr_reverse_iterator al_ustr_rend(ALLEGRO_USTR* ustr);
		al_ustr_const_reverse_iterator al_ustr_rbegin(const ALLEGRO_USTR* ustr);
		al_ustr_const_reverse_iterator al_ustr_rend(const ALLEGRO_USTR* ustr);
		al_ustr_const_reverse_iterator al_ustr_crbegin(const ALLEGRO_USTR* ustr);
		al_ustr_const_reverse_iterator al_ustr_crend(const ALLEGRO_USTR* ustr);

	}
}