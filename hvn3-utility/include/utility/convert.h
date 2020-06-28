#pragma once

#include "utility/string_utilities.h"

#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>
#include <type_traits>

namespace hvn3::utility {

	class Convert {

	private:

		// Generic converter

		template <typename ToType, typename FromType, typename DummyType = void>
		struct InternalConverter {

			static ToType Convert(const FromType& input) {

				static_assert(false, "No valid conversion exists between the given types.");

			}
			static bool TryConvert(const FromType& input, ToType& output) {

				return false;

			}

		};

		template <typename ToType, typename FromType>
		struct InternalConverter<ToType, FromType, typename std::enable_if<std::is_convertible<FromType, ToType>::value>::type> {

			static ToType Convert(const FromType& input) {

				return static_cast<ToType>(input);

			}
			static bool TryConvert(const FromType& input, ToType& output) {

				output = std::move(Convert(input));

				return true;

			}

		};

		// String converter

		template <typename FromType>
		struct InternalConverter<std::string, FromType, typename std::enable_if<!std::is_convertible<FromType, std::string>::value>::type> {

			static std::string Convert(const FromType& input) {

				std::stringstream ss;

				ss << input;

				return ss.str();

			}

		};

		template <typename ToType>
		struct InternalConverter<ToType, std::string, typename std::enable_if<std::is_arithmetic<ToType>::value>::type> {

			static ToType Convert(const std::string& input) {

				ToType output;

				assert(TryConvert(input, output));

				return output;

			}
			static bool TryConvert(const std::string& input, ToType& output) {

				std::stringstream reader;

				size_t startIndex = 0;
				bool enableShorthandHex = false;

				if (StartsWith(input, "0x", StringComparison::OrdinalIgnoreCase)) {

					reader << std::hex;

					startIndex = 2;

				}
				else if (StartsWith(input, '#')) {

					reader << std::hex;

					startIndex = 1;

					// Enable shorthand notation for hex strings (e.g. "#f60" = "#ff6600").
					// Only do this for hex strings beginning with "#", because this is likely undesirable in other contexts.

					if (input.length() - startIndex == 3)
						enableShorthandHex = true;

				}
				else if (StartsWith(input, '0')) {

					reader << std::oct;

				}

				reader << input.substr(startIndex, input.length() - startIndex);

				if (enableShorthandHex) {

					uint32_t shorthand;
					uint32_t expanded = 0;

					reader >> shorthand;

					expanded |= (shorthand & 0xF);
					expanded |= (shorthand & 0xF) << 4;
					expanded |= (shorthand >> 4 & 0xF) << 8;
					expanded |= (shorthand >> 4 & 0xF) << 12;
					expanded |= (shorthand >> 8 & 0xF) << 16;
					expanded |= (shorthand >> 8 & 0xF) << 20;

					output = static_cast<ToType>(expanded);

				}
				else {

					reader >> output;

				}

				return !reader.fail();

			}

		};

		template <typename ToType>
		struct InternalConverter<ToType, const char*, typename std::enable_if<!std::is_convertible<const char*, ToType>::value>::type> {

			static ToType Convert(const char* input) {

				return InternalConverter<ToType, std::string>::Convert(input);

			}
			static bool TryConvert(const char* input, ToType& output) {

				return InternalConverter<ToType, std::string>::TryConvert(input, output);

			}

		};

		template <typename ToType, std::size_t Length>
		struct InternalConverter<ToType, char[Length], typename std::enable_if<!std::is_convertible<char[Length], ToType>::value>::type> {

			static ToType Convert(const char input[Length]) {

				return InternalConverter<ToType, std::string>::Convert(input);

			}
			static bool TryConvert(const char* input, ToType& output) {

				return InternalConverter<ToType, std::string>::TryConvert(input, output);

			}

		};

	public:
		template <typename ToType, typename FromType>
		static ToType To(const FromType& input) {

			return InternalConverter<ToType, FromType>::Convert(input);

		}
		template <typename ToType, typename FromType>
		static bool To(const FromType& input, ToType& output) {

			return InternalConverter<ToType, FromType>::TryConvert(input, output);

		}

		template<typename FromType>
		static std::string ToString(const FromType& input) {

			return To<std::string>(input);

		}

	};

}