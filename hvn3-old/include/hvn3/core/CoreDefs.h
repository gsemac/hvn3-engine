#pragma once
#include <cassert>

#define HVN3_BREAK return hvn3::BREAK
#define HVN3_CONTINUE return hvn3::CONTINUE

#define HVN3_ASSERT(expression, message)\
	assert(((void)(message), (expression)));

namespace hvn3 {

	const bool BREAK = false;
	const bool CONTINUE = true;

}