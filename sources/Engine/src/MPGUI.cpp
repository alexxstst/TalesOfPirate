#include "Stdafx.h"
#include "MPGUI.h"
#include <algorithm>


MINDPOWER_API CGuiMgr g_GuiMgr;

CGuiMgr::CGuiMgr()
{
    _pFocusObj        = NULL;
    _CallBack_MsgProc = NULL;
    _bEditor          = FALSE;
	_bShareMsg		  = FALSE;
	_nIDCnt           = 0;
}

CGuiMgr::~CGuiMgr()
{
    SaveResource("resource/gui.res");
    
    vector<CGuiObj*> ObjList;
    
    for(list<CGuiObj*>::iterator it = _GuiObjList.begin(); it!=_GuiObjList.end(); it++)
    {
        CGuiObj *pObj = (*it);
        if(pObj->GetParent()==NULL)
        {
            ObjList.push_back(pObj);
        }
    }
    
    for(unsigned int i = 0; i < ObjList.size(); i++)
    {
        CGuiObj *pObj = ObjList[i];
        delete pObj;
    }
    ObjList.clear();
    _GuiObjList.clear();
}



int CGuiMgr::HandleMsg(unsigned long dwMsgType, DWORD dwParam1, DWORD dwParam2)
{
    if(!IsVisible())
    {
        return 0;
    }
    
    switch(dwMsgType)
    {
        case MSG_GUI_MOUSEDOWN:
        {
			if(_MouseDown(dwParam1, dwParam2))
            {
				if(_bShareMsg)
                {
					return 0;
                }
                else
                {
					return 1;
                }
             }
             break;
        }
        case MSG_GUI_MOUSEUP:
		{
			_pFocusObj = NULL;
            if(_MouseUp(dwParam1, dwParam2))
            {
				if(_bShareMsg)
                {
					return 0;
                }
                else
                {
                    return 1;
                }
            }
            break;
        }
        case MSG_GUI_MOUSEMOVE:
        {
			int nMouseX = dwParam1;
			int nMouseY = dwParam2;
			if(_bEditor && _pFocusObj) // && 
            {
				_pFocusObj->SetPos(_pFocusObj->GetX() + nMouseX - _nLastMouseX, _pFocusObj->GetY() + nMouseY - _nLastMouseY);
            }
             
            _nLastMouseX = nMouseX;
            _nLastMouseY = nMouseY;
                 
            if(_MouseMove(dwParam1, dwParam2))
            {
				if(_bShareMsg)
                {
					return 0;
                }
                else
                {
					return 1;
                }
			}
			break;
        }
    }
    return 0;
}

void CGuiMgr::FrameMove(DWORD dwTimeParam)
{
}

void CGuiMgr::_RenderGuiObj(CGuiObj *pObj)
{
	if(pObj->IsVisible()==FALSE) return;
		
	if(pObj->GetParent() && pObj->GetParent()->IsVisible()==FALSE) return;

	pObj->_Render();
	
	list<CGuiObj*>::iterator it;
	for(it = pObj->_ChildList.begin(); it!= pObj->_ChildList.end(); it++)
	{
		CGuiObj *pChildObj = (*it);
		_RenderGuiObj(pChildObj);
	}
}

void CGuiMgr::Render()
{
	list<CGuiObj*>::iterator it;
	for(it = _GuiObjList.begin(); it!= _GuiObjList.end(); it++)
	{
		CGuiObj *pObj = (*it);
		if(pObj->GetParent()==NULL)
		{
			_RenderGuiObj((*it));
		}
	}
}

/*int CGuiMgr::RenderAll()
{
    _bFocus = FALSE;
    list<CGuiObj*>::iterator it;
    for(it = _GuiObjList.begin() ; it!=_GuiObjList.end() ; it++)
    {
        CGuiObj *pObj = (*it);
        if(pObj->IsVisible()) 
        {
            if(pObj->_Render(&m_Input))
            {
                if(!_bFocus)
                {
                    pObj->SendMsg(MSG_GUI_HOVER , pObj->GetID() , 0);
                }
                _bFocus = TRUE;
            }
        }
    }
    return 1;
}*/

int CGuiMgr::_MouseDown(int nMouseX, int nMouseY)
{
    list<CGuiObj*>::iterator it;
    for(it = _GuiObjList.begin() ; it!=_GuiObjList.end() ; it++)
    {
        CGuiObj *pObj = (*it);
        if(pObj->IsEnable() && pObj->IsVisible() && pObj->_MouseDown(nMouseX, nMouseY))
        {
            //_pFocusObj = pObj;
            //if(pObj->GetParent()!=NULL)
            {
                //pObj->GetParent()->_ChildMouseDown(nMouseX, nMouseY, pObj);
            }
            return 1;
        }
    }
    return 0;
}

int CGuiMgr::_MouseUp(int nMouseX, int nMouseY)
{
    list<CGuiObj*>::iterator it;
    for(it = _GuiObjList.begin() ; it!=_GuiObjList.end() ; it++)
    {
        CGuiObj *pObj = (*it);
        if(pObj->IsVisible() && pObj->_MouseUp(nMouseX, nMouseY))
        {
            if(pObj->GetParent()!=NULL)
            {
				pObj->GetParent()->_ChildMouseUp(nMouseX, nMouseY , pObj);
			}
            return 1;
        }
    }
    return 0;
}

int CGuiMgr::_MouseMove(int nMouseX, int nMouseY)
{
    list<CGuiObj*>::iterator it;
    for(it = _GuiObjList.begin() ; it!=_GuiObjList.end() ; it++)
    {
        CGuiObj *pObj = (*it);
        if(pObj->IsVisible() && pObj->_MouseMove(nMouseX, nMouseY))
        {
            return 1;
        }
    }
	return 0;
}


void CGuiMgr::DebugOut()
{
    ToLogService("common", "All Gui Object List : ");
    list<CGuiObj*>::iterator it;
    int n = 0;
    for(it = _GuiObjList.begin() ; it!=_GuiObjList.end() ; it++)
    {
        CGuiObj *pObj = (*it);
        ToLogService("common", "Obj[{}], ID = {} ({}, {}) Show = {}", n,  pObj->GetID(), pObj->GetX(), pObj->GetY(), pObj->IsVisible());
        if(pObj->GetParent())
        {
            ToLogService("common", " , Parent ID = {}", pObj->GetParent()->GetID());
        }
        else
        {
            ToLogService("common", "");
        }
        n++;
    }
    ToLogService("common", "Gui Object List End");
}


void CGuiMgr::_Add(CGuiObj* pGuiObj)
{
    if(pGuiObj!=NULL)
    {
        CGuiObj *pParent = pGuiObj->GetParent();
        if(pParent!=NULL)
        {
			list<CGuiObj*>::iterator it = std::find(_GuiObjList.begin(), _GuiObjList.end(), pParent);
            if(it!=_GuiObjList.end())
            {
                _GuiObjList.insert(it , pGuiObj); // child obj should in front of it's parent
            }
            else
            {
                //  GUI-  
                g_logManager.InternalLog(LogLevel::Error, "common", std::format("ERR, GuiObj ID = <{}>, No Parent Found!", pGuiObj->GetID()));
            }
        }
        else
        {
            _GuiObjList.push_front(pGuiObj);
        }
    }
}

void CGuiMgr::_Remove(CGuiObj *pGuiObj)
{
    if(pGuiObj!=NULL)
    {
        _GuiObjList.remove(pGuiObj);
        //  GUI-
        g_logManager.InternalLog(LogLevel::Debug, "common", std::format("Remove Obj [{}]", pGuiObj->GetID()));
    }
}

CGuiObj *CGuiMgr::GetGuiObj(int nID)
{
    list<CGuiObj*>::iterator it;
    for(it = _GuiObjList.begin() ; it!=_GuiObjList.end() ; it++)
    {
        CGuiObj *pObj = (*it);
        if(pObj->GetID()==nID)
        {
            return pObj;
        }
    }
    return NULL;
}

int CGuiMgr::GetVisibleGuiObjCnt()
{
    int n = 0;
    list<CGuiObj*>::iterator it;
    for(it = _GuiObjList.begin() ; it!=_GuiObjList.end() ; it++)
    {
        CGuiObj *pObj = (*it);
        if(pObj->IsVisible())
        {
            n++;
        }
    }
    return n;
}

BOOL CGuiMgr::IsVisible()
{
    list<CGuiObj*>::iterator it;
    for(it = _GuiObjList.begin() ; it!=_GuiObjList.end() ; it++)
    {
        CGuiObj *pObj = (*it);
        if(pObj->IsVisible())
        {
            return TRUE;
        }
    }
    return FALSE;
}

void CGuiMgr::SaveResource(const char *pszFile)
{
    return;
	FILE *fp = fopen(pszFile, "wb");
    for(list<CGuiObj*>::iterator it = _GuiObjList.begin(); it!=_GuiObjList.end(); it++)
    {
        CGuiObj *pObj = (*it);
        int nID = pObj->GetID();
        int nX  = pObj->GetX();
        int nY  = pObj->GetY();
        fwrite(&nID, sizeof(int), 1, fp);
        fwrite(&nX, sizeof(int), 1, fp);
        fwrite(&nY, sizeof(int), 1, fp);
    }
    fclose(fp);
}

BOOL CGuiMgr::LoadFromResource(const char *pszFile)
{
    FILE *fp = fopen(pszFile, "rb");
    if(fp==NULL)
    {
        return false;
    }
    while(!feof(fp))
    {
        int nID, nX, nY;
        fread(&nID, sizeof(int), 1, fp);
        fread(&nX, sizeof(int), 1, fp);
        fread(&nY, sizeof(int), 1, fp);
        CGuiObj *pObj = g_GuiMgr.GetGuiObj(nID);
        if(pObj)
        {
            pObj->SetPos(nX, nY);
        }
    }
    fclose(fp);
    return true;
}

void CGuiMgr::SendMessage(int nMsgType, int nGuiObjID, DWORD dwParam1, DWORD dwParam2)
{
	// 
	if(nMsgType==MSG_GUI_BUTTON_CHECK && dwParam1==1) 
	{
		CGuiButton *pButton = (CGuiButton*)GetGuiObj(nGuiObjID);
		if(pButton->GetGroup())
		{
			for(list<CGuiObj*>::iterator it = _GuiObjList.begin(); it!=_GuiObjList.end(); it++)
			{
				CGuiObj *pObj = (*it);
				if(pObj!=pButton && pObj->GetType()==GUI_TYPE_BUTTON)
				{
					CGuiButton *pCur = (CGuiButton*)pObj;
					if(pCur->GetGroup()==pButton->GetGroup())
					{
                        if(pCur->IsChecked())
                        {
                            pCur->SetCheck(FALSE);
						    if(_CallBack_MsgProc)
						    {
							    long r = _CallBack_MsgProc(MSG_GUI_BUTTON_CHECK, pCur->GetID(), 0, 0);
						    }
                        }
					}
				}
			}
		}
	}	

	if(_CallBack_MsgProc)
	{
		long r = _CallBack_MsgProc(nMsgType, nGuiObjID, dwParam1, dwParam2);
	}
}




const int CGuiObj::STATE_NORMAL  = 0;
const int CGuiObj::STATE_HOVER   = 1;
const int CGuiObj::STATE_DOWN    = 2;
const int CGuiObj::STATE_DISABLE = 3;

CGuiObj::CGuiObj()
:_nID(0), _nX(0), _nY(0), _nW(0),_nH(0),
 _bShow(TRUE),
 _pParent(NULL),
 _nLayer(0),
 _bEnable(TRUE),
 _nCurState(STATE_NORMAL)
{
	_nStateMask[STATE_NORMAL]  = 1;
    _nStateMask[STATE_HOVER]   = 0;
    _nStateMask[STATE_DOWN]    = 0;
    _nStateMask[STATE_DISABLE] = 0;

	_szName[0] = 0;
    
	g_GuiMgr._Add(this);
}


BOOL CGuiObj::SetParent(CGuiObj *pParent)
{
    if(pParent==NULL)
    {
        return FALSE;
    }
    if(pParent!=_pParent)
    {
        if(_pParent) _pParent->_RemoveChild(this);        
        _pParent = pParent;
        pParent->_AddChild(this);
    }
    return TRUE;
}


void CGuiObj::SetPos(int x , int y)   
{ 
    _nX = x; 
    _nY = y;
}

int CGuiObj::GetAbsX()
{
    int x = _nX;
    CGuiObj *pParent = _pParent;
    while(pParent)
    {
        x+=pParent->GetX();
        pParent = pParent->GetParent();
    }
    return x;
}

int CGuiObj::GetAbsY()
{
    int y = _nY;
    CGuiObj *pParent = _pParent;
    while(pParent)
    {
        y+=pParent->GetY();
        pParent = pParent->GetParent();
    }
    return y;
}

void CGuiObj::SetSize(int w , int h)
{ 
    _nW = w ; 
    _nH = h ; 
}

void CGuiObj::Show(BOOL bShow)
{
   _bShow = bShow;
}

//--------------
//  
//--------------
void CGuiObj::SetLayer(int nLayer)
{
    _nLayer = nLayer;
}

BOOL CGuiObj::HitTest(int x , int y)
{
    int x1 = GetAbsX() + 1;
    int x2 = x1 + _nW - 2;
    int y1 = GetAbsY() + 1;
    int y2 = y1 + _nH - 2;
    if(x > x1 && x < x2 && y > y1 && y < y2)
    {
        return TRUE;
    }
    return FALSE;
}



CGuiObj::~CGuiObj() 
{
   g_GuiMgr._Remove(this);
   
   if(_pParent)
   {
       _pParent->_RemoveChild(this);
   }
   
   if(_ChildList.size() > 0)
   {
        vector<CGuiObj*> ObjList;
        for(list<CGuiObj*>::iterator it = _ChildList.begin() ; it!=_ChildList.end();it++)
        {
           ObjList.push_back((*it));
        }
        for(unsigned int i = 0; i < ObjList.size(); i++)
        {
            CGuiObj *pObj = ObjList[i];
            delete pObj; // 'Parent'
        }
        ObjList.clear();
        _ChildList.clear();
   }
}


/*CGuiLabel::CGuiLabel(int w, int h, int nID, CGuiObj *pParent)
{
    _nW = w;
    _nH = h;
    SetID(nID);
    SetParent(pParent);
    _nSprText     = 0;
    _nSprBkground = 0;
    _btNormalColor[0] = _btNormalColor[1]  = _btNormalColor[2] = 255;
    _btTextColor[0]   = _btTextColor[1]    = _btTextColor[2]   = 255;
    _btHoverColor[1]  = _btHoverColor[2]   =  0;
    _btHoverColor[0]  = 255;
    
    _nFontW    = 16; 
    _nFontH    = 16;
    _bBold     = FALSE;
    _bCanHover = FALSE;
}

CGuiLabel::~CGuiLabel()
{
    if(_nSprText)
    {
        //LzDeleteSprite(_nSprText);
    }
    if(_nSprBkground)
    {
        //LzDeleteSprite(_nSprBkground);
    }
}


void CGuiLabel::SetText(const char *pszText , int nFontW , int nFontH , BOOL bBold)
{
    _nFontW = nFontW;
    _nFontH = nFontH;
    _bBold  = bBold;
    
    //LzUseFontSet("Arial", _nFontW , _nFontH);
    strcpy(_szText , pszText);
    if(_nSprText==0)
    {
        _nSprText = 0;// CreateTextSprite(_szText , _nW , _nH , 0 , 0 , 0);
    }
    //LzClearSprite(_nSprText , 0 , 0 , 0 , 0);
	//LzDrawTextOnSprite(_nSprText , 0 , 0 , _nH - 1, _szText , _btTextColor[0] , _btTextColor[1] , _btTextColor[2]);
    if(_bBold)
    {
        //LzDrawTextOnSprite(_nSprText , 0 , 0 , _nH - 2 , _szText , _btTextColor[0] , _btTextColor[1] , _btTextColor[2]);
    }
    // LzUseFontSet("Arial", 12 , 12);
}

void CGuiLabel::SetText(int nValue , int nFontW , int nFontH , BOOL bBold)
{
    char szValue[12]; itoa(nValue , szValue , 10);
    SetText(szValue , nFontW , nFontH , bBold);
}

void CGuiLabel::SetTextColor(BYTE r , BYTE g , BYTE b)
{
    _btTextColor[0] = r;
    _btTextColor[1] = g;
    _btTextColor[2] = b;
    SetText(_szText , _nFontW , _nFontH , _bBold);
}

BOOL CGuiLabel::LoadImage(char *pszBackground , char *pszPath  , int nPage)
{
    if(pszPath==NULL)
    {
        return FALSE;
    }
    string strPath = pszPath;
    strPath+="/";
    
    int nColorKey[3] = { 0 , 0 , 240 };
    if(pszBackground)
    {
        string str = strPath;
        str+=pszBackground;
        _nSprBkground = 0; //CreateImageSprite(str.c_str()  , _nW , _nH , nColorKey[0] , nColorKey[1] , nColorKey[2] , nPage);
        // LzSetSpriteVisible(_nSprBkground , FALSE);
        return TRUE;
    }
    return FALSE;
}

int CGuiLabel::_MouseUp(int x , int y)
{
    if(!_bEnable)   return 0;
    if(!_bCanHover) return 0;
    
    if(HitTest(x , y))
    {
        SendMsg(MSG_GUI_LABEL_CLICK, _nID, _nAccessory);
        return 1;
    }
    return 0;
}*/


CGuiButton::CGuiButton(int nWidth, int nHeight, int nID, CGuiObj *pParent)
{
    _nCurState = STATE_NORMAL;
    _nStateMask[STATE_HOVER]   = 1;
	_nStateMask[STATE_DOWN]    = 1;
	_nStateMask[STATE_DISABLE] = 1;
    _bCheckBox = FALSE;
    _nW = nWidth;
    _nH = nHeight;
    SetID(nID);
    SetParent(pParent);
	_nGroupNo = 0;
}

CGuiButton::~CGuiButton()
{
}


void CGuiButton::SetCheck(BOOL bCheck)
{
    if(bCheck)
    {
        _nCurState = STATE_DOWN;
		SendMsg(MSG_GUI_BUTTON_CHECK, _nID);
    }
    else
    {
		_nCurState = STATE_NORMAL;
		SendMsg(MSG_GUI_BUTTON_UNCHECK, _nID);
    }
}

int CGuiButton::_MouseDown(int x , int y)
{
    if(_nCurState==STATE_DISABLE) return 0;
    
    if(HitTest(x, y))
    {
        if(!_bCheckBox)
        {
            _nCurState = STATE_DOWN;
            SendMsg(MSG_GUI_BUTTON_DOWN , _nID);
        }
        
        return 1;
    }
    return 0;
}

int CGuiButton::_MouseUp(int x , int y)
{
    if(_nCurState==STATE_DISABLE) return 0;
    
    if(HitTest(x , y))
    {
        if(!_bCheckBox)
        {
            //_ChangeState(STATE_DOWN);
            //SendMsg(MSG_GUI_BUTTON_DOWN , _nID);
        }
        else
        {
            if(_nCurState!=STATE_DOWN)
            {
                _nCurState = STATE_DOWN;
                SendMsg(MSG_GUI_BUTTON_DOWN, _nID);
                SendMsg(MSG_GUI_BUTTON_CHECK, _nID, 1);
            }
            else
            {
                _nCurState = STATE_NORMAL;
                SendMsg(MSG_GUI_BUTTON_CHECK, _nID, 0);
            }
        }
        return 1;
    }

    if(!_bCheckBox)
    {
        _nCurState = STATE_NORMAL;
    }
    return 0;    
}

int CGuiButton::_MouseMove(int nMouseX, int nMouseY)
{
    if(_nCurState==STATE_DISABLE) return 0;
    if(_nStateMask[STATE_HOVER]==0) return 0;
    
    int nNewState = _nCurState;

	BOOL bHit = HitTest(nMouseX, nMouseY);
    if(bHit)
    {
        if(_nCurState!=STATE_DOWN)
        {
            if(_nCurState!=STATE_HOVER)
            {
                // ShowHint(_nID);
            }
            _nCurState = STATE_HOVER;
        }
    }
    else
    {
       	if(!(_bCheckBox && _nCurState==STATE_DOWN))
        {
			 _nCurState = STATE_NORMAL;
		}
    }
    return bHit;
}





CGuiButtonGroup::CGuiButtonGroup(int nID , CGuiObj *pParent)
{
    _nMaxColumn       = 10;
    _pSelButton       = NULL;
    _bEnableSelect    = TRUE;
    SetID(nID);
    SetParent(pParent);
}

int CGuiButtonGroup::_ChildMouseDown(int x , int y , CGuiObj *pChild)
{
    return 0;
    if(pChild!=NULL)
    {
        if(!_bEnableSelect) return 0;
        
        SelectButton((CGuiButton*)pChild);
        
        CGuiButton *pLastSel = _pSelButton;
        if(pLastSel!=NULL)
        {
            SendMsg(MSG_GUI_BUTTONGROUP_SELECT, pChild->GetID(), pLastSel->GetID());
        }
        else
        {
            SendMsg(MSG_GUI_BUTTONGROUP_SELECT, pChild->GetID(), 0);
        }
    }
    return 1;
}

int CGuiButtonGroup::_ChildMouseUp(int x , int y , CGuiObj *pChild)
{
    if(pChild!=NULL)
    {
        SelectButton((CGuiButton*)pChild);    
        
        CGuiButton *pLastSel = _pSelButton;
        if(pLastSel!=NULL)
        {
            SendMsg(MSG_GUI_BUTTONGROUP_SELECT, pChild->GetID(), pLastSel->GetID());
        }
        else
        {
            SendMsg(MSG_GUI_BUTTONGROUP_SELECT, pChild->GetID(), 0);
        }
        // SelectButton((CGuiButton*)pChild);
    }
    return 1;
}

void CGuiButtonGroup::ShowSelectRect(BOOL bShow)
{
    _bShowRect = bShow;
    // LzSetSpriteVisible(_nSprRect , _bShowRect);
}


int CGuiButtonGroup::_ChildHover(int x , int y , BOOL bHover , CGuiObj *pChild)
{
    if(pChild!=NULL)
    {
        // if(!_bEnableSelect) return 0;
        if(bHover)
        {
            _ChildHoverMap[pChild] = 1;
            if(_bShowRect)
            {
                //LzTranslateSprite(_nSprRect , pChild->GetAbsX() - 1 ,pChild->GetAbsY() - 1);
                //LzSetSpriteVisible(_nSprRect , TRUE);
            }
            SendMsg(MSG_GUI_BUTTONGROUP_HOVER , pChild->GetID() , 0);
        }
        else
        {
            _ChildHoverMap[pChild] = 0;
            if(_bShowRect)
            {
                BOOL bChildHover = FALSE;
                map<CGuiObj* , char>::iterator it;
                for(it = _ChildHoverMap.begin(); it!=_ChildHoverMap.end(); it++)
                {
                    if((*it).second==1)
                    {
                        bChildHover = TRUE;
                        break;
                    }
                }
                if(!bChildHover)
                {
                    //LzSetSpriteVisible(_nSprRect , FALSE);
                }
            }
        }
    }
    return 1;
}


BOOL CGuiButtonGroup::AddButton(CGuiButton *pButton)
{
    if(pButton==NULL)
    {
        return FALSE;
    }
    pButton->SetCheckBox(TRUE);
    if(pButton->GetParent()!=this)
    {
        _AddChild(pButton);
        //_UpdateShow();
    }
    return TRUE;
}

void CGuiButtonGroup::RemoveAllButton(BOOL bDelete)
{
    vector<CGuiButton*> ChildList;
    list<CGuiObj*>::iterator it;
    for(it = _ChildList.begin() ; it!=_ChildList.end() ; it++)
    {
        CGuiButton *pButton = (CGuiButton*)(*it);
        ChildList.push_back(pButton);
    }
    _ChildHoverMap.clear();

    if(bDelete)
    {
        for(unsigned int i = 0 ; i < ChildList.size() ; i++)
        {
            CGuiButton *pButton = ChildList[i];
            delete pButton;
        }
    }
    ChildList.clear();
    _ChildList.clear();
}

CGuiButton *CGuiButtonGroup::FindButton(int nAccessory)
{
    list<CGuiObj*>::iterator it;
    for(it = _ChildList.begin() ; it!=_ChildList.end() ; it++)
    {
        CGuiButton *pButton = (CGuiButton*)(*it);
        if(pButton->GetAccessory()==nAccessory)
        {
            return pButton; // get first one
        }
    }
    return NULL;
}


void CGuiButtonGroup::ResetButtonState()
{
    list<CGuiObj*>::iterator it;
    for(it = _ChildList.begin() ; it!=_ChildList.end() ; it++)
    {
        CGuiButton *pButton = (CGuiButton*)(*it);
        pButton->SetState(STATE_NORMAL);
        _ChildHoverMap[pButton] = 0;
    }
    if(_bShowRect)
    {
        // LzSetSpriteVisible(_nSprRect , FALSE);
    }
    // _UpdateShow();
}
    

void CGuiButtonGroup::SelectButton(CGuiButton *pSelButton)
{
    if(pSelButton)
    {
        _pSelButton = pSelButton;
        
        _pSelButton->SetCheck(TRUE);
        // pSelButton->_ChangeState(CGuiButton::STATE_DOWN);
        if(_nSprSelectedMark!=0)
        {
            //LzTranslateSprite(_nSprSelectedMark, pSelButton->GetAbsX() - 1, pSelButton->GetAbsY() - 1);
            //LzSetSpriteVisible(_nSprSelectedMark, TRUE);
        }
    }
    else
    {
        if(_nSprSelectedMark!=0)
        {
            //LzSetSpriteVisible(_nSprSelectedMark , FALSE);
        }
    }

    
    list<CGuiObj*>::iterator it;
    for(it = _ChildList.begin() ; it!=_ChildList.end() ; it++)
    {
        CGuiButton *pButton = (CGuiButton*)(*it);
        if(pButton!=pSelButton)
        {
            pButton->SetCheck(FALSE);
            // pButton->_ChangeState(CGuiButton::STATE_NORMAL);
        }
    }
}

void CGuiButtonGroup::EnableSelect(BOOL bEnable)
{
    _bEnableSelect = bEnable;
    ShowSelectRect(_bEnableSelect);
    if(bEnable==FALSE)
    {
        // LzSetSpriteVisible(_nSprSelectedMark , FALSE);
    }
}




CGuiWindow::CGuiWindow(int w, int h , int nID)
{
    _nW               = w;
    _nH               = h;
    SetID(nID);
    _nAutoHideTickCnt = 0;
    _nSlideMode       = 0;
}

CGuiWindow::~CGuiWindow()
{
}

int CGuiWindow::_MouseDown(int x , int y)
{
    if(HitTest(x , y))
    {
        return SendMsg(MSG_GUI_WINDOW_CLICK, _nID, ((x&0x0FFFF)<<16)|(y&0x0FFFF));
    }
    return 0;
}

void CGuiWindow::_FrameMove(DWORD dwTimeParam)
{
    if(!_bEnable) return;
    if(!_bShow)   return;
    
    if(_nAutoHideTickCnt > 0)
    {
        _nAutoHideTickCnt--;
        if(_nAutoHideTickCnt==0)
        {
            Show(FALSE);
        }
    }

    if(_nSlideMode > 0)
    {
        _SlideMove();
    }
    return;
}

void CGuiWindow::_SlideMove()
{
    /*BOOL bStop = FALSE;

    if( (_fSlideAccelSpeed * _fSlideInitSpeed) < 0.0f)
    {
        if((_fSlideSpeed * _fSlideInitSpeed) < 0.0f)
        {
            bStop = TRUE;
        }
    }
    else // speed up
    {
        if((_nX + _nW) < 0 || _nX > SCREENSIZEX || (_nY + _nH) < 0 || _nY > SCREENSIZEY)
        {
            bStop = TRUE;
        }
    }
    
    if(!bStop) 
    {
        float fTick = (float)_nSlideTick;
        _fSlideSpeed = _fSlideInitSpeed + _fSlideAccelSpeed * fTick;
        float fSlide = _fSlideInitSpeed * fTick + 0.5f * _fSlideAccelSpeed * fTick * fTick;
        _nSlideTick++;
        
        switch(_nSlideMode)
        {
            case 1: // horizon
                {
                    SetPos(_nSlideStart + (int)fSlide , _nY);
                    break;
                }
            case 2: // vertical
                {
                    SetPos(_nX , _nSlideStart + (int)fSlide);
                    break;
                }
        }
    }
    else
    {
        _nSlideMode = 0;   
        if(_bSlideHide)
        {
            Show(FALSE);
        }
    }*/
}

void CGuiWindow::SetSlideMode(int nMode , int nStart , float fInitSpeed , float fAccelSpeed , BOOL bHide)
{
    _nSlideMode       = nMode;
    _fSlideInitSpeed  = fInitSpeed;
    _fSlideAccelSpeed = fAccelSpeed;
    _fSlideSpeed      = fInitSpeed;
    _nSlide           = 0;
    _nSlideTick       = 0;
    _nSlideStart      = nStart;

    _bSlideHide       = bHide;
    
    switch(_nSlideMode)
    {
        case 1: // horizon
            {
                SetPos(_nSlideStart  , _nY);
                break;
            }
        case 2: // vertical
            {
                SetPos(_nX , _nSlideStart);
                break;
            }
    }
}

/*
CGuiPattern::CGuiPattern(int w , int h , int nID , CGuiObj *pParent)
{
    _nW = w;
    _nH = h;
    SetID(nID);
    SetParent(pParent);
    
    _nSprHeader   = 0;
    _nSprTail     = 0;
    _nBodyCnt     = 0;
    _nHalfBody    = 0;
    _nHeaderSizeX = 0;
    _nBodySizeX   = 0;
    _nTailSizeX   = 0;
    _nHeaderSizeY = 0;
    _nBodySizeY   = 0;
    _nTailSizeY   = 0;
    for(int i = 0 ; i < MAX_PATTERN_BODY ; i++)
    {
        _nSprBody[i] = 0;
    }
    SetLayer(LAYER_PATTERN);
}

BOOL CGuiPattern::LoadImage(char *pszHeader, int nHeaderSizeX, int nHeaderSizeY,
               char *pszBody, int nBodySizeX, int nBodySizeY, 
               char *pszTail, int nTailSizeX, int nTailSizeY,
               char *pszPath, int nPage)

// BOOL CGuiPattern::LoadImage(char *pszHeader , int nHeaderSize , char *pszBody , int nBodySize , char *pszTail , int nTailSize , char *pszPath , int nPage)
{
    string strPath = pszPath;
    strPath+="/";
    
    _nHeaderSizeX = nHeaderSizeX;
    _nHeaderSizeY = nHeaderSizeY;
    _nBodySizeX   = nBodySizeX;
    _nBodySizeY   = nBodySizeY;
    _nTailSizeX   = nTailSizeX;
    _nTailSizeY   = nTailSizeY;

    _nBodyCnt    = (_nW  - nHeaderSizeX - _nTailSizeX) / nBodySizeX;
    _nHalfBody   = (_nW  - nHeaderSizeX - _nTailSizeX) % nBodySizeX;


    _nBodyTotalSize = _nBodyCnt * _nBodySizeX + _nHalfBody;

    //_nW = _nHeaderSize + _nTailSize + _nBodySize * _nBodyCnt; // update new width
    
    int nColorKey[3] = { 0, 0, 240 };
    if(pszHeader)
    {
        string str = strPath;
        str+=pszHeader;
        _nSprHeader = CreateImageSprite(str.c_str(), _nHeaderSizeX, _nHeaderSizeY , nColorKey[0] , nColorKey[1] , nColorKey[2] , nPage);
        LzSetSpriteVisible(_nSprHeader, TRUE);
    }
    
    if(pszBody)
    {
        int i;
        string str = strPath;
        str+=pszBody;
        _nSprBody[0] = CreateImageSprite(str.c_str()  , _nBodySizeX , _nBodySizeY , nColorKey[0] , nColorKey[1] , nColorKey[2] , nPage);
        LzSetSpriteVisible(_nSprBody[0], TRUE);
        for(i = 1 ; i < _nBodyCnt ; i++)
        {
            _nSprBody[i] = LzCloneSprite(_nSprBody[0]);
            LzSetSpriteColorKey(_nSprBody[i], nColorKey[0] , nColorKey[1] , nColorKey[2] , TRUE);
            LzSetSpriteVisible(_nSprBody[i], TRUE);
        }
        if (_nHalfBody)
        {
            _nSprBody[i] = LzCloneSprite(_nSprBody[0]);
            LzSetSpriteColorKey(_nSprBody[i], nColorKey[0] , nColorKey[1] , nColorKey[2] , TRUE);
            LzSetSpriteVisible(_nSprBody[i], TRUE);
        }
    }

    if(pszTail)
    {
        string str = strPath;
        str+=pszTail;
        _nSprTail = CreateImageSprite(str.c_str()  , _nTailSizeX , _nTailSizeY, nColorKey[0] , nColorKey[1] , nColorKey[2] , nPage);
        LzSetSpriteVisible(_nSprTail , TRUE);
    }
    return TRUE;
}

int CGuiPattern::_UpdatePos()
{
    int x = GetAbsX();
    int y = GetAbsY();
    int i;
    LzTranslateSprite(_nSprHeader, x, y);
    x+=_nHeaderSizeX;
    int nBodyOffY = _nHeaderSizeY - _nBodySizeY;
    int nTailOffY = _nHeaderSizeY - _nTailSizeY;
    for(i = 0 ; i < _nBodyCnt ; i++)
    {
        LzTranslateSprite(_nSprBody[i], x, y + nBodyOffY);
        x+=_nBodySizeX;
    }
    if (_nHalfBody)
    {
        LzTranslateSprite(_nSprBody[i] , x + _nHalfBody - _nBodySizeX, y + nBodyOffY);
        x+=_nHalfBody;
    }
    LzTranslateSprite(_nSprTail , x , y + nTailOffY);
    return 1;
}

int CGuiPattern::_UpdateShow()
{
    LzSetSpriteVisible(_nSprHeader , _bShow);
    for(int i = 0 ; i < _nBodyCnt ; i++)
    {
        LzSetSpriteVisible(_nSprBody[i] , _bShow);
    }
    if (_nHalfBody)
    {
        LzSetSpriteVisible(_nSprBody[i] , _bShow);
    }
    LzSetSpriteVisible(_nSprTail , _bShow);
    return 1;
}

int CGuiPattern::_UpdateLayer()
{
    LzSetSpriteHotSpot(_nSprHeader, 0, 0, _nLayer + 1);
    for(int i = 0 ; i < _nBodyCnt ; i++)
    {
        LzSetSpriteHotSpot(_nSprBody[i], 0, 0, _nLayer + 1);
    }
    if (_nHalfBody)
    {
        LzSetSpriteHotSpot(_nSprBody[i], 0, 0, _nLayer + 1);
    }
    LzSetSpriteHotSpot(_nSprTail, 0, 0, _nLayer + 1);
    return 1;
}




CGuiClipBar::CGuiClipBar(int w , int h , int nID , CGuiObj *pParent)
{
    _nW = w;
    _nH = h;
    SetID(nID);
    SetParent(pParent);
    _nSprBar = 0;
    _nMax    = 0;
    _nNow    = 0;
    SetLayer(LAYER_CLIPBAR);
}

CGuiClipBar::~CGuiClipBar()
{
    if(_nSprBar!=0)
    {
        LzDeleteSprite(_nSprBar);
    }
}
    

BOOL CGuiClipBar::LoadImage(char *pszBar , char *pszPath , int nPage)
{
    string strPath = pszPath;
    strPath+="/";
    int nColorKey[3] = { 0 , 0 , 240 };
    if(pszBar)
    {
        string str = strPath;
        str+=pszBar;
        _nSprBar = CreateImageSprite(str.c_str()  , _nW , _nH , nColorKey[0] , nColorKey[1] , nColorKey[2]);
    
    }
    return TRUE;
}

int CGuiClipBar::_UpdatePos()
{
    if(_nSprBar)
    {
        LzTranslateSprite(_nSprBar , GetAbsX() , GetAbsY());
    }
    return 1;
}

int CGuiClipBar::_UpdateShow()
{
   if(_nSprBar)
   {
       LzSetSpriteVisible(_nSprBar , _bShow);
       if(_bShow)
       {
           float r = (float)_nNow / _nMax;
           int nBarLen = (int)((float)_nW * r);
           if(nBarLen==0)
           {
                if(r > 0.001f)
                {
                    nBarLen = 1;   
                }
           }
           LzSetSpriteCrop(_nSprBar , 0 , 0 , nBarLen , _nH);
       }
   }
   return 1;
}

int CGuiClipBar::_UpdateLayer()
{
   if(_nSprBar)
   {
       LzSetSpriteHotSpot(_nSprBar, 0, 0, _nLayer + 1);
   }
   return 1;
}

void CGuiClipBar::SetBarRange(int nNow , int nMax)
{
    _nNow = nNow;
    _nMax = nMax;
    _UpdateShow();
}*/
