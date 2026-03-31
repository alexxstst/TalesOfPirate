#pragma once
#include "UIGlobalVar.h"

namespace GUI
{


#define ERROR_DATA -99999999

/**
 * The system properties class. It takes charge load or save the system properties.
 * @author Michael Chen 
 * @time 2005-4-19
 */
class CSystemProperties
{
public:
	struct SVideo
	{
		int nTexture;
		bool bAnimation;
		bool bCameraRotate;
		//bool bViewFar;        //(Michael Chen 2005-04-22
		bool bGroundMark;
		bool bDepth32;
		int  nQuality;
		bool bFullScreen;
		int bResolution;

		SVideo();

	} m_videoProp;

	struct Audio
	{
		int nMusicSound;
		int nMusicEffect;

		Audio() : nMusicSound(0), nMusicEffect(0) {}
	} m_audioProp;

	struct SGameOption {
		bool bRunMode;
		bool bLockMode;
		bool bHelpMode;
		bool bCameraMode;
		bool bAppMode;
		bool bEffMode;
		bool bStateMode;
		bool bEnemyNames;
		bool bShowBars;
		bool bShowPercentages;
		bool bShowInfo;
		bool bFramerate;
		bool bShowMounts;

	} m_gameOption;

	// Add by lark.li 20080826 begin
	struct StartOption {
		bool bFirst;

		StartOption() : bFirst(true) {}
	} m_startOption;
	// End

public:
	CSystemProperties() {}
	~CSystemProperties() {}
	/**
	 * Загрузить настройки из INI-файла.
	 * @param: szIniFileName Путь к INI-файлу.
	 * @return: 0 — успех, иначе — ошибка.
	 */
	int Load(const char * szIniFileName)
	{
		return readFromFile(szIniFileName);
	}
	/**
	 * Сохранить настройки в INI-файл.
	 * @param: szIniFileName Путь к INI-файлу.
	 * @return: 0 — успех, иначе — ошибка.
	 */
	int Save(const char * szIniFileName)
	{
		return writeToFile(szIniFileName);
	}

	/**
	* .
	* @return: success Return 0.
	*          video failure Return -1.
	*          audio failure Return -2.
	*          gameOption failureboth Return -3.
	*		   other failure Return -4.
	*/
    int Apply();

    /**
    * .
    * @return: success Return 0.
    */
    int ApplyVideo();
    /**
    * .
    * @return: success Return 0.
    */
    int ApplyAudio();
	/**
	* .
	* @return: success Return 0.
	*/
	int ApplyGameOption();

private:
	/**
	 * The help function of reading the propties from the file(*.ini).
	 * @param: szIniFileName The name of ini file
	 * @return: success Return 0.
	 */
	int readFromFile(const char * szIniFileName);
	/**
	 * The help function of write the propties to the file(*.ini).
	 * @param: szIniFileName The name of ini file.
	 * @return: success Return 0.
	 */
	int writeToFile(const char * szIniFileName);

};

class CChaExitOnTime
{
public:
	CChaExitOnTime();

public:	// 
	void	ChangeCha();	// 
	void	ExitApp();		// 
	void	OfflineMode();
	void	Relogin();		// 

	void	Cancel();		// 

	void	FrameMove(DWORD dwTime);

	bool	TimeArrived();

	void	Reset();

public:	// 
	void	NetStartExit( DWORD dwExitTime );	// 
	void	NetCancelExit();					// 

private:
	bool	_IsTime();				// ,

private:
	enum eOptionType
	{
		enumInit,
		enumChangeCha,
		enumExitApp,
		enumOfflineMode,
		enumRelogin,
	};

	eOptionType		_eOptionType;
	DWORD			_dwStartTime;
	DWORD			_dwEndTime;
	bool			_IsEnabled;

};

extern CChaExitOnTime g_ChaExitOnTime;

// ,,
class CSystemMgr : public CUIInterface
{
public:
    CSystemMgr();
	CForm*			GetSystemForm()		{ return frmSystem;			}

    void LoadCustomProp();

public:
	virtual void CloseForm();

protected:
	virtual bool Init();
    virtual void End();
	virtual void FrameMove(DWORD dwTime);

private:
	static void		_evtVideoChangeChange(CGuiData *pSender);
	static void		_evtMainMusicMouseDown(CGuiData *pSender,int x,int y ,DWORD key);		//

	static void		_evtVideoFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtSystemFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtAudioFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	static void		_evtAskReloginFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtAskExitFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtAskOfflineModeFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtAskChangeFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtGameOptionFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void		_evtGameOptionFormBeforeShow(CForm* pForm, bool& IsShow);

private:
	CForm			 *frmSystem;

	CForm            *frmAudio;
	CProgressBar     *proAudioMusic;
	CProgressBar     *proAudioMidi;

	CForm            *frmVideo;
	CCheckGroup      *cbxTexture;
	CCheckGroup      *cbxMovie;
	CCheckGroup      *cbxCamera;
	//CCheckGroup      *cbxView;        //(Michael Chen 2005-04-22
	CCheckGroup      *cbxTrail;
	CCheckGroup      *cbxColor;
	CCombo			 *cboResolution;
	CCheckGroup      *cbxModel;
	CCheckGroup      *cbxQuality;

	CForm			*frmGameOption;
	CCheckGroup		*cbxRunMode;
	CCheckGroup		*cbxLockMode;
	CCheckGroup		*cbxHelpMode;
	CCheckGroup		*cbxCameraMode;
	CCheckGroup		*cbxAppMode;
	CCheckGroup		*cbxEffMode;
	CCheckGroup		*cbxStateMode;


	// Add by mdrst may 2020 FPO alpha
	CCheckGroup		*cbxEnemyNames;
	CCheckGroup		*cbxShowBars;
	CCheckGroup		*cbxShowPercentages;
	CCheckGroup		*cbxShowInfo;
	CCheckGroup		*cbxFramerate;
	CCheckGroup		*cbxShowMounts;

	CForm			*frmAskRelogin;
	CForm			*frmAskExit;
	CForm			*frmAskChange;
	CForm			*frmAskOfflineMode;

    
public:
    CSystemProperties   m_sysProp;
    bool                m_isLoad;

};

}

