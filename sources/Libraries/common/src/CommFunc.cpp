//=============================================================================
// FileName: CommFunc.cpp
// Creater: Jerry Li
// Date: 2005.02.23
// Comment:
//	2005.4.28	Arcol	add the text filter manager class: CTextFilter
//=============================================================================

#include "CommFunc.h"
#include "GameCommon.h"
#include <charconv>
#include <string_view>
#include <vector>

using namespace std;

const char	*g_szJobName[MAX_JOB_TYPE] = 
{\
"Newbie",\
"Swordsman",\
"Hunter",\
"Sailor",\
"Explorer",\
"Herbalist",\
"Engineer",\
"Merchant",\
"Champion",\
"Crusader",\
"White Knight",\
"Animal Tamer",\
"Sharpshooter",\
"Cleric",\
"Seal master",\
"Captain",\
"Voyager",\
"Upstart",\
"Engineer",\
};

const char	*g_szCityName[defMAX_CITY_NUM] =
{\
"Argent city",\
"Thundoria Castle",\
"Shaitan City",\
"Icicle",\
};

const long g_PartIdRange[PLAY_NUM][enumEQUIP_NUM + 1][2] = 
{
						/**/		/**/
	//
		/**/			0,					0,
		/*HEAD */	2554,				2561,
		/*BODY */	0,					0,
		/*GLOVE*/	0,					0,
		/*SHOES*/	0,					0,
		/*NECK */	0,					0,
		/*LHAND*/	0,					0,
		/*HAND1*/	0,					0,
		/*HAND2*/	0,					0,
		/*RHAND*/	0,					0,
		/*FACE */	2000,				2007,

	//
		/**/			0,					0,
		/*HEAD */	2554,				2561,
		/*BODY */	0,					0,
		/*GLOVE*/	0,					0,
		/*SHOES*/	0,					0,
		/*NECK */	0,					0,
		/*LHAND*/	0,					0,
		/*HAND1*/	0,					0,
		/*HAND2*/	0,					0,
		/*RHAND*/	0,					0,
		/*FACE */	2062,				2069,

	//
		/**/			0,					0,
		/*HEAD */	2554,				2561,
		/*BODY */	0,					0,
		/*GLOVE*/	0,					0,
		/*SHOES*/	0,					0,
		/*NECK */	0,					0,
		/*LHAND*/	0,					0,
		/*HAND1*/	0,					0,
		/*HAND2*/	0,					0,
		/*RHAND*/	0,					0,
		/*FACE */	2124,				2131,

	//
		/**/			0,					0,
		/*HEAD */	2554,				2561,
		/*BODY */	0,					0,
		/*GLOVE*/	0,					0,
		/*SHOES*/	0,					0,
		/*NECK */	0,					0,
		/*LHAND*/	0,					0,
		/*HAND1*/	0,					0,
		/*HAND2*/	0,					0,
		/*RHAND*/	0,					0,
		/*FACE */	2291,				2294,
};

bool g_IsValidLook( int nType, int nPart, long nValue )
{
	if( (nType>=1 && nType<=PLAY_NUM) && (nPart>=0 && nPart<enumEQUIP_NUM + 1)  )
	{
		/*return	(nValue >= g_PartIdRange[nType-1][nPart][0]) && 
				(nValue <= g_PartIdRange[nType-1][nPart][1]);*/
		return true;
	}
	return false;
}

BOOL IsDist( int x1, int y1, int x2, int y2, DWORD dwDist )
{
	DWORD dwxDist = ( x1 - x2 ) * ( x1 - x2 );
	DWORD dwyDist = ( y1 - y2 ) * ( y1 - y2 );
	dwDist *= dwDist;
	return ( dwxDist + dwyDist < dwDist * 10000 );
}

const char* g_GetAreaName( int nValue )
{
    switch( nValue )
    {
    case 1:  return "Land / Sea"; //"/";
    case 2:  return "safe zone"; //"";
    case 3:  return "Non PK zone"; //"PK";
    case 4:  return "Bridge"; //"";
    case 5:  return "Forbid monster zone"; //"";
    case 6:  return "Mining Area"; //"";
    default: return "Unknown"; //"";
    }
}

inline bool g_IsRealItemID(int nItemID)
{
	if (nItemID > 0 && nItemID != enumEQUIP_BOTH_HAND && nItemID != enumEQUIP_TOTEM)
		return true;
	return false;
}

static inline int GetItemType( int nItemID )
{
	if( g_IsRealItemID(nItemID) )
	{
		CItemRecord* pItem = GetItemRecordInfo( nItemID );
		if( pItem )
		{
			return pItem->sType;
		}

		return -1;
	}

	return nItemID;
}

int g_GetItemSkill( int nLeftItemID, int nRightItemID )
{
	int nRightType = GetItemType( nRightItemID );
	int nLeftType = GetItemType( nLeftItemID );

	const int nLRSkill[][3] = {
		// 
		0,		0,		25,
		1,		1,		38,
		11,		1,		28,
		9999,	2,		29,
		9999,	6,		33
	};

		// 
	const int nRSkill[][3] = {
		-1,		1,		28,
		-1,		4,		31,
		-1,		5,		32,
		-1,		7,		34,
		-1,		8,		35,
		-1,		9,		36,
		-1,		10,		37,
		-1,		18,		200,
		-1,		19,		201
	};

		// 
	const int nLSkill[][3] = {
		11,		-1,		25,
		3,		-1,		30
	};

	const int nLRCount = sizeof(nLRSkill) / sizeof(nLRSkill[0]);
	for( int i=0; i<nLRCount; i++ )
	{
		if( nRightType==nLRSkill[i][1] && nLeftType==nLRSkill[i][0] )
		{
			return nLRSkill[i][2];
		}
	}

	const int nRCount = sizeof(nRSkill) / sizeof(nRSkill[0]);
	for( int i=0; i<nRCount; i++ )
	{
		if( nRightType==nRSkill[i][1] )
		{
			return nRSkill[i][2];
		}
	}

	const int nLCount = sizeof(nLSkill) / sizeof(nLSkill[0]);
	for( int i=0; i<nLCount; i++ )
	{
		if( nLeftType==nLSkill[i][0] )
		{
			return nLSkill[i][2];
		}
	}
	return -1;
}

int g_IsUseSkill( stNetChangeChaPart *pSEquip, int nSkillID )
{
	CSkillRecord *p =  GetSkillRecordInfo( nSkillID );
	if( !p )
	{
		return -1;
	}

	int	nLHandID = pSEquip->SLink[enumEQUIP_LHAND].sID;
	int	nRHandID = pSEquip->SLink[enumEQUIP_RHAND].sID;
	int	nBodyID = pSEquip->SLink[enumEQUIP_BODY].sID;
	if (!pSEquip->SLink[enumEQUIP_LHAND].IsValid())
		nLHandID = 0;
	if (!pSEquip->SLink[enumEQUIP_RHAND].IsValid())
		nRHandID = 0;
	if (!pSEquip->SLink[enumEQUIP_BODY].IsValid())
		nBodyID = 0;

	short sLHandType = 0, sRHandType = 0, sBodyType = 0;

	CItemRecord	*pItem = GetItemRecordInfo(nLHandID);
	if (pItem) sLHandType = pItem->sType;

	pItem = GetItemRecordInfo(nRHandID);
	if (pItem) sRHandType = pItem->sType;

	pItem = GetItemRecordInfo(nBodyID);
	if (pItem) sBodyType = pItem->sType;

	bool IsLeft = false;
	bool IsRight = false;
	bool IsBody = false;
	bool IsConch = false;

	// 
	for( int i=0; i<defSKILL_ITEM_NEED_NUM; i++ )
	{	
		if( p->sItemNeed[0][i][0] == cchSkillRecordKeyValue )
			break;

		if(p->sItemNeed[0][i][0] == enumSKILL_ITEM_NEED_TYPE)
		{
			if (p->sItemNeed[0][i][1] == -1 || p->sItemNeed[0][i][1] == sLHandType)
			{
				IsLeft = true;
				break;
			}
		}
		else if (p->sItemNeed[0][i][0] == enumSKILL_ITEM_NEED_ID)
		{
			if (p->sItemNeed[0][i][1] == nLHandID)
			{
				IsLeft = true;
				break;
			}
		}
	}
	if (!IsLeft)
		return 0;

	// 
	for( int i=0; i<defSKILL_ITEM_NEED_NUM; i++ )
	{	
		if( p->sItemNeed[1][i][0] == cchSkillRecordKeyValue )
			break;

		if(p->sItemNeed[1][i][0] == enumSKILL_ITEM_NEED_TYPE)
		{
			if (p->sItemNeed[1][i][1] == -1 || p->sItemNeed[1][i][1] == sRHandType)
			{
				IsRight = true;
				break;
			}
		}
		else if (p->sItemNeed[1][i][0] == enumSKILL_ITEM_NEED_ID)
		{
			if (p->sItemNeed[1][i][1] == nRHandID)
			{
				IsRight = true;
				break;
			}
		}
	}
	if (!IsRight)
		return 0;

	// 
	for( int i = 0; i < defSKILL_ITEM_NEED_NUM; i++ )
	{	
		if( p->sItemNeed[2][i][0] == cchSkillRecordKeyValue )
			break;

		if(p->sItemNeed[2][i][0] == enumSKILL_ITEM_NEED_TYPE)
		{
			if (p->sItemNeed[2][i][1] == -1 || p->sItemNeed[2][i][1] == sBodyType)
			{
				IsBody = true;
				break;
			}
		}
		else if (p->sItemNeed[2][i][0] == enumSKILL_ITEM_NEED_ID)
		{
			if (p->sItemNeed[2][i][1] == nBodyID)
			{
				IsBody = true;
				break;
			}
		}
	}
	if (!IsBody)
		return 0;

	// 
	for (int i = 0; i < defSKILL_ITEM_NEED_NUM; i++)
	{
		if (p->sConchNeed[i][0] == cchSkillRecordKeyValue)
			break;

		if (p->sConchNeed[i][0] == -1) // 
		{
			IsConch = true;
			break;
		}
		if (!pSEquip->SLink[p->sConchNeed[i][0]].IsValid())
			continue;
		pItem = GetItemRecordInfo(pSEquip->SLink[p->sConchNeed[i][0]].sID);
		if (pItem)
		{
			if (p->sConchNeed[i][1] == enumSKILL_ITEM_NEED_TYPE)
			{
				if (pItem->sType == p->sConchNeed[i][2])
				{
					IsConch = true;
					break;
				}
			}
			else if (p->sConchNeed[i][1] == enumSKILL_ITEM_NEED_ID)
			{
				if (pItem->lID == p->sConchNeed[i][2])
				{
					IsConch = true;
					break;
				}
			}
		}
	}
	if (!IsConch)
		return 0;

	return 1;
}

//=============================================================================
// 
// nTChaCtrlType EChaCtrlType
// bTIsDie 
// bTChaBeSkilled 
// nTChaArea EAreaMask
// nSSkillObjType ESkillObjType
// nSSkillObjHabitat ESkillTarHabitatType
// nSSkillEffType ESkillEffType
// bIsTeammate 
// bIsTeammate 
// bIsSelf 
//=============================================================================
int	g_IsRightSkillTar(int nTChaCtrlType, bool bTIsDie, bool bTChaBeSkilled, int nTChaArea,
					  int nSChaCtrlType, int nSSkillObjType, int nSSkillObjHabitat, int nSSkillEffType,
					  bool bIsTeammate, bool bIsFriend, bool bIsSelf)
{
	bool bTIsPlayer = g_IsPlyCtrlCha(nTChaCtrlType);

	if (g_IsNPCCtrlCha(nTChaCtrlType)) // NPC
		return enumESKILL_FAILD_NPC;
	if (!bTChaBeSkilled) // 
		return enumESKILL_FAILD_NOT_SKILLED;

	if (nTChaArea & enumAREA_TYPE_NOT_FIGHT) // 
	{
		if (nSSkillEffType != enumSKILL_EFF_HELPFUL)
			return enumESKILL_FAILD_SAFETY_BELT;
	}
	if (nTChaArea & enumSKILL_TAR_LAND || nTChaArea & enumAREA_TYPE_BRIDGE ) // 
	{
		if (nSSkillObjHabitat == enumSKILL_TAR_SEA)
			return enumESKILL_FAILD_NOT_LAND;
	}
	else if (!(nTChaArea & enumSKILL_TAR_LAND)) // 
	{
		if (nSSkillObjHabitat == enumSKILL_TAR_LAND)
			return enumESKILL_FAILD_NOT_SEA;
	}

	if (!bIsSelf) // 
	{
		if (nSSkillObjType == enumSKILL_TYPE_SELF)
			return enumESKILL_FAILD_ONLY_SELF;
		else if(nSSkillObjType == enumSKILL_TYPE_EXCEPT_SELF) // 
		{
			return enumESKILL_SUCCESS;
		}
	}
	else
	{
		if(nSSkillObjType == enumSKILL_TYPE_EXCEPT_SELF)
			return enumESKILL_FAILD_SELF;
	}

	if (bTIsDie) // 
	{
		if (!bTIsPlayer)
			return enumESKILL_FAILD_ONLY_DIEPLY;
		if( nSSkillObjType != enumSKILL_TYPE_PLAYER_DIE )
			return enumESKILL_FAILD_ONLY_DIEPLY;
	}

	if (nTChaCtrlType == enumCHACTRL_MONS_TREE) // 
	{
		if (nSSkillObjType != enumSKILL_TYPE_TREE)
			return enumESKILL_FAILD_ESP_MONS;
	}
	else if (nTChaCtrlType == enumCHACTRL_MONS_MINE) // 
	{
		if (nSSkillObjType != enumSKILL_TYPE_MINE)
			return enumESKILL_FAILD_ESP_MONS;
	}
	else if(nTChaCtrlType == enumCHACTRL_MONS_FISH) // 
	{
		if (nSSkillObjType != enumSKILL_TYPE_FISH)
			return enumESKILL_FAILD_ESP_MONS;
	}
	else if (nTChaCtrlType == enumCHACTRL_MONS_DBOAT) // 
	{
		if (nSSkillObjType != enumSKILL_TYPE_SALVAGE)
			return enumESKILL_FAILD_ESP_MONS;
	}

	if (nSSkillObjType == enumSKILL_TYPE_REPAIR)
	{
		if (nTChaCtrlType != enumCHACTRL_MONS_REPAIRABLE)
			return enumESKILL_FAILD_ESP_MONS;
	}
	if (nSSkillObjType == enumSKILL_TYPE_TREE)
	{
		if (nTChaCtrlType != enumCHACTRL_MONS_TREE)
			return enumESKILL_FAILD_ESP_MONS;
	}
	else if (nSSkillObjType == enumSKILL_TYPE_MINE)
	{
		if (nTChaCtrlType != enumCHACTRL_MONS_MINE)
			return enumESKILL_FAILD_ESP_MONS;
	}
	else if(nSSkillObjType == enumSKILL_TYPE_FISH)
	{
		if (nTChaCtrlType != enumCHACTRL_MONS_FISH)
			return enumESKILL_FAILD_ESP_MONS;
	}
	else if (nSSkillObjType == enumSKILL_TYPE_SALVAGE)
	{
		if (nTChaCtrlType != enumCHACTRL_MONS_DBOAT)
			return enumESKILL_FAILD_ESP_MONS;
	}

	if(nSSkillObjType == enumSKILL_TYPE_ENEMY)
	{
		if (bIsFriend)
			return enumESKILL_FAILD_FRIEND;
	}
	else if(nSSkillObjType == enumSKILL_TYPE_TEAM )
	{
		if (!bIsTeammate)
			return enumESKILL_FAILD_NOT_FRIEND;
	}
	else if(nSSkillObjType == enumSKILL_TYPE_ALL )
	{
		if (nSSkillEffType == enumSKILL_EFF_HELPFUL) // 
		{
			if (!bIsFriend)
				return enumESKILL_FAILD_NOT_FRIEND;
		}
		else
		{
			if (bIsFriend)
				return enumESKILL_FAILD_FRIEND;
		}
	}

	return enumESKILL_SUCCESS;
}

//------------------------------------------------------------------------
//	CTextFilter 
//------------------------------------------------------------------------

BYTE CTextFilter::m_NowSign[eTableMax][8];
vector<string> CTextFilter::m_FilterTable[eTableMax];
static CTextFilter bin;

CTextFilter::CTextFilter()
{
	setlocale(LC_CTYPE,"chs");
	ZeroMemory(m_NowSign,sizeof(m_NowSign));
}

CTextFilter::~CTextFilter()
{
}

bool CTextFilter::Add(const eFilterTable eTable, const char *szFilterText)
{
	if (!szFilterText) return false;
	if ( strlen(szFilterText)<=0 ) return false;
	m_FilterTable[eTable].push_back(*(new string(szFilterText)));
	for (int i=0;i<(int)strlen(szFilterText);i++)
	{
		BYTE j=szFilterText[i]/32;
		int n=(i+j)%8;
		m_NowSign[eTable][n]+=j+i;
	}
	return true;
}

bool CTextFilter::IsLegalText(const eFilterTable eTable, const string strText)
{
	vector <string>::iterator iter;
	for (iter=m_FilterTable[eTable].begin();iter!=m_FilterTable[eTable].end();iter++)
	{
		if (!bCheckLegalText(strText, &(*iter)))
		{
			return false;
		}
	}
	return true;
}

bool CTextFilter::Filter(const eFilterTable eTable, string &strText)
{
	bool ret=false;
	vector <string>::iterator iter;
	for (iter=m_FilterTable[eTable].begin();iter!=m_FilterTable[eTable].end();iter++)
	{
		if (ReplaceText(strText, &(*iter)))
		{
			ret=true;
		}
	}
	return ret;
}

bool CTextFilter::ReplaceText(string &strText, const string *pstrFilterText)
{
	bool ret=false;
	size_t nPos=strText.find(*pstrFilterText);
	static const basic_string <char>::size_type errPos = std::string::npos;
	while (nPos!=errPos)
	{

		if ( _ismbslead((unsigned char*)pstrFilterText->c_str(),(unsigned char*)pstrFilterText->c_str()) ==
			_ismbslead((unsigned char*)strText.c_str(),(unsigned char*)&strText[nPos]))
		{
			strText.replace(nPos,pstrFilterText->length(),pstrFilterText->length(),'*');
			ret=true;
		}
		else
		{
			nPos++;
		}
		nPos=strText.find(*pstrFilterText,nPos+pstrFilterText->length());
	}
	return ret;
}

bool CTextFilter::bCheckLegalText(const string &strText, const string *pstrIllegalText)
{
	size_t nPos=strText.find(*pstrIllegalText);
	static const basic_string <char>::size_type errPos = std::string::npos;
	while (nPos!=errPos)
	{
		if (  _ismbslead((unsigned char*)pstrIllegalText->c_str(),(unsigned char*)pstrIllegalText->c_str()) ==
			_ismbslead((unsigned char*)strText.c_str(),(unsigned char*)&strText[nPos]))
		{
			return false;
		}
		else
		{
			nPos++;
		}
		nPos=strText.find(*pstrIllegalText,nPos+pstrIllegalText->length());
	}
	return true;
}

bool CTextFilter::LoadFile(const char *szFileName, const eFilterTable eTable)
{
	if (!szFileName) return false;
	ifstream filterTxt(szFileName,ios::in);
	if (!filterTxt.is_open()) return false;
	char buf[500]={0};
	filterTxt.getline(buf,500);
	while (!filterTxt.fail())
	{
		char *pText=new char[strlen(buf)+2];
		// modify by lark.li 20080424 begin
		//TODO
		strcpy(pText,buf);
		//strcpy(pText,ConvertResString(buf));
		// End
		m_FilterTable[eTable].push_back(pText);
		filterTxt.getline(buf,500);
	}
	filterTxt.close();
	return true;
}

BYTE* CTextFilter::GetNowSign(const eFilterTable eTable)
{
	return m_NowSign[eTable];
}

void String2Item(const char* pszData, SItemGrid* SGridCont) {
	const short csSubNum = 8 + enumITEMDBP_MAXNUM + defITEM_INSTANCE_ATTR_NUM_VER110 * 2 + 1;
	std::string strSubList[csSubNum];

	int sTCount = 0;
	Util_ResolveTextLine(pszData, strSubList, csSubNum, ',');
	SGridCont->sID = Str2Int(strSubList[sTCount++]);
	SGridCont->sNum = Str2Int(strSubList[sTCount++]);
	SGridCont->sEndure[0] = Str2Int(strSubList[sTCount++]);
	SGridCont->sEndure[1] = Str2Int(strSubList[sTCount++]);
	SGridCont->sEnergy[0] = Str2Int(strSubList[sTCount++]);
	SGridCont->sEnergy[1] = Str2Int(strSubList[sTCount++]);
	SGridCont->chForgeLv = Str2Int(strSubList[sTCount++]);


	for (int m = 0; m < enumITEMDBP_MAXNUM; m++)
	{
		SGridCont->SetDBParam(m, Str2Int(strSubList[sTCount++]));
	}
	if (Str2Int(strSubList[sTCount++]) > 0) // 
	{
		for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
		{
			SGridCont->sInstAttr[k][0] = Str2Int(strSubList[sTCount + k * 2]);
			SGridCont->sInstAttr[k][1] = Str2Int(strSubList[sTCount + k * 2 + 1]);
		}
	}
	else
		SGridCont->SetInstAttrInvalid();
}


//  :    
static std::vector<std::string_view> SplitView(std::string_view sv, char delim)
{
	std::vector<std::string_view> result;
	while (!sv.empty())
	{
		auto pos = sv.find(delim);
		if (pos == std::string_view::npos)
		{
			result.push_back(sv);
			break;
		}
		result.push_back(sv.substr(0, pos));
		sv.remove_prefix(pos + 1);
	}
	return result;
}

static int ParseInt(std::string_view sv)
{
	int value = 0;
	std::from_chars(sv.data(), sv.data() + sv.size(), value);
	return value;
}

static long ParseLong(std::string_view sv)
{
	long value = 0;
	std::from_chars(sv.data(), sv.data() + sv.size(), value);
	return value;
}

bool LookData2String(const stNetChangeChaPart &pLook, std::string &strData)
{
	int64_t checkSum = 0;
	strData.clear();
	strData.reserve(4096);

	// 
	strData += "112#";

	// TypeID, HairID
	strData += std::to_string(pLook.sTypeID);
	strData += ',';
	strData += std::to_string(pLook.sHairID);
	checkSum += pLook.sTypeID + pLook.sHairID;

	// 34  
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		const auto &item = pLook.SLink[i];
		strData += ';';

		// expiration, bItemTradable, bIsLock, sNeedLv, dwDBID, sID, sNum,
		// sEndure[0], sEndure[1], sEnergy[0], sEnergy[1], chForgeLv
		strData += std::to_string(item.expiration);
		strData += ','; strData += std::to_string(static_cast<int>(item.bItemTradable));
		strData += ','; strData += std::to_string(static_cast<int>(item.bIsLock));
		strData += ','; strData += std::to_string(item.sNeedLv);
		strData += ','; strData += std::to_string(item.dwDBID);
		strData += ','; strData += std::to_string(item.sID);
		strData += ','; strData += std::to_string(item.sNum);
		strData += ','; strData += std::to_string(item.sEndure[0]);
		strData += ','; strData += std::to_string(item.sEndure[1]);
		strData += ','; strData += std::to_string(item.sEnergy[0]);
		strData += ','; strData += std::to_string(item.sEnergy[1]);
		strData += ','; strData += std::to_string(static_cast<int>(item.chForgeLv));

		checkSum += item.bItemTradable + item.bIsLock + item.sNeedLv + item.dwDBID
			+ item.sID + item.sNum + item.sEndure[0] + item.sEndure[1]
			+ item.sEnergy[0] + item.sEnergy[1] + item.chForgeLv;

		// DB 
		for (int m = 0; m < enumITEMDBP_MAXNUM; m++)
		{
			strData += ','; strData += std::to_string(item.GetDBParam(m));
			checkSum += item.GetDBParam(m);
		}

		// Instance-
		if (item.IsInstAttrValid())
		{
			strData += ",1";
			for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
			{
				strData += ','; strData += std::to_string(item.sInstAttr[k][0]);
				strData += ','; strData += std::to_string(item.sInstAttr[k][1]);
				checkSum += item.sInstAttr[k][0] + item.sInstAttr[k][1];
			}
		}
		else
		{
			strData += ",0";
		}
	}

	//  
	strData += ';';
	strData += std::to_string(checkSum);

	return true;
}

bool String2LookData(stNetChangeChaPart &pLook, const std::string &strData)
{
	if (strData.empty())
		return false;

	int64_t checkSum = 0;

	//  : "version#data"   "data"
	int version = 0;
	std::string_view dataView(strData);
	auto hashPos = dataView.find('#');
	if (hashPos != std::string_view::npos)
	{
		version = ParseInt(dataView.substr(0, hashPos));
		dataView.remove_prefix(hashPos + 1);
	}

	//   ';': section[0]=header, section[1..34]=slots, section[35]=checksum
	auto sections = SplitView(dataView, ';');

	//  F# : "typeID;hairID;faceID" (2-3 ,    header)
	if (sections.size() < static_cast<size_t>(enumEQUIP_NUM) + 1)
	{
		if (sections.size() >= 2 && sections[0].find(',') == std::string_view::npos)
		{
			pLook.sTypeID = static_cast<short>(ParseInt(sections[0]));
			pLook.sHairID = static_cast<short>(ParseInt(sections[1]));
			for (auto &item : pLook.SLink)
				item = SItemGrid();
			return true;
		}
		return false;
	}

	// Section 0: "typeID,hairID"
	auto headerFields = SplitView(sections[0], ',');
	if (headerFields.size() < 2)
		return false;
	pLook.sTypeID = static_cast<short>(ParseInt(headerFields[0]));
	pLook.sHairID = static_cast<short>(ParseInt(headerFields[1]));
	checkSum += pLook.sTypeID + pLook.sHairID;

	// Sections 1..34:  
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		auto fields = SplitView(sections[i + 1], ',');
		int idx = 0;
		auto &item = pLook.SLink[i];

		auto nextInt = [&]() -> int {
			return (idx < static_cast<int>(fields.size())) ? ParseInt(fields[idx++]) : 0;
		};
		auto nextLong = [&]() -> long {
			return (idx < static_cast<int>(fields.size())) ? ParseLong(fields[idx++]) : 0L;
		};

		item.expiration = nextLong();
		item.bItemTradable = nextInt();

		if (version == 112)
		{
			item.bIsLock = nextInt();
			item.sNeedLv = nextInt();
			item.dwDBID = nextInt();
		}

		item.sID = static_cast<short>(nextInt());
		item.sNum = static_cast<short>(nextInt());
		item.sEndure[0] = static_cast<short>(nextInt());
		item.sEndure[1] = static_cast<short>(nextInt());
		item.sEnergy[0] = static_cast<short>(nextInt());
		item.sEnergy[1] = static_cast<short>(nextInt());
		item.chForgeLv = static_cast<char>(nextInt());

		if (version == 112)
		{
			checkSum += item.bIsLock + item.sNeedLv + item.dwDBID
				+ item.sID + item.sNum + item.sEndure[0] + item.sEndure[1]
				+ item.sEnergy[0] + item.sEnergy[1] + item.chForgeLv;
		}
		else
		{
			checkSum += item.sID + item.sNum + item.sEndure[0] + item.sEndure[1]
				+ item.sEnergy[0] + item.sEnergy[1] + item.chForgeLv;
		}
		checkSum += item.bItemTradable;

		// DB 
		for (int m = 0; m < enumITEMDBP_MAXNUM; m++)
		{
			item.SetDBParam(m, nextLong());
			checkSum += item.GetDBParam(m);
		}

		// Instance-
		bool hasInstAttr = version >= defLOOK_CUR_VER && hashPos != std::string_view::npos;
		if (hasInstAttr)
		{
			if (nextInt() > 0)
			{
				for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
				{
					item.sInstAttr[k][0] = static_cast<short>(nextInt());
					item.sInstAttr[k][1] = static_cast<short>(nextInt());
					checkSum += item.sInstAttr[k][0] + item.sInstAttr[k][1];
				}
			}
			else
			{
				item.SetInstAttrInvalid();
			}
		}
		else
		{
			for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
			{
				item.sInstAttr[k][0] = static_cast<short>(nextInt());
				item.sInstAttr[k][1] = static_cast<short>(nextInt());
				checkSum += item.sInstAttr[k][0] + item.sInstAttr[k][1];
			}
		}
	}

	//    (     )
	if (hashPos != std::string_view::npos)
	{
		auto checksumIdx = static_cast<size_t>(enumEQUIP_NUM) + 1;
		if (checksumIdx < sections.size())
		{
			int64_t expected = 0;
			std::from_chars(sections[checksumIdx].data(),
				sections[checksumIdx].data() + sections[checksumIdx].size(), expected);
			if (expected != checkSum)
				return false;
		}
	}
	else
	{
		pLook.sVer = defLOOK_CUR_VER;
	}

	return true;
}

// 
char* ShortcutData2String(const stNetShortCut *pShortcut, char *szShortcutBuf, int nLen)
{
	if (!pShortcut || !szShortcutBuf) return NULL;

	char	szData[512];
	int		nBufLen = 0, nDataLen;
	szShortcutBuf[0] = '\0';

	for (int i = 0; i < SHORT_CUT_NUM; i++)
	{
		sprintf(szData, "%d,%d;", pShortcut->chType[i], pShortcut->byGridID[i]);
		nDataLen = (int)strlen(szData);
		if (nBufLen + nDataLen >= nLen) return NULL;
		strcat(szShortcutBuf, szData);
		nBufLen += nDataLen;
	}

	return szShortcutBuf;
}

// 
bool String2ShortcutData(stNetShortCut *pShortcut, std::string &strData)
{
	if (!pShortcut)
		return false;

	std::string strList[SHORT_CUT_NUM + 1];
	const short csSubNum = 2;
	std::string strSubList[csSubNum];
	Util_ResolveTextLine(strData.c_str(), strList, SHORT_CUT_NUM + 1, ';');
	for (int i = 0; i < SHORT_CUT_NUM; i++)
	{
		Util_ResolveTextLine(strList[i].c_str(), strSubList, csSubNum, ',');
		pShortcut->chType[i] = Str2Int(strSubList[0]);
		pShortcut->byGridID[i] = Str2Int(strSubList[1]);
	}

	return true;
}

bool KitbagStringConv(short sKbCapacity, std::string &strData)
{
	int	nInsertPos = 0;
	if (strData == "")
		return true;
	if ((int)strlen(strData.c_str()) < nInsertPos)
		return true;
	char szCap[10];
	sprintf(szCap, "%d@", sKbCapacity);
	strData.insert(nInsertPos, szCap);
	return true;
}
