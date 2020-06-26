#include "utility/random.h"

#include <ctime>
#include <limits>

namespace hvn3::utility {

	// Public members

	Random::Random() :
		Random(Random::UInt()) {
	}
	Random::Random(seed_t seed) :
		generator(seed) {
	}

	int Random::NextInt() {

		return generator() % std::numeric_limits<int>::max();

	}
	int Random::NextInt(int maximum) {

		return NextInt() % (maximum + 1);

	}
	int Random::NextInt(int minimum, int maximum) {

		return NextInt() % (maximum + 1 - minimum) + minimum;

	}
	unsigned int Random::NextUInt() {

		return generator() % std::numeric_limits<unsigned int>::max();

	}
	unsigned int Random::NextUInt(unsigned int maximum) {

		return NextUInt() % (maximum + 1);

	}
	unsigned int Random::NextUInt(unsigned int minimum, unsigned int maximum) {

		return NextUInt() % (maximum + 1 - minimum) + minimum;

	}
	bool Random::NextBool() {

		return NextInt() % 2 == 0;

	}
	Random::byte Random::NextByte() {

		return NextUInt() % (std::numeric_limits<byte>::max() + 1);

	}
	void Random::NextBytes(byte* buffer, size_t size) {

		for (size_t i = 0; i < size; ++i)
			*(buffer++) = NextByte();

	}
	char Random::NextChar() {

		return NextInt() % (std::numeric_limits<char>::max() + 1);

	}
	void Random::NextChars(char* buffer, size_t size) {

		for (size_t i = 0; i < size; ++i)
			*(buffer++) = NextChar();

	}
	float Random::NextFloat() {

		return static_cast<float>(generator()) / generator.max();

	}
	float Random::NextFloat(float maximum) {

		return NextFloat() * maximum;

	}
	float Random::NextFloat(float minimum, float maximum) {

		return NextFloat() * (maximum - minimum) + minimum;

	}
	double Random::NextDouble() {

		return static_cast<double>(generator()) / generator.max();

	}
	double Random::NextDouble(double maximum) {

		return NextDouble() * maximum;

	}
	double Random::NextDouble(double minimum, double maximum) {

		return NextDouble() * (maximum - minimum) + minimum;

	}

	bool Random::Chance(float probability) {

		return Random::Float(0.0f, 1.0f) <= probability;

	}

	int Random::Int() {

		return singleton.NextInt();

	}
	int Random::Int(int maximum) {

		return singleton.NextInt(maximum);

	}
	int Random::Int(int minimum, int maximum) {

		return singleton.NextInt(minimum, maximum);

	}
	unsigned int Random::UInt() {

		return singleton.NextUInt();

	}
	unsigned int Random::UInt(unsigned int maximum) {

		return singleton.NextUInt(maximum);

	}
	unsigned int Random::UInt(unsigned int minimum, unsigned int maximum) {

		return singleton.NextUInt(minimum, maximum);

	}
	bool Random::Bool() {

		return singleton.NextBool();

	}
	Random::byte Random::Byte() {

		return singleton.NextByte();

	}
	void Random::Bytes(byte* buffer, size_t size) {

		singleton.NextBytes(buffer, size);

	}
	char Random::Char() {

		return singleton.NextChar();

	}
	void Random::Chars(char* buffer, size_t size) {

		singleton.NextChars(buffer, size);

	}
	float Random::Float() {

		return singleton.NextFloat();

	}
	float Random::Float(float maximum) {

		return singleton.NextFloat(maximum);

	}
	float Random::Float(float minimum, float maximum) {

		return singleton.NextFloat(minimum, maximum);

	}
	double Random::Double() {

		return singleton.NextDouble();

	}
	double Random::Double(double maximum) {

		return singleton.NextDouble(maximum);

	}
	double Random::Double(double minimum, double maximum) {

		return singleton.NextDouble(minimum, maximum);

	}

	// Private members

	Random Random::singleton(static_cast<seed_t>(time(nullptr)));

}