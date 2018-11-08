#pragma once
#include <limits>
#include <ctime>
#include "hvn3/utility/Random.h"

namespace hvn3 {

	Random::Random() : _rand(_default_rand.NextInteger()) {}
	Random::Random(unsigned int seed) : _rand(seed) {}

	int Random::NextInteger() {

		return _rand() % std::numeric_limits<int>::max();

	}
	int Random::NextInteger(int maximum) {

		return NextInteger() % (maximum + 1);

	}
	int Random::NextInteger(int minimum, int maximum) {

		return NextInteger() % (maximum + 1 - minimum) + minimum;

	}
	unsigned int Random::NextUInteger() {

		return _rand() % std::numeric_limits<unsigned int>::max();

	}
	unsigned int Random::NextUInteger(unsigned int maximum) {

		return NextUInteger() % (maximum + 1);

	}
	unsigned int Random::NextUInteger(unsigned int minimum, unsigned int maximum) {

		return NextUInteger() % (maximum + 1 - minimum) + minimum;

	}
	bool Random::NextBoolean() {

		return _rand() % 2 == 0;

	}
	unsigned char Random::NextByte() {

		return _rand() % (std::numeric_limits<unsigned char>::max() + 1);

	}
	void Random::NextBytes(unsigned char* buffer, size_t size) {

		for (size_t i = 0; i < size; ++i)
			*(buffer++) = NextByte();

	}
	char Random::NextChar() {

		return _rand() % (std::numeric_limits<char>::max() + 1);

	}
	void Random::NextChars(char* buffer, size_t size) {

		for (size_t i = 0; i < size; ++i)
			*(buffer++) = NextChar();

	}
	short Random::NextShort() {

		return _rand() % (std::numeric_limits<short>::max() + 1);

	}
	float Random::NextFloat() {

		return static_cast<float>(_rand()) / _rand.max();

	}
	float Random::NextFloat(float maximum) {

		return NextFloat() * maximum;

	}
	float Random::NextFloat(float minimum, float maximum) {

		return NextFloat() * (maximum - minimum) + minimum;

	}
	double Random::NextDouble() {

		return static_cast<double>(_rand()) / _rand.max();

	}
	double Random::NextDouble(double maximum) {

		return NextDouble() * maximum;

	}
	double Random::NextDouble(double minimum, double maximum) {

		return NextDouble() * (maximum - minimum) + minimum;

	}
	int Random::NextSign() {

		return NextBoolean() ? 1 : -1;

	}

	int Random::Integer() {

		return _default_rand.NextInteger();

	}
	int Random::Integer(int maximum) {

		return _default_rand.NextInteger(maximum);

	}
	int Random::Integer(int minimum, int maximum) {

		return _default_rand.NextInteger(minimum, maximum);

	}
	unsigned int Random::UInteger() {

		return _default_rand.NextUInteger();

	}
	unsigned int Random::UInteger(unsigned int maximum) {

		return _default_rand.NextUInteger(maximum);

	}
	unsigned int Random::UInteger(unsigned int minimum, unsigned int maximum) {

		return _default_rand.NextUInteger(minimum, maximum);

	}
	bool Random::Boolean() {

		return _default_rand.NextBoolean();

	}
	unsigned char Random::Byte() {

		return _default_rand.NextByte();

	}
	void Random::Bytes(unsigned char* buffer, size_t size) {

		_default_rand.NextBytes(buffer, size);

	}
	char Random::Char() {

		return _default_rand.NextChar();

	}
	void Random::Chars(char* buffer, size_t size) {

		_default_rand.NextChars(buffer, size);

	}
	unsigned short Random::Short() {

		return _default_rand.NextShort();

	}
	float Random::Float() {

		return _default_rand.NextFloat();

	}
	float Random::Float(float maximum) {

		return _default_rand.NextFloat(maximum);

	}
	float Random::Float(float minimum, float maximum) {

		return _default_rand.NextFloat(minimum, maximum);

	}
	double Random::Double() {

		return _default_rand.NextDouble();

	}
	double Random::Double(double maximum) {

		return _default_rand.NextDouble(maximum);

	}
	double Random::Double(double minimum, double maximum) {

		return _default_rand.NextDouble(minimum, maximum);

	}
	int Random::Sign() {

		return _default_rand.NextSign();

	}

	bool Random::TestChance(float p) {

		return 0; // Random::Float(0.0f, 1.0f) <= p;

	}

	Random Random::_default_rand(static_cast<unsigned int>(time(NULL)));

}