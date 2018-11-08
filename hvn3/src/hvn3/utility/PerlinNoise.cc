#include "hvn3/utility/PerlinNoise.h"

namespace hvn3 {

	PerlinNoise::PerlinNoise() {

		_init();
		_generatePermutationVector();

	}
	PerlinNoise::PerlinNoise(std::default_random_engine::result_type seed) {

		_init();
		_generatePermutationVector(seed);

	}
	void PerlinNoise::SetSeed(std::default_random_engine::result_type seed) {

		_generatePermutationVector(seed);

	}
	void PerlinNoise::SetScale(result_type min, result_type max) {

		_scale_min = min;
		_scale_max = max;

	}
	PerlinNoise::result_type PerlinNoise::Noise(result_type x) {
		return Noise(x, 0.0, 0.0);
	}
	PerlinNoise::result_type PerlinNoise::Noise(result_type x, result_type y) {
		return Noise(x, y, 0.0);
	}
	PerlinNoise::result_type PerlinNoise::Noise(result_type x, result_type y, result_type z) {

		int32_t xi = static_cast<int32_t>(x) & 255;
		int32_t yi = static_cast<int32_t>(y) & 255;
		int32_t zi = static_cast<int32_t>(z) & 255;

		// Calculate the decimal offset of the coordinate in the unit cube.

		result_type xf = x - static_cast<int32_t>(x);
		result_type yf = y - static_cast<int32_t>(y);
		result_type zf = z - static_cast<int32_t>(z);

		result_type u = _fade(xf);
		result_type v = _fade(yf);
		result_type w = _fade(zf);

		// Hash all 8 coordinates of the unit cube.

		int32_t aaa, aba, aab, abb, baa, bba, bab, bbb;

		aaa = _p[_p[_p[xi] + yi] + zi];
		aba = _p[_p[_p[xi] + _inc(yi)] + zi];
		aab = _p[_p[_p[xi] + yi] + _inc(zi)];
		abb = _p[_p[_p[xi] + _inc(yi)] + _inc(zi)];
		baa = _p[_p[_p[_inc(xi)] + yi] + zi];
		bba = _p[_p[_p[_inc(xi)] + _inc(yi)] + zi];
		bab = _p[_p[_p[_inc(xi)] + yi] + _inc(zi)];
		bbb = _p[_p[_p[_inc(xi)] + _inc(yi)] + _inc(zi)];

		result_type x1, x2, y1, y2;

		x1 = _lerp(_grad(aaa, xf, yf, zf), _grad(baa, xf - 1, yf, zf), u);
		x2 = _lerp(_grad(aba, xf, yf - 1, zf), _grad(bba, xf - 1, yf - 1, zf), u);

		y1 = _lerp(x1, x2, v);

		x1 = _lerp(_grad(aab, xf, yf, zf - 1), _grad(bab, xf - 1, yf, zf - 1), u);
		x2 = _lerp(_grad(abb, xf, yf - 1, zf - 1), _grad(bbb, xf - 1, yf - 1, zf - 1), u);

		y2 = _lerp(x1, x2, v);

		// Bound the result to [0, 1] for convenience (instead of [-1, 1]).
		result_type result = static_cast<result_type>((_lerp(y1, y2, w) + 1.0) / 2.0);

		// Bound the result to the user-provided range.
		result = (result * (_scale_max - _scale_min)) + _scale_min;

		return result;

	}
	PerlinNoise::result_type PerlinNoise::OctaveNoise(result_type x, int32_t octaves) {
		return OctaveNoise(x, 0.0, 0.0, octaves);
	}
	PerlinNoise::result_type PerlinNoise::OctaveNoise(result_type x, result_type y, int32_t octaves) {
		return OctaveNoise(x, y, 0.0, octaves);
	}
	PerlinNoise::result_type PerlinNoise::OctaveNoise(result_type x, result_type y, result_type z, int32_t octaves) {
		return OctaveNoise(x, y, z, octaves, 1.0, 1.0, 1.0);
	}
	PerlinNoise::result_type PerlinNoise::OctaveNoise(result_type x, result_type y, result_type z, int32_t octaves, result_type frequency, result_type amplitude, result_type persistence) {

		result_type result = 0.0;
		result_type max_value = 0.0; // Used to normalize the result to [0, 1]

		for (int32_t i = 0; i < octaves; ++i) {

			result += Noise(x * frequency, y * frequency, z * frequency) * amplitude;

			max_value += amplitude;

			amplitude *= persistence;
			frequency *= 2.0;

		}

		return (result / max_value);

	}


	PerlinNoise::result_type PerlinNoise::_fade(result_type t) {

		// The fade function as defined by Perlin:
		// 6t^5 - 15t^4 + 10t^3

		return static_cast<result_type>(t * t * t * (t * (t * 6.0 - 15.0) + 10.0));

	}
	int PerlinNoise::_inc(int32_t n) {

		return ++n;

	}
	PerlinNoise::result_type PerlinNoise::_grad(int32_t hash, result_type x, result_type y, result_type z) {

		// Optimization from:
		//	http://riven8192.blogspot.com/2010/08/calculate-perlinnoise-twice-as-fast.html

		switch (hash & 0xF) {

		case 0x0:
			return  x + y;
		case 0x1:
			return -x + y;
		case 0x2:
			return  x - y;
		case 0x3:
			return -x - y;
		case 0x4:
			return  x + z;
		case 0x5:
			return -x + z;
		case 0x6:
			return  x - z;
		case 0x7:
			return -x - z;
		case 0x8:
			return  y + z;
		case 0x9:
			return -y + z;
		case 0xA:
			return  y - z;
		case 0xB:
			return -y - z;
		case 0xC:
			return  y + x;
		case 0xD:
			return -y + z;
		case 0xE:
			return  y - x;
		case 0xF:
			return -y - z;

		default: // (should never happen)
			return 0;

		}

	}
	PerlinNoise::result_type PerlinNoise::_lerp(result_type a, result_type b, result_type x) {

		return a + x * (b - a);

	}

	void PerlinNoise::_init() {

		_scale_min = 0.0f;
		_scale_max = 1.0f;

	}
	void PerlinNoise::_generatePermutationVector() {

		std::copy(_permutation.begin(), _permutation.end(), _p.begin());
		std::copy(_permutation.begin(), _permutation.end(), _p.begin() + 256);

	}
	void PerlinNoise::_generatePermutationVector(std::default_random_engine::result_type seed) {

		std::default_random_engine rng(seed);

		std::iota(_p.begin(), _p.end(), 0);
		std::shuffle(_p.begin(), _p.begin() + 255, std::default_random_engine{});
		std::copy(_p.begin(), _p.begin() + 256, _p.begin() + 256);

	}

	std::array<int32_t, 256> PerlinNoise::_permutation = {
		151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225,
		140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148,
		247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
		57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175,
		74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122,
		60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54,
		65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
		200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64,
		52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212,
		207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213,
		119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
		129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104,
		218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
		81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157,
		184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93,
		222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
	};

}