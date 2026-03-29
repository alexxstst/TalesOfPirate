//----------------------------------------------------------------------
// :
// :lh 2004-08-21
// :2004-10-09
//----------------------------------------------------------------------

#pragma once
#include "uiGuidata.h"
#include "UIGlobalVar.h"

namespace GUI
{
class CForm;
class CCheckBox;
class CTreeView;
class CLabel;
class CTextButton;

class CEditor : public CUIInterface
{
public:
    CEditor();
    bool Init();			// 

    bool Error(const char* strInfo, const char* strFormName, const char* strCompentName) {
        { char _buf[512]; snprintf(_buf, sizeof(_buf), strInfo, strFormName, strCompentName); g_logManager.InternalLog(LogLevel::Error, "errors", _buf); }
        return false;
    }
    void SetEnabled( bool v );

    CForm		*frmEditor;
    CTreeView	*trvEditor;
    CTextButton *btnColor;
    CCheckBox	*chkSize01;
    CCheckBox	*chkSize02;
    CCheckBox	*chkSize03;
    CImage		*imgEditor;
    CCheckGroup	*chgGroup;
    CCheckBox	*chkWireFrame;
    CCheckBox	*chkEnabledAlpha;
    CCheckBox	*chkSmooth;
    CCheckBox   *chkShowLightBrush ;
    CCheckBox	*chkHideTree;
    CFrameImage	*imgTreeFrame;
    CCheckBox	*chkModifyHeight;
    CCheckBox	*chkColor;
    CCheckBox	*ckhHideOptionEx;
    CImage		*imgEditorEx;
    CEdit		*edtSetMainChaPos;
    CEdit		*edtSetBrushHeight;
    CLabel		*lblSetMainChaPos;
    CLabel		*lblSetBrushHeight;
    CCheckBox	*ckhShowCompass;
    CEdit		*edtSetIslandIndex;
    CLabel		*lblSetIslandIndex;
    CCheckBox   *chkEraseAttrib;
    CLabel      *lblEraseAttrib;
    CLabel      *lblShotMiniMap;

public:
    bool GetIsShowCompass();

private:
    static void _TreeMouseDown(CGuiData *pSender, int x, int y, DWORD key);
    static void _TreeSelectLost(CGuiData *pSender);
    static void _ColorMouseDown(CGuiData *pSender, int x, int y, DWORD key);
    static void _CheckSelectChange(CGuiData *pSender);
    static void _WireCheckChange(CGuiData *pSender);
    static void _SmoothCheckChange(CGuiData *pSender);

    static void _ShowLightBrushCheckChange(CGuiData *pSender);


    static void _HideTreeCheckChange(CGuiData *pSender);
    static void _ModifyHeightCheckChange(CGuiData *pSender);
    static void _EnabledAlphaCheckChange(CGuiData *pSender);
    static void _ShowEditor(CGuiData *pSender);
    static void _HideEditor(CGuiData *pSender);
    static bool _evtSetChaPosKeyChar(CGuiData *pSender, char& key);
    static bool _evtShotMiniMap(CGuiData *pSender, char& key);
    static bool _evtSetBrushHeightKeyChar(CGuiData *pSender, char& key);
    static void _ColorCheckChange(CGuiData *pSender);
    static void _HideOptionExCheckChange(CGuiData *pSender);
    static void _ShowCompassCheckChange(CGuiData *pSender);
    static bool _evtSetIslandIndex(CGuiData *pSender, char& key);
    static void _EraseAttribCheckChange(CGuiData* pSender);

private:
    void _TreeEvent(CTreeView* tree, bool press );
    void _ClearCheckState( CCheckBox* pCheckBox );

};


}
