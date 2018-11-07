#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>
#include <random>

namespace hvn3 {

	// Implementation adapted from:
	//	https://flafla2.github.io/2014/08/09/perlinnoise.html

	// Perlin's Java implementation, on which the above is based, is available here:
	//	https://mrl.nyu.edu/~perlin/noise/

	class PerlinNoise {

		typedef float result_type;

	public:
		PerlinNoise();
		PerlinNoise(std::default_random_engine::result_type seed);

		void SetSeed(std::default_random_engine::result_type seed);

		result_type Noise(result_type x);
		result_type Noise(result_type x, result_type y);
		result_type Noise(result_type x, result_type y, result_type z);

		result_type OctaveNoise(result_type x, int32_t octaves);
		result_type OctaveNoise(result_type x, result_type y, int32_t octaves);
		result_type OctaveNoise(result_type x, result_type y, result_type z, int32_t octaves);
		result_type OctaveNoise(result_type x, result_type y, result_type z, int32_t octaves, result_type frequency, result_type amplitude, result_type persistence);

	private:
		// Array of all integers from 0-255, twice.
		std::array<int32_t, 512> _p;

		// Array of all integers from 0-255, as defined by Perlin.
		// https://mrl.nyu.edu/~perlin/noise/
		static std::array<int32_t, 256> _permutation;

		static result_type _fade(result_type t);
		static int _inc(int32_t n);
		static result_type _grad(int32_t hash, result_type x, result_type y, result_type z);
		static result_type _lerp(result_type a, result_type b, result_type x);

		void _generatePermutationVector();
		void _generatePermutationVector(std::default_random_engine::result_type seed);

	};

}