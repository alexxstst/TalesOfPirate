#pragma once

#include <array>
#include <cstdint>
#include <string>

struct SPlaceCha
{
	int nTypeID;
	int nX;
	int nY;
};

class GameConfig
{
public:
	GameConfig();

	void Load();
	void SetDefault();

	void SetMoveClient(bool v);
	void SetEditor(bool v)        { _editor = v; }
	void SetFullScreen(bool v)    { _fullScreen = v; }
	void SetCreateScene(int v)    { _createScene = v; }
	void SetDoublePwd(bool v)     { _doublePwd = v; }

	bool IsAutoLogin()              const { return _autoLogin; }
	bool IsFullScreen()             const { return _fullScreen; }
	bool IsNoObj()                  const { return _noObj; }
	bool IsEditor()                 const { return _editor; }
	bool IsMusicEnabled()           const { return _musicEnabled; }
	bool IsCheckOvermax()           const { return _checkOvermax; }
	bool IsLgEnabled()              const { return _enableLg; }
	bool IsLgMsgEnabled()           const { return _enableLgMsg; }
	bool IsMultiThreadRes()         const { return _mThreadRes; }
	bool IsRenderSceneObj()         const { return _renderSceneObj; }
	bool IsRenderCha()              const { return _renderCha; }
	bool IsRenderEffect()           const { return _renderEffect; }
	bool IsRenderTerrain()          const { return _renderTerrain; }
	bool IsRenderUi()               const { return _renderUi; }
	bool IsRenderMinimap()          const { return _renderMinimap; }
	bool IsRender()                 const { return _render; }
	bool IsResourcePreload()        const { return _resourcePreload; }
	bool IsDiscordEnabled()         const { return _discordEnabled; }
	bool IsPower()                  const { return _showConsole; }
	bool IsConsoleEnabled()         const { return _consoleEnabled; }
	bool IsConsoleRequireSuperKey() const { return _consoleRequireSuperKey; }
	bool IsTextureLogEnabled()      const { return _textureLogEnabled; }
	bool IsMoveClient()             const { return _moveClient; }
	bool IsDoublePwd()              const { return _doublePwd; }

	int             GetChaCnt()             const { return _chaCnt; }
	const SPlaceCha& GetChaAt(int i)        const { return _chaList[i]; }
	const float*    GetLightDir()           const { return _lightDir.data(); }
	const float*    GetLightColor()         const { return _lightColor.data(); }
	std::uint8_t    GetScreenMode()         const { return _screenMode; }
	int             GetFogR()               const { return _fogR; }
	int             GetFogG()               const { return _fogG; }
	int             GetFogB()               const { return _fogB; }
	float           GetFogExp2()            const { return _fogExp2; }
	int             GetMaxChaType()         const { return _maxChaType; }
	int             GetMaxSceneObjType()    const { return _maxSceneObjType; }
	int             GetMaxEffectType()      const { return _maxEffectType; }
	int             GetMaxResourceNum()     const { return _maxResourceNum; }
	int             GetMaxItemType()        const { return _maxItemType; }
	int             GetSendHeartbeat()      const { return _sendHeartbeat; }
	std::uint32_t   GetConnectTimeOut()     const { return _connectTimeOut; }
	int             GetCreateScene()        const { return _createScene; }
	float           GetCameraVel()          const { return _cameraVel; }
	float           GetCameraAccl()         const { return _cameraAccl; }
	float           GetEyeX()               const { return _eyeX; }
	float           GetEyeY()               const { return _eyeY; }
	float           GetEyeZ()               const { return _eyeZ; }
	float           GetRefX()               const { return _refX; }
	float           GetRefY()               const { return _refY; }
	float           GetRefZ()               const { return _refZ; }
	float           GetFov()                const { return _fov; }
	float           GetAspect()             const { return _aspect; }
	float           GetNearClip()           const { return _nearClip; }
	float           GetFarClip()            const { return _farClip; }
	float           GetLgtFactor()          const { return _lgtFactor; }
	std::uint32_t   GetLgtBkColor()         const { return _lgtBkColor; }
	int             GetLeftHand()           const { return _leftHand; }
	int             GetRightHand()          const { return _rightHand; }
	std::uint32_t   GetFullScreenAntialias() const { return _fullScreenAntialias; }
	std::uint32_t   GetMaxCha()             const { return _maxCha; }
	std::uint32_t   GetMaxEff()             const { return _maxEff; }
	std::uint32_t   GetMaxItem()            const { return _maxItem; }
	std::uint32_t   GetMaxObj()             const { return _maxObj; }
	int             GetMovieW()             const { return _movieW; }
	int             GetMovieH()             const { return _movieH; }

	const std::string& GetMd5Pass()      const { return _md5Pass; }
	const std::string& GetVerErrorHttp() const { return _verErrorHttp; }
	const std::string& GetLocale()       const { return _locale; }
	const std::string& GetResPath()      const { return _resPath; }
	const std::string& GetFontName1()    const { return _fontName1; }
	const std::string& GetFontName2()    const { return _fontName2; }

private:
	bool          _autoLogin;
	bool          _fullScreen;
	SPlaceCha     _chaList[20];
	int           _chaCnt;
	std::array<float, 3> _lightDir;
	std::array<float, 3> _lightColor;
	bool          _noObj;
	bool          _editor;
	std::uint8_t  _screenMode;

	int           _fogR;
	int           _fogG;
	int           _fogB;
	float         _fogExp2;

	int           _maxChaType;
	int           _maxSceneObjType;
	int           _maxEffectType;
	int           _maxResourceNum;
	int           _maxItemType;
	bool          _musicEnabled;
	bool          _checkOvermax;

	int           _sendHeartbeat;
	std::uint32_t _connectTimeOut;

	bool          _enableLg;
	bool          _enableLgMsg;
	bool          _mThreadRes;

	int           _createScene;

	float         _cameraVel;
	float         _cameraAccl;

	float         _eyeX, _eyeY, _eyeZ;
	float         _refX, _refY, _refZ;
	float         _fov;
	float         _aspect;
	float         _nearClip;
	float         _farClip;

	float         _lgtFactor;
	std::uint32_t _lgtBkColor;

	int           _leftHand;
	int           _rightHand;

	bool          _renderSceneObj;
	bool          _renderCha;
	bool          _renderEffect;
	bool          _renderTerrain;
	bool          _renderUi;
	bool          _renderMinimap;
	bool          _render;

	std::uint32_t _fullScreenAntialias;

	std::uint32_t _maxCha;
	std::uint32_t _maxEff;
	std::uint32_t _maxItem;
	std::uint32_t _maxObj;

	bool          _resourcePreload;
	bool          _discordEnabled;

	std::string   _md5Pass;

	bool          _showConsole;
	bool          _consoleEnabled;
	bool          _consoleRequireSuperKey;
	bool          _textureLogEnabled;

	bool          _moveClient;

	std::string   _verErrorHttp;

	bool          _doublePwd;

	std::string   _locale;
	std::string   _resPath;
	std::string   _fontName1;
	std::string   _fontName2;

	int           _movieW;
	int           _movieH;
};

extern GameConfig GlobalAppConfig;
