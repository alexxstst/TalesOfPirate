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

			rv->SetName(std::format("{}{}", rv->GetName(), nCountForm++).c_str());

			_vfrm.push_back(rv);
			_nCount++;
		}
	}
	else {
		rv = _vfrm[_nCount++];
	}

	if (_nCount > 30) {
		g_logManager.InternalLog(LogLevel::Error, "errors",
								 SafeVFormat(GetLanguageString(490), rv->GetName(), _nCount));
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
	for (vfrm::iterator it = _vfrm.begin(); it != _vfrm.end(); it++)
		if (!(*it)->GetIsShow())
			return *it;

	CForm* frm = dynamic_cast<CForm*>(_vfrm[0]->Clone());
	if (frm) {
		CFormMgr::s_Mgr.AddForm(frm, enumMainForm);

		frm->SetName(std::format("{}{}", frm->GetName(), nCountForm++).c_str());
		frm->Init();

		_vfrm.push_back(frm);
	}

	if (_vfrm.size() > 30) {
		g_logManager.InternalLog(LogLevel::Error, "errors",
								 SafeVFormat(GetLanguageString(491), frm->GetName(), _vfrm.size()));
	}

	return frm;
}

void CHideForm::CloseAll() {
	for (vfrm::iterator it = _vfrm.begin(); it != _vfrm.end(); it++)
		(*it)->Close();
}
