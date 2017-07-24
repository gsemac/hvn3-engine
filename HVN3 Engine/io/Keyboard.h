#pragma once
#include <queue>
#include <string>
#include "EventSource.h"

namespace hvn3 {

	class GameManager;

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
		Control = -2,
		Shift,
		Any,

		// Modifier keys
		LShift = 215,
		RShift,
		LControl,
		RControl,
		LAlt,
		RAlt,
		LWin,
		RWin,
		Menu,
		ScrollLock,
		NumLock,
		CapsLock

	};

	class Keyboard {
		friend class StateAccessor;

	public:
		class StateAccessor {

		public:
			static void ResetKeyStates(bool pressed, bool released, bool held);
			static void ResetKeyStates();
			static void SetKeyState(int key_code, bool pressed);
			static void SetLastChar(int key_char);

		};

		static bool KeyPressed(int key_code);
		static bool KeyDown(int key_code);
		static bool KeyReleased(int key_code);
		static bool KeyPressedOrHeld(int key_code);
		static bool KeyPressed(hvn3::Key key);
		static bool KeyDown(hvn3::Key key);
		static bool KeyReleased(hvn3::Key key);
		static bool KeyPressedOrHeld(hvn3::Key key);

		static int LastChar();
		static int LastKey();
		static void ClearLastChar();
		static void ClearLastKey();
		static bool IsChar(int key_code);
		static bool IsChar(hvn3::Key key);

		static System::EventSource EventSource();

	private:
		struct Key {

			bool held; // whether or not key is held
			bool pressed; // whether or not key is pressed (instantaneous)
			bool locked; // if locked, pressed is false until released and pressed again (for single key presses)
			bool released; // whether or not the key was released (instaneous)

			Key() : held(0), pressed(0), locked(0), released(0) {}

		};

		static Key _key_states[];
		static bool _pressed_any, _released_any;
		static int _held_count;
		static int _last_char;
		static int _last_key;
		static int _modifier;

		Keyboard();

	};

}