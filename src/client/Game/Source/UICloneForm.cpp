#include "StdAfx.h"
#include "uicloneform.h"
#include "uiform.h"
#include "uitemplete.h"
#include "uiformmgr.h"

using namespace GUI;

static int nCountForm = 0;

//---------------------------------------------------------------------------
// class CCloneForm
//---------------------------------------------------------------------------
CCloneForm::CCloneForm()
	: _pSample(NULL), _nCount(0) {
}

CCloneForm::~CCloneForm() {
}

CForm* CCloneForm::Clone() {
	CForm* rv = NULL;
	if (_nCount >= (int)_vfrm.size()) {
		rv = dynamic_cast<CForm*>(_pSample->Clone());
		if (rv) {
			CFormMgr::s_Mgr.AddForm(rv, enumMainForm);
			rv->Init();

			char buf[80] = {0};
			sprintf(buf, "%s%d", rv->GetName(), nCountForm++);
			rv->SetName(buf);

			_vfrm.push_back(rv);
			_nCount++;
		}
	}
	else {
		rv = _vfrm[_nCount++];
	}

	if (_nCount > 30) {
		ToLogService("error", "{} {} {}", g_oLangRec.GetString(490), rv->GetName(), _nCount);
	}
	return rv;
}

bool CCloneForm::Release(CForm* p) {
	for (int i = 0; i < _nCount; i++) {
		if (_vfrm[i] == p) {
			p->Close();
			_nCount--;

			if (i != _nCount) {
				//
				CForm* tmp = _vfrm[i];
				_vfrm[i] = _vfrm[_nCount];
				_vfrm[_nCount] = tmp;
			}
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------
// class CHideForm
//---------------------------------------------------------------------------
CForm* CHideForm::GetHide() {
	for (auto it = _vfrm.begin(); it != _vfrm.end(); it++)
		if (!(*it)->GetIsShow())
			return *it;

	CForm* frm = dynamic_cast<CForm*>(_vfrm[0]->Clone());
	if (frm) {
		CFormMgr::s_Mgr.AddForm(frm, enumMainForm);

		char buf[80] = {0};
		sprintf(buf, "%s%d", frm->GetName(), nCountForm++);
		frm->SetName(buf);
		frm->Init();

		_vfrm.push_back(frm);
	}

	if (_vfrm.size() > 30) {
		ToLogService("error", "{} {} {}", g_oLangRec.GetString(491), frm->GetName(), (int)_vfrm.size());
	}

	return frm;
}

void CHideForm::CloseAll() {
	for (auto it = _vfrm.begin(); it != _vfrm.end(); it++)
		(*it)->Close();
}
