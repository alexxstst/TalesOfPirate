//=============================================================================
// FileName: ItemContent.h
// Creater: ZhangXuedong
// Date: 2005.10.19
// Comment: Item Content class
//=============================================================================
#pragma once
#include "ItemAttr.h"
#include <array>
#include <fstream>
enum EItemDBParam
{
	enumITEMDBP_FORGE,			// 
	enumITEMDBP_INST_ID,		// (1.ID, 2.....)

	enumITEMDBP_MAXNUM,			// 
};

#define defITEM_INSTANCE_ATTR_NUM			5

//#pragma pack(push)
//#pragma pack(2)
struct SItemGrid // 
{

	//#pragma pack(pop)
	SItemGrid(short sId = 0, short sINum = 0) : sID(sId), sNum(sINum) {}

// 
	void	SetInstAttrInvalid() {sInstAttr[0][0] = 0;}
	bool	IsInstAttrValid() const { return sInstAttr[0][0] > 0; }

	// 
	char	GetItemLevel() 
	{ 
		char chLevel = char(lDBParam[1]&0xFFFF);
		return chLevel; 
	}

	void    SetItemLevel( char chLevel ) 
	{
		lDBParam[1] &= 0xFFFF0000;
		lDBParam[1] |= chLevel; 
	}

	void	AddItemLevel( char chLevel = 1 ) 
	{
		char chData = char(lDBParam[1]&0xFFFF);
		chData += chLevel;
		SetItemLevel( chData );
	}

	USHORT  GetFusionItemID() 
	{
		USHORT sID = USHORT(lDBParam[1]>>16);
		return sID;
	}

	void	SetFusionItemID(USHORT sID)
	{
		lDBParam[1] &= 0x0000FFFF;
		lDBParam[1] |= DWORD(sID)<<16;
	}

	char	GetForgeLv(void) const {return chForgeLv;}
	void	SetForgeLv(char chFLv) {chForgeLv = chFLv; SetChange();}
	void	AddForgeLv(char chAddLv) {chForgeLv += chAddLv; if (chForgeLv < 0) chForgeLv = 0; SetChange();}
	long	GetDBParam(short sParamID) const {return lDBParam[sParamID];}
	void	SetDBParam(short sParamID, long lParamVal);
	long	GetForgeParam(void) const { return GetDBParam(enumITEMDBP_FORGE); }
	void	SetForgeParam(long lVal) {SetDBParam(enumITEMDBP_FORGE, lVal);}

	bool	operator == (SItemGrid& SItem);
	bool	HasInstAttr(long lAttrID);
	short	GetInstAttr(long lAttrID) const;
	bool	SetInstAttr(long lAttrID, short sAttr);
	bool	AddInstAttr(long lAttrID, short sAttr);

	bool	InitAttr();
	bool	CheckAttr();
	short	GetAttr(long lAttrID);
	short	SetAttr(long lAttrID, short sAttr);
	short	AddAttr(long lAttrID, short sAttr);

	void	CheckValid();
	bool	IsValid() const { return bValid; }
	void	SetValid(bool bVld = true) {bValid = bVld; SetChange();}
	bool	IsChange() const { return bChange; }
	void	SetChange(bool bChg = true) {bChange = bChg;}
	
	// 
	void    CopyInstAttr( SItemGrid& item );
	bool	FusionCheck( SItemGrid& item ); 

	bool	bIsLock{ false };
	short	sNeedLv{ 0 };
	DWORD	dwDBID{ 0 };
	short	sID{ 0 };		// ID0
	short	sNum{ 0 };		// 
	std::array<short, 2> sEndure{};
	std::array<short, 2> sEnergy{};
	char	chForgeLv{ 0 };	// 
	std::array<long, enumITEMDBP_MAXNUM> lDBParam{};
	std::array<std::array<short, 2>, defITEM_INSTANCE_ATTR_NUM> sInstAttr{};
	CItemAttr	CAttr;	// 

	bool	bValid{ true };
	bool	bChange{ false }; // 
	bool	bItemTradable{ true };
	long	expiration{ 0 };
};

inline void SItemGrid::SetDBParam(short sParamID, long lParamVal)
{
	if (sParamID >= 0 && sParamID < lDBParam.size())
	{
		lDBParam[sParamID] = lParamVal;
	}
	else
	{
		//NOTE(Ogge): What's the logic in resetting whole array here? Legacy code decision
		lDBParam = {};
	}
	SetChange();
}

inline bool SItemGrid::operator == (SItemGrid& SItem)
{
	return
		sID == SItem.sID
		&& sNum == SItem.sNum
		&& sEndure[0] == SItem.sEndure[0]
		&& sEndure[1] == SItem.sEndure[1]
		&& sEnergy[0] == SItem.sEnergy[0]
		&& sEnergy[1] == SItem.sEnergy[1]
		&& chForgeLv == SItem.chForgeLv
		&& bItemTradable == SItem.bItemTradable
		&& lDBParam == SItem.lDBParam
		&& sInstAttr == SItem.sInstAttr;
}

inline bool SItemGrid::HasInstAttr(long lAttrID)
{
	if (lAttrID == ITEMATTR_URE)
		return true;
	if (lAttrID == ITEMATTR_MAXURE)
		return true;
	if (lAttrID == ITEMATTR_ENERGY)
		return true;
	if (lAttrID == ITEMATTR_MAXENERGY)
		return true;
	if (lAttrID == ITEMATTR_FORGE)
		return true;
	if (lAttrID == ITEMATTR_TRADABLE)
		return true;


	for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
	{
		if (lAttrID == sInstAttr[i][0])
			return true;
	}

	return false;
}

inline short SItemGrid::GetInstAttr(long lAttrID) const
{
	if (lAttrID == ITEMATTR_URE)
		return sEndure[0];
	if (lAttrID == ITEMATTR_MAXURE)
		return sEndure[1];
	if (lAttrID == ITEMATTR_ENERGY)
		return sEnergy[0];
	if (lAttrID == ITEMATTR_MAXENERGY)
		return sEnergy[1];
	if (lAttrID == ITEMATTR_FORGE)
		return chForgeLv;
	if (lAttrID == ITEMATTR_TRADABLE)
		return bItemTradable;


	for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
	{
		if (lAttrID == sInstAttr[i][0])
			return sInstAttr[i][1];
	}

	return 0;
}

inline bool SItemGrid::SetInstAttr(long lAttrID, short sAttr)
{
	if (lAttrID == ITEMATTR_TRADABLE){
		bItemTradable = sAttr == 0 ? false : true;
		goto ItemAttrSetSuc;
	}
	if (lAttrID == ITEMATTR_MAXURE)
	{
		sEndure[1] = sAttr;
		if (sEndure[0] > sEndure[1])
			sEndure[0] = sEndure[1];
		goto ItemAttrSetSuc;
	}
	if (lAttrID == ITEMATTR_URE)
	{
		sEndure[0] = sAttr;
		if (sEndure[0] > sEndure[1])
			sEndure[0] = sEndure[1];
		goto ItemAttrSetSuc;
	}
	if (lAttrID == ITEMATTR_MAXENERGY)
	{
		sEnergy[1] = sAttr;
		if (sEnergy[0] > sEnergy[1])
			sEnergy[0] = sEnergy[1];
		goto ItemAttrSetSuc;
	}
	if (lAttrID == ITEMATTR_ENERGY)
	{
		sEnergy[0] = sAttr;
		if (sEnergy[0] > sEnergy[1])
			sEnergy[0] = sEnergy[1];
		goto ItemAttrSetSuc;
	}
	if (lAttrID == ITEMATTR_FORGE)
	{
		chForgeLv = (char)sAttr;
		goto ItemAttrSetSuc;
	}

	{
		for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
		{
			if (lAttrID == sInstAttr[i][0])
			{
				sInstAttr[i][1] = sAttr;
				goto ItemAttrSetSuc;
			}
		}
	}

	return false;

ItemAttrSetSuc:
	SetChange();
	return true;
}

inline bool SItemGrid::AddInstAttr(long lAttrID, short sAttr)
{
	if (lAttrID == ITEMATTR_TRADABLE){
		bItemTradable = sAttr == 0 ? false : true;
		goto ItemAttrAddSuc;
	}

	if (lAttrID == ITEMATTR_MAXURE)
	{
		sEndure[1] += sAttr;
		if (sEndure[0] > sEndure[1])
			sEndure[0] = sEndure[1];
		goto ItemAttrAddSuc;
	}
	if (lAttrID == ITEMATTR_URE)
	{
		sEndure[0] += sAttr;
		if (sEndure[0] > sEndure[1])
			sEndure[0] = sEndure[1];
		goto ItemAttrAddSuc;
	}
	if (lAttrID == ITEMATTR_MAXENERGY)
	{
		sEnergy[1] += sAttr;
		if (sEnergy[0] > sEnergy[1])
			sEnergy[0] = sEnergy[1];
		goto ItemAttrAddSuc;
	}
	if (lAttrID == ITEMATTR_ENERGY)
	{
		sEnergy[0] += sAttr;
		if (sEnergy[0] > sEnergy[1])
			sEnergy[0] = sEnergy[1];
		goto ItemAttrAddSuc;
	}
	if (lAttrID == ITEMATTR_FORGE)
	{
		chForgeLv += (char)sAttr;
		goto ItemAttrAddSuc;
	}

	{
		for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
		{
			if (lAttrID == sInstAttr[i][0])
			{
				sInstAttr[i][1] += sAttr;
				goto ItemAttrAddSuc;
			}
		}
	}

	return false;

ItemAttrAddSuc:
	SetChange();
	return true;
}

//inline bool SItemGrid::InitAttr(void)
//{
//	if( sID >= enumItemFusionStart && sID < enumItemFusionEnd && lDBParam[1] )
//	{
//		sID = (USHORT)lDBParam[1];
//	}
//
//	if (!CAttr.Init(sID))
//		return false;
//	SetAttr(ITEMATTR_URE, sEndure[0]);
//	SetAttr(ITEMATTR_MAXURE, sEndure[1]);
//	SetAttr(ITEMATTR_ENERGY, sEnergy[0]);
//	SetAttr(ITEMATTR_MAXENERGY, sEnergy[1]);
//	SetAttr(ITEMATTR_FORGE, chForgeLv);
//	for (char i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
//	{
//		if (sInstAttr[i][0] == 0)
//			break;
//		SetAttr(sInstAttr[i][0], sInstAttr[i][1]);
//	}
//
//	return true;
//}

inline bool SItemGrid::CheckAttr()
{
	if (!CAttr.HasInit())
		return InitAttr();
	return true;
}

inline short SItemGrid::GetAttr(long lAttrID)
{
	if (!CheckAttr()){
		return 0;
	}
	return CAttr.GetAttr(short(lAttrID));
}

inline short SItemGrid::SetAttr(long lAttrID, short sAttr)
{
	if (!CheckAttr())
		return 0;
	return CAttr.SetAttr(short(lAttrID), sAttr);
}

inline short SItemGrid::AddAttr(long lAttrID, short sAttr)
{
	if (!CheckAttr())
		return 0;
	return CAttr.AddAttr(short(lAttrID), sAttr);
}

inline void SItemGrid::CopyInstAttr( SItemGrid& item )
{
	sEndure = item.sEndure;
	sEnergy = item.sEnergy;
	chForgeLv = item.chForgeLv;
	sInstAttr = item.sInstAttr;
	bItemTradable = item.bItemTradable;
	expiration = item.expiration;
	InitAttr();
}
