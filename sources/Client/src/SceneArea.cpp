#include	"Stdafx.h"
#include	"SceneArea.h"
#include	"GameConfig.h"

CSceneArea::CSceneArea() {
	m_bInitSuccess = false;
	m_lUnitNum = 0;
	m_fRdWr = NULL;
}

CSceneArea::~CSceneArea() {
	if (m_fRdWr)
		fclose(m_fRdWr);
}

long CSceneArea::Init(_TCHAR* ptcsAreaFile, bool bSilence) {
	long lRet = 1;
	_TCHAR tcsPrint[256];

	if (ptcsAreaFile == NULL)
		return -1;

	if (m_bInitSuccess)
		Free();

	if (GlobalAppConfig.IsEditor())
		m_fRdWr = _tfopen(ptcsAreaFile, _TEXT("r+b"));
	else
		m_fRdWr = _tfopen(ptcsAreaFile, _TEXT("rb"));
	if (m_fRdWr == NULL) {
		if (!bSilence) {
			_stprintf(tcsPrint, _TEXT("%s %s"), ptcsAreaFile, GetLanguageString(339).c_str());
			MessageBox(NULL, tcsPrint, GetLanguageString(25).c_str(), 0);
		}
		lRet = -1;
		goto end;
	}
	fread(&m_SFileHead, sizeof(SFileHead), 1, m_fRdWr);
	if (m_SFileHead.lVersion != SCENE_AREA_FILE_VER100) {
		if (!bSilence) {
			_stprintf(tcsPrint, _TEXT("%s %s"), ptcsAreaFile, GetLanguageString(340).c_str());
			MessageBox(NULL, tcsPrint, GetLanguageString(25).c_str(), 0);
		}
		lRet = -2;
		goto end;
	}

	m_lUnitNum = m_SFileHead.iSceneWidth * m_SFileHead.iSceneHeight;
	m_bInitSuccess = true;

end:
	if (lRet < 0)
		if (m_fRdWr)
			fclose(m_fRdWr);
	return lRet;
}

void CSceneArea::Free() {
	if (m_bInitSuccess) {
		if (m_fRdWr) {
			fclose(m_fRdWr);
			m_fRdWr = NULL;
		}
	}
	m_lUnitNum = 0;
	m_bInitSuccess = false;
}

long CSceneArea::CreateFile(_TCHAR* ptcsAreaFile, int iSceneWidth, int iSceneHeight) {
	if (m_bInitSuccess)
		Free();

	FILE* fFile = NULL;
	SFileHead SHead;
	SAreaUnit* pSAreaUnit = NULL;

	if (GlobalAppConfig.IsEditor())
		fFile = _tfopen(ptcsAreaFile, _TEXT("wb"));
	else
		fFile = _tfopen(ptcsAreaFile, _TEXT("rb"));
	if (fFile == NULL)
		return -1;

	_tcscpy(SHead.tcsTitle, _TEXT("HF Scene Area File!"));
	SHead.lVersion = SCENE_AREA_FILE_VER100;
	SHead.lFileSize = sizeof(SFileHead) + sizeof(SAreaUnit) * iSceneWidth * iSceneHeight;

	pSAreaUnit = new (SAreaUnit[iSceneWidth * iSceneHeight]);
	if (pSAreaUnit == NULL) {
		fclose(fFile);
		return -2;
	}

	fwrite(&SHead, sizeof(SFileHead), 1, fFile);
	fwrite(pSAreaUnit, sizeof(SAreaUnit), iSceneWidth * iSceneHeight, fFile);

	if (fFile)
		fclose(fFile);
	if (pSAreaUnit)
		delete [] pSAreaUnit;
	return 1;
}

long CSceneArea::CreateFileFromMap(_TCHAR* ptcsMapFile, _TCHAR* ptcsAreaFile) {
	long lRet = 1;

	//CreateFile(ptcsAreaFile);
	//Init(ptcsAreaFile);

	return lRet;
}

// lUnitNO 
long CSceneArea::ReadAreaInfo(long lUnitNO, SAreaUnit* pUnitData, long* lpUnitNum) {
	if (!m_bInitSuccess)
		return -1;
	if (lUnitNO >= m_lUnitNum)
		return -1;
	if (lUnitNO + *lpUnitNum > m_lUnitNum)
		*lpUnitNum = m_lUnitNum - lUnitNO;

	fseek(m_fRdWr, sizeof(SFileHead) + sizeof(SAreaUnit) * lUnitNO, SEEK_SET);
	fread(pUnitData, sizeof(SAreaUnit), *lpUnitNum, m_fRdWr);

	return 1;
}

long CSceneArea::WriteAreaInfo(long lUnitNO, SAreaUnit* pUnitData, long* lpUnitNum) {
	if (!m_bInitSuccess)
		return -1;
	if (lUnitNO >= m_lUnitNum)
		return -1;
	if (lUnitNO + *lpUnitNum > m_lUnitNum)
		*lpUnitNum = m_lUnitNum - lUnitNO;

	fseek(m_fRdWr, sizeof(SFileHead) + sizeof(SAreaUnit) * lUnitNO, SEEK_SET);
	fwrite(pUnitData, sizeof(SAreaUnit), *lpUnitNum, m_fRdWr);

	return 1;
}
