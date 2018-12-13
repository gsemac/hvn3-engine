#pragma once

#include "hvn3/utility/BitFlags.h"

namespace hvn3 {

	enum class MouseButton {
		Left = 0x01,
		Middle = 0x02,
		Right = 0x04
	};
	ENABLE_BITFLAG_OPERATORS(MouseButton);

	enum class MouseScrollDirection {
		Up = 1,
		Down = 2,
		Left = 4,
		Right = 8
	};
	ENABLE_BITFLAG_OPERATORS(MouseScrollDirection);

	enum class SystemCursor {
		None = 0,
		Default = 1,
		Arrow = 2,
		Busy = 3,
		Question = 4,
		Edit = 5,
		Move = 6,
		ResizeN = 7,
		ResizeW = 8,
		ResizeS = 9,
		ResizeE = 10,
		ResizeNW = 11,
		ResizeSW = 12,
		ResizeSE = 13,
		ResizeNE = 14,
		Progress = 15,
		Precision = 16,
		Link = 17,
		AltSelect = 18,
		Unavailable = 19
	};

	enum class Key {

		A = 1,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		D0,
		D1,
		D2,
		D3,
		D4,
		D5,
		D6,
		D7,
		D8,
		D9,

		KeyPad0,
		KeyPad1,
		KeyPad2,
		KeyPad3,
		KeyPad4,
		KeyPad5,
		KeyPad6,
		KeyPad7,
		KeyPad8,
		KeyPad9,

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		Escape,
		Tilde,
		Minus,
		Equals,
		Backspace,
		Tab,
		OpenBrace,
		CloseBrace,
		Enter,
		Semicolon,
		Quote,
		Backslash,
		Backslash2, // DirectInput calls this DIK_OEM_102: "< > | on UK/Germany keyboards"
		Comma,
		FullStop,
		Slash,
		Space,

		Insert,
		Delete,
		Home,
		End,
		PageUp,
		PageDown,
		Left,
		Right,
		Up,
		Down,

		KeyPadSlash,
		KeyPadAsterisk,
		KeyPadMinus,
		KeyPadPlus,
		KeyPadDelete,
		KeyPadEnter,

		PrintScreen,
		Pause,

		AbntC1,
		Yen,
		Kana,
		Convert,
		NoConvert,
		At,
		Circumflex,
		Colon2,
		Kanji,

		KeyPadEquals, // MacOS X
		BackQuote, // MacOS X
		Semicolon2, // MacOS X
		Command, // MacOS X

		AndroidBack, // Android
		AndroidVolumeUp, // Android
		AndroidVolumeDown, // Android

		AndroidSearch,
		AndroidDPadCenter,
		AndroidButtonX,
		AndroidButtonY,
		AndroidDPadUp,
		AndroidDPadDown,
		AndroidDPadLeft,
		AndroidDPadRight,
		AndroidSelect,
		AndroidStart,
		AndroidButtonL1,
		AndroidButtonR1,
		AndroidButtonL2,
		AndroidButtonR2,
		AndroidButtonA,
		AndroidButtonB,
		AndroidThumbL,
		AndroidThumbR,

		Unknown,

		// Insert additional keys here
		Alt = -3,
		Control,
		Shift,
		Any,

		// Modifier keys
		LShift = 215,
		RShift,
		LControl,
		RControl,
		LAlt,
		RAlt, // AltGr
		LWin,
		RWin,
		Menu,
		ScrollLock,
		NumLock,
		CapsLock

	};

	enum class KeyModifiers {
		LShift = 1,
		RShift = 2,
		LControl = 4,
		RControl = 8,
		LAlt = 16,
		RAlt = 32,
		Shift = LShift | RShift,
		Control = LControl | RControl,
		Alt = LAlt | RAlt
	};
	ENABLE_BITFLAG_OPERATORS(KeyModifiers);

}