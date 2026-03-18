// EXPAND_H//=============================================================================
// FileName: Expand.h
// Creater: ZhangXuedong
// Date: 2004.11.22
// Comment: expand
//=============================================================================
//modify by alfred.shi 20080306

#pragma once

#include "stdafx.h"   //add by alfred.shi 20080304

#include "Character.h"
#include "LogFile.h"
#include "excp.h"
#include "Parser.h"
#include "GameDB.h"
#include "MapEntry.h"
//#define defPARSE_LOG


_DBC_USING

extern const char* GetResPath(const char *pszRes);

inline int lua_SynLook(lua_State *pLS){
	CCharacter *pCha = (CCharacter*)lua_touserdata(pLS, 1);
	pCha->SynSkillStateToEyeshot();
	pCha->SynLook();
	return 0;
}

inline int lua_IsAttributeEditable(lua_State *pLS){
	SItemGrid* item = (SItemGrid*)lua_touserdata(pLS, 1);
	int attribute = lua_tonumber(pLS, 3);
	for (int i = 0; i < 5; i++){
		if (item->sInstAttr[i][0] == attribute){
			lua_pushnumber(pLS, i);
			return 1;
		}
	}
	lua_pushnumber(pLS, -1);
	return 1;
}

inline int lua_SetAttributeEditable(lua_State *pLS){
	SItemGrid* item = (SItemGrid*)lua_touserdata(pLS, 1);
	int slot = lua_tonumber(pLS, 2);
	if (slot >= 0 && slot < 5){
		int attribute = lua_tonumber(pLS, 3);
		item->sInstAttr[slot][0] = attribute;
		item->sInstAttr[slot][1] = g_pCItemAttr[item->sID].GetAttr(attribute, false);
	}
	return 0;
}

//TODO(Ogge): No verification of pointers from lua
inline int lua_EquipItem(lua_State *pLS){
	//todo - check if valid equip?
	CCharacter *pCha = (CCharacter*)lua_touserdata(pLS, 1);
	int chEquipPos = lua_tonumber(pLS, 2);
	SItemGrid* equip = (SItemGrid*)lua_touserdata(pLS, 3);

	pCha->m_SChaPart.SLink[chEquipPos] = *equip;
	pCha->m_SChaPart.SLink[chEquipPos].SetChange();
	pCha->ChangeItem(true, equip, chEquipPos);

	pCha->SynSkillStateToEyeshot();
	pCha->SynLook();
	return 0;
}

inline int lua_EquipStringItem(lua_State *pLS){
	/*
	//todo - check if valid equip?
	CCharacter *pCha = (CCharacter*)lua_touserdata(pLS, 1);
	int chEquipPos = lua_tonumber(pLS, 2);
	const char* pszData = lua_tostring(pLS, 3);
	const short csSubNum = 8 + enumITEMDBP_MAXNUM + defITEM_INSTANCE_ATTR_NUM_VER110 * 2 + 1;
	std::string strSubList[csSubNum];

	SItemGrid SGridCont;
	int sTCount = 0;
	Util_ResolveTextLine(pszData, strSubList, csSubNum, ',');
	SGridCont.sID = Str2Int(strSubList[sTCount++]);
	SGridCont.sNum = Str2Int(strSubList[sTCount++]);
	SGridCont.sEndure[0] = Str2Int(strSubList[sTCount++]);
	SGridCont.sEndure[1] = Str2Int(strSubList[sTCount++]);
	SGridCont.sEnergy[0] = Str2Int(strSubList[sTCount++]);
	SGridCont.sEnergy[1] = Str2Int(strSubList[sTCount++]);
	SGridCont.chForgeLv = Str2Int(strSubList[sTCount++]);
	

	for (int m = 0; m < enumITEMDBP_MAXNUM; m++)
	{
		SGridCont.SetDBParam(m, Str2Int(strSubList[sTCount++]));
	}
	if (Str2Int(strSubList[sTCount++]) > 0)
	{
		for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
		{
			SGridCont.sInstAttr[k][0] = Str2Int(strSubList[sTCount + k * 2]);
			SGridCont.sInstAttr[k][1] = Str2Int(strSubList[sTCount + k * 2 + 1]);
		}
	}
	else
		SGridCont.SetInstAttrInvalid();

	memcpy(pCha->m_SChaPart.SLink + chEquipPos, &SGridCont, sizeof(SItemGrid));
	pCha->m_SChaPart.SLink[chEquipPos].SetChange();
	pCha->ChangeItem(true, &SGridCont, chEquipPos);

	pCha->SynSkillStateToEyeshot();
	pCha->SynLook();
	return 0;
	*/
	return 0;
}


inline int lua_GetChaGuildPermission(lua_State *pLS)
{
	int nParaNum = lua_gettop(pLS); // 取得参数个数
	if (nParaNum != 1){
		return 0;
	}
	CCharacter *pCha = (CCharacter*)lua_touserdata(pLS, 1);
	lua_pushnumber(pLS, pCha->guildPermission);
	return 1;
}

inline int lua_GetIMP(lua_State *pLS){
	if (lua_gettop(pLS) != 1 || !lua_islightuserdata(pLS, 1)){
		return 0;
	}
	CCharacter *pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	int IMP = pCCha->GetIMP();
	lua_pushnumber(pLS, IMP);
	return 1;
}

inline int lua_SetIMP(lua_State *pLS){

	if ((lua_gettop(pLS) == 2 || lua_gettop(pLS) == 3) && lua_islightuserdata(pLS, 1) && lua_isnumber(pLS, 2)){
		CCharacter *pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		int IMP = lua_tonumber(pLS, 2);
		if (lua_gettop(pLS) == 3){
			pCCha->SetIMP(IMP,false);
		}else{
			pCCha->SetIMP(IMP,true);
		}
	}

	
	
	return 0;
}

inline int lua_GetChaAttr(lua_State *pLS)
{
	bool	bSuccess = true;
	int		nAttrVal;
	LONG32  lAttrVal;

	int nParaNum = lua_gettop(pLS); // 取得参数个数
	short sAttrIndex = -1;
	if (nParaNum > 2)
	{
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		sAttrIndex = (unsigned __int64)lua_tonumber(pLS, 2);
		if (!pCCha)
		{
			bSuccess = false;
			goto End;
		}
		if (sAttrIndex < 0 || sAttrIndex >= ATTR_MAX_NUM)
		{
			bSuccess = false;
			goto End;
		}

		/*if(sAttrIndex == ATTR_CEXP || sAttrIndex == ATTR_CEXP || sAttrIndex == ATTR_CEXP)
			lAttrVal = pCCha->getAttr(sAttrIndex);
		else */
		//nAttrVal = (long)pCCha->getAttr(sAttrIndex);
		nAttrVal = pCCha->getAttr(sAttrIndex);

	}
	End:
	if (bSuccess)
	{
		/*if(sAttrIndex == ATTR_CEXP || sAttrIndex == ATTR_CEXP || sAttrIndex == ATTR_CEXP)
		{
			if (lAttrVal < 0 && g_IsNosignChaAttr(sAttrIndex))
				lua_pushnumber(pLS, (unsigned)lAttrVal);
			else
				lua_pushnumber(pLS, lAttrVal);
		}
		else
		{*/
		if (nAttrVal < 0 && g_IsNosignChaAttr(sAttrIndex))
			lua_pushnumber(pLS, (double)(unsigned int)nAttrVal);
		else
			lua_pushnumber(pLS, (double)nAttrVal);
		//}
		return 1;
	}
	else
		return 0;
}

// 设置角色对应属性的值
inline int lua_SetChaAttr(lua_State *pLS)
{
	bool	bSuccess = true;

	int nParaNum = lua_gettop(pLS); // 取得参数个数
	if (nParaNum > 3)
	{
		bSuccess = false;
		goto End;
	}

	{
		CCharacter* pCCha = static_cast<CCharacter*>(lua_touserdata(pLS, 1));
		short sAttrIndex = static_cast<int64_t>(lua_tonumber(pLS, 2));
		LONG32 lValue = static_cast<int64_t>(lua_tonumber(pLS, 3));

		if (!pCCha)
		{
			bSuccess = false;
			goto End;
		}
		if (sAttrIndex < 0 || sAttrIndex >= ATTR_MAX_NUM)
		{
			bSuccess = false;
			goto End;
		}

		long	lSetRet = pCCha->setAttr(sAttrIndex, lValue);
		if (lSetRet == 0)
		{
			bSuccess = false;
			goto End;
		}
	}
	End:
	if (bSuccess)
	{
		return 1;
	}
	return 0;
}

// 返回角色类型：0，非玩家。1，玩家。
inline int lua_CheckChaRole(lua_State *pLS)
{
bool	bSuccess = true;
int		nType = 0;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("获取角色类型 CheckChaRole\n");
g_pCLogObj->Log("Get character style :CheckChaRole\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum > 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	if (pCCha->IsPlayerCha())
		nType = 1;
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	char	szPrint[256] = "";
	//sprintf(szPrint, "获取角色%s 的类型成功。值：%d\n", pCCha->GetLogName(), nType);
	sprintf(szPrint, RES_STRING(GM_EXPAND_H_00012), pCCha->GetLogName(), nType);
	g_pCLogObj->Log(szPrint);
	g_pCLogObj->Log("\n");
#endif

	lua_pushnumber(pLS, nType);
	return 1;
}
else
return 0;
}

// 取角色的玩家对象
// 参数：角色对象
// 返回：玩家对象（非玩家对象返回0）
inline int lua_GetChaPlayer(lua_State *pLS)
{
bool	bSuccess = true;
CPlayer	*pCPly = 0;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("取角色的玩家对象 GetChaPlayer\n");
g_pCLogObj->Log("Getting the character's player object : GetChaPlayer\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum > 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	pCPly = pCCha->GetPlayer();
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取角色的玩家对象成功\n");
	g_pCLogObj->Log("Getting the character's player object  succeed\n");
#endif
}

if (pCPly)
{
	lua_pushlightuserdata(pLS, pCPly);
	return 1;
}
else
return 0;
}

// 取玩家的队伍编号
// 参数：玩家对象
// 返回：队伍编号
inline int lua_GetPlayerTeamID(lua_State* pLS)
{
	{
		bool bSuccess = true;
		DWORD ret = 0;
#ifdef defPARSE_LOG
		g_pCLogObj->Log("Getting the player Team ID\n");
#endif

		int nParaNum = lua_gettop(pLS);

		if (nParaNum != 1) {
#ifdef defPARSE_LOG
			g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
			bSuccess = false;
			goto End;
		}
		{
			CPlayer* pCPly = (CPlayer*)lua_touserdata(pLS, 1);
			if (!pCPly) {
#ifdef defPARSE_LOG
				g_pCLogObj->Log("the player object is inexistence,transfer failed\n");
#endif
				bSuccess = false;
				goto End;
			}
			ret = pCPly->getTeamLeaderID();
		}
	End:
#ifdef defPARSE_LOG
		if (bSuccess) {
			g_pCLogObj->Log("Getting the player Team ID succeed \n");
		}
#endif
		lua_pushnumber(pLS, ret);

		return 1;
	}
}

// 取玩家的编号
// 参数：玩家对象
// 返回：编号
inline int lua_GetPlayerID(lua_State *pLS)
{
bool	bSuccess = true;
short ret = 0;
#ifdef defPARSE_LOG
//g_pCLogObj->Log("取玩家的编号 GetPlayerID\n");
g_pCLogObj->Log("Getting the player ID: GetPlayerID\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CPlayer* pCPly = (CPlayer*)lua_touserdata(pLS, 1);
	if (!pCPly)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t玩家对象不存在，调用失败\n");
		g_pCLogObj->Log("the player object is inexistence,transfer failed\n");
#endif
		bSuccess = false;
		goto End;
	}
	ret = pCPly->GetID();
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取玩家的编号成功\n");
	g_pCLogObj->Log("Getting the player ID succeed\n");
#endif
}
lua_pushnumber(pLS, ret);

return 1;
}

// 设置技能区域
// 参数：区域类型编号，对应该类型的参数（参见CompCommand.h ERangeType）
// 返回值：无
inline int lua_SetSkillRange(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("设置技能的作用区域 SetSkillRange\n");
g_pCLogObj->Log("Setting the effect range of Skill SetSkillRange\n");

#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum < 1 || nParaNum >= defSKILL_RANGE_EXTEP_NUM)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}

short	sRangeE[defSKILL_RANGE_EXTEP_NUM];
for (int i = 0; i < nParaNum; i++)
sRangeE[i] = (short)lua_tonumber(pLS, i + 1);
g_pGameApp->SetSkillTDataRange(sRangeE);

End:
if (bSuccess)
{
	char	szGene[512] = "";
	for (int i = 0; i < nParaNum; i++)
		sprintf(szGene + strlen(szGene), "%d,", sRangeE[i]);
	szGene[strlen(szGene)] = '\0';

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置技能作用区域成功：区域数据[%s]\n", szGene);
	g_pCLogObj->Log("Setting the effect range of Skill succeed : range data[%s]\n", szGene);
#endif
}

return 0;
}

// 设置技能区域状态
// 参数：状态编号，等级，时间（秒）
// 返回值：无
inline int lua_SetRangeState(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("设置技能区域状态 SetRangeState\n");
g_pCLogObj->Log("Setting the range of Skill state : SetRangeState\n ");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 3)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}

short	sState[defSKILL_STATE_PARAM_NUM];
for (int i = 0; i < nParaNum; i++)
sState[i] = (short)lua_tonumber(pLS, i + 1);
g_pGameApp->SetSkillTDataState(sState);

End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置技能区域状态成功：编号 %d，等级 %d，持续时间 %d\n",
	g_pCLogObj->Log("Setting the range of Skill state succeed :ID %d,grade %d,duration %d\n",sState[0], sState[1], sState[2]);

#endif
}

return 0;
}

// 取得技能点的位置（单位：厘米）
// 参数：无
// 返回值：位置[x,y]
inline int lua_GetSkillPos(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("取得技能点的位置 GetSkillPos\n");
g_pCLogObj->Log("Getting the position of Skill spot :GetSkillPos\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}

End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取得技能点的位置成功：[%d,%d]\n", g_SSkillPoint.x, g_SSkillPoint.y);
	g_pCLogObj->Log("Getting the position of Skill spot succeed :[%d,%d]\n", g_SSkillPoint.x, g_SSkillPoint.y);
#endif

	lua_pushnumber(pLS, g_SSkillPoint.x);
	lua_pushnumber(pLS, g_SSkillPoint.y);
	return 2;
}
else
return 0;
}

// 取得对象的技能等级
// 参数：角色对象
//       技能编号
// 返回值：技能等级
inline int lua_GetSkillLv(lua_State *pLS)
{
bool	bSuccess = true;
char chSkillLv = 0;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("取得对象的技能等级 GetSkillLv\n");
g_pCLogObj->Log("Getting the object's Skill grade : GetSkillLv\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 2)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	int nSkillID = (int)lua_tonumber(pLS, 2);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	{
		SSkillGrid* pSkill = pCCha->m_CSkillBag.GetSkillContByID(nSkillID);
		if (pSkill)
		{
			chSkillLv = pSkill->chLv;
		}
	}
}

End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取得对象 %s 的技能 %d 等级成功：%d\n", pCCha->GetLogName(), nSkillID, chSkillLv);
	g_pCLogObj->Log("Getting the object %s Skill and %d grade succeed :%d\n", pCCha->GetLogName(), nSkillID, chSkillLv);
#endif

	lua_pushnumber(pLS, chSkillLv);
	return 1;
}
else
return 0;
}

// 取得角色对应的状态等级
// 参数：角色对象
//       状态编号
// 返回值：状态等级
inline int lua_GetChaStateLv(lua_State *pLS)
{
bool	bSuccess = true;
unsigned char uchStateLv = 0;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("取得角色对应的状态等级 GetChaStateLv\n");
g_pCLogObj->Log("Getting the character corresponding grade state :GetChaStateLv\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 2)
{
#ifdef defPARSE_LOG
	//	g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	unsigned char uchStateID = (unsigned char)lua_tonumber(pLS, 2);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	SSkillStateUnit* pState = pCCha->m_CSkillState.GetSStateByID(uchStateID);
	if (pState)
	{
		uchStateLv = pState->GetStateLv();
	}
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取得角色 %s 的状态 %d 等级成功：%d\n", pCCha->GetLogName(), uchStateID, uchStateLv);
	g_pCLogObj->Log("Getting the character %s state and %d grade succeed :%d\n", pCCha->GetLogName(), uchStateID, uchStateLv);
#endif

	lua_pushnumber(pLS, uchStateLv);
	return 1;
}
else
return 0;
}

// 取得技能对象的方向
// 参数：角色对象
// 返回值：方向（单位：度）
inline int lua_GetObjDire(lua_State *pLS)
{
bool	bSuccess = true;
int ret = 0;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("取得技能对象的方向 GetObjDire\n");
g_pCLogObj->Log("Getting the direction of Skill object: GetObjDire\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}
	ret = pCCha->GetAngle();
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取得技能对象 %s 的方向成功：%d\n", pCCha->GetLogName(), pCCha->GetAngle());
	g_pCLogObj->Log("Getting the %s direction of Skill object succeed :%d\n ", pCCha->GetLogName(), pCCha->GetAngle());
#endif

	lua_pushnumber(pLS, ret);
	return 1;
}
else
return 0;
}

// 增加技能对象的状态
// 参数：使用方对象，被用方对象
//       状态编号，状态等级，状态持续时间
// 返回值：无
inline int lua_AddState(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("增加技能对象的状态 AddState\n");
g_pCLogObj->Log("Increasing the state of Skill object : AddState\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 5)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}

{
	CCharacter* pCSrcCha = (CCharacter*)lua_touserdata(pLS, 1);
	CCharacter* pCTarCha = (CCharacter*)lua_touserdata(pLS, 2);
	unsigned char	uchStateID = (unsigned char)lua_tonumber(pLS, 3);
	unsigned char	uchStateLV = (unsigned char)lua_tonumber(pLS, 4);
	int		nOnTime = (int)lua_tonumber(pLS, 5);

	if (!pCSrcCha || !pCTarCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象 %d 不存在，调用失败\n");
		g_pCLogObj->Log("\tthe player object %d is inexistence,transfer failed\n");
#endif
		bSuccess = false;
		goto End;
	}

	if (!pCTarCha->AddSkillState(g_uchFightID, pCSrcCha->GetID(), pCSrcCha->GetHandle(), enumSKILL_TYPE_SELF, enumSKILL_TAR_LORS, enumSKILL_EFF_HELPFUL, uchStateID, uchStateLV, nOnTime, enumSSTATE_ADD_UNDEFINED, false))
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t向主程序添加状态不成功[添加状态号%d，最大状态号%d]，调用失败\n", uchStateID, SKILL_STATE_MAXID);
		g_pCLogObj->Log("\tAdding the state to Main program failed [Adding uchStateID %d,SKILL_STATE_MAXID %d] ,transfer failed", uchStateID, SKILL_STATE_MAXID);
#endif
		bSuccess = false;
		goto End;
	}
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("%s 向 %s 增加技能状态成功：状态 %d，等级 %d，持续时间 %d\n", pCSrcCha->GetLogName(), pCTarCha->GetLogName(), uchStateID, uchStateLV, nOnTime);
	g_pCLogObj->Log("%s add Skill state to %s succeed : state %d,grade %d,duration %d\n", pCSrcCha->GetLogName(), pCTarCha->GetLogName(), uchStateID, uchStateLV, nOnTime);
#endif
}

return 0;
}

// 删除技能对象的状态
// 参数：角色对象，状态编号
// 返回值：无
inline int lua_RemoveState(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("删除技能对象的状态 RemoveState\n");
g_pCLogObj->Log("Delete the Skill object state :RemoveState\n ");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 2)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	unsigned char	uchStateID = (unsigned char)lua_tonumber(pLS, 2);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	if (!pCCha->DelSkillState(uchStateID))
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t向主程序删除状态不成功[添加状态号%d，最大状态号%d]，调用失败\n", uchStateID, SKILL_STATE_MAXID);
		g_pCLogObj->Log("\tDelete the state to Main program failed[uchStateID %d,SKILL_STATE_MAXID %d],transfer failed", uchStateID, SKILL_STATE_MAXID);
#endif
		bSuccess = false;
		goto End;
	}
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("删除技能对象 %s 的状态成功：状态 %d\n", pCCha->GetLogName(), uchStateID);
	g_pCLogObj->Log("Delete the %s Skill object state succeed : uchStateID %d\n", pCCha->GetLogName(), uchStateID);
#endif
}

return 0;
}

// 角色所在地表的某个状态的等级
// 参数：角色对象，状态编号
// 返回值：状态等级（0表示没有该状态）
inline int lua_GetAreaStateLevel(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("角色所在地表的某个状态的等级 GetAreaStateLevel\n");
	g_pCLogObj->Log("The certain state grade of player in area ");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数
	unsigned char	uchStateLv = 0;

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		unsigned char	uchStateID = (unsigned char)lua_tonumber(pLS, 2);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
			g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
			bSuccess = false;
			goto End;
		}

		SSkillStateUnit* pState = pCCha->GetAreaState(uchStateID);
		if (pState)
			uchStateLv = pState->GetStateLv();
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("角色 %s 所处的地表存在该状态：%d\n", pCCha->GetLogName(), uchStateID);
		g_pCLogObj->Log("the character %s  has this state in area : %d\n", pCCha->GetLogName(), uchStateID);

#endif
	}

	lua_pushnumber(pLS, uchStateLv);
	return 1;
}

// 技能Miss
// 参数：角色对象
// 返回值：无
inline int lua_SkillMiss(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("技能没有命中 SkillMiss\n");
g_pCLogObj->Log("Skill has not hit the target : SkillMiss\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	pCCha->m_SFightProc.bMiss = true;
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置对象 %s 的“技能没有命中“成功\n", pCCha->GetLogName());
	g_pCLogObj->Log("Setting object %s 's"SkillMiss"succeed\n", pCCha->GetLogName());
#endif
}

return 0;
}

// 技能Crt
// 参数：角色对象
// 返回值：无
inline int lua_SkillCrt(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("技能爆击 SkillCrt\n");
g_pCLogObj->Log("SkillCrt :SkillCrt\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	pCCha->m_SFightProc.bCrt = true;
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置对象 %s 的“技能爆击“成功\n", pCCha->GetLogName());
	g_pCLogObj->Log("Setting object %s 's"SkillCrt"succeed\n", pCCha->GetLogName());
#endif
}

return 0;
}

// 技能不能使用
// 参数：角色对象
// 返回值：无
inline int lua_SkillUnable(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("技能不能使用（MP，能量，道具等不足） SkillUnable\n");
g_pCLogObj->Log("the Skill can't use(MP,energy,Item and otherwise deficiency ) SkillUnable\n");
#endif


int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	pCCha->m_SFightProc.sState = enumFSTATE_NO_EXPEND;

	//LG("技能失败", "角色名 %s。\n", pCCha->m_CLog.GetLogName());
	LG("skill failed", "role's name %s。\n", pCCha->m_CLog.GetLogName());
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置对象 %s 的“技能不能使用“成功\n", pCCha->GetLogName());
	g_pCLogObj->Log("Setting object %s 's"SkillUnable"succeed \n", pCCha->GetLogName());
#endif
}

return 0;
}

// 增加技能
// 参数：角色对象，技能编号，等级，等级是设置还是在原来的基础上增加（1，设置。0，增加），是否扣除需要的技能点（１，扣除。０，不扣除）
// 返回值：1，成功。0，不成功
inline int lua_AddChaSkill(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("增加技能 AddChaSkill\n");
g_pCLogObj->Log("Adding Skill : AddChaSkill\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum < 5)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	int		nSkillID = (int)lua_tonumber(pLS, 2);
	int		nSkillLv = (int)lua_tonumber(pLS, 3);
	bool	bSetLv = (int)lua_tonumber(pLS, 4) == 1 ? true : false;
	bool	bUsePoint = (int)lua_tonumber(pLS, 5) == 1 ? true : false;

	bool	checkReq = true;
	if (nParaNum == 6) {
		checkReq = (int)lua_tonumber(pLS, 6) == 1 ? false : true;
	}


	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t对象不存在，调用失败\n");
		g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
		bSuccess = false;
		goto End;
	}
	if (pCCha->GetPlayer())
		pCCha = pCCha->GetPlayer()->GetMainCha();

	if (!pCCha->LearnSkill((short)nSkillID, (char)nSkillLv, bSetLv, bUsePoint, checkReq))
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t向主程序增加技能不成功[添加技能号%d，等级%d，最大技能号%d]，调用失败\n", nSkillID, nSkillLv, defMAX_SKILL_NO);
		g_pCLogObj->Log("\tAdding the Skill to Main program failed[nSkillID %d,nSkillLv %d,defMAX_SKILL_NO %d],transfer failed\n", nSkillID, nSkillLv, defMAX_SKILL_NO);
#endif
		bSuccess = false;
		goto End;
	}
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("向 %s 增加技能成功：技能编号 %d，等级 %d。（设置 %d。扣除技能点 %d。）\n",
	g_pCLogObj->Log("Setting add Skill to %s succeed : nSkillID%d,nSkillLv%d,(Setting %d.Deduct SkillID %d.)\n",
		pCCha->GetLogName(), nSkillID, nSkillLv, bSetLv ? 1 : 0, bUsePoint ? 1 : 0);
#endif
	lua_pushnumber(pLS, 1);
}
else
lua_pushnumber(pLS, 0);

return 1;
}

// 使用道具失败
inline int lua_UseItemFailed(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("使用道具失败 UseItemFailed\n");
g_pCLogObj->Log("Using Item Failed :UseItemFailed\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	g_chUseItemFailed[0] = 1;
}

End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("调用\"使用道具失败\"成功\n");
	g_pCLogObj->Log("transfer\"use the Item failed \"succeed \n ");
#endif
}

return 0;
}

//// 给任务时使用道具失败
//inline int lua_UseItemGiveMission(lua_State *pLS)
//{
//	bool	bSuccess = true;
//
//
//	int nParaNum = lua_gettop(pLS); // 取得参数个数
//
//	if (nParaNum != 1)
//	{
//		bSuccess = false;
//		goto End;
//	}
//
//	CCharacter	*pCCha = (CCharacter*)lua_touserdata(pLS, 1);
//	g_chUseItemGiveMission[0] = 1;
//
//End:
//	if (bSuccess)
//	{
//	}
//
//	return 0;
//}

// 设置物品掉落
// 参数：掉落个数，物品索引（以基数1进行编号）
// 返回值：无
inline int lua_SetItemFall(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("设置物品掉落 SetItemFall\n");
g_pCLogObj->Log("Setting Item Fall : SetItemFall\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum < 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}

g_chItemFall[0] = (char)lua_tonumber(pLS, 1);
if (g_chItemFall[0] > defCHA_INIT_ITEM_NUM)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\tmsg设置爆料个数错误（设置值 %d，允许值 %d），调用失败\n", g_chItemFall[0], defCHA_INIT_ITEM_NUM);
	g_pCLogObj->Log("\tmsg Setting ", g_chItemFall[0], defCHA_INIT_ITEM_NUM);
#endif
	bSuccess = false;
	goto End;
}
for (int i = 0; i < g_chItemFall[0]; i++)
g_chItemFall[i + 1] = (char)lua_tonumber(pLS, i + 2);

End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置物品掉落成功，掉落个数：%d\n", g_chItemFall[0]);
	g_pCLogObj->Log("Setting Item fall succeed,the number of fall:%d\n", g_chItemFall[0]);
#endif
}

return 0;
}

// 击退
// 参数：攻击方对象，受击方对象，击退长度（厘米）
// 返回值：无
inline int lua_BeatBack(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("击退 BeatBack\n");
g_pCLogObj->Log("Beat Back :BeatBack \n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 3)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCSrcCha = (CCharacter*)lua_touserdata(pLS, 1);
	CCharacter* pCTarCha = (CCharacter*)lua_touserdata(pLS, 2);
	int		nBackLen = (int)lua_tonumber(pLS, 3);

	if (!pCSrcCha || !pCTarCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t击退对象不存在，调用失败\n");
		g_pCLogObj->Log("\tthe Beatback object is inexistence,transfer failed \n");
#endif
		bSuccess = false;
		goto End;
	}

	Point	SSrcPos, STarPos;
	Point	STarNewPos;
	SSrcPos = pCSrcCha->GetPos();
	STarPos = pCTarCha->GetPos();
	int		nDist1 = (int)sqrt(double((SSrcPos.x - STarPos.x) * (SSrcPos.x - STarPos.x) + (SSrcPos.y - STarPos.y) * (SSrcPos.y - STarPos.y)));
	int		nDist2 = nDist1 + nBackLen;
	STarNewPos.x = nDist2 * (STarPos.x - SSrcPos.x) / nDist1 + SSrcPos.x;
	STarNewPos.y = nDist2 * (STarPos.y - SSrcPos.y) / nDist1 + SSrcPos.y;
	unsigned long	ulElapse;
	pCTarCha->LinearAttemptMove(STarNewPos, nBackLen, &ulElapse);
	STarNewPos = pCTarCha->GetPos();
	pCTarCha->SetPos(STarPos);
	pCTarCha->m_submap->MoveTo(pCTarCha, STarNewPos);
	g_bBeatBack = true;
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("击退%dcm成功\n", nBackLen);
	g_pCLogObj->Log("BeatBack %dcm succeed \n", nBackLen);
#endif
}

return 0;
}

// 是否在竞技
// 参数：角色对象
// 返回值：1，是。0，不是
inline int lua_IsInGymkhana(lua_State *pLS)
{
bool	bSuccess = true;
char chRet = 0; 

#ifdef defPARSE_LOG
//g_pCLogObj->Log("是否在竞技 IsInGymkhana\n");
g_pCLogObj->Log("whether is in gymkhana : IsInGymkhana\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t对象不存在，调用失败\n");
		g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
		bSuccess = false;
		goto End;
	}

	chRet = pCCha->IsInGymkhana() ? 1 : 0;
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("角色 %s 的竞技状态 %d\n", pCCha->GetLogName(), chRet);
	g_pCLogObj->Log("character %s 's state of athletics ", pCCha->GetLogName(), chRet);
#endif
	lua_pushnumber(pLS, chRet);
	return 1;
}
else
return 0;
}

// 是否在PK
// 参数：角色对象
// 返回值：1，是。0，不是
inline int lua_IsInPK(lua_State *pLS)
{
bool	bSuccess = true;
char chRet = 0;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("是否在竞技 IsInGymkhana\n");
g_pCLogObj->Log("whether is in gymkhana : IsInGymkhana\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t对象不存在，调用失败\n");
		g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
		bSuccess = false;
		goto End;
	}

	chRet = pCCha->IsInPK() ? 1 : 0;
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("角色 %s 的PK状态 %d\n", pCCha->GetLogName(), chRet);
	g_pCLogObj->Log("character %s 's PK state %d\n", pCCha->GetLogName(), chRet);
#endif
	lua_pushnumber(pLS, chRet);
	return 1;
}
else
return 0;
}

// 得到背包中某一道具的数目
// 参数：角色，道具编号
// 返回值：道具的数目
inline int lua_CheckBagItem(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("得到背包中某一道具的数目 CheckBagItem\n");
	g_pCLogObj->Log("Get the number of some items in kitbag : CheckBagItem \n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数
	short sItemNum = 0;

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}

	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");

#endif
			bSuccess = false;
			goto End;
		}

		short sID = (short)lua_tonumber(pLS, 2);

		short sGridNum = pCCha->m_CKitbag.GetUseGridNum();
		SItemGrid* pGridCont;
		for (short i = 0; i < sGridNum; i++)
		{
			pGridCont = pCCha->m_CKitbag.GetGridContByNum(i);
			if (!pGridCont || pGridCont->sID != sID)
				continue;
			sItemNum += pGridCont->sNum;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("角色 %s 的背包中道具（编号 %d）的数目是 %d\n", pCCha->GetLogName(), sID, sItemNum);
		g_pCLogObj->Log("the character %s 's kitbag Item(ID %d) number is %d \n", pCCha->GetLogName(), sID, sItemNum);
#endif
	}

	lua_pushnumber(pLS, sItemNum);
	return 1;
}

// 得到背包的闲置的格子数
// 参数：角色对象
// 返回值：闲置的格子数
inline int lua_GetChaFreeTempBagGridNum(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("得到背包的闲置的格子数 GetChaFreeBagGridNum\n");
	g_pCLogObj->Log("Get the number of grid from kitbag where leave unused :GetChaFreeBagGridNum \n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数
	short sFreeNum = 0;

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		sFreeNum = pCCha->m_pCKitbagTmp->GetCapacity() - pCCha->m_pCKitbagTmp->GetUseGridNum();
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("角色 %s 的背包中道具（编号 %d）的数目是 %d\n", pCCha->GetLogName(), sID, sItemNum);
		g_pCLogObj->Log("the character %s 's kitbag Item(ID %d) number is %d \n", pCCha->GetLogName(), sID, sItemNum);
#endif
	}

	lua_pushnumber(pLS, sFreeNum);
	return 1;
}

inline int lua_GetChaFreeBagGridNum(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("得到背包的闲置的格子数 GetChaFreeBagGridNum\n");
	g_pCLogObj->Log("Get the number of grid from kitbag where leave unused :GetChaFreeBagGridNum \n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数
	short sFreeNum = 0;

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		sFreeNum = pCCha->m_CKitbag.GetCapacity() - pCCha->m_CKitbag.GetUseGridNum();
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("角色 %s 的背包中道具（编号 %d）的数目是 %d\n", pCCha->GetLogName(), sID, sItemNum);
		g_pCLogObj->Log("the character %s 's kitbag Item(ID %d) number is %d \n", pCCha->GetLogName(), sID, sItemNum);
#endif
	}

	lua_pushnumber(pLS, sFreeNum);
	return 1;
}

// 删除背包中的道具。
// 参数：角色，道具编号，道具数目
// 返回值：1 成功，0 失败
inline int lua_DelBagItem(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("删除背包中的道具 DelBagItem\n");
	g_pCLogObj->Log("Delete the Item from kitbag : DelBagItem \n");
#endif

	SItemGrid *pGridCont, DelCont;

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");

#endif
			bSuccess = false;
			goto End;
		}

		short sID = (short)lua_tonumber(pLS, 2);
		short sNum = (short)lua_tonumber(pLS, 3);

		short sLeftNum = sNum;
		short sGridNum = pCCha->m_CKitbag.GetUseGridNum();
		for (short i = 0; i < sGridNum; i++)
		{
			pGridCont = pCCha->m_CKitbag.GetGridContByNum(i);
			if (!pGridCont || pGridCont->sID != sID)
				continue;

			sLeftNum -= pGridCont->sNum;
			if (sLeftNum <= 0)
				break;
		}
		if (sLeftNum > 0)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t道具数目不够（要求数目 %d，当前数目%d），调用失败\n", sNum, sNum - sLeftNum);
			g_pCLogObj->Log("\tHave not enough Items(require number %d,current number %d),transfer failed \n", sNum, sNum - sLeftNum);
#endif
			bSuccess = false;
			goto End;
		}

		sLeftNum = sNum;
		pCCha->m_CKitbag.SetChangeFlag(false);
		DelCont.sID = sID;
		for (short i = 0; i < sGridNum; i++)
		{
			pGridCont = pCCha->m_CKitbag.GetGridContByNum(i);
			if (!pGridCont || pGridCont->sID != sID)
				continue;

			if (pGridCont->sNum >= sLeftNum)
				DelCont.sNum = sLeftNum;
			else
				DelCont.sNum = pGridCont->sNum;
			pCCha->KbPopItem(true, true, &DelCont, pCCha->m_CKitbag.GetPosIDByNum(i));
			sLeftNum -= pGridCont->sNum;
			if (sLeftNum <= 0)
				break;
		}
		pCCha->SynKitbagNew(enumSYN_KITBAG_TO_NPC);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("删除背包中的道具（编号，数目）成功\n", sID, sNum);
		g_pCLogObj->Log("Delete the item of kitbag (ID,number) succeed \n", sID, sNum);
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 删除背包中的道具。
// 参数：角色，道具对象，道具数目（0，为删除该格上的所有道具）
// 返回值：1 成功，0 失败
inline int lua_DelBagItem2(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("删除背包中的道具 DelBagItem2\n");
	g_pCLogObj->Log("Delete the item of kitbag : DelBagItem2 \n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		SItemGrid* pGridCont = (SItemGrid*)lua_touserdata(pLS, 2);
		if (!pGridCont)
		{
			bSuccess = false;
			goto End;
		}
		short sNum = (short)lua_tonumber(pLS, 3);
		if (sNum == 0)
			sNum = pGridCont->sNum;

		if (pGridCont->sNum < sNum)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t道具数目不够（要求数目 %d，当前数目%d），调用失败\n", sNum, pGridCont->sNum);
			g_pCLogObj->Log("\tHave not enough Items(require number %d,current number %d),transfer failed \n", sNum, pGridCont->sNum);
#endif
			bSuccess = false;
			goto End;
		}

		pCCha->m_CKitbag.SetChangeFlag(false);
		pCCha->KbClearItem(true, true, pGridCont, sNum);
		pCCha->SynKitbagNew(enumSYN_KITBAG_TO_NPC);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("删除背包中的道具（编号，数目）成功\n", sID, sNum);
		g_pCLogObj->Log("Delete the item of kitbag (ID,number) succeed \n", sID, sNum);
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 移除角色道具
// 参数：角色对象
//       道具编号
//       道具数目（0为对应位置的全部数目）
//       移除部位（1，从装备栏移除。2，从道具栏移除。0，从装备栏和道具栏移除）
//       移除位置（-1为对应部位的全部位置）
//       目标部位（0，移除到地面。1，到道具栏。2，删除）
//       是否强制移除（1，强制移除。0，不强制移除），当道具栏被锁或存在不能操作道具的状态时，使用此参数可忽略这些因数
//       是否需要同步信息（缺省值为1）
// 返回值：1 成功，0 失败
inline int lua_RemoveChaItem(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("删除背包中的道具 DelBagItem\n");
	g_pCLogObj->Log("Delete the Item from kitbag : DelBagItem \n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum < 7)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		if (pCCha->m_CKitbag.IsLock())
		{
			pCCha->SystemNotice("Unable to remove item. Inventory locked!");
			bSuccess = false;
			return FALSE;
		}
		{
			long	lItemID = (long)lua_tonumber(pLS, 2);
			long	lItemNum = (long)lua_tonumber(pLS, 3);
			char	chFromType = (char)lua_tonumber(pLS, 4);
			short	sFromID = (short)lua_tonumber(pLS, 5);
			char	chToType = (char)lua_tonumber(pLS, 6);

			char	chForcible = (char)lua_tonumber(pLS, 7);
			bool	bNotice = true;
			if (nParaNum == 8)
				bNotice = (char)lua_tonumber(pLS, 8) != 0 ? true : false;
			if (pCCha->Cmd_RemoveItem(lItemID, lItemNum, chFromType, sFromID, chToType, 0, bNotice, chForcible) != enumITEMOPT_SUCCESS)
			{
				bSuccess = false;
				goto End;
			}
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("移除角色道具成功\n");
		g_pCLogObj->Log("remove the character item succeed \n");
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 取角色所在地图的名称
// 参数：角色
// 返回值：地图名
inline int lua_GetChaMapName(lua_State *pLS)
{
	bool	bSuccess = true;
	const char* ret = "";

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取角色所在地图的名称 GetMapName\n");
	g_pCLogObj->Log("Get the Map name where character in : GetMapName\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log(""\tThe parameter numbers [%d] is unlawful,transfer failed!\n"", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		if (!pCCha->GetSubMap())
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象地图不存在，调用失败\n");
			g_pCLogObj->Log("\tthe object map is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		ret = pCCha->GetSubMap()->GetName();
	}

End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取角色所在地图的名称成功：%s\n", sID, szMapName);
		g_pCLogObj->Log("Get the Map name where character in is succeed :%s\n ", sID, szMapName);
#endif
		lua_pushstring(pLS, ret);
		return 1;
	}

	return 0;
}

// 取角色所在的地图副本号
// 参数：角色
// 返回值：副本号（小于等于0为无效的编号）
inline int lua_GetChaMapCopyNO(lua_State *pLS)
{
	bool	bSuccess = true;
	short ret = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取角色所在地图的名称 GetMapName\n");
	g_pCLogObj->Log("Get the Map name where character in : GetMapName\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		if (!pCCha->GetSubMap())
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象地图不存在，调用失败\n");
			g_pCLogObj->Log("\tthe object map is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		ret = pCCha->GetSubMap()->GetCopyNO() + 1;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		g_pCLogObj->Log("Get the Map name where character in is succeed :%s\n ", sID, szMapName);
#endif
	}
	lua_pushnumber(pLS, ret);

	return 1;
}

// 取角色所在的地图副本对象
// 参数：角色
// 返回值：副本对象
inline int lua_GetChaMapCopy(lua_State *pLS)
{
	bool	bSuccess = true;
	SubMap	*pCMapCopy = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取角色所在的地图副本对象 GetChaMapCopy\n");
	g_pCLogObj->Log("Get the character 's map copy object : GetChaMapCopy\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed \n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMapCopy = pCCha->GetSubMap();
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取角色所在地图的名称成功：%s\n", sID, szMapName);
		g_pCLogObj->Log("Get the Map name where character in is succeed :%s\n ", sID, szMapName);
#endif
	}

	if (pCMapCopy)
	{
		lua_pushlightuserdata(pLS, pCMapCopy);
		return 1;
	}
	else
		return 0;
}

// 取角色的主角
// 参数：角色
// 返回值：主角
inline int lua_GetMainCha(lua_State *pLS)
{
	bool	bSuccess = true;
	CCharacter	*pCMainCha = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取角色的主角 GetMainCha\n");
	g_pCLogObj->Log("Get the Main character of player : GetMainCha\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha || !pCCha->GetPlayer())
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed \n");
#endif
			bSuccess = false;
			goto End;
		}

		pCMainCha = pCCha->GetPlayer()->GetMainCha();
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取角色的主角成功\n");
		g_pCLogObj->Log("Get the Main character of player succeed\n");
#endif
	}

	if (!pCMainCha)
		return 0;

	lua_pushlightuserdata(pLS, pCMainCha);
	return 1;
}

// 取船角色
// 参数：主角色
// 返回值：船角色，如果不存在，返回0
inline int lua_GetCtrlBoat(lua_State *pLS)
{
	bool	bSuccess = true;
	CCharacter	*pCCtrlBoat = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取船角色 GetCtrlBoat\n");
	g_pCLogObj->Log("Get the boat character : GetCtrlBoat\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha || !pCCha->GetPlayer())
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		pCCtrlBoat = pCCha->GetPlayer()->GetCtrlCha();
		if (!pCCtrlBoat->IsBoat())
			pCCtrlBoat = 0;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取船角色成功\n");
		g_pCLogObj->Log("Get the boat character succeed\n");
#endif
	}

	if (!pCCtrlBoat)
		return 0;

	lua_pushlightuserdata(pLS, pCCtrlBoat);
	return 1;
}

// 判断是否船角色
// 参数：角色
// 返回值：1 船角色，0 不是
inline int lua_ChaIsBoat(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("判断是否船角色 ChaIsBoat\n");
	g_pCLogObj->Log("judge the character whether is boat : ChaIsBoat\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数
	char	chIsBoat = 0;

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha || !pCCha->GetPlayer())
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		if (pCCha->IsBoat())
			chIsBoat = 1;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("判断是否船角色成功\n");
		g_pCLogObj->Log("judge the character whether is boat succeed \n");
#endif
	}

	lua_pushnumber(pLS, chIsBoat);
	return 1;
}

// 取角色的道具
// 角色对象；道具区域：1，装备栏。2，道具栏；位置编号
// 返回值：道具指针
inline int lua_GetChaItem(lua_State *pLS)
{
	bool	bSuccess = true;
	SItemGrid	*pSItem = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取角色的道具 GetChaItem\n");
	g_pCLogObj->Log("Get the character Item : GetChaItem\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		pSItem = pCCha->GetItem2((char)lua_tonumber(pLS, 2), (long)lua_tonumber(pLS, 3));
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取角色的道具成功\n");
		g_pCLogObj->Log("Get the character Item succeed \n");
#endif
	}

	if (pSItem)
	{
		lua_pushlightuserdata(pLS, pSItem);
		return 1;
	}
	else
		return 0;
}

// 取角色的道具
// 角色对象；道具区域：1，装备栏。2，道具栏；道具编号
// 返回值：道具指针
inline int lua_GetChaItem2(lua_State *pLS)
{
	bool	bSuccess = true;
	SItemGrid	*pSItem = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取角色的道具 GetChaItem\n");
	g_pCLogObj->Log("Get the character Item : GetChaItem\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		pSItem = pCCha->GetItem((char)lua_tonumber(pLS, 2), (long)lua_tonumber(pLS, 3));
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取角色的道具成功\n");
		g_pCLogObj->Log("Get the character Item succeed \n");
#endif
	}

	if (pSItem)
	{
		lua_pushlightuserdata(pLS, pSItem);
		return 1;
	}
	else
		return 0;
}

inline int lua_MoveToTemp(lua_State *pLS){
	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2){
		lua_pushnumber(pLS,  LUA_FALSE);
		return 1;
	}

	CCharacter	*pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	SItemGrid	*pSItem = (SItemGrid *)lua_touserdata(pLS, 2);

	if (!pCCha || !pSItem){
		lua_pushnumber(pLS,  LUA_FALSE);
		return 1;
	}
	short gridID1 = 0;
	short sPushPos = defKITBAG_DEFPUSH_POS;
	SItemGrid Grid;
	Grid.sNum = 0;

	short slot = -1;
	SItemGrid	*pSItemCont;
	for (int i = 0;i<48; i++){
		pSItemCont = pCCha->m_CKitbag.GetGridContByID(i);
		if (pSItemCont == pSItem){
			slot = i;
			break;
		}
	}

	if (slot == -1){
		lua_pushnumber(pLS, LUA_FALSE);
		return 1;
	}


	pCCha->m_pCKitbagTmp->Push(pSItem, sPushPos);
	pCCha->m_CKitbag.Pop(&Grid, slot);
	pCCha->SynKitbagNew(enumSYN_KITBAG_SWITCH);

	if (Grid.sNum > 0)
	{
		pCCha->m_CKitbag.Push(&Grid, slot);
		pCCha->SynKitbagTmpNew(enumSYN_KITBAG_SWITCH);
		pCCha->SynKitbagNew(enumSYN_KITBAG_SWITCH);
		lua_pushnumber(pLS, LUA_FALSE);
		return 1;
	}

	pCCha->SynKitbagTmpNew(enumSYN_KITBAG_SWITCH);

	lua_pushnumber(pLS, LUA_TRUE);
	return 1;
}

inline int lua_GetItemAttr(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取道具实例属性 GetItemAttr\n");
	g_pCLogObj->Log("Get Item instance attribute : GetItemAttr\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数
	long	lItemAttr = 0;

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		{
			long	lAttrID = (int)lua_tonumber(pLS, 2);
			if (lAttrID == ITEMATTR_VAL_PARAM1)
			{
				lItemAttr = pSItem->GetDBParam(0);
			}
			else if (lAttrID == ITEMATTR_VAL_PARAM2)
			{
				lItemAttr = pSItem->GetDBParam(1);
			}
			else if (lAttrID == ITEMATTR_VAL_LEVEL)
			{
				lItemAttr = pSItem->GetItemLevel();
			}
			else if (lAttrID == ITEMATTR_VAL_FUSIONID)
			{
				lItemAttr = pSItem->GetFusionItemID();
			}
			else
			{
				lItemAttr = pSItem->GetInstAttr(lAttrID);
			}
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取道具实例属性成功\n");
		g_pCLogObj->Log("Get Item instance attribute succeed\n");
#endif

		lua_pushnumber(pLS, lItemAttr);
		return 1;
	}

	return 0;
}


inline int lua_GetItemStackSize(lua_State *pLS){
	int nParaNum = lua_gettop(pLS);
	if (nParaNum != 1 || !lua_islightuserdata(pLS,1)){
		return 0;
	}
	SItemGrid	*pSItem = (SItemGrid *)lua_touserdata(pLS, 1);
	if (!pSItem){
		lua_pushnumber(pLS, 0);
	}else{
		lua_pushnumber(pLS, pSItem->sNum);
	}
	return 1;
}

inline int lua_IsItemLocked(lua_State *pLS){
	int nParaNum = lua_gettop(pLS);
	if (nParaNum != 1 || !lua_islightuserdata(pLS, 1)){
		return 0;
	}
	SItemGrid	*pSItem = (SItemGrid *)lua_touserdata(pLS, 1);
	if (!pSItem){
		lua_pushboolean(pLS, false);
	}
	else{
		lua_pushboolean(pLS, pSItem->dwDBID > 0);
	}
	return 1;
}


// 设置实例道具属性
// 道具对象；道具属性编号；属性值
// 返回值：１，成功。０，失败
inline int lua_SetItemAttr(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置实例道具属性 SetItemAttr\n");
	g_pCLogObj->Log("Set Item instance attribute : SetItemAttr\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		{
			long	lAttrID = (int)lua_tonumber(pLS, 2);
			short	sAttr = (short)lua_tonumber(pLS, 3);
			if (lAttrID == ITEMATTR_VAL_PARAM1)
			{
				pSItem->SetDBParam(0, sAttr);
			}
			else if (lAttrID == ITEMATTR_VAL_PARAM2)
			{
				pSItem->SetDBParam(1, sAttr);
			}
			else if (lAttrID == ITEMATTR_VAL_LEVEL)
			{
				pSItem->SetItemLevel(char(sAttr));
			}
			else if (lAttrID == ITEMATTR_VAL_FUSIONID)
			{
				pSItem->SetFusionItemID(sAttr);
			}
			else
			{
				if (!pSItem->SetInstAttr(lAttrID, sAttr))
					bSuccess = false;
			}
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置实例道具属性成功\n");
		g_pCLogObj->Log("Set Item instance attribute succeed \n" );
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 增加道具实例属性
// 道具对象；道具属性编号；属性值
// 返回值：１，成功。０，失败
inline int lua_AddItemAttr(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("增加道具实例属性 AddItemAttr\n");
	g_pCLogObj->Log("Add Item instance attribute : AddItemAttr\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		{
			long	lAttrID = (int)lua_tonumber(pLS, 2);
			short	sAttr = (short)lua_tonumber(pLS, 3);

			if (lAttrID == ITEMATTR_VAL_PARAM1)
			{
			}
			else if (lAttrID == ITEMATTR_VAL_PARAM2)
			{
			}
			else if (lAttrID == ITEMATTR_VAL_LEVEL)
			{
				pSItem->AddItemLevel(char(sAttr));
			}
			else if (lAttrID == ITEMATTR_VAL_FUSIONID)
			{
			}
			else
			{
				if (!pSItem->SetInstAttr(lAttrID, sAttr))
					bSuccess = false;
			}
		}
	}

End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("增加道具实例属性成功\n");
		g_pCLogObj->Log("Add Item instance attribute succeed\n");
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 取道具属性
// 道具对象；道具属性编号
// 返回值：道具属性值
inline int lua_GetItemFinalAttr(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取道具属性 GetItemFinalAttr\n");
	g_pCLogObj->Log("Get the Item Final attribute ：GetItemFinalAttr\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数
	long	lItemAttr = 0;

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		{
			long	lAttrID = (int)lua_tonumber(pLS, 2);
			lItemAttr = pSItem->GetAttr(lAttrID);
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取道具属性成功\n");
		g_pCLogObj->Log("Get the Item Final attribute succeed \n");
#endif

		lua_pushnumber(pLS, lItemAttr);
		return 1;
	}

	return 0;
}

// 设置道具属性
// 道具对象；道具属性编号；属性值
// 返回值：１，成功。０，失败
inline int lua_SetItemFinalAttr(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置道具属性 SetItemFinalAttr\n");
	g_pCLogObj->Log("Set the Item Final attribute : SetItemFinalAttr\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		{
			long	lAttrID = (int)lua_tonumber(pLS, 2);
			short	sAttr = (short)lua_tonumber(pLS, 3);
			if (!pSItem->SetAttr(lAttrID, sAttr))
				bSuccess = false;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置道具属性成功\n");
		g_pCLogObj->Log("Set the Item Final attribute succeed\n");
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 增加道具属性
// 道具对象；道具属性编号；属性值
// 返回值：１，成功。０，失败
inline int lua_AddItemFinalAttr(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("增加道具属性 AddItemFinalAttr\n");
	g_pCLogObj->Log("Add the Item Final attribute : AddItemFinalAttr\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		{
			long	lAttrID = (int)lua_tonumber(pLS, 2);
			short	sAttr = (short)lua_tonumber(pLS, 3);
			if (!pSItem->AddAttr(lAttrID, sAttr))
				bSuccess = false;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("增加道具属性成功\n");
		g_pCLogObj->Log("Add the Item Final attribute succeed ");
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 复位道具属性
// 道具对象
// 返回值：１，成功。０，失败
inline int lua_ResetItemFinalAttr(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("增加道具属性 AddItemFinalAttr\n");
	g_pCLogObj->Log("Add the Item Final attribute : AddItemFinalAttr\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		if (!pSItem->InitAttr())
		{
			bSuccess = false;
			goto End;
		}

	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("增加道具属性成功\n");
		g_pCLogObj->Log("Add the Item Final attribute succeed ");
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

inline int lua_GetItemAttrRange(lua_State* pLS)
{
	bool	bSuccess = true;
	short sValue = 0;

	int nParaNum = lua_gettop(pLS); // 取得参数个数
	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		short sItemID = (short)lua_tonumber(pLS, 1);
		short sAttrID = (short)lua_tonumber(pLS, 2);
		short sType = (short)lua_tonumber(pLS, 3);

		CItemRecord* pCItemRec;
		pCItemRec = GetItemRecordInfo(sItemID);
		if (!pCItemRec)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		{
			bool bMax = (sType == 0) ? false : true;
			sValue = g_pCItemAttr[sItemID].GetAttr(sAttrID, bMax);
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("增加道具属性成功\n");
		g_pCLogObj->Log("Add the Item Final attribute succeed ");
#endif
	}

	lua_pushnumber(pLS, sValue);

	return 1;
}

// 取道具精炼参数
// 道具对象；参数类型（0，精炼等级。1，精炼内容）
// 返回值：精炼参数
inline int lua_GetItemForgeParam(lua_State *pLS)
{
	bool	bSuccess = true;
	long	lForgeP = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取道具精炼参数 GetItemForgeParam\n");
	g_pCLogObj->Log("Get Item forge parameter : GetItemForgeParam\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		{
			long	lType = (int)lua_tonumber(pLS, 2);
			if (lType == 0)
				lForgeP = pSItem->GetForgeLv();
			else
				lForgeP = pSItem->GetDBParam(enumITEMDBP_FORGE);
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("增加道具属性成功\n");
		g_pCLogObj->Log("Add the Item Final attribute succeed ");
#endif
	}

	lua_pushnumber(pLS, lForgeP);

	return 1;
}

// 设置道具精炼参数
// 道具对象；参数类型（0，精炼等级。1，精炼内容）；参数值
// 返回值：１，成功。０，失败
inline int lua_SetItemForgeParam(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置道具精炼参数 SetItemForgeParam\n");
	g_pCLogObj->Log("Set Item forge parameter : SetItemForgeParam\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		{
			long	lType = (int)lua_tonumber(pLS, 2);
			if (lType == 0)
			{
				pSItem->SetForgeLv((char)lua_tonumber(pLS, 3));
			}
			else
			{
				pSItem->SetDBParam(enumITEMDBP_FORGE, (unsigned __int64)lua_tonumber(pLS, 3));
			}

		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("增加道具属性成功\n");
		g_pCLogObj->Log("Add the Item Final attribute succeed ");
#endif
	}

	lua_pushnumber(pLS, 1);

	return 1;
}

// 增加装备道具的能量
// 参数：角色，外观位置，道具类型，增量
// 返回值：1 成功，0 失败
inline int lua_AddEquipEnergy(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("增加装备道具的能量 AddEquipEnergy\n");
	g_pCLogObj->Log("Add equip item energy : AddEquipEnergy\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 4)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

		if (!pCCha || !pCCha->GetPlayer())
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		if (pCCha->GetPlayer())
			pCCha = pCCha->GetPlayer()->GetMainCha();

		char	chPos = (char)lua_tonumber(pLS, 2);
		if (chPos < enumEQUIP_HEAD || chPos >= enumEQUIP_NUM)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t装备的位置%d错误，调用失败\n", chPos);
			g_pCLogObj->Log("\tequip position %d error,transfer failed\n", chPos);
#endif
			bSuccess = false;
			goto End;
		}
		{
			short	sItemType = (short)lua_tonumber(pLS, 3);
			short	sVal = (short)lua_tonumber(pLS, 4);

			SItemGrid* pEquip = &pCCha->m_SChaPart.SLink[chPos];
			if (pEquip->sID > 0)
			{
				if (GetItemRecordInfo(pEquip->sID)->sType == sItemType)
					pEquip->AddInstAttr(ITEMATTR_ENERGY, sVal);
			}
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("增加装备道具的能量成功\n");
		g_pCLogObj->Log("Add equip item energy succeed\n");
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 设置非自然复活信息
// 参数：源方角色，目标角色，复活等级，终端提示信息
// 返回值：无
inline int lua_SetRelive(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置非自然复活信息 SetRelive\n");
	g_pCLogObj->Log("Set nonnature relive message : SetRelive\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 4)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCSrcCha = (CCharacter*)lua_touserdata(pLS, 1);
		CCharacter* pCTarCha = (CCharacter*)lua_touserdata(pLS, 2);

		if (!pCSrcCha || !pCSrcCha->GetPlayer() || !pCTarCha || !pCTarCha->GetPlayer())
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		if (pCTarCha->GetChaRelive())
		{
			//pCSrcCha->SystemNotice("目标正在接受其他人的复活请求!");  
			pCSrcCha->SystemNotice(RES_STRING(GM_EXPAND_H_00007));
			bSuccess = false;
			goto End;
		}

		if (!pCTarCha->IsBoat()) // 船不能复活
		{
			pCTarCha->SetRelive(enumEPLAYER_RELIVE_ORIGIN, (int)lua_tonumber(pLS, 3), lua_tostring(pLS, 4));
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置非自然复活信息成功\n");
		g_pCLogObj->Log("Set nonnature relive message succeed\n");
#endif
	}

	return 0;
}

inline int lua_LuaPrint(lua_State *pLS)
{
#ifdef defPARSE_LOG
g_pCLogObj->Log(lua_tostring(pLS, 1));
#endif

return 0;
}

inline int lua_Stop(lua_State *pLS)
{
g_pGameApp->m_CTimerReset.Begin(1000);
g_pGameApp->m_ulLeftSec = (int)lua_tonumber(pLS, 1);

return 0;
}

// 全服通告
inline int lua_Notice(lua_State *pLS)
{
if (!lua_isstring(pLS, 1))
return 0;

const char	*cszNotiStr = lua_tostring(pLS, 1);
g_pGameApp->WorldNotice(cszNotiStr);

//if (strstr(cszNotiStr, "海盗广播"))
if (strstr(cszNotiStr, RES_STRING(GM_EXPAND_H_00102)))
if (g_cchLogMapEntry)
//LG("地图入口流程", "系统通告：%s\n", cszNotiStr);
LG("map_entrance_flow", "system notice : %s\n", cszNotiStr);

return 0;
}

inline int lua_GuildNotice(lua_State *pLS)
{
	if (!lua_isnumber(pLS, 1) || !lua_isstring(pLS, 2))
		return 0;

	DWORD guildID = (DWORD)lua_tonumber(pLS, 1);
	const char *cszNotiStr = lua_tostring(pLS, 2);
	g_pGameApp->GuildNotice(guildID, cszNotiStr);
	return 0;
}

//Add by sunny.sun20080804
inline int lua_ScrollNotice( lua_State* L )
{
int nParaNum = lua_gettop(L); // 取得参数个数
if(	nParaNum != 3 )
{
#ifdef defPARSE_LOG
		g_pCLogObj->Log("error pragram num!");
#endif
	return 0;
}
if (!lua_isstring(L, 1))
return 0;

int SetNum = (int)lua_tonumber(L, 2);
DWORD color = (DWORD)lua_tonumber(L, 3);
const char	*cszNotiStr = lua_tostring(L, 1);
g_pGameApp->ScrollNotice(cszNotiStr,SetNum, color);

//if (strstr(cszNotiStr, "海盗广播"))
if (strstr(cszNotiStr, RES_STRING(GM_EXPAND_H_00102)))
if (g_cchLogMapEntry)
//LG("地图入口流程", "系统通告：%s\n", cszNotiStr);
LG("map_entrance_flow", "system notice : %s\n", cszNotiStr);

return 0;

}

//Add by sunny.sun 20080821
inline int lua_GMNotice( lua_State *pLS )
{
	const char *gmNotice =	lua_tostring(pLS, 1);
	g_pGameApp->GMNotice( gmNotice );
	if (strstr(gmNotice, RES_STRING(GM_EXPAND_H_00102)))
	if (g_cchLogMapEntry)
	//LG("地图入口流程", "系统通告：%s\n", cszNotiStr);
	LG("map_entrance_flow", "system notice : %s\n", gmNotice);

	return 0;

}

// 玩家通告
inline int lua_ChaNotice(lua_State *pLS)
{
if (!lua_isstring(pLS, 1) || !lua_isstring(pLS, 2))
return 0;

const char	*cszChaName = lua_tostring(pLS, 1);
const char	*cszNotiStr = lua_tostring(pLS, 2);
g_pGameApp->ChaNotice(cszNotiStr, cszChaName);
//if (strstr(cszNotiStr, "海盗广播"))
if (strstr(cszNotiStr, RES_STRING(GM_EXPAND_H_00102)))
if (g_cchLogMapEntry)
//LG("地图入口流程", "系统通告：%s\n", cszNotiStr);
LG("map_entrance_flow", "system notice : %s\n", cszNotiStr);

return 0;
}

// 地图副本通告
// 参数：地图副本对象，通告内容
// 返回值：无
inline int lua_MapCopyNotice(lua_State *pLS)
{
if (!lua_islightuserdata(pLS, 1) || !lua_isstring(pLS, 2))
return 0;

SubMap	*pCMapCopy = (SubMap *)lua_touserdata(pLS, 1);
const char	*cszNotiStr = lua_tostring(pLS, 2);
pCMapCopy->Notice(cszNotiStr);

return 0;
}

// 地图副本通告
// 参数：地图对象，副本编号（０为所有副本），通告内容
// 返回值：无
inline int lua_MapCopyNotice2(lua_State *pLS)
{
if (!lua_islightuserdata(pLS, 1) || !lua_isnumber(pLS, 2) || !lua_isstring(pLS, 3))
return 0;

CMapRes	*pCMap = (CMapRes *)lua_touserdata(pLS, 1);
pCMap->CopyNotice(lua_tostring(pLS, 3), (short)lua_tonumber(pLS, 2) - 1);

return 0;
}

inline int lua_MapChaLight(lua_State *pLS)
{
	return 0;
}

// 设置角色掉料的属主
// 参数：掉料对象，属主对象
// 返回值：成功1，失败0
inline int lua_SetItemHost(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("设置角色掉料的属主 SetItemHost\n");
g_pCLogObj->Log("Set the character fall item host : SetItemHost\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 2)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCDropCha = (CCharacter*)lua_touserdata(pLS, 1);
	CCharacter* pCOwnCha = (CCharacter*)lua_touserdata(pLS, 2);
	if (!pCDropCha || !pCOwnCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed!\n");
#endif
		bSuccess = false;
		goto End;
	}
	pCDropCha->SetItemHostObj(pCOwnCha);
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置角色 %s 掉料的属主 %s 成功：%d\n", pCDropCha->GetLogName(), pCOwnCha->GetLogName());
	g_pCLogObj->Log("Set the character %s fall item host %s succeed :%d\n", pCDropCha->GetLogName(), pCOwnCha->GetLogName());
#endif
	lua_pushnumber(pLS, 1);
}
else
lua_pushnumber(pLS, 0);

return 1;
}

// 取得角色名称
// 参数：角色对象
// 返回值：角色名称
inline int lua_GetChaName(lua_State *pLS)
{
bool	bSuccess = true;
const char* ret = "";

#ifdef defPARSE_LOG
//g_pCLogObj->Log("取得角色名称 GetChaName\n");
g_pCLogObj->Log("Get the character Name : GetChaName\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}
	ret = pCCha->GetLogName();
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取得角色 %s 的名称成功：%d\n", pCCha->GetLogName());
	g_pCLogObj->Log("Get the character  %s 's Name succeed : %d\n", ret);
#endif

	lua_pushstring(pLS, ret);
	return 1;
}

return 0;
}

// 设置地图入口的控制时间
// 参数：地图指针，首次执行时间，后续执行间隔，执行后的消失间隔，执行后的地图关闭间隔
// 返回值：1，成功。0，失败
inline int lua_SetMapEntryTime(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置地图入口的控制时间 SetMapEntryTime\n");
	g_pCLogObj->Log("Set the map entrance Time : SetMapEntryTime\n");
#endif

	std::string	strList[5];
	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 5)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		auto pCMap = static_cast<CMapRes*>(lua_touserdata(pLS, 1));
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		// 首次开启时间
		struct tm	time_set, * time_get;
		time_t	timep;
		time(&timep);
		time_get = localtime(&timep);
		const char* szTime = (const char*)lua_tostring(pLS, 2);
		int n = Util_ResolveTextLine(szTime, strList, 5, '/');
		if (n != 5)
		{
			//MessageBox(0, szTime, "设置地图入口控制时间，其格式错误", MB_OK);
			MessageBox(0, szTime, RES_STRING(GM_EXPAND_H_00111), MB_OK);
			bSuccess = false;
			goto End;
		}
		time_set.tm_year = Str2Int(strList[0]) - 1900;
		time_set.tm_mon = Str2Int(strList[1]) - 1;
		time_set.tm_mday = Str2Int(strList[2]);
		time_set.tm_hour = Str2Int(strList[3]);
		time_set.tm_min = Str2Int(strList[4]);
		time_set.tm_sec = 0;
		time_set.tm_isdst = time_get->tm_isdst;
		pCMap->m_tEntryFirstTm = mktime(&time_set);

		// 后续执行间隔
		szTime = (const char*)lua_tostring(pLS, 3);
		n = Util_ResolveTextLine(szTime, strList, 4, '/');
		if (n != 3)
		{
			//MessageBox(0, szTime, "设置地图入口控制时间，其格式错误", MB_OK);
			MessageBox(0, szTime, RES_STRING(GM_EXPAND_H_00111), MB_OK);
			bSuccess = false;
			goto End;
		}
		pCMap->m_tEntryTmDis = ((Str2Int(strList[0]) * 24 + Str2Int(strList[1])) * 60 + Str2Int(strList[2])) * 60;

		// 执行后的消失间隔
		szTime = (const char*)lua_tostring(pLS, 4);
		n = Util_ResolveTextLine(szTime, strList, 4, '/');
		if (n != 3)
		{
			//MessageBox(0, szTime, "设置地图入口控制时间，其格式错误", MB_OK);
			MessageBox(0, szTime, RES_STRING(GM_EXPAND_H_00111), MB_OK);
			bSuccess = false;
			goto End;
		}
		pCMap->m_tEntryOutTmDis = ((Str2Int(strList[0]) * 24 + Str2Int(strList[1])) * 60 + Str2Int(strList[2])) * 60;

		// 执行后的地图关闭间隔
		szTime = (const char*)lua_tostring(pLS, 5);
		n = Util_ResolveTextLine(szTime, strList, 4, '/');
		if (n != 3)
		{
			//MessageBox(0, szTime, "设置地图入口控制时间，其格式错误", MB_OK);
			MessageBox(0, szTime, RES_STRING(GM_EXPAND_H_00111), MB_OK);
			bSuccess = false;
			goto End;
		}
		pCMap->m_tMapClsTmDis = ((Str2Int(strList[0]) * 24 + Str2Int(strList[1])) * 60 + Str2Int(strList[2])) * 60;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图(%s)入口的控制时间成功：\n", pCMap->GetName());
		g_pCLogObj->Log("Set the map %s entrance Time succeed :\n", pCMap->GetName());
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 设置地图是否保存位置
// 参数：地图指针，是否保存位置
// 返回值：1，成功。0，失败
inline int lua_MapCanSavePos(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置地图是否保存位置 MapCanSavePos\n");
	g_pCLogObj->Log("Set the map whether save position : MapCanSavePos\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		auto pCMap = static_cast<CMapRes*>(lua_touserdata(pLS, 1));
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->SetCanSavePos(lua_tonumber(pLS, 2) != 0 ? true : false);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图(%s)是否保存位置成功：\n", pCMap->GetName());
		g_pCLogObj->Log("Set the map %s whether save position succeed :\n", pCMap->GetName());
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 设置地图是否要遣返死亡角色
// 参数：地图指针，是否遣返
// 返回值：无
inline int lua_RepatriateDie(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置地图是否要遣返死亡角色 RepatriateDie\n");
	g_pCLogObj->Log("Set the map whether Repatriate the die character : RepatriateDie\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapRes* pCMap = (CMapRes*)lua_touserdata(pLS, 1);
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->SetRepatriateDie(lua_tonumber(pLS, 2) != 0 ? true : false);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图(%s)是否要遣返死亡角色成功：\n", pCMap->GetName());
		g_pCLogObj->Log("Set the map %s whether Repatriate the die character succeed : \n", pCMap->GetName());
#endif
	}

	return 0;
}

// 设置地图是否PK
// 参数：地图指针，是否PK
// 返回值：1，成功。0，失败
inline int lua_MapCanPK(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置地图是否PK MapCanPK\n");
	g_pCLogObj->Log("Set the map whether PK : MapCanPK\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapRes* pCMap = (CMapRes*)lua_touserdata(pLS, 1);
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->SetCanPK(lua_tonumber(pLS, 2) != 0 ? true : false);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图(%s)是否PK成功：\n", pCMap->GetName());
		g_pCLogObj->Log("Set the map(%s) whether PK succeed : \n", pCMap->GetName());
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 设置地图是否可以操作队伍
// 参数：地图指针，是否可操作（1，可操作。0，不可操作）
// 返回值：1，成功。0，失败
inline int lua_MapCanTeam(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置地图是否可以操作队伍 MapCanTeam\n");
	g_pCLogObj->Log("Set the map whether can work Team : MapCanTeam\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapRes* pCMap = (CMapRes*)lua_touserdata(pLS, 1);
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->SetCanTeam(lua_tonumber(pLS, 2) != 0 ? true : false);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图(%s)是否PK成功：\n", pCMap->GetName());
		g_pCLogObj->Log("Set the map(%s) whether PK succeed : \n", pCMap->GetName());
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 地图是否可以摆摊。
// 参数：地图指针，是否可操作（1，可操作。0，不可操作）
// 返回值：1，成功。0，失败
inline int lua_MapCanStall(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置可否摆摊， MapCanTeam\n");
	g_pCLogObj->Log("Set the map whether can stall: MapCanTeam\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapRes* pCMap = (CMapRes*)lua_touserdata(pLS, 1);
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->SetCanStall(lua_tonumber(pLS, 2) != 0 ? true : false);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图(%s)是否PK成功：\n", pCMap->GetName());
		g_pCLogObj->Log("Set the map(%s) whether PK succeed : \n", pCMap->GetName());
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

inline int lua_MapCanGuild(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置可否退会， MapCanTeam\n");
	g_pCLogObj->Log("Set whether can quit Team : MapCanTeam\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}

	{
		CMapRes* pCMap = (CMapRes*)lua_touserdata(pLS, 1);
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->SetCanGuild(lua_tonumber(pLS, 2) != 0 ? true : false);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图(%s)是否退会成功：\n", pCMap->GetName());
		g_pCLogObj->Log("Set the map(%s)whether quit Team succeed :\n ", pCMap->GetName());
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

//玩家可以杀死自己的召唤怪
//参数：玩家角色指针，怪指针
//返回值：1，成功。0，失败
inline int lua_KillMyMonster(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("玩家杀死自己的召唤怪， KillMyMonster\n");
	g_pCLogObj->Log("the player killed the Monster by call up ：KillMyMonster\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCha = (CCharacter*)lua_touserdata(pLS, 1);
		CCharacter* pChaMonster = (CCharacter*)lua_touserdata(pLS, 2);
		if (!pCha || !pChaMonster)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
			g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
			bSuccess = false;
			goto End;
		}

		pChaMonster->Free();
		pChaMonster = 0;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("玩家(%s)杀死召唤怪(%s)成功\n", pCha ->GetName(),pChaMonster ->GetName());
		g_pCLogObj->Log("the player(%s) killed the Monster(%s) by call up succeed\n", pCha ->GetName(),pChaMonster ->GetName());
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;

}

//根据怪指针杀死特定的召唤怪
//参数：地图指针，怪名称
//返回值：1，成功。0，失败
inline int lua_KillMonsterInMapByName(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("杀死特定的召唤怪， KillMonsterInMapByName\n");
	g_pCLogObj->Log("killed the specifically monster by call up :KillMonsterInMapByName\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SubMap* pSubmap = (SubMap*)lua_touserdata(pLS, 1);
		const char* pMonstername = lua_tostring(pLS, 2);
		if (!pSubmap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图不存在，调用失败\n");
			g_pCLogObj->Log("the map is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		if (!pMonstername)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t怪物名称不存在，调用失败\n");
			g_pCLogObj->Log("\tthe Monster name is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		pSubmap->ClearAllMonsterByName(pMonstername);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("杀死特定的召唤怪(%s)成功\n", pChaMonster ->GetName());
		g_pCLogObj->Log("killed the specifically monster (%s)by call up succeed\n", pChaMonster ->GetName());
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;

}

// 设置地图副本数目
// 参数：地图指针，副本数
// 返回值：1，成功。0，失败
inline int lua_MapCopyNum(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置地图副本数目PK MapCopyNum\n");
	g_pCLogObj->Log("Set the map copy numbers PK : MapCopyNum\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapRes* pCMap = (CMapRes*)lua_touserdata(pLS, 1);
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图资源对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map resource object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->SetCopyNum((short)lua_tonumber(pLS, 2));
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图(%s)副本数目成功：\n", pCMap->GetName());
		g_pCLogObj->Log("Set the map(%s)copy number succeed :\n", pCMap->GetName());
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 设置地图副本的开始类型
// 参数：地图指针，开始类型（1，立即开始。2，有玩家时开始。3，手动开始。默认值为2）
// 返回值：1，成功。0，失败
inline int lua_MapCopyStartType(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置地图副本数目PK MapCopyNum\n");
	g_pCLogObj->Log("Set the map copy numbers PK : MapCopyNum\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapRes* pCMap = (CMapRes*)lua_touserdata(pLS, 1);
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图资源对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map resource object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->SetCopyStartType((char)lua_tonumber(pLS, 2));
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图(%s)副本数目成功：\n", pCMap->GetName());
		g_pCLogObj->Log("Set the map(%s)copy number succeed : \n", pCMap->GetName());
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 设置地图类型
// 参数：地图对象，类型（1，普通地图。2，公会战地图。3，队伍战地图）
// 返回值：1，成功。0，失败
inline int lua_MapType(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置地图类型 MapType\n");
	g_pCLogObj->Log("Set the map Type : MapType\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapRes* pCMap = (CMapRes*)lua_touserdata(pLS, 1);
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->SetType((char)lua_tonumber(pLS, 2));
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图类型成功：\n");
		g_pCLogObj->Log("Set the map Type succeed : \n");
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 设置单个地图副本的玩家数目限制
// 参数：入口对象，玩家数
// 返回值：1，成功。0，失败
inline int lua_SingleMapCopyPlyNum(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置单个地图副本的玩家数目限制 SingleMapCopyPlyNum\n");
	g_pCLogObj->Log("Set the single map copy players number : SingleMapCopyPlyNum\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapRes* pCMap = (CMapRes*)lua_touserdata(pLS, 1);
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图资源对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map resource object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->SetCopyPlyNum((short)lua_tonumber(pLS, 2));
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置单个地图副本的玩家数目限制(%d)成功：\n", (int)lua_tonumber(pLS, 2));
		g_pCLogObj->Log("Set the single map copy players number(%d) succeed :\n" , (int)lua_tonumber(pLS, 2));
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 设置地图入口所在的地图名
// 参数：地图指针，地图名称
// 返回值：1，成功。0，失败
inline int lua_SetMapEntryMapName(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置地图入口位置 SetMapEntryMapName\n");
	g_pCLogObj->Log("Set the map entrance position : SetMapEntryMapName\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapRes* pCMap = (CMapRes*)lua_touserdata(pLS, 1);
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图对象不存在，调用失败\n");
			g_pCLogObj->Log("\tthe map object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->SetEntryMapName((const char*)lua_tostring(pLS, 2));
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图(%s)入口位置(%s, [%d, %d])成功：\n", pCMap->GetName(), pszName, Pos.x, Pos.y);
		g_pCLogObj->Log("Set the map(%s) entrance position(%s,[%d, %d]) succeed : \n", pCMap->GetName(), pszName, Pos.x, Pos.y);
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 设置地图入口实体的编号
// 参数：入口对象，实体编号，事件编号
// 返回值：1，成功。0，失败
inline int lua_SetMapEntryEntiID(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置地图入口实体的编号 SetMapEntryEntiID\n");
	g_pCLogObj->Log("Set the map entrance instance ID : SetMapEntryEntiID\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CDynMapEntryCell* pEntry = (CDynMapEntryCell*)lua_touserdata(pLS, 1);
		if (!pEntry)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t入口对象不存在，调用失败\n");
			g_pCLogObj->Log("\t entrance object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pEntry->SetEntiID((int)lua_tonumber(pLS, 2));
		pEntry->SetEventID((int)lua_tonumber(pLS, 3));
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("地图入口实体的编号(%d)成功：\n", (int)lua_tonumber(pLS, 2));
		g_pCLogObj->Log("Set the map entrance instance ID(%d) succeed : \n", (int)lua_tonumber(pLS, 2));
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 取地图入口的位置信息
// 参数：入口对象
// 返回值：地图名，坐标，目标地图名
inline int lua_GetMapEntryPosInfo(lua_State *pLS)
{
	bool	bSuccess = true;
	const char* pMapN = "", * pTMapN = "";
	long lPosX = 0, lPosY = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取地图入口的位置信息 GetMapEntryPosInfo\n");
	g_pCLogObj->Log("Get the map entrance position info : GetMapEntryPosInfo\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
		//LG("入口err", "\t参数个数[%d]非法，调用失败\n", nParaNum);
		LG("entry error", "\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
		bSuccess = false;
		goto End;
	}
	{
		CDynMapEntryCell* pEntry = (CDynMapEntryCell*)lua_touserdata(pLS, 1);
		if (!pEntry)
		{
			//LG("入口err", "\t入口对象不存在，调用失败\n"); 
			LG("entry error", "\t entrance object is inexistence,transfer failed\n");
			bSuccess = false;
			goto End;
		}

		pEntry->GetPosInfo(&pMapN, &lPosX, &lPosY, &pTMapN);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图入口的限制单位数(%d)成功：\n", (int)lua_tonumber(pLS, 2));
		g_pCLogObj->Log("Set the restrict number (%d)of map entrance succeed :\n", (int)lua_tonumber(pLS, 2));
#endif
		lua_pushstring(pLS, pMapN);
		lua_pushnumber(pLS, lPosX / 100);
		lua_pushnumber(pLS, lPosY / 100);
		lua_pushstring(pLS, pTMapN);
		return 4;
	}
	return 0;
}

// 设置地图入口事件名
// 参数：入口对象，事件名
// 返回值：1，成功。0，失败
inline int lua_SetMapEntryEventName(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置地图入口实体的编号 SetMapEntryEntiID\n");
	g_pCLogObj->Log("Set the map entrance instance ID : SetMapEntryEntiID\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}

	{
		CDynMapEntryCell* pEntry = (CDynMapEntryCell*)lua_touserdata(pLS, 1);
		if (!pEntry)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t入口对象不存在，调用失败\n");
			g_pCLogObj->Log("\t entrance object is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pEntry->SetEventName(lua_tostring(pLS, 2));
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置地图入口事件名成功：\n");
		g_pCLogObj->Log("successful set the event of entry of the map ：\n");
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 唤起地图入口
// 参数：地图名
// 返回值：1，成功。0，失败
inline int lua_CallMapEntry(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("唤起地图入口 CallMapEntry\n");
	g_pCLogObj->Log("Call the map entrance : CallMapEntry\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}

	{
		const char* szMapN = (const char*)lua_tostring(pLS, 1);
		CMapRes* pCMap = g_pGameApp->FindMapByName(szMapN);
		if (!pCMap)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t地图不存在，调用失败\n");
			g_pCLogObj->Log("the map is inexistence,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		pCMap->CreateEntry();
	}

End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("唤起地图入口成功：\n");
		g_pCLogObj->Log("arouse the map entry succeed：\n");
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 取角色分边编号
// 参数：角色对象
// 返回值：分边编号
inline int lua_GetChaSideID(lua_State *pLS)
{
bool	bSuccess = true;
int		nSideID = 0;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("取角色分边编号 GetChaSideID\n");
g_pCLogObj->Log("Get character side ID : GetChaSideID\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}

{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	nSideID = pCCha->GetSideID();

}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	char	szPrint[256] = "";
	//sprintf(szPrint, "取角色%s, 分边编号 %u 成功。值：%d\n", pCCha->GetLogName(), nSideID);
	sprintf(szPrint, RES_STRING(GM_EXPAND_H_00144), pCCha->GetLogName(), nSideID);
	g_pCLogObj->Log(szPrint);
	g_pCLogObj->Log("\n");
#endif

	lua_pushnumber(pLS, nSideID);
	return 1;
}

return 0;
}

// 设置角色分边编号
// 参数：角色对象，分边编号
// 返回值：１，成功。０，失败
inline int lua_SetChaSideID(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("设置角色分边编号 SetChaSideID\n");
g_pCLogObj->Log("Set character side ID : SetChaSideID\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 2)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	long lSideID = (long)lua_tonumber(pLS, 2);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	pCCha->SetSideID(lSideID);
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	char	szPrint[256] = "";
	//sprintf(szPrint, "设置角色%s, 分边编号 %u 成功\n", pCCha->GetLogName(), lSideID);
	sprintf(szPrint, RES_STRING(GM_EXPAND_H_00145), pCCha->GetLogName(), lSideID);
	g_pCLogObj->Log(szPrint);
	g_pCLogObj->Log("\n");
#endif
	lua_pushnumber(pLS, 1);
}
else
lua_pushnumber(pLS, 0);

return 1;
}

// 取角色公会编号
// 参数：角色对象
// 返回值：公会编号（０为无效公会）
inline int lua_GetChaGuildID(lua_State *pLS)
{
bool	bSuccess = true;
int		nGuildID = 0;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("取角色公会编号 GetChaGuildID\n");
g_pCLogObj->Log("Get character Guild ID : GetChaGuildID\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	nGuildID = pCCha->GetValidGuildID();
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	char	szPrint[256] = "";
	//sprintf(szPrint, "取角色%s, 公会编号 %u 成功。值：%d\n", pCCha->GetLogName(), nGuildID);
	sprintf(szPrint, RES_STRING(GM_EXPAND_H_00147), pCCha->GetLogName(), nGuildID);
	g_pCLogObj->Log(szPrint);
	g_pCLogObj->Log("\n");
#endif
}

lua_pushnumber(pLS, nGuildID);
return 1;
}

// 取角色队伍编号
// 参数：角色对象
// 返回值：队伍编号（０为无效队伍）
inline int lua_GetChaTeamID(lua_State *pLS)
{
bool	bSuccess = true;
int		nTeamID = 0;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("取角色队伍编号 GetChaTeamID\n");
g_pCLogObj->Log("Get character Team ID : GetChaTeamID\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	nTeamID = pCCha->GetTeamID();
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	char	szPrint[256] = "";
	//sprintf(szPrint, "取角色%s, 队伍编号 %u 成功。值：%d\n", pCCha->GetLogName(), nTeamID);
	sprintf(szPrint, RES_STRING(GM_EXPAND_H_00149), pCCha->GetLogName(), nTeamID);
	g_pCLogObj->Log(szPrint);
	g_pCLogObj->Log("\n");
#endif
}

lua_pushnumber(pLS, nTeamID);
return 1;
}

// 判断角色是否在PK状态
// 参数：角色对象
// 返回角色PK状态：1，可以PK。0，不可以。
inline int lua_CheckChaPKState(lua_State *pLS)
{
bool	bSuccess = true;
int		nPKState = 0;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("判断角色是否在PK状态 CheckChaPKState\n");
g_pCLogObj->Log("check character whether in PK state : CheckChaPKState\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	nPKState = pCCha->CanPK() ? 1 : 0;
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	char	szPrint[256] = "";
	//sprintf(szPrint, "判断角色是否在PK状态成功\n");
	sprintf(szPrint, RES_STRING(GM_EXPAND_H_00151));
	g_pCLogObj->Log(szPrint);
	g_pCLogObj->Log("\n");
#endif
}

lua_pushnumber(pLS, nPKState);
return 1;
}

// 取公会名
// 参数：公会ID
// 返回：公会名
inline int lua_GetGuildName(lua_State *pLS)
{
bool	bSuccess = true;
std::string	strGuildName;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("取公会名 GetGuildName\n");
g_pCLogObj->Log("Get the Guild name : GetGuildName\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	long	lGuildID = (long)lua_tonumber(pLS, 1);
	game_db.GetGuildName(lGuildID, strGuildName);
}

End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取公会名成功\n");
	g_pCLogObj->Log("Get guild name succeed\n");
#endif

	lua_pushstring(pLS, strGuildName.c_str());
	return 1;
}

return 0;
}

// 关闭地图入口
// 参数：地图名称
// 返回值：1，成功关闭。0，关闭失败
inline int lua_CloseMapEntry(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("关闭地图入口 CloseMapEntry\n");
	g_pCLogObj->Log("Close the map entrance : CloseMapEntry\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapRes* pCMap = g_pGameApp->FindMapByName(lua_tostring(pLS, 1));

		if (!pCMap || !pCMap->CloseEntry())
		{
			bSuccess = false;
			goto End;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("关闭地图入口成功\n");
		g_pCLogObj->Log("Close map entry succeed\n");
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 关闭地图副本
// 参数：地图名称，副本编号
// 返回值：1，成功关闭。0，关闭失败
inline int lua_CloseMapCopy(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("关闭地图副本 CloseMapCopy\n");
	g_pCLogObj->Log("Close the map copy : CloseMapCopy\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2 && nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		short	sCopyNO = -1;
		if (nParaNum == 2)
			sCopyNO = (short)lua_tonumber(pLS, 2) - 1;
		if (sCopyNO < 0)
			sCopyNO = 0;
		CMapRes* pCMap = g_pGameApp->FindMapByName(lua_tostring(pLS, 1));
		if (!pCMap || !pCMap->ReleaseCopy(sCopyNO))
		{
			bSuccess = false;
			goto End;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("关闭地图副本成功\n");
		g_pCLogObj->Log("Close map copy succeed\n");
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 设置角色座右铭
// 参数：角色对象，座右铭
// 返回值：１，成功。０，失败
inline int lua_SetChaMotto(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("设置角色分边编号 SetChaSideID\n");
g_pCLogObj->Log("Set character side ID : SetChaSideID\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 2)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	const char* cszMotto = lua_tostring(pLS, 2);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	pCCha->SetMotto(cszMotto);
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	char	szPrint[256] = "";
	//sprintf(szPrint, "设置角色%s, 座右铭 %s 成功\n", pCCha->GetLogName(), cszMotto);
	sprintf(szPrint, RES_STRING(GM_EXPAND_H_00152), pCCha->GetLogName(), cszMotto);
	g_pCLogObj->Log(szPrint);
	g_pCLogObj->Log("\n");
#endif
	lua_pushnumber(pLS, 1);
}
else
lua_pushnumber(pLS, 0);

return 1;
}

// 角色是否在陆地区域
// 参数：角色对象
// 返回值：１，陆地。０，海洋
inline int lua_IsChaInLand(lua_State *pLS)
{
bool	bSuccess = true;
char	chIsLand = 0;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("设置角色分边编号 SetChaSideID\n");
g_pCLogObj->Log("Set character Side ID : SetChaSideID\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);

	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}
	if (!pCCha->GetSubMap())
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色地图不存在，调用失败");
		g_pCLogObj->Log("\tthe character map is inexistence,transfer failed");
#endif
		bSuccess = false;
		goto End;
	}

	short	sAreaAttr = pCCha->GetSubMap()->GetAreaAttr(pCCha->GetPos());
	chIsLand = g_IsLand(sAreaAttr) ? 1 : 0;
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	char	szPrint[256] = "";
	//sprintf(szPrint, "设置角色%s, 座右铭 %s 成功\n", pCCha->GetLogName(), cszMotto);
	sprintf(szPrint, RES_STRING(GM_EXPAND_H_00152), pCCha->GetLogName(), cszMotto);
	g_pCLogObj->Log(szPrint);
	g_pCLogObj->Log("\n");
#endif
	lua_pushnumber(pLS, chIsLand);
	return 1;
}
else
return 0;
}

// 设置队伍挑战地图名
// 参数：地图名
// 返回值：1，成功。0，失败
inline int lua_SetTeamFightMapName(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置队伍挑战地图名 SetTeamFightMapName\n");
	g_pCLogObj->Log("Set the Team fight map name : SetTeamFightMapName\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		const char* cszMapName = lua_tostring(pLS, 1);

		if (!cszMapName)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t非法地图名，调用失败");
			g_pCLogObj->Log("\tUnusable map name，transfer failed");
#endif
			bSuccess = false;
			goto End;
		}

		g_SetTeamFightMapName(cszMapName);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		char	szPrint[256] = "";
		//sprintf(szPrint, "设置队伍挑战地图名 %s 成功\n", cszMapName);
		sprintf(szPrint, RES_STRING(GM_EXPAND_H_00155), cszMapName);
		g_pCLogObj->Log(szPrint);
		g_pCLogObj->Log("\n");
#endif
		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 取副本的参数
// 参数：副本管理对象，参数编号
// 返回值：参数值
inline int lua_GetMapCopyParam(lua_State *pLS)
{
	bool	bSuccess = true;
	short ret = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取副本的参数 GetMapCopyParam\n");
	g_pCLogObj->Log("Get the map copy parameter :  GetMapCopyParam\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapEntryCopyCell* pCCpyMgr = (CMapEntryCopyCell*)lua_touserdata(pLS, 1);
		if (!pCCpyMgr)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t副本管理对象不存在，调用失败\n");
			g_pCLogObj->Log("\tMapcopy manage object inexistent，transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		ret = pCCpyMgr->GetParam((char)lua_tonumber(pLS, 2) - 1);
	}

End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取副本的参数成功：\n");
		g_pCLogObj->Log("Get mapcopy's param succeed：\n");
#endif

		lua_pushnumber(pLS, ret);
		return 1;
	}

	return 0;
}

// 取副本的参数
// 参数：副本对象，参数编号
// 返回值：参数值
inline int lua_GetMapCopyParam2(lua_State *pLS)
{
	bool	bSuccess = true;
	long ret = 0;
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取副本的参数 GetMapCopyParam2\n");
	g_pCLogObj->Log("Get the map copy param2 : GetMapCopyParam2\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SubMap* pCMapCpy = (SubMap*)lua_touserdata(pLS, 1);
		if (!pCMapCpy)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t副本对象不存在，调用失败\n");
			g_pCLogObj->Log("\Mapcopy object inexistent，transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		ret = pCMapCpy->GetInfoParam((char)lua_tonumber(pLS, 2) - 1);
	}

End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取副本的参数成功：\n");
		g_pCLogObj->Log("Get mapcopy's param succeed：\n");
#endif

		lua_pushnumber(pLS, ret);
		return 1;
	}

	return 0;
}

// 取副本编号
// 参数：副本管理对象
// 返回值：编号（小于等于0为无效值）
inline int lua_GetMapCopyID(lua_State *pLS)
{
	bool	bSuccess = true;
	long ret = 0;
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取副本编号 GetMapCopyID\n");
	g_pCLogObj->Log("Get the map copy ID :  GetMapCopyID\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapEntryCopyCell* pCCpyMgr = (CMapEntryCopyCell*)lua_touserdata(pLS, 1);
		if (!pCCpyMgr)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t副本管理对象不存在，调用失败\n");
			g_pCLogObj->Log("\tMapcopy manage object inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		ret = pCCpyMgr->GetPosID() + 1;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取副本编号成功：\n");
		g_pCLogObj->Log("Get mapcopy ID succeed：\n");
#endif
	}
	lua_pushnumber(pLS, ret);

	return 1;
}

// 取副本编号
// 参数：副本对象
// 返回值：编号（小于等于0为无效值）
inline int lua_GetMapCopyID2(lua_State *pLS)
{
	bool	bSuccess = true;
	short ret = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取副本编号 GetMapCopyID2\n");
	g_pCLogObj->Log(" GetMapCopyID2\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SubMap* pCMapCopy = (SubMap*)lua_touserdata(pLS, 1);
		if (!pCMapCopy)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t副本对象不存在，调用失败\n");
			g_pCLogObj->Log("\tMapcopy object inexistent，transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		ret = pCMapCopy->GetCopyNO() + 1;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取副本编号成功：\n");
		g_pCLogObj->Log("Get mapcopy ID succeed：\n");
#endif
	}

	lua_pushnumber(pLS, ret);

	return 1;
}

// 设置副本的参数
// 参数：副本管理对象，参数索引，参数值
// 返回值：1，成功。0，失败。
inline int lua_SetMapCopyParam(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置副本的参数 SetMapCopyParam\n");
	g_pCLogObj->Log("SetMapCopyParam\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CMapEntryCopyCell* pCCpyMgr = (CMapEntryCopyCell*)lua_touserdata(pLS, 1);
		if (!pCCpyMgr)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t副本管理对象不存在，调用失败\n");
			g_pCLogObj->Log("\tMapcopy manage object inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		if (!pCCpyMgr->SetParam((char)lua_tonumber(pLS, 2) - 1, (long)lua_tonumber(pLS, 3)))
		{
			bSuccess = false;
			goto End;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置副本的参数成功：\n");
		g_pCLogObj->Log("Set mapcopy's param succeed：\n");
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 设置副本的参数
// 参数：副本对象，参数索引，参数值
// 返回值：1，成功。0，失败。
inline int lua_SetMapCopyParam2(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置副本的参数 SetMapCopyParam\n");
	g_pCLogObj->Log("SetMapCopyParam\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}

	{
		SubMap* pCMapCpy = (SubMap*)lua_touserdata(pLS, 1);
		if (!pCMapCpy)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t副本对象不存在，调用失败\n");
			g_pCLogObj->Log("\tmapcopy object inexistent，transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		if (!pCMapCpy->SetInfoParam((char)lua_tonumber(pLS, 2) - 1, (long)lua_tonumber(pLS, 3)))
		{
			bSuccess = false;
			goto End;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置副本的参数成功：\n");
		g_pCLogObj->Log("Set mapcopy's param succeed：\n");
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 取入口副本对象
// 参数：入口对象，副本编号（小于等于0，为取新的空闲副本。大于0，为取指定的副本，若不存在，则新增）
// 返回值：入口副本对象
inline int lua_GetMapEntryCopyObj(lua_State *pLS)
{
	bool	bSuccess = true;
	CMapEntryCopyCell	*pCCopyCell = NULL;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取入口副本对象 GetMapEntryCopyObj\n");
	g_pCLogObj->Log("GetMapEntryCopyObj\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CDynMapEntryCell* pCMapEntry = (CDynMapEntryCell*)lua_touserdata(pLS, 1);
		if (!pCMapEntry)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t副本管理对象不存在，调用失败\n");
			g_pCLogObj->Log("\mapcopy managed object inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		short	sCopyID = (short)lua_tonumber(pLS, 2) - 1;
		pCCopyCell = pCMapEntry->GetCopy(sCopyID);
		if (!pCCopyCell)
		{
			CMapEntryCopyCell	CCopyCell(pCMapEntry->GetCopyPlyNum());
			CCopyCell.SetPosID(sCopyID);
			pCCopyCell = pCMapEntry->AddCopy(&CCopyCell);
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取副本编号成功：\n");
		g_pCLogObj->Log("Get mapcopy ID succeed：\n");
#endif
	}

	if (pCCopyCell)
	{
		lua_pushlightuserdata(pLS, pCCopyCell);
		return 1;
	}

	return 0;
}

// 取副本的玩家数目
// 参数：副本对象
// 返回值：玩家数目
inline int lua_GetMapCopyPlayerNum(lua_State *pLS)
{
	bool	bSuccess = true;
	long ret = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取副本的玩家数目 GetMapCopyPlayerNum\n");
	g_pCLogObj->Log("GetMapCopyPlayerNum\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SubMap* pCMapCopy = (SubMap*)lua_touserdata(pLS, 1);
		if (!pCMapCopy)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t副本对象不存在，调用失败\n");
			g_pCLogObj->Log("\tmapcopy object inexistent, transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		ret = pCMapCopy->GetPlayerNum();
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取副本编号成功：\n");
		g_pCLogObj->Log("Get mapcopy ID succeed：\n");
#endif
	}
	lua_pushnumber(pLS, ret);

	return 1;
}

// 开始取副本的玩家角色
// 参数：副本对象
// 返回值：无
inline int lua_BeginGetMapCopyPlayerCha(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取副本的玩家数目 GetMapCopyPlayerNum\n");
	g_pCLogObj->Log("GetMapCopyPlayerNum\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SubMap* pCMapCopy = (SubMap*)lua_touserdata(pLS, 1);
		if (!pCMapCopy)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t副本对象不存在，调用失败\n");
			g_pCLogObj->Log("\tmapcopy object inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		pCMapCopy->BeginGetPlyCha();
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取副本编号成功：\n");
		g_pCLogObj->Log("Get mapcopy ID succeed：\n");
#endif
	}

	return 0;
}

// 取副本的玩家角色
// 参数：副本对象
// 返回值：玩家角色
inline int lua_GetMapCopyNextPlayerCha(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取副本的玩家数目 GetMapCopyPlayerNum\n");
	g_pCLogObj->Log("GetMapCopyPlayerNum\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数
	CCharacter	*pCCha = 0;

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SubMap* pCMapCopy = (SubMap*)lua_touserdata(pLS, 1);
		if (!pCMapCopy)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t副本对象不存在，调用失败\n");
			g_pCLogObj->Log("\tmapcopy object inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		pCCha = pCMapCopy->GetNextPlyCha();
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取副本编号成功：\n");
		g_pCLogObj->Log("Get mapcopy IDsucceed：\n");
#endif
	}
	if (pCCha)
	{
		lua_pushlightuserdata(pLS, pCCha);
		return 1;
	}
	return 0;
}

// 取得角色的地图类型
// 参数：角色对象
// 返回值：地图类型
inline int lua_GetChaMapType(lua_State *pLS)
{
bool	bSuccess = true;
char chMapType = enumMAPTYPE_NORMAL;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("取得角色名称 GetChaName\n");
g_pCLogObj->Log("GetChaName\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	CMapRes* pCMap = nullptr;
	if (pCCha->GetSubMap())
	{
		pCMap = pCCha->GetSubMap()->GetMapRes();
	}
	if (!pCMap)
	{
		pCMap = g_pGameApp->FindMapByName(pCCha->GetBirthMap(), true);
	}
	if (pCMap)
	{
		chMapType = pCMap->GetType();
	}
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取得角色 %s 的名称成功：%d\n", pCCha->GetLogName());
	g_pCLogObj->Log("Get role %s name succeed：%d\n", pCCha->GetLogName());
#endif

	lua_pushnumber(pLS, chMapType);
	return 1;
}

return 0;

}

// 设置角色背包的改动标志
// 参数：角色对象，改动（1，不管实际改动与否均设为全部被改动。0，仅设置实际改动的标志）
// 返回值：无
inline int lua_SetChaKitbagChange(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("设置角色背包的改动标志 SetChaKitbagChange\n");
g_pCLogObj->Log("SetChaKitbagChange\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 2)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	pCCha->m_CKitbag.SetChangeFlag((int)lua_tonumber(pLS, 2) != 0 ? true : false);
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置角色背包的改动标志成功：%d\n", pCCha->GetLogName());
	g_pCLogObj->Log("SetChaKitbagChange succeed：%d\n", pCCha->GetLogName());
#endif
}

return 0;
}

// 同步角色背包
// 参数：角色对象，同步类型
// 返回值：无
inline int lua_SynChaKitbag(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("同步角色背包 SynChaKitbag\n");
g_pCLogObj->Log("SynChaKitbag\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 2)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	pCCha->SynKitbagNew((char)lua_tonumber(pLS, 2));
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("同步角色背包成功：%d\n", pCCha->GetLogName());
	g_pCLogObj->Log("SynChaKitbag succeed：%d\n", pCCha->GetLogName());
#endif
}

return 0;
}

// 取得角色当前地图的探索度
// 参数：角色对象
// 返回值：地图探索度
inline int lua_GetChaMapOpenScale(lua_State *pLS)
{
bool	bSuccess = true;
float ret = 0.0;
#ifdef defPARSE_LOG
//g_pCLogObj->Log("取得角色当前地图的探索度 GetChaMapOpenScale\n");
g_pCLogObj->Log("GetChaMapOpenScale\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 1)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
	if (!pCCha)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
		g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
		bSuccess = false;
		goto End;
	}

	if (!pCCha->IsPlayerCha() || !pCCha->GetSubMap())
	{
		bSuccess = false;
		goto End;
	}
	ret = pCCha->GetPlayer()->GetMapMaskOpenScale(pCCha->GetSubMap()->GetName());
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取得角色当前地图的探索度成功\n");
	g_pCLogObj->Log("Get the Character Map Open Scale succeed\n");
#endif
}

lua_pushnumber(pLS, ret);

return 1;
}

// 完成地图入口的副本设置
// 参数：地图入口对象，副本编号
// 返回值：无
inline int lua_FinishSetMapEntryCopy(lua_State *pLS)
{
bool	bSuccess = true;

#ifdef defPARSE_LOG
//g_pCLogObj->Log("完成地图入口的副本设置 FinishSetMapEntryCopy\n");
g_pCLogObj->Log("FinishSetMapEntryCopy\n");
#endif

int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 2)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
	g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
	bSuccess = false;
	goto End;
}
{
	CDynMapEntryCell* pCMapEntryCell = (CDynMapEntryCell*)lua_touserdata(pLS, 1);
	if (!pCMapEntryCell)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t地图入口对象不存在，调用失败\n");
		g_pCLogObj->Log("\tMap entry object is inexistent,transfer failed\n");
#endif
		bSuccess = false;
		goto End;
	}

	pCMapEntryCell->SynCopyParam((short)lua_tonumber(pLS, 2) - 1);
}
End:
if (bSuccess)
{
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("完成地图入口的副本设置成功\n");
	g_pCLogObj->Log("complete set the map entrance copy succeed\n"));
#endif
}

return 0;
}

// 取道具类型
// 道具对象
// 返回值：道具类型
inline int lua_GetItemType(lua_State *pLS)
{
	bool	bSuccess = true;
	short	sItemType = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取道具类型 GetItemType\n");
	g_pCLogObj->Log("GetItemType\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		CItemRecord* pCItemRec = GetItemRecordInfo(pSItem->sID);
		if (!pCItemRec)
		{
			bSuccess = false;
			goto End;
		}
		sItemType = pCItemRec->sType;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取道具类型成功\n");
		g_pCLogObj->Log("Get item type succeed\n");
#endif
	}

	lua_pushnumber(pLS, sItemType);
	return 1;
}

// 取道具类型
// 道具ID
// 返回值：道具类型
inline int lua_GetItemType2(lua_State *pLS)
{
	bool	bSuccess = true;
	short	sItemType = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取道具类型 GetItemType2\n");
	g_pCLogObj->Log("GetItemType2\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		short sItemID = (short)lua_tonumber(pLS, 1);
		CItemRecord* pCItemRec = GetItemRecordInfo(sItemID);
		if (!pCItemRec)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		sItemType = pCItemRec->sType;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取道具类型成功\n");
		g_pCLogObj->Log("Get item type succeed\n");
#endif
	}

	lua_pushnumber(pLS, sItemType);

	return 1;
}

// 取道具等级
// 道具对象
// 返回值：道具等级

// lua param: item datatype
inline int lua_GetItemLv(lua_State *pLS)
{
	bool	bSuccess = true;
	short	sItemLv = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取道具等级 GetItemLv\n");
	g_pCLogObj->Log("GetItemLv\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		/*CItemRecord	*pCItemRec = GetItemRecordInfo(pSItem->sID);
		if (!pCItemRec)
		{
			bSuccess = false;
			goto End;
		}
		sItemLv = pCItemRec->sNeedLv;*/
		sItemLv = pSItem->sNeedLv;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取道具等级成功\n");
		g_pCLogObj->Log("Get item level\n");
#endif
	}

	lua_pushnumber(pLS, sItemLv);
	return 1;
}

inline int lua_GetItemOriginalLv(lua_State* pLS)
{
	bool	bSuccess = true;
	short	sItemLv = 0;

	int nParaNum = lua_gettop(pLS);

	if (nParaNum != 1)
	{
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
			bSuccess = false;
			goto End;
		}
		{
			CItemRecord* pCItemRec = GetItemRecordInfo(pSItem->sID);
			if (!pCItemRec)
			{
				bSuccess = false;
				goto End;
			}
			sItemLv = pCItemRec->sNeedLv;
		}
	}
End:
	lua_pushnumber(pLS, sItemLv);
	return 1;
}

inline int lua_SetItemLv(lua_State *pLS)
{
	bool	bSuccess = true;
	short	sItemLv = 0;

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
		bSuccess = false;
		goto End;
	}

	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);
		short nItemLv = (short)lua_tonumber(pLS, 2);

		if (!pSItem)
		{
			bSuccess = false;
			goto End;
		}
		pSItem->sNeedLv = nItemLv;
	}
End:
	return 1;
}

// 取道具等级
// 道具编号
// 返回值：道具等级
// lua param: itemid
inline int lua_GetItemLv2(lua_State *pLS)
{
	bool	bSuccess = true;
	short	sItemLv = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取道具等级 GetItemLv\n");
	g_pCLogObj->Log("GetItemLv\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}

	{
		CItemRecord* pCItemRec = GetItemRecordInfo((int)lua_tonumber(pLS, 1));
		if (!pCItemRec)
		{
			bSuccess = false;
			goto End;
		}
		sItemLv = pCItemRec->sNeedLv;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取道具等级成功\n");
		g_pCLogObj->Log("Get item level\n");
#endif
	}

	lua_pushnumber(pLS, sItemLv);
	return 1;
}

// 取道具编号
// 道具对象
// 返回值：道具编号
inline int lua_GetItemID(lua_State *pLS)
{
	bool	bSuccess = true;
	short	sItemID = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取道具ID GetItemID\n");
	g_pCLogObj->Log("GetItemID\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 1);

		if (!pSItem)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		sItemID = pSItem->sID;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取道具类型成功\n");
		g_pCLogObj->Log("Get item type succeed\n");
#endif
	}

	lua_pushnumber(pLS, sItemID);
	return 1;
}

// 取道具编号
// 道具编号
// 返回值：道具洞数
inline int lua_GetItemHoleNum(lua_State *pLS)
{
	bool	bSuccess = true;
	short	sHoleNum = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取道具洞数 GetItemHoleNum\n");
	g_pCLogObj->Log("GetItemHoleNum\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CItemRecord* pCItemRec = GetItemRecordInfo((long)lua_tonumber(pLS, 1));
		if (!pCItemRec)
		{
			bSuccess = false;
			goto End;
		}
		sHoleNum = pCItemRec->sHole;
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取道具类型成功\n");
		g_pCLogObj->Log("Get item type succeed\n");
#endif
	}

	lua_pushnumber(pLS, sHoleNum);
	return 1;
}

// 设置角色装备有效性
// 参数：角色对象，装备位置，是否有效（1，有效。0，无效）
// 返回值：1，设置成功。0，设置失败
inline int lua_SetChaEquipValid(lua_State *pLS)
{
	bool	bSuccess = true;
	char	chSetSuc = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置角色装备有效性 SetChaEquipValid\n");
	g_pCLogObj->Log("SetChaEquipValid\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		//add by ALLEN 2007-10-16	
		if (pCCha->IsReadBook())    //读书状态
		{
			bSuccess = false;
			goto End;
		}
		{
			char	chEquipPos = (char)lua_tonumber(pLS, 2);
			bool	bValid = (int)lua_tonumber(pLS, 3) != 0 ? true : false;
			chSetSuc = pCCha->SetEquipValid(chEquipPos, bValid) ? 1 : 0;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置角色装备有效性成功\n");
		g_pCLogObj->Log("SetChaEquipValidSucceed\n");
#endif
	}

	lua_pushnumber(pLS, chSetSuc);
	return 1;
}

// 设置角色背包道具的有效性
// 参数：角色对象，背包位置，是否有效（1，有效。0，无效），是否需要同步（1，需要[用于道具恢复]。0，不需要[用于使用道具]）
// 返回值：1，设置成功。0，设置失败
inline int lua_SetChaKbItemValid(lua_State *pLS)
{
	bool	bSuccess = true;
	char	chSetSuc = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置角色背包道具的有效性 SetChaKbItemValid\n");
	g_pCLogObj->Log("SetChaKbItemValid\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 4)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}
		{
			char	chKbPos = (char)lua_tonumber(pLS, 2);
			bool	bValid = (int)lua_tonumber(pLS, 3) != 0 ? true : false;
			bool	bSyn = (int)lua_tonumber(pLS, 4) != 0 ? true : false;
			chSetSuc = pCCha->SetKitbagItemValid(chKbPos, bValid, true, bSyn) ? 1 : 0;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置角色背包道具的有效性成功\n");
		g_pCLogObj->Log("SetChaKbItemValidSucceed");
#endif
	}

	lua_pushnumber(pLS, chSetSuc);
	return 1;
}

// 设置角色背包道具的有效性
// 参数：角色对象，道具对象，是否有效（1，有效。0，无效），是否需要同步（1，需要[用于道具恢复]。0，不需要[用于使用道具]）
// 返回值：1，设置成功。0，设置失败
inline int lua_SetChaKbItemValid2(lua_State* pLS)
{
	bool	bSuccess = true;
	char	chSetSuc = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置角色背包道具的有效性 SetChaKbItemValid2\n");
	g_pCLogObj->Log("SetChaKbItemValid2\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 4)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t对象不存在，调用失败\n");
			g_pCLogObj->Log("\tObject is inexistent,transfer failed\n");
#endif
			bSuccess = false;
			goto End;
		}

		{
			SItemGrid* pSItem = (SItemGrid*)lua_touserdata(pLS, 2);
			bool		bValid = (int)lua_tonumber(pLS, 3) != 0 ? true : false;
			bool		bSyn = (int)lua_tonumber(pLS, 4) != 0 ? true : false;
			chSetSuc = pCCha->SetKitbagItemValid(pSItem, bValid, true, bSyn) ? 1 : 0;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置角色背包道具的有效性成功\n");
		g_pCLogObj->Log("SetChaKbItemValid 2 Succeed\n");
#endif
	}

	lua_pushnumber(pLS, chSetSuc);
	return 1;
}

// 取公会挑战赛的双方公会ID
// 参数：挑战等级
// 返回值：擂主公会ID，挑战方公会ID
inline int lua_GetChallengeGuildID(lua_State *pLS)
{
	bool	bSuccess = true;
	char	chSetSuc = 0;
	DWORD	dwHostID = 0, dwReqID = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取公会挑战赛的双方公会ID GetChallengeGuildID\n");

	g_pCLogObj->Log("GetChallengeGuildID\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}

	DWORD	dwMoney;
	if (!game_db.GetChall((BYTE)lua_tonumber(pLS, 1), dwHostID, dwReqID, dwMoney))
	{
		bSuccess = false;
		goto End;
	}

End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取道具类型成功\n");
		g_pCLogObj->Log("Get item type succeed\n");
#endif

		lua_pushnumber(pLS, dwHostID);
		lua_pushnumber(pLS, dwReqID);
		return 2;
	}
	else
		return 0;
}

// 停止公会赛竞标
// 参数：挑战等级
// 返回值：无
inline int lua_EndGuildBid(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("开始公会赛竞标 BeginGuildBid\n");
	g_pCLogObj->Log("BeginGuildBid\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		BYTE	byLevel = (BYTE)lua_tonumber(pLS, 1);
		if (!game_db.HasChall(byLevel))
		{
			if (!game_db.StartChall(byLevel))
			{
				bSuccess = false;
				goto End;
			}
		}
	}

End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取道具类型成功\n");
		g_pCLogObj->Log("Get item type succeed\n");
#endif
	}

	return 0;
}

// 公会赛结束
// 参数：擂主公会ID，挑战方公会ID，擂主是否获胜
// 返回值：无
inline int lua_EndGuildChallenge(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("公会赛结束 EndGuildChallenge\n");
	g_pCLogObj->Log("EndGuildChallenge\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}

	game_db.EndChall((DWORD)lua_tonumber(pLS, 1), (DWORD)lua_tonumber(pLS, 2), (DWORD)lua_tonumber(pLS, 3) ? TRUE : FALSE);

End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取道具类型成功\n");
		g_pCLogObj->Log("Get item type succeed\n");
#endif
	}

	return 0;
}

// 增加背包容量
// 参数：角色对象，容量的增量
// 返回值：1，成功。0，失败
inline int lua_AddKbCap(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("增加背包容量 AddKbCap\n");
	g_pCLogObj->Log("AddKbCap\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t角色对象不存在，调用失败");
			g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
			bSuccess = false;
			goto End;
		}

		if (!pCCha->AddKitbagCapacity((short)lua_tonumber(pLS, 2)))
		{
			bSuccess = false;
			goto End;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("增加背包容量成功\n");
		g_pCLogObj->Log("AddKitbagCapSucceed\n");
#endif


		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 取背包容量
// 参数：角色对象
// 返回值：背包容量
inline int lua_GetKbCap(lua_State *pLS)
{
	bool	bSuccess = true;
	short	sCap = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取背包容量 GetKbCap\n");
	g_pCLogObj->Log("GetKbCap\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t角色对象不存在，调用失败");
			g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
			bSuccess = false;
			goto End;
		}
		sCap = pCCha->m_CKitbag.GetCapacity();
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取背包容量成功\n");
		g_pCLogObj->Log("GetKbCapSucceed\n");
#endif
	}

	lua_pushnumber(pLS, sCap);

	return 1;
}

// 两个角色是否在同一地图
// 参数：角色对象1，角色对象2
// 返回值：1，同一地图。0，不同地图。
inline int lua_IsInSameMap(lua_State *pLS)
{
	bool	bSuccess = true;
	char	chSameMap = 0;

#ifdef defPARSE_LOG
	g_pCLogObj->Log("两个角色是否在同一地图 IsInSameMap\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha1 = (CCharacter*)lua_touserdata(pLS, 1);
		CCharacter* pCCha2 = (CCharacter*)lua_touserdata(pLS, 2);
		if (!pCCha1 || !pCCha2)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t角色对象不存在，调用失败");
			g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
			bSuccess = false;
			goto End;
		}

		if (pCCha1->GetSubMap() && pCCha2->GetSubMap())
		{
			if (pCCha1->GetSubMap()->GetMapID() == pCCha2->GetSubMap()->GetMapID())
				chSameMap = 1;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("判断两个角色是否在同一地图成功\n");
		g_pCLogObj->Log("Check two characters whether in one map succeed\n");
#endif
	}

	lua_pushnumber(pLS, chSameMap);
	return 1;
}

// 两个角色是否在同一地图副本
// 参数：角色对象1，角色对象2
// 返回值：1，同一副本。0，不同副本。
inline int lua_IsInSameMapCopy(lua_State *pLS)
{
	bool	bSuccess = true;
	char	chSameCopy = 0;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("两个角色是否在同一地图副本 IsInSameMapCopy\n");
	g_pCLogObj->Log("IsInSameMapCopy\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha1 = (CCharacter*)lua_touserdata(pLS, 1);
		CCharacter* pCCha2 = (CCharacter*)lua_touserdata(pLS, 2);
		if (!pCCha1 || !pCCha2)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t角色对象不存在，调用失败");
			g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
			bSuccess = false;
			goto End;
		}

		if (pCCha1->GetSubMap() && pCCha2->GetSubMap())
		{
			if (pCCha1->GetSubMap() == pCCha2->GetSubMap())
				chSameCopy = 1;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("判断两个角色是否在同一地图副本成功\n");
		g_pCLogObj->Log("Check two characters whether in one map copy succeed\n");
#endif
	}

	lua_pushnumber(pLS, chSameCopy);
	return 1;
}

// 角色是否存活的
// 参数：角色对象
// 返回值：1，存活的；2，死亡的。
inline int lua_IsChaLiving(lua_State *pLS)
{
	bool	bSuccess = true;
	bool	bIsLiving = false;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("角色是否存活的 IsChaLiving\n");
	g_pCLogObj->Log("IsChaLiving\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t角色对象不存在，调用失败");
			g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
			bSuccess = false;
			goto End;
		}

		bIsLiving = pCCha->IsLiveing();
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("判断角色是否存活的成功\n");
		g_pCLogObj->Log("Check the  Character whether Living Succeed\n");
#endif
	}

	lua_pushnumber(pLS, bIsLiving ? 1 : 0);
	return 1;
}

// 设置角色的参数
// 参数：角色对象，参数索引，参数值
// 返回值：1，成功。0，失败。
inline int lua_SetChaParam(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("设置角色的参数 SetChaParam\n");
	g_pCLogObj->Log("SetChaParam\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t角色对象不存在，调用失败");
			g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
			bSuccess = false;
			goto End;
		}
		if (!pCCha->SetScriptParam((char)lua_tonumber(pLS, 2) - 1, (long)lua_tonumber(pLS, 3)))
		{
			bSuccess = false;
			goto End;
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("设置角色的参数成功：\n");
		g_pCLogObj->Log("Set character param succeed：\n");
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

// 取角色的参数
// 参数：角色对象，参数编号
// 返回值：参数值
inline int lua_GetChaParam(lua_State *pLS)
{
	bool	bSuccess = true;
	long ret = 0;
#ifdef defPARSE_LOG
	//g_pCLogObj->Log("取角色的参数 GetChaParam\n");
	g_pCLogObj->Log("GetChaParam\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t角色对象不存在，调用失败");
			g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
			bSuccess = false;
			goto End;
		}
		ret = pCCha->GetScriptParam((char)lua_tonumber(pLS, 2) - 1);
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("取角色的参数成功：\n");
		g_pCLogObj->Log("Get character param succeed：\n");
#endif

		lua_pushnumber(pLS, ret);
		return 1;
	}

	return 0;
}

// 增加道具影响
// 参数：角色对象，道具对象，方向（1，增加影响。0，移除影响）
// 返回值：1，成功。0，失败。
inline int lua_AddItemEffect(lua_State *pLS)
{
	bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("增加道具影响 AddItemEffect\n");
	g_pCLogObj->Log("AddItemEffect\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 3)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t角色对象不存在，调用失败");
			g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
			bSuccess = false;
			goto End;
		}
		{
			SItemGrid* pItem = (SItemGrid*)lua_touserdata(pLS, 2);
			if (!pItem)
			{
				bSuccess = false;
				goto End;
			}
			bool	bValid = (int)lua_tonumber(pLS, 3) != 0 ? true : false;
			pCCha->ChangeItem(bValid, pItem, enumEQUIP_HEAD);
		}
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("增加道具影响成功：\n");
		g_pCLogObj->Log("AddItemEffectSucceed：\n");
#endif

		lua_pushnumber(pLS, 1);
	}
	else
		lua_pushnumber(pLS, 0);

	return 1;
}

inline int lua_Lua(lua_State *pLS)
{
	if (!lua_isstring(pLS, 1))
		return 0;

	luaL_dostring(pLS, lua_tostring(pLS, 1));

	return 1;
}

inline int lua_LuaAll(lua_State *pLS)
{
	if (!lua_isstring(pLS, 1))
		return 0;

	WPACKET WtPk	=GETWPACKET();
	WRITE_CMD(WtPk, CMD_MM_DO_STRING);
	WRITE_LONG(WtPk, 0);
	WRITE_STRING(WtPk, lua_tostring(pLS, 1));

	BEGINGETGATE();
	GateServer	*pGateServer = NULL;
	while (pGateServer = GETNEXTGATE())
	{
		pGateServer->SendData(WtPk);
		break;
	}

	return 1;
}

inline int lua_ReloadCal(lua_State *L)
{
	luaL_dofile(L, GetResPath("script\\calculate\\skilleffect.lua"));
	return 1;
}

inline void ReloadCal()
{
	lua_ReloadCal(g_pLuaState);
}

// Add by lark.li 20080527

inline int lua_GetWinLotteryItemno(lua_State *pLS)
{
	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 2)
	{
		return 0;
	}

	int issue = (int)lua_tonumber(pLS, 1);
	int index = (int)lua_tonumber(pLS, 2);

	if(index >=0 && index <7)
	{
		std::string itemno;
		if(game_db.GetWinItemno(issue, itemno))
		{
			if(index ==0 )
				lua_pushstring(pLS, itemno.c_str());
			else
			{
				lua_pushstring(pLS, itemno.substr(index - 1, 1).c_str());
			}

			return 1;
		}
	}
	return 0;
}

// 生成中奖号码
inline int lua_CalWinLottery(lua_State *pLS)
{
int nParaNum = lua_gettop(pLS); // 取得参数个数
if (nParaNum != 2)
{
	return 0;
}
int issue = (int)lua_tonumber(pLS, 1);
int index = (int)lua_tonumber(pLS, 2);

if(index >=0 && index <7)
{
	size_t i = (size_t)index;
	std::string itemno;
	if(game_db.GetWinItemno(issue, itemno))
	{
		if(index ==0 )
			lua_pushstring(pLS, itemno.c_str());
		else
		{
			if(itemno.size() >= i)
				lua_pushstring(pLS, itemno.substr(index - 1, 1).c_str());
			else
				return 0;
		}
		return 1;
	}
}
return 0;	
}

// 获取当前期号
inline int lua_GetLotteryIssue(lua_State *pLS)
{
int issue;

if(game_db.GetLotteryIssue(issue))
{
	lua_pushnumber(pLS, (long)issue);

	return 1;
}

return 0;
}

// 追加彩票期
inline int lua_AddLotteryIssue(lua_State *pLS)
{
int issue = (int)lua_tonumber(pLS, 1);
game_db.AddIssue(issue);

return 1;
}

// 更改彩票期状态
inline int lua_DisuseLotteryIssue(lua_State *pLS)
{
int issue = (int)lua_tonumber(pLS, 1);
int state = (int)lua_tonumber(pLS, 2);
game_db.DisuseIssue(issue, state);

return 1;
}

// 判断是否是注册的合法队伍
inline int lua_IsValidRegTeam(lua_State *pLS)
{
int teamID =(int)lua_tonumber(pLS, 1);
CCharacter *pCaptain = (CCharacter*)lua_touserdata(pLS, 2);
CCharacter *pMember1 = (CCharacter*)lua_touserdata(pLS, 3);
CCharacter *pMember2 = (CCharacter*)lua_touserdata(pLS, 4);

if(game_db.IsValidAmphitheaterTeam(teamID, pCaptain->GetID(), pMember1->GetID(), pMember2->GetID()))
{
	lua_pushnumber(pLS, (long)1);	
}
else
{
	lua_pushnumber(pLS, (long)0);
}

return 1;
}

// 判断合法的队伍
// -1 不是队长
// -2 队伍不是3个人
// -3  不是师徒
// -4 不存在角色
// 1 OK
inline int lua_IsValidTeam(lua_State *pLS)
{
// 是否在队伍中
// 是否是3个人的队伍
// 是否是师徒关系
//int masterID = (int)lua_tonumber(pLS, 1);
CCharacter *pCCha = (CCharacter*)lua_touserdata(pLS, 1);
CPlayer*	pTeamPlayer = pCCha->GetPlayer();
int masterID = pTeamPlayer->GetDBChaId();
//CPlayer*	player = g_pGameApp->GetPlayerByDBID(masterID);

if(pTeamPlayer == NULL)
{
	lua_pushnumber(pLS, (long)-4);
	return 1;
}

//CCharacter *pCCha = player->GetMainCha();	

if(!pTeamPlayer->IsTeamLeader())
{
	lua_pushnumber(pLS, (long)-1);
	return 1;
}

if(pTeamPlayer->GetTeamMemberCnt() != 2)
{
	lua_pushnumber(pLS, (long)-2);
	return 1;
}

DWORD prenticeID1 = pTeamPlayer->GetTeamMemberDBID(0);
DWORD prenticeID2 = pTeamPlayer->GetTeamMemberDBID(1);


if(game_db.IsMasterRelation(masterID, prenticeID1) && game_db.IsMasterRelation(masterID, prenticeID2))
{
	lua_pushnumber(pLS, (long)1);
	return 1;
}


lua_pushnumber(pLS, (long)-3);
return 1;
}

// 获取当前竞技场赛季号
inline int lua_GetAmphitheaterSeason(lua_State *pLS)
{
int season = -1;
int round = -1;

if(game_db.GetAmphitheaterSeasonAndRound(season, round))
{
	lua_pushnumber(pLS, (long)season);
	return 1;
}
else
{
	lua_pushnumber(pLS, (long)0);
	return 0;
}

return 0;
}

// 获取当前竞技场轮次
inline int lua_GetAmphitheaterRound(lua_State *pLS)
{
int season = -1;
int round = -1;

if(game_db.GetAmphitheaterSeasonAndRound(season, round))
{
	lua_pushnumber(pLS, (long)round);
	return 1;
}
else
{
	lua_pushnumber(pLS, (long)0);
	return 1;
}

return 0;
}

// 追加竞技场赛季号和轮次
inline int lua_AddAmphitheaterSeason(lua_State *pLS)
{
int season  = (int)lua_tonumber(pLS, 1);

game_db.AddAmphitheaterSeason(season);

return 1;
}

// 更改赛季状态
inline int lua_DisuseAmphitheaterSeason(lua_State *pLS)
{
int season = (int)lua_tonumber(pLS, 1);
int state = (int)lua_tonumber(pLS, 2);
const char* winner = (const char*)lua_tostring(pLS, 3);

if(game_db.DisuseAmphitheaterSeason(season, state,winner ))
return 1;

return 0;
}

// 更改赛季轮次
inline int lua_UpdateAmphitheaterRound(lua_State *pLS)
{
int season = (int)lua_tonumber(pLS, 1);
int round = (int)lua_tonumber(pLS, 2);

if(game_db.UpdateAmphitheaterRound(season, round))
return 1;

return 0;
}

// 取得竞技场参赛队伍个数
inline int lua_GetAmphitheaterTeamCount(lua_State *pLS)
{
int count = 0;
if(game_db.GetAmphitheaterTeamCount(count))
{
	lua_pushnumber(pLS, (long)count);
	return 1;
}

lua_pushnumber(pLS, (long)0);
return 0;
}

// 取得参赛队伍ID
inline int lua_GetAmphitheaterNoUseTeamID(lua_State *pLS)
{
int teamID = 0;
if(game_db.GetAmphitheaterNoUseTeamID(teamID))
{
	lua_pushnumber(pLS, (long)teamID);
	return 1;
}

lua_pushnumber(pLS, (long)0);
return 1;
}

// 队伍注册
inline int lua_AmphitheaterTeamSignUP(lua_State *pLS)
{
int teamID = (int)lua_tonumber(pLS, 1);
CCharacter *pCaptain = (CCharacter*)lua_touserdata(pLS, 2);
CCharacter *pMember1 = (CCharacter*)lua_touserdata(pLS, 3);
CCharacter* pMember2= (CCharacter*)lua_touserdata(pLS, 4);

if(game_db.AmphitheaterTeamSignUP(teamID,pCaptain->GetPlayer()->GetDBChaId(), pMember1->GetPlayer()->GetDBChaId(), pMember2->GetPlayer()->GetDBChaId()))
return 1;

return 0;
}

// 队伍解散
inline int lua_AmphitheaterTeamCancel(lua_State *pLS)
{
int teamID = (int)lua_tonumber(pLS, 1);

if(game_db.AmphitheaterTeamCancel(teamID))
return 1;

return 0;
}

//Add by sunny.sun 20080723
//判断当前人物是否已经注册
//参数 角色
//返回值 注册返回1，没有返回0
inline int lua_IsAmphitheaterLogin(lua_State *pLS)
{
//int characterid = (int)lua_tonumber(pLS,1);
CCharacter *pActor = (CCharacter*)lua_touserdata(pLS, 1);
if(game_db.IsAmphitheaterLogin(pActor->GetID()))
{	
	lua_pushnumber(pLS,(long)0);
	return 1;
}
lua_pushnumber(pLS,(long)1);
return 1;
}

//判断地图队伍已经注册个数
//参数地图id
//返回值 该地图注册队伍个数0，1，2
inline int lua_IsMapFull(lua_State *pLS)
{
int PActorIDNum = 0;

int MapID = (int)lua_tonumber(pLS, 1);
if(game_db.IsMapFull(MapID,PActorIDNum))
{	
	lua_pushnumber(pLS,PActorIDNum);
	return 1;
}
return 0;
}

//更新进入地图后map字段的值为地图id
// 参数 队长角色，地图id
inline int lua_UpdateMapAfterEnter(lua_State *pLS)
{
	CCharacter *  Captain = (CCharacter*)lua_touserdata(pLS, 1);
	int CaptainID = Captain->GetID();
	int MapID = (int)lua_tonumber(pLS, 2);

	if(game_db.UpdateMapAfterEnter(CaptainID,MapID))
		return 1;
	return 0;
}

//更新比赛结束后map字段
inline int lua_UpdateMap(lua_State *pLS)
{
int Mapid = (int)lua_tonumber(pLS, 1);
if(game_db.UpdateMap(Mapid))
{
	return 1;
}
return 0;
}
//更新地图队伍中mapflag值
//参数 队伍id，地图id，要设定的mapflag值
inline int lua_UpdateMapNum(lua_State *pLS)
{
int nParaNum = lua_gettop(pLS); // 取得参数个数

if (nParaNum != 3)
{
	return 0;
}

int Teamid = (int)lua_tonumber(pLS,1);
int Mapid = (int)lua_tonumber(pLS,2);
int MapFlag = (int)lua_tonumber(pLS,3);

if(game_db.UpdateMapNum(Teamid,Mapid,MapFlag))
{
	return 1;
}
return 0;

}
//获得mapflag的值
//参数 队伍id
inline int lua_GetMapFlag(lua_State *pLS)
{
int Mapflag = 0;
int Teamid = (int)lua_tonumber(pLS,1);
if(game_db.GetMapFlag(Teamid,Mapflag))
{
	lua_pushnumber(pLS,Mapflag);
	return 1;
}
return 0;
}
//计算票数最高的复活队伍，并更新使其复活
inline int lua_SetMaxBallotTeamRelive( lua_State *pLS)
{
if(game_db.SetMaxBallotTeamRelive())
{
	return 1;
}
return 0;
}

//设置队伍state状态
inline int lua_SetMatchResult(lua_State *pLS)
{
int nParaNum = lua_gettop(pLS); // 取得参数个数
if (nParaNum != 4)
{
	return 0;
}

int Teamid1 = (int)lua_tonumber(pLS,1);
int Teamid2 = (int)lua_tonumber(pLS,2);
int Id1state = (int)lua_tonumber(pLS,3);
int Id2state = (int)lua_tonumber(pLS,4);
if(game_db.SetMatchResult(Teamid1,Teamid2,Id1state,Id2state))
{
	return 1;
}
return 0;

}
//根据地图id取地图中的队伍的队长id
inline int lua_GetCaptainByMapId(lua_State *pLS)
{
int Mapid = (int)lua_tonumber(pLS,1);
std::string Captainid1;
std::string Captainid2;
DWORD Capid1 = 0;
DWORD Capid2 = 0;
CCharacter *pCCha1 = NULL;
CCharacter *pCCha2 = NULL;
CPlayer	*	player1 = NULL;
CPlayer	*	player2 = NULL;
const char * nocaptain = "";
if(game_db.GetCaptainByMapId(Mapid,Captainid1,Captainid2))
{
	if( strcmp( nocaptain,Captainid2.c_str() ) == 0 )
	{
		Capid1 = atoi(Captainid1.c_str());
		Capid2 = 0;
	}
	else
	{
		Capid1 = atoi(Captainid1.c_str());
		Capid2 = atoi(Captainid2.c_str());
	}

	player1 = g_pGameApp->GetPlayerByDBID(Capid1);
	player2 = g_pGameApp->GetPlayerByDBID(Capid2);

	if( player2 == NULL )
	{	
		pCCha1 = player1->GetMainCha();	
		if( pCCha1 == NULL)
			return 0;
	}
	else
	{
		if( player1 == NULL )
			return 0;
		pCCha1 = player1->GetMainCha();	
		pCCha2 = player2->GetMainCha();	
		if(pCCha1 == NULL || pCCha2 == NULL )
			return 0;
	}
	lua_pushlightuserdata(pLS, pCCha1);
	lua_pushlightuserdata(pLS, pCCha2);
	return 2;
}
return 0;
}

//更新没参加的队伍为复活
inline int lua_UpdateAbsentTeamRelive(lua_State *pLS)
{
if(game_db.UpdateAbsentTeamRelive())
	return 1;
return 0;
}

// 更新胜利队伍winnum值
inline int lua_UpdateWinnum( lua_State *pLS )
{
	int teamid = (int)lua_tonumber(pLS,1);
	if(game_db.UpdateWinnum( teamid ))
		return 1;
	return 0;
}

//取winnum中唯一最大的值的队伍
inline int lua_GetUniqueMaxWinnum( lua_State *pLS )
{
	int teamid = 0;
	if(game_db.GetUniqueMaxWinnum( teamid ))
	{	
		lua_pushnumber(pLS,teamid);
		return 1;
	}
	return 0;
}

// 设置matchno状态，用来更新队伍是否进入地图
inline int lua_SetMatchnoState( lua_State *pLS )
{
	int teamid = (int)lua_tonumber(pLS,1);
	if(game_db.SetMatchnoState( teamid ))
		return 1;
	return 0;
}

// 更新下一轮比赛前state状态
inline int lua_UpdateState( lua_State *pLS )
{
	if(game_db.UpdateState())	
			return 1;
	return 0;
}

//计算state=1时的队伍个数，来判断复活条件
inline int lua_CloseReliveByState( lua_State *pLS )
{
	int statenum = 0;
	if(game_db.CloseReliveByState(statenum))
	{	
		lua_pushnumber(pLS,statenum);
		return 1;
	}
	return 0;
}

//根据队伍清除mapflag为空
inline int lua_CleanMapFlag( lua_State *pLS )
{
	int teamid1 = (int)lua_tonumber(pLS,1);
	int teamid2 = (int)lua_tonumber(pLS,2);
	if(game_db.CleanMapFlag(teamid1,teamid2))
		return 1;
	return 0;
}

//根据队伍id得到队伍状态
inline int lua_GetStateByTeamid( lua_State *pLS )
{
	int teamid = (int)lua_tonumber(pLS,1);
	int state = 0;
	if( game_db.GetStateByTeamid( teamid, state ))
	{		
		lua_pushnumber(pLS, state);
		return 1;
	}
	return 0;
}

inline int lua_LookEnergy(lua_State* pLS)
{
		bool	bSuccess = true;

#ifdef defPARSE_LOG
	//g_pCLogObj->Log("同步角色背包 SynChaKitbag\n");
	g_pCLogObj->Log("LookEnergy\n");
#endif

	int nParaNum = lua_gettop(pLS); // 取得参数个数

	if (nParaNum != 1)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("\t参数个数[%d]非法，调用失败\n", nParaNum);
		g_pCLogObj->Log("\tThe parameter numbers [%d] is unlawful,transfer failed!\n", nParaNum);
#endif
		bSuccess = false;
		goto End;
	}
	{
		CCharacter* pCCha = (CCharacter*)lua_touserdata(pLS, 1);
		if (!pCCha)
		{
#ifdef defPARSE_LOG
			//g_pCLogObj->Log("\t角色对象不存在，调用失败\n");
			g_pCLogObj->Log("\tThe character object is inexistence,transfer failed ! \n");
#endif
			bSuccess = false;
			goto End;
		}

		pCCha->SynLookEnergy();
	}
End:
	if (bSuccess)
	{
#ifdef defPARSE_LOG
		//g_pCLogObj->Log("同步角色背包成功：%d\n", pCCha->GetLogName());
		g_pCLogObj->Log("LookEnergy succeed：%d\n", pCCha->GetLogName());
#endif
	}

	return 0;
}

inline int lua_SetExpiration(lua_State* L) {
	bool bSuccess = true;
	int nParaNum = lua_gettop(L);

	if (nParaNum != 2)
	{
		bSuccess = false;
		goto End;
	}
	{
		SItemGrid* pSItem = static_cast<SItemGrid*> (lua_touserdata(L, 1));
		long expiration = *static_cast<long*> (lua_touserdata(L, 2));

		if (!pSItem) {
			bSuccess = false;
			goto End;
		}
		if (expiration == -1 || expiration == 0) {
			pSItem->expiration = 0;
		}
		else {
			pSItem->expiration = std::time(0) + expiration;
		}
	}
End:
	if (bSuccess)
	{
		lua_pushnumber(L, 1);
	}
	else
		lua_pushnumber(L, 0);

	return 1;
}

inline void RegisterLuaGameLogic(lua_State *L)
{ 
g_pCLogObj = new CLogFile;
if (!g_pCLogObj)
{
	//THROW_EXCP(excpMem,"lua错误纪录文件建立错误!");
	THROW_EXCP(excpMem,RES_STRING(GM_EXPAND_H_00160));
}
g_pCLogObj->SetLogName("ExecLua");
g_pCLogObj->SetEnable(true);

g_CParser.Init(L);

REGFN(GetChaAttr);
REGFN(SetChaAttr);
REGFN(CheckChaRole);
REGFN(SetSkillRange);
REGFN(SetRangeState);
REGFN(GetSkillPos);
REGFN(GetSkillLv);
REGFN(GetChaStateLv);
REGFN(GetObjDire);
REGFN(GetChaName);
REGFN(AddState);
REGFN(RemoveState);
REGFN(GetAreaStateLevel);
REGFN(SkillMiss);
REGFN(SkillCrt);
REGFN(SkillUnable);
REGFN(AddChaSkill);
REGFN(UseItemFailed);

//// Add by lark.li 20080721 begin
//REGFN(UseItemGiveMission);
//// End

REGFN(SetItemFall);
REGFN(BeatBack);
REGFN(IsInGymkhana);
REGFN(IsInPK);
REGFN(CheckBagItem);
REGFN(GetChaFreeBagGridNum);
REGFN(DelBagItem);
REGFN(DelBagItem2);
REGFN(RemoveChaItem);
REGFN(GetChaMapName);
REGFN(GetChaMapCopyNO);
REGFN(GetChaMapCopy);
REGFN(GetMainCha);
REGFN(GetCtrlBoat);
REGFN(ChaIsBoat);
REGFN(GetChaItem);
REGFN(GetChaItem2);
REGFN(GetItemAttr);
REGFN(SetItemAttr);
REGFN(AddItemAttr);
REGFN(GetItemFinalAttr);
REGFN(SetItemFinalAttr);
REGFN(AddItemFinalAttr);
REGFN(ResetItemFinalAttr);
REGFN(GetItemAttrRange);
REGFN(GetItemForgeParam);
REGFN(SetItemForgeParam);
REGFN(AddEquipEnergy);
REGFN(SetRelive);
REGFN(LuaPrint);
REGFN(ReloadCal);
REGFN(Stop);
REGFN(Notice);
REGFN(GuildNotice);
REGFN(ChaNotice);
REGFN(MapChaLight);
REGFN(SetItemHost);
REGFN(MapCanSavePos);
REGFN(MapCanPK);
REGFN(MapCanTeam);
REGFN(MapCopyNum);
REGFN(MapCopyStartType);
REGFN(MapType);
REGFN(SingleMapCopyPlyNum);
REGFN(SetMapEntryMapName);
REGFN(SetMapEntryEntiID);
REGFN(SetMapEntryEventName);
REGFN(CallMapEntry);
REGFN(GetMapEntryPosInfo);
REGFN(SetMapEntryTime);
REGFN(GetChaSideID);
REGFN(SetChaSideID);
REGFN(GetChaGuildID);
REGFN(GetChaTeamID);
REGFN(CheckChaPKState);
REGFN(GetChaPlayer);
REGFN(GetPlayerTeamID);
REGFN(GetPlayerID);
REGFN(GetGuildName);
REGFN(CloseMapEntry);
REGFN(CloseMapCopy);
REGFN(SetChaMotto);
REGFN(IsChaInLand);
REGFN(SetTeamFightMapName);
REGFN(Lua);
REGFN(LuaAll);
REGFN(RepatriateDie);
REGFN(SetMapCopyParam);
REGFN(SetMapCopyParam2);
REGFN(GetMapCopyParam);
REGFN(GetMapCopyParam2);
REGFN(GetMapCopyID);
REGFN(GetMapCopyID2);
REGFN(GetMapEntryCopyObj);
REGFN(GetMapCopyPlayerNum);
REGFN(BeginGetMapCopyPlayerCha);
REGFN(GetMapCopyNextPlayerCha);
REGFN(GetChaMapType);
REGFN(SetChaKitbagChange);
REGFN(SynChaKitbag);
REGFN(MapCopyNotice);
REGFN(MapCopyNotice2);
REGFN(GetChaMapOpenScale);
REGFN(FinishSetMapEntryCopy);
REGFN(GetItemType);
REGFN(GetItemType2);
REGFN(GetItemLv);
REGFN(GetItemOriginalLv);
REGFN(SetItemLv);
REGFN(GetItemLv2);
REGFN(GetItemID);
REGFN(GetItemHoleNum);
REGFN(SetChaEquipValid);
REGFN(SetChaKbItemValid);
REGFN(SetChaKbItemValid2);
REGFN(GetChallengeGuildID);
REGFN(EndGuildBid);
REGFN(EndGuildChallenge);
REGFN(AddKbCap);
REGFN(GetKbCap);
REGFN(IsInSameMap);
REGFN(IsInSameMapCopy);
REGFN(IsChaLiving);
REGFN(SetChaParam);
REGFN(GetChaParam);
REGFN(AddItemEffect);
REGFN(MapCanStall);
REGFN(MapCanGuild);
REGFN(KillMyMonster);
REGFN(KillMonsterInMapByName);

// Add by lark.li 20080527 begin
REGFN(GetLotteryIssue); 
REGFN(CalWinLottery); 
REGFN(GetWinLotteryItemno); 
REGFN(AddLotteryIssue); 
REGFN(DisuseLotteryIssue); 

REGFN(IsValidRegTeam); 
REGFN(IsValidTeam); 
REGFN(GetAmphitheaterSeason); 
REGFN(GetAmphitheaterRound); 
REGFN(AddAmphitheaterSeason); 
REGFN(DisuseAmphitheaterSeason); 
REGFN(UpdateAmphitheaterRound); 

REGFN(GetAmphitheaterTeamCount);
REGFN(GetAmphitheaterNoUseTeamID);
REGFN(AmphitheaterTeamSignUP);
REGFN(AmphitheaterTeamCancel);
REGFN(IsAmphitheaterLogin);
REGFN(IsMapFull);
REGFN(UpdateMapNum);//Add by sunny.sun 2080723
REGFN(SetMaxBallotTeamRelive);
REGFN(SetMatchResult);
REGFN(GetMapFlag);
REGFN(UpdateMap);
REGFN(GetCaptainByMapId);
REGFN(UpdateAbsentTeamRelive);
REGFN(UpdateMapAfterEnter);

REGFN(CleanMapFlag);
REGFN(UpdateWinnum);
REGFN(GetUniqueMaxWinnum);
REGFN(SetMatchnoState);
REGFN(UpdateState);
REGFN(CloseReliveByState);
REGFN(GetStateByTeamid);

REGFN(ScrollNotice);
REGFN(GMNotice);

REGFN(MoveToTemp);
REGFN(GetChaGuildPermission);
REGFN(GetItemStackSize);
REGFN(IsItemLocked);
REGFN(EquipItem);
REGFN(SynLook);
REGFN(EquipStringItem);

REGFN(IsAttributeEditable);
REGFN(SetAttributeEditable);
REGFN(GetChaFreeTempBagGridNum);

REGFN(GetIMP);

REGFN(SetIMP);
//Add by Mdr
REGFN(LookEnergy);
REGFN(SetExpiration);
// End
}