#include "StdAfx.h"
#include "ConsoleProcessor.h"
#include "EncodingUtil.h"

namespace {
	constexpr int kLineStep = 22; // согласовано с FontSlot::Console в клиенте

	int ComputeMaxLine(int height) {
		const int raw = (height - 18) / kLineStep;
		return raw > 1 ? raw : 1;
	}
} // namespace

ConsoleProcessor::ConsoleProcessor() {
	_maxLine = ComputeMaxLine(_height);
	for (int i = 0; i < _maxLine; ++i) {
		_textList.emplace_back();
	}
	_cmdCursor = _cmdList.begin();
	_AddText("Welcome To MindPower3D Engine", false);
}

void ConsoleProcessor::Resize(int newWidth, int newHeight) {
	if (newWidth <= 0 || newHeight <= 0) {
		return;
	}
	_width = newWidth;
	_height = newHeight;
	_maxLine = ComputeMaxLine(_height);
	while (static_cast<int>(_textList.size()) < _maxLine) {
		_textList.emplace_back();
	}
}

bool ConsoleProcessor::HandleKeyDown(int keyCode) {
	if (!_visible) {
		return false;
	}
	if (keyCode == VK_UP) {
		if (_cmdList.empty()) {
			return true;
		}
		if (_cmdCursor == _cmdList.begin()) {
			_cmdCursor = _cmdList.end();
		}
		--_cmdCursor;
		_input = *_cmdCursor;
	}
	return true;
}

bool ConsoleProcessor::HandleChar(char ch) {
	// Backtick — toggle. Разрешение открытия — извне через _canOpenCheck.
	if (ch == '`') {
		if (!_visible) {
			if (_canOpenCheck && _canOpenCheck()) {
				_visible = true;
			}
		}
		else {
			_visible = false;
		}
		return true;
	}
	if (!_visible) {
		return false;
	}

	if (ch == '\r') {
		if (_input.empty()) {
			return true;
		}
		// _input уже в UTF-8 (накапливается через encoding::AppendAnsiByteAsUtf8).
		_AddText(">" + _input, false);
		if (_cmdHandler) {
			std::string out = _cmdHandler(_input);
			if (!out.empty()) {
				_AddText(std::move(out), false);
			}
		}
		_AddText(_input, true);
		_input.clear();
	}
	else if (ch == '\t') {
		// TODO: autocomplete.
	}
	else if (ch == 0x08) {
		// Backspace — удаляет последний UTF-8 codepoint
		encoding::PopLastUtf8Codepoint(_input);
	}
	else if (_input.size() < 48) {
		encoding::AppendAnsiByteAsUtf8(static_cast<unsigned char>(ch), _input);
	}

	return true;
}

std::string ConsoleProcessor::GetDisplayLine() const {
	// _input уже в UTF-8 (накопление через encoding::AppendAnsiByteAsUtf8
	// в HandleChar).
	std::string out;
	out.reserve(_input.size() + 2);
	out.push_back(']');
	out += _input;
	if (_showCursor) {
		out.push_back('_');
	}
	return out;
}

void ConsoleProcessor::_AddText(std::string text, bool isCmd) {
	if (text.empty()) {
		return;
	}
	auto& list = isCmd ? _cmdList : _textList;
	if (static_cast<int>(list.size()) > _maxLine) {
		list.pop_front();
	}
	list.push_back(std::move(text));
	if (isCmd) {
		_cmdCursor = list.end();
	}
}

void ConsoleProcessor::Tick() {
	const std::uint32_t tick = ::GetTickCount();
	if (_cursorTick == 0) {
		_cursorTick = tick;
	}
	if (tick - _cursorTick < 500) {
		return;
	}
	_cursorTick = tick;
	_showCursor = !_showCursor;
}

void ConsoleProcessor::Clear() {
	_textList.clear();
	for (int i = 0; i < _maxLine; ++i) {
		_textList.emplace_back();
	}
}
