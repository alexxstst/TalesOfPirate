#include "StdAfx.h"
#include "uisystemform.h"
#include "uiform.h"
#include "uicheckbox.h"
#include "Gameapp.h"
#include "uiformmgr.h"
#include "uiprogressbar.h"
#include "uiCombo.h"
#include "packetcmd.h"
#include "loginscene.h"
#include "cameractrl.h"
#include "Character.h"
#include "GameConfig.h"
#include "UIMinimapForm.h"
#include "uistartform.h"
#include "uiheadsay.h"
#include "netchat.h"
#include "smallmap.h"

#include "uiboatform.h"
#include "uiequipform.h"
#include "GlobalVar.h"
#include "scene.h"

#ifndef USE_DSOUND
#include "AudioThread.h"
extern CAudioThread g_AudioThread;
#endif
using namespace std;
using namespace GUI;

extern bool g_IsShowStates;
extern bool g_IsCameraMode;

//---------------------------------------------------------
// class CSystemProperties
//---------------------------------------------------------

/**
* .
* @return: success Return 0.
*/

CSystemProperties::SVideo::SVideo() :	
bFullScreen(false), bResolution(0), nTexture(0), nQuality(0),
bAnimation(false), bCameraRotate(false), bGroundMark(false), bDepth32(false)
{
}

int CSystemProperties::ApplyVideo()
{
	//video

	//bCameraRotate
	g_pGameApp->GetMainCam()->EnableRotation( m_videoProp.bCameraRotate );
	//bViewFar
	//g_pGameApp->GetMainCam()->EnableUpdown( m_videoProp.bViewFar ) ;//(Michael Chen 2005-04-22
	//nTexture-bGroundMark-nQuality
	g_pGameApp->GetCurScene()->SetTextureLOD(m_videoProp.nTexture);

	GetRender().SetIsChangeResolution(true);

	//
	//bAnimation

	//bDepth32-bFullScreen-bResolution
	int width(0), height(0);
	switch (m_videoProp.bResolution) 
	{
		case 0:
			width = TINY_RES_X;
			height = TINY_RES_Y;
			break;
		case 1:
			width = SMALL_RES_X;
			height = SMALL_RES_Y;
			break;
		case 2:
			width = MID_RES_X;
			height = MID_RES_Y;
			break;
		case 3:
			width = LARGE_RES_X;
			height = LARGE_RES_Y;
			break;
		case 4:
			width = EXTRA_LARGE_RES_X;
			height = EXTRA_LARGE_RES_Y;
			break;
		case 5:
			width = FULL_LARGE_RES_X;
			height = FULL_LARGE_RES_Y;
			break;
		default:
			width = TINY_RES_X;
			height = TINY_RES_Y;
			break;
	}
	D3DFORMAT  format = m_videoProp.bDepth32 ? D3DFMT_D24X8 : D3DFMT_D16;

	MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;
	if(FAILED(dev_obj->CheckCurrentDeviceFormat(BBFI_DEPTHSTENCIL, format)))
	{
		format = D3DFMT_D16;
	}
	g_pGameApp->ChangeVideoStyle( width ,height ,format ,!m_videoProp.bFullScreen  );

	return 0;
}
/**
* .
* @return: success Return 0.
*/
int CSystemProperties::ApplyAudio()
{
	//audio
	g_pGameApp->GetCurScene()->SetSoundSize( m_audioProp.nMusicEffect / 10.0f );			
	g_pGameApp->SetMusicSize( m_audioProp.nMusicSound / 10.0f  );
	g_pGameApp->mSoundManager->SetVolume(  m_audioProp.nMusicEffect / 10.0f  );

	return 0;

}
//-----------------------------------------------------------------------------
int CSystemProperties::ApplyGameOption()
{
	g_Config.SetMoveClient(m_gameOption.bRunMode);
	g_pGameApp->SysInfo("Game settings have been updated");
	// Success
	return 0;
}


/**
* .
* @return: success Return 0.
*          video failure Return -1.
*          audio failure Return -2.
*          gameOption failureboth Return -3.
*			other failure Return -4.
*/
int CSystemProperties::Apply()
{
	int nVideo = ApplyVideo();
	int nAudio = ApplyAudio();
	int nGameOption = ApplyGameOption();
	if (nVideo == 0 && nAudio == 0 && nGameOption == 0)
		return 0;
	if (nVideo != 0)
		return -1;
	if (nAudio != 0)
		return -2;
	if (nGameOption != 0)
		return -3;
	return -4;

}

/**
* The help function of reading the propties from the file(*.ini).
* @param: szIniFileName The name of ini file
* @return: 0 Success.
*          -1 File is not exist.
*          -2 File is destroyed.
*			-3 Filename is null or its length is zero.
*/
int CSystemProperties::readFromFile(const char * szIniFileName)
{
	//return 0;
#define DEFAULT_NUM -2

	if ((!szIniFileName) || (strlen(szIniFileName) == 0))
	{
		return -3;
	}

	int iTemp;
	// video
	iTemp = GetPrivateProfileInt("video", "texture", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.nTexture = iTemp;

	iTemp = GetPrivateProfileInt("video", "animation", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bAnimation = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("video", "cameraRotate", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bCameraRotate = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("video", "groundMark", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bGroundMark = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("video", "depth32", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bDepth32 = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("video", "quality", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.nQuality = iTemp;

	iTemp = GetPrivateProfileInt("video", "fullScreen", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bFullScreen = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("video", "resolution", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_videoProp.bResolution = iTemp;

	// audio
	iTemp = GetPrivateProfileInt("audio", "musicSound", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_audioProp.nMusicSound = iTemp;

	iTemp = GetPrivateProfileInt("audio", "musicEffect", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_audioProp.nMusicEffect = iTemp;

	// gameOption
	iTemp = GetPrivateProfileInt("gameOption", "runMode", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bRunMode = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("gameOption", "helpMode", 1, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bHelpMode = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("gameOption", "cameraMode", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bCameraMode = int2bool(iTemp);
	
	iTemp = GetPrivateProfileInt("gameOption", "apparel", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bAppMode = int2bool(iTemp);
	
	iTemp = GetPrivateProfileInt("gameOption", "effect", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bEffMode = int2bool(iTemp);
	
	iTemp = GetPrivateProfileInt("gameOption", "state", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bStateMode = int2bool(iTemp);

	// Add by Mdr.st May 2020 - FPO alpha
	iTemp = GetPrivateProfileInt("gameOption", "enemynames", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bEnemyNames = int2bool(iTemp);

	// Add by Mdr.st May 2020 - FPO alpha
	iTemp = GetPrivateProfileInt("gameOption", "showbars", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bShowBars = int2bool(iTemp);
	// Add by Mdr.st May 2020 - FPO alpha
	iTemp = GetPrivateProfileInt("gameOption", "showpercentages", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bShowPercentages = int2bool(iTemp);
	// Add by Mdr.st May 2020 - FPO alpha
	iTemp = GetPrivateProfileInt("gameOption", "showinfo", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bShowInfo = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("gameOption", "framerate", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bFramerate = int2bool(iTemp);

	iTemp = GetPrivateProfileInt("gameOption", "showmounts", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_gameOption.bShowMounts = int2bool(iTemp);


	// Add by lark.li 20080826 begin
	iTemp = GetPrivateProfileInt("startOption", "first", DEFAULT_NUM, szIniFileName);
	if (iTemp == DEFAULT_NUM)	return DEFAULT_NUM;
	m_startOption.bFirst = int2bool(iTemp);

	// End

	// success
	return 0;
}
/**
* The help function of write the propties to the file(*.ini).
* @param: szIniFileName The name of ini file.
* @return: 0 Success.
*          -1 Error.
*			-2 File can not be created.
*			-3 Filename is null or its length is zero.
*/
int CSystemProperties::writeToFile(const char * szIniFileName)
{
#define OTHER_ERROR -1
#define ERROE_FILE_CANNT_CREAT -2

	if ((!szIniFileName) || (strlen(szIniFileName) == 0))
	{
		return -3;
	}

	// video
	if (!WriteInteger("video", "texture", m_videoProp.nTexture, szIniFileName))
	{
		FILE *fp;
		fp=fopen(szIniFileName, "wb");
		if( fp )
		{
			fclose(fp);
		}
		else
		{
			return ERROE_FILE_CANNT_CREAT;
		}
	}

	if (!WriteInteger("video", "animation", bool2int(m_videoProp.bAnimation), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "cameraRotate", bool2int(m_videoProp.bCameraRotate), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "groundMark", bool2int(m_videoProp.bGroundMark), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "depth32", bool2int(m_videoProp.bDepth32), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "quality", m_videoProp.nQuality, szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "fullScreen", bool2int(m_videoProp.bFullScreen), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("video", "resolution", m_videoProp.bResolution, szIniFileName))
		return OTHER_ERROR;

	// audio
	if (!WriteInteger("audio", "musicSound", m_audioProp.nMusicSound, szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("audio", "musicEffect", m_audioProp.nMusicEffect, szIniFileName))
		return OTHER_ERROR;

	// gameOption
	//if (!WriteInteger("gameOption", "runMode", bool2int(m_gameOption.bRunMode), szIniFileName))
	if (!WriteInteger("gameOption", "runMode", bool2int(true), szIniFileName))
		return OTHER_ERROR;

	if (!WriteInteger("gameOption", "helpMode", bool2int(m_gameOption.bHelpMode), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("gameOption", "cameraMode", bool2int(m_gameOption.bCameraMode), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("gameOption", "apparel", bool2int(m_gameOption.bAppMode), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("gameOption", "effect", bool2int(m_gameOption.bEffMode), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("gameOption", "state", bool2int(m_gameOption.bStateMode), szIniFileName))
		return OTHER_ERROR;

	if (!WriteInteger("gameOption", "enemynames", bool2int(m_gameOption.bEnemyNames), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("gameOption", "showbars", bool2int(m_gameOption.bShowBars), szIniFileName))
		return OTHER_ERROR;


	if (!WriteInteger("gameOption", "showpercentages", bool2int(m_gameOption.bShowPercentages), szIniFileName))		// ADd by Mdr.st May 2020 - FPO alpha
		return OTHER_ERROR;
	if (!WriteInteger("gameOption", "showinfo", bool2int(m_gameOption.bShowInfo), szIniFileName))
		return OTHER_ERROR;
	if (!WriteInteger("gameOption", "framerate", bool2int(m_gameOption.bFramerate), szIniFileName))
		return OTHER_ERROR;

	if (!WriteInteger("gameOption", "showmounts", bool2int(m_gameOption.bShowMounts), szIniFileName))
		return OTHER_ERROR;
			

	// Add by lark.li 20080826 begin
	if (!WriteInteger("startOption", "first", bool2int(m_startOption.bFirst), szIniFileName))
		return OTHER_ERROR;
	// End

	// Success
	return 0;
}

bool CSystemProperties::int2bool(int n)
{
	if (0 == n)
		return false;
	else 
		return true;
}

int	CSystemProperties::bool2int(bool b)
{
	return b?1:0;
}



//video
static int     	g_nCbxTexture;
static int		g_nCbxMovie;
static int		g_nCbxCamera;
static int		g_nCbxView;
static int     	g_nCbxTrail;
static int     	g_nCbxColor;
static int     	g_nCboResolution;
static int     	g_nCbxModel;
static int     	g_bCbxQuality;
static float   	g_fPosMusic = -1.0f;           //
static float   	g_fPosMidi =  -1.0f;
static bool    	g_bChangeAudio = false;        //

//---------------------------------------------------------------------------
// class CVideoMgr
//---------------------------------------------------------------------------

CSystemMgr::CSystemMgr() 
: m_isLoad(false),frmSystem(0), frmAudio(0), proAudioMusic(0), proAudioMidi(0),
frmVideo(0), cbxTexture(0), cbxMovie(0), cbxCamera(0), cbxTrail(0), 
cbxColor(0), cboResolution(0), cbxModel(0), cbxQuality(0), frmAskRelogin(0),
frmAskExit(0), frmAskOfflineMode(0), frmAskChange(0)
{}



void CSystemMgr::LoadCustomProp()
{
	//(Michael Chen 2005-04-27)

	if (!m_isLoad)
	{
		if (m_sysProp.Load("user\\system.ini"))
		{
			//,
			m_sysProp.m_videoProp.nTexture=0;
			m_sysProp.m_videoProp.bAnimation=true;
			m_sysProp.m_videoProp.bCameraRotate=true;
			//m_sysProp.m_videoProp.bViewFar=true;      //(Michael Chen 2005-04-22
			m_sysProp.m_videoProp.bGroundMark=true;
			m_sysProp.m_videoProp.bDepth32=true;
			m_sysProp.m_videoProp.nQuality=0;
			m_sysProp.m_videoProp.bFullScreen=false;
			m_sysProp.m_videoProp.bResolution=0;

			m_sysProp.m_audioProp.nMusicSound=static_cast<int>(10.0f* g_pGameApp->GetMusicSize());
			m_sysProp.m_audioProp.nMusicEffect=static_cast<int>(10.0f* g_pGameApp->GetCurScene()->GetSoundSize());

			m_sysProp.m_gameOption.bRunMode = true;
			m_sysProp.m_gameOption.bLockMode = false;
			m_sysProp.m_gameOption.bCameraMode = true;
			m_sysProp.m_gameOption.bAppMode = true;
			m_sysProp.m_gameOption.bEffMode = true;
			m_sysProp.m_gameOption.bStateMode = false;
			m_sysProp.m_gameOption.bEnemyNames = true;
			m_sysProp.m_gameOption.bShowBars = false;
			m_sysProp.m_gameOption.bShowPercentages = false;
			m_sysProp.m_gameOption.bShowInfo = false;
			m_sysProp.m_gameOption.bFramerate = true;
			m_sysProp.m_gameOption.bShowMounts = true;
		}
	//	m_sysProp.m_gameOption.bRunMode = true;	//true
		m_isLoad = true;
	}
	CCharacter::SetIsShowShadow(m_sysProp.m_videoProp.bGroundMark);
	CCharacter::SetIsShowApparel(m_sysProp.m_gameOption.bAppMode);
	CCharacter::SetIsShowEffects(m_sysProp.m_gameOption.bEffMode);
	g_IsShowStates = m_sysProp.m_gameOption.bStateMode;
	g_IsCameraMode = m_sysProp.m_gameOption.bCameraMode;

	CHeadSay::SetIsShowEnemyNames(m_sysProp.m_gameOption.bEnemyNames);
	CHeadSay::SetIsShowBars(m_sysProp.m_gameOption.bShowBars);
	CHeadSay::SetIsShowPercentages(m_sysProp.m_gameOption.bShowPercentages);
	CHeadSay::SetIsShowInfo(m_sysProp.m_gameOption.bShowInfo);
	CSteadyFrame::SetFramerate60(m_sysProp.m_gameOption.bFramerate); //CSteadyFrame::SetFramerate60(m_sysProp.m_gameOption.bFramerate);
}
bool CSystemMgr::Init()
{

	frmSystem = _FindForm("frmSystem");// 
	if(!frmSystem ) 		return false;
	frmSystem->evtEntrustMouseEvent = _evtSystemFromMouseEvent;


	LoadCustomProp();       //

	///////////// Video
	frmVideo = _FindForm("frmVideo");
	if( !frmVideo )		return false;
	frmVideo->evtEntrustMouseEvent = _evtVideoFormMouseEvent;

	cbxTexture = ( CCheckGroup *)frmVideo->Find( "cbxTexture" ); //
	if (! cbxTexture)	return Error( "msgui.clu<%s><%s>", frmVideo->GetName(), "cbxTexture" );
	cbxTexture->SetActiveIndex(m_sysProp.m_videoProp.nTexture); // Michael Chen 2005-04-22

	cbxMovie = ( CCheckGroup *)frmVideo->Find( "cbxMovie" ); //
	if (! cbxMovie )	return Error( "msgui.clu<%s><%s>", frmVideo->GetName(), "cbxMovie" );
	cbxMovie->SetActiveIndex(m_sysProp.m_videoProp.bAnimation ? 0 : 1); // Michael Chen 2005-04-22

	cbxCamera = ( CCheckGroup *)frmVideo->Find( "cbxCamera" ); //
	if (! cbxCamera )	return Error( "msgui.clu<%s><%s>", frmVideo->GetName(), "cbxCamera" );
	cbxCamera->SetActiveIndex(m_sysProp.m_videoProp.bCameraRotate ? 0 : 1); // Michael Chen 2005-04-22

	/** (Michael Chen 2005-04-22
	cbxView = ( CCheckGroup *)frmVideo->Find( "cbxView" ); //
	if (! cbxView )		return Error( "msgui.clu<%s><%s>", frmVideo->GetName(), "cbxView" );
	cbxView->SetActiveIndex(m_sysProp.m_videoProp.bViewFar ? 0 : 1);    // Michael Chen 2005-04-22
	*/

	cbxTrail = ( CCheckGroup *)frmVideo->Find( "cbxTrail" ); //
	if (! cbxTrail )	return Error( g_oLangRec.GetString(45), frmVideo->GetName(), "cbxTrail" );
	cbxTrail->SetActiveIndex(m_sysProp.m_videoProp.bGroundMark ? 0 : 1);    // Michael Chen 2005-04-22

	cbxColor = ( CCheckGroup *)frmVideo->Find( "cbxColor" ); //
	if (! cbxColor )	return Error( g_oLangRec.GetString(45), frmVideo->GetName(), "cbxColor" );
	cbxColor->SetActiveIndex(m_sysProp.m_videoProp.bDepth32 ? 1 : 0);   // Michael Chen 2005-04-22

	cboResolution = (CCombo*)frmVideo->Find("cboResolution"); //
	if (!cboResolution)		return Error(g_oLangRec.GetString(45), frmVideo->GetName(), "cboResolution");
	cboResolution->GetList()->GetItems()->Select(m_sysProp.m_videoProp.bResolution);

	cbxModel = ( CCheckGroup *)frmVideo->Find( "cbxModel" ); //
	if (! cbxModel )	return Error( g_oLangRec.GetString(45), frmVideo->GetName(), "cbxModel" );	
	cbxModel->SetActiveIndex(m_sysProp.m_videoProp.bFullScreen ? 0 : 1);    // Michael Chen 2005-04-22

	cbxQuality = ( CCheckGroup *)frmVideo->Find( "cbxQuality" ); //
	if (! cbxQuality )	return Error( g_oLangRec.GetString(45), frmVideo->GetName(), "cbxQuality" );
	cbxQuality->SetActiveIndex(m_sysProp.m_videoProp.nQuality); // Michael Chen 2005-04-22
	cbxQuality->evtSelectChange  = _evtVideoChangeChange;

	//////////Audio
	frmAudio = _FindForm("frmAudio");
	if( !frmAudio )         return false;
	frmAudio->evtEntrustMouseEvent = _evtAudioFormMouseEvent;

	proAudioMusic  = dynamic_cast<CProgressBar *>(frmAudio->Find("proAudioMusic"));    
	if( !proAudioMusic )		return Error( g_oLangRec.GetString(45), frmAudio->GetName(), "proAudioMusic" );
	proAudioMusic->SetPosition( static_cast<float>(m_sysProp.m_audioProp.nMusicSound) );    // Michael Chen 2005-04-22
	proAudioMusic->evtMouseDown = _evtMainMusicMouseDown;

	proAudioMidi  = dynamic_cast<CProgressBar *>(frmAudio->Find("proAudioMidi"));    
	if( !proAudioMidi )			return Error( g_oLangRec.GetString(45), frmAudio->GetName(), "proAudioMidi" );
	proAudioMidi->SetPosition( static_cast<float>(m_sysProp.m_audioProp.nMusicEffect) );    // Michael Chen 2005-04-22
	proAudioMidi->evtMouseDown = _evtMainMusicMouseDown;

	//////// GameOption
	frmGameOption = _FindForm("frmGame");
	if (!frmGameOption) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "frmGame");
	frmGameOption->evtEntrustMouseEvent = _evtGameOptionFormMouseDown;
	frmGameOption->evtBeforeShow = _evtGameOptionFormBeforeShow;

	cbxRunMode = (CCheckGroup*)frmGameOption->Find("cbxRunmodel");
	if (!cbxRunMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxRunmodel");

	cbxLockMode = (CCheckGroup*)frmGameOption->Find("cbxLockmodel");
	if (!cbxLockMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxLockmodel");

	cbxHelpMode = (CCheckGroup*)frmGameOption->Find("cbxHelpmodel");
	if (!cbxHelpMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxHelpmodel");

	cbxCameraMode = static_cast<CCheckGroup*>(frmGameOption->Find("cbxCameraMode_p"));
	if (!cbxCameraMode)
	{
		return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxCameraMode_p");
	}
	
	cbxAppMode = (CCheckGroup*)frmGameOption->Find("cbxAppmodel");
	if (!cbxAppMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxAppmodel");

	cbxEffMode = (CCheckGroup*)frmGameOption->Find("cbxEffmodel");
	if (!cbxEffMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxEffmodel");

	cbxStateMode = (CCheckGroup*)frmGameOption->Find("cbxStatemodel");
	if (!cbxStateMode) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxStatemodel");

	// Added by mdrst May 2020 - FPO alpha

	cbxEnemyNames = (CCheckGroup*)frmGameOption->Find("cbxEnemyNames_p");
	if (!cbxEnemyNames) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxEnemyNames_p");

	cbxShowBars = (CCheckGroup*)frmGameOption->Find("cbxShowBars_p");
	if (!cbxShowBars) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxShowBars_p");
	cbxShowPercentages = (CCheckGroup*)frmGameOption->Find("cbxShowPercentages_p");
	if (!cbxShowPercentages) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxShowPercentages_p");

	cbxShowInfo = (CCheckGroup*)frmGameOption->Find("cbxShowInfo_p");
	if (!cbxShowInfo) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxShowInfo_p");
	cbxFramerate = (CCheckGroup*)frmGameOption->Find("cbxFramerate_p");
	if (!cbxFramerate) return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxFramerate_p");

	cbxShowMounts = static_cast<CCheckGroup*>(frmGameOption->Find("cbxShowMounts_p"));
	if (!cbxShowMounts)
	{
		return Error(g_oLangRec.GetString(45), frmGameOption->GetName(), "cbxShowMounts_p");
	}



	//////// 
	frmAskRelogin = _FindForm("frmAskRelogin");
	if( frmAskRelogin ) frmAskRelogin->evtEntrustMouseEvent = _evtAskReloginFormMouseDown;

	frmAskExit = _FindForm("frmAskExit");
	if( frmAskExit ) frmAskExit->evtEntrustMouseEvent = _evtAskExitFormMouseDown;
	
	frmAskOfflineMode = _FindForm("frmAskOfflineMode");
	if( frmAskOfflineMode ) frmAskOfflineMode->evtEntrustMouseEvent = _evtAskOfflineModeFormMouseDown;

	frmAskChange = _FindForm("frmAskChange");
	if( frmAskChange ) frmAskChange->evtEntrustMouseEvent = _evtAskChangeFormMouseDown;

	return true;
}

void CSystemMgr::End()
{
	const char* Value = cboResolution->GetText();
	int setResolution;
	if (strcmp("800x600", Value) == 0)
	{
		setResolution = 0;
	}
	if (strcmp("1152x648", Value) == 0)
	{
		setResolution = 1;
	}
	else if(strcmp("1280x720", Value) == 0)
	{
		setResolution = 2;
	}
	else if(strcmp("1366x768", Value) == 0)
	{
		setResolution = 3;
	}
	else if(strcmp("1600x900", Value) == 0)
	{
		setResolution = 4;
	}
	else if(strcmp("1920x1080", Value) == 0)
	{
		setResolution = 5;
	} 

	//(Michael Chen 2005-04-19)
	if (cbxTexture)
		m_sysProp.m_videoProp.nTexture = cbxTexture->GetActiveIndex();
	if (cbxMovie)
		m_sysProp.m_videoProp.bAnimation = cbxMovie->GetActiveIndex() == 0 ? true : false;
	if (cbxCamera)
		m_sysProp.m_videoProp.bCameraRotate = cbxCamera->GetActiveIndex() == 0 ? true : false;
	//m_sysProp.m_videoProp.bViewFar = cbxView->GetActiveIndex() == 0 ? true : false;(Michael Chen 2005-04-22
	if (cbxTrail)
		m_sysProp.m_videoProp.bGroundMark = cbxTrail->GetActiveIndex() == 0 ? true : false;
	if (cbxColor)
		m_sysProp.m_videoProp.bDepth32 = cbxColor->GetActiveIndex() == 0? false : true;
	if (cbxQuality)
		m_sysProp.m_videoProp.nQuality = cbxQuality->GetActiveIndex();
	if (cbxModel)
		m_sysProp.m_videoProp.bFullScreen = cbxModel->GetActiveIndex() == 0 ? true : false;
	if (cboResolution)
		m_sysProp.m_videoProp.bResolution = setResolution;
	if (proAudioMusic)
		m_sysProp.m_audioProp.nMusicSound=static_cast<int>(proAudioMusic->GetPosition());
	if (proAudioMidi)
		m_sysProp.m_audioProp.nMusicEffect=static_cast<int>(proAudioMidi->GetPosition());

	if (cbxRunMode)
		m_sysProp.m_gameOption.bRunMode = cbxRunMode->GetActiveIndex() == 0 ? false : true;
	
	//if (cbxAppMode)
	//	m_sysProp.m_gameOption.bAppMode = cbxAppMode->GetActiveIndex() == 0 ? false : true;

	if (m_sysProp.Save("user\\system.ini"))
	{
		// error when save the system properties.
	}
	//end of modifying by Michael Chen
}

void CSystemMgr::_evtVideoChangeChange(CGuiData *pSender)
{
	CCheckGroup * g = dynamic_cast<CCheckGroup*>(pSender);
	if( !g ) return;

	if (g->GetActiveIndex()==0 )
	{
		g_stUISystem.cbxTexture->SetActiveIndex(0);
		g_stUISystem.cbxMovie->SetActiveIndex(0);
		g_stUISystem.cbxCamera->SetActiveIndex(0);
		//g_stUISystem.cbxView->SetActiveIndex(0);//(Michael Chen 2005-04-22
		g_stUISystem.cbxTrail->SetActiveIndex(0);
		g_stUISystem.cbxColor->SetActiveIndex(0);			
	}
	else if ( g->GetActiveIndex()==1)
	{
		g_stUISystem.cbxTexture->SetActiveIndex(1);
		g_stUISystem.cbxMovie->SetActiveIndex(0);
		g_stUISystem.cbxCamera->SetActiveIndex(1);
		//g_stUISystem.cbxView->SetActiveIndex(0);//(Michael Chen 2005-04-22
		g_stUISystem.cbxTrail->SetActiveIndex(0);
		g_stUISystem.cbxColor->SetActiveIndex(1);	
	}
	else if (g->GetActiveIndex()==2 )
	{
		g_stUISystem.cbxTexture->SetActiveIndex(2);
		g_stUISystem.cbxMovie->SetActiveIndex(1);
		g_stUISystem.cbxCamera->SetActiveIndex(1);
		//g_stUISystem.cbxView->SetActiveIndex(1);//(Michael Chen 2005-04-22
		g_stUISystem.cbxTrail->SetActiveIndex(1);
		g_stUISystem.cbxColor->SetActiveIndex(1);	
	}
}

void CSystemMgr::_evtVideoFormMouseEvent(CCompent * pSender, int nMsgType, int x, int y, DWORD dwKey) 
{
    std::string name = pSender->GetName();
    if (name == "btnYes") 
	{
        int nTextureHigh = g_stUISystem.cbxTexture->GetActiveIndex();
        bool bMovieOn = g_stUISystem.cbxMovie->GetActiveIndex() == 0 ? true : false;
        bool bCameraOn = g_stUISystem.cbxCamera->GetActiveIndex() == 0 ? true : false;
        //bool bViewFar     = g_stUISystem.cbxView->GetActiveIndex()==0?true:false;//(Michael Chen 2005-04-22
        bool bTrailOn = g_stUISystem.cbxTrail->GetActiveIndex() == 0 ? true : false;
        CCharacter::SetIsShowShadow(bTrailOn);
        D3DFORMAT format = g_stUISystem.cbxColor->GetActiveIndex() == 0 ? D3DFMT_D24X8 : D3DFMT_D16;

        int width;
        int height;

        const char* getValue = g_stUISystem.cboResolution->GetText();
        if (strcmp("800x600", getValue) == 0) 
		{
            width = TINY_RES_X;
            height = TINY_RES_Y;
        } 
        if (strcmp("1152x648", getValue) == 0) 
		{
            width = SMALL_RES_X;
            height = SMALL_RES_Y;
        } 
		else if (strcmp("1280x720", getValue) == 0) 
		{
            width = MID_RES_X;
            height = MID_RES_Y;
        } 
		else if (strcmp("1366x768", getValue) == 0) 
		{
            width = LARGE_RES_X;
            height = LARGE_RES_Y;
        } 
		else if (strcmp("1600x900", getValue) == 0) 
		{
            width = EXTRA_LARGE_RES_X;
            height = EXTRA_LARGE_RES_Y;
        } 
		else if (strcmp("1920x1080", getValue) == 0) 
		{
            width = FULL_LARGE_RES_X;
            height = FULL_LARGE_RES_Y;
        }

        bool bWindowed = g_stUISystem.cbxModel->GetActiveIndex() == 0 ? false : true;

        g_pGameApp->GetMainCam()->EnableRotation(bCameraOn);
        g_stUISystem.m_sysProp.m_videoProp.bCameraRotate = bCameraOn;

        if (bCameraOn == false) 
		{
            g_Render.EnableClearTarget(FALSE);
        } 
		else 
		{
            g_Render.EnableClearTarget(TRUE);
        }
        //g_pGameApp->GetMainCam()->EnableUpdown( bViewFar ) ;//(Michael Chen 2005-04-22
        g_pGameApp->GetCurScene()->SetTextureLOD(nTextureHigh);

        g_Config.m_bFullScreen = FALSE;
        if (!bWindowed)
		{
            width = GetSystemMetrics(SM_CXSCREEN);
            height = GetSystemMetrics(SM_CYSCREEN);
            bWindowed = TRUE;
            g_Config.m_bFullScreen = TRUE;
        }

        GetRender().SetIsChangeResolution(true);
		
        MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;
        if (FAILED(dev_obj->CheckCurrentDeviceFormat(BBFI_DEPTHSTENCIL, format))) 
		{
            format = D3DFMT_D16;
        }
        g_pGameApp->ChangeVideoStyle(width, height, format, bWindowed);
        pSender->GetForm()->SetIsShow(false);

        g_stUISystem.frmSystem->SetIsShow(false);
        return;
    } 
	else if (name == "btnNo" || name == "btnClose") 
	{
        g_stUISystem.cbxTexture->SetActiveIndex(g_nCbxTexture);
        g_stUISystem.cbxMovie->SetActiveIndex(g_nCbxMovie);
        g_stUISystem.cbxCamera->SetActiveIndex(g_nCbxCamera);
        //g_stUISystem.cbxView->SetActiveIndex(g_nCbxView);//(Michael Chen 2005-04-22
        g_stUISystem.cbxTrail->SetActiveIndex(g_nCbxTrail);
        g_stUISystem.cbxColor->SetActiveIndex(g_nCbxColor);
        g_stUISystem.cbxModel->SetActiveIndex(g_nCbxModel);
        g_stUISystem.cbxQuality->SetActiveIndex(g_bCbxQuality);

        pSender->GetForm()->SetIsShow(false);
        return;
    }
}

void CSystemMgr::_evtSystemFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();

	//frmSystem
	if( name=="btnClose" || name == "btnNo" )
	{
		pSender->GetForm()->Close();
		return;
	}
	if (name== "btnChange") 
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmAskChange" );
		if( f ) 	f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return;
	}
	else if( name=="btnGame" )
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmGame" );
		if( f ) 	f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return;
	}
	else if (name == "btnRelogin")
	{
		if (g_TomServer.bEnable)
		{
			g_pGameApp->MsgBox( g_oLangRec.GetString(773) );
			return;
		}
		CForm* f = CFormMgr::s_Mgr.Find( "frmAskRelogin" );
		if( f ) 	f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return;
	}
	else if (name == "btnExit" )
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmAskExit" );
		if(f)  	f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return;
	}
	else if (name == "btnOfflineMode" )
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmAskOfflineMode" );
		if(f)  	f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return;
	}		
	else if (name == "btnAudio")
	{
		CForm *f = CFormMgr::s_Mgr.Find("frmAudio");
		if (!f)   return ;

		f->SetIsShow(true);
		if(g_fPosMusic<0.0f && g_fPosMusic<0.0f )
		{
			g_fPosMusic = g_stUISystem.proAudioMusic->GetPosition();				
			g_fPosMidi  = g_stUISystem.proAudioMidi->GetPosition();
			g_pGameApp->SetMusicSize( g_fPosMusic/10.0f);
			g_pGameApp->GetCurScene()->SetSoundSize( g_fPosMidi /10.0f );
			g_pGameApp->mSoundManager->SetVolume(  g_fPosMidi /10.0f   );

		}
		if(g_bChangeAudio)
		{
			g_fPosMusic = g_stUISystem.proAudioMusic->GetPosition();				
			g_fPosMidi  = g_stUISystem.proAudioMidi->GetPosition();
			g_pGameApp->SetMusicSize( g_fPosMusic/10.0f);
			g_pGameApp->GetCurScene()->SetSoundSize( g_fPosMidi /10.0f );
			g_pGameApp->mSoundManager->SetVolume(  g_fPosMidi /10.0f   );
		}							
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return ;
	}
	else if (name == "btnVideo")
	{
		CForm *f = CFormMgr::s_Mgr.Find("frmVideo") ;
		if(!f)      return;

		f->SetIsShow(true);				
		g_nCbxTexture = g_stUISystem.cbxTexture->GetActiveIndex();
		g_nCbxMovie   = g_stUISystem.cbxMovie->GetActiveIndex();
		g_nCbxCamera  = g_stUISystem.cbxCamera->GetActiveIndex();
		//g_nCbxView    = g_stUISystem.cbxView->GetActiveIndex();//(Michael Chen 2005-04-22
		g_nCbxTrail   = g_stUISystem.cbxTrail->GetActiveIndex();
		g_nCbxColor   = g_stUISystem.cbxColor->GetActiveIndex();
		g_nCboResolution    = g_stUISystem.cboResolution->GetList()->GetItems()->GetSelect()->GetIndex();
		g_nCbxModel   = g_stUISystem.cbxModel->GetActiveIndex();
		g_bCbxQuality = g_stUISystem.cbxQuality->GetActiveIndex();		
		g_stUISystem.frmSystem->SetIsShow(false) ;
		return ;
	}
}

void CSystemMgr::_evtAudioFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name=pSender->GetName();

	//frmAudio
	if (name=="btnYes")						//
	{
		g_fPosMusic = g_stUISystem.proAudioMusic->GetPosition();				
		g_fPosMidi  = g_stUISystem.proAudioMidi->GetPosition();
		g_pGameApp->SetMusicSize( g_fPosMusic/10.0f);
		g_pGameApp->GetCurScene()->SetSoundSize( g_fPosMidi /10.0f );
		g_pGameApp->mSoundManager->SetVolume(  g_fPosMidi /10.0f   );

		g_bChangeAudio = true;
		pSender->GetForm()->SetIsShow(false) ;  
		return  ;
	}
	else if (name=="btnNo")                   // 
	{
		g_stUISystem.proAudioMusic->SetPosition(g_fPosMusic);	
		g_stUISystem.proAudioMidi->SetPosition(g_fPosMidi);
		g_pGameApp->SetMusicSize( g_fPosMusic/10.0f);
		g_pGameApp->GetCurScene()->SetSoundSize( g_fPosMidi /10.0f );
		g_pGameApp->mSoundManager->SetVolume(  g_fPosMidi /10.0f   );
		g_bChangeAudio = false;
		pSender->GetForm()->SetIsShow(false) ;  
		return;
	}	 
}

void CSystemMgr::_evtMainMusicMouseDown(CGuiData *pSender, int x, int y, DWORD key)
{
	CProgressBar *	proAudioMidi  = g_stUISystem.proAudioMidi;
	CProgressBar *  proAudioMusic = g_stUISystem.proAudioMusic;

	string name = pSender->GetName();
	float fPos;

	if ( _stricmp ("frmAudio", pSender->GetForm()->GetName() ) == 0 )
	{
		if(name =="proAudioMusic")
		{
			fPos = 10.0f*(float)(x - proAudioMusic->GetLeft() - proAudioMusic->GetForm()->GetLeft() ) /(float)proAudioMusic->GetWidth() ;
			proAudioMusic->SetPosition( fPos);			
			proAudioMusic->Refresh();	
			g_pGameApp->SetMusicSize( fPos/10.0f  );

		}
		else if(name =="proAudioMidi" )
		{
			fPos = 10.0f* (float)(x - proAudioMidi->GetLeft() - proAudioMidi->GetForm()->GetLeft() ) /(float)proAudioMidi->GetWidth() ;
			proAudioMidi->SetPosition( fPos);			
			proAudioMidi->Refresh();
			g_pGameApp->mSoundManager->SetVolume( fPos/10.0f  );
			g_pGameApp->GetCurScene()->SetSoundSize( fPos/10.0f  );
		}
	}
}

void CSystemMgr::_evtAskReloginFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	pSender->GetForm()->SetIsShow(false);

	if (name== "btnYes") 
	{			
		g_ChaExitOnTime.Relogin();
		return;
	}
}

void CSystemMgr::_evtAskExitFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	pSender->GetForm()->SetIsShow(false);

	if (name== "btnYes") 
	{
		g_ChaExitOnTime.ExitApp();
	}
}

void CSystemMgr::_evtAskOfflineModeFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	pSender->GetForm()->SetIsShow(false);

	if (name== "btnYes") 
	{
		g_ChaExitOnTime.OfflineMode();
	}
}

void CSystemMgr::_evtAskChangeFormMouseDown(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	pSender->GetForm()->SetIsShow(false);

	if (name== "btnYes")	// 
	{
		g_ChaExitOnTime.ChangeCha();
	}
}

void CSystemMgr::_evtGameOptionFormMouseDown(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	if (name!="btnYes") return;

	CCheckGroup *	pGroup  = g_stUISystem.cbxRunMode;
	if( pGroup )
	{
		const auto v = pGroup->GetActiveIndex() == 0 ? false : true;
		if (v != g_stUISystem.m_sysProp.m_gameOption.bRunMode)
		{
			g_stUISystem.m_sysProp.m_gameOption.bRunMode = v;
			g_stUISystem.m_sysProp.ApplyGameOption();
		}
	}

	// 
	pGroup = g_stUISystem.cbxLockMode;
	if(pGroup)
	{
		const auto v = pGroup->GetActiveIndex() == 1 ? true : false;
		if (v != g_stUISystem.m_sysProp.m_gameOption.bLockMode)
		{
			g_stUISystem.m_sysProp.m_gameOption.bLockMode = v;
			CS_AutoKitbagLock(g_stUISystem.m_sysProp.m_gameOption.bLockMode);
		}
	}

	// 
	pGroup = g_stUISystem.cbxHelpMode;
	if(pGroup)
	{
		const bool bHelpMode = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bHelpMode != g_stUISystem.m_sysProp.m_gameOption.bHelpMode)
		{
			g_stUISystem.m_sysProp.m_gameOption.bHelpMode = bHelpMode;
			if (!bHelpMode) g_stUIStart.ShowLevelUpHelpButton(bHelpMode);
			g_stUIStart.ShowInfoCenterButton(bHelpMode);
			::WritePrivateProfileString("gameOption", "helpMode", bHelpMode ? "1" : "0", "./user/system.ini");

		}
	}

	pGroup = g_stUISystem.cbxCameraMode;
	if (pGroup)
	{
		const bool bCameraMode = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bCameraMode != g_stUISystem.m_sysProp.m_gameOption.bCameraMode)
		{
			g_stUISystem.m_sysProp.m_gameOption.bCameraMode = bCameraMode;
			g_IsCameraMode = bCameraMode;
			::WritePrivateProfileString("gameOption", "cameraMode", bCameraMode ? "1" : "0", "./user/system.ini");
		}
	}
	
	pGroup = g_stUISystem.cbxAppMode;
	if(pGroup)
	{
		const bool bAppMode = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bAppMode != g_stUISystem.m_sysProp.m_gameOption.bAppMode)
		{
			g_stUISystem.m_sysProp.m_gameOption.bAppMode = bAppMode;
			CCharacter::SetIsShowApparel(bAppMode);
			g_stUIStart.RefreshPet();
			//for each player in scene, re-render them.
			CGameScene* curScene = g_pGameApp->GetCurScene();
			CCharacter* pCha = curScene->_pChaArray;
			for (int i = 0; i < curScene->_nChaCnt; i++) {
				if (pCha->IsValid() && !pCha->IsNPC() && pCha->IsEnabled()) {
					pCha->UpdataFace(pCha->GetPart());
				}
				pCha++;
			}

			//update player portrait
			pCha = g_stUIBoat.GetHuman();
			static stNetTeamChaPart stTeamPart;
			stTeamPart.Convert(pCha->GetPart());
			g_stUIStart.GetMainCha()->UpdataFace(stTeamPart);

			::WritePrivateProfileString("gameOption", "apparel", bAppMode ? "1" : "0", "./user/system.ini");
		}
	}
	
	pGroup = g_stUISystem.cbxEffMode;
	if (pGroup)
	{
		const bool bEffMode = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bEffMode != g_stUISystem.m_sysProp.m_gameOption.bEffMode)
		{

			g_stUISystem.m_sysProp.m_gameOption.bEffMode = bEffMode;
			CCharacter::SetIsShowEffects(bEffMode);
			g_stUIStart.RefreshPet();
			//for each player in scene, re-render them.
			CGameScene* curScene = g_pGameApp->GetCurScene();
			CCharacter* pCha = curScene->_pChaArray;
			for (int i = 0; i < curScene->_nChaCnt; i++) {
				if (pCha->IsValid() && !pCha->IsNPC() && pCha->IsEnabled()) {
					pCha->UpdataFace(pCha->GetPart());
					pCha->RefreshSelfEffects();
				}
				pCha++;
			}

			::WritePrivateProfileString("gameOption", "effect", bEffMode ? "1" : "0", "./user/system.ini");
		}
	}

	pGroup = g_stUISystem.cbxStateMode;
	if (pGroup)
	{
		const bool bStateMode = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bStateMode != g_stUISystem.m_sysProp.m_gameOption.bStateMode)
		{
			g_stUISystem.m_sysProp.m_gameOption.bStateMode = bStateMode;
			g_IsShowStates = bStateMode;
			::WritePrivateProfileString("gameOption", "state", bStateMode ? "1" : "0", "./user/system.ini");
		}
	}

	pGroup = g_stUISystem.cbxEnemyNames;	// Add by mdr.st May 2020 - FPO alpha
	if(pGroup) 
	{
		const bool bEnemyNames = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bEnemyNames != g_stUISystem.m_sysProp.m_gameOption.bEnemyNames)
		{

			g_stUISystem.m_sysProp.m_gameOption.bEnemyNames = bEnemyNames;
			CHeadSay::SetIsShowEnemyNames(bEnemyNames); //Put this in an if statement inside UIHeadSay.cpp
			//g_stUIStart.RefreshPet();
			//CHeadSay::Render();
		}
	}
	pGroup = g_stUISystem.cbxShowBars;	// Add by mdr.st May 2020 - FPO alpha
	if(pGroup) 
	{
		const bool bShowBars = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bShowBars != g_stUISystem.m_sysProp.m_gameOption.bShowBars)
		{
			g_stUISystem.m_sysProp.m_gameOption.bShowBars = bShowBars;
			CHeadSay::SetIsShowBars(bShowBars); //Put this in an if statement inside UIHeadSay.cpp
			//g_stUIStart.RefreshPet();
			//CHeadSay::Render();
		}
	}

	pGroup = g_stUISystem.cbxShowPercentages;	// Add by mdr.st May 2020 - FPO alpha
	if(pGroup) 
	{
		const bool bShowPercentages = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bShowPercentages != g_stUISystem.m_sysProp.m_gameOption.bShowPercentages)
		{
			g_stUISystem.m_sysProp.m_gameOption.bShowPercentages = bShowPercentages;
			CHeadSay::SetIsShowPercentages(bShowPercentages); //Put this in an if statement inside UIHeadSay.cpp
			//g_stUIStart.RefreshPet();
			//CHeadSay::Render();
		}
	}

	pGroup = g_stUISystem.cbxShowInfo;	// Add by mdr.st May 2020 - FPO alpha
	if(pGroup) 
	{
		const bool bShowInfo = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bShowInfo != g_stUISystem.m_sysProp.m_gameOption.bShowInfo)
		{
			g_stUISystem.m_sysProp.m_gameOption.bShowInfo = bShowInfo;
			CHeadSay::SetIsShowInfo(bShowInfo); //Put this in an if statement inside UIHeadSay.cpp
			//g_stUIStart.RefreshPet();
			//CHeadSay::Render();
		}
	}
	pGroup = g_stUISystem.cbxFramerate;
	if (pGroup)
	{
		const bool bFramerate = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bFramerate != g_stUISystem.m_sysProp.m_gameOption.bFramerate)
		{
			g_stUISystem.m_sysProp.m_gameOption.bFramerate = bFramerate;
			g_pGameApp->SetFrame(bFramerate);
			g_pGameApp->MsgBox("Please switch character to update framerate");
		}
	}

	pGroup = g_stUISystem.cbxShowMounts;
	if (pGroup)
	{
		const bool showMounts = pGroup->GetActiveIndex() == 1 ? true : false;
		if (showMounts != g_stUISystem.m_sysProp.m_gameOption.bShowMounts)
		{
			g_stUISystem.m_sysProp.m_gameOption.bShowMounts = showMounts;
			showMounts ? RespawnAllPlayerMounts() : DespawnAllPlayerMounts();
		}
	}
}

void CSystemMgr::_evtGameOptionFormBeforeShow(CForm* pForm, bool& IsShow)
{
	CCheckGroup *	pGroup  = g_stUISystem.cbxRunMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bRunMode ? 1 : 0);

	pGroup = g_stUISystem.cbxLockMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bLockMode ? 1 : 0);

	pGroup = g_stUISystem.cbxHelpMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bHelpMode ? 1 : 0);

	pGroup = g_stUISystem.cbxCameraMode;
	if(pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bCameraMode ? 1 : 0);
	
	pGroup = g_stUISystem.cbxAppMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bAppMode ? 1 : 0);
	
	pGroup = g_stUISystem.cbxEffMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bEffMode ? 1 : 0);
	pGroup = g_stUISystem.cbxStateMode;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bStateMode ? 1 : 0);
	//Add by Mdr.st May 2020 - FPO alpha
	pGroup = g_stUISystem.cbxEnemyNames;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bEnemyNames ? 1 : 0);

	pGroup = g_stUISystem.cbxShowBars;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bShowBars ? 1 : 0);

	pGroup = g_stUISystem.cbxShowPercentages;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bShowPercentages ? 1 : 0);
	pGroup = g_stUISystem.cbxShowInfo;
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bShowInfo ? 1 : 0);
	pGroup = g_stUISystem.cbxFramerate;	
	if( pGroup )
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bFramerate ? 1 : 0);
	
	pGroup = g_stUISystem.cbxShowMounts;
	if(pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bShowMounts ? 1 : 0);
}


void CSystemMgr::CloseForm()
{
	g_ChaExitOnTime.Cancel();
}

void CSystemMgr::FrameMove(DWORD dwTime)
{
	g_ChaExitOnTime.FrameMove( dwTime );
}

//---------------------------------------------------------------------------
// class CChaExitOnTime
//---------------------------------------------------------------------------
namespace GUI
{
	CChaExitOnTime g_ChaExitOnTime;
};

CChaExitOnTime::CChaExitOnTime()
: _eOptionType(enumInit), _dwStartTime(0), _dwEndTime(0), _IsEnabled(false)
{
}

bool CChaExitOnTime::_IsTime()
{
	if( _eOptionType!=enumInit )
	{
		if( CGameApp::GetCurTick() > _dwStartTime + 60 * 1000 * 5 )
		{
			_eOptionType = enumInit;
			return false;
		}

		g_pGameApp->SysInfo( g_oLangRec.GetString(774) );
		return true;
	}

	return false;
}

void CChaExitOnTime::ChangeCha()
{
	if( _IsTime() ) return;

	if(g_pGameApp->GetCurScene()->GetMainCha()->IsShop()){
		g_pGameApp->MsgBox("Please close your stall before switching characters.");
		return;
	}

	_eOptionType = enumChangeCha;
	_dwStartTime = CGameApp::GetCurTick();

	_dwEndTime = 0;

	g_stUIMap.CloseRadar();	//   add by Philip.Wu  2006-06-21

	g_pGameApp->ClearAllSkillClocks();
	CS_EndPlay();

#ifdef USE_DSOUND
	if( g_dwCurMusicID )
	{
		AudioSDL::get_instance()->stop( g_dwCurMusicID );
		g_dwCurMusicID = 0;
		Sleep( 60 );
	}
#endif

	if( !_IsEnabled )
	{
		TimeArrived();
	}
}

void CChaExitOnTime::ExitApp()
{
	if( _IsTime() ) return;

	_eOptionType = enumExitApp;
	_dwStartTime = CGameApp::GetCurTick();

	_dwEndTime = 0;
	CS_Logout();

	if( !_IsEnabled )
	{
		TimeArrived();
	}
}

void CChaExitOnTime::OfflineMode()
{
	if( _IsTime() ) return;

	_eOptionType = enumOfflineMode;
	_dwStartTime = CGameApp::GetCurTick();

	_dwEndTime = 0;
	CS_OfflineMode();

	if( !_IsEnabled )
	{
		TimeArrived();
	}
}

void CChaExitOnTime::Relogin()
{
	if( _IsTime() ) return;

	_eOptionType = enumRelogin;
	_dwStartTime = CGameApp::GetCurTick();

	_dwEndTime = 0;

	g_stUIMap.CloseRadar();	//   add by Philip.Wu  2006-06-21
	g_pGameApp->ClearAllSkillClocks();
	CS_Logout();

#ifdef USE_DSOUND
	if( g_dwCurMusicID )
	{
		AudioSDL::get_instance()->stop( g_dwCurMusicID );
		g_dwCurMusicID = 0;
		Sleep( 60 );
	}
#endif

	if( !_IsEnabled )
	{
		TimeArrived();
	}
}

void CChaExitOnTime::Cancel()
{
	if( !_IsEnabled ) return;

	if( _eOptionType==enumInit ) return;

	extern void CS_CancelExit();
	CS_CancelExit();

	_eOptionType = enumInit;
}

void CChaExitOnTime::FrameMove(DWORD dwTime)
{
	if( !_IsEnabled ) return;

	if( _eOptionType==enumInit ) return;

	if( _dwEndTime==0 ) return;

	if( dwTime < _dwEndTime ) 
	{
		static CTimeWork time(1000);
		if( time.IsTimeOut( dwTime ) )
		{
			g_pGameApp->ShowBigText( g_oLangRec.GetString(775), (_dwEndTime - dwTime)/1000 );
			return;
		}
	}
}

bool CChaExitOnTime::TimeArrived()
{
	switch( _eOptionType )
	{
	case enumChangeCha:
		{
			g_pGameApp->LoadScriptScene(enumLoginScene);
			g_pGameApp->SetLoginTime(0);

			CLoginScene* pScene = dynamic_cast<CLoginScene*>(g_pGameApp->GetCurScene());
			if( pScene ) 
			{
				if( g_NetIF->IsConnected() )
					pScene->ShowChaList();
				else
					pScene->ShowRegionList();
					//pScene->ShowLoginForm();
			}
		}
		break;
	case enumExitApp:
		{
			CS_Disconnect( DS_DISCONN );
			g_pGameApp->SetLoginTime(0);

			g_pGameApp->SetIsRun( false );
		}
		break;
	case enumRelogin:
		{
			CS_Disconnect( DS_DISCONN );
			g_pGameApp->SetLoginTime(0);

			g_pGameApp->LoadScriptScene(enumLoginScene);
			CLoginScene* pScene = dynamic_cast<CLoginScene*>(g_pGameApp->GetCurScene());
			if( pScene )
			{
				pScene->ShowRegionList();
				//pScene->ShowLoginForm();
			}
		}
		break;
	};

	if( _eOptionType!=enumInit ) 
	{
		_eOptionType = enumInit;	
		return true;
	}
	return false;
}

void CChaExitOnTime::NetStartExit( DWORD dwExitTime )
{
	_dwEndTime = CGameApp::GetCurTick() + dwExitTime;

	g_pGameApp->SysInfo( g_oLangRec.GetString(776), dwExitTime / 1000 );
}

void CChaExitOnTime::NetCancelExit()
{
	_eOptionType = enumInit;

	g_pGameApp->SysInfo( g_oLangRec.GetString(777) );
}

void CChaExitOnTime::Reset()		
{ 
	_eOptionType = enumInit;
}

