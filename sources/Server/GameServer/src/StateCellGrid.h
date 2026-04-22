// StateCellGrid.h
// 2D-сетка указателей на CStateCell (замена CStateCell*** m_pCStateCell).
// Плоский std::vector вместо тройного указателя, bounds checking в debug.

#pragma once

#include <vector>

class CStateCell;

class StateCellGrid {
public:
	void         Init(short linCount, short colCount);

	CStateCell*  Get(short x, short y) const;
	void         Set(short x, short y, CStateCell* cell);

	// Адрес слота (CStateCell**) — для CEyeshotCell, которому нужен указатель
	// внутрь сетки, чтобы видеть lazy-аллокацию/деаллокацию ячеек.
	CStateCell** SlotAddress(short x, short y);

	bool         IsValidCoord(short x, short y) const;

	short        GetLinCount() const;
	short        GetColCount() const;

private:
	std::vector<CStateCell*> _cells;
	short _linCount{};
	short _colCount{};
};
