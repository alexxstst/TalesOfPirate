#include "StdAfx.h"
#include "isskilluse.h"
#include "skillrecord.h"
#include "character.h"
#include "gameapp.h"
#include "uiitemcommand.h"
#include "uiequipform.h"
#include "uiboatform.h"

CIsSkillUse	g_SkillUse;

//---------------------------------------------------------------------------
// class CIsSkillUse
//---------------------------------------------------------------------------
CIsSkillUse::CIsSkillUse()
{
	_eError = enumNone;
}

bool CIsSkillUse::IsValid( CSkillRecord* pSkill, CCharacter* pSelf )
{
	_pSkill = pSkill;
	if( !pSkill->GetIsValid() )
	{
		_eError = enumInValid;
		return false;
	}

	//if( !pSkill->IsAttackTime( CGameApp::GetCurTick() ) )
	//{
	//	_eError = enumNoTime;
	//	return false;
	//}

	int nEnergy = 0;
	CItemCommand* pItem = NULL;
	bool isCheckEnergy = false;
	for( int i=0; i<defSKILL_ITEM_NEED_NUM; i++ )
	{
		if( pSkill->sConchNeed[i][0]>=0 )
		{
			pItem = g_stUIEquip.GetEquipItem( pSkill->sConchNeed[i][0] );
			if( pItem )
			{
				nEnergy += pItem->GetData().sEnergy[0];
			}
			isCheckEnergy = true;
		}
		else
		{
			break;
		}
	}
	if( isCheckEnergy && pSkill->GetSkillGrid().sUseEnergy>nEnergy )
	{
		_eError = enumNotEnergy;
		return false;		
	}

	if( pSkill==pSelf->GetDefaultSkillInfo() )
	{
		if( pSelf->GetChaState()->IsFalse(enumChaStateAttack) )
		{
			_eError = enumNotAttack;
			return false;
		}
	}
	else
	{
		if( pSelf->GetChaState()->IsFalse(enumChaStateUseSkill) )
		{
			_eError = enumNotUse;
			return false;
		}
	}

	if( pSkill->GetSPExpend()>g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_SP) )
	{
		_eError = enumNotMP;
		return false;
	}

	return true;
}

bool CIsSkillUse::IsUse( CSkillRecord* pSkill, CCharacter* pSelf, CCharacter* pTarget )
{
	if( !IsValid( pSkill, pSelf ) )
		return false;

	if( pSkill->IsAttackArea() ) return true;

	if( !pTarget ) 
	{
		_eError = enumNotTarget;
		return false;
	}

	return IsAttack( pSkill, pSelf, pTarget );
}

bool CIsSkillUse::IsAttack( CSkillRecord* pSkill, CCharacter* pSelf, CCharacter* pTarget )
{
	_pSkill = pSkill;

	if( enumSKILL_TYPE_PLAYER_DIE==pSkill->chApplyTarget )
	{
		if( pTarget->getChaCtrlType()==enumCHACTRL_PLAYER && !pTarget->IsEnabled() )
			return true;

		_eError = enumDie;
		return false;
	}

	if( !pTarget->IsEnabled() )
	{
		_eError = enumAttackDie;
		return false;		
	}

	switch( pSkill->chApplyTarget )
	{
	case enumSKILL_TYPE_SELF:
		if( pSelf!=pTarget ) 
		{
			_eError = enumSelf;
			return false;
		}
		return true;
	case enumSKILL_TYPE_TEAM:
		if( pSelf==pTarget 
			|| ( pSelf->GetTeamLeaderID()>0 && pSelf->GetTeamLeaderID()==pTarget->GetTeamLeaderID() )
			)
		{
			return true;
		}
		_eError = enumOnlyTeam;
		return false;
	case enumSKILL_TYPE_TREE:
		if( pTarget->getChaCtrlType()!=enumCHACTRL_MONS_TREE )
		{
			_eError = enumTree;
			return false;
		}
		return true;
	case enumSKILL_TYPE_MINE:
		if( pTarget->getChaCtrlType()!=enumCHACTRL_MONS_MINE )
		{
			_eError = enumMine;
			return false;
		}
		return true;
	case enumSKILL_TYPE_FISH:
		if( pTarget->getChaCtrlType()!=enumCHACTRL_MONS_FISH )
		{
			_eError = enumFish;
			return false;
		}
		return true;
	case enumSKILL_TYPE_SALVAGE:
		if( pTarget->getChaCtrlType()!=enumCHACTRL_MONS_DBOAT )
		{
			_eError = enumDieBoat;
			return false;
		}
		return true;
	case enumSKILL_TYPE_REPAIR:
		if( pTarget->getChaCtrlType()!=enumCHACTRL_MONS_REPAIRABLE )
		{
			_eError = enumRepair;
			return false;
		}
		return true;
	default:
		if( pTarget->IsResource() )
		{
			_eError = enumTargetError;
			return false;
		}
		break;
	};

	if( pSkill->GetIsHelpful() ) 
	{
		if( pTarget->getChaCtrlType()==enumCHACTRL_MONS )
		{
			_eError = enumHelpMons;
			return false;
		}

		return true;
	}

	if( pTarget->IsMainCha() ) 
	{
		_eError = enumAttackMain;
		return false;
	}

	if( pSelf->GetTeamLeaderID()>0 && pSelf->GetTeamLeaderID()==pTarget->GetTeamLeaderID() )
	{
		_eError = enumAttackTeam;
		return false;
	}

	if( pSelf->getSideID()!=0 && pSelf->getSideID()==pTarget->getSideID() )
	{
		_eError = enumAttackTeam;
		return false;
	}

	if( pSelf->GetIsPK() )
	{
		if (pSelf->GetPK().IsFalse(enumChaPkGuild) && 
			pSelf->getGuildID() > 0 &&
			pSelf->getGuildID() == pTarget->getGuildID())
		{
			return false;
		}

		return true;
	}
	else if( pTarget->getChaCtrlType()==enumCHACTRL_PLAYER )
	{
		_eError = enumAttackPlayer;
		return false;
	}
    return true;
}

const char*	CIsSkillUse::GetError()
{
	static char buf[64] = { "CIsSkillUse ???" };
	switch( _eError )
	{
	case enumInValid: 
		sprintf( buf, g_oLangRec.GetString(149), _pSkill->szName.c_str());
		break;
	case enumNotEnergy:
		sprintf( buf, g_oLangRec.GetString(150), _pSkill->szName.c_str());
		break;
	case enumNotAttack: return g_oLangRec.GetString(151);
	case enumNotUse: return g_oLangRec.GetString(152);
	case enumNotMP: 
		sprintf( buf, g_oLangRec.GetString(153), _pSkill->szName.c_str());
		break;

	case enumSelf: 
		sprintf( buf, g_oLangRec.GetString(154), _pSkill->szName.c_str());
		break;
	case enumFish:
		sprintf( buf, g_oLangRec.GetString(155), _pSkill->szName.c_str());
		break;
	case enumDieBoat:
		sprintf( buf, g_oLangRec.GetString(156), _pSkill->szName.c_str());
		break;
	case enumTree:
		sprintf( buf, g_oLangRec.GetString(157), _pSkill->szName.c_str());
		break;
	case enumMine:
		sprintf( buf, g_oLangRec.GetString(158), _pSkill->szName.c_str());
		break;
	case enumOnlyTeam:
		sprintf( buf, g_oLangRec.GetString(159), _pSkill->szName.c_str());
		break;		
	case enumDie:
		sprintf( buf, g_oLangRec.GetString(160), _pSkill->szName.c_str());
		break;
	case enumTargetError:
		sprintf( buf, g_oLangRec.GetString(161), _pSkill->szName.c_str());
		break;
		
	case enumHelpMons:
		sprintf( buf, g_oLangRec.GetString(162), _pSkill->szName.c_str());
		break;
	case enumAttackMain:
		sprintf( buf, g_oLangRec.GetString(163), _pSkill->szName.c_str());
		break;
	case enumAttackTeam:
		sprintf( buf, g_oLangRec.GetString(164), _pSkill->szName.c_str());
		break;
	case enumAttackDie:
		sprintf( buf, g_oLangRec.GetString(165), _pSkill->szName.c_str());
		break;
	case enumAttackPlayer:
		sprintf( buf, g_oLangRec.GetString(166), _pSkill->szName.c_str());
		break;
	case enumRepair:
		sprintf( buf, g_oLangRec.GetString(167), _pSkill->szName.c_str());
		break;
	}
	return buf;
}
