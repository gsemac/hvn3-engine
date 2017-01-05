#pragma once
#include <random>
#include <cassert>

class Random {

public:
	Random();
	Random(unsigned int seed);

	int NextInteger();
	int NextInteger(int maximum);
	int NextInteger(int minimum, int maximum);
	unsigned int NextUInteger();
	unsigned int NextUInteger(unsigned int maximum);
	unsigned int NextUInteger(unsigned int minimum, unsigned int maximum);
	bool NextBoolean();
	unsigned char NextByte();
	void NextBytes(unsigned char* buffer, size_t size);
	char NextChar();
	void NextChars(char* buffer, size_t size);
	short NextShort();
	float NextFloat();
	float NextFloat(float maximum);
	float NextFloat(float minimum, float maximum);
	double NextDouble();
	double NextDouble(double maximum);
	double NextDouble(double minimum, double maximum);
	int NextSign();

	static int Integer();
	static int Integer(int maximum);
	static int Integer(int minimum, int maximum);
	static unsigned int UInteger();
	static unsigned int UInteger(unsigned int maximum);
	static unsigned int UInteger(unsigned int minimum, unsigned int maximum);
	static bool Boolean();
	static unsigned char Byte();
	static void Bytes(unsigned char* buffer, size_t size);
	static char Char();
	static void Chars(char* buffer, size_t size);
	static unsigned short Short();
	static float Float();
	static float Float(float maximum);
	static float Float(float minimum, float maximum);
	static double Double();
	static double Double(double maximum);
	static double Double(double minimum, double maximum);
	static int Sign();

	// Returns true with probability 1/p.
	static bool TestChance(float p);
	// 
	template <typename T>
	static T Choose(const std::initializer_list<T>& items) {

		// Ensure the number of items is greater than 0.
		assert(items.size() > 0);

		// Return a randomly-selected item.
		return *(items.begin() + Integer(0, -1 + items.size()));

	}
	template <typename T>
	static T Choose(T* items, size_t size) {

		// Ensure that the given size is greater than 0.
		assert(size > 0);

		// Return a randomly-selected item.
		return *(items + Integer(0, -1 + size));

	}

private:
	// The generator used for static function calls.
	static Random __default_rand;
	// Generator used for class instances.
	std::mt19937 __rand;

};