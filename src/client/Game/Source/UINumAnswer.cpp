
#include "stdafx.h"

#include "uiNumAnswer.h"

using namespace std;

CImageBMP::CImageBMP()
: _pData(0), _nDataSize(0), _nBmpData(0), _nWidth(0), _nHeight(0)
{
}

CImageBMP::~CImageBMP()
{
	Clear();
}


bool CImageBMP::Load(BYTE* pData, int nSize)
{
	Clear();

	_pData = new BYTE[nSize];
	memcpy(_pData, pData, nSize);

	_nDataSize = nSize;

	// 
	memcpy(&_nBmpData, _pData + 10, sizeof(int));
	memcpy(&_nWidth,   _pData + 18, sizeof(int));
	memcpy(&_nHeight,  _pData + 22, sizeof(int));

	return true;
}


void CImageBMP::RandomPoint(void)
{
	static BYTE BitMask[] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};

	if(_pData)
	{
		for(int i = _nBmpData; i < _nDataSize; ++i)
		{
			if((rand() & 3) == 0)
			{
				_pData[i] &= BitMask[rand() & 7];
			}
		}
	}
}


void CImageBMP::Clear(void)
{
	SAFE_DELETE_ARRAY(_pData);
	_nDataSize = 0;
	_nBmpData  = 0;
	_nWidth    = 0;
	_nHeight   = 0;
}


int CImageBMP::GetWidth(void)
{
	return _nWidth;
}


int CImageBMP::GetHeight(void)
{
	return _nHeight;
}


DWORD CImageBMP::GetColor(int x, int y)
{
	static BYTE BitMask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	if(_pData)
	{
		//   =   + ( BMP  *       ) + 
		int nPos = _nBmpData + (_nHeight - y - 1) * (((_nWidth >> 5) + 1) << 2) + (x >> 3);

		return (!(_pData[nPos] & BitMask[x & 7]));
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


namespace GUI
{

	CNumAnswerMgr::CNumAnswerMgr()
	{
		_pNumTexture = 0;
		_dwTickCount = 0;
	}

	CNumAnswerMgr::~CNumAnswerMgr()
	{
		SAFE_RELEASE(_pNumTexture);
	}


	bool CNumAnswerMgr::Init()
	{
		#if !ENABLE_NUMANSWER
			return true;
		#endif

		frmNumAnswer = CFormMgr::s_Mgr.Find("frmWGAnswer");
		if(!frmNumAnswer) return false;
		frmNumAnswer->evtEntrustMouseEvent = _evtCheckCodeMouseEvent;

		frmFast = CFormMgr::s_Mgr.Find("frmFast");
		if(! frmFast) return false;

		edtUserInput = dynamic_cast<CEdit*>(frmNumAnswer->Find("edtUserInput"));
		if(! edtUserInput) return false;

		ui3dCheckCode = dynamic_cast<C3DCompent*>(frmNumAnswer->Find("ui3dCheckCode"));
		if(!ui3dCheckCode) return false;
		ui3dCheckCode->SetRenderEvent(_evtCheckCodeRenderEvent);

		labTimeLeft = dynamic_cast<CLabelEx*>(frmNumAnswer->Find("labTimeLeft"));
		if(!labTimeLeft) return false;

		if(FAILED(g_Render.GetDevice()->CreateTexture(256, 256, 1, 0, D3DFMT_R5G6B5, D3DPOOL_MANAGED, &_pNumTexture, NULL)))
		{
			MessageBox(0, TEXT("NumAnswer_CreateTexture Failed!"), 0, MB_ICONERROR);
			return false;
		}

		return true;
	}


	void CNumAnswerMgr::ShowForm(bool v)
	{
		#if !ENABLE_NUMANSWER
			return;
		#endif

		if(v)
		{
			_dwTickCount = GetTickCount();
			edtUserInput->SetCaption("");

			frmNumAnswer->SetPos(frmNumAnswer->GetLeft(), frmFast->GetTop());
			frmNumAnswer->Refresh();

			frmNumAnswer->SetIsShow(v);
			frmNumAnswer->SetActiveCompent(edtUserInput);
		}
		else
		{
			frmNumAnswer->SetIsShow(v);
		}
	}


	void CNumAnswerMgr::FrameMove(DWORD dwTime)
	{
		#if !ENABLE_NUMANSWER
			return;
		#endif

		if(frmNumAnswer && frmNumAnswer->GetIsShow())
		{
			if(frmNumAnswer->GetBottom() > frmFast->GetTop())
			{
				int nLeft = (frmFast->GetWidth() - frmNumAnswer->GetWidth()) / 2 + frmFast->GetLeft();
				int nTop  = frmNumAnswer->GetTop() - 2;
				frmNumAnswer->SetPos(nLeft, nTop);
				frmNumAnswer->Refresh();
			}

			if(_dwTickCount)
			{
				int nSecond = 60 - ((GetTickCount() - _dwTickCount) / 1000);

				char szTime[64] = {0};
				sprintf(szTime, "%d", nSecond);
				labTimeLeft->SetCaption(szTime);

				if(nSecond <= 0)
				{
					frmNumAnswer->SetIsShow(false);
					_dwTickCount = 0;
				}
			}
		}
	}


	void CNumAnswerMgr::SetBmp(int nSeq, BYTE* pData, int nSize)
	{
		_imgBMP[nSeq].Load(pData, nSize);
		//_imgBMP[nSeq].RandomPoint();
	}


	// 
	void CNumAnswerMgr::Refresh()
	{
		if(_pNumTexture)
		{
			D3DLOCKED_RECT stLock;
			HRESULT hr = _pNumTexture->LockRect(0, &stLock, 0, D3DLOCK_DISCARD);
			if(FAILED(hr))
			{
				return ;
			}

			srand((unsigned int)time(0));

			int nLeft = 0;
			for(int nBMP = 0; nBMP < NUMBER_COUNT; ++nBMP)
			{
				int nWidth  = _imgBMP[nBMP].GetWidth();
				int nHeight = _imgBMP[nBMP].GetHeight();

				for(int x = 0; x < nWidth; ++x)
				{
					for(int y = 0; y < nHeight; ++y)
					{
						((unsigned short*)stLock.pBits)[y * 256 + x + nLeft] = _imgBMP[nBMP].GetColor(x, y) ? (unsigned short)(rand()) : 0xFFFF;
					}
				}

				nLeft += nWidth;
			}

			_pNumTexture->UnlockRect(0);
		}
	}


	void CNumAnswerMgr::RenderCheckCode(void)
	{
		if(_pNumTexture && frmNumAnswer && ui3dCheckCode)
		{
			RECT rc;
			memset(&rc, 0, sizeof(RECT));

			for(int i = 0; i < NUMBER_COUNT; ++i)
			{
				rc.right += _imgBMP[i].GetWidth();
				rc.bottom = _imgBMP[i].GetHeight() > rc.bottom ? _imgBMP[i].GetHeight() : rc.bottom;
			}

			VECTOR2 vecDest;
			vecDest.x = (FLOAT)(frmNumAnswer->GetLeft() + ui3dCheckCode->GetLeft());
			vecDest.y = (FLOAT)(frmNumAnswer->GetTop()  + ui3dCheckCode->GetTop() );

			GetRender().RenderSprite(_pNumTexture, &rc, 0, &vecDest, 0xFFFFFFFF);
		}
	}


	void CNumAnswerMgr::SendCheckCode(void)
	{
		CS_CheatCheck(edtUserInput->GetCaption());
	}


	void CNumAnswerMgr::_evtCheckCodeMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string strName = pSender->GetName();

		if(strName == "btnYes")
		{
			g_stUINumAnswer.SendCheckCode();
		}
	}


	void CNumAnswerMgr::_evtCheckCodeRenderEvent(C3DCompent *pSender, int x, int y)
	{
		g_stUINumAnswer.RenderCheckCode();
	}

}


