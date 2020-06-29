#pragma once

#include <type_traits>

// Define to quickly enable bitflag operators.

#define HVN3_ENABLE_BITFLAG_OPERATORS(EnumClass) \
template<> struct EnableBitFlagOperators<EnumClass> {  \
	static const bool enable = true;  \
}; \

// Template struct that must be specialized in order to enable bitflag operators.

template<typename Enum>
struct EnableBitFlagOperators {

	static const bool enable = false;

};

// | operator

template<typename Enum>
typename std::enable_if<EnableBitFlagOperators<Enum>::enable, Enum>::type operator|(Enum a, Enum b) {

	using underlying_type = typename std::underlying_type<Enum>::type;
	return static_cast<Enum>(static_cast<underlying_type>(a) | static_cast<underlying_type>(b));

};

template<typename Enum>
typename std::enable_if<EnableBitFlagOperators<Enum>::enable, Enum&>::type operator|=(Enum& a, Enum b) {

	using underlying_type = typename std::underlying_type<Enum>::type;
	a = static_cast<Enum>(static_cast<underlying_type>(a) | static_cast<underlying_type>(b));
	return a;

};

// & operator

template<typename Enum>
typename std::enable_if<EnableBitFlagOperators<Enum>::enable, Enum>::type operator&(Enum a, Enum b) {

	using underlying_type = typename std::underlying_type<Enum>::type;
	return static_cast<Enum>(static_cast<underlying_type>(a) & static_cast<underlying_type>(b));

};

template<typename Enum>
typename std::enable_if<EnableBitFlagOperators<Enum>::enable, Enum&>::type operator&=(Enum& a, Enum b) {

	using underlying_type = typename std::underlying_type<Enum>::type;
	a = static_cast<Enum>(static_cast<underlying_type>(a) & static_cast<underlying_type>(b));
	return a;

};

// ~ operator

template<typename Enum>
typename std::enable_if<EnableBitFlagOperators<Enum>::enable, Enum>::type operator~(Enum a) {

	using underlying_type = typename std::underlying_type<Enum>::type;
	return static_cast<Enum>(~static_cast<underlying_type>(a));

};

// ^ operator

template<typename Enum>
typename std::enable_if<EnableBitFlagOperators<Enum>::enable, Enum>::type operator^(Enum a, Enum b) {

	using underlying_type = typename std::underlying_type<Enum>::type;
	return static_cast<Enum>(static_cast<underlying_type>(a) ^ static_cast<underlying_type>(b));

};

template<typename Enum>
typename std::enable_if<EnableBitFlagOperators<Enum>::enable, Enum&>::type operator^=(Enum& a, Enum b) {

	using underlying_type = typename std::underlying_type<Enum>::type;
	a = static_cast<Enum>(static_cast<underlying_type>(a) ^ static_cast<underlying_type>(b));
	return a;

};

// Returns true if the enum value contains the given flag.
template<typename Enum>
typename std::enable_if<EnableBitFlagOperators<Enum>::enable, bool>::type HasFlag(Enum a, Enum b) {

	using underlying_type = typename std::underlying_type<Enum>::type;

	return (static_cast<underlying_type>(a) & static_cast<underlying_type>(b)) != 0;

}