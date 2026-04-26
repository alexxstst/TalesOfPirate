#include "stdafx.h"
#include "GameConfig.h"
#include "GameApp.h"
#include "GlobalVar.h"

using namespace std;

GameConfig GlobalAppConfig;

static float IniGetFloat(dbc::IniSection& sec, std::string_view key, float def = 0.0f) {
	auto s = sec.GetString(key);
	return s.empty() ? def : Str2Float(s);
}

GameConfig::GameConfig()
{
	g_bBinaryTable = TRUE;
	SetDefault();
}

void GameConfig::SetDefault()
{
	memset(_chaList, 0, sizeof(SPlaceCha) * 20);

	_autoLogin    = false;
	_fullScreen   = false;
	_musicEnabled = true;

	_chaCnt = 0;

	_lightColor = { 1.0f, 1.0f, 1.0f };
	_lightDir   = { 1.0f, 1.0f, -1.0f };

	_noObj       = false;
	_editor      = false;
	_screenMode  = 0;

	_maxChaType      = 350;
	_maxSceneObjType = 3000;
	_maxEffectType   = 14000;
	_maxItemType     = 32768;
	_maxResourceNum  = 3000;

	_cameraVel  = 0;
	_cameraAccl = 0;

	_createScene = 1;

	_leftHand  = 0;
	_rightHand = 0;

	_checkOvermax  = true;
	_sendHeartbeat = 30;
	_connectTimeOut = 0;

	_enableLgMsg = true;
	_enableLg    = true;

	_renderSceneObj = true;
	_renderCha      = true;
	_renderEffect   = true;
	_renderTerrain  = true;
	_renderUi       = true;
	_renderMinimap  = true;
	_render         = true;

	_mThreadRes = true;

	_fullScreenAntialias = 0;

	_lgtFactor   = 0.0f;
	_lgtBkColor  = 0xffc0c0c0;

	_maxCha  = 300;
	_maxEff  = 500;
	_maxItem = 400;
	_maxObj  = 800;

	_resourcePreload = true;

	_discordEnabled = false;

	_md5Pass.clear();
	_verErrorHttp.clear();

	_showConsole            = false;
	_consoleEnabled         = false;
	_consoleRequireSuperKey = true;
	_textureLogEnabled      = false;
	_moveClient             = true;

	_fontName1.clear();
	_fontName2.clear();
	_locale.clear();
	_resPath.clear();

	_movieW = -1;
	_movieH = -1;
}

void GameConfig::Load()
{
	ToLogService("common", "Load Game Config from g_SystemIni");

	{
		auto& sec = g_SystemIni["Character Display"];
		_autoLogin = sec.GetInt64("autologin", 0) != 0;
		auto chaStr = sec.GetString("cha");
		if (!chaStr.empty()) {
			string strCha[3];
			if (Util_ResolveTextLine(chaStr.c_str(), strCha, 3, ',') == 3 && _chaCnt < 20) {
				_chaList[_chaCnt].nTypeID = Str2Int(strCha[0]);
				_chaList[_chaCnt].nX      = Str2Int(strCha[1]);
				_chaList[_chaCnt].nY      = Str2Int(strCha[2]);
				_chaCnt++;
			}
		}
		_leftHand  = static_cast<int>(sec.GetInt64("left_hand", 0));
		_rightHand = static_cast<int>(sec.GetInt64("right_hand", 0));
	}

	{
		auto& sec = g_SystemIni["Menu"];
		_fullScreen = sec.GetInt64("fullscreen", 0) != 0;
		_screenMode = static_cast<std::uint8_t>(sec.GetInt64("screenmode", 0));
		_noObj      = sec.GetInt64("noobj", 0) != 0;
		_fogR = static_cast<int>(sec.GetInt64("fogcolorR", 0));
		_fogG = static_cast<int>(sec.GetInt64("fogcolorG", 0));
		_fogB = static_cast<int>(sec.GetInt64("fogcolorB", 0));
		_fogExp2 = IniGetFloat(sec, "fogexp2");
		_fullScreenAntialias = static_cast<std::uint32_t>(sec.GetInt64("fullscreen_antialias", 0));
		_lgtFactor = IniGetFloat(sec, "lgt_factor", 0.4f);

		auto lightDir = sec.GetString("light_dir");
		if (!lightDir.empty()) {
			string parts[3];
			if (Util_ResolveTextLine(lightDir.c_str(), parts, 3, ',') == 3) {
				_lightDir[0] = Str2Float(parts[0]);
				_lightDir[1] = Str2Float(parts[1]);
				_lightDir[2] = Str2Float(parts[2]);
			}
		}
		auto lightColor = sec.GetString("light_color");
		if (!lightColor.empty()) {
			string parts[3];
			if (Util_ResolveTextLine(lightColor.c_str(), parts, 3, ',') == 3) {
				_lightColor[0] = Str2Float(parts[0]);
				_lightColor[1] = Str2Float(parts[1]);
				_lightColor[2] = Str2Float(parts[2]);
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
				_lgtBkColor = c.d;
			}
		}
		_movieW = static_cast<int>(sec.GetInt64("movie_w", -1));
		_movieH = static_cast<int>(sec.GetInt64("movie_h", -1));
	}

	{
		auto& sec = g_SystemIni["Login Camera Angle"];
		_eyeX    = IniGetFloat(sec, "eyeX",   104.362f);
		_eyeY    = IniGetFloat(sec, "eyeY",   99.325f);
		_eyeZ    = IniGetFloat(sec, "eyeZ",   293.851f);
		_refX    = IniGetFloat(sec, "refX",   -348.953f);
		_refY    = IniGetFloat(sec, "refY",   -1714.419f);
		_refZ    = IniGetFloat(sec, "refZ",   -1070.085f);
		_fov     = IniGetFloat(sec, "fov",    90.0f);
		_aspect  = IniGetFloat(sec, "Aspect", 1.0f);
		_nearClip = IniGetFloat(sec, "near",   20.0f);
		_farClip  = IniGetFloat(sec, "far",    2000.0f);
	}

	_createScene = static_cast<int>(g_SystemIni["Activate Scene"].GetInt64("CreateScene", 1));

	_musicEnabled = g_SystemIni["audio"].GetInt64("musicEnabled", 1) != 0;

	{
		auto& sec = g_SystemIni["Camera Speed"];
		_cameraVel  = IniGetFloat(sec, "cameraVel",  8.7f);
		_cameraAccl = IniGetFloat(sec, "cameraAccl", 0.05f);
	}

	_checkOvermax = g_SystemIni["Editor"].GetInt64("check_overmax", 1) != 0;

	{
		auto& sec = g_SystemIni["Internet"];
		_sendHeartbeat  = static_cast<int>(sec.GetInt64("send_heartbeat", 600));
		_connectTimeOut = 1000 * static_cast<std::uint32_t>(sec.GetInt64("connect_time_out", 40));
	}

	{
		auto& sec = g_SystemIni["Log"];
		_enableLg    = sec.GetInt64("enable_lg", 1) != 0;
		_enableLgMsg = sec.GetInt64("enable_lg_msg", 0) != 0;
		_showConsole = sec.GetInt64("console", 0) != 0;
	}

	{
		auto& sec = g_SystemIni["Console"];
		_consoleEnabled         = sec.GetInt64("enabled", 0) != 0;
		_consoleRequireSuperKey = sec.GetInt64("requireSuperKey", 1) != 0;
	}

	{
		auto& sec = g_SystemIni["TextureLog"];
		_textureLogEnabled = sec.GetInt64("enabled", 0) != 0;
	}

	{
		auto& sec = g_SystemIni["Romance Setting"];
		_render         = sec.GetInt64("render", 1) != 0;
		_renderSceneObj = sec.GetInt64("sceneobj_render", 1) != 0;
		_renderCha      = sec.GetInt64("cha_render", 1) != 0;
		_renderEffect   = sec.GetInt64("effect_render", 1) != 0;
		_renderTerrain  = sec.GetInt64("terrain_render", 1) != 0;
		_renderMinimap  = sec.GetInt64("minimap_render", 1) != 0;
		_renderUi       = sec.GetInt64("ui_render", 1) != 0;
	}

	_mThreadRes = g_SystemIni["Read Resource"].GetInt64("multithreadres", 1) != 0;

	{
		auto& sec = g_SystemIni["Resources"];
		_maxCha  = static_cast<std::uint32_t>(sec.GetInt64("MaxChaNum", 300));
		_maxEff  = static_cast<std::uint32_t>(sec.GetInt64("MaxEffNum", 500));
		_maxItem = static_cast<std::uint32_t>(sec.GetInt64("MaxItemNum", 400));
		_maxObj  = static_cast<std::uint32_t>(sec.GetInt64("MaxObjNum", 800));
		_resourcePreload = sec.GetInt64("preload_at_start", 1) != 0;
		auto locale = sec.GetString("locale");
		if (!locale.empty()) _locale = locale;
		auto path = sec.GetString("path");
		if (!path.empty()) _resPath = path;
		auto font1 = sec.GetString("fontname1");
		if (!font1.empty()) _fontName1 = font1;
		auto font2 = sec.GetString("fontname2");
		if (!font2.empty()) _fontName2 = font2;
	}

	_discordEnabled = g_SystemIni["Discord"].GetInt64("enabled", 0) != 0;

	{
		auto md5 = g_SystemIni["Security"].GetString("md5pass");
		if (!md5.empty()) _md5Pass = md5;
	}

	{
		auto http = g_SystemIni["Version"].GetString("HTTP");
		if (!http.empty()) _verErrorHttp = http;
	}
}

void GameConfig::SetMoveClient(bool v)
{
	_moveClient = v;
}
