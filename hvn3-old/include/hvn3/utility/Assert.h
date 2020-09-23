#pragma once
#include <cstdlib>
#include <iostream>

#ifdef HVN3_DEBUG
#define HVN3_ASSERT(EXPRESSION, MESSAGE) \
	if(!(EXPRESSION)) { \
		std::cerr << "Assertion failed: " << MESSAGE << '\n'\
		<< "Expected: " << #EXPRESSION << '\n'\
		<< "File: " << __FILE__ << '\n'\
		<< "Line: " << __LINE__ << '\n';\
		abort();\
	}
#else
#define HVN3_ASSERT(EXPRESSION, MESSAGE)
#endif