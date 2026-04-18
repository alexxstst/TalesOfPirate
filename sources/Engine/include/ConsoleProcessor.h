// ConsoleProcessor — игровая отладочная консоль (преемник MPConsole).
//
// Чистый C++23: std::string / std::list / std::function вместо char-буферов
// и C-указателей. Хранит историю введённых команд, историю вывода, текущую
// строку ввода и состояние курсора. Отрисовка — ответственность клиента
// (CGameApp::_RenderConsoleText), здесь только состояние + логика.
//
// Backdrop-цвет и геометрия задаются извне (Lua → console_bootstrap.lua):
//   - SetBackdropColor(uint32_t argb)
//   - Resize(int width, int height)
//
// Обработчики команд и permission задаются как std::function:
//   - SetCmdHandler(std::function<std::string(std::string_view)>)
//   - SetCanOpenCheck(std::function<bool()>)
//
// Thread-safety: ожидается работа только из главного потока (сам guard —
// в ConsoleBridge; здесь опускаем).
#pragma once

#include <cstdint>
#include <functional>
#include <list>
#include <string>
#include <string_view>

class ConsoleProcessor {
public:
	using CmdHandler = std::function<std::string(std::string_view)>;
	using CanOpenCheck = std::function<bool()>;

	ConsoleProcessor();

	// Обработка символа от WM_CHAR. Возвращает true если событие обработано.
	bool HandleChar(char ch);
	// Обработка клавиш навигации (VK_UP для истории). Возвращает true если обработано.
	bool HandleKeyDown(int keyCode);

	void SetVisible(bool visible) {
		_visible = visible;
	}

	bool IsVisible() const {
		return _visible;
	}

	void Tick();

	void AddText(std::string text) {
		_AddText(std::move(text), false);
	}

	void AddHelp(std::string text) {
		_helpList.push_back(std::move(text));
	}

	void Clear();

	void SetCmdHandler(CmdHandler fn) {
		_cmdHandler = std::move(fn);
	}

	void SetCanOpenCheck(CanOpenCheck fn) {
		_canOpenCheck = std::move(fn);
	}

	int GetWidth() const {
		return _width;
	}

	int GetHeight() const {
		return _height;
	}

	// Пересчитывает размеры консоли + _maxLine; донакачивает буфер пустыми
	// строками до нового _maxLine. Вызывается из Lua (console_set_size).
	void Resize(int newWidth, int newHeight);

	void SetBackdropColor(std::uint32_t argb) {
		_backdropColor = argb;
	}

	std::uint32_t GetBackdropColor() const {
		return _backdropColor;
	}

	const std::list<std::string>& TextList() const {
		return _textList;
	}

	// Текущая строка ввода в отображаемом виде: "]<input>_" (с мигающим курсором).
	std::string GetDisplayLine() const;

private:
	void _AddText(std::string text, bool isCmd);

	std::string _input;
	std::list<std::string> _cmdList;
	std::list<std::string> _textList;
	std::list<std::string> _helpList;
	std::list<std::string>::iterator _cmdCursor;

	CmdHandler _cmdHandler;
	CanOpenCheck _canOpenCheck;

	int _width = 450;
	int _height = 140;
	int _maxLine = 0;

	bool _visible = false;
	std::uint32_t _cursorTick = 0;
	bool _showCursor = false;

	std::uint32_t _backdropColor = 0xE0101018; // ARGB: плотный тёмный
};
