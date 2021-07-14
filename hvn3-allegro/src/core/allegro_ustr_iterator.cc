#include "core/allegro_ustr_iterator.h"

namespace hvn3 {

	al_ustr_iterator al_ustr_begin(ALLEGRO_USTR* ustr) {

		return al_ustr_iterator(ustr, 0);

	}
	al_ustr_iterator al_ustr_end(ALLEGRO_USTR* ustr) {

		return al_ustr_iterator(ustr, al_ustr_length(ustr));

	}
	al_ustr_const_iterator al_ustr_begin(const ALLEGRO_USTR* ustr) {

		return al_ustr_cbegin(ustr);

	}
	al_ustr_const_iterator al_ustr_end(const ALLEGRO_USTR* ustr) {

		return al_ustr_cend(ustr);

	}
	al_ustr_const_iterator al_ustr_cbegin(const ALLEGRO_USTR* ustr) {

		return al_ustr_const_iterator(ustr, 0);

	}
	al_ustr_const_iterator al_ustr_cend(const ALLEGRO_USTR* ustr) {

		return al_ustr_const_iterator(ustr, al_ustr_length(ustr));

	}
	al_ustr_reverse_iterator al_ustr_rbegin(ALLEGRO_USTR* ustr) {

		return hvn3::iterators::make_reverse_iterator(al_ustr_end(ustr));

	}
	al_ustr_reverse_iterator al_ustr_rend(ALLEGRO_USTR* ustr) {

		return hvn3::iterators::make_reverse_iterator(al_ustr_begin(ustr));

	}
	al_ustr_const_reverse_iterator al_ustr_rbegin(const ALLEGRO_USTR* ustr) {

		return al_ustr_crbegin(ustr);

	}
	al_ustr_const_reverse_iterator al_ustr_rend(const ALLEGRO_USTR* ustr) {

		return al_ustr_crend(ustr);

	}
	al_ustr_const_reverse_iterator al_ustr_crbegin(const ALLEGRO_USTR* ustr) {

		return hvn3::iterators::make_reverse_iterator(al_ustr_end(ustr));

	}
	al_ustr_const_reverse_iterator al_ustr_crend(const ALLEGRO_USTR* ustr) {

		return hvn3::iterators::make_reverse_iterator(al_ustr_begin(ustr));

	}

}