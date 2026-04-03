#include "lua_gamectrl.h"

// , 0
int IsChaInTeam(CCharacter* pCha);

// , 0
CCharacter* GetTeamCha(CCharacter* pCha, int nNo);

//
int IsChaInRegion(CCharacter* pCha, int nRegionDef);

//
std::string GetChaDefaultName(CCharacter* pCha);

extern int GetChaAttr_raw(lua_State *pLS);
//
// Delegates to raw GetChaAttr_raw from Expand.h
int GetChaAttrI_raw(lua_State *L);

//
// Inlined version of SetChaAttr logic (original lua_SetChaAttr was removed)
int SetChaAttrI_raw(lua_State *L);

//
int IsPlayer(CCharacter* pCha);

// ,
void SetChaAttrMax(int nNo, unsigned int lValue);

// , ,
void AddWeatherRegion(int btType, int dwFre, int dwLastTime, int sx, int sy, int w, int h);

//
void ClearMapWeather();

//
void SetBoatCtrlTick(CCharacter* pCha, int tick);

//
int GetBoatCtrlTick(CCharacter* pCha);

// ,
//  : , (1   2),
//
CCharacter* SummonCha(CCharacter* pHost, int sType, int sChaInfoID);

//
//  :
//
void DelCha(CCharacter* pCTarCha);
