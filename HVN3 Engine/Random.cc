#pragma once
#include <limits>
#include <ctime>
#include "Random.h"

Random::Random() : __rand(__default_rand.NextInteger()) {}
Random::Random(unsigned int seed) : __rand(seed) {}

int Random::NextInteger() {

	return __rand() % std::numeric_limits<int>::max();

}
int Random::NextInteger(int maximum) {

	return NextInteger() % (maximum + 1);

}
int Random::NextInteger(int minimum, int maximum) {

	return NextInteger() % (maximum + 1 - minimum) + minimum;

}
unsigned int Random::NextUInteger() {

	return __rand() % std::numeric_limits<unsigned int>::max();

}
unsigned int Random::NextUInteger(unsigned int maximum) {

	return NextUInteger() % (maximum + 1);

}
unsigned int Random::NextUInteger(unsigned int minimum, unsigned int maximum) {

	return NextUInteger() % (maximum + 1 - minimum) + minimum;

}
bool Random::NextBoolean() {

	return __rand() % 2 == 0;

}
unsigned char Random::NextByte() {

	return __rand() % (std::numeric_limits<unsigned char>::max() + 1);

}
void Random::NextBytes(unsigned char* buffer, size_t size) {

	for (size_t i = 0; i < size; ++i)
		*(buffer++) = NextByte();

}
char Random::NextChar() {

	return __rand() % (std::numeric_limits<char>::max() + 1);

}
void Random::NextChars(char* buffer, size_t size) {

	for (size_t i = 0; i < size; ++i)
		*(buffer++) = NextChar();

}
short Random::NextShort() {

	return __rand() % (std::numeric_limits<short>::max() + 1);

}
float Random::NextFloat() {

	return static_cast<float>(__rand()) / __rand.max();

}
float Random::NextFloat(float maximum) {

	return NextFloat() * maximum;

}
float Random::NextFloat(float minimum, float maximum) {

	return NextFloat() * (maximum - minimum) + minimum;

}
double Random::NextDouble() {

	return static_cast<double>(__rand()) / __rand.max();

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

	return __default_rand.NextInteger();

}
int Random::Integer(int maximum) {

	return __default_rand.NextInteger(maximum);

}
int Random::Integer(int minimum, int maximum) {

	return __default_rand.NextInteger(minimum, maximum);

}
unsigned int Random::UInteger() {

	return __default_rand.NextUInteger();

}
unsigned int Random::UInteger(unsigned int maximum) {

	return __default_rand.NextUInteger(maximum);

}
unsigned int Random::UInteger(unsigned int minimum, unsigned int maximum) {

	return __default_rand.NextUInteger(minimum, maximum);

}
bool Random::Boolean() {

	return __default_rand.NextBoolean();

}
unsigned char Random::Byte() {

	return __default_rand.NextByte();

}
void Random::Bytes(unsigned char* buffer, size_t size) {

	__default_rand.NextBytes(buffer, size);

}
char Random::Char() {

	return __default_rand.NextChar();

}
void Random::Chars(char* buffer, size_t size) {

	__default_rand.NextChars(buffer, size);

}
unsigned short Random::Short() {

	return __default_rand.NextShort();

}
float Random::Float() {

	return __default_rand.NextFloat();

}
float Random::Float(float maximum) {

	return __default_rand.NextFloat(maximum);

}
float Random::Float(float minimum, float maximum) {

	return __default_rand.NextFloat(minimum, maximum);

}
double Random::Double() {

	return __default_rand.NextDouble();

}
double Random::Double(double maximum) {

	return __default_rand.NextDouble(maximum);

}
double Random::Double(double minimum, double maximum) {

	return __default_rand.NextDouble(minimum, maximum);

}
int Random::Sign() {

	return __default_rand.NextSign();

}

bool Random::TestChance(float p) {

	return 0; // Random::Float(0.0f, 1.0f) <= p;

}

Random Random::__default_rand(static_cast<unsigned int>(time(NULL)));