//======================================================================================================================
// FileName: CharacterRecord.cpp
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CChaRecordSet class
//======================================================================================================================

#include "CharacterRecord.h"
#include <memory.h>
using namespace std;
//---------------------------------------------------------------------------
// class CChaRecord
//---------------------------------------------------------------------------
void CChaRecord::RefreshPrivateData()
{
	_HaveEffectFog = false;
	for( int i=0; i<defCHA_HP_EFFECT_NUM; i++ )
	{
		if( _nHPEffect[i] != 0 ) 
		{
			_HaveEffectFog = true;
			break;
		}
	}
}

//---------------------------------------------------------------------------
// class CChaRecordSet
//---------------------------------------------------------------------------
CChaRecordSet * CChaRecordSet::_Instance = NULL;

BOOL CChaRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{
	if(ParamList.size()==0) return FALSE;

	CChaRecord *pInfo = (CChaRecord*)pRawDataInfo;

	pInfo->lID = pInfo->nID;
	_tcsncpy(pInfo->szName, pInfo->szDataName, defCHA_NAME_LEN);
	pInfo->szName[defCHA_NAME_LEN - 1] = _TEXT('\0');

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// ͷ��ͼ������
	_tcsncpy(pInfo->szIconName, ParamList[m++].c_str(), defCHA_ICON_NAME_LEN);
	pInfo->szIconName[defCHA_ICON_NAME_LEN - 1] = _TEXT('\0');
	// ģ������
	pInfo->chModalType = Str2Int(ParamList[m++]);
	// ��������
	pInfo->chCtrlType = Str2Int(ParamList[m++]);
	// �Ǽܺ�
	pInfo->sModel = Str2Int(ParamList[m++]);
	// �׺�
	pInfo->sSuitID = Str2Int(ParamList[m++]);
	// ��װ����
	pInfo->sSuitNum = Str2Int(ParamList[m++]);
	// Ƥ��
	memset(pInfo->sSkinInfo, cchChaRecordKeyValue, sizeof(pInfo->sSkinInfo));
	for (int i = 0; i < defCHA_SKIN_NUM; i++)
	{
		pInfo->sSkinInfo[i] = Str2Int(ParamList[m++]);
	}
	//// FeffID
	//pInfo->sFeffID = Str2Int(ParamList[m++]);
	// //EeffID
	//pInfo->sEeffID = Str2Int(ParamList[m++]);
	string lstr[4]; 
	strLine = ParamList[m++];

	n = Util_ResolveTextLine(strLine.c_str(), lstr, 4, ',');
	memset(pInfo->sFeffID,0 ,sizeof(pInfo->sFeffID));
	for (int e = 0; e < n; e++)
	{
		pInfo->sFeffID[e] = Str2Int(lstr[e]);
	}
	pInfo->sEeffID = n;
	m++;

	// ��Ч�������
	memset(pInfo->sEffectActionID, 0, sizeof(pInfo->sEffectActionID));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine( strLine.c_str(), strList, 80, ',' );
	int nCount = 
	n = n > 3 ? 3 : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->sEffectActionID[i] = Str2Int(strList[i]);
	}
	// Ӱ��
	pInfo->sShadow = Str2Int(ParamList[m++]);
    // �������
    pInfo->sActionID = Str2Int(ParamList[m++]);    
    // ͸����
    pInfo->chDiaphaneity = Str2Int(ParamList[m++]);
	// ������Ч
	pInfo->sFootfall = Str2Int(ParamList[m++]);
	// ��Ϣ��Ч
	pInfo->sWhoop = Str2Int(ParamList[m++]);
	// ������Ч
	pInfo->sDirge = Str2Int(ParamList[m++]);
	// �Ƿ�ɿ�
	pInfo->chControlAble = Str2Int(ParamList[m++]);
	//// �ɷ��ƶ�
	//pInfo->chMoveAble = Str2Int(ParamList[m++]);
	// ��������
	pInfo->chTerritory = Str2Int(ParamList[m++]);
	// �߶�ƫ��
	pInfo->sSeaHeight = Str2Int(ParamList[m++]);	
	// ��װ������
	memset(pInfo->sItemType, cchChaRecordKeyValue, sizeof(pInfo->sItemType));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_ITEM_KIND_NUM ? defCHA_ITEM_KIND_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->sItemType[i] = Str2Int(strList[i]);
	}
	// �����ף�
	pInfo->fLengh = Str2Float(ParamList[m++]);
	// �����ף�
	pInfo->fWidth = Str2Float(ParamList[m++]);
	// �ߣ��ף�
	pInfo->fHeight = Str2Float(ParamList[m++]);
	// �ߣ��ף�
	pInfo->sRadii = Str2Int(ParamList[m++]);

	// ��������
	memset( pInfo->nBirthBehave, 0, sizeof(pInfo->nBirthBehave) );
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_BIRTH_EFFECT_NUM ? defCHA_BIRTH_EFFECT_NUM : n;	
	for (int i = 0; i < n; i++)
	{		
		pInfo->nBirthBehave[i] = Str2Int(strList[i]);
	}

	// ��������
	memset( pInfo->nDiedBehave, 0, sizeof(pInfo->nDiedBehave) );
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_DIE_EFFECT_NUM ? defCHA_DIE_EFFECT_NUM : n;	
	for (int i = 0; i < n; i++)
	{		
		pInfo->nDiedBehave[i] = Str2Int(strList[i]);
	}

    // ������Ч
    pInfo->sBornEff = Str2Int(ParamList[m++]);

    // ������Ч
    pInfo->sDieEff = Str2Int(ParamList[m++]);

	// ���߶���
    pInfo->sDormancy = Str2Int(ParamList[m++]);

    // ����ʱ�Ķ���
    pInfo->chDieAction = Str2Int(ParamList[m++]);

	// ʣ��hp��Ч����
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_HP_EFFECT_NUM ? defCHA_HP_EFFECT_NUM : n;
	memset( pInfo->_nHPEffect, 0, sizeof(pInfo->_nHPEffect) );
	for (int i = 0; i < n; i++)
	{
		pInfo->_nHPEffect[i] = Str2Int(strList[i]);
	}

	// �Ƿ����ת
	pInfo->_IsFace = Str2Int(ParamList[m++]) != 0 ? true : false;

	// �Ƿ�ɱ�쫷紵��
	pInfo->_IsCyclone = Str2Int(ParamList[m++]) != 0 ? true : false;

	// �ű��ļ�ID
	pInfo->lScript = Str2Int(ParamList[m++]);
	// ���߱��е�����ID
	pInfo->lWeapon = Str2Int(ParamList[m++]);

	// ���ܼ�ʹ��Ƶ��
	memset(pInfo->lSkill, cchChaRecordKeyValue, sizeof(pInfo->lSkill));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_INIT_SKILL_NUM ? defCHA_INIT_SKILL_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lSkill[i][0] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_INIT_SKILL_NUM ? defCHA_INIT_SKILL_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lSkill[i][1] = Str2Int(strList[i]);
	}

	// ��Ʒ�����ϼ���
	for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++)
	{
		pInfo->lItem[i][0] = cchChaRecordKeyValue;
		pInfo->lItem[i][1] = cchChaRecordKeyValue;
	}
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 160, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lItem[i][0] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 160, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lItem[i][1] = Str2Int(strList[i]);
	}

	// ÿ����౬����Ʒ����
	pInfo->lMaxShowItem = Str2Int(ParamList[m++]);
	// �򱬼���
	pInfo->fAllShow = Str2Float(ParamList[m++]);
	// ǰ׺�ȼ�
	pInfo->lPrefix = Str2Int(ParamList[m++]);

	// ������Ʒ�����ϼ���
	for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++)
	{
		pInfo->lTaskItem[i][0] = cchChaRecordKeyValue;
		pInfo->lTaskItem[i][1] = cchChaRecordKeyValue;
	}
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 160, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lTaskItem[i][0] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 160, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lTaskItem[i][1] = Str2Int(strList[i]);
	}

	// AI���
	pInfo->lAiNo = Str2Int(ParamList[m++]);
	// ����ʱ�Ƿ�ת��
	pInfo->chCanTurn = Str2Int(ParamList[m++]);
	// ��Ұ��Χ�����ף�
	pInfo->lVision = Str2Int(ParamList[m++]);
	// ���з�Χ�����ף�
	pInfo->lNoise = Str2Int(ParamList[m++]);
	// ֱ�ӻ�õ�EXP
	pInfo->lGetEXP = Str2Int(ParamList[m++]);
	// �Ƿ��ܹ���Ӱ��
	pInfo->bLight = Str2Int(ParamList[m++]) ? true : false;

	// ��ͨexp
	pInfo->lMobexp = Str2Int(ParamList[m++]);
	// ��ɫ�ȼ�
	pInfo->lLv = Str2Int(ParamList[m++]);
	// ���HP
	pInfo->lMxHp = Str2Int(ParamList[m++]);
	// ��ǰHP
	pInfo->lHp = Str2Int(ParamList[m++]);
	// ���SP
	pInfo->lMxSp = Str2Int(ParamList[m++]);
	// ��ǰSP
	pInfo->lSp = Str2Int(ParamList[m++]);
	// ��С������
	pInfo->lMnAtk = Str2Int(ParamList[m++]);
	// ��󹥻���
	pInfo->lMxAtk = Str2Int(ParamList[m++]);
	// �����ֿ�
	pInfo->lPDef = Str2Int(ParamList[m++]);
	// ������
	pInfo->lDef = Str2Int(ParamList[m++]);
	// ������
	pInfo->lHit = Str2Int(ParamList[m++]);
	// ������
	pInfo->lFlee = Str2Int(ParamList[m++]);
	// ������
	pInfo->lCrt = Str2Int(ParamList[m++]);
	// Ѱ����
	pInfo->lMf = Str2Int(ParamList[m++]);
	// hp�ָ��ٶ�
	pInfo->lHRec = Str2Int(ParamList[m++]);
	// sp�ָ��ٶ�
	pInfo->lSRec = Str2Int(ParamList[m++]);
	// �������
	pInfo->lASpd = Str2Int(ParamList[m++]);
	// ��������
	pInfo->lADis = Str2Int(ParamList[m++]);
	// ׷����Χ
	pInfo->lCDis = Str2Int(ParamList[m++]);
	// �ƶ��ٶ�
	pInfo->lMSpd = Str2Int(ParamList[m++]);
	// ��Դ�ɼ��ٶ�
	pInfo->lCol = Str2Int(ParamList[m++]);
	// ����
	pInfo->lStr = Str2Int(ParamList[m++]);
	// ����
	pInfo->lAgi = Str2Int(ParamList[m++]);
	// רע
	pInfo->lDex = Str2Int(ParamList[m++]);
	// ����
	pInfo->lCon = Str2Int(ParamList[m++]);
	// ����
	pInfo->lSta = Str2Int(ParamList[m++]);
	// ����
	pInfo->lLuk = Str2Int(ParamList[m++]);
	// ������������
	pInfo->lLHandVal = Str2Int(ParamList[m++]);

	// �л�����
	_tcsncpy(pInfo->szGuild, ParamList[m++].c_str(), defCHA_GUILD_NAME_LEN);
	pInfo->szGuild[defCHA_GUILD_NAME_LEN - 1] = _TEXT('\0');
	// ��ɫ�ƺ�
	_tcsncpy(pInfo->szTitle, ParamList[m++].c_str(), defCHA_TITLE_NAME_LEN);
	pInfo->szGuild[defCHA_GUILD_NAME_LEN - 1] = _TEXT('\0');
	// ��ɫְҵ
	_tcsncpy(pInfo->szJob, ParamList[m++].c_str(), defCHA_JOB_NAME_LEN);
	pInfo->szGuild[defCHA_GUILD_NAME_LEN - 1] = _TEXT('\0');

	// ��ǰ����
	pInfo->lCExp = Str2Int(ParamList[m++].c_str());
	// ��һ�����辭��
	pInfo->lNExp = Str2Int(ParamList[m++].c_str());
	// ����
	pInfo->lFame = Str2Int(ParamList[m++]);
	// ���Ե�
	pInfo->lAp = Str2Int(ParamList[m++]);
	// ���ܵ�
	pInfo->lTp = Str2Int(ParamList[m++]);
	// ��Ǯ
	pInfo->lGd = Str2Int(ParamList[m++]);
	// �ڵ������ٶ�
	pInfo->lSpri = Str2Int(ParamList[m++]);
	// ����(����)����
	pInfo->lStor = Str2Int(ParamList[m++]);
	// ��Ա����
	pInfo->lMxSail = Str2Int(ParamList[m++]);
	// ���д�Ա������
	pInfo->lSail = Str2Int(ParamList[m++]);
	// ��׼��Ա��������
	pInfo->lStasa = Str2Int(ParamList[m++]);
	// ��������
	pInfo->lScsm = Str2Int(ParamList[m++]);

	// ��ʱ����
	pInfo->lTStr = Str2Int(ParamList[m++]);
	// ��ʱ����
	pInfo->lTAgi = Str2Int(ParamList[m++]);
	// ��ʱרע
	pInfo->lTDex = Str2Int(ParamList[m++]);
	// ��ʱ����
	pInfo->lTCon = Str2Int(ParamList[m++]);
	// ��ʱ����
	pInfo->lTSta = Str2Int(ParamList[m++]);
	// ��ʱ����
	pInfo->lTLuk = Str2Int(ParamList[m++]);
	// ��ʱ���Ѫ��
	pInfo->lTMxHp = Str2Int(ParamList[m++]);
	// ��ʱ�����ֵ
	pInfo->lTMxSp = Str2Int(ParamList[m++]);
	// ��ʱ������
	pInfo->lTAtk = Str2Int(ParamList[m++]);
	// ��ʱ������
	pInfo->lTDef = Str2Int(ParamList[m++]);
	// ��ʱ������
	pInfo->lTHit = Str2Int(ParamList[m++]);
	// ��ʱ������
	pInfo->lTFlee = Str2Int(ParamList[m++]);
	// ��ʱѰ����
	pInfo->lTMf = Str2Int(ParamList[m++]);
	// ��ʱ������
	pInfo->lTCrt = Str2Int(ParamList[m++]);
	// ��ʱhp�ָ��ٶ�
	pInfo->lTHRec = Str2Int(ParamList[m++]);
	// ��ʱsp�ָ��ٶ�
	pInfo->lTSRec = Str2Int(ParamList[m++]);
	// ��ʱ�������
	pInfo->lTASpd = Str2Int(ParamList[m++]);
	// ��ʱ��������
	pInfo->lTADis = Str2Int(ParamList[m++]);
	// ��ʱ�ƶ��ٶ�
	pInfo->lTSpd = Str2Int(ParamList[m++]);
	// ��ʱ�����ٶ�
	pInfo->lTSpri = Str2Int(ParamList[m++]);
	// ��ʱ��������
	pInfo->lTScsm = Str2Int(ParamList[m++]);

	// ������Ϣ
	memset(pInfo->scaling, 0, sizeof(pInfo->scaling));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 3 ? 3 : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->scaling[i] = Str2Float(strList[i]);
	}

	return TRUE;
}

void CChaRecordSet::_ProcessRawDataInfo(CRawDataInfo *pInfo)
{
	CChaRecord *pChaInfo = (CChaRecord*)pInfo;

    // ���¶�̬����
	pChaInfo->RefreshPrivateData();	
}