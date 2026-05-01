#pragma once

#define NOMINMAX
#include <windows.h>

#include <cstdint>
#include <memory>


namespace Corsairs::Engine::Input {
	//  Стадия клавиши за один кадр (edge-detected).
	//  Значения совместимы со старыми #define KEY_FREE/PUSH/HOLD/POP (MPGameApp.h).
	enum KeyPhaseBits : std::uint8_t {
		KeyFree = 0x01,
		KeyPush = 0x02, //  Только что нажата (1 кадр)
		KeyHold = 0x04, //  Удерживается
		KeyPop = 0x08, //  Только что отпущена (1 кадр)
	};

	enum class MouseButton : std::uint8_t {
		Left = 0,
		Right = 1,
		Middle = 2,
	};


	class InputSystem {
	public:
		static InputSystem& Instance();

		InputSystem(const InputSystem&) = delete;
		InputSystem& operator=(const InputSystem&) = delete;

		bool Init(HWND hwnd);
		void Release();

		//  Вызывать раз за кадр — обновить edge-состояния клавиш и мыши.
		void Update();

		//  Колбэк из WndProc. Возвращает true если сообщение нужно "поглотить".
		bool OnWmMessage(std::uint32_t msg, std::uintptr_t wparam, std::intptr_t lparam);

		//  При потере фокуса окна — сбросить всё в KeyFree.
		void Reset();

		//  DIK-совместимый scancode (из младшего байта Windows scancode: (LPARAM >> 16) & 0xFF).
		//  Extended-биту соответствует |0x80, т.е. DIK_RCONTROL = 0x9D.
		std::uint8_t GetKeyPhase(std::uint8_t dik) const;
		std::uint8_t GetAsciiPhase(std::uint8_t ch) const;

		bool IsKeyDown(std::uint8_t dik) const; //  PUSH || HOLD

		//  Мышь.
		int GetMouseX() const;
		int GetMouseY() const;
		int GetMouseDeltaX() const;
		int GetMouseDeltaY() const;
		int GetMouseWheelDelta() const;
		bool IsMouseButtonDown(MouseButton b) const;

	private:
		InputSystem();
		~InputSystem();

		struct Impl;
		std::unique_ptr<Impl> _impl;
	};
} // namespace Corsairs::Engine::Input
