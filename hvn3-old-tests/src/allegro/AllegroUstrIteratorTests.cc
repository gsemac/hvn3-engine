#include "hvn3/allegro/allegro_ustr_iterator.h"

#include <cctype>
#include <string>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace hvn3::allegro;

namespace hvn3test {

	TEST_CLASS(AllegroUstrIteratorTests) {

public:
	TEST_METHOD(TestForwardIteration) {

		ALLEGRO_USTR* ustr = al_ustr_new("hello world");
		std::string copy = "";

		std::copy(al_ustr_begin(ustr), al_ustr_end(ustr), std::back_inserter(copy));

		Assert::AreEqual(std::string("hello world"), copy);

	}
	TEST_METHOD(TestForwardIterationWithModification) {

		ALLEGRO_USTR* ustr = al_ustr_new("hello world");

		for (auto i = al_ustr_begin(ustr); i != al_ustr_end(ustr); ++i)
			*i = std::toupper(*i);

		std::string copy = "";

		std::copy(al_ustr_begin(ustr), al_ustr_end(ustr), std::back_inserter(copy));

		Assert::AreEqual(std::string("HELLO WORLD"), copy);

	}
	TEST_METHOD(TestForwardIterationWithEmptyString) {

		ALLEGRO_USTR* ustr = al_ustr_new("");
		std::string copy = "";

		std::copy(al_ustr_begin(ustr), al_ustr_end(ustr), std::back_inserter(copy));

		Assert::AreEqual(std::string(""), copy);

	}
	TEST_METHOD(TestForwardIterationWithNullString) {

		ALLEGRO_USTR* ustr = nullptr;
		std::string copy = "";

		std::copy(al_ustr_begin(ustr), al_ustr_end(ustr), std::back_inserter(copy));

		Assert::AreEqual(std::string(""), copy);

	}
	TEST_METHOD(TestForwardIterationWithConstString) {

		const ALLEGRO_USTR* ustr = al_ustr_new("hello world");
		std::string copy = "";

		std::copy(al_ustr_begin(ustr), al_ustr_end(ustr), std::back_inserter(copy));

		Assert::AreEqual(std::string("hello world"), copy);

	}
	TEST_METHOD(TestReverseIteration) {

		ALLEGRO_USTR* ustr = al_ustr_new("hello world");
		std::string copy = "";

		std::copy(al_ustr_rbegin(ustr), al_ustr_rend(ustr), std::back_inserter(copy));

		Assert::AreEqual(std::string("dlrow olleh"), copy);

	}
	TEST_METHOD(TestReverseIterationWithEmptyString) {

		ALLEGRO_USTR* ustr = al_ustr_new("");
		std::string copy = "";

		std::copy(al_ustr_rbegin(ustr), al_ustr_rend(ustr), std::back_inserter(copy));

		Assert::AreEqual(std::string(""), copy);

	}
	TEST_METHOD(TestReverseIterationWithConstString) {

		const ALLEGRO_USTR* ustr = al_ustr_new("hello world");
		std::string copy = "";

		std::copy(al_ustr_rbegin(ustr), al_ustr_rend(ustr), std::back_inserter(copy));

		Assert::AreEqual(std::string("dlrow olleh"), copy);

	}

	};

}