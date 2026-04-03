#include "Stdafx.h"

#include "GameApp.h"

#include "SceneObjRecordStore.h"
#include "EffectRecordStore.h"
#include "MusicRecordStore.h"
#include "MapRecordStore.h"
#include "AreaRecord.h"

#include "MPEditor.h"
#include "FindPath.h"
#include "MPFont.h"

#include "EffectObj.h"
#include "InputBox.h"
#include "ItemRefineSet.h"
#include "ItemRefineEffectRecordStore.h"
#include "GameWG.h"
#include "GameMovie.h"
#include "ResourceBundleManage.h"
#include "pi_Alloc.h"
#include "LootFilter.h"

#ifndef USE_DSOUND
#include "AudioThread.h"
CAudioThread	g_AudioThread;
#endif

// 
CLanguageRecord g_oLangRec("./scripts/table/StringSet.bin", "./scripts/table/StringSet.txt");
CResourceBundleManage g_ResourceBundleManage("Game.loc");			// These objects are just being called here to avoid linker errors,
pi_LeakReporter pi_leakReporter("gameleak.log");		// since client uses StringSet instead of .res files.


bool	volatile	g_bLoadRes				  = FALSE;
CGameApp*	        g_pGameApp	              = NULL;




MPEditor	        g_Editor;
CFindPath			g_cFindPath(128,38);
CFindPathEx         g_cFindPathEx;          //Add by mdr
CInputBox			g_InputBox;

CGameWG             g_oGameWG;
CGameMovie			g_GameMovie;

LootFilter*         g_lootFilter            = NULL;

// , GateServer
short g_sClientVer = 32125;//32140 //32125
short g_sKeyData = short(g_sClientVer * g_sClientVer * 0x93828311); // 0x1232222
char g_szSendKey[4];
char g_szRecvKey[4];

DWORD g_dwCurMusicID = 0;

// CLightEff plight;
