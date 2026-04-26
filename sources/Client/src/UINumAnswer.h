#pragma once


#include "uiForm.h"
#include "uiFormMgr.h"
#include "uiEdit.h"
#include "uiLabel.h"
#include "ui3DCompent.h"
#include "uiRender.h"
#include "uiGlobalVar.h"
#include "PacketCmd.h"

#include "uiStoreForm.h"


// 
#define ENABLE_NUMANSWER   1


// BMP
class CImageBMP {
public:
	CImageBMP();
	~CImageBMP();

	bool Load(BYTE* pData, int nSize);
	void Clear(void);

	void RandomPoint(void);

	int GetWidth(void);
	int GetHeight(void);
	DWORD GetColor(int x, int y);

private:
	BYTE* _pData;
	int _nDataSize;
	int _nBmpData;
	int _nWidth;
	int _nHeight;
};


namespace GUI {
	class CNumAnswerMgr : public CUIInterface {
	public:
		CNumAnswerMgr();
		~CNumAnswerMgr();

		void ShowForm(bool v = true);

		void SetBmp(int nSeq, BYTE* pData, int nSize);
		void Refresh();

		void RenderCheckCode(void);
		void SendCheckCode(void);

	protected:
		virtual bool Init();

		virtual void CloseForm() {
		}

		virtual void FrameMove(DWORD dwTime);

	private:
		CForm* frmNumAnswer;
		CForm* frmFast; // 
		CEdit* edtUserInput;
		CLabelEx* labTimeLeft;
		C3DCompent* ui3dCheckCode;

		LPTEXTURE _pNumTexture;

		static const int NUMBER_COUNT = 4;
		CImageBMP _imgBMP[NUMBER_COUNT];

		DWORD _dwTickCount;

		static void _evtCheckCodeMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtCheckCodeRenderEvent(C3DCompent* pSender, int x, int y);
	};
}
