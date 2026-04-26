//----------------------------------------------------------------------
// :
// :lh 2004-07-15
// :2004-10-20
//----------------------------------------------------------------------
#pragma once
#include "UIRender.h"
#include "GlobalInc.h"

namespace GUI {
	class CGuiData;

	class CGuiPic {
	public: // 
		CGuiPic(CGuiData* pParent = NULL, unsigned int max = 1);
		CGuiPic(const CGuiPic& rhs);
		CGuiPic& operator=(const CGuiPic& rhs); // _pOwn
		~CGuiPic() {
			SAFE_DELETE_ARRAY(_pImage);
		} // UI //delete [] _pImage; }

		void SetMax(int v);

		void FrameRender(unsigned int nFrame, int x, int y);

		void Render(int x, int y);
		void Render(int x, int y, BYTE bAplha);
		void Render(int x, int y, DWORD dwColor);
		void RenderAll(int x, int y);
		void RenderAll(int x, int y, BYTE bAlpha);
		void RenderAll(int x, int y, DWORD dwColor);

		void SetFrame(unsigned int num) {
			if (num < _max) _frame = num;
		} // 
		MPTexRect* GetImage(unsigned int v = 0) {
			if (v < _max) return &_pImage[v];
			return NULL;
		}

		void Next() {
			_frame++;
			if (_frame >= _max) _frame = 0;
		} // 
		void Prior() {
			_frame--;
			if (_frame >= _max) _frame = _max - 1;
		} // 

		void SetScale(int w, int h);
		void SetScale(unsigned int frame, int w, int h);
		void SetScaleW(int w);
		void SetScaleH(int h);
		void SetScaleW(unsigned int nFrame, int nW);
		void SetScaleH(unsigned int nFrame, int nH);

		void SetScaleW(unsigned int nFrame, float fW);
		void SetScaleH(unsigned int nFrame, float fH);

		void SetAlpha(BYTE alpha);
		void TintColour(int red, int blue, int green);

		unsigned int GetMax() {
			return _max;
		}

		unsigned int GetFrame() {
			return _frame;
		}

		bool LoadImage(std::string_view file);
		bool LoadImage(std::string_view file, int w, int h, int frame = 0, int tx = 0, int ty = 0, float scalex = 0.0,
					   float scaley = 0.0);
		bool LoadAllImage(std::string_view file, int w, int h, int tx = 0, int ty = 0);
		bool LoadImage(int frame, int nTextureID, int tx, int ty, int tw, int th, float scale_x = 0.0,
					   float scale_y = 0.0);
		void Refresh();

		void SetParent(CGuiData* p) {
			_pParent = p;
		}

		void SetIsScale(bool v) {
			_bIsScale = v;
		}

		static bool LoadImage(MPTexRect* pTex, std::string_view file, int w, int h, int tx = 0, int ty = 0,
							  float scale_x = 1.0, float scale_y = 1.0);

		int GetWidth(unsigned int frame = 0) {
			return (int)(_pImage[frame].fScaleX * _pImage[frame].nTexW);
		}

		int GetHeight(unsigned int frame = 0) {
			return (int)(_pImage[frame].fScaleY * _pImage[frame].nTexH);
		}

		void SetColor(DWORD color) {
			for (unsigned int i = 0; i < _max; i++) _pImage[i].dwColor = color;
		}

		bool IsNull() {
			return _pImage[0].nTextureNo == -1;
		}

		void UnLoadImage(int frame = -1);

	private:
		MPTexRect* _pImage;
		unsigned int _frame;
		unsigned int _max;
		CGuiData* _pParent;
		bool _bIsScale; // 
	};

	// 
	class CFramePic {
	public:
		enum ePicPos {
			ppLeft = 0, // 
			ppUp,
			ppRight,
			ppBottom,
			ppRightBottom,
			ppLeftBottom,
			ppRightUp,
			ppLeftUp, // 
			ppClient, // 
			ppEnd,
		};

		// 

	public:
		CFramePic(CGuiData* pOwn);
		CFramePic(const CFramePic& rhs);
		CFramePic& operator=(const CFramePic& rhs);

		~CFramePic() {
			SAFE_DELETE_ARRAY(_pImage);
		} // UI //delete [] _pImage; }

		void Render();
		void Render(BYTE bAplha);
		void Render(int x, int y, BYTE alpha);
		void Refresh();

		void SetAlpha(BYTE alpha);
		bool LoadImage(ePicPos p, int nTextureID, int tx, int ty, int tw, int th);

		// w,hclientcw,ch
		bool LoadImage(const char* client, int cw, int ch, int tx, int ty, std::string_view file, int w, int h);

		void SetIsTitle(bool v) {
			_bIsTitle = v;
		}

		void SetIsShowFrame(bool v) {
			_bIsShowFrame = v;
		}

		void SetParent(CGuiData* p) {
			_pOwn = p;
		}

		CGuiData* GetParent() {
			return _pOwn;
		}

		MPTexRect* GetImage(unsigned int v = 0) {
			if (v < ppEnd) return &_pImage[v];
			return NULL;
		}

	private:
		void _ClientShow(int x, int y, DWORD color); // 

	private:
		MPTexRect* _pImage;
		CGuiData* _pOwn;
		int _nX[ppEnd], _nY[ppEnd];
		bool _bIsTitle; // ppClient
		bool _bIsShowFrame; // 
	};

	// 
	inline void CGuiPic::SetScale(unsigned int frame, int w, int h) {
		if (frame < _max) {
			_pImage[frame].fScaleX = (float)w / (float)_pImage[frame].nTexW;
			_pImage[frame].fScaleY = (float)h / (float)_pImage[frame].nTexH;
		}
	}

	inline void CGuiPic::SetScaleW(unsigned int frame, int w) {
		if (frame < _max)
			_pImage[frame].fScaleX = (float)w / (float)_pImage[frame].nTexW;
	}

	inline void CGuiPic::SetScaleH(unsigned int frame, int h) {
		if (frame < _max)
			_pImage[frame].fScaleY = (float)h / (float)_pImage[frame].nTexH;
	}

	inline void CGuiPic::SetScaleW(unsigned int nFrame, float fW) {
		if (nFrame < _max)
			_pImage[nFrame].fScaleX = fW;
	}

	inline void CGuiPic::SetScaleH(unsigned int nFrame, float fH) {
		if (nFrame < _max)
			_pImage[nFrame].fScaleY = fH;
	}

	inline void CGuiPic::Render(int x, int y) {
		GetRender().RenderTextureRect(x, y, &_pImage[_frame]);
	}

	inline void CGuiPic::Render(int x, int y, DWORD color) {
		GetRender().RenderTextureRect(x, y, &_pImage[_frame], color);
	}

	inline void CGuiPic::Render(int x, int y, BYTE bAplha) {
		GetRender().RenderTextureRect(x, y, &_pImage[_frame], (bAplha << 24) | 0x00FFFFFF);
	}

	inline void CGuiPic::FrameRender(unsigned int nFrame, int x, int y) {
		if (nFrame < (int)_max) GetRender().RenderTextureRect(x, y, &_pImage[nFrame]);
	}
}
