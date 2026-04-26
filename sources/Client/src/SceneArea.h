#ifndef	SCENEAREA_H
#define	SCENEAREA_H

#define	SCENE_AREA_FILE_VER100	100

struct SAreaUnit {
	short siType; // 12
};

class CSceneArea {
	struct SFileHead {
		_TCHAR tcsTitle[32]; // "HF Scene Area File!"
		int lVersion;
		long lFileSize;

		int iSceneWidth; // Tile
		int iSceneHeight;
	};

public:
	CSceneArea();
	~CSceneArea();
	long Init(_TCHAR* ptcsAreaFile, bool bSilence = true);
	void Free();
	long CreateFile(_TCHAR* ptcsAreaFile, int iSceneWidth = 4096, int iSceneHeight = 4096);
	long CreateFileFromMap(_TCHAR* ptcsMapFile, _TCHAR* ptcsAreaFile);
	long ReadAreaInfo(long lUnitNO, SAreaUnit* pUnitData, long* lpUnitNum);
	long WriteAreaInfo(long lUnitNO, SAreaUnit* pUnitData, long* lpUnitNum);

private:
	bool m_bInitSuccess;
	long m_lUnitNum;
	FILE* m_fRdWr;
	SFileHead m_SFileHead;
};


#endif //SCENEAREA_H
