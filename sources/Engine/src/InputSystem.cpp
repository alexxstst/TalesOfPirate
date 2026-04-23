#include "stdafx.h"
#include "InputSystem.h"

#include <array>
#include <cstring>


namespace Corsairs::Engine::Input {

	namespace {
		constexpr std::size_t kKeyCount   = 256;
		constexpr std::size_t kAsciiCount = 256;

		//  Извлечь DIK-совместимый scancode из LPARAM сообщений WM_KEY*.
		//  Bits 16..23 — базовый scancode; бит 24 — extended-флаг (для RCTRL/RALT/NUMPAD_ENTER).
		inline std::uint8_t ScanCodeFromLParam(std::intptr_t lparam) {
			const auto lp = static_cast<std::uint32_t>(lparam);
			const std::uint8_t base     = static_cast<std::uint8_t>((lp >> 16) & 0xFF);
			const bool         extended = (lp >> 24) & 1;
			if (!extended) {
				return base;
			}
			//  DIK_* для extended-клавиш имеет старший бит (например DIK_RCONTROL=0x9D).
			return static_cast<std::uint8_t>(base | 0x80);
		}
	} // anonymous

	struct InputSystem::Impl {
		HWND hwnd = nullptr;

		//  Нажатость клавиш по DIK-скан-кодам за текущий и предыдущий кадр.
		std::array<bool, kKeyCount> current{};
		std::array<bool, kKeyCount> previous{};

		//  Выходные phase-массивы (для совместимости с KEY_FREE/PUSH/HOLD/POP API).
		std::array<std::uint8_t, kKeyCount>   keyPhase{};
		std::array<std::uint8_t, kAsciiCount> asciiPhase{};

		//  Очередь ASCII-символов за текущий кадр (WM_CHAR).
		std::array<bool, kAsciiCount> asciiCurrent{};
		std::array<bool, kAsciiCount> asciiPrevious{};

		//  Мышь.
		int  mouseX         = 0;
		int  mouseY         = 0;
		int  mousePrevX     = 0;
		int  mousePrevY     = 0;
		int  mouseWheelAcc  = 0; //  Накоплено за кадр из WM_MOUSEWHEEL.
		int  mouseWheelLast = 0; //  Значение, видимое после Update.
		bool mouseDown[3]   = {false, false, false};

		void ResetAll() {
			current.fill(false);
			previous.fill(false);
			keyPhase.fill(KeyFree);
			asciiPhase.fill(KeyFree);
			asciiCurrent.fill(false);
			asciiPrevious.fill(false);
			mouseWheelAcc  = 0;
			mouseWheelLast = 0;
			for (auto& v : mouseDown) {
				v = false;
			}
		}
	};


	InputSystem::InputSystem()
		: _impl(std::make_unique<Impl>()) {
		_impl->ResetAll();
	}

	InputSystem::~InputSystem() = default;

	InputSystem& InputSystem::Instance() {
		static InputSystem instance;
		return instance;
	}

	bool InputSystem::Init(HWND hwnd) {
		_impl->hwnd = hwnd;
		_impl->ResetAll();
		return true;
	}

	void InputSystem::Release() {
		_impl->ResetAll();
		_impl->hwnd = nullptr;
	}

	void InputSystem::Reset() {
		_impl->ResetAll();
	}

	void InputSystem::Update() {
		Impl& s = *_impl;

		//  Обновляем phase для клавиш на основе (previous, current).
		for (std::size_t i = 0; i < kKeyCount; ++i) {
			const bool prev = s.previous[i];
			const bool curr = s.current[i];
			std::uint8_t phase;
			if (!prev && curr) {
				phase = KeyPush;
			} else if (prev && curr) {
				phase = KeyHold;
			} else if (prev && !curr) {
				phase = KeyPop;
			} else {
				phase = KeyFree;
			}
			s.keyPhase[i] = phase;
		}
		//  Сдвиг: previous = current. current не обнуляем — WM_KEYDOWN/UP держат его актуальным.
		s.previous = s.current;

		//  ASCII phase — аналогично.
		for (std::size_t i = 0; i < kAsciiCount; ++i) {
			const bool prev = s.asciiPrevious[i];
			const bool curr = s.asciiCurrent[i];
			std::uint8_t phase;
			if (!prev && curr) {
				phase = KeyPush;
			} else if (prev && curr) {
				phase = KeyHold;
			} else if (prev && !curr) {
				phase = KeyPop;
			} else {
				phase = KeyFree;
			}
			s.asciiPhase[i] = phase;
		}
		s.asciiPrevious = s.asciiCurrent;
		//  ASCII — одноразовые события из WM_CHAR. После кадра сбрасываем.
		s.asciiCurrent.fill(false);

		//  Мышь: дельта — разница снапшотов, колесо — накопленное.
		s.mousePrevX = s.mouseX;
		s.mousePrevY = s.mouseY;
		s.mouseWheelLast = s.mouseWheelAcc;
		s.mouseWheelAcc  = 0;
	}

	bool InputSystem::OnWmMessage(std::uint32_t msg, std::uintptr_t wparam, std::intptr_t lparam) {
		Impl& s = *_impl;
		switch (msg) {
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN: {
				const std::uint8_t dik = ScanCodeFromLParam(lparam);
				s.current[dik] = true;
				return false;
			}
			case WM_KEYUP:
			case WM_SYSKEYUP: {
				const std::uint8_t dik = ScanCodeFromLParam(lparam);
				s.current[dik] = false;
				return false;
			}
			case WM_CHAR: {
				const auto ch = static_cast<std::uint32_t>(wparam);
				if (ch < kAsciiCount) {
					s.asciiCurrent[static_cast<std::size_t>(ch)] = true;
				}
				return false;
			}
			case WM_MOUSEMOVE: {
				s.mouseX = static_cast<std::int16_t>(LOWORD(lparam));
				s.mouseY = static_cast<std::int16_t>(HIWORD(lparam));
				return false;
			}
			case WM_MOUSEWHEEL: {
				s.mouseWheelAcc += GET_WHEEL_DELTA_WPARAM(wparam);
				return false;
			}
			case WM_LBUTTONDOWN: s.mouseDown[0] = true;  return false;
			case WM_LBUTTONUP:   s.mouseDown[0] = false; return false;
			case WM_RBUTTONDOWN: s.mouseDown[1] = true;  return false;
			case WM_RBUTTONUP:   s.mouseDown[1] = false; return false;
			case WM_MBUTTONDOWN: s.mouseDown[2] = true;  return false;
			case WM_MBUTTONUP:   s.mouseDown[2] = false; return false;
			case WM_KILLFOCUS: {
				//  При потере фокуса — сбросить все клавиши в FREE (иначе подвиснут).
				s.current.fill(false);
				return false;
			}
			default:
				return false;
		}
	}

	std::uint8_t InputSystem::GetKeyPhase(std::uint8_t dik) const {
		return _impl->keyPhase[dik];
	}

	std::uint8_t InputSystem::GetAsciiPhase(std::uint8_t ch) const {
		return _impl->asciiPhase[ch];
	}

	bool InputSystem::IsKeyDown(std::uint8_t dik) const {
		const auto p = _impl->keyPhase[dik];
		return (p & (KeyPush | KeyHold)) != 0;
	}

	int  InputSystem::GetMouseX() const           { return _impl->mouseX; }
	int  InputSystem::GetMouseY() const           { return _impl->mouseY; }
	int  InputSystem::GetMouseDeltaX() const      { return _impl->mouseX - _impl->mousePrevX; }
	int  InputSystem::GetMouseDeltaY() const      { return _impl->mouseY - _impl->mousePrevY; }
	int  InputSystem::GetMouseWheelDelta() const  { return _impl->mouseWheelLast; }

	bool InputSystem::IsMouseButtonDown(MouseButton b) const {
		const auto i = static_cast<std::size_t>(b);
		return i < 3 && _impl->mouseDown[i];
	}

} // namespace Corsairs::Engine::Input
