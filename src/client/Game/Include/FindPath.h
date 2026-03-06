#pragma once
#include <stack>

#define GUIDE_RANDGE 150
#define GUIDE_RANDGE_SMALL 30
#define MAX_RESET_TARGET_TIMES 2

class CGameScene;
class CCharacter;

class CFindPath
{
public:
    CFindPath(long step,int range);
	~CFindPath();

    void	RenderPath(MPTerrain* pTerrain);
    S_BVECTOR<D3DXVECTOR3>&		GetResultPath() {return _vecPathPoint;}

    int     GetLength()         { return _nLength;      }
    bool    Find( CGameScene* pScene, CCharacter* pCha, int nSelfX, int nSelfY, int nTargetX, int nTargetY, bool &IsWalkLine );
	bool    GetPathFindingState(){return m_bFindPath;}
	bool    LongPathFinding(){return m_bLongPathFinding;}
	void    SetLongPathFinding(long step, int range);
	void    SetShortPathFinding(long step, int range);

protected:	
    void		SetTargetPos(int nCurX,int nCurY,int& nTargetX, int& nTargetY, bool bback = true);
	BOOL		IsCross(int nCurX,int nCurY, int nTargetX, int nTargetY);
	BYTE*		GetTempTerrain(CGameScene* pScene, CCharacter* pCha,int iCurX, int iCurY);

    void	    Lock()		{ _bblock = true;	}
    void	    UnLock()	{ _bblock = false;	}
    BOOL	    FindPath(CGameScene* pScene, CCharacter* pCha, int nSelfX, int nSelfY, int nTargetX, int nTargetY, bool &IsWalkLine);

protected:
	D3DXVECTOR3	_vStart;

	int			_nCurX,_nCurY;

	int			_nTargetX;
	int			_nTargetY;

	S_BVECTOR<BYTE>			_vecDir;
	S_BVECTOR<BYTE>			_vecDirSave;
	S_BVECTOR<D3DXVECTOR3>	_vecPathPoint;

	int			_nWidth;

	bool		_bblock;

    int         _nLength;

private:
	//NOTE(Ogge): MAX_PATH_STEP and STEP_LIMIT member variable conflict with a macro with the same names..
	// Not sure what to do about it yet
	long		MAX_PATH_STEP;	
	long		STEP_LIMIT;		

	std::unique_ptr<uint8_t[]> _byTempBlock{};
	bool        m_bFindPath; //jze
	int			m_iRange; //jze
	bool        m_bLongPathFinding; //jze
};

class CFindPathEx 
{
public:
	CFindPathEx();
	void	Reset();

	void	SetTarget(int CurX, int CurY, int TargetX, int TargetY); 
	void	SetDestDirection(int CurX, int CurY, int TargetX, int TargetY);
	void    ClearDestDirection();
	bool	HaveTarget();
	void	Move(); //jze
private:
	void	CalculateGuideStraight();
	void	CalculateGuideBypass();  
private:
	int		m_iCurX;
	int		m_iCurY;

	int		m_iDeltaFlagX;
	int		m_iDeltaFlagY;

	int		m_iResetTargetTimes;

	int		m_iRecordX;
	int		m_iRecordY;

	int		m_iTargetX;
	int		m_iTargetY;

	int		m_iGuideTargetX;
	int     m_iGuideTargetY;

	std::stack<D3DXVECTOR2> m_GuideTargetList;
	
	DWORD	m_dwTime;

	bool	m_bReach;
	bool    m_bReachGuide;

	bool	m_bBigObstacle;
};

extern CFindPath g_cFindPath;
extern CFindPathEx g_cFindPathEx;
