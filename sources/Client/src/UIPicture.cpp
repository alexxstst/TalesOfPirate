#include "StdAfx.h"
#include "uipicture.h"
#include "UIGuiData.h"

using namespace GUI;
//---------------------------------------------------------------------------
// class CGuiPic
//---------------------------------------------------------------------------
CGuiPic::CGuiPic(CGuiData* pParent, unsigned int max)
	: _pParent(pParent), _pImage(NULL), _max(max), _frame(0), _bIsScale(false) {
	if (_max == 0) _max = 1;
	_pImage = new MPTexRect[_max];
}

CGuiPic::CGuiPic(const CGuiPic& rhs)
	: _pParent(rhs._pParent), _pImage(new MPTexRect[rhs._max]), _max(rhs._max), _frame(rhs._frame),
	  _bIsScale(rhs._bIsScale) {
	for (unsigned int i = 0; i < _max; i++) {
		_pImage[i] = rhs._pImage[i];
	}
}


CGuiPic& CGuiPic::operator=(const CGuiPic& rhs) {
	_max = rhs._max;
	_frame = rhs._frame;
	_bIsScale = rhs._bIsScale;
	for (unsigned int i = 0; i < _max; i++) {
		_pImage[i] = rhs._pImage[i];
	}
	return *this;
}

void CGuiPic::SetMax(int v) {
	if (v < 1) return;
	if (_max == v) return;

	_max = v;
	//delete [] _pImage;
	SAFE_DELETE_ARRAY(_pImage); // UI

	_pImage = new MPTexRect[v];
	_frame = 0;
}

void CGuiPic::SetAlpha(BYTE alpha) {
	for (unsigned int i = 0; i < _max; i++) {
		_pImage[i].dwColor = (_pImage[i].dwColor & 0x00FFFFFF) | (alpha << 24);
	}
}

void CGuiPic::TintColour(int red, int green, int blue) {
	for (unsigned int i = 0; i < _max; i++) {
		_pImage[i].dwColor = D3DCOLOR_ARGB(255, red, green, blue);
	}
}

void CGuiPic::SetScale(int w, int h) {
	if (_pImage[0].nTextureNo == -1) return;

	for (unsigned int i = 0; i < _max; i++) {
		_pImage[i].fScaleX = (float)w / (float)_pImage[i].nTexW;
		_pImage[i].fScaleY = (float)h / (float)_pImage[i].nTexH;
	}
}

void CGuiPic::SetScaleW(int nW) {
	for (unsigned int i = 0; i < _max; i++)
		_pImage[i].fScaleX = (float)nW / (float)_pImage[i].nTexW;
}

void CGuiPic::SetScaleH(int nH) {
	for (unsigned int i = 0; i < _max; i++)
		_pImage[i].fScaleY = (float)nH / (float)_pImage[i].nTexH;
}

void CGuiPic::UnLoadImage(int frame) {
	if (frame < 0) {
		memset(_pImage, 0, sizeof(_pImage));
		for (unsigned int i = 0; i < _max; i++)
			_pImage[i].nTextureNo = -1;
	}
	else if (frame < (int)_max) {
		memset(&_pImage[frame], 0, sizeof(_pImage[frame]));
		_pImage[frame].nTextureNo = -1;
	}
}

//---------------------------------------------------------------------------
// class CFramePic
//---------------------------------------------------------------------------
CFramePic::CFramePic(CGuiData* pOwn)
	: _pImage(new MPTexRect[ppEnd]), _pOwn(pOwn), _bIsShowFrame(false), _bIsTitle(true) {
	memset(_nX, 0, sizeof(_nX));
	memset(_nY, 0, sizeof(_nY));
}

CFramePic::CFramePic(const CFramePic& rhs)
	: _pImage(new MPTexRect[ppEnd]), _bIsShowFrame(rhs._bIsShowFrame), _bIsTitle(rhs._bIsTitle) {
	// _pOwn = rhs._pOwn;
	for (unsigned int i = 0; i < ppEnd; i++) {
		_pImage[i] = rhs._pImage[i];
		_nX[i] = rhs._nX[i];
		_nY[i] = rhs._nY[i];
	}
}

CFramePic& CFramePic::operator=(const CFramePic& rhs) {
	// _pOwn = rhs._pOwn;
	_bIsShowFrame = rhs._bIsShowFrame;
	_bIsTitle = rhs._bIsTitle;
	for (unsigned int i = 0; i < ppEnd; i++) {
		_pImage[i] = rhs._pImage[i];
		_nX[i] = rhs._nX[i];
		_nY[i] = rhs._nY[i];
	}
	return *this;
}

void CFramePic::SetAlpha(BYTE alpha) {
	for (unsigned int i = 0; i < ppEnd; i++) {
		_pImage[i].dwColor = (_pImage[i].dwColor & 0x00FFFFFF) | (alpha << 24);
	}
}
