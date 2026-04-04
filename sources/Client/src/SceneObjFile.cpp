#include	"Stdafx.h"
#include	<sys/stat.h>
#include	<sys/timeb.h>
#include	<time.h>
#include	"SceneObjFile.h"
#include	"GameConfig.h"
#include "scene.h"
#include "GameApp.h"
#include "SceneObj.h"
#include "SceneObjRecordStore.h"

CSceneObjFile	g_ObjFile;


CSceneObjFile::CSceneObjFile()
{
	m_bInitSuccess = false;
	m_fRdWr = NULL;
	m_fAppend = NULL;
	m_SSectionIndex = NULL;
}

CSceneObjFile::~CSceneObjFile()
{
	Free();
	//end_RBO();
}

// Added by clp
void CSceneObjFile::_init_RBO( const std::string &filename )
{
	filename_RBO = filename + "rbo";
	RBOinfoList.clear();
	//std::ifstream file ( filename_RBO.c_str() );
	//struct ReallyBigObjectInfo info;
	//while ( file >> info )
	//{
	//	RBOinfoList.insert( info );
	//}
	//file.close();
}

void CSceneObjFile::end_RBO()
{
	_Serialize_RBO();
	//_Serialize_RBO_ToMap();
}

void CSceneObjFile::_Serialize_RBO()
{
	if( !filename_RBO.empty() )
	{
		std::ofstream file ( filename_RBO.c_str() );
		std::set < struct ReallyBigObjectInfo >::iterator itr = RBOinfoList.begin();
		std::set < struct ReallyBigObjectInfo >::iterator end = RBOinfoList.end();
		while( itr != end )
		{
			file << *(itr++);
		}
		file.close();

		//FILE *file = fopen ( filename_RBO.c_str(), "wt" );
		//if( !file )
		//{
		//	return;
		//}

		//std::set < struct ReallyBigObjectInfo >::iterator itr = RBOinfoList.begin();
		//std::set < struct ReallyBigObjectInfo >::iterator end = RBOinfoList.end();
		//while( itr != end )
		//{
		//	file << *(itr++);
		//}
		//fclose ( file );
	}
}

void CSceneObjFile::_Serialize_RBO_ToMap()
{
	SSceneObjInfo	infoex[MAX_MAP_SECTION_OBJ];
	CGameScene* scene = CGameApp::GetCurScene();
	MPTerrain* terrain = scene->GetTerrain();

	std::set < struct ReallyBigObjectInfo >::iterator itr = RBOinfoList.begin();
	std::set < struct ReallyBigObjectInfo >::iterator end = RBOinfoList.end();

	while( itr != end )
	{
		int sectionX = itr->position.x / ( 100.f * terrain->GetSectionWidth() );
		int sectionY = itr->position.y / ( 100.f * terrain->GetSectionHeight() );

		int sectionNumber = sectionY * terrain->GetSectionCntX() + sectionX;

		long sectionObjCount = 0;

		if( ReadSectionObjInfo( sectionNumber, infoex, &sectionObjCount ) )
		{
			sectionObjCount++;
			int RBOIndex = sectionObjCount - 1;
			if( RBOIndex <= MAX_MAP_SECTION_OBJ )
			{
				infoex[RBOIndex].sTypeID    = itr->typeID;
				infoex[RBOIndex].nX         = itr->position.x;
				infoex[RBOIndex].nY         = itr->position.y;
				infoex[RBOIndex].sYawAngle  = itr->orientation.w;
				infoex[RBOIndex].sHeightOff = itr->position.z;
			}
			g_ObjFile.WriteSectionObjInfo(sectionNumber, infoex, sectionObjCount);
        }
		++itr;
	}
}

long CSceneObjFile::Init(const char *ptcsFileName, bool bSilence)
{
	long	lRet = 1;
	long	lFileSize;
	char	tcsPrint[256];
	FILE	*fFile = nullptr;

	Free();

	_tchmod(ptcsFileName, _S_IWRITE );

	fFile = fopen(ptcsFileName, "rb");

	// Added by clp
	std::string filename ( ptcsFileName );
	filename.erase ( filename.length() - 3 );
	_init_RBO( filename.c_str() );

	if (fFile == nullptr)
	{
		if (CreateFile(ptcsFileName) == 0)
		{
			if (!bSilence)
			{
				_stprintf(tcsPrint, "%s %s %s", GetLanguageString(355).c_str(), ptcsFileName, GetLanguageString(356).c_str());
				MessageBox(nullptr, tcsPrint, GetLanguageString(25).c_str(), 0);
			}
			lRet = 0;
			goto end;
		}
		fFile = _tfopen(ptcsFileName, "rb");
	}

	fseek(fFile, 0, SEEK_END);
	lFileSize = ftell(fFile);
	fseek(fFile, 0, SEEK_SET);
	fread((void *)&m_SFileHead, sizeof(SFileHead), 1, fFile);
	if (m_SFileHead.lFileSize != lFileSize)
	{
		if (!bSilence)
		{
			_stprintf(tcsPrint, "%s %s", ptcsFileName, GetLanguageString(357).c_str());
			MessageBox(nullptr, tcsPrint, GetLanguageString(25).c_str(), 0);
		}
		lRet = 0;
		goto end;
	}
	if (m_SFileHead.lVersion == OBJ_FILE_VER500) //
	{
		fclose(fFile);
		if (ConvertObjFileVer(ptcsFileName) <= 0) //
		{
			lRet = 0;
			goto end;
		}

		fFile = _tfopen(ptcsFileName, "rb");
		if (fFile == NULL)
		{
			if (!bSilence)
			{
				_stprintf(tcsPrint, "%s %s", ptcsFileName, GetLanguageString(358).c_str());
				MessageBox(nullptr, tcsPrint, GetLanguageString(25).c_str(), 0);
			}
			lRet = 0;
			goto end;
		}

		fseek(fFile, 0, SEEK_END);
		lFileSize = ftell(fFile);
		fseek(fFile, 0, SEEK_SET);
		fread((void *)&m_SFileHead, sizeof(SFileHead), 1, fFile);
	}
	if (m_SFileHead.lVersion != OBJ_FILE_VER600)
	{
		if (!bSilence)
		{
			_stprintf(tcsPrint, "%s %s", ptcsFileName, GetLanguageString(340).c_str());
			MessageBox(nullptr, tcsPrint, GetLanguageString(25).c_str(), 0);
		}
		lRet = 0;
		goto end;
	}

	m_SSectionIndex = new (SSectionIndex[m_SFileHead.iSectionCntX * m_SFileHead.iSectionCntY]);
	if (m_SSectionIndex == NULL)
	{
		lRet = 0;
		goto end;
	}
	fread((void *)m_SSectionIndex, sizeof(SSectionIndex), m_SFileHead.iSectionCntX * m_SFileHead.iSectionCntY, fFile);

	if( g_Config.m_bEditor )
		m_fRdWr = _tfopen(ptcsFileName, "r+b");
	else
		m_fRdWr = _tfopen(ptcsFileName, "rb");

	if (m_fRdWr == NULL)
	{
		if (!bSilence)
		{
			_stprintf(tcsPrint, "%s %s", ptcsFileName, GetLanguageString(339).c_str());
			MessageBox(nullptr, tcsPrint, GetLanguageString(25).c_str(), 0);
		}
		lRet = 0;
		goto end;
	}

	if( g_Config.m_bEditor )
		m_fAppend = _tfopen(ptcsFileName, "a+b");
	else
		m_fAppend = _tfopen(ptcsFileName, "rb");

	if (m_fAppend == NULL)
	{
		if (!bSilence)
		{
			_stprintf(tcsPrint, "%s %s", ptcsFileName, GetLanguageString(359).c_str());
			MessageBox(NULL, tcsPrint, GetLanguageString(25).c_str(), 0);
		}
		lRet = 0;
		goto end;
	}

	m_bInitSuccess = true;

end:
	if (lRet == 0)
	{
		if (m_fRdWr)
		{
			fclose(m_fRdWr);
			m_fRdWr = NULL;
		}
		if (m_fAppend)
		{
			fclose(m_fAppend);
			m_fAppend = NULL;
		}
		if (m_SSectionIndex)
		{
			delete [] m_SSectionIndex;
			m_SSectionIndex = NULL;
		}
	}
	if (fFile)
		fclose(fFile);

	return lRet;
}
void CSceneObjFile::Free(void)
{
	m_bInitSuccess = false;
	if (m_fRdWr)
	{
		fclose(m_fRdWr);
		m_fRdWr = NULL;
	}
	if (m_fAppend)
	{
		fclose(m_fAppend);
		m_fAppend = NULL;
	}
	if (m_SSectionIndex)
	{
		delete [] m_SSectionIndex;
		m_SSectionIndex = NULL;
	}
}

long CSceneObjFile::CreateFile(const char* ptcsFileName,
								 int iSectionCntX, int iSectionCntY, int iSectionWidth,
								 int iSectionHeight, int iMaxSectionObjNum)
{
	FILE				*fFile = NULL;
	SFileHead			SHead;
	SSectionIndex		*lFileSectionIndex = NULL;

	if( g_Config.m_bEditor )
		fFile = _tfopen(ptcsFileName, "wb");
	else
		fFile = _tfopen(ptcsFileName, "rb");
	if (fFile == NULL)
		return 0;

	_tcscpy(SHead.tcsTitle, "HF Object File!");
	SHead.lVersion = OBJ_FILE_VER600;
	SHead.lFileSize = sizeof (SFileHead) + sizeof (SSectionIndex) * iSectionCntX * iSectionCntY;
	SHead.iSectionCntX = iSectionCntX;
	SHead.iSectionCntY = iSectionCntY;
	SHead.iSectionHeight = iSectionHeight;
	SHead.iSectionWidth = iSectionWidth;
	SHead.iSectionObjNum = iMaxSectionObjNum;

	lFileSectionIndex = new (SSectionIndex[iSectionCntX * iSectionCntY]);
	if (lFileSectionIndex == NULL)
	{
		fclose(fFile);
		return 0;
	}
	memset(lFileSectionIndex, 0, sizeof(SSectionIndex) * iSectionCntX * iSectionCntY);

	fwrite((const void *)&SHead, sizeof(SFileHead), 1, fFile);
	fwrite((const void *)lFileSectionIndex, sizeof(SSectionIndex), iSectionCntX * iSectionCntY, fFile);

	if (fFile)
		fclose(fFile);
	if (lFileSectionIndex)
		delete [] lFileSectionIndex;
	return 1;
}

long CSceneObjFile::ConvertObjFileVer(const char* ptcsFileName, bool bBackUp) // 500600(section)
{
	long			lRet = 2;
	char			tcsBackUpName[_MAX_FNAME] = "";
	char			tcsPrint[256];
	long			i, j;
	long			lMaxSectionNum;
	FILE			*fFileOld, *fFileNew;
	SSceneObjInfo	*pSObjInfo = NULL;
	SFileHead		SHead;
	SSectionIndex	*pSSectionIndex = NULL;
	unsigned long ulFileSize;

	_tcscpy(tcsBackUpName, ptcsFileName);
	for (i = 0; i < _MAX_FNAME - (long)_tcslen(ptcsFileName); i += 4)
	{
		_tcscat(tcsBackUpName, ".bak");
		fFileNew = _tfopen(tcsBackUpName, "rb");
		if (fFileNew == NULL)
			break;
		else
		{
			fclose(fFileNew);
			fFileNew = NULL;
		}
	}
	if (i >= _MAX_FNAME - (long)_tcslen(ptcsFileName))
	{
		lRet = -1; // 
		goto end;
	}
	if (_trename(ptcsFileName, tcsBackUpName) != 0) // 
	{
		lRet = -2; // 
		goto end;
	}

	fFileOld = _tfopen(tcsBackUpName, "rb");
	if (fFileOld == NULL)
	{
		lRet = -3; // 
		goto end;
	}

	if( g_Config.m_bEditor )
		fFileNew = _tfopen(ptcsFileName, "wb");
	else
		fFileNew = _tfopen(ptcsFileName, "rb");
	if (fFileNew == NULL)
	{
		lRet = -1; // 
		goto end;
	}

	fseek(fFileOld, 0, SEEK_END);
	ulFileSize = ftell(fFileOld);
	fseek(fFileOld, 0, SEEK_SET);
	fread(&SHead, sizeof(SFileHead), 1, fFileOld);
	if (SHead.lVersion != OBJ_FILE_VER500) // 
	{
		lRet = 1;
		goto end;
	}

	_stprintf(tcsPrint, GetLanguageString(360).c_str(), ptcsFileName);
	MessageBox(NULL, tcsPrint, GetLanguageString(361).c_str(), 0);

	lMaxSectionNum = SHead.iSectionCntX * SHead.iSectionCntY;
	pSSectionIndex = new (SSectionIndex[lMaxSectionNum]);
	if (pSSectionIndex == NULL)
	{
		lRet = -4; // 
		goto end;
	}

	pSObjInfo = new (SSceneObjInfo[SHead.iSectionObjNum]);
	if (pSObjInfo == NULL)
	{
		lRet = -4;
		goto end;
	}

	fread((void *)pSSectionIndex, sizeof(SSectionIndex), lMaxSectionNum, fFileOld);
	fseek(fFileNew, sizeof(SFileHead) + sizeof(SSectionIndex) * lMaxSectionNum, SEEK_SET);

	int nSectionX, nSectionY;
	for (i = 0; i < lMaxSectionNum && ulFileSize >= (unsigned long)ftell(fFileOld); i ++)
	{
	    if (pSSectionIndex[i].iObjNum > 0) //
		{
			fseek(fFileOld, pSSectionIndex[i].lObjInfoPos, SEEK_SET);
			fread(pSObjInfo, sizeof(SSceneObjInfo) * SHead.iSectionObjNum, 1, fFileOld);
			for (j = 0; j < pSSectionIndex[i].iObjNum; j++)
			{
				nSectionX = i % SHead.iSectionCntX * SHead.iSectionWidth * 100;
				nSectionY = i / SHead.iSectionCntX * SHead.iSectionHeight * 100;
				pSObjInfo[j].nX -= nSectionX;
				pSObjInfo[j].nY -= nSectionY;
                if(pSObjInfo[j].nY < 0)
                {
                    int nnn = 0;
                }
			}
			pSSectionIndex[i].lObjInfoPos = ftell(fFileNew);
			fwrite(pSObjInfo, sizeof(SSceneObjInfo) * SHead.iSectionObjNum, 1, fFileNew);
		}
		else
		{
			pSSectionIndex[i].lObjInfoPos = 0;
		}
	}

	SHead.lVersion = OBJ_FILE_VER600;
	SHead.lFileSize = ftell(fFileNew);
	fseek(fFileNew, 0, SEEK_SET);
	fwrite((const void *)&SHead, sizeof(SFileHead), 1, fFileNew);
	fwrite((const void *)pSSectionIndex, sizeof(SSectionIndex), lMaxSectionNum, fFileNew);

	if (!bBackUp)
	{
		fclose(fFileOld);
		fFileOld = NULL;
		_tremove(tcsBackUpName);
		_tcscpy(tcsBackUpName, ptcsFileName);
	}

end:
	if (lRet == 2)
	{
		_stprintf(tcsPrint, GetLanguageString(362).c_str(), ptcsFileName, tcsBackUpName);
		MessageBox(NULL, tcsPrint, GetLanguageString(363).c_str(), 0);
	}
	if (fFileOld)
		fclose(fFileOld);
	if (fFileNew)
		fclose(fFileNew);
	if (pSSectionIndex)
		delete [] pSSectionIndex;
	if (pSObjInfo)
		delete [] pSObjInfo;
	return lRet;
}

long CSceneObjFile::ReadSectionObjInfo(int nSectionNO, SSceneObjInfo *SSceneObj, long *lSectionObjNum)
{
	if (!m_bInitSuccess)
		return 0;

	if (nSectionNO >= m_SFileHead.iSectionCntX * m_SFileHead.iSectionCntY)
		return 0;

	if ((*lSectionObjNum = m_SSectionIndex[nSectionNO].iObjNum) > 0)
	{
		fseek(m_fRdWr, m_SSectionIndex[nSectionNO].lObjInfoPos, SEEK_SET);
        ToLogService("common", "Seek Offset [{} {}] = {}", nSectionNO % m_SFileHead.iSectionCntX, nSectionNO / m_SFileHead.iSectionCntX, m_SSectionIndex[nSectionNO].lObjInfoPos);
        fread(SSceneObj, sizeof(SSceneObjInfo), m_SSectionIndex[nSectionNO].iObjNum, m_fRdWr);
		// 
		int nSectionX, nSectionY;
		for (int i = 0; i < m_SSectionIndex[nSectionNO].iObjNum; i++)
		{
			nSectionX = nSectionNO % m_SFileHead.iSectionCntX * m_SFileHead.iSectionWidth * 100;
			nSectionY = nSectionNO / m_SFileHead.iSectionCntX * m_SFileHead.iSectionHeight * 100;
            if(SSceneObj[i].nX < 0)
            {
                int jjj = 0;
            }
            SSceneObj[i].nX += nSectionX;
			SSceneObj[i].nY += nSectionY;
			SSceneObjInfo *pObj = (SSceneObj + i);
			if(pObj->GetID()==0)
			{
				g_logManager.InternalLog(LogLevel::Error, "errors", GetLanguageString(364));
			}
			
		}
		//
	}

	return 1;
}

long CSceneObjFile::WriteSectionObjInfo(int nSectionNO, SSceneObjInfo *SSceneObj, long lSectionObjNum)
{
	FILE	*fFile;

	if (!m_bInitSuccess)
		return 0;

	if (nSectionNO >= m_SFileHead.iSectionCntX * m_SFileHead.iSectionCntY)
		return 0;

	m_SSectionIndex[nSectionNO].iObjNum = lSectionObjNum;
	if (lSectionObjNum > 0)
	{
		if (m_SSectionIndex[nSectionNO].lObjInfoPos > 0)
		{
			fFile = m_fRdWr;
			fseek(fFile, m_SSectionIndex[nSectionNO].lObjInfoPos, SEEK_SET);
		}
		else
		{
			m_SSectionIndex[nSectionNO].lObjInfoPos = m_SFileHead.lFileSize;
			m_SFileHead.lFileSize += sizeof(SSceneObjInfo) * m_SFileHead.iSectionObjNum;
			fseek(m_fRdWr, 0, SEEK_SET);
			fwrite((const void *)&m_SFileHead, sizeof(SFileHead), 1, m_fRdWr);
			fflush(m_fRdWr);

			fFile = m_fAppend;
		}
		// 
		int nSectionX, nSectionY;
		for (int j = 0; j < m_SSectionIndex[nSectionNO].iObjNum; j++)
		{
			nSectionX = nSectionNO % m_SFileHead.iSectionCntX * m_SFileHead.iSectionWidth * 100;
			nSectionY = nSectionNO / m_SFileHead.iSectionCntX * m_SFileHead.iSectionHeight * 100;
			SSceneObj[j].nX -= nSectionX;
			SSceneObj[j].nY -= nSectionY;
		}
		//
		// obj
		fwrite((const void *)SSceneObj, sizeof(SSceneObjInfo), m_SFileHead.iSectionObjNum, fFile);
		fflush(fFile);
	}
	else
	{
		m_SSectionIndex[nSectionNO].lObjInfoPos = 0;
	}

	fseek(m_fRdWr, sizeof(SFileHead) + sizeof(SSectionIndex) * nSectionNO, SEEK_SET);
	fwrite((const void *)(m_SSectionIndex + nSectionNO), sizeof(SSectionIndex), 1, m_fRdWr);
	fflush(m_fRdWr);

	// 
	// 
	long	lFileSize;
	fseek(m_fRdWr, 0, SEEK_END);
	lFileSize = ftell(m_fRdWr);
	if (lFileSize != m_SFileHead.lFileSize)
	{
		MessageBox(nullptr, GetLanguageString(365).c_str(), GetLanguageString(25).c_str(), 0);
	}
	//

	return 1;
}

long CSceneObjFile::TrimFile(const char* ptcsFileName, bool bBackUp)
{
	long		lRet = 1;
	_TCHAR		tcsBackUpName[_MAX_FNAME] = _TEXT("");
	long		i;
	long		lPos = 0;
	SFileHead	SHead;
	long		lMaxSectionNum;
	FILE		*fFileOld = NULL, *fFileNew = NULL; 
	char		*pszSectionInfo = NULL;
	SSectionIndex		*pSSectionIndex = NULL;
	unsigned long ulFileSize;

	_tcscpy(tcsBackUpName, ptcsFileName);
	for (i = 0; i < _MAX_FNAME - (long)_tcslen(ptcsFileName); i += 4)
	{
		_tcscat(tcsBackUpName, _TEXT(".bak"));
		fFileNew = _tfopen(tcsBackUpName, _TEXT("rb"));
		if (fFileNew == NULL)
			break;
		else
		{
			fclose(fFileNew);
			fFileNew = NULL;
		}
	}
	if (i >= _MAX_FNAME - (long)_tcslen(ptcsFileName))
	{
		lRet = -1; // 
		goto end;
	}
	if (_trename(ptcsFileName, tcsBackUpName) != 0) // 
	{
		lRet = -2; // 
		goto end;
	}

	fFileOld = _tfopen(tcsBackUpName, _TEXT("rb"));
	if (fFileOld == NULL)
	{
		lRet = -3; // 
		goto end;
	}

	if( g_Config.m_bEditor )
		fFileNew = _tfopen(ptcsFileName, _TEXT("wb"));
	else
		fFileNew = _tfopen(ptcsFileName, _TEXT("rb"));
	if (fFileNew == NULL)
	{
		lRet = -3; // 
		goto end;
	}

	fseek(fFileOld, 0, SEEK_END);
	ulFileSize = ftell(fFileOld);
	fseek(fFileOld, 0, SEEK_SET);
	fread((void *)&SHead, sizeof(SFileHead), 1, fFileOld);
	if (_tcscmp(SHead.tcsTitle, _TEXT("HF Object File!")) != 0
		|| SHead.lVersion != OBJ_FILE_VER600)// || SHead.lFileSize != i)
	{
		lRet = -4; // 
		goto end;
	}

    lMaxSectionNum = SHead.iSectionCntX * SHead.iSectionCntY;
	pSSectionIndex = new (SSectionIndex[lMaxSectionNum]);
	if (pSSectionIndex == NULL)
	{
		lRet = -5; // 
		goto end;
	}
	fread((void *)pSSectionIndex, sizeof(SSectionIndex), lMaxSectionNum, fFileOld);
	pszSectionInfo = new (char[sizeof(SSceneObjInfo) * lMaxSectionNum]);
	if (pszSectionInfo == NULL)
	{
		lRet = -5;
		goto end;
	}

	fseek(fFileNew, sizeof(SFileHead) + sizeof(SSectionIndex) * lMaxSectionNum, SEEK_SET);

	for (i = 0; i < lMaxSectionNum && ulFileSize >= (unsigned long)ftell(fFileOld); i ++)
	{
	    if (pSSectionIndex[i].iObjNum > 0) //
		{
			fseek(fFileOld, pSSectionIndex[i].lObjInfoPos, SEEK_SET);
			fread(pszSectionInfo, sizeof(SSceneObjInfo) * SHead.iSectionObjNum, 1, fFileOld);
			pSSectionIndex[i].lObjInfoPos = ftell(fFileNew);
			fwrite(pszSectionInfo, sizeof(SSceneObjInfo) * SHead.iSectionObjNum, 1, fFileNew);
		}
		else
		{
			pSSectionIndex[i].lObjInfoPos = 0;
		}
	}

	SHead.lFileSize = ftell(fFileNew);
	fseek(fFileNew, 0, SEEK_SET);
	fwrite(&SHead, sizeof(SFileHead), 1, fFileNew);
	fwrite(pSSectionIndex, sizeof(SSectionIndex), lMaxSectionNum, fFileNew);

	if (!bBackUp)
	{
		fclose(fFileOld);
		fFileOld = NULL;
		_tremove(tcsBackUpName);
	}

end:
	if (fFileOld)
		fclose(fFileOld);
	if (fFileNew)
		fclose(fFileNew);
	if (pSSectionIndex)
		delete [] pSSectionIndex;
	if (pszSectionInfo)
		delete [] pszSectionInfo;
	return lRet;
}

long CSceneObjFile::TrimDirectory(const char *ptcsDirectory, bool bBackUp)
{
	char tcsFileName[_MAX_FNAME], tcsPath[_MAX_PATH];
	size_t		lLen;
	_finddata_t	c_file;
	long		hFile;
	const char* ptcsRecordFile = "TrimRecord.txt";
	FILE		*fRecord = NULL;
	_timeb		tTimeBuffer;
	_TCHAR			tcsPrint[256];

	if (bBackUp)
		_stprintf(tcsPrint, GetLanguageString(366).c_str(), ptcsDirectory);
	else
		_stprintf(tcsPrint, GetLanguageString(367).c_str(), ptcsDirectory);
	if (IDYES != MessageBox(NULL, tcsPrint, GetLanguageString(361).c_str(), MB_YESNO))
		return 1;

	lLen = _tcslen(ptcsDirectory);
	_tcscpy(tcsPath, ptcsDirectory);
	if (tcsPath[lLen - 1] != '\\' || tcsPath[lLen - 1] !='/')
	{
		tcsPath[lLen] = '/';
		tcsPath[lLen + 1] = 0;
	}

	_stprintf(tcsFileName, "%s%s", tcsPath, ptcsRecordFile);
	if( g_Config.m_bEditor )
		fRecord = _tfopen(tcsFileName, "w");
	else
		fRecord = _tfopen(tcsFileName, "rb");
	if (fRecord == NULL)
		return 0;
	_ftime(&tTimeBuffer);
	_ftprintf(fRecord, GetLanguageString(368).c_str(), _tctime(&tTimeBuffer.time));

	_stprintf(tcsFileName,"%s%s", tcsPath, "*.obj");
	if( (hFile = (long)_tfindfirst(tcsFileName, &c_file)) == -1L )
	{
		fclose(fRecord);
		return 1;
	}
	else
	{
		if (!(c_file.attrib & _A_SUBDIR))
		{
			_stprintf(tcsFileName, "%s%s", tcsPath, c_file.name);
			if (c_file.attrib & _A_RDONLY)
				_tchmod(tcsFileName, _S_IREAD | _S_IWRITE);
			_ftprintf(fRecord, "%s:\n", tcsFileName);
			switch (TrimFile(tcsFileName, bBackUp))
			{
			case	1:
				_ftprintf(fRecord, GetLanguageString(369).c_str());
				break;
			case	-1:
				_ftprintf(fRecord,GetLanguageString(370).c_str());
				break;
			case	-2:
				_ftprintf(fRecord, GetLanguageString(371).c_str());
				break;
			case	-3:
				_ftprintf(fRecord, GetLanguageString(372).c_str());
				break;
			case	-4:
				_ftprintf(fRecord, GetLanguageString(373).c_str());
				break;
			case	-5:
				_ftprintf(fRecord, GetLanguageString(374).c_str());
				break;
			default:
				_ftprintf(fRecord, GetLanguageString(375).c_str());
			}
		}
		while( _findnext( hFile, &c_file ) == 0 )
		{
			if (!(c_file.attrib & _A_SUBDIR))
			{
				_stprintf(tcsFileName, "%s%s", tcsPath,c_file.name);
				if (c_file.attrib & _A_RDONLY)
					_tchmod(tcsFileName, _S_IREAD | _S_IWRITE);
				_ftprintf(fRecord,"%s:\n", tcsFileName);
				switch (TrimFile(tcsFileName, bBackUp))
				{
				case	1:
					_ftprintf(fRecord, GetLanguageString(369).c_str());
					break;
				case	-1:
					_ftprintf(fRecord, GetLanguageString(370).c_str());
					break;
				case	-2:
					_ftprintf(fRecord,GetLanguageString(371).c_str());
					break;
				case	-3:
					_ftprintf(fRecord, GetLanguageString(372).c_str());
					break;
				case	-4:
					_ftprintf(fRecord, GetLanguageString(373).c_str());
					break;
				case	-5:
					_ftprintf(fRecord, GetLanguageString(374).c_str());
					break;
				default:
					_ftprintf(fRecord, GetLanguageString(375).c_str());
				}
			}
		}

		_findclose( hFile );
	}

	if (fRecord)
		fclose(fRecord);
	return 1;
}
