#include "stdafx.h"
#include "GameConfig.h"
#include "GameApp.h"
#include "GlobalVar.h"

using namespace std;

CGameConfig g_Config;

static float IniGetFloat(dbc::IniSection& sec, std::string_view key, float def = 0.0f) {
	auto s = sec.GetString(key);
	return s.empty() ? def : Str2Float(s);
}

CGameConfig::CGameConfig()
{
	g_bBinaryTable	= TRUE;
	SetDefault();
}

void CGameConfig::SetDefault()   // 
{
	memset(m_ChaList, 0, sizeof(SPlaceCha) * 20);

	m_bAutoLogin	    = FALSE; 
	
	m_bFullScreen       = FALSE;
	MusicEnabled        = true;
	
	m_nChaCnt			= 0;  // 

	m_LightColor[0]     =  1.0f; 
	m_LightColor[1]     =  1.0f;
	m_LightColor[2]     =  1.0f;
	m_LightDir[0]       =  1.0f;
	m_LightDir[1]       =  1.0f;
	m_LightDir[2]       = -1.0f;

    m_bNoObj            = FALSE; 
    m_bEditor           = FALSE; //  
    m_btScreenMode      = 0;

    m_nMaxChaType       = 350;
	m_nMaxSceneObjType  = 3000;
	m_nMaxEffectType    = 14000;		// 	modify by Michael 2005-11-9
	m_nMaxItemType      = 32768;
	m_nMaxResourceNum	= 3000;        


	m_fCameraVel		= 0;  
	m_fCameraAccl		= 0;

	m_nCreateScene		= 1;          // 


	nLeftHand            = 0;      // 
    nRightHand           = 0;

	m_bCheckOvermax      = TRUE; 
    m_nSendHeartbeat     = 30;
	m_nConnectTimeOut	 = 0;   // 

    m_bEnableLGMsg       = TRUE;     //   LG-Box
    m_bEnableLG          = TRUE;     //   LG

	m_bRenderSceneObj = TRUE;  //
	m_bRenderCha      = TRUE;  //
	m_bRenderEffect   = TRUE;
	m_bRenderTerrain  = TRUE;
	m_bRenderUI       = TRUE;
	m_bRenderMinimap  = TRUE;
	m_bRender         = TRUE;

	m_bMThreadRes	  = TRUE;	//

    m_dwFullScreenAntialias = 0;

    m_fLgtFactor = 0.0f;
    m_dwLgtBkColor = 0xffc0c0c0;
    //
	m_dwMaxCha	= 300;
	m_dwMaxEff	= 500;
	m_dwMaxItem = 400;
	m_dwMaxObj	= 800;

	m_bResourcePreload = TRUE;

	strcpy(m_szMD5Pass, "");  // MD5
	memset( m_szVerErrorHTTP, 0, sizeof(m_szVerErrorHTTP) ); 

	m_IsShowConsole = false; //
	m_bConsoleEnabled = FALSE;
	m_bConsoleRequireSuperKey = TRUE;
	m_bTextureLogEnabled = FALSE;
	m_IsMoveClient = true;
	m_IsBill = false; //  

	// Add by lark.li 20080429 for res
	memcpy(m_szFontName1, "", sizeof(m_szFontName1));
	memcpy(m_szFontName2, "", sizeof(m_szFontName1));
	// Res

	// Add by lark.li 20080825 begin
	m_nMovieW = -1;
	m_nMovieH = -1;
	// End
}

void CGameConfig::Load()
{
	ToLogService("common", "Load Game Config from g_SystemIni");

	// [Character Display]
	{
		auto& sec = g_SystemIni["Character Display"];
		m_bAutoLogin = static_cast<int>(sec.GetInt64("autologin", 0));
		auto chaStr = sec.GetString("cha");
		if (!chaStr.empty()) {
			string strCha[3];
			if (Util_ResolveTextLine(chaStr.c_str(), strCha, 3, ',') == 3 && m_nChaCnt < 20) {
				m_ChaList[m_nChaCnt].nTypeID = Str2Int(strCha[0]);
				m_ChaList[m_nChaCnt].nX      = Str2Int(strCha[1]);
				m_ChaList[m_nChaCnt].nY      = Str2Int(strCha[2]);
				m_nChaCnt++;
			}
		}
		nLeftHand  = static_cast<int>(sec.GetInt64("left_hand", 0));
		nRightHand = static_cast<int>(sec.GetInt64("right_hand", 0));
	}

	// [Menu]
	{
		auto& sec = g_SystemIni["Menu"];
		m_bFullScreen  = static_cast<int>(sec.GetInt64("fullscreen", 0));
		m_btScreenMode = static_cast<BYTE>(sec.GetInt64("screenmode", 0));
		m_bNoObj       = static_cast<int>(sec.GetInt64("noobj", 0));
		m_iFogR = static_cast<int>(sec.GetInt64("fogcolorR", 0));
		m_iFogG = static_cast<int>(sec.GetInt64("fogcolorG", 0));
		m_iFogB = static_cast<int>(sec.GetInt64("fogcolorB", 0));
		m_fExp2 = IniGetFloat(sec, "fogexp2");
		m_dwFullScreenAntialias = static_cast<DWORD>(sec.GetInt64("fullscreen_antialias", 0));
		m_fLgtFactor = IniGetFloat(sec, "lgt_factor", 0.4f);

		auto lightDir = sec.GetString("light_dir");
		if (!lightDir.empty()) {
			string parts[3];
			if (Util_ResolveTextLine(lightDir.c_str(), parts, 3, ',') == 3) {
				m_LightDir[0] = Str2Float(parts[0]);
				m_LightDir[1] = Str2Float(parts[1]);
				m_LightDir[2] = Str2Float(parts[2]);
			}
		}
		auto lightColor = sec.GetString("light_color");
		if (!lightColor.empty()) {
			string parts[3];
			if (Util_ResolveTextLine(lightColor.c_str(), parts, 3, ',') == 3) {
				m_LightColor[0] = Str2Float(parts[0]);
				m_LightColor[1] = Str2Float(parts[1]);
				m_LightColor[2] = Str2Float(parts[2]);
			}
		}
		auto bkColor = sec.GetString("lgt_bkcolor");
		if (!bkColor.empty()) {
			string parts[3];
			if (Util_ResolveTextLine(bkColor.c_str(), parts, 3, ',') == 3) {
				lwDwordByte4 c;
				c.b[3] = 0xff;
				c.b[2] = Str2Int(parts[0]);
				c.b[1] = Str2Int(parts[1]);
				c.b[0] = Str2Int(parts[2]);
				m_dwLgtBkColor = c.d;
			}
		}
		m_nMovieW = static_cast<int>(sec.GetInt64("movie_w", -1));
		m_nMovieH = static_cast<int>(sec.GetInt64("movie_h", -1));
	}

	// [Login Camera Angle]
	{
		auto& sec = g_SystemIni["Login Camera Angle"];
		eyeX    = IniGetFloat(sec, "eyeX",   104.362f);
		eyeY    = IniGetFloat(sec, "eyeY",   99.325f);
		eyeZ    = IniGetFloat(sec, "eyeZ",   293.851f);
		refX    = IniGetFloat(sec, "refX",   -348.953f);
		refY    = IniGetFloat(sec, "refY",   -1714.419f);
		refZ    = IniGetFloat(sec, "refZ",   -1070.085f);
		fov     = IniGetFloat(sec, "fov",    90.0f);
		fAspect = IniGetFloat(sec, "Aspect", 1.0f);
		fnear   = IniGetFloat(sec, "near",   20.0f);
		ffar    = IniGetFloat(sec, "far",    2000.0f);
	}

	// [Activate Scene]
	m_nCreateScene = static_cast<int>(g_SystemIni["Activate Scene"].GetInt64("CreateScene", 1));

	//  [audio] musicEnabled — глобальный переключатель звука (музыка + SFX).
	//  Громкости (musicSound / musicEffect) в той же секции регулируются отдельно через UI.
	MusicEnabled = g_SystemIni["audio"].GetInt64("musicEnabled", 1) != 0;

	// [Camera Speed]
	{
		auto& sec = g_SystemIni["Camera Speed"];
		m_fCameraVel  = IniGetFloat(sec, "cameraVel",  8.7f);
		m_fCameraAccl = IniGetFloat(sec, "cameraAccl", 0.05f);
	}

	// [Editor]
	m_bCheckOvermax = static_cast<int>(g_SystemIni["Editor"].GetInt64("check_overmax", 1));

	// [Internet]
	{
		auto& sec = g_SystemIni["Internet"];
		m_nSendHeartbeat  = static_cast<int>(sec.GetInt64("send_heartbeat", 600));
		m_nConnectTimeOut = 1000 * static_cast<int>(sec.GetInt64("connect_time_out", 40));
#ifdef USED_BILL
		m_IsBill = true;
#else
		m_IsBill = false;
#endif
	}

	// [Log]
	{
		auto& sec = g_SystemIni["Log"];
		m_bEnableLG    = static_cast<int>(sec.GetInt64("enable_lg", 1));
		m_bEnableLGMsg = static_cast<int>(sec.GetInt64("enable_lg_msg", 0));
		m_IsShowConsole = sec.GetInt64("console", 0) != 0;
	}

	// [Console] — новая секция, гейт игровой консоли (замена `#ifdef DEBUG`).
	{
		auto& sec = g_SystemIni["Console"];
		m_bConsoleEnabled         = static_cast<int>(sec.GetInt64("enabled", 0));
		m_bConsoleRequireSuperKey = static_cast<int>(sec.GetInt64("requireSuperKey", 1));
	}

	// [TextureLog] — диагностический лог загрузок текстур, канал "textures".
	{
		auto& sec = g_SystemIni["TextureLog"];
		m_bTextureLogEnabled = static_cast<int>(sec.GetInt64("enabled", 0));
	}

	// [Console Debug] — визуальные настройки читаются из Lua
	// (console_bootstrap.lua через SystemIni:Section(...)), см. font_name /
	// font_size / font_bold / width_percent / transparency_percent / lines.

	// [Romance Setting]
	{
		auto& sec = g_SystemIni["Romance Setting"];
		m_bRender         = static_cast<int>(sec.GetInt64("render", 1));
		m_bRenderSceneObj = static_cast<int>(sec.GetInt64("sceneobj_render", 1));
		m_bRenderCha      = static_cast<int>(sec.GetInt64("cha_render", 1));
		m_bRenderEffect   = static_cast<int>(sec.GetInt64("effect_render", 1));
		m_bRenderTerrain  = static_cast<int>(sec.GetInt64("terrain_render", 1));
		m_bRenderMinimap  = static_cast<int>(sec.GetInt64("minimap_render", 1));
		m_bRenderUI       = static_cast<int>(sec.GetInt64("ui_render", 1));
	}

	// [Read Resource]
	m_bMThreadRes = static_cast<int>(g_SystemIni["Read Resource"].GetInt64("multithreadres", 1));

	// [Resources]
	{
		auto& sec = g_SystemIni["Resources"];
		m_dwMaxCha  = static_cast<DWORD>(sec.GetInt64("MaxChaNum", 300));
		m_dwMaxEff  = static_cast<DWORD>(sec.GetInt64("MaxEffNum", 500));
		m_dwMaxItem = static_cast<DWORD>(sec.GetInt64("MaxItemNum", 400));
		m_dwMaxObj  = static_cast<DWORD>(sec.GetInt64("MaxObjNum", 800));
		m_bResourcePreload = static_cast<int>(sec.GetInt64("preload_at_start", 1));
		auto locale = sec.GetString("locale");
		if (!locale.empty()) strcpy(m_szLocale, locale.c_str());
		auto path = sec.GetString("path");
		if (!path.empty()) strcpy(m_szResPath, path.c_str());
		auto font1 = sec.GetString("fontname1");
		if (!font1.empty()) strcpy(m_szFontName1, font1.c_str());
		auto font2 = sec.GetString("fontname2");
		if (!font2.empty()) strcpy(m_szFontName2, font2.c_str());
	}

	// [Security]
	{
		auto md5 = g_SystemIni["Security"].GetString("md5pass");
		if (!md5.empty()) strcpy(m_szMD5Pass, md5.c_str());
	}

	// [Version]
	{
		auto http = g_SystemIni["Version"].GetString("HTTP");
		if (!http.empty()) strcpy(m_szVerErrorHTTP, http.c_str());
	}
}

void CGameConfig::SetMoveClient( bool v )  // 
{
	m_IsMoveClient = v;
	//g_pGameApp->SysInfo( g_Config.m_IsMoveClient ? GetLanguageString(142) : GetLanguageString(141) );
}
