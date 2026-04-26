#include "StdAfx.h"
#include "UICommandCompent.h"

using namespace GUI;

//---------------------------------------------------------------------------
// class CCommandCompent
//---------------------------------------------------------------------------
CCommandCompent::CCommandCompent(CForm& frmOwn)
	: CCompent(frmOwn) {
}

CCommandCompent::CCommandCompent(const CCommandCompent& rhs)
	: CCompent(rhs) {
}

CCommandCompent& CCommandCompent::operator=(const CCommandCompent& rhs) {
	CCompent::operator =(rhs);
	return *this;
}

CCompent* CCommandCompent::GetHitCommand(int x, int y) {
	if (IsNormal() && InRect(x, y))
		return this;

	return NULL;
}
