#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <random>

namespace hvn3::core {

	class Random {

		using byte = uint8_t;
		using seed_t = uint32_t;

	public:
		Random();
		Random(seed_t seed);

		int NextInt();
		int NextInt(int maximum);
		int NextInt(int minimum, int maximum);
		unsigned int NextUInt();
		unsigned int NextUInt(unsigned int maximum);
		unsigned int NextUInt(unsigned int minimum, unsigned int maximum);
		bool NextBool();
		byte NextByte();
		void NextBytes(byte* buffer, size_t size);
		char NextChar();
		void NextChars(char* buffer, size_t size);
		float NextFloat();
		float NextFloat(float maximum);
		float NextFloat(float minimum, float maximum);
		double NextDouble();
		double NextDouble(double maximum);
		double NextDouble(double minimum, double maximum);

		bool Chance(float probability);

		template<typename Iter>
		const typename Iter::reference Choose(Iter begin, Iter end) {

			using difference_type = typename Iter::difference_type;

			difference_type diff = end - begin;
			difference_type randomOffset = static_cast<difference_type>(NextUInt(0, static_cast<unsigned int>(diff)));

			assert(randomOffset >= 0);
			assert(randomOffset < diff);

			return *(begin + randomOffset);

		}

		static int Int();
		static int Int(int maximum);
		static int Int(int minimum, int maximum);
		static unsigned int UInt();
		static unsigned int UInt(unsigned int maximum);
		static unsigned int UInt(unsigned int minimum, unsigned int maximum);
		static bool Bool();
		static unsigned char Byte();
		static void Bytes(unsigned char* buffer, size_t size);
		static char Char();
		static void Chars(char* buffer, size_t size);
		static float Float();
		static float Float(float maximum);
		static float Float(float minimum, float maximum);
		static double Double();
		static double Double(double maximum);
		static double Double(double minimum, double maximum);

	private:
		static Random singleton;

		std::mt19937 generator;

	};

}