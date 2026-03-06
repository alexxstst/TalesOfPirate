//------------------------------------------------------------------------
//	2005.4.27	Arcol	create this file
//------------------------------------------------------------------------

#include "stdafx.h"
#include "uiguildapply.h"
#include "netguild.h"
#include "UIFormMgr.h"
#include "UITextButton.h"
#include "UIBoxForm.h"
#include "UIEdit.h"
#include "GuildData.h"
#include "commfunc.h"

using namespace std;

CForm* CUIGuildApply::m_pGuildNameInputForm=NULL;
CEdit* CUIGuildApply::m_pGuildNameEdit=NULL;
CEdit* CUIGuildApply::m_pGuildPasswordEdit=NULL;
CEdit* CUIGuildApply::m_pGuildConfirmEdit=NULL;

CUIGuildApply::CUIGuildApply(void)
{
}

CUIGuildApply::~CUIGuildApply(void)
{
}

bool CUIGuildApply::Init()
{
	FORM_LOADING_CHECK(m_pGuildNameInputForm,"npc.lua","frmName");
	FORM_CONTROL_LOADING_CHECK(m_pGuildNameEdit,m_pGuildNameInputForm,CEdit,"npc.lua","edtName");
	FORM_CONTROL_LOADING_CHECK(m_pGuildPasswordEdit,m_pGuildNameInputForm,CEdit,"npc.lua","edtPCode");
	FORM_CONTROL_LOADING_CHECK(m_pGuildConfirmEdit,m_pGuildNameInputForm,CEdit,"npc.lua","edtPCode2");
	m_pGuildPasswordEdit->SetIsPassWord(true);
	m_pGuildConfirmEdit->SetIsPassWord(true);
	m_pGuildNameInputForm->evtEntrustMouseEvent = OnConfirm;
	m_pGuildNameInputForm->evtEscClose = OnEscClose;
	

	return true;
}

void CUIGuildApply::ShowForm()
{
	m_pGuildPasswordEdit->SetCaption("");
	m_pGuildConfirmEdit->SetCaption("");
	m_pGuildNameEdit->SetCaption("");
	m_pGuildNameEdit->SetIsEnabled(true);
	m_pGuildNameInputForm->ShowModal();
	m_pGuildNameEdit->SetActive(m_pGuildNameEdit);
}

//GBK
//nametrue;
//lenname=strlen(name),NULL
inline bool IsValidGuildName(const char *name,unsigned short len, bool bEng)
{
	const unsigned char *l_name =reinterpret_cast<const unsigned char *>(name);
	bool l_ishan	=false;
	for(unsigned short i=0;i<len;i++)
	{
		if(!l_name[i])
		{
			return false;
		}else if(l_ishan)
		{
			if(l_name[i-1] ==0xA1 && l_name[i] ==0xA1)	//
			{
				return false;
			}
			if(l_name[i] >0x3F && l_name[i] <0xFF && l_name[i] !=0x7F)
			{
				l_ishan =false;
			}else
			{
				return false;
			}
		}else if(l_name[i]>0x80 && l_name[i] <0xFF)
		{
			l_ishan	=true;
		}else if((l_name[i] >='A' && l_name[i] <='Z') ||(l_name[i] >='a' && l_name[i] <='z') ||(l_name[i] >='0' && l_name[i] <='9'))
		{
			
		}
		else if( bEng && l_name[i] == ' ' )
		{
		}
		else
		{
			return false;
		}
	}
	return !l_ishan;
}

void CUIGuildApply::OnEscClose(CForm* pForm)
{
	if (m_pGuildNameInputForm == pForm)
	{
		CM_GUILD_PUTNAME(false,"","");
		m_pGuildNameInputForm->SetIsShow(false);
	}
}
void CUIGuildApply::OnConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if (nMsgType!=CForm::mrYes )
	{
		CM_GUILD_PUTNAME(false,"","");
		return;
	}

	if ( strlen(m_pGuildNameEdit->GetCaption())>0)
	{
		string name = m_pGuildNameEdit->GetCaption();
        bool bEnglishName = true;
		// Xu qin added for illegal text filtering
		if( !CTextFilter::IsLegalText(CTextFilter::DIALOG_TABLE, name ) )
		{
			m_pGuildNameEdit->SetCaption("");
			CBoxMgr::ShowMsgBox( OnShowForm, "", true );
			return;
		}

        int nLen = (int)name.size();
        for(int i = 0; i < nLen; ++i)
        {
            if(name[i] & 0x80)
            {
                bEnglishName = false;
                break;
            }
        }

        if(!IsValidGuildName(name.c_str(), (unsigned short)name.length(), bEnglishName))
		{
			CBoxMgr::ShowMsgBox( OnShowForm, g_oLangRec.GetString(51), true );
		}
		else
		{
			string strPass=m_pGuildPasswordEdit->GetCaption();
			if ( strPass.length()>0 )
			{
				if ( strPass==m_pGuildConfirmEdit->GetCaption() )
				{
					CM_GUILD_PUTNAME(true,m_pGuildNameEdit->GetCaption(),m_pGuildPasswordEdit->GetCaption());
				}
				else
				{
					m_pGuildPasswordEdit->SetCaption("");
					m_pGuildConfirmEdit->SetCaption("");
					CBoxMgr::ShowMsgBox( OnShowForm, g_oLangRec.GetString(580), true );
				}
			}
			else
			{
				CBoxMgr::ShowMsgBox( OnShowForm, g_oLangRec.GetString(581), true );
			}
		}
	}
	else
	{
		CBoxMgr::ShowMsgBox( OnShowForm, g_oLangRec.GetString(582), true );
	}
}

void CUIGuildApply::OnShowForm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	m_pGuildNameInputForm->ShowModal();
	if ((strlen(m_pGuildNameEdit->GetCaption()) > 0))
	{
		m_pGuildPasswordEdit->SetActive(m_pGuildPasswordEdit);
	}
}
