// StateCellGrid.h
// 2D-сетка указателей на CStateCell (замена CStateCell*** m_pCStateCell).
// Плоский std::vector вместо тройного указателя, bounds checking в debug.

#pragma once

#include <vector>
#include <cassert>
#include <stacktrace>
#include <iostream>

class CStateCell;

class StateCellGrid {
public:
	void Init(short linCount, short colCount) {
		_linCount = linCount;
		_colCount = colCount;
		_cells.assign(static_cast<size_t>(linCount) * colCount, nullptr);
	}

	CStateCell* Get(short x, short y) const {
		assert(IsValidCoord(x, y));
		return _cells[static_cast<size_t>(y) * _colCount + x];
	}

	void Set(short x, short y, CStateCell* cell) {
		assert(IsValidCoord(x, y));
		_cells[static_cast<size_t>(y) * _colCount + x] = cell;
	}

	// Адрес слота (CStateCell**) — для CEyeshotCell, которому нужен указатель
	// внутрь сетки, чтобы видеть lazy-аллокацию/деаллокацию ячеек.
	CStateCell** SlotAddress(short x, short y) {
		assert(IsValidCoord(x, y));
		return &_cells[static_cast<size_t>(y) * _colCount + x];
	}

	bool IsValidCoord(short x, short y) const {
		if (x >= 0 && x < _colCount && y >= 0 && y < _linCount) {
			return true;
		}
		auto msg = std::format(
			"StateCellGrid: invalid coords ({}, {}), valid range: x=[0, {}), y=[0, {})\nStacktrace:\n{}",
			x, y, _colCount, _linCount, std::stacktrace::current());
		OutputDebugStringA(msg.c_str());
		std::cerr << msg << std::endl;
		return false;
	}

	short GetLinCount() const {
		return _linCount;
	}

	short GetColCount() const {
		return _colCount;
	}

private:
	std::vector<CStateCell*> _cells;
	short _linCount{};
	short _colCount{};
};
