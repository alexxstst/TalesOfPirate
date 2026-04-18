#include "StdAfx.h"
#include "UIText.h"
#include "uiimeinput.h"
#include "GameApp.h"
#include "GameConfig.h"
#include "EncodingUtil.h"

#include <format>
#include <vector>

#pragma comment( lib, "Imm32.lib" )

using namespace GUI;

CImeInput  CImeInput::s_Ime;

extern HINSTANCE hInst;

CImeInput::CImeInput(void)
    : _pImage(NULL), _ImmNameColor(0xf00000ff), _pList(NULL),
      _nShowX(0), _nShowY(0), _bIsShow(false), _bIsFull(false),
      _nScreenWidth(0), _nScreenHeight(0), _lConversion(0),
      _hImc(NULL), _oldImc(NULL), _nWidth(0), _nHeight(0)
{
    SetSize( InputX, InputY );
}

CImeInput::~CImeInput(void)
{
    Clear();
}

void CImeInput::Clear()
{
    if( _hImc )
    {
        _hImc = NULL;
    }
    if( _pImage )
    {
        delete _pImage;
        _pImage = NULL;
    }
}

void CImeInput::Init()
{
}

bool CImeInput::HandleWindowMsg(DWORD dwMsg, WPARAM wParam, LPARAM lParam)
{
    if( !_bIsFull ) return false;

    switch (dwMsg)
    {
    case WM_INPUTLANGCHANGEREQUEST:
    case WM_INPUTLANGCHANGE:
        {
            // ImmGetDescriptionW возвращает число wchar_t (без NUL).
            const int wlen = ::ImmGetDescriptionW((HKL)lParam, nullptr, 0);
            if (wlen > 0)
            {
                std::wstring wbuf(static_cast<size_t>(wlen), L'\0');
                ::ImmGetDescriptionW((HKL)lParam, wbuf.data(), wlen + 1);
                _immName = "[";
                _immName += encoding::WideToUtf8(wbuf);
                _immName += "]";
                _composition.clear();
                _bIsShow = true;
            }
            else
            {
                _bIsShow = false;
                _immName = GetLanguageString(622);
            }
            return true;
        }
        break;
    case WM_IME_SETCONTEXT:
        return true;
    case WM_IME_COMPOSITION:
        {
            _GetCompositionStringUtf8(_composition, GCS_COMPSTR);
            return true;
        }
        break;
    case WM_IME_NOTIFY:
        {
            switch (wParam)
            {
            case IMN_CHANGECANDIDATE:
            case IMN_CLOSECANDIDATE:
            case IMN_OPENCANDIDATE:
                _GetCandidateList();
                break;
            case IMN_SETCONVERSIONMODE:
            case IMN_SETSENTENCEMODE:
            case IMN_OPENSTATUSWINDOW:
                _GetConversion();
                break;
            }
            return true;
        }
        break;
    case WM_IME_STARTCOMPOSITION:
    case WM_IME_ENDCOMPOSITION:
        return true;
    default:
        return false;
    }
    return false;
}

void CImeInput::Render()
{
    if (!_bIsShow || !_bIsFull) return;

    if (_composition.empty() && _candidate.empty())
    {
        return;
    }

    _pImage->Render(_nShowX, _nShowY);
    ui::Render(_immName.c_str(),            _nShowX + 7,  _nShowY + 7,  _ImmNameColor);
    ui::Render(_inputMode.c_str(),          _nShowX + 40, _nShowY + 7,  _ImmNameColor);
    ui::Render(_sbcMode.c_str(),            _nShowX + 61, _nShowY + 7,  _ImmNameColor);
    ui::Render(_interpunctionMode.c_str(),  _nShowX + 82, _nShowY + 7,  _ImmNameColor);
    ui::Render(_composition.c_str(),        _nShowX + 7,  _nShowY + 25, _ImmNameColor);
    ui::Render(_candidate.c_str(),          _nShowX,      _nShowY + 50, _ImmNameColor);
}

bool CImeInput::_GetCompositionStringUtf8(std::string& out, DWORD ImeValue)
{
    out.clear();
    // Возвращает размер в БАЙТАХ (для W-версии — байт WCHAR). <=0 → nodata/error.
    const LONG byteSize = ::ImmGetCompositionStringW(_hImc, ImeValue, nullptr, 0);
    if (byteSize <= 0)
    {
        return false;
    }
    const size_t wcharCount = static_cast<size_t>(byteSize) / sizeof(wchar_t);
    std::wstring wbuf(wcharCount, L'\0');
    ::ImmGetCompositionStringW(_hImc, ImeValue, wbuf.data(), static_cast<DWORD>(byteSize));
    out = encoding::WideToUtf8(wbuf);
    return true;
}

bool CImeInput::_GetCandidateList()
{
    SAFE_DELETE(_pList);
    _candidate.clear();

    // ImmGetCandidateListW с buf=nullptr возвращает требуемый размер в байтах.
    const DWORD dwSize = ::ImmGetCandidateListW(_hImc, 0x0, nullptr, 0);
    if (dwSize == 0)
    {
        return false;
    }

    std::vector<BYTE> buf(dwSize, 0);
    LPCANDIDATELIST pList = reinterpret_cast<LPCANDIDATELIST>(buf.data());
    const DWORD written = ::ImmGetCandidateListW(_hImc, 0x0, pList, dwSize);
    if (written == 0)
    {
        return false;
    }

    // Offsets указывают на wchar_t-строки относительно начала pList.
    // Воспроизводим формат " N:<text>\n" в UTF-8.
    const unsigned int sel = pList->dwSelection;
    const unsigned int start = (pList->dwPageStart == 0)
                                   ? pList->dwPageStart + sel
                                   : sel;
    const unsigned int end = (pList->dwPageStart == 0)
                                 ? pList->dwPageStart + pList->dwPageSize + sel
                                 : pList->dwPageStart + pList->dwPageSize;

    for (unsigned int i = start; i < pList->dwCount && i < end; ++i)
    {
        const wchar_t* candW = reinterpret_cast<const wchar_t*>(
            buf.data() + pList->dwOffset[i]);
        const std::string candUtf8 = encoding::WideToUtf8(candW);
        _candidate += std::format(" {}:{}\n", i + 1 - sel, candUtf8);
    }
    return true;
}

bool CImeInput::_GetConversion()
{
    unsigned long sentence = 0;
    ::ImmGetConversionStatus(_hImc, &_lConversion, &sentence);
    // Тексты-индикаторы оставлены пустыми [—]: legacy-код подставлял
    // в strncpy пустые литералы, визуальных меток не было. При необходимости
    // можно добавить нужные символы (например "EN"/"CN", "半"/"全" и т.п.).
    _inputMode         = (_lConversion & 0x01)  ? "[EN]" : "[RU]";
    _sbcMode           = (_lConversion & 0x08)  ? "[F]"  : "[H]";
    _interpunctionMode = (_lConversion & 0x400) ? "[.]"  : "[,.]";
    return true;
}

void CImeInput::SetShowPos(int x, int y)
{
    _nShowX = x + 10;
    _nShowY = y;

    if (_nShowX > _nScreenWidth)
    {
        _nShowX = _nScreenWidth;
    }

    if (_nShowY > _nScreenHeight)
    {
        _nShowY = GetRender().GetScreenHeight() - InputY;
    }
}

void CImeInput::SetScreen(bool isFull, int w, int h)
{
    _bIsFull = (TRUE == g_Config.m_bFullScreen);

    if (_bIsFull)
    {
        _nScreenWidth  = w - GetWidth();
        _nScreenHeight = h - GetHeight();

        if (!_hImc)
        {
            _hImc = ::ImmGetContext(g_pGameApp->GetHWND());
        }

        if (!_pImage)
        {
            _pImage = new CGuiPic(NULL);
            _pImage->LoadImage("texture/ui/ime/background.tga", 122, 172);
        }
    }
    else
    {
        ::ImmAssociateContextEx(g_pGameApp->GetHWND(), NULL, IACE_DEFAULT);
    }
}

void CImeInput::SetAlpha(BYTE alpha)
{
    _pImage->SetAlpha(alpha);
    _ImmNameColor = (_ImmNameColor & 0x00ffffff) & (alpha << 24);
}
