#include "stdafx.h"
#include "Character.h"
#include "Player.h"
#include "GameDB.h"
#include "ChaAttrType.h"
#include "SubMap.h"
#include "Config.h"
#include "Guild.h"
#include "CommFunc.h"
#include "lua_gamectrl.h"

using namespace std;

char	szDBLog[256] = "DBData";
//-------------------
// �����Ƿ���ͬ����ɫ
//-------------------
BOOL CTableCha::VerifyName(const char* pszName)
{
	string buf[1];
	char param[] = "atorNome";
	char filter[80]; sprintf(filter, "atorNome='%s'", pszName);
	bool ret = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();
	if (ret && r1 > 0)
	{
		return TRUE;
	}
	return FALSE;
}

std::string CTableCha::GetName(int cha_id) 
{
	string buf[1];
	const auto param = "atorNome";
	char filter[80]; 
	sprintf(filter, "atorID = %d", cha_id);
	bool ret = _get_row(buf, 1, param, filter);
	return ret ? buf[0] : "";
}

#define defKITBAG_DATA_STRING_LEN	8192
#define defSKILLBAG_DATA_STRING_LEN	1500
#define defSHORTCUT_DATA_STRING_LEN	1500
#define defSSTATE_DATE_STRING_LIN	1024

const int g_cnCol = 64;
string g_buf[g_cnCol];
char g_sql[1024 * 1024]{};
char g_kitbag[defKITBAG_DATA_STRING_LEN] = {};
char g_kitbagTmp[defKITBAG_DATA_STRING_LEN] = {};
char g_equip[defKITBAG_DATA_STRING_LEN] = {};
char g_look[defLOOK_DATA_STRING_LEN] = {};
char g_skillbag[defSKILLBAG_DATA_STRING_LEN] = {};
char g_shortcut[defSHORTCUT_DATA_STRING_LEN] = {};
char g_skillstate[defSSTATE_DATE_STRING_LIN] = {};

// Add by lark.li 20080723 begin
char g_extendAttr[ROLE_MAXSIZE_DBMISCOUNT];
// End

// ����������Ϣ�洢
char g_szMisInfo[ROLE_MAXSIZE_DBMISSION];
char g_szRecord[ROLE_MAXSIZE_DBRECORD];
char g_szTrigger[ROLE_MAXSIZE_DBTRIGGER];
char g_szMisCount[ROLE_MAXSIZE_DBMISCOUNT];

bool CTableMaster::Init(void)
{
	sprintf(g_sql, "select \
				   cha_id1, cha_id2, finish, relation\
				   from %s \
				   (nolock) where 1=2", \
				   _get_table());

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(master)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "master");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

unsigned long CTableMaster::GetMasterDBID(CPlayer *pPlayer)
{
	if(!pPlayer)
	{
		return 0;
	}
	CCharacter *pCha = pPlayer->GetMainCha();
	
	if (!pCha)
	{
		return 0;
	}

	int nIndex = 0;
	char param[] = "cha_id2";
	char filter[80]; sprintf(filter, "cha_id1=%d", pPlayer->GetDBChaId());
	int r = _get_row(g_buf, g_cnCol, param, filter);
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		return (unsigned long)(Str2Int(g_buf[nIndex++]));
	}
	else
	{
		return (unsigned long)0;
	}
}

bool CTableMaster::IsMasterRelation(int masterID, int prenticeID)
{
	char param[] = "count(*)";
	char filter[80]; 
	sprintf(filter, "(cha_id1=%d) and (cha_id2=%d)", prenticeID, masterID);
	int r = _get_row(g_buf, g_cnCol, param, filter);
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		if(Str2Int(g_buf[0]) > 0)
			return true;
	}
	
	return false;
}

bool CTableCha::Init(void)
{
	sprintf(g_sql, "select \
				atorID, atorNome, motto, icon, version, pk_ctrl, endeMem, ato_id, guild_id, guild_stat, guild_permission, job, degree, exp, \
				hp, sp, ap, tp, bomd, str, dex, agi, con, sta, luk, sail_lv, sail_exp, sail_left_exp, live_lv, live_exp, map, main_map, map_x, map_y, radius, \
				angle, olhe, skillbag, shortcut, mission, misrecord, mistrigger, miscount, birth, login_cha, live_tp, bank, \
				delflag, operdate, skill_state, kitbag, kitbag_tmp, kb_locked, credit, store_item \
				from %s \
				(nolock) where 1=2", \
				_get_table());
	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(character)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "character");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

bool CTableCha::ShowExpRank(CCharacter* pCha, int count)
{
	bool ret = false;


	const char *sql_syntax = "select top %d atorNome,job,degree from %s where delflag =0 ORDER BY CASE WHEN (exp < 0) THEN (exp+4294967296) ELSE exp END DESC";
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax, count, _get_table());

	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try
	{
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO))
		{
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret);

			throw 1;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR *)sql, SQL_NTS);
		if (sqlret != SQL_SUCCESS)
		{
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);

			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		// Bind Column
		for (int i = 0; i < col_num; ++i)
		{
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
		}

		WPACKET		l_wpk = GETWPACKET();
		WRITE_CMD(l_wpk, CMD_MC_RANK);


		int f_row = 0;
		for (; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO; ++f_row)
		{
			if (sqlret != SQL_SUCCESS)
			{
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			}

			WRITE_STRING(l_wpk, (char const *)_buf[0]);	//name
			WRITE_STRING(l_wpk, (char const *)_buf[1]);		//job
			WRITE_SHORT(l_wpk, atol((char const *)_buf[2]));		//lv
		}

		WRITE_SHORT(l_wpk, f_row);
		pCha->ReflectINFof(pCha, l_wpk);

		SQLFreeStmt(hstmt, SQL_UNBIND);
		ret = true;
	}
	catch (int&e)
	{
		LG("consortia system", "consult apply consortia process memeberODBC interface transfer error,position ID:%d\n", e);
	}
	catch (...)
	{
		LG("consortia system", "Unknown Exception raised when list rank\n");
	}

	if (hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}

//-----------------------
// ��ɫ������Ϸʱ��ȡ���� 
//-----------------------
bool CTableCha::ReadAllData(CPlayer *pPlayer, DWORD atorID)
{
	if(!pPlayer)
	{
		//LG("enter_map", "�����ݿ����PlayerΪ��.\n");
		LG("enter_map", "Loading database error��Player is empty.\n");
		return false;
	}
	CCharacter *pCha = pPlayer->GetMainCha();
	if (!pCha || (pPlayer->GetDBChaId() != atorID))
	{
		//LG("enter_map", "�����ݿ��������ɫ�����ڻ�ƥ��.\n");
		LG("enter_map", "Loading database error,the Main character is inexistence or not matching.\n");
		return false;
	}

	int nIndex = 0;
	char param[] = "ato_id, guild_stat, guild_id, hp, sp, exp, radius, angle, atorNome, motto, icon, version, pk_ctrl, degree, job, bomd, ap, tp, str, dex, agi, con, sta, luk, sail_lv, sail_exp, sail_left_exp, live_lv, live_exp, live_tp, main_map, map_x, map_y, birth, olhe, skillbag, shortcut, mission, misrecord, mistrigger, miscount, login_cha, kitbag, kitbag_tmp, map_mask, skill_state, bank, kb_locked, credit, store_item, extend, guild_permission, chatColour, IMP";
	char filter[80]; sprintf(filter, "atorID=%d", atorID);
	int r = _get_row(g_buf, g_cnCol, param, filter);
	int	r1 = get_affected_rows();
	//LG("enter_map", "�����ݿ�ɹ���_get_row.\n");
	LG("enter_map", "Loading database succeed��_get_row.\n");
	if (DBOK(r) && r1 > 0)
	{
		pPlayer->SetDBActId(Str2Int(g_buf[nIndex++]));
		pCha->SetGuildState( Str2Int(g_buf[nIndex++]) );
		pCha->SetGuildID( Str2Int(g_buf[nIndex++]) );
		//pCha->setAttr(ATTR_GUILD_STATE, Str2Int(g_buf[nIndex++]), 1);
		//pCha->setAttr(ATTR_GUILD, Str2Int(g_buf[nIndex++]), 1);
		//pCha->setAttr(ATTR_GUILD_TYPE, game_db.GetGuildTypeByID(pCha, pCha->getAttr(ATTR_GUILD)), 1);

		pCha->setAttr(ATTR_HP, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_SP, Str2Int(g_buf[nIndex++]), 1);
		//pCha->setAttr(ATTR_CEXP, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_CEXP, _atoi64(g_buf[nIndex++].c_str()), 1);

		pCha->SetRadius(Str2Int(g_buf[nIndex++]));
		pCha->SetAngle(Str2Int(g_buf[nIndex++]));
		pCha->SetName((char *)g_buf[nIndex++].c_str());
		Char szLogName[defLOG_NAME_LEN] = "";
		sprintf(szLogName, "Cha-%s+%u", pCha->GetName(), pCha->GetID());
		pCha->m_CLog.SetLogName(szLogName);

		pCha->SetMotto(g_buf[nIndex++].c_str());
		pCha->SetIcon(Str2Int(g_buf[nIndex++]));

		long lVer = Str2Int(g_buf[nIndex++]);
		if (pCha->getAttr(ATTR_HP) < 0) // �½�ɫ
			lVer = defCHA_TABLE_NEW_VER;
		pCha->SetPKCtrl(Str2Int(g_buf[nIndex++]));

		strcpy(pCha->m_CChaAttr.m_szName, pCha->GetName());
		
		pCha->setAttr(ATTR_LV, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_JOB, g_GetJobID(g_buf[nIndex++].c_str()), 1);
		pCha->setAttr(ATTR_GD, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_AP, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_TP, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_BSTR, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_BDEX, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_BAGI, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_BCON, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_BSTA, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_BLUK, Str2Int(g_buf[nIndex++]), 1);

		pCha->setAttr(ATTR_SAILLV, Str2Int(g_buf[nIndex++]), 1);
		//pCha->setAttr(ATTR_CSAILEXP, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_CSAILEXP, _atoi64(g_buf[nIndex++].c_str()), 1);
		pCha->setAttr(ATTR_CLEFT_SAILEXP, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_LIFELV, Str2Int(g_buf[nIndex++]), 1);
		//pCha->setAttr(ATTR_CLIFEEXP, Str2Int(g_buf[nIndex++]), 1);
		pCha->setAttr(ATTR_CLIFEEXP, _atoi64(g_buf[nIndex++].c_str()), 1);
		pCha->setAttr(ATTR_LIFETP, Str2Int(g_buf[nIndex++]), 1);

		pCha->SetBirthMap(g_buf[nIndex++].c_str());
		long lPosX = Str2Int(g_buf[nIndex++]);
		long lPosY = Str2Int(g_buf[nIndex++]);
		pCha->SetPos(lPosX, lPosY);
		pCha->SetBirthCity(g_buf[nIndex++].c_str());
		//LG("enter_map", "���ý�ɫ�������ݳɹ�.\n");

		try
		{
			int	nLookDataID = nIndex;
			if (!pCha->String2LookDate(g_buf[nIndex++]))
			{
				//LG("enter_map", "�������У��ʹ���.\n");
				LG("enter_map", "Appearance data check sum error.\n");
				//LG("У��ʹ���", "��ɫ��dbid %u��name %s��resid %u���ĸĹ�����У��ʹ���.\n", atorID, pCha->GetLogName(), pCha->GetKitbagRecDBID());
				LG("Check sum error", "the character (dbid %u��name %s��resid %u)'s change appearance data check sum error.\n", atorID, pCha->GetLogName(), pCha->GetKitbagRecDBID());
				return false;
			}
			pCha->SetCat(pCha->m_SChaPart.sTypeID);
			//LG("enter_map", "���ý�ɫ��۳ɹ�.\n");
		}
		catch (...)
		{
			//LG("enter_map", "Strin2LookData����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			LG("enter_map", "Strin2LookData error!!\n");
			//LG("enter_map", "����ַ��� %s\n", g_buf[nIndex - 1]);
			LG("enter_map", "Appearance String %s\n", g_buf[nIndex - 1].c_str());
			throw;
		}

		int	nSkillBagDataID = nIndex;
		String2SkillBagData(&pCha->m_CSkillBag, g_buf[nIndex++]);
		//LG("enter_map", "���ý�ɫ�������ɹ�.\n");

		int	nSortcutDataID = nIndex;
		String2ShortcutData(&pCha->m_CShortcut, g_buf[nIndex++]);
		//LG("enter_map", "���ý�ɫ������ɹ�.\n");

		// ��ȡ������Ϣ
		pPlayer->MisClear();
		memset( g_szMisInfo, 0, ROLE_MAXSIZE_DBMISSION );
		strncpy( g_szMisInfo, g_buf[nIndex++].c_str(), ROLE_MAXSIZE_DBMISSION - 1 );
		if( !pPlayer->MisInit( g_szMisInfo ) )
		{
			//pCha->SystemNotice( "�ý�ɫ�����¼������Ϣ��ʼ��ʧ��!" );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00009) );

		}
		//LG("enter_map", "���ý�ɫ����1�ɹ�.\n");

		memset( g_szRecord, 0, ROLE_MAXSIZE_DBRECORD );
		strncpy( g_szRecord, g_buf[nIndex++].c_str(), ROLE_MAXSIZE_DBRECORD - 1 );
		if( !pPlayer->MisInitRecord( g_szRecord ) )
		{
			//pCha->SystemNotice( "�ý�ɫ������ʷ��¼������Ϣ��ʼ��ʧ��!" );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00010) );
		}
		//LG("enter_map", "���ý�ɫ����2�ɹ�.\n");

		memset( g_szTrigger, 0, ROLE_MAXSIZE_DBTRIGGER );
		strncpy( g_szTrigger, g_buf[nIndex++].c_str(), ROLE_MAXSIZE_DBTRIGGER - 1 );
		if( !pPlayer->MisInitTrigger( g_szTrigger ) )
		{
			//pCha->SystemNotice( "�ý�ɫ���񴥷������ݳ�ʼ��ʧ��!" );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00011) );
		}
		//LG("enter_map", "���ý�ɫ����3�ɹ�.\n");

		memset( g_szMisCount, 0, ROLE_MAXSIZE_DBMISCOUNT );
		strncpy( g_szMisCount, g_buf[nIndex++].c_str(), ROLE_MAXSIZE_DBMISCOUNT - 1 );
		if( !pPlayer->MisInitMissionCount( g_szMisCount ) )
		{
			//pCha->SystemNotice( "�ý�ɫ�������������ݳ�ʼ��ʧ��!" );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00012) );
		}
		//LG("enter_map", "���ý�ɫ����4�ɹ�.\n");

		string strList[2];
		Util_ResolveTextLine(g_buf[nIndex++].c_str(), strList, 2, ',');
		pPlayer->SetLoginCha(Str2Int(strList[0]), Str2Int(strList[1]));

		//if (lVer != defCHA_TABLE_NEW_VER) // ��Ҫ�汾ת��
		//	SaveTableVer(atorID);

		pCha->SetKitbagRecDBID(Str2Int(g_buf[nIndex++]));
        
        pCha->SetKitbagTmpRecDBID(Str2Int(g_buf[nIndex++]));//��ȡ��ʱ��������ԴID
        
		pPlayer->SetMapMaskDBID(Str2Int(g_buf[nIndex++]));
		g_strChaState[0] = g_buf[nIndex++];
		pPlayer->Strin2BankDBIDData(g_buf[nIndex++]);

        //����������
        int iLocked = Str2Int(g_buf[nIndex++]);
        pCha->m_CKitbag.SetPwdLockState(iLocked);
        
		//����
		int nCredit = Str2Int(g_buf[nIndex++]);
		pCha->SetCredit(nCredit);

		pCha->SetStoreItemID(Str2Int(g_buf[nIndex++]));

		// Add by lark.li 20080723 begin
		Strin2ChaExtendAttr(pCha, g_buf[nIndex++]);
		
		
		pCha->guildPermission = stoull(g_buf[nIndex++]);// Str2Int(g_buf[nIndex++]);
		pCha->chatColour = Str2Int(g_buf[nIndex++]);
		pCha->SetIMP(Str2Int(g_buf[nIndex++]));
		// End

		//LG("enter_map", "����ȫ�����ݳɹ�.\n");
		LG("enter_map", "Set the whole data succeed.\n");	
}
	else
	{
		//LG("enter_map", "�����ݿ����_get_row()����ֵ��%d.%u\n", r);
		LG("enter_map", "Loading database error��_get_row() return value��%d.%u\n", r, r1);
		return false;
	}

	return true;
}

//-----------------
// ��ʱ��ɫ��Ϣ����
//-----------------
bool CTableCha::SaveAllData(CPlayer *pPlayer, char chSaveType)
{
	if(!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter *pCha = pPlayer->GetMainCha();
	if (!pCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	CCharacter *pCCtrlCha = pPlayer->GetCtrlCha();
	if (pPlayer->GetLoginChaType() == enumLOGIN_CHA_BOAT) // �Դ�����̬��½
	{
		CCharacter *pCLogCha = pPlayer->GetBoat(pPlayer->GetLoginChaID());
		if (pCLogCha != pCCtrlCha) // ��Ӧ�ó��ֵ����
		{
			pCCtrlCha->SetToMainCha();
			pCCtrlCha = pCha;
			if (pCLogCha)
				//LG("��½��ɫ���ƴ���", "��½��ɫ %s�����ƽ�ɫ %s������ɫ %s.\n", pCLogCha->GetLogName(), pCCtrlCha->GetLogName(), pCha->GetLogName());
				LG("logging character control error", "logging character %s,control character %s��Main character %s.\n", pCLogCha->GetLogName(), pCCtrlCha->GetLogName(), pCha->GetLogName());
			else
				//LG("��½��ɫ���ƴ���", "��½��ɫ %s�����ƽ�ɫ %s������ɫ %s.\n", "", pCCtrlCha->GetLogName(), pCha->GetLogName());
				LG("logging character control error", "logging character %s,control character %s��Main character %s.\n", "", pCCtrlCha->GetLogName(), pCha->GetLogName());
			return false;
		}
	}
	else
	{
		if (pCha != pCCtrlCha) // ��Ӧ�ó��ֵ����
		{
			pCCtrlCha = pCha;
			//LG("��½��ɫ���ƴ���", "��½��ɫ %s�����ƽ�ɫ %s������ɫ %s.\n", pCCtrlCha->GetLogName(), pCCtrlCha->GetLogName(), pCha->GetLogName());
			LG("logging character control error", "logging character %s,control character %s��Main character %s.\n", pCCtrlCha->GetLogName(), pCCtrlCha->GetLogName(), pCha->GetLogName());
			return false;
		}
	}

	if(pCha)
	{
		//LG("enter_map", "%s ��ʼ���ñ�������.\n", pCha->GetLogName());
		LG("enter_map", "%s start configure save data.\n", pCha->GetLogName());

		//pCha->m_CLog.Log("^^^^^^^^^^^^�㿪ʼ�����ɫ\n");
		pCha->m_CLog.Log("........... now you start save character\n");
		//pCha->m_CLog.Log("�ȼ� %d����ͼ %s������ [%d,%d]�������� %s.\n", pCha->m_CChaAttr.GetAttr(ATTR_LV), pCha->GetBirthMap(), pCha->GetPos().x, pCha->GetPos().y, pCha->GetBirthCity());
		pCha->m_CLog.Log("grade %d��map %s��coordinate [%d,%d]��birth city %s.\n", (int)pCha->m_CChaAttr.GetAttr(ATTR_LV), pCha->GetBirthMap(), pCha->GetPos().x, pCha->GetPos().y, pCha->GetBirthCity());
	}

	//char	szSaveCha[256] = "�����ɫ��ʱ";
	char szSaveCha[256];
	strncpy( szSaveCha, RES_STRING(GM_GAMEDB_CPP_00013), 256 - 1 );

	//char	szSaveChaFile[256] = "log\\�����ɫ��ʱ.log";
	char szSaveChaFile[256];
	strncpy( szSaveChaFile, RES_STRING(GM_GAMEDB_CPP_00014), 256 - 1 );

	char	szLogMsg[1024] = "";
	//FILE	*fp;
	//if (!(fp = fopen(szSaveChaFile, "r")))
	//	LG(szSaveCha, "����\t���\t���ܰ�\t�����\t����\t���SQL\tִ��SQL[����(�ֽ�)]\t�ܺ�ʱ\t��ɫ����\n");
	//if (fp)
	//	fclose(fp);
	DWORD	dwNowTick = GetTickCount();
	DWORD	dwOldTick;
	DWORD	dwTotalTick = 0;

	DWORD hp    = (long)pCha->getAttr(ATTR_HP);
	DWORD sp    = (long)pCha->getAttr(ATTR_SP);
	DWORD exp   = (long)pCha->getAttr(ATTR_CEXP);

	const char *map	= pCCtrlCha->GetBirthMap();
	const char *main_map = pCha->GetBirthMap();
	DWORD map_x = pCha->GetShape().centre.x;
	DWORD map_y = pCha->GetShape().centre.y;
	DWORD radius= pCha->GetShape().radius;
	short angle = pCha->GetAngle();
	short degree= (short)pCha->getAttr(ATTR_LV);
	const char *job	= g_GetJobName((short)pCha->getAttr(ATTR_JOB));
	DWORD bomd	= (long)pCha->getAttr(ATTR_GD);
	DWORD ap	= (long)pCha->getAttr(ATTR_AP);
	DWORD tp	= (long)pCha->getAttr(ATTR_TP);
	DWORD str	= (long)pCha->getAttr(ATTR_BSTR);
	DWORD dex	= (long)pCha->getAttr(ATTR_BDEX);
	DWORD agi	= (long)pCha->getAttr(ATTR_BAGI);
	DWORD con	= (long)pCha->getAttr(ATTR_BCON);
	DWORD sta	= (long)pCha->getAttr(ATTR_BSTA);
	DWORD luk	= (long)pCha->getAttr(ATTR_BLUK);

	DWORD sail_lv		= (long)pCha->getAttr(ATTR_SAILLV);
	DWORD sail_exp		= (long)pCha->getAttr(ATTR_CSAILEXP);
	DWORD sail_left_exp	= (long)pCha->getAttr(ATTR_CLEFT_SAILEXP);
	DWORD live_lv		= (long)pCha->getAttr(ATTR_LIFELV);
	DWORD live_exp		= (long)pCha->getAttr(ATTR_CLIFEEXP);
	DWORD live_tp		= (long)pCha->getAttr(ATTR_LIFETP);
    
    DWORD nLocked       = pCha->m_CKitbag.GetPwdLockState();

	DWORD dwCredit		= (long)pCha->GetCredit();
	DWORD dwStoreItemID = pCha->GetStoreItemID();

	int chaIMP			= pCha->GetIMP();

	char  pk_ctrl = pCha->IsInPK();
	dwOldTick = dwNowTick;
	dwNowTick = GetTickCount();
	dwTotalTick += dwNowTick - dwOldTick;
	sprintf(szLogMsg + strlen(szLogMsg), "%4u", dwNowTick - dwOldTick);
	//LG("enter_map", "���ý�ɫ�������ݳɹ�.\n");

	g_look[0] = 0;
	if(!LookData2String(&pCha->m_SChaPart, g_look, defLOOK_DATA_STRING_LEN, false))
	{
		//LG("enter_map", "��ɫ%s\t�������ݣ���ۣ�ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\tsave data (surface) error!\n", pCha->GetLogName());
		return false;
	}
	//LG("enter_map", "���ý�ɫ��۳ɹ�.\n");

	dwOldTick = dwNowTick;
	dwNowTick = GetTickCount();
	dwTotalTick += dwNowTick - dwOldTick;
	sprintf(szLogMsg + strlen(szLogMsg), "\t%4u", dwNowTick - dwOldTick);

	g_skillbag[0] = 0;
	if (!SkillBagData2String(&pCha->m_CSkillBag, g_skillbag, defSKILLBAG_DATA_STRING_LEN))
	{
		//LG("enter_map", "��ɫ%s\t�������ݣ����ܣ�ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\tsave data(skill) error!\n", pCha->GetLogName());
		return false;
	}
	//LG("enter_map", "���ý�ɫ�������ɹ�.\n");

	dwOldTick = dwNowTick;
	dwNowTick = GetTickCount();
	dwTotalTick += dwNowTick - dwOldTick;
	sprintf(szLogMsg + strlen(szLogMsg), "\t%4u", dwNowTick - dwOldTick);

	g_shortcut[0] = 0;
	if (!ShortcutData2String(&pCha->m_CShortcut, g_shortcut, defSHORTCUT_DATA_STRING_LEN))
	{
		//LG("enter_map", "��ɫ%s\t�������ݣ��������ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\tsave data(shortcut)error!\n", pCha->GetLogName());
		return false;
	}
	//LG("enter_map", "���ý�ɫ������ɹ�.\n");

	dwOldTick = dwNowTick;
	dwNowTick = GetTickCount();
	dwTotalTick += dwNowTick - dwOldTick;
	sprintf(szLogMsg + strlen(szLogMsg), "\t%4u", dwNowTick - dwOldTick);

	// �����ɫ�����¼��Ϣ
	memset( g_szMisInfo, 0, ROLE_MAXSIZE_DBMISSION );
	if( !pPlayer->MisGetData( g_szMisInfo, ROLE_MAXSIZE_DBMISSION - 1 ) )
	{
		//pCha->SystemNotice( "�ý�ɫ����������Ϣ���ݶ�ȡ����!ID = %d", pCha->GetID() );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00015), pCha->GetID() );
		//LG(szDBLog, "�����ɫ[ID: %d\tNAME: %s]������Ϣ����ȡ��������ʧ��!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		LG(szDBLog, "save character[ID: %d\tNAME: %s]data info,Get mission data error!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
	}
	//LG("enter_map", "���ý�ɫ����1�ɹ�.\n");

	memset( g_szRecord, 0, ROLE_MAXSIZE_DBRECORD );
	if( !pPlayer->MisGetRecord( g_szRecord, ROLE_MAXSIZE_DBRECORD - 1 ) )
	{
		//pCha->SystemNotice( "�ý�ɫ����������Ϣ���ݶ�ȡ����!ID = %d", pCha->GetID() );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00015), pCha->GetID() );
		//LG(szDBLog, "�����ɫ[ID: %d\tNAME: %s]������Ϣ����ȡ������ʷ��¼����ʧ��!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		LG(szDBLog, "save character[ID: %d\tNAME: %s]data info,Get mission history data error !ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
	}
	//LG("enter_map", "���ý�ɫ����2�ɹ�.\n");

	memset( g_szTrigger, 0, ROLE_MAXSIZE_DBTRIGGER );
	if( !pPlayer->MisGetTrigger( g_szTrigger, ROLE_MAXSIZE_DBTRIGGER - 1 ) )
	{
		//pCha->SystemNotice( "�ý�ɫ����������Ϣ���������ݶ�ȡ����!ID = %d", pCha->GetID() );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00016), pCha->GetID() );
		//LG(szDBLog, "�����ɫ[ID: %d\tNAME: %s]������Ϣ����ȡ���񴥷�������ʧ��!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		LG(szDBLog, "save character[ID: %d\tNAME: %s]data info��Get mission trigger data error!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
	}
	//LG("enter_map", "���ý�ɫ����3�ɹ�.\n");

	memset( g_szMisCount, 0, ROLE_MAXSIZE_DBMISCOUNT );
	if( !pPlayer->MisGetMissionCount( g_szMisCount, ROLE_MAXSIZE_DBMISCOUNT - 1 ) )
	{
		//pCha->SystemNotice( "�ý�ɫ����������������Ϣ���ݶ�ȡ����!ID = %d", pCha->GetID() );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00017), pCha->GetID() );
		//LG(szDBLog, "�����ɫ[ID: %d\tNAME: %s]������Ϣ����ȡ��������������ʧ��!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		LG(szDBLog, "save character[ID: %d\tNAME: %s]data info��Get randomicity mission take count of data error!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
	}
	//LG("enter_map", "���ý�ɫ����4�ɹ�.\n");

	const char	*szBirthName = pCha->GetBirthCity();
	//dwOldTick = dwNowTick;
	//dwNowTick = GetTickCount();
	//dwTotalTick += dwNowTick - dwOldTick;
	//sprintf(szLogMsg + strlen(szLogMsg), "\t%4u", dwNowTick - dwOldTick);

	char	szLoginCha[50];
	sprintf(szLoginCha, "%u,%u", pPlayer->GetLoginChaType(), pPlayer->GetLoginChaID());

	if (chSaveType == enumSAVE_TYPE_OFFLINE) // ����
	{
		SStateData2String(pCha, g_skillstate, defSSTATE_DATE_STRING_LIN, chSaveType);
	}
	else if (!SStateData2String(pCha, g_skillstate, defSSTATE_DATE_STRING_LIN, chSaveType))
	{
		LG("enter_map", "character %s\tsave data(shortcut)error!\n", pCha->GetLogName());
		return false;
	}

	// Add by lark.li 20080723 begin
	memset(g_extendAttr, 0 , ROLE_MAXSIZE_DBMISCOUNT);
	if(!ChaExtendAttr2String(pCha, g_extendAttr, ROLE_MAXSIZE_DBMISCOUNT))
	{
		LG("enter_map", "character %s\tsave data (extend attr) error!\n", pCha->GetLogName());
		return false;
	}

	// End
	char str_exp[32];
     _i64toa( exp, str_exp, 10 ); // C4996

	bool bWithPos = false;
	if (pCCtrlCha->GetSubMap())
		bWithPos = pCCtrlCha->GetSubMap()->CanSavePos();
	if (bWithPos)
		sprintf(g_sql, "update %s set \
					hp=%d, sp=%d, exp=%s, map='%s', main_map='%s', map_x=%d, map_y=%d, radius=%d, angle=%d, pk_ctrl=%d, degree=%d, job='%s', bomd=%d, ap=%d, tp=%d, str=%d, dex=%d, agi=%d, con=%d, sta=%d, luk=%d, olhe='%s', skillbag='%s', \
					shortcut='%s', mission='%s', misrecord='%s', mistrigger='%s', miscount='%s', birth='%s', login_cha='%s', \
					sail_lv=%d, sail_exp=%d, sail_left_exp=%d, live_lv=%d, live_exp=%d, live_tp=%d, kb_locked=%d, credit=%d, store_item=%d, skill_state='%s', extend ='%s', IMP = '%d' \
					where atorID=%d", \
					_get_table(), \
					hp, sp, str_exp, map, main_map, map_x, map_y, radius, angle, pk_ctrl, degree, job, bomd, ap, tp, str, dex, agi, con, sta, luk, g_look, g_skillbag, g_shortcut, g_szMisInfo, g_szRecord, g_szTrigger, g_szMisCount, szBirthName, szLoginCha, sail_lv, sail_exp, sail_left_exp, live_lv, live_exp, live_tp, nLocked, dwCredit, dwStoreItemID, g_skillstate, g_extendAttr, chaIMP, \
					atorID);
	else
		sprintf(g_sql, "update %s set \
					hp=%d, sp=%d, exp=%s, radius=%d, pk_ctrl=%d, degree=%d, job='%s', bomd=%d, ap=%d, tp=%d, str=%d, dex=%d, agi=%d, con=%d, sta=%d, luk=%d, olhe='%s', skillbag='%s', \
					shortcut='%s', mission='%s', misrecord='%s', mistrigger='%s', miscount='%s', birth='%s', login_cha='%s', \
					sail_lv=%d, sail_exp=%d, sail_left_exp=%d, live_lv=%d, live_exp=%d, live_tp=%d, kb_locked=%d, credit=%d, store_item=%d, skill_state='%s', extend ='%s', IMP = '%d' \
					where atorID=%d", \
					_get_table(), \
					hp, sp, str_exp, radius, pk_ctrl, degree, job, bomd, ap, tp, str, dex, agi, con, sta, luk, g_look, g_skillbag, g_shortcut, g_szMisInfo, g_szRecord, g_szTrigger, g_szMisCount, szBirthName, szLoginCha, sail_lv, sail_exp, sail_left_exp, live_lv, live_exp, live_tp, nLocked, dwCredit, dwStoreItemID, g_skillstate, g_extendAttr, chaIMP, \
					atorID);
	//LG("enter_map", "��֯SQL���ɹ�.\n");
	//dwOldTick = dwNowTick;
	//dwNowTick = GetTickCount();
	//dwTotalTick += dwNowTick - dwOldTick;
	//sprintf(szLogMsg + strlen(szLogMsg), "\t%7u", dwNowTick - dwOldTick);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//LG("enter_map", "��ɫ%s\tִ��SQL���ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\tcarry out SQL sentence error!\n", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ�������%u!\n", atorID);
		LG("enter_map", "Database couldn't find the character %u!\n", atorID);
		return false;
	}

	//game_db.UpdateIMP(pPlayer);

	//LG("enter_map", "ִ�б���SQL���ɹ�.\n");

	dwOldTick = dwNowTick;
	dwNowTick = GetTickCount();
	dwTotalTick += dwNowTick - dwOldTick;
	sprintf(szLogMsg + strlen(szLogMsg), "\t%7u[%10u]", dwNowTick - dwOldTick, (unsigned long)strlen(g_sql));
	sprintf(szLogMsg + strlen(szLogMsg), "\t%6u", dwTotalTick);
	sprintf(szLogMsg + strlen(szLogMsg), "\t%s\n", pCha->m_CLog.GetLogName());
	//LG(szSaveCha, szLogMsg);

	//pCha->m_CLog.Log("^^^^^^^^^^^^�㱣���ɫ���\n");
	pCha->m_CLog.Log("...............you finish save the character \n");
	//pCha->SystemNotice("��Ľ�ɫ�ɹ����浽���ݿ⣬�ȼ� %d����ͼ %s������ [%d,%d]�������� %s.\n", pCha->m_CChaAttr.GetAttr(ATTR_LV), pCha->GetBirthMap(), pCha->GetPos().x, pCha->GetPos().y, pCha->GetBirthCity());
	//LG("enter_map", "��������ɫȫ�����ݳɹ�.\n", pCha->GetLogName());
	LG("enter_map", "save the main character whole data succeed!\n", pCha->GetLogName());

	return true;
}

bool CTableCha::SavePos(CPlayer *pPlayer)
{
	if(!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter *pCha = pPlayer->GetMainCha();
	CCharacter *pCCtrlCha = pPlayer->GetCtrlCha();
	if (!pCha || !pCCtrlCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	sprintf(g_sql, "update %s set \
				map='%s', main_map='%s', map_x=%d, map_y=%d, angle=%d \
				where atorID=%d", \
				_get_table(), \
				pCCtrlCha->GetBirthMap(), pCha->GetBirthMap(), pCha->GetPos().x, pCha->GetPos().y, pCha->GetAngle(), \
				atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		//LG("enter_map", "��ɫ%s\tִ�б���λ�õ�SQL���ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\tcarry out save position SQL sentence error!\n", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ�������%u!\n", atorID);
		LG("enter_map", "Database couldn't find the character%u!\n", atorID);
		return false;
	}

	return true;
}

bool CTableCha::SaveMoney(CPlayer *pPlayer)
{
	if(!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter *pCha = pPlayer->GetMainCha();
	if (!pCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	sprintf(g_sql, "update %s set \
				bomd=%d \
				where atorID=%d", \
				_get_table(), \
				(int)pCha->getAttr(ATTR_GD), \
				atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		//LG("enter_map", "��ɫ%s\tִ�б����Ǯ��SQL���ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\tcarry out save money SQL sentence error!\n", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ�������%u!\n", atorID);
		LG("enter_map", "Database couldn't find the character%u!\n", atorID);
		return false;
	}

	return true;
}

bool CTableCha::SaveKBagDBID(CPlayer *pPlayer)
{
	if(!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter *pCha = pPlayer->GetMainCha();
	if (!pCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	sprintf(g_sql, "update %s set \
				kitbag=%d \
				where atorID=%d", \
				_get_table(), \
				pCha->GetKitbagRecDBID(), \
				atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		//LG("enter_map", "��ɫ%s\tִ�б��汳��������SQL���ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character%s\tcarry out save kitbag indexical SQL sentence error!\n", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ�������%u!\n", atorID);
		LG("enter_map", "Database couldn't find the character%u!\n", atorID);
		return false;
	}

	return true;
}

bool CTableCha::SaveKBagTmpDBID(CPlayer *pPlayer)
{
    if(!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter *pCha = pPlayer->GetMainCha();
	if (!pCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	sprintf(g_sql, "update %s set \
				kitbag_tmp=%d \
				where atorID=%d", \
				_get_table(), \
				pCha->GetKitbagTmpRecDBID(), \
				atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		//LG("enter_map", "��ɫ%s\tִ�б�����ʱ����������SQL���ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\tcarry out save temp kitbag indexical SQL sentence error!\n", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ�������%u!\n", atorID);
		LG("enter_map", "Database couldn't find the character%u!\n", atorID);
		return false;
	}

	return true;
}

bool CTableCha::SaveKBState(CPlayer *pPlayer)
{
    if(!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter *pCha = pPlayer->GetMainCha();
	if (!pCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

    int iLocked = pCha->m_CKitbag.GetPwdLockState();
	sprintf(g_sql, "update %s set \
				kb_locked=%d \
				where atorID=%d", \
				_get_table(), \
				iLocked, \
				atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		//LG("enter_map", "��ɫ%s\tִ�б��汳������״̬��SQL���ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\tcarry out save kitbag lock state SQL sentence error!\n", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ�������%u!\n", atorID);
		LG("enter_map", "Database couldn't find the character%u!\n", atorID);
		return false;
	}

	return true;
}

BOOL CTableCha::SaveStoreItemID(DWORD atorID, long lStoreItemID)
{
	if(atorID == 0)
	{
		return false;
	}

	sprintf(g_sql, "update %s set \
				   store_item=%d \
				   where atorID=%d", \
				   _get_table(), \
				   lStoreItemID, \
				   atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		return false;
	}

	return true;
}

BOOL CTableCha::AddMoney(DWORD atorID, long money)
{
	if(atorID == 0)
	{
		return false;
	}

	sprintf(g_sql, "update %s set \
				   bomd=bomd+%d \
				   where atorID=%d", \
				   _get_table(), \
				   money, \
				   atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		return false;
	}

	return true;
}

BOOL CTableCha::AddCreditByDBID(DWORD atorID, long lCredit)
{
	if(atorID == 0)
	{
		return false;
	}

	sprintf(g_sql, "update %s set \
				   credit=credit+%d \
				   where atorID=%d", \
				   _get_table(), \
				   lCredit, \
				   atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		return false;
	}

	return true;
}

BOOL CTableCha::IsChaOnline(DWORD atorID, BOOL &bOnline)
{
	if(atorID == 0)
	{
		return false;
	}

	BOOL ret = false;

	long lMemAddr = 0;

	const char *sql_syntax = "select endeMem from %s where atorID=%d";
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax, _get_table(), atorID);

	// ִ�в�ѯ����
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try
	{
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO))
		{
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret);

			throw 1;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR *)sql, SQL_NTS);
		if (sqlret != SQL_SUCCESS)
		{
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);

			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		// Bind Column
		for (int i = 0; i < col_num; ++ i)
		{
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
		}

		// Fetch each Row	int i; // ȡ��������
		for (int f_row = 0; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO; ++ f_row)
		{
			if (sqlret != SQL_SUCCESS)
			{
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			}

			lMemAddr = atol((char const *)_buf[0]);
		}
		SQLFreeStmt(hstmt, SQL_UNBIND);
		ret = true;
	}catch(int&e)
	{
		//LG("Store_msg", "IsChaOnline ODBC �ӿڵ��ô���λ���룺%d\n",e);
		LG("Store_msg", "IsChaOnline ODBC interface transfer error ,position ID��%d\n",e);
	}catch (...)
	{
		LG("Store_msg", "Unknown Exception raised when IsChaOnline\n");
	}

	if (hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	if(lMemAddr > 0)
	{
		bOnline = true;
	}
	else
	{
		bOnline = false;
	}

	return ret;
}

Long CTableCha::GetChaAddr(DWORD atorID)
{
	if(atorID == 0)
		return false;

	long lMemAddr = 0;
	const char *sql_syntax = "select endeMem from %s where atorID=%d";
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax, _get_table(), atorID);

	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try
	{
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO))
		{
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret);

			throw 1;
		}
		sqlret = SQLExecDirect(hstmt, (SQLCHAR *)sql, SQL_NTS);
		if (sqlret != SQL_SUCCESS)
		{
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);

			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}
		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);
		for (int i = 0; i < col_num; ++ i)
		{
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
		}
		for (int f_row = 0; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO; ++ f_row)
		{
			if (sqlret != SQL_SUCCESS)
			{
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			}

			lMemAddr = atol((char const *)_buf[0]);
		}
		SQLFreeStmt(hstmt, SQL_UNBIND);
	}
	catch(int&e)
	{
		LG("Store_msg", "IsChaOnline ODBC interface transfer error ,position ID��%d\n",e);
	}
	catch (...)
	{
		LG("Store_msg", "Unknown Exception raised when IsChaOnline\n");
	}
	if (hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}
	return lMemAddr;
}

bool CTableCha::SaveMMaskDBID(CPlayer *pPlayer)
{
	if(!pPlayer || !pPlayer->IsValid()) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	sprintf(g_sql, "update %s set \
				map_mask=%d \
				where atorID=%d", \
				_get_table(), \
				pPlayer->GetMapMaskDBID(), \
				atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		//LG("enter_map", "���%d\tִ�б�����ͼ������SQL���ʱ����!\n", pPlayer->GetDBChaId());
		LG("enter_map", "character %d\tcarry out save big map indexical SQL senternce error!\n", pPlayer->GetDBChaId());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ�������%u!\n", atorID);
		LG("enter_map", "Database couldn't find the character%u!\n", atorID);
		return false;
	}

	return true;
}

bool CTableCha::SaveBankDBID(CPlayer *pPlayer)
{
	if(!pPlayer || !pPlayer->IsValid()) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	const short	csIDBufLen = 200;
	char	szIDBuf[csIDBufLen];
	if (!pPlayer->BankDBIDData2String(szIDBuf, csIDBufLen))
		return false;

	sprintf(g_sql, "update %s set \
				bank=%s \
				where atorID=%d", \
				_get_table(), \
				szIDBuf, \
				atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");

		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		//LG("enter_map", "���%d\tִ�б�������������SQL���ʱ����!\n", pPlayer->GetDBChaId());
		LG("enter_map", "character %d\tcarry out save bank indexcial SQL sentence error!\n", pPlayer->GetDBChaId());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ�������%u!\n", atorID);
		LG("enter_map", "Database couldn't find the character%u!\n", atorID);
		return false;
	}

	return true;
}

bool CTableCha::SaveTableVer(DWORD atorID)
{
	sprintf(g_sql, "update %s set \
				version=%d \
				where atorID=%d", \
				_get_table(), \
				defCHA_TABLE_NEW_VER, \
				atorID);

	short sExec =  exec_sql_direct(g_sql);

	return DBOK(sExec) && !DBNODATA(sExec);
}

BOOL CTableCha::SaveMissionData(CPlayer *pPlayer, DWORD atorID)
{
	if( !pPlayer ) return FALSE;
	CCharacter *pCha = pPlayer->GetMainCha();
	if( !pCha ) return FALSE;

	// �����ɫ�����¼��Ϣ
	memset( g_szMisInfo, 0, ROLE_MAXSIZE_DBMISSION );
	if( !pPlayer->MisGetData( g_szMisInfo, ROLE_MAXSIZE_DBMISSION - 1 ) )
	{
		//pCha->SystemNotice( "SaveMissionData:�ý�ɫ����������Ϣ���ݶ�ȡ����!ID = %d", pCha->GetID() );
		//LG(szDBLog, "SaveMissionData:�����ɫ[ID: %d\tNAME: %s]������Ϣ����ȡ��������ʧ��!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00018), pCha->GetID() );
		LG(szDBLog, "SaveMissionData:save character[ID: %d\tNAME: %s]data info,Get mission data error!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );

	}

	memset( g_szRecord, 0, ROLE_MAXSIZE_DBRECORD );
	if( !pPlayer->MisGetRecord( g_szRecord, ROLE_MAXSIZE_DBRECORD - 1 ) )
	{
		//pCha->SystemNotice( "SaveMissionData:�ý�ɫ����������Ϣ���ݶ�ȡ����!ID = %d", pCha->GetID() );
		//LG(szDBLog, "SaveMissionData:�����ɫ[ID: %d\tNAME: %s]������Ϣ����ȡ������ʷ��¼����ʧ��!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00018), pCha->GetID() );
		LG(szDBLog, "SaveMissionData:save character[ID: %d\tNAME: %s]data info,Get mission history data error !ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
	
	}

	memset( g_szTrigger, 0, ROLE_MAXSIZE_DBTRIGGER );
	if( !pPlayer->MisGetTrigger( g_szTrigger, ROLE_MAXSIZE_DBTRIGGER - 1 ) )
	{
		//pCha->SystemNotice( "SaveMissionData:�ý�ɫ����������Ϣ���������ݶ�ȡ����!ID = %d", pCha->GetID() );
		//LG(szDBLog, "SaveMissionData:�����ɫ[ID: %d\tNAME: %s]������Ϣ����ȡ���񴥷�������ʧ��!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00019), pCha->GetID() );
		LG(szDBLog, "SaveMissionData:save character[ID: %d\tNAME: %s]data info��Get mission trigger data error!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );

	}

	sprintf( g_sql, "update %s set mission='%s', misrecord='%s', mistrigger='%s' \
		where atorID=%d", _get_table(), g_szMisInfo, g_szRecord, g_szTrigger, atorID );

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return FALSE;
	}
	short sExec = exec_sql_direct( g_sql );
	return DBOK(sExec) && !DBNODATA(sExec);
}

// Add by lark.li 20080521 begin
bool CTableLotterySetting::Init(void)
{
	sprintf(g_sql, "select section from %s (nolock) where 1=2", _get_table());

	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(LotterySetting)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "LotterySetting");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

bool CTableLotterySetting::GetCurrentIssue(int &issue)
{
	issue = -1;
	string buf[1]; 
	char param[] = "issue";
	char filter[80]; 
	sprintf(filter, "state  =%d", enumCURRENT);

	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		issue = Str2Int(buf[0]);
		return true;
	}

    return false;
}

bool CTableLotterySetting::AddIssue(int issue)
{
	//insert into %s (section, issue, state, createdate, updatetime) values (%d, %d, %d, getdate(), getdate())

	sprintf( g_sql, "insert into %s (section, issue, state, createdate, updatetime) values (%d, %d, %d, getdate(), getdate())",
		_get_table(), 1, issue, enumCURRENT);

	short sExec = exec_sql_direct( g_sql );
	if( DBOK(sExec) )
	{
		return true;
	}

	return false;
}

bool CTableLotterySetting::DisuseIssue(int issue, int state)
{
	sprintf( g_sql, "update %s set state = %d, updatetime = getdate() where issue = %d", _get_table(), state, issue );

	short sExec = exec_sql_direct( g_sql );

	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		LG("lottery", "lottery couldn't find the issue %d!\n", issue);
		return false;
	}

	return true;
}

bool CTableLotterySetting::SetWinItemNo(int issue, const char* itemno)
{
	sprintf( g_sql, "update %s set itemno = '%s', updatetime = getdate() where issue = %d", _get_table(), itemno, issue );

	short sExec = exec_sql_direct( g_sql );

	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		LG("lottery", "lottery couldn't find the issue %d!\n", issue);
		return false;
	}

	return true;
}

bool CTableLotterySetting::GetWinItemNo(int issue, string& itemno)
{
	string buf[1]; 
	char param[] = "itemno";
	char filter[80]; 
	sprintf(filter, "state  =%d and issue = %d", enumCURRENT, issue);

	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		itemno = buf[0];
		return true;
	}

    return false;
}

bool CTableTicket::Init(void)
{
	sprintf(g_sql, "select itemno from %s (nolock) where 1=2", _get_table());

	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(Ticket)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "Ticket");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

bool CTableTicket::AddTicket(int atorID, int issue, char itemno[6][2])
{
	int index = -1;
	char no[10][6];

	for(int i=0; i<6; i++)
	{
		if(itemno[i][0] == 'X')
		{
			index = i;
			break;
		}
	}

	if(index >0)
	{
		for(int i=0;i<10;i++)
		{
			for(int j=0;j<6;j++)
			{
				if(j == index)
					no[i][j] = '0' + i;
				else
					no[i][j] = itemno[j][0];
			}
		}

		for(int i=0;i<10;i++)
			AddTicket(atorID, issue, no[i][0], no[i][1], no[i][2], no[i][3], no[i][4], no[i][5], 0);
	}

	AddTicket(atorID, issue, itemno[0][0], itemno[1][0], itemno[2][0], itemno[3][0], itemno[4][0], itemno[5][0]);

	return false;
}

bool CTableTicket::AddTicket(int atorID, int issue, char itemno1, char itemno2, char itemno3, char itemno4, char itemno5, char itemno6, int real)
{
	sprintf( g_sql, "insert into %s (atorID, issue, itemno, real, buydate) values (%d, '%d', '%c%c%c%c%c%c', %d, getdate())",
		_get_table(), atorID, issue, itemno1, itemno2, itemno3, itemno4, itemno5, itemno6, real);

	short sExec = exec_sql_direct( g_sql );
	if( DBOK(sExec) )
	{
		return true;
	}

	return false;
}

bool CTableTicket::IsExist(int issue, char* itemno)
{
	string buf[1]; 
	char param[] = "count(*)";
	char filter[50]; 
	sprintf(filter, " issue = %d and itemno  ='%s'", issue, itemno);

	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		if(Str2Int(buf[0]) > 0)
			return true;
	}

    return false;
}

bool CTableTicket::CalWinTicket(int issue, int max, string& itemno)
{
	int RANGE_MIN = 1;
    int RANGE_MAX = 2;

	// ���ʣ�2��ѡһ������
	int probability = rand() % RANGE_MAX + RANGE_MIN;

	// ���ѡ��
	if(issue % probability == 0)
	{
		char sql[256];

		sprintf(sql, "SELECT top 10 * FROM\
						(\
						SELECT     itemno, COUNT(*) AS num\
						FROM         Ticket\
						WHERE     (issue = %d) AND real = 0\
						GROUP BY itemno\
						) \
						AS A\
						WHERE num <= %d\
						ORDER BY num",
						issue, max);

		vector< vector< string > > data;

		getalldata(sql, data);

		RANGE_MAX = (int)data.size();

		if(RANGE_MAX > 0)
		{
			int index = rand() % RANGE_MAX + RANGE_MIN;

			itemno = data[index][0];
			return true;

		}
	}

	char buffer[7];
	RANGE_MAX = 999999;

	while(1)
	{
		int num = rand() % RANGE_MAX + RANGE_MIN;
		sprintf(buffer, "%06d", num);

		if(!IsExist(issue, buffer))
			break;
	}

	itemno = buffer;

	return true;			
}

bool CTableWinTicket::Init(void)
{
	sprintf(g_sql, "select issue from %s (nolock) where 1=2", _get_table());

	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(WinTicket)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "WinTicket");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

bool CTableWinTicket::GetTicket(int issue)
{
	return true;
}

bool CTableWinTicket::AddTicket(int issue, char* itemno, int grade)
{
	sprintf( g_sql, "insert into %s (issue, itemno, grade, createdate) values (%d, '%s', %d, getdate())",
		_get_table(), issue, itemno, grade);

	short sExec = exec_sql_direct( g_sql );
	if( DBOK(sExec) )
	{
		return true;
	}

	return false;
}

bool CTableWinTicket::Exchange(int issue, char* itemno)
{
	sprintf( g_sql, "update %s set num = num + 1 where issue = %d and itemno = '%s'", _get_table(), issue, itemno );

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec = exec_sql_direct( g_sql );

	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		LG("lottery", "lottery couldn't find the issue %d!\n", issue);
		return false;
	}

	return true;
}

bool CTableAmphitheaterSetting::Init(void)
{
	sprintf(g_sql, "select season from %s (nolock) where 1=2", _get_table());

	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(AmphitheaterSetting)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "AmphitheaterSetting");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

// ��õ�ǰ�����ź��ִκ�
bool CTableAmphitheaterSetting::GetCurrentSeason(int& season, int& round)
{
	season = -1;
	round = -1;
	string buf[2]; 
	char param[] = "season, [round]";
	char filter[80]; 
	sprintf(filter, "state  =%d", AmphitheaterSetting::enumCURRENT);

	int r = _get_row(buf, 2, param, filter);
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		season = Str2Int(buf[0]);
		round = Str2Int(buf[1]);
		return true;
	}

    return false;
}

// ׷��һ������
bool CTableAmphitheaterSetting::AddSeason(int season)
{
	//insert into AmphitheaterSetting (section, season, [round], state, createdate, updatetime) values (1, 1, 1, 0, getdate(), getdate())

	sprintf( g_sql, "insert into %s (section, season, [round], state, createdate, updatetime,winner) values (%d, %d, %d, %d, getdate(), getdate(),%s)",
		_get_table(), 1, season, 1, AmphitheaterSetting::enumCURRENT,NULL);
	short sExec = exec_sql_direct( g_sql );
	if( DBOK(sExec) )
	{
		return true;
	}

	return false;
}

// ��������״̬
bool CTableAmphitheaterSetting::DisuseSeason(int season, int state,const char* winner)
{
	sprintf( g_sql, "update %s set state = %d, updatetime = getdate(),winner = %s where season = %d", _get_table(), state, winner, season );

	short sExec = exec_sql_direct( g_sql );

	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		LG("AmphitheaterSetting", "amphitheater couldn't find the season %d!\n", season);
		return false;
	}

	return true;
}

// �����ִ�
bool CTableAmphitheaterSetting::UpdateRound(int season, int round)
{
	sprintf( g_sql, "update %s set [round] = %d, updatetime = getdate() where season = %d", _get_table(), round, season );

	short sExec = exec_sql_direct( g_sql );

	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		LG("AmphitheaterSetting", "amphitheater couldn't find the season %d!\n", season);
		return false;
	}

	return true;
}

bool CTableAmphitheaterTeam::Init(void)
{
	sprintf(g_sql, "select id from %s (nolock) where 1=2", _get_table());

	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(AmphitheaterTeam)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "AmphitheaterTeam");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

// ȡ�ò����������� 
bool CTableAmphitheaterTeam::GetTeamCount(int& count)
{
	count = -1;
	string buf[1]; 
	char param[] = "count(*)";
	char filter[80]; 
	sprintf(filter, "state > %d", AmphitheaterTeam::enumNotUse );

	int r = _get_row(buf, 2, param, filter);
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		count = Str2Int(buf[0]);
		return true;
	}
    return false;
}

// ȡ�ö�����
bool CTableAmphitheaterTeam::GetNoUseTeamID(int &teamID)
{
	// ȡ�ÿհ׶���ID
	teamID = 0;
	string buf[1]; 
	char param[] = "top(1) id";
	char filter[80]; 
	sprintf(filter, "state = %d", AmphitheaterTeam::enumNotUse );

	int r = _get_row(buf, 2, param, filter);
	int	r1 = get_affected_rows();
	if ( DBOK(r) && r1 > 0)
	{
		teamID = Str2Int(buf[0]);
		return true;
	}

	return false;
}

// ����ע�� ����ע����teamID
bool CTableAmphitheaterTeam::TeamSignUP(int &teamID, int captain, int member1, int member2)
{
	// ȡ�ÿհ׶���ID
	if(teamID < 0)
	{
		if(!GetNoUseTeamID(teamID))
			return false;
	}

	char member[50];
	sprintf(member,"%d,%d", member1, member2);

	sprintf( g_sql, "update %s set captain = %d, member = '%s', state = %d, updatetime = getdate() where id = %d", _get_table(), captain, member, AmphitheaterTeam::enumUse, teamID );

	short sExec = exec_sql_direct( g_sql );

	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		LG("AmphitheaterTeam", "amphitheater couldn't find the id %d!\n", teamID);
		return false;
	}

    return true;
}

// ����ȡ��
bool CTableAmphitheaterTeam::TeamCancel(int teamID)
{
	sprintf( g_sql, "update %s set captain = null, member = null, matchno = 0, state = %d, updatetime = getdate() where id = %d", _get_table(), AmphitheaterTeam::enumNotUse, teamID );

	short sExec = exec_sql_direct( g_sql );

	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		LG("AmphitheaterTeam", "amphitheater couldn't find the id %d!\n", teamID);
		return false;
	}

    return true;
}

// �������
bool CTableAmphitheaterTeam::TeamUpdate(int teamID, int matchNo, int state, int winnum, int losenum, int relivenum)
{

    return false;
}

// �ж���Ч�Ķ���
bool CTableAmphitheaterTeam::IsValidAmphitheaterTeam(int teamID, int captainID, int member1, int member2)
{
	string buf[1]; 
	char param[] = "count(*)";
	char filter[80]; 
	int HasValid = 0;
	sprintf(filter, "id = %d and captain = %d and (member = '%d,%d' or member = '%d,%d')", teamID, captainID, member1, member2, member2, member1);

	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();
	if ( DBOK(r) && r1 > 0)
	{
		HasValid = Str2Int(buf[0]);
		if ( HasValid>0)
			return true;
		else
			return false;
	}

	return false;
}
//Add by sunny.sun20080714
//�ж��Ƿ��Ѿ�ע��
bool CTableAmphitheaterTeam::IsLogin(int pActorID)
{
	string buf[1];
	char param[] = "count(*)";
	char filter[80];
	int PActorIDNum = 0;

	sprintf(filter, "captain = %d or member like '%d,%' or member like '%,%d'", pActorID,pActorID,pActorID);

	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();
	short sExec =  exec_sql_direct(g_sql);
	if ( DBOK(r) && r1 > 0)
	{
		PActorIDNum = Str2Int(buf[0]);
		if ( PActorIDNum>0)
			return false;
		else
			return true;
	}
	return false;
}

//�ж��Ƿ�õ�ͼ�������
bool CTableAmphitheaterTeam::IsMapFull(int MapID,int & PActorIDNum)
{
	string buf[1];
	char param[] = "count(*)";
	char filter[80];
	
	sprintf(filter, "map = %d ",MapID);

	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();

	if ( DBOK(r) && r1 > 0)
	{
		PActorIDNum = Str2Int(buf[0]);
		if ( PActorIDNum > 2)
			return false;
		else
			return true;
	}
	return false;
}
//���µ�ͼmapflag
bool CTableAmphitheaterTeam::UpdateMapNum(int Teamid,int Mapid,int MapFlag)
{
	sprintf( g_sql, "update %s set mapflag = %d where id = %d and map = %d", _get_table(),MapFlag, Teamid, Mapid );

	short sExec = exec_sql_direct( g_sql );

	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		return false;
	}

	return true;
}

//���mapflagֵ
bool CTableAmphitheaterTeam::GetMapFlag(int Teamid,int & Mapflag)
{
	string buf[1];
	char param[] = "mapflag";
	char filter[80];
	
	sprintf(filter, "id = %d ",Teamid);

	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();

	if ( DBOK(r) && r1 > 0)
	{
		Mapflag = Str2Int(buf[0]);
		if ( Mapflag >= 2)
			return false;
		else
			return true;
	}
	return false;
}

//����Ʊ�����Ķ���״̬Ϊ����,state = 1 ��state= 3�ĸ�Ϊ��̭
bool CTableAmphitheaterTeam::SetMaxBallotTeamRelive(void)
{
	string buf[1];
	char param[] = "count(*)";
	char filter[80];
	int state = 0;
	int OddOrEven = 0;

	sprintf(filter, "state = %d",AmphitheaterTeam::enumPromotion);
	int r = _get_row(buf, 1, param, filter);
	if ( DBOK(r))
	{
		state = Str2Int(buf[0]);
		if ( state%2 == 0 )
			OddOrEven = 2;
		else
			OddOrEven = 1;
	}
	sprintf( g_sql, "update %s set state = %d ,relivenum = 0 where id in (select top %d id from %s where state = %d  order by relivenum desc)", _get_table(),AmphitheaterTeam::enumPromotion,OddOrEven,_get_table(),AmphitheaterTeam::enumRelive);
	short sExec1 = exec_sql_direct( g_sql );

	sprintf( g_sql, "update %s set state = %d ,relivenum = 0 where state = %d or state = %d", _get_table(),AmphitheaterTeam::enumOut,AmphitheaterTeam::enumRelive,AmphitheaterTeam::enumUse );
	short sExec2 = exec_sql_direct( g_sql );

	if (!DBOK(sExec1)||!DBOK(sExec2))
	{
		return false;
	}
	return true;
}

//���ñ��������״̬
bool CTableAmphitheaterTeam::SetMatchResult(int Teamid1,int Teamid2,int Id1state,int Id2state)
{
	sprintf( g_sql, "update %s set state = %d where id = %d", _get_table(),Id1state,Teamid1);
	short sExec1 = exec_sql_direct( g_sql );
	sprintf( g_sql, "update %s set state = %d where id = %d", _get_table(),Id2state,Teamid2);
	short sExec2 = exec_sql_direct( g_sql );

	if (!DBOK(sExec1)||!DBOK(sExec2))
	{
		return false;
	}
	if (DBNODATA(sExec1)||DBNODATA(sExec2))
	{
		return false;
	}
	return true;
}
//����mapid ȡ�õ�ͼ�������ӳ���id
bool CTableAmphitheaterTeam::GetCaptainByMapId(int Mapid,string &Captainid1,string &Captainid2)
{
	string NoCaptain = "";
	sprintf(g_sql,"select captain from %s where map = %d", _get_table(),Mapid);
	vector< vector< string > > data;

	getalldata(g_sql, data);

	int MapCaptainNum = (int)data.size();
	if( MapCaptainNum > 2 || MapCaptainNum == 0)
	{
		return false;
	}
	if( MapCaptainNum < 2)
	{	

		Captainid1 = data[0][0];
		Captainid2 = NoCaptain;
		return true;
	}
	else
	{
		Captainid1 = data[0][0];
		Captainid2 = data[1][0];
		return true;
	}
	return false;
}
//����map�ֶ�
bool CTableAmphitheaterTeam::UpdateMap(int Mapid)
{
	sprintf( g_sql, "update %s set map = null where map = %d  ", _get_table(),Mapid );
	short sExec = exec_sql_direct( g_sql );
	
	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		return false;
	}
	return true;
}
//ȡ�����а�����
bool CTableAmphitheaterTeam::GetPromotionAndReliveTeam(vector< vector< string > > &dataPromotion, vector< vector< string > > &dataRelive)
{
	// select A.id, A.captain, A.relivenum, B.atorNome from AmphitheaterTeam A,  character B where B.atorID = A.captain
	//sprintf(g_sql,"select A.id, A.captain, B.atorNome, A.winnum from AmphitheaterTeam A,  character B where B.atorID = A.captain and a.state = %d  order by A.winnum DESC", AmphitheaterTeam::enumPromotion );
	sprintf(g_sql,"select B.atorNome,A.id,A.winnum from AmphitheaterTeam A, character B where B.atorID = A.captain and a.state = %d  order by A.winnum DESC", AmphitheaterTeam::enumPromotion );


	if(!getalldata(g_sql, dataPromotion))
		return false;

	//sprintf(g_sql,"select A.id, A.captain, B.atorNome, A.relivenum from AmphitheaterTeam A,  character B where B.atorID = A.captain and A.state = %d  order by A.relivenum DESC", AmphitheaterTeam::enumRelive );
	sprintf(g_sql,"select B.atorNome, A.relivenum ,A.id from AmphitheaterTeam A,  character B where B.atorID = A.captain and A.state = %d  order by A.relivenum DESC", AmphitheaterTeam::enumRelive );

	if(!getalldata(g_sql, dataRelive))
		return false;

	return true;
}

//����Ʊ��
bool CTableAmphitheaterTeam::UpdatReliveNum( int ReID )
{
	string buf[1];
	char param[] = "relivenum";
	char filter[80];
	int relivenumber = 0;
	sprintf(filter, "id = %d ",ReID);

	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();
	if ( DBOK(r) && r1 > 0)
	{
		relivenumber = Str2Int(buf[0]) + 1;
		sprintf( g_sql, "update %s set relivenum = %d where id = %d ", _get_table(),relivenumber ,ReID );
		short sExec =  exec_sql_direct(g_sql);
		if(!DBOK(sExec))
			return false;
		return true;
	}
	return false;
}

//����һ��û�μӱ����Ķ�Ϊ����
bool CTableAmphitheaterTeam::UpdateAbsentTeamRelive()
{
	sprintf( g_sql, "update %s set state = %d where state = %d ", _get_table(),AmphitheaterTeam::enumRelive,AmphitheaterTeam::enumUse );
	short sExec = exec_sql_direct( g_sql );
	
	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		return false;
	}
	return true;
}
//���¶�������ͼ���map�ֶ�
bool CTableAmphitheaterTeam::UpdateMapAfterEnter(int CaptainID,int MapID)
{
	sprintf( g_sql, "update %s set map = %d where captain = %d ", _get_table(),MapID,CaptainID);
	short sExec = exec_sql_direct( g_sql );
	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		return false;
	}
	return true;
}

//Add by sunny.sun20080806
//����winnum��ֵ��ʤ����1
bool CTableAmphitheaterTeam::UpdateWinnum( int teamid )
{
	sprintf( g_sql, "update %s set winnum = winnum+1 where id = %d ", _get_table(),teamid);
	short sExec = exec_sql_direct( g_sql );
	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		return false;
	}
	return true;
}
//��ȡwinnum�����Ψһ�Ķ����id
bool CTableAmphitheaterTeam::GetUniqueMaxWinnum( int &teamid )
{
	//select id from AmphitheaterTeam where winnum in (select  max(winnum) from AmphitheaterTeam)
	string buf[1];
	char param[] = "id";
	char filter[80];
	
	sprintf(filter, "winnum in (select  max(winnum) from AmphitheaterTeam)");
	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();
	if ( DBOK(r) && r1 > 0)
	{
		if( r1 == 1 )
		{
			teamid = Str2Int(buf[0]);
			return true;
		}
		else
			return false;
	}
	return false;
}

bool CTableAmphitheaterTeam::SetMatchnoState( int teamid )
{
	sprintf( g_sql, "update %s set matchno = 1 where id = %d ", _get_table(),teamid);
	short sExec = exec_sql_direct( g_sql );
	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		return false;
	}
	return true;
}

bool CTableAmphitheaterTeam::UpdateState()
{
	sprintf( g_sql, "update %s set state = %d where state= %d ", _get_table(),AmphitheaterTeam::enumUse,AmphitheaterTeam::enumPromotion);
	short sExec = exec_sql_direct( g_sql );
	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		return false;
	}
	return true;
}

bool CTableAmphitheaterTeam::CloseReliveByState(int &statenum )
{
	string buf[1];
	char param[] = "count(*)";
	char filter[80];
	
	sprintf(filter, "state = %d ",AmphitheaterTeam::enumUse);
	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();
	if ( DBOK(r) && r1 > 0)
	{
		statenum = Str2Int(buf[0]);
		return true;
	}
	return false;
	
}

bool CTableAmphitheaterTeam::CleanMapFlag(int teamid1,int teamid2)
{
	sprintf( g_sql, "update %s set mapflag = null where id= %d or id = %d", _get_table(),teamid1,teamid2);
	short sExec = exec_sql_direct( g_sql );
	if (!DBOK(sExec))
	{
		return false;
	}
	if (DBNODATA(sExec))
	{
		return false;
	}
	return true;
	
}

bool CTableAmphitheaterTeam::GetStateByTeamid( int teamid, int &state )
{
	string buf[1];
	char param[] = "state";
	char filter[80];
	
	sprintf(filter, "id = %d ",teamid);
	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();
	if ( DBOK(r) && r1 > 0)
	{
		state = Str2Int(buf[0]);
		return true;
	}
	return false;
}

//personinfo ��ʼ��
bool CTablePersoninfo::Init( void )
{
	sprintf(g_sql, "select atorID from %s (nolock) where 1=2", _get_table());

	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(AmphitheaterSetting)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "personinfo");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

//��ȡbirthday
bool CTablePersoninfo::GetPersonBirthday( int chaid, int &birthday )
{
	string buf[1];
	char param[] = "birthday";
	char filter[80];
	
	sprintf(filter, "atorID = %d ",chaid);

	int r = _get_row(buf, 1, param, filter);
	int	r1 = get_affected_rows();
	if ( DBOK(r) && r1 > 0)
	{
		if(strcmp(buf[0].c_str() ,"NULL") == 0)
			return false;
		else
			birthday = Str2Int(buf[0]);
		return true;
	}
	return false;
}

// End

bool CTableResource::Init(void)
{
	sprintf(g_sql, "select \
				id, atorID, type_id, content \
				from %s \
				(nolock) where 1=2", \
				_get_table());
	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(resource)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "resource");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

bool CTableResource::Create(long &lDBID, long lChaId, long lTypeId)
{
	sprintf( g_sql, "insert %s (atorID, type_id) \
					values(%d, %d) ",	_get_table(), 
					lChaId, lTypeId);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec = exec_sql_direct( g_sql );
	if( DBOK(sExec) )
	{
		string buf[1]; 
		char param[] = "@@identity";
		char filter[80]; 
		sprintf(filter, "1=1");
		bool ret = _get_row(buf, 1, param, filter);
		lDBID = atol( buf[0].c_str() );
		return true;
	}

	return false;
}

bool CTableResource::ReadKitbagData(CCharacter *pCha)
{
	if (!pCha)
	{
		//LG("enter_map", "����Դ���ݿ���󣬽�ɫ������.\n");
		LG("enter_map", "Load resource database error,character is inexistence\n");
		return false;
	}
	if (pCha->GetKitbagRecDBID() == 0)
	{
		long	lDBID;
		if (!Create(lDBID, pCha->GetPlayer()->GetDBChaId(), enumRESDB_TYPE_KITBAG))
			return false;
		pCha->SetKitbagRecDBID(lDBID);
	}

	int nIndex = 0;
	char param[] = "atorID, type_id, content";
	char filter[80]; sprintf(filter, "id=%d", pCha->GetKitbagRecDBID());
	int r = _get_row3(g_buf, g_cnCol, param, filter);
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		DWORD	dwChaId = Str2Int(g_buf[nIndex++]);
		char	chType = Str2Int(g_buf[nIndex++]);
		if (dwChaId != pCha->GetPlayer()->GetDBChaId() || chType != enumRESDB_TYPE_KITBAG)
		{
			//LG("enter_map", "����Դ���ݿ���󣬽�ɫ��ƥ��.\n");
			LG("enter_map", "Load resource database error��character is not matching.\n");
			return false;
		}
		if (!pCha->String2KitbagData(g_buf[nIndex++]))
		{
			//LG("enter_map", "��������У��ʹ���.\n");
			LG("enter_map", "kitbag data check sum error.\n");
			//LG("У��ʹ���", "��ɫ��%s���ı������ݣ�resource_id %u��У��ʹ���.\n", pCha->GetLogName(), pCha->GetKitbagRecDBID());
			LG("check sum error", "character(%s) kitbag data(resource_id %u) check sum error.\n", pCha->GetLogName(), pCha->GetKitbagRecDBID());
			return false;
		}
	}
	else
	{
		//LG("enter_map", "����Դ���ݿ����_get_row()����ֵ��%d.\n", r);
		LG("enter_map", "Load resource database error��_get_row()return value:%d.\n", r);
		return false;
	}
	//LG("enter_map", "���������ݳɹ�.\n");
	LG("enter_map", "Load kitbag data succeed.\n");
	return true;
}

bool CTableResource::SaveKitbagData(CCharacter *pCha)
{
	if(!pCha || !pCha->IsValid()) return false;

	//LG("enter_map", "��ʼ���汳������!\n");
	g_kitbag[0] = 0;
	if (!KitbagData2String(&pCha->m_CKitbag, g_kitbag, defKITBAG_DATA_STRING_LEN))
	{
		//LG("enter_map", "��ɫ%s\t�������ݣ�������ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\tsave data(kitbag) error!\n", pCha->GetLogName());
		return false;
	}
	//LG("enter_map", "ת���������ݳɹ�\n");

	sprintf(g_sql, "update %s set \
				   content='%s' \
				   where id=%d", \
				   _get_table(), \
				   g_kitbag, \
				   pCha->GetKitbagRecDBID());

	//LG("enter_map", "��֯SQL���ɹ�\n");
	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);
	//LG("enter_map", "ִ��SQL���ɹ�\n");
	if (!DBOK(sExec))
	{
		//LG("enter_map", "��ɫ%s\tִ��SQL���ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\t carry out SQL sentence error!\n", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ���ñ�����Դ%u!\n", pCha->GetKitbagRecDBID());
		LG("enter_map", "Database couldn't find the kitbag resource %u!\n", pCha->GetKitbagRecDBID());
		return false;
	}
	//LG("enter_map", "��ɱ�������.\n");
	LG("enter_map", "finish kitbag save.\n");

	return true;
}

bool CTableResource::ReadKitbagTmpData(CCharacter *pCha)
{
    if (!pCha)
	{
		//LG("enter_map", "����Դ���ݿ���󣬽�ɫ������.\n");
		LG("enter_map", "Load resource database error,character is inexistence.\n");
		return false;
	}
	if (pCha->GetKitbagTmpRecDBID() == 0)
	{
		long	lDBID;
		if (!Create(lDBID, pCha->GetPlayer()->GetDBChaId(), enumRESDB_TYPE_KITBAGTMP))
			return false;
		pCha->SetKitbagTmpRecDBID(lDBID);
	}

	int nIndex = 0;
	char param[] = "atorID, type_id, content";
	char filter[80]; sprintf(filter, "id=%d", pCha->GetKitbagTmpRecDBID());
	int r = _get_row3(g_buf, g_cnCol, param, filter);
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		DWORD	dwChaId = Str2Int(g_buf[nIndex++]);
		char	chType = Str2Int(g_buf[nIndex++]);
		if (dwChaId != pCha->GetPlayer()->GetDBChaId() || chType != enumRESDB_TYPE_KITBAGTMP)
		{
			//LG("enter_map", "����Դ���ݿ���󣬽�ɫ��ƥ��.\n");
			LG("enter_map", "Load resource database error,character is not matching.\n");
			return false;
		}
		if (!pCha->String2KitbagTmpData(g_buf[nIndex++]))
		{
			//LG("enter_map", "��ʱ��������У��ʹ���.\n");
			LG("enter_map", "Temp kitbag data check sum error.\n");
			//LG("У��ʹ���", "��ɫ��%s������ʱ�������ݣ�resource_id %u��У��ʹ���.\n", pCha->GetLogName(), pCha->GetKitbagTmpRecDBID());
			LG("check sum error", "character(%s) temp kitbag data(resource_id %u)check sum error.\n", pCha->GetLogName(), pCha->GetKitbagTmpRecDBID());
			return false;
		}
	}
	else
	{
		//LG("enter_map", "����Դ���ݿ����_get_row()����ֵ��%d.\n", r);
		LG("enter_map", "Load resource database error,_get_row() return value:%d.\n", r);
		return false;
	}
	//LG("enter_map", "����ʱ�������ݳɹ�.\n");
	LG("enter_map", "Load temp kitbag data succeed.\n");

	return true;
}

bool CTableResource::ReadKitbagTmpData(long lRecDBID, string& strData)
{
	if(lRecDBID == 0)
	{
		return false;
	}

	BOOL ret = false;

	const char *sql_syntax = "select content from %s where id=%d";
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax, _get_table(), lRecDBID);

	// ִ�в�ѯ����
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try
	{
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO))
		{
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret);

			throw 1;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR *)sql, SQL_NTS);
		if (sqlret != SQL_SUCCESS)
		{
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);

			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		// Bind Column
		for (int i = 0; i < col_num; ++ i)
		{
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
		}

		// Fetch each Row	int i; // ȡ��������
		for (int f_row = 0; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO; ++ f_row)
		{
			if (sqlret != SQL_SUCCESS)
			{
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			}

			strData = (char const *)_buf[0];
		}
		SQLFreeStmt(hstmt, SQL_UNBIND);
		ret = true;
	}catch(int&e)
	{
		//LG("Store_msg", "ReadKitbagTmpData ODBC �ӿڵ��ô���λ���룺%d\n",e);
		LG("Store_msg", "ReadKitbagTmpData ODBC interface transfer error,position ID:%d\n",e);
	}catch (...)
	{
		LG("Store_msg", "Unknown Exception raised when ReadKitbagTmpData\n");
	}

	if (hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}

bool CTableResource::SaveKitbagTmpData(long lRecDBID, const string& strData)
{
	if(lRecDBID == 0)
	{
		return false;
	}
	
	sprintf(g_sql, "update %s set \
				   content='%s' \
				   where id=%d", \
				   _get_table(), \
				   strData.c_str(), \
				   lRecDBID);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("Store_msg", "SQL��䳤��Խ��!\n");
		LG("Store_msg", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		//LG("Store_msg", "ִ��SQL���ʱ����!\n");
		LG("Store_msg", "carry out SQL sentence error!\n");
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("Store_msg", "���ݿ�û�в�ѯ������ʱ������Դ%u!\n", lRecDBID);
		LG("Store_msg", "Database couldn't find the temp kitbag resource %u!\n", lRecDBID);
		return false;
	}
	//LG("Store_msg", "�����ʱ��������.\n");
	LG("Store_msg", "finish the temp kitbag save.\n");

	return true;
}

bool CTableResource::SaveKitbagTmpData(CCharacter *pCha)
{
    if(!pCha || !pCha->IsValid()) return false;

	g_kitbagTmp[0] = 0;
	if (!KitbagData2String(pCha->m_pCKitbagTmp, g_kitbagTmp, defKITBAG_DATA_STRING_LEN))
	{
		//LG("enter_map", "��ɫ%s\t�������ݣ���ʱ������ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\tsave data(temp kitbag)error!\n", pCha->GetLogName());
		return false;
	}

	sprintf(g_sql, "update %s set \
				   content='%s' \
				   where id=%d", \
				   _get_table(), \
				   g_kitbagTmp, \
				   pCha->GetKitbagTmpRecDBID());

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);

	if (!DBOK(sExec))
	{
		//LG("enter_map", "��ɫ%s\tִ��SQL���ʱ����!\n", pCha->GetLogName());
		LG("enter_map", "character %s\tcarry out SQL sentence error!\n", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ������ʱ������Դ%u!\n", pCha->GetKitbagTmpRecDBID());
		LG("enter_map", "Database couldn't find the temp kitbag resource%u!\n", pCha->GetKitbagTmpRecDBID());
		return false;
	}
	//LG("enter_map", "�����ʱ��������.\n");
	LG("enter_map", "finish save the temp kitbag.\n");

	return true;
}

bool CTableResource::ReadBankData(CPlayer *pCPly, char chBankNO)
{
	if (!pCPly)
	{
		//LG("enter_map", "����Դ���ݿ���󣬽�ɫ������.\n");
		LG("enter_map", "Load resource database error,character is inexistence.\n");
		return false;
	}
	if (pCPly->GetCurBankNum() == 0)
	{
		long	lDBID;
		if (!Create(lDBID, pCPly->GetDBChaId(), enumRESDB_TYPE_BANK))
			return false;
		pCPly->AddBankDBID(lDBID);
	}

	char	chStart, chEnd;
	if (chBankNO < 0)
	{
		chStart = 0;
		chEnd = pCPly->GetCurBankNum() - 1;
	}
	else
	{
		if (chBankNO >= pCPly->GetCurBankNum())
			return false;
		chStart = chEnd = chBankNO;
	}

	int nIndex = 0;
	char param[] = "atorID, type_id, content";
	char filter[80];
	int r;
	int	r1;
	for (char i = chStart; i <= chEnd; i++)
	{
		nIndex = 0;
		sprintf(filter, "id=%d", pCPly->GetBankDBID(i));
		r = _get_row3(g_buf, g_cnCol, param, filter);
		r1 = get_affected_rows();
		if (DBOK(r) && r1 > 0)
		{
			DWORD	dwChaId = Str2Int(g_buf[nIndex++]);
			char	chType = Str2Int(g_buf[nIndex++]);
			if (dwChaId != pCPly->GetDBChaId() || chType != enumRESDB_TYPE_BANK)
			{
				//LG("enter_map", "����Դ���ݿ���󣬽�ɫ��ƥ��.\n");
				LG("enter_map", "Load resource database error,character is not matching.\n");
				return false;
			}
			if (!pCPly->String2BankData(i, g_buf[nIndex++]))
			{
				//LG("enter_map", "��������У��ʹ���.\n");
				LG("enter_map", "kitbag data check sum error.\n");
				//LG("У��ʹ���", "��ң�%u�����������ݣ�resource_id %u��У��ʹ���.\n", pCPly->GetDBChaId(), pCPly->GetBankDBID(i));
				LG("check sum error", "player (%u) bank data(resource_id %u)check sum error.\n", pCPly->GetDBChaId(), pCPly->GetBankDBID(i));
				return false;
			}
		}
		else
		{
			//LG("enter_map", "����Դ���ݿ����_get_row()����ֵ��%d.\n", r);
			LG("enter_map", "Load resource database error��_get_row() return value:%d.\n", r);
			return false;
		}
	}
	//LG("enter_map", "���������ݳɹ�.\n");
	LG("enter_map", "Load bank data succeed.\n");
	return true;
}

bool CTableResource::SaveBankData(CPlayer *pCPly, char chBankNO)
{
	if(!pCPly || !pCPly->IsValid()) return false;
	if (pCPly->GetCurBankNum() == 0)
		return true;

	char	chStart, chEnd;
	if (chBankNO < 0)
	{
		chStart = 0;
		chEnd = pCPly->GetCurBankNum() - 1;
	}
	else
	{
		if (chBankNO >= pCPly->GetCurBankNum())
			return false;
		chStart = chEnd = chBankNO;
	}

	//LG("enter_map", "��ʼ������������!\n");

	for (char i = chStart; i <= chEnd; i++)
	{
		if (!pCPly->BankWillSave(i))
			continue;
		pCPly->SetBankSaveFlag(i, false);
		g_kitbag[0] = 0;
		if (!KitbagData2String(pCPly->GetBank(i), g_kitbag, defKITBAG_DATA_STRING_LEN))
		{
			//LG("enter_map", "��ɫ%u\t�������ݣ����У�ʱ����!\n", pCPly->GetBankDBID(i));
			LG("enter_map", "character%u\tsave data(bank) error!\n", pCPly->GetBankDBID(i));
			return false;
		}
		//LG("enter_map", "ת���������ݳɹ�\n");

		sprintf(g_sql, "update %s set \
					content='%s' \
					where id=%d", \
					_get_table(), \
					g_kitbag, \
					pCPly->GetBankDBID(i));

		//LG("enter_map", "��֯SQL���ɹ�\n");
		if (strlen(g_sql) >= SQL_MAXLEN)
		{
			//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
			FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
			if (pf)
			{
				fprintf(pf, "%s\n\n", g_sql);
				fclose(pf);
			}
			//LG("enter_map", "SQL��䳤��Խ��!\n");
			LG("enter_map", "SQL sentence length slop over\n");
			return false;
		}
		short sExec =  exec_sql_direct(g_sql);
		//LG("enter_map", "ִ��SQL���ɹ�\n");
		if (!DBOK(sExec))
		{
			//LG("enter_map", "���%u\tִ��SQL���ʱ����!\n", pCPly->GetDBChaId());
			LG("enter_map", "player %u\tcarry out SQL sentence error!\n", pCPly->GetDBChaId());
			return false;
		}
		if (DBNODATA(sExec))
		{
			//LG("enter_map", "���ݿ�û�в�ѯ����������Դ%u!\n", pCPly->GetBankDBID(i));
			LG("enter_map", "Database couldn't find the bank resource%u!\n", pCPly->GetBankDBID(i));
			return false;
		}
	}
	//LG("enter_map", "���ȫ������[%d->%d]����.\n", chStart, chEnd);
	LG("enter_map", "finish the whole bank[%d->%d]save\n", chStart, chEnd);
	return true;
}

bool CTableMapMask::Init(void)
{
	sprintf(g_sql, "select \
				id, atorID, content1, content2, content3, content4, content5 \
				from %s \
				(nolock) where 1=2", \
				_get_table());
	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(map_mask)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "map_mask");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

bool CTableMapMask::GetColNameByMapName(const char *szMapName, char *szColName, long lColNameLen)
{
	if (!szMapName || !szColName)
		return false;
	const char	*szColBase = "content";
	if ((long)strlen(szColBase) + 1 >= lColNameLen)
		return false;

	char	chIndex = 0;
	if (!strcmp(szMapName, "garner"))
		chIndex = 1;
	else if (!strcmp(szMapName, "magicsea"))
		chIndex = 2;
	else if (!strcmp(szMapName, "darkblue"))
		chIndex = 3;
	else if (!strcmp(szMapName, "winterland"))	// Add by lark.li 20080812 
		chIndex = 4;
	//else if (!strcmp(szMapName, "eastgoaf"))
	//	chIndex = 4;
	//else if (!strcmp(szMapName, "lonetower"))
	//	chIndex = 5;
	else
		return false;

	sprintf(szColName, "%s%d", szColBase, chIndex);
	return true;
}

bool CTableMapMask::Create(long &lDBID, long lChaId)
{
	sprintf( g_sql, "insert %s (atorID) \
					values(%d) ",	_get_table(), 
					lChaId);

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec = exec_sql_direct( g_sql );
	if( DBOK(sExec) )
	{
		string buf[1]; 
		char param[] = "@@identity";
		char filter[80]; 
		sprintf(filter, "1=1");
		bool ret = _get_row(buf, 1, param, filter);
		lDBID = atol( buf[0].c_str() );
		return true;
	}

	return false;
}

bool CTableMapMask::ReadData(CPlayer *pCPly)
{
	if(!pCPly || !pCPly->IsValid())
	{
		//LG("enter_map", "����ͼ�������ݿ���󣬽�ɫ������.\n");
		LG("enter_map", "Load map hideID database error,character is inexistence.\n");
		return false;
	}
	if (pCPly->GetMapMaskDBID() == 0)
	{
		long	lDBID;
		if (!Create(lDBID, pCPly->GetDBChaId()))
			return false;
		pCPly->SetMapMaskDBID(lDBID);
	}

	char szMaskColName[30];
	if (!GetColNameByMapName(pCPly->GetMaskMapName(), szMaskColName, 30))
	{
		//LG("enter_map", "ѡ���ͼ�������ݴ���.\n");
		LG("enter_map", "choice map hideID data error.\n");
		return false;
	}

	int nIndex = 0;
	char param[80];
	sprintf(param, "atorID, %s", szMaskColName);
	char filter[80]; sprintf(filter, "id=%d", pCPly->GetMapMaskDBID());
	int r = _get_row3(g_buf, g_cnCol, param, filter);
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		DWORD	dwChaId = Str2Int(g_buf[nIndex++]);
		if (dwChaId != pCPly->GetDBChaId())
		{
			//LG("enter_map", "����ͼ�������ݿ���󣬽�ɫ��ƥ��.\n");
			LG("enter_map", "Load map hideID database error,character is not matching.\n");
			return false;
		}
		pCPly->SetMapMaskBase64(g_buf[nIndex++].c_str());
		//if (strcmp(g_buf[nIndex-1].c_str(), "0"))
		//	LG("���ͼ����", "��ͼ %s������ %u.\n", pCPly->GetMaskMapName(), strlen(g_buf[nIndex-1].c_str()));
	}
	else
	{
		//LG("enter_map", "����ͼ�������ݿ����_get_row()����ֵ��%d.%u\n", r);
		LG("enter_map", "Load map hideID database error,_get_row() return value:%d.%u\n", r);
		return false;
	}
	//LG("enter_map", "�����ͼ���ݳɹ�.\n");
	LG("enter_map", "Load big map data succeed.\n");
	return true;
}

bool CTableMapMask::SaveData(CPlayer *pCPly, BOOL bDirect)
{
	if(!pCPly || !pCPly->IsValid()) return false;

	char szMaskColName[30];
	if (!GetColNameByMapName(pCPly->GetMaskMapName(), szMaskColName, 30))
	{
		//LG("enter_map", "ѡ���ͼ�������ݴ���.\n");
		LG("enter_map", "choice map hideID data error.\n");
		return false;
	}

	//LG("enter_map", "��ʼ������ͼ����!\n");
	LG("enter_map", "start save big map data!\n");
	sprintf(g_sql, "update %s set \
				   %s='%s' \
				   where id=%d", \
				   _get_table(), \
				   szMaskColName, \
				   pCPly->GetMapMaskBase64(), \
				   pCPly->GetMapMaskDBID());

	//LG("enter_map", "��֯���ͼ���ݳɹ�!\n");
	LG("enter_map", "organize big map data succeed!\n");

	if(!bDirect) 
	{
		AddSaveQuest(g_sql);
	}
	else		 
	{
		return _ExecSQL(g_sql);
	}
	return true;
}

// ִ�е���������ͼ��SQL���
BOOL CTableMapMask::_ExecSQL(const char *pszSQL)
{
	MPTimer t;
	t.Begin();
	if (strlen(pszSQL) >= SQL_MAXLEN)
	{
		//FILE *pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");`
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", pszSQL);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return FALSE;
	}
	
	short sExec =  exec_sql_direct(pszSQL);
	// LG("enter_map", "ִ�д��ͼSQL�ɹ�!\n");
	if (!DBOK(sExec))
	{
		//LG("enter_map", "���ִ��SQL���ʱ����sql = [%s]\n", pszSQL);
		LG("enter_map", "player carry out SQL sentence error sql = [%s]\n", pszSQL);
		return FALSE;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ���õ�ͼ����sql = [%s]\n", pszSQL);
		LG("enter_map", "Database couldn't find the map hideID sql = [%s]\n", pszSQL);
		return FALSE;
	}
	//LG("enter_map", "��ɴ��ͼ�������.\n");
	//LG("�������ݺ�ʱ", "������ͼ��ʱ[%d], ����[%d]\n", t.End(), _SaveMapMaskList.size() - 1);
	LG("save data waste time", "save big map waste time[%d],queue[%d]\n", t.End(), _SaveMapMaskList.size() - 1);
	return TRUE;
}

// ���еĴ��ͼ��������һ����ִ��
void CTableMapMask::SaveAll()
{
	for(list<string>::iterator it = _SaveMapMaskList.begin(); it!=_SaveMapMaskList.end(); it++)
	{
		string &strSQL = (*it);
		_ExecSQL(strSQL.c_str());
	}
	//LG("enter_map", "һ����ִ�����д��ͼ����SQL, �ܼ�[%d]��!\n", _SaveMapMaskList.size());
	LG("enter_map", "one-off carry out every big map save SQL,totalize[%d] piece!\n", _SaveMapMaskList.size());
	_SaveMapMaskList.clear();
}

// ��ʱִ�еĴ��ͼ��������
void CTableMapMask::HandleSaveList()
{
	//	yyy	add!
	//RECORDCALL( __FUNCTION__ );
	DWORD dwTick = GetTickCount();
	static DWORD g_dwLastSaveTick = 0;

	if( (dwTick - g_dwLastSaveTick) > 2000) // ÿ�����Ӽ��һ�δ��ͼ��������
	{
		g_dwLastSaveTick = dwTick;

		int nSize = (int)(_SaveMapMaskList.size());
		if(nSize==0) return;
		
		string &strSQL = _SaveMapMaskList.front();

		_ExecSQL(strSQL.c_str());

		_SaveMapMaskList.pop_front();

		// LG("�������ݺ�ʱ", "���ͼ�������[%d]\n", nSize - 1);
	}
}

bool CTableAct::Init(void)
{
	sprintf(g_sql, "select \
				ato_id, ato_nome, jmes, ator_ids, last_ip, disc_reason, last_leave \
				from %s \
				(nolock) where 1=2", \
				_get_table());
	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(account)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "account");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

bool CTableAct::ReadAllData(CPlayer *pPlayer, DWORD ato_id)
{
	if(!pPlayer) return false;

	string buf[3]; 
    char param[] = "jmes, ato_nome, IMP";
    char filter[80]; sprintf(filter, "ato_id=%d", ato_id);
    int r = _get_row(buf, 3, param, filter);
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
    {
        pPlayer->SetGMLev(Str2Int(buf[0]));
		pPlayer->SetActName(buf[1].c_str());
		pPlayer->SetIMP(Str2Int(buf[2].c_str()));
		return true;
    }

    return false;
}

bool CTableAct::SaveIMP(CPlayer *pPlayer){
	int IMP = pPlayer->GetMainCha()->GetIMP();
	int actID = pPlayer->GetMainCha()->GetID();
	sprintf(g_sql, "Update %s set IMP = %d where atorID = %d;", "character",IMP,actID);
	short l_sqlret = exec_sql_direct(g_sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0){
		return false;
	}
	return true;
}

bool CTableAct::SaveGmLv(CPlayer *pPlayer)
{

	int gmLv = pPlayer->GetGMLev();
	int actID = pPlayer->GetDBActId();
	sprintf(g_sql, "Update %s set jmes = %d where ato_id = %d;", _get_table(), gmLv, actID);
	short l_sqlret = exec_sql_direct(g_sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0){
		return false;
	}
	return true;
}

bool CTableBoat::Init(void)
{
	sprintf(g_sql, "select \
				boat_id, boat_berth, boat_name, boat_boatid, boat_header, boat_body, boat_engine, boat_cannon, boat_equipment, \
				boat_bag, boat_diecount, boat_isdead, boat_ownerid, boat_createtime, boat_isdeleted, cur_endure, mx_endure, cur_supply, mx_supply, skill_state, \
				map, map_x, map_y, angle, degree, exp \
				from %s \
				(nolock) where 1=2", \
				_get_table());
	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(boat)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "boat");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

BOOL CTableBoat::Create( DWORD& dwBoatID, const BOAT_DATA& Data )
{
	string	strKitbag = "";
	KitbagStringConv(Data.sCapacity, strKitbag);
	char szTime[128] = "";
	SYSTEMTIME SysTime;
	GetLocalTime( &SysTime );
	sprintf( szTime, "%d-%d-%d %d:%d:%d", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond );
	sprintf( g_sql, "insert %s (boat_name, boat_berth, boat_boatid, boat_header, boat_body, \
					boat_engine, boat_cannon, boat_equipment, boat_bag, boat_diecount, \
					boat_isdead, boat_ownerid, boat_createtime, boat_isdeleted) \
					values( '%s', %d, %d, %d, %d, %d, %d, %d, '%s', %d, %d, %d, '%s', %d ) ",	_get_table(), 
					Data.szName, Data.sBerth, Data.sBoat, Data.sHeader, Data.sBody, 
					Data.sEngine, Data.sCannon, Data.sEquipment, strKitbag.c_str(), 0, 0, Data.dwOwnerID, szTime, 0 );

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return FALSE;
	}
	short sExec = exec_sql_direct( g_sql );
	if( DBOK(sExec) )
	{
		string buf[1]; 
		char param[] = "@@identity";
		char filter[80]; 
		sprintf(filter, "1=1");
		bool ret = _get_row(buf, 1, param, filter);
		dwBoatID = atol( buf[0].c_str() );
		return TRUE;
	}

	return FALSE;
}

BOOL CTableBoat::GetBoat( CCharacter& Boat )
{
	DWORD dwBoatID = (DWORD)Boat.getAttr( ATTR_BOAT_DBID );
	BOAT_DATA Data;
	memset( &Data, 0, sizeof(BOAT_DATA) );
	int nIndex = 0;
	char param[] = "boat_name, boat_boatid, boat_berth, boat_header, boat_body, \
					boat_engine, boat_cannon, boat_equipment, boat_diecount, boat_isdead, \
					boat_ownerid, boat_isdeleted, \
					cur_endure, mx_endure, cur_supply, mx_supply, skill_state, \
					map, map_x, map_y, angle, degree, exp";
	char filter[80]; 
	sprintf(filter, "boat_id=%d", dwBoatID );
	int r = _get_row3( g_buf, g_cnCol, param, filter );
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		// ��ֻ��������		
		strncpy( Data.szName, g_buf[nIndex++].c_str(), BOAT_MAXSIZE_BOATNAME - 1 );
		Data.sBoat = (USHORT)Str2Int(g_buf[nIndex++]);
		Data.sBerth = (USHORT)Str2Int(g_buf[nIndex++]);
		Data.sHeader = (USHORT)Str2Int(g_buf[nIndex++]);
		Data.sBody = (USHORT)Str2Int(g_buf[nIndex++]);
		Data.sEngine = (USHORT)Str2Int(g_buf[nIndex++]);
		Data.sCannon = (USHORT)Str2Int(g_buf[nIndex++]);
		Data.sEquipment = (USHORT)Str2Int(g_buf[nIndex++]);
		USHORT sDieCount = (USHORT)Str2Int(g_buf[nIndex++]);
		BYTE byIsDead = (BYTE)Str2Int(g_buf[nIndex++]);
		DWORD dwOwnerID = (DWORD)Str2Int(g_buf[nIndex++]);
		BYTE byIsDeleted = (BYTE)Str2Int(g_buf[nIndex++]);
		//if( dwOwnerID != Boat.GetPlayer()->GetDBChaId() )
		//{
		//	LG( "boat_error", "��ֻ��%s��ID[0x%X]ӵ����ID[0x%X]�뵱ǰ��ɫ��%s��ID[0x%X]����.", 
		//		Data.szName, dwBoatID, dwOwnerID, 
		//		Boat.GetName(), Boat.GetPlayer()->GetDBChaId() );
		//	Boat.SystemNotice( "��ȡ���Ĵ�ֻ��%s��������������ʧ�ܣ���֪ͨά����Ա���!лл!" );
		//	return FALSE;
		//}

		if( byIsDeleted == 1 )
		{
			/*LG( "boat_error", "��ֻ��%s��ID[0x%X]ӵ����ID[0x%X]�Ѿ�ɾ�����뵱ǰ��ɫ��%s������֤�����ݲ�����.", 
				Data.szName, dwBoatID, dwOwnerID, 
				Boat.GetName() );*/
			LG( "boat_error", "boat(%s)ID[0x%X]owner ID[0x%X]had delete,is not fall short of the currently character (%s) captain prove data.", 
				Data.szName, dwBoatID, dwOwnerID, 
				Boat.GetName() );
			//Boat.SystemNotice( "��ȡ���Ĵ�ֻ��%s�����ݵ�ǰ״̬ʧ�ܣ���֪ͨά����Ա���!лл!", Boat.GetName());
			Boat.SystemNotice( RES_STRING(GM_GAMEDB_CPP_00020), Boat.GetName());
			return FALSE;
		}

		Boat.SetName( Data.szName );
		Boat.setAttr( ATTR_BOAT_BERTH, Data.sBerth, 1 );
		Boat.setAttr( ATTR_BOAT_SHIP, Data.sBoat, 1 );
		Boat.setAttr( ATTR_BOAT_HEADER, Data.sHeader, 1 );
		Boat.setAttr( ATTR_BOAT_BODY, Data.sBody, 1 );
		Boat.setAttr( ATTR_BOAT_ENGINE, Data.sEngine, 1 );
		Boat.setAttr( ATTR_BOAT_CANNON, Data.sCannon, 1 );
		Boat.setAttr( ATTR_BOAT_PART, Data.sEquipment, 1 );
		Boat.setAttr( ATTR_BOAT_DIECOUNT, sDieCount, 1 );
		Boat.setAttr( ATTR_BOAT_ISDEAD, byIsDead, 1 );
		
		// ����
		Boat.setAttr(ATTR_HP, Str2Int(g_buf[nIndex++]), 1);
		Boat.setAttr(ATTR_BMXHP, Str2Int(g_buf[nIndex++]), 1);
		Boat.setAttr(ATTR_SP, Str2Int(g_buf[nIndex++]), 1);
		Boat.setAttr(ATTR_BMXSP, Str2Int(g_buf[nIndex++]), 1);
		g_strChaState[1] = g_buf[nIndex++];
		// λ��
		Boat.SetBirthMap(g_buf[nIndex++].c_str());
		long lPosX = Str2Int(g_buf[nIndex++]);
		long lPosY = Str2Int(g_buf[nIndex++]);
		Boat.SetPos(lPosX, lPosY);
		Boat.SetAngle(Str2Int(g_buf[nIndex++]));
		// �ȼ�
		Boat.setAttr(ATTR_LV, Str2Int(g_buf[nIndex++]), 1);
		Boat.setAttr(ATTR_CEXP, Str2Int(g_buf[nIndex++]), 1);

	}
	else
		return FALSE;

	if (!ReadCabin(Boat))
		return FALSE;

    //  �����������½�ص���,��Ϊ�Ƿ�
    SItemGrid	*pGridCont = NULL;
    CItemRecord *pItem = NULL;
    Short sPos = 0;
    int i = 0;
    Short sUsedNum = Boat.m_CKitbag.GetUseGridNum();
	while(i < sUsedNum)
	{
		pGridCont = Boat.m_CKitbag.GetGridContByNum(i);
        if(pGridCont)
        {
            pItem = GetItemRecordInfo( pGridCont->sID );
            if(pItem)
            {
                if(enumITEM_PICKTO_KITBAG == pItem->chPickTo)
                {
                    sPos = Boat.m_CKitbag.GetPosIDByNum(i);
                    LG("boat_excp", "Character %s Remove %s.\n", Boat.GetName(), pItem->szName);
                    Boat.m_CKitbag.Pop(pGridCont, sPos);
                    sUsedNum = Boat.m_CKitbag.GetUseGridNum();
                    i = 0;
                    continue;
                    //return FALSE;
                }
            }
        }
        i++;
	}

	return TRUE;
}


BOOL CTableBoat::SaveBoatTempData( DWORD dwBoatID, DWORD dwOwnerID, BYTE byIsDeleted )
{
	sprintf( g_sql, "update %s set boat_ownerid=%d, boat_isdeleted=%d\
					where boat_id=%d", _get_table(), dwOwnerID, byIsDeleted, dwBoatID );

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return FALSE;
	}
	short sExec = exec_sql_direct( g_sql );
	return DBOK(sExec) && !DBNODATA(sExec);
}

BOOL CTableBoat::SaveBoatDelTag( DWORD dwBoatID, BYTE byIsDeleted )
{
	sprintf( g_sql, "update %s set boat_isdeleted=%d\
					where boat_id=%d", _get_table(), byIsDeleted, dwBoatID );

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return FALSE;
	}
	short sExec = exec_sql_direct( g_sql );
	return DBOK(sExec) && !DBNODATA(sExec);
}

BOOL CTableBoat::SaveBoatTempData( CCharacter& Boat, BYTE byIsDeleted )
{
	DWORD dwBoatID = (DWORD)Boat.getAttr( ATTR_BOAT_DBID );
	USHORT sDieCount = (USHORT)Boat.getAttr( ATTR_BOAT_DIECOUNT );
	BYTE byIsDead = (BYTE)Boat.getAttr( ATTR_BOAT_ISDEAD );
	DWORD dwOwnerID = Boat.GetPlayer()->GetDBChaId();

	sprintf( g_sql, "update %s set boat_diecount=%d, boat_isdead=%d , boat_ownerid=%d, boat_isdeleted=%d\
					where boat_id=%d", _get_table(), sDieCount, byIsDead, dwOwnerID, byIsDeleted, dwBoatID );

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return FALSE;
	}
	short sExec = exec_sql_direct( g_sql );
	return DBOK(sExec) && !DBNODATA(sExec);
}

BOOL CTableBoat::SaveBoat( CCharacter& Boat, char chSaveType )
{
	//LG("enter_map", "�� %s (%s)��ʼ���ñ�������.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());
	DWORD dwBoatID = (DWORD)Boat.getAttr( ATTR_BOAT_DBID );
	USHORT sBerthID = (USHORT)Boat.getAttr( ATTR_BOAT_BERTH );
	if (chSaveType == enumSAVE_TYPE_OFFLINE) // ����
		g_skillstate[0] = '\0';
	else // �л���ͼ
	{
		if (!SStateData2String(&Boat, g_skillstate, defSSTATE_DATE_STRING_LIN, chSaveType))
		{
			//LG("enter_map", "�� %s (%s)��֯״̬���ݲ��ɹ�.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());
			LG("enter_map", "boat %s (%s)organize state data failed.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());
			return false;
		}
	}
	g_kitbag[0] = '\0';
	KitbagData2String( &Boat.m_CKitbag, g_kitbag, defKITBAG_DATA_STRING_LEN );
	//LG("enter_map", "����״̬���ݳɹ�.\n");

	bool bWithPos = false;
	if (Boat.GetPlyCtrlCha()->GetSubMap())
		bWithPos = Boat.GetPlyCtrlCha()->GetSubMap()->CanSavePos();
	if (bWithPos)
		sprintf( g_sql, "update %s set boat_berth=%d, boat_ownerid=%d, cur_endure=%d, mx_endure=%d, cur_supply=%d, mx_supply=%d, skill_state='%s', \
						map='%s', map_x=%d, map_y=%d, angle=%d, degree=%d, exp=%d, boat_bag='%s' \
						where boat_id=%d",
			_get_table(),
			sBerthID,
			( Boat.GetPlayer() ) ? Boat.GetPlayer()->GetDBChaId() : 0,
			(long)Boat.getAttr(ATTR_HP),
			(long)Boat.getAttr(ATTR_BMXHP),
			(long)Boat.getAttr(ATTR_SP),
			(long)Boat.getAttr(ATTR_BMXSP),
			g_skillstate,
			Boat.GetBirthMap(),
			Boat.GetPos().x,
			Boat.GetPos().y,
			Boat.GetAngle(),
			(long)Boat.getAttr(ATTR_LV),
			(long)Boat.getAttr(ATTR_CEXP),
			g_kitbag,
			dwBoatID );
	else
		sprintf( g_sql, "update %s set boat_berth=%d, boat_ownerid=%d, cur_endure=%d, mx_endure=%d, cur_supply=%d, mx_supply=%d, skill_state='%s', \
						degree=%d, exp=%d, boat_bag='%s' \
						where boat_id=%d",
			_get_table(),
			sBerthID,
			( Boat.GetPlayer() ) ? Boat.GetPlayer()->GetDBChaId() : 0,
			(long)Boat.getAttr(ATTR_HP),
			(long)Boat.getAttr(ATTR_BMXHP),
			(long)Boat.getAttr(ATTR_SP),
			(long)Boat.getAttr(ATTR_BMXSP),
			g_skillstate,
			(long)Boat.getAttr(ATTR_LV),
			(long)Boat.getAttr(ATTR_CEXP),
			g_kitbag,
			dwBoatID );
	//LG("enter_map", "��֯SQL���ɹ�.\n");

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return FALSE;
	}
	short sExec = exec_sql_direct( g_sql );
	//LG("enter_map", "ִ��SQL���ɹ�.\n");

	if (!DBOK(sExec))
	{
		//LG("enter_map", "�� %s (%s)����������ݲ��ɹ�.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());
		LG("enter_map", "boat %s (%s)save basic data failed.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ�������%u!\n", dwBoatID);
		LG("enter_map", "Database couldn't find the character%u!\n",dwBoatID);
		return false;
	}
	//LG("enter_map", "������������ݳɹ�.\n");

	//if (!SaveCabin(Boat, chSaveType))
	//	return false;

	//LG("enter_map", "�� %s (%s)ȫ�����ݱ���ɹ�.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());
	LG("enter_map", "boat %s (%s) the whole data save succeed.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());

	return true;
}

bool CTableBoat::SaveAllData(CPlayer *pPlayer, char chSaveType)
{
	if (!pPlayer)
		return false;
	CCharacter	*pCBoat;
	for (BYTE i = 0; i < pPlayer->GetNumBoat(); i++)
	{
		pCBoat = pPlayer->GetBoat(i);
		if (!pCBoat)
			continue;
		if (!SaveBoat(*pCBoat, chSaveType))
			return false;
	}
	//LG("enter_map", "�������д����ݳɹ�.\n");
	LG("enter_map", "save the whole boat data succeed\n");

	return true;
}

bool CTableBoat::SaveCabin(CCharacter& Boat, char chSaveType)
{
	DWORD dwBoatID = (DWORD)Boat.getAttr( ATTR_BOAT_DBID );
	int kb_capacity = Boat.m_CKitbag.GetCapacity();
	g_kitbag[0] = '\0';
	KitbagData2String( &Boat.m_CKitbag, g_kitbag, defKITBAG_DATA_STRING_LEN );

	sprintf( g_sql, "update %s set boat_bag='%s' where boat_id=%d", _get_table(),
		g_kitbag,
		dwBoatID );

	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec = exec_sql_direct( g_sql );

	if (!DBOK(sExec))
	{
		//LG("enter_map", "�� %s (%s)�Ĵ������ݱ��治�ɹ�.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());
		LG("enter_map", "boat %s (%s) cabin data save failed.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());
		return false;
	}
	if (DBNODATA(sExec))
	{
		//LG("enter_map", "���ݿ�û�в�ѯ�������%u!\n", dwBoatID);
		LG("enter_map", "Database couldn't find the character%u!\n", dwBoatID);
		return false;
	}
	//LG("enter_map", "�� %s (%s)�Ĵ������ݱ���ɹ�.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());
	LG("enter_map", "boat %s (%s)cabin data save succeed.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());

	return true;
}

bool CTableBoat::SaveAllCabin(CPlayer *pPlayer, char chSaveType)
{
	CCharacter	*pCBoat;
	for (BYTE i = 0; i < pPlayer->GetNumBoat(); i++)
	{
		pCBoat = pPlayer->GetBoat(i);
		if (!pCBoat)
			continue;
		if (!SaveCabin(*pCBoat, chSaveType))
			return false;
	}

	return true;
}

bool CTableBoat::ReadCabin(CCharacter& Boat) // ��ȡ����
{
	DWORD dwBoatID = (DWORD)Boat.getAttr( ATTR_BOAT_DBID );
	int nIndex = 0;
	char param[] = "boat_bag";
	char filter[80]; 
	sprintf(filter, "boat_id=%d", dwBoatID );
	int r = _get_row3( g_buf, g_cnCol, param, filter );
	int	r1 = get_affected_rows();
	if (DBOK(r) && r1 > 0)
	{
		//LG("enter_map", "�� %u (%s, %s)�Ĵ������� %s.\n", dwBoatID, Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName(), g_buf[nIndex].c_str());
		if (!Boat.String2KitbagData(g_buf[nIndex++]))
		{
			//LG("enter_map", "��������У��ʹ���.\n");
			LG("enter_map", "cabin data check sum error.\n");
			//LG("У��ʹ���", "����%s���������ݣ�boat_id %u��У��ʹ���.\n", Boat.GetLogName(), Boat.getAttr( ATTR_BOAT_DBID ));
			LG("check sum error", "boat (%s) cabin data (boat_id %u)check sum error.\n", Boat.GetLogName(), Boat.getAttr( ATTR_BOAT_DBID ));
			return false;
		}

		//LG("enter_map", "�� %s (%s)�Ĵ����������óɹ�.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());
		LG("enter_map", "boat %s (%s) cabin data set succeed.\n", Boat.GetLogName(), Boat.GetPlyMainCha()->GetLogName());
		return true;
	}

	return false;
}

BOOL CGameDB::Init()
{
	m_bInitOK = FALSE;

    _connect.enable_errinfo();

	static const char* s_szDsn = "DRIVER={ODBC Driver 17 for SQL Server};SERVER=localhost;DATABASE=gamedb;Trusted_Connection=Yes;";
	printf("Connecting database [%s]... ", s_szDsn);

	string err_info;

	bool r = _connect.connect(s_szDsn, err_info);
    if(!r)
    {
		MessageBox(NULL, "Database Connection Failed!", "Database Connection Error" , MB_ICONERROR | MB_OK);
		LG("gamedb", "Database Connect Failed!, ERROR REPORT[%s]", err_info.c_str() );
        return FALSE;
    }
	C_PRINT("sucess!\n");

    _tab_cha   = new CTableCha(&_connect);
	_tab_master = new CTableMaster(&_connect);
	_tab_res   = new CTableResource(&_connect);

	// Add by lark.li 20080521 begin
	_tab_setting   = new CTableLotterySetting(&_connect);
	_tab_ticket   = new CTableTicket(&_connect);
	_tab_winticket   = new CTableWinTicket(&_connect);

	_tab_amp_setting = new CTableAmphitheaterSetting(&_connect);
	_tab_amp_team = new CTableAmphitheaterTeam(&_connect);
	// End
	
	_tab_mmask = new CTableMapMask(&_connect);
	_tab_act   = new CTableAct(&_connect);
	_tab_gld   = new CTableGuild(&_connect);
	_tab_boat  = new CTableBoat(&_connect);
	_tab_log   = new CTableLog(&_connect);
	_tab_item	=	new	CTableItem(&_connect);

	// Modify by lark.li 20080521
	//if (!_tab_cha || !_tab_act || !_tab_gld || !_tab_boat || !_tab_master)
	if (!_tab_cha || !_tab_act || !_tab_gld || !_tab_boat || !_tab_master 
			|| !_tab_setting || !_tab_ticket || !_tab_winticket
			|| !_tab_amp_setting || !_tab_amp_team || !_tab_log || !_tab_item)
		return FALSE;

	//if (!_tab_cha->Init() || !_tab_act->Init() || !_tab_gld->Init() || !_tab_boat->Init() || !_tab_res->Init() || !_tab_mmask->Init() || !_tab_master->Init())
	if (!_tab_cha->Init() || !_tab_act->Init() || !_tab_gld->Init() || !_tab_boat->Init() || !_tab_res->Init() 
			|| !_tab_mmask->Init() || !_tab_master->Init() || !_tab_setting->Init() || !_tab_ticket->Init()|| !_tab_winticket->Init()
			|| !_tab_amp_setting->Init() || !_tab_amp_team->Init())
		return FALSE;

	//int issue;
	//_tab_setting->GetCurrentIssue(issue);
	//_tab_setting->AddIssue(10);
	//_tab_setting->DisuseIssue(1, enumDISUSE);
	//_tab_ticket->AddTicket(1 , "123456", 1);
	//_tab_ticket->AddTicket(1 , "12345X", 1);
	//_tab_ticket->AddTicket(1 , "12345X", 1);
	//_tab_ticket->Exchange(1 , "123456");

	if(!_tab_log)
	{
		//LG("init", "gamelog���ݱ���ʼ��ʧ��\n");
		LG("init", "gamelog data list init failed\n");
	}
	
	m_bInitOK = TRUE;

	return TRUE;
}

bool CGameDB::ReadPlayer(CPlayer *pPlayer, DWORD atorID)
{
	if (!_tab_cha->ReadAllData(pPlayer, atorID))
		return false;

	long	lKbDBID = pPlayer->GetMainCha()->GetKitbagRecDBID();
    long    lkbTmpDBID = pPlayer->GetMainCha()->GetKitbagTmpRecDBID();//��ʱ����ID
	long	lMMaskDBID = pPlayer->GetMapMaskDBID();
	long	lBankNum = pPlayer->GetCurBankNum();
	if (!_tab_res->ReadKitbagData(pPlayer->GetMainCha()))
		return false;
	if (lKbDBID == 0)
		if (!SavePlayerKBagDBID(pPlayer))
			return false;

    if(!_tab_res->ReadKitbagTmpData(pPlayer->GetMainCha()))
        return false;
    if(lkbTmpDBID == 0)
        if(!SavePlayerKBagTmpDBID(pPlayer))
            return false;
	pPlayer->GetMainCha()->LogAssets(enumLASSETS_INIT);

	if (!_tab_res->ReadBankData(pPlayer))
		return false;
	if (lBankNum == 0)
		if (!_tab_cha->SaveBankDBID(pPlayer))
			return false;

	//if (g_Config.m_chMapMask > 0)
	{
		// ��ͼ����ʧ�ܣ���Ӱ��������Ϸ
		_tab_mmask->ReadData(pPlayer);
		if (lMMaskDBID == 0)
			SavePlayerMMaskDBID(pPlayer);
	}

	if (!_tab_act->ReadAllData(pPlayer, pPlayer->GetDBActId()))
		return false;

	// �л���Ϣ
	if (pPlayer->m_lGuildID > 0)
	{
		_tab_gld->GetGuildInfo(pPlayer->GetMainCha(), pPlayer->m_lGuildID);
		//long	lType = _tab_gld->GetTypeByID(pPlayer->GetMainCha()->getAttr(ATTR_GUILD));
		//if (lType >= 0)
		//	pPlayer->GetMainCha()->setAttr(ATTR_GUILD_TYPE, lType, 1);
	}
	//LG("enter_map", "�����ȫ�����ݳɹ�.\n");
	LG("enter_map", "Load the character whole data succeed.\n");

	// ��¼���ļ�
	CKitbag		*pCKb;
	CCharacter	*pCMainC = pPlayer->GetMainCha();
	short	sItemNum = pCMainC->m_CKitbag.GetUseGridNum();
	g_kitbag[0] = '\0';
	//sprintf(g_kitbag, "��Ǯ��%u ���ߣ�%d@", pCMainC->getAttr(ATTR_GD), sItemNum);
	sprintf(g_kitbag, RES_STRING(GM_GAMEDB_CPP_00021), pCMainC->getAttr(ATTR_GD), sItemNum);
	SItemGrid *pGridCont;
	CItemRecord *pCItem;
	pCKb = &(pCMainC->m_CKitbag);
	for (short i = 0; i < sItemNum; i++)
	{
		pGridCont = pCKb->GetGridContByNum(i);
		if (!pGridCont)
			continue;
		pCItem = GetItemRecordInfo(pGridCont->sID);
		if (!pCItem)
			continue;
		sprintf(g_kitbag + strlen(g_kitbag), "%s[%d],%d;", pCItem->szName, pGridCont->sID, pGridCont->sNum);
	}
	TL(CHA_ENTER, pCMainC->GetName(), "", g_kitbag);

    short	sItemTmpNum = pCMainC->m_pCKitbagTmp->GetUseGridNum();
	g_kitbagTmp[0] = '\0';
	//sprintf(g_kitbagTmp, "��ʱ������%d@", sItemTmpNum);
	sprintf(g_kitbagTmp, RES_STRING(GM_GAMEDB_CPP_00022), sItemTmpNum);
	pCKb = pCMainC->m_pCKitbagTmp;
	for (short i = 0; i < sItemTmpNum; i++)
	{
		pGridCont = pCKb->GetGridContByNum(i);
		if (!pGridCont)
			continue;
		pCItem = GetItemRecordInfo(pGridCont->sID);
		if (!pCItem)
			continue;
		sprintf(g_kitbagTmp + strlen(g_kitbagTmp), "%s[%d],%d;", pCItem->szName, pGridCont->sID, pGridCont->sNum);
	}
	TL(CHA_ENTER, pCMainC->GetName(), "", g_kitbagTmp);

	char	chStart = 0, chEnd = pPlayer->GetCurBankNum() - 1;
	for (char i = chStart; i <= chEnd; i++)
	{
		//sprintf(g_kitbag, "����ID(%d):", i + 1 );
		sprintf(g_kitbag, RES_STRING(GM_GAMEDB_CPP_00023), i + 1 );
		pCKb = pPlayer->GetBank(i);
		sItemNum = pCKb->GetUseGridNum();
		sprintf(g_kitbag + strlen(g_kitbag), "[%d]%d@;", i + 1, sItemNum);
		for (short i = 0; i < sItemNum; i++)
		{
			pGridCont = pCKb->GetGridContByNum(i);
			if (!pGridCont)
				continue;
			pCItem = GetItemRecordInfo(pGridCont->sID);
			if (!pCItem)
				continue;
			sprintf(g_kitbag + strlen(g_kitbag), "%s[%d],%d;", pCItem->szName, pGridCont->sID, pGridCont->sNum);
		}
		TL(CHA_ENTER, pCMainC->GetName(), "", g_kitbag);
	}

	g_equip[0] = '\0';
	//sprintf(g_equip, "װ����%d@", enumEQUIP_NUM);
	sprintf(g_equip, RES_STRING(GM_GAMEDB_CPP_00024), enumEQUIP_NUM);
	for (short i = 0; i < enumEQUIP_NUM; i++)
	{
		pGridCont = &pCMainC->m_SChaPart.SLink[i];
		if (!pGridCont)
			continue;
		pCItem = GetItemRecordInfo(pGridCont->sID);
		if (!pCItem)
			continue;
		sprintf(g_equip + strlen(g_equip), "%s[%d],%d;", pCItem->szName, pGridCont->sID, pGridCont->sNum);
	}
	TL(CHA_EQUIP, pCMainC->GetName(), "", g_equip);
	
	//
	return true;
}

bool CGameDB::SavePlayer(CPlayer *pPlayer, char chSaveType)
{
	if (!pPlayer || !pPlayer->GetMainCha())
		return FALSE;

	if (pPlayer->GetMainCha()->GetPlayer() != pPlayer)
	{
		//LG("�����ɫ�ش����", "�����Player��ַ %p[dbid %u]������ҵ����� %s���ý�ɫ��Player��ַ %p��\n",
		LG("save character great error", "save Player address %p[dbid %u]��the character main player %s��the character 's Player address %p��\n",		
			pPlayer, pPlayer->GetDBChaId(), pPlayer->GetMainCha()->GetLogName(), pPlayer->GetMainCha()->GetPlayer());
		//pPlayer->SystemNotice("��ҽ�ɫ��ƥ�䣬�������Ͽⲻ�ɹ�");
		pPlayer->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00025));
		return FALSE;
	}

	bool	bSaveMainCha = false, bSaveBoat = false, bSaveKitBag = false, bSaveMMask = false, bSaveBank = false;
    bool    bSaveKitBagTmp = false;
    bool    bSaveKBState = false;
	BeginTran();
	try
	{
		DWORD	dwStartTick = GetTickCount();

		bSaveMainCha = _tab_cha->SaveAllData(pPlayer, chSaveType); // ��������ɫ
		DWORD	dwSaveMainTick = GetTickCount();
		bSaveKitBag = _tab_res->SaveKitbagData(pPlayer->GetMainCha());
        //������ʱ����
        bSaveKitBagTmp = _tab_res->SaveKitbagTmpData(pPlayer->GetMainCha());
        //������������״̬
        //bSaveKBState = _tab_cha->SaveKBState(pPlayer);
		DWORD	dwSaveKbTick = GetTickCount();
		bSaveBank = _tab_res->SaveBankData(pPlayer);
		DWORD	dwSaveBankTick = GetTickCount();
		if ((chSaveType != enumSAVE_TYPE_TIMER) && (g_Config.m_chMapMask > 0))
		{
			if (pPlayer->IsMapMaskChange())
			{
				bSaveMMask = _tab_mmask->SaveData(pPlayer);
				pPlayer->ResetMapMaskChange();
			}
		}
		else
			bSaveMMask = true;
		DWORD	dwSaveMMaskTick = GetTickCount();
		bSaveBoat = _tab_boat->SaveAllData(pPlayer, chSaveType); // ���洬
		DWORD	dwSaveBoatTick = GetTickCount();

		//LG("�������ݺ�ʱ", "�ܼ�%-8d������ɫ%-8d������ɫ����%-8d������%-8d�����ͼ%-8d����%-8d.[%d %s]\n",
		LG("save data waste time", "totalize %-8d��main character %-8d��main character kitbag %-8d��bank %-8d��big map %-8d��boat %-8d.[%d %s]\n",
			dwSaveBoatTick - dwStartTick, dwSaveMainTick - dwStartTick, dwSaveKbTick - dwSaveMainTick, dwSaveBankTick - dwSaveKbTick, dwSaveMMaskTick - dwSaveBankTick, dwSaveBoatTick - dwSaveMMaskTick,
			pPlayer->GetDBChaId(), pPlayer->GetMainCha()->GetLogName());
	}
	catch (...)
	{
		//LG("enter_map", "�������ȫ�����ݵĹ����з����쳣.\n");
		LG("enter_map", "It's abnormity when saving the character's whole data.\n");
	}

	if (!bSaveMainCha || !bSaveBoat || !bSaveKitBag
		)
	{
		RollBack();
		return false;
	}
	CommitTran();

	//LG("enter_map", "��������������ݳɹ�.\n");
	LG("enter_map", "save character whole data succeed.\n");
	// ��¼���ļ�
	if (chSaveType != enumSAVE_TYPE_TIMER)
	{
		CKitbag		*pCKb;
		CCharacter	*pCMainC = pPlayer->GetMainCha();
		short	sItemNum = pCMainC->m_CKitbag.GetUseGridNum();
		g_kitbag[0] = '\0';
		//sprintf(g_kitbag, "��Ǯ��%u ��������%d@", pCMainC->getAttr(ATTR_GD), sItemNum);
		sprintf(g_kitbag, RES_STRING(GM_GAMEDB_CPP_00026), pCMainC->getAttr(ATTR_GD), sItemNum);
		SItemGrid *pGridCont;
		CItemRecord *pCItem;
		pCKb = &(pCMainC->m_CKitbag);
		for (short i = 0; i < sItemNum; i++)
		{
			pGridCont = pCKb->GetGridContByNum(i);
			if (!pGridCont)
				continue;
			pCItem = GetItemRecordInfo(pGridCont->sID);
			if (!pCItem)
				continue;
			sprintf(g_kitbag + strlen(g_kitbag), "%s[%d],%d;", pCItem->szName, pGridCont->sID, pGridCont->sNum);
		}
		TL(CHA_OUT, pCMainC->GetName(), "", g_kitbag);

        short	sItemTmpNum = pCMainC->m_pCKitbagTmp->GetUseGridNum();
		g_kitbagTmp[0] = '\0';
		//sprintf(g_kitbagTmp, "��ʱ������%d@", sItemTmpNum);
		sprintf(g_kitbagTmp, RES_STRING(GM_GAMEDB_CPP_00022), sItemTmpNum);
		pCKb = pCMainC->m_pCKitbagTmp;
		for (short i = 0; i < sItemTmpNum; i++)
		{
			pGridCont = pCKb->GetGridContByNum(i);
			if (!pGridCont)
				continue;
			pCItem = GetItemRecordInfo(pGridCont->sID);
			if (!pCItem)
				continue;
			sprintf(g_kitbagTmp + strlen(g_kitbagTmp), "%s[%d],%d;", pCItem->szName, pGridCont->sID, pGridCont->sNum);
		}
		TL(CHA_OUT, pCMainC->GetName(), "", g_kitbagTmp);

		g_equip[0] = '\0';
		//sprintf(g_equip, "װ����%d@", enumEQUIP_NUM);
		sprintf(g_equip, RES_STRING(GM_GAMEDB_CPP_00024), enumEQUIP_NUM);
		for (short i = 0; i < enumEQUIP_NUM; i++)
		{
			pGridCont = &pCMainC->m_SChaPart.SLink[i];
			if (!pGridCont)
				continue;
			pCItem = GetItemRecordInfo(pGridCont->sID);
			if (!pCItem)
				continue;
			sprintf(g_equip + strlen(g_equip), "%s[%d],%d;", pCItem->szName, pGridCont->sID, pGridCont->sNum);
		}
		TL(CHA_EQUIP, pCMainC->GetName(), "", g_equip);

		char	chStart = 0, chEnd = pPlayer->GetCurBankNum() - 1;
		//sprintf(g_kitbag, "����ID(%d):", pPlayer->GetCurBankNum());
		sprintf(g_kitbag, RES_STRING(GM_GAMEDB_CPP_00023), pPlayer->GetCurBankNum());
		for (char i = chStart; i <= chEnd; i++)
		{
			pCKb = pPlayer->GetBank(i);
			sItemNum = pCKb->GetUseGridNum();
			sprintf(g_kitbag + strlen(g_kitbag), "[%d]%d@;", i + 1, sItemNum);
			for (short i = 0; i < sItemNum; i++)
			{
				pGridCont = pCKb->GetGridContByNum(i);
				if (!pGridCont)
					continue;
				pCItem = GetItemRecordInfo(pGridCont->sID);
				if (!pCItem)
					continue;
				sprintf(g_kitbag + strlen(g_kitbag), "%s[%d],%d;", pCItem->szName, pGridCont->sID, pGridCont->sNum);
			}
		}
		TL(CHA_BANK, pCMainC->GetName(), "", g_kitbag);
	}
	//

	return true;
}

/*
// �����Ϊд����־
#include "lua_gamectrl.h"
extern char g_TradeName[][8]; 
#include "SystemDialog.h"

void CGameDB::Log(const char *type, const char *c1, const char *c2, const char *c3, const char *c4, const char *p, BOOL bAddToList)
{
	if(g_Config.m_bLogDB==FALSE) return;
	
	if(!_tab_log) return;

	char szSQL[8192];

	sprintf(szSQL, "insert gamelog (action, c1, c2, c3, c4, content) \
				   values('%s', '%s', '%s', '%s', '%s', '%s')", type, c1, c2, c3, c4, p);
	
	//if(bAddToList)
	{
		// ʹ��SendMessage��֤�̰߳�ȫ, ��Ϊֻ���ַ�������, ���Բ���Ӱ�쵽���̵߳�Ч��
	//	extern HWND g_SysDlg;
	//	PostMessage(g_SysDlg, WM_USER_LOG, 0, 0);
	}
	//else
	{
		ExecLogSQL(szSQL);
	}
}

void CGameDB::Log1(int nType, const char *cha1, const char *cha2, const char *pszContent)
{
	Log( g_TradeName[nType], cha1, "", cha2, "", pszContent);
}


void CGameDB::Log2(int nType, CCharacter *pCha1, CCharacter *pCha2, const char *pszContent)
{
	if(!_tab_log) return;

	char szName1[32]    = "";
	char szName2[32]    = "";
	char szActName1[32] = "";
	char szActName2[32] = "";

	if(pCha1) 
	{ 
		strcpy(szName1, pCha1->GetName());
		if(pCha1->GetPlayer()) strcpy(szActName1, pCha1->GetPlayer()->GetActName());
	}
	if(pCha2) 
	{ 
		strcpy(szName2, pCha2->GetName());
		if(pCha2->GetPlayer()) strcpy(szActName1, pCha2->GetPlayer()->GetActName());
	}
	
	Log(g_TradeName[nType], szName1, szActName1, szName2, szActName2, pszContent);
}*/

//===============CTableGuild Begin===========================================
bool CTableGuild::Init(void)
{
	sprintf(g_sql, "select \
				guild_id, guild_name, motto, passwd, leader_id, exp, member_total,\
				try_total, disband_date \
				from %s \
				(nolock) where 1=2", \
				_get_table());
	if (strlen(g_sql) >= SQL_MAXLEN)
	{
		//FILE	*pf = fopen("log\\SQL��䳤��Խ��.txt", "a+");
		FILE	*pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf)
		{
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL��䳤��Խ��!\n");
		LG("enter_map", "SQL sentence length slop over\n");
		return false;
	}
	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
	{
		//MessageBox(0, "���ݿ�(guild)��ʼ����������", "����", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "guild");
		MessageBox(0,buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

long CTableGuild::Create(CCharacter* pCha, char *guildname, cChar *passwd)
{
	long l_ret_guild_id	=0;
	string buf[1]; 
	char sql[SQL_MAXLEN];

	while(true)
	{
		//��ȡ�չ����ID
		const char *param = "isnull(min(guild_id),0)";
		char filter[80]; sprintf(filter, "guild_id >0 and leader_id =0");
		bool ret = _get_row(buf, 1, param, filter);
		if (!ret)	
		{
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00027));
			LG("consortia system","found consortia system occur SQL operator error.");
			return 0;
		}
		l_ret_guild_id	=atol(buf[0].c_str());
		if(!l_ret_guild_id)		
		{
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00030));
			return 0;
		}

		

			sprintf(sql,	"update %s set leader_id =%d ,passwd ='%s', guild_name ='%s', exp =0,\
										member_total =1,try_total =0\
								where leader_id =0 and guild_id =%d",
				_get_table(), pCha->GetID(), passwd, guildname, l_ret_guild_id);
			SQLRETURN l_sqlret =exec_sql_direct(sql);
			if(!DBOK(l_sqlret))	//�������������ظ�
			{
				//pCha->SystemNotice("�������������ѱ�ʹ��");
				pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00031));
				return 0;
			}
			if(get_affected_rows() !=1)
			{
				continue;
			}
		

		break;
	}
	sprintf(sql,"update character set guild_id =%d,guild_stat =0,guild_permission =%d \
								where atorID =%d", l_ret_guild_id, emGldPermMax, pCha->GetID());
	exec_sql_direct(sql);

	WPACKET l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MP_GUILD_CREATE);
	WRITE_LONG(l_wpk,l_ret_guild_id);	//����ID
	WRITE_STRING(l_wpk,guildname);		//����Name
	WRITE_STRING(l_wpk,g_GetJobName(uShort(pCha->getAttr(ATTR_JOB))));//ְҵ
	WRITE_SHORT(l_wpk,uShort(pCha->getAttr(ATTR_LV)));		//�ȼ�
	pCha->ReflectINFof(pCha,l_wpk);

    return l_ret_guild_id;	//���ᴴ���ɹ�,���ع���ID
}

bool CTableGuild::ListAll(CCharacter* pCha ,char disband_days)
{
	
	const char *sql_syntax =0;
	if(!pCha || disband_days<1)
	{
		return false;
	}
	else{
		sql_syntax =
			"select gld.guild_id, gld.guild_name, gld.motto, gld.leader_id, cha.atorNome leader_name, gld.exp, gld.member_total "
			"from guild As gld, character As cha where gld.leader_id =cha.atorID";
	}
	bool ret = false;
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax, disband_days);

	// ִ�в�ѯ����
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try
	{
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO))
		{
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret);

			throw 1;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR *)sql, SQL_NTS);
		if (sqlret != SQL_SUCCESS)
		{
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);

			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		// Bind Column
		for (int i = 0; i < col_num; ++ i)
		{
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
		}

		WPACKET	l_wpk,l_wpk0 =GETWPACKET();
		WRITE_CMD(l_wpk0, CMD_MC_LISTGUILD);

		// Fetch each Row	int i; // ȡ��������
		int f_row = 1;
		for (; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO; ++ f_row)
		{
			if (sqlret != SQL_SUCCESS)
			{
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			}
			if((f_row %20) ==1)
			{
				l_wpk	=l_wpk0;
			}
			WRITE_LONG(l_wpk, atol((char const*)_buf[0]));	// guild id
			WRITE_STRING(l_wpk, (char const*)_buf[1]);		// guild name
			WRITE_STRING(l_wpk, (char const*)_buf[2]);		// guild motto
			WRITE_STRING(l_wpk, (char const*)_buf[4]);		// leader name
			WRITE_SHORT(l_wpk, atoi((const char*)_buf[6]));	// guild member count
			l_wpk.WriteInt64(std::atoll((const char*)_buf[5]));	// guild exp

			if(!(f_row %20))
			{
				WRITE_CHAR(l_wpk,((f_row-1)%20)+1);	//���ΰ���������
				pCha->ReflectINFof(pCha,l_wpk);
			}
		}
		if((f_row%20) ==1)
		{
			l_wpk	=l_wpk0;
		}
		WRITE_LONG(l_wpk,(f_row -1)%20);	//���ΰ���������
		pCha->ReflectINFof(pCha,l_wpk);

		SQLFreeStmt(hstmt, SQL_UNBIND);
		ret = true;
	}catch(int&e)
	{
		//LG("����ϵͳ", "�����������ODBC �ӿڵ��ô���λ���룺%d\n",e);
		LG("consortia system", "found consortia process ODBC interfance transfer error,position ID:%d\n",e);
	}catch (...)
	{
		//LG("����ϵͳ", "Unknown Exception raised when list all guilds\n");
		LG("consortia system", "Unknown Exception raised when list all guilds\n");
	}

	if (hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}
void CTableGuild::TryFor(CCharacter* pCha, uLong guildid)
{
	if( pCha->HasGuild() )
	{
		//pCha->SystemNotice( "���Ѿ��ǹ��ᡶ%s����Ա,�����ظ�������빫��!", pCha->GetGuildName() );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00032), pCha->GetGuildName() );
		return;
	}
	else if( guildid == pCha->GetGuildID() )
	{
		//pCha->SystemNotice( "�����ظ�������빫�ᡶ%s��!", pCha->GetGuildName() );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00033), pCha->GetGuildName() );
		return;
	}

	string buf[3];
	char filter[80];
	const char	*	param	="guild_id";
	sprintf(filter,"leader_id >0 and guild_id =%d",guildid);
	int l_ret =_get_row(buf,3,param,filter);
	if(!DBOK(l_ret))
	{
		//pCha->SystemNotice("������빫��ʧ��.");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00034));
		//LG("����ϵͳ","�û�[%s]������빫��[ID=%d]ʱ��SQLִ��ʧ��.\n",pCha->GetName(),guildid);
		LG("consortia system","character[%s]apply join in consortia [ID=%d] carry out SQL failed.\n",pCha->GetName(),guildid);
		return;
	}else if(get_affected_rows() !=1)
	{
		//pCha->SystemNotice("�������Ĺ����޻᳤.");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00035));
		return;
	}
	param	="c.guild_id, c.guild_stat, g.guild_name";
	string	l_tbl_name =_tbl_name;
	_tbl_name	="character c,guild g";
	sprintf(filter, "c.guild_id =g.guild_id and c.atorID =%d and g.guild_id <>%d",pCha->GetID(), guildid);
	l_ret = _get_row(buf, 3, param, filter);
	_tbl_name	=l_tbl_name;
	if(!DBOK(l_ret) || get_affected_rows() !=1)
	{
		//pCha->SystemNotice("������빫��ʧ��.");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00034));
		//LG("����ϵͳ","�û�[%s]������빫��[ID=%d]ʱ��SQLִ��ʧ��.\n",pCha->GetName(),guildid);
		LG("consortia system","character[%s]apply join in consortia [ID=%d] carry out SQL failed.\n",pCha->GetName(),guildid);
		return;
	}

	// ��ѯ�����빫����Ϣ
	string bufnew[1];
	param	="guild_name";
	sprintf(filter, "guild_id =%d",guildid);
	int	 l_retrow =0;
	l_ret = _get_row(bufnew, 1, param, filter, &l_retrow);
	if(l_retrow ==1)
	{
	}else
	{
		//LG( "����ϵͳ", "TryFor����ɫ%s���빫��ID[0x%X]������!", pCha->GetName(), guildid );
		LG( "consortia system", "TryFor: character %s apply consortia ID[0x%X]is inexistence!", pCha->GetName(), guildid );
		//pCha->SystemNotice( "������Ĺ��᲻����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00036) );
		return;
	}

	// ��ʱ�����ѯ���Ĺ�������
	strncpy( pCha->GetPlayer()->m_szTempGuildName, bufnew[0].c_str(), defGUILD_NAME_LEN - 1 );

	if (const auto guild_id = std::stoi(buf[0]); guild_id)
	{
		if (const auto status = std::stoi(buf[1]); status == emGldMembStatNormal)
		{
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00037),buf[2].c_str());
			return;
		}
		else if(status == emGldMembStatTry && pCha->GetPlayer()->m_GuildState.IsFalse(emGuildReplaceOldTry))
		{
			pCha->GetPlayer()->m_GuildState.SetBit(emGuildReplaceOldTry);
			pCha->GetPlayer()->m_lTempGuildID = guildid;
			WPACKET l_wpk =GETWPACKET();
			WRITE_CMD(l_wpk,CMD_MC_GUILD_TRYFORCFM);
			WRITE_STRING(l_wpk,buf[2].c_str());
			pCha->ReflectINFof(pCha,l_wpk);
			return;
		}
	}else
	{
		TryForConfirm(pCha,guildid);
	}
}
void CTableGuild::TryForConfirm(CCharacter* pCha, uLong guildid)
{
	char sql[SQL_MAXLEN];

	if( pCha->HasGuild() )
	{
		//pCha->SystemNotice( "���Ѿ��ǹ��ᡶ%s����Ա,�����ظ�������빫��!", pCha->GetGuildName() );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00038), pCha->GetGuildName() );
		return;
	}

	DWORD dwOldGuildID = pCha->GetGuildID();

		// ��ʼ����
	if( !begin_tran() )
	{
		//pCha->SystemNotice( "���빫�����ݲ���ʧ��!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00039) );
		return;
	}

	sprintf(sql,	"update character set guild_id =%d ,guild_stat =1,guild_permission =0\
						where atorID =%d and\
								%d in (select guild_id from guild where leader_id >0 and guild_id =%d and try_total <%d and member_total <%d)",
								guildid, pCha->GetID(), guildid, guildid, emMaxTryMemberNum, emMaxMemberNum);
	SQLRETURN l_sqlret =exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//pCha->SystemNotice( "������빫�����ʧ�ܣ����ܸù��������������������Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00040) );
		return;
	}

	sprintf(sql,	"update guild set try_total =try_total +1 where guild_id =%d",guildid);
	l_sqlret =exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//pCha->SystemNotice( "������빫�����ʧ�ܣ����ܸù��������������������Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00040) );
		return;
	}

	// �ж��Ƿ��ǿͻ��˸������빫��
	if( dwOldGuildID && pCha->GetPlayer()->m_GuildState.IsTrue(emGuildReplaceOldTry) )
	{
		sprintf(sql,"update guild set try_total =try_total -1 where guild_id =%d and try_total > 0"
			,dwOldGuildID);
		SQLRETURN l_sqlret = exec_sql_direct(sql);
		if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
		{
			this->rollback();
			//pCha->SystemNotice( "�����˳�ԭ�й������ʧ�ܣ����Ժ�����!" );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00041) );
			return;
		}
	}

	if( !commit_tran() )
	{
		this->rollback();
		//pCha->SystemNotice( "������빫�����ʧ�ܣ����ܸù��������������������Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00040) );
		return;
	}

	// �����¹���������Ϣ
	pCha->SetGuildID( guildid );
	pCha->SetGuildState( emGldMembStatTry );

	pCha->SetGuildName( pCha->GetPlayer()->m_szTempGuildName );
	//pCha->SystemNotice( "��ϲ!��������빫�ᡶ%s���ɹ�,�����ĵȴ��������Ա������.", pCha->GetGuildName() );
	pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00042), pCha->GetGuildName() );
}


bool CTableGuild::GetGuildBank(uLong guildid, CKitbag * bag){
	string buf[3];
	char filter[80];
	const char*param = "bank";
	sprintf(filter, "guild_id =%d", guildid);
	int	 l_retrow = 0;
	bool l_ret = _get_row(buf, 1, param, filter, &l_retrow);
	if (l_retrow == 1)
	{
		if (buf[0].length() == 0){
			bag->SetCapacity(48);
			return true;
		}
		if (String2KitbagData(bag, buf[0])){
			return true;
		}
	}
	return false;
}

int CTableGuild::GetGuildLeaderID(uLong guildid){
	string buf[3];
	char filter[80];
	const char*param = "leader_id";
	sprintf(filter, "guild_id =%d", guildid);
	int	 l_retrow = 0;
	bool l_ret = _get_row(buf, 1, param, filter, &l_retrow);
	if (l_retrow == 1)
	{
		return atoi(buf[0].c_str());
	}
	return 0;
}

bool CTableGuild::UpdateGuildBank(uLong guildid, CKitbag * bag){
	char sql[SQL_MAXLEN];
	char bagStr[defKITBAG_DATA_STRING_LEN];
	if (KitbagData2String(bag, bagStr, defKITBAG_DATA_STRING_LEN)){
		sprintf(sql, "update guild set bank = '%s' where guild_id =%d", bagStr, guildid);
		SQLRETURN l_sqlret = exec_sql_direct(sql);
		if (!DBOK(l_sqlret) || get_affected_rows() == 0){
			//this->rollback(); // dont think we need to rollback??
			return false;
		}
		return true;
	}
	return false;
}

bool CTableGuild::UpdateGuildBankGold(int guildID, int money){
	char sql[SQL_MAXLEN];
	sprintf(sql, "update guild set gold = gold + %d where guild_id =%d", money, guildID);
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0){
		return false;
	}
	return true;
}

unsigned long long CTableGuild::GetGuildBankGold(uLong guildid){
	string buf[1];
	char filter[80];
	const char*param = "gold";
	sprintf(filter, "guild_id =%d", guildid);
	int	 l_retrow = 0;
	bool l_ret = _get_row(buf, 1, param, filter, &l_retrow);
	if (l_retrow == 1){
		return stoll(buf[0]);
	}
	return false;
}

std::vector<CTableGuild::BankLog> CTableGuild::GetGuildLog(uLong guildid) {
	string buf[1];
	string logList[1024]; //Max number of strings (1 log = 5 strings)
	std::vector<CTableGuild::BankLog> logs;

	char filter[SQL_MAXLEN];
	const char* param = "banklog";
	sprintf(filter, "guild_id =%d", guildid);
	int	 l_retrow = 0;
	bool l_ret = _get_row(buf, 1, param, filter, &l_retrow);
	if (l_retrow == 1) {

		int n = Util_ResolveTextLine(buf[0].c_str(), logList,1024, '-', ';');
		int i = 0;
		while(i < n){
				BankLog p;
				p.type = Str2Int(logList[i].c_str());
				p.time = Str2Int(logList[i + 1].c_str());
				p.parameter = Str2Int(logList[i + 2].c_str());
				p.quantity = Str2Int(logList[i + 3].c_str());
				p.userID = Str2Int(logList[i + 4].c_str());
				logs.push_back(p);
				i += 5;
		}
	}

	if (logs.size() == 200) {
		logs.erase(logs.begin());		// size is 200, let's erase the first log
	}

	return logs;
	}

bool CTableGuild::SetGuildLog(std::vector<BankLog> log, uLong guild_id) {
	char sql[SQL_MAXLEN];
	char data[8000];
	data[0] = '\0';
	for (int i = 0; i < log.size(); i++) {
		if (log.at(i).userID == 0) {
			continue;
		}
		char buf[100];
		//if (i != log.size() - 1) {
		if(true){
			sprintf(buf, "%d-%lld-%lld-%d-%d;", log.at(i).type, log.at(i).time, log.at(i).parameter, log.at(i).quantity, log.at(i).userID);
			//printf(buf);
		}
		strcat(data, buf);
	}

	sprintf(sql, "update guild set banklog = '%s' where guild_id =%d", data, guild_id);
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		return false;
	}
	return true;


}









bool CTableGuild::GetGuildInfo(CCharacter* pCha, uLong guildid )
{
	string buf[4];
	char filter[80];

	const char*param	="guild_name, motto";
	sprintf(filter, "guild_id =%d",guildid);
	int	 l_retrow =0;
	bool l_ret = _get_row(buf, 2, param, filter, &l_retrow);
	if(l_retrow ==1)
	{
		pCha->SetGuildName( buf[0].c_str() );
		pCha->SetGuildMotto( buf[1].c_str() );
		return true;
	}else
	{
		return false;
	}
}

bool CTableGuild::ListTryPlayer(CCharacter* pCha, char disband_days)
{
	bool ret = false;

	if(!pCha || !pCha->HasGuild())
	{
		return ret;
	}

	string buf[10];
	char filter[80];

	const char *sql_syntax = "g.guild_id, g.guild_name,g.motto, c.atorNome, g.member_total,g.exp, g.level";

	char param[500];
	//sprintf(param , sql_syntax , disband_days);
	sprintf(param, sql_syntax);

	string	l_tbl_name =_tbl_name;
	_tbl_name	="character c,guild g";
	sprintf(filter, "g.leader_id =c.atorID and g.guild_id =%d",pCha->GetGuildID());
	int	 l_retrow =0;

	bool l_ret = _get_row(buf, 10, param, filter, &l_retrow);
	_tbl_name	=l_tbl_name;
	if(!l_ret || !l_retrow || this->get_affected_rows() !=1)
	{
		return ret;
	}
	WPACKET		l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk, CMD_MC_GUILD_LISTTRYPLAYER);
	WRITE_LONG(l_wpk,	atol(buf[0].c_str()));	//guild_id 1
	WRITE_STRING(l_wpk,		 buf[1].c_str());	//guild_name 2
	WRITE_STRING(l_wpk,		 buf[2].c_str());	//motto 3
	WRITE_STRING(l_wpk,		 buf[3].c_str());	//atorNome 4
	WRITE_SHORT(l_wpk,	atoi(buf[4].c_str()));	//member_total 5
	WRITE_SHORT(l_wpk,	g_Config.m_sGuildNum);		//6
	l_wpk.WriteInt64(_atoi64(buf[5].c_str())); //7 exp

	WRITE_LONG(l_wpk, 0); //8
	WRITE_LONG(l_wpk, atol(buf[6].c_str())); //level 9

	sql_syntax =
		"select c.atorID,c.atorNome,c.job,c.degree\
			from character c\
			where (c.guild_stat =1 and c.guild_id =%d and c.delflag =0)\
		";
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax, pCha->GetGuildID());

	// ִ�в�ѯ����
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try
	{
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO))
		{
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret);

			throw 1;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR *)sql, SQL_NTS);
		if (sqlret != SQL_SUCCESS)
		{
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);

			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		// Bind Column
		for (int i = 0; i < col_num; ++ i)
		{
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
		}

		// Fetch each Row	int i; // ȡ��������
		int f_row;
		for (f_row = 0; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO; ++ f_row)
		{
			if (sqlret != SQL_SUCCESS)
			{
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			}
			//printf_s("trylist: %s ,%s ,%s , %s \n", (char const*)_buf[0] , (char const*)_buf[1], (char const*)_buf[2], (char const*)_buf[3]);
			WRITE_LONG(l_wpk,atol(reinterpret_cast<char const*>(_buf[0])));	//ID
			WRITE_STRING(l_wpk, reinterpret_cast<char const*>(_buf[1]));		//����
			WRITE_STRING(l_wpk, reinterpret_cast<char const*>(_buf[2]));	//ְҵ
			WRITE_SHORT(l_wpk, Str2Int(reinterpret_cast<char const*>(_buf[3])));
		}

		WRITE_LONG(l_wpk,f_row);	//���ΰ���������
		pCha->ReflectINFof(pCha,l_wpk);

		SQLFreeStmt(hstmt, SQL_UNBIND);
		ret = true;
	}catch(int&e)
	{
		//LG("����ϵͳ", "�ο����������Ա����ODBC �ӿڵ��ô���λ���룺%d\n",e);
		LG("consortia system", "consult apply consortia process memeberODBC interface transfer error,position ID:%d\n",e);
	}catch (...)
	{
		//LG("����ϵͳ", "Unknown Exception raised when list all guilds\n");
		LG("consortia system", "Unknown Exception raised when list all guilds\n");
	}

	if (hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}
bool CTableGuild::Approve(CCharacter* pCha, uLong chaid)
{
	if(!pCha || !pCha->HasGuild())
	{
		return false;
	}

	string buf[3];
	char filter[80];
	
	const char*param	="c.atorID";
	string	l_tbl_name =_tbl_name;
	_tbl_name	="character c";
	sprintf(filter, "c.atorID =%d and c.guild_id =%d and c.guild_permission & %d =%d", pCha->GetID(), pCha->GetGuildID(), emGldPermRecruit, emGldPermRecruit);
	int retrow =0;
	bool l_ret = _get_row(buf, 3, param, filter, &retrow);
	_tbl_name	=l_tbl_name;
	if(!l_ret)
	{
		//pCha->SystemNotice("��׼�����������ʧ��.");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00043));
		return false;
	}
	if(!retrow)
	{
		//pCha->SystemNotice("��û�й���Ĺ���Ȩ��");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00044));
		return false;
	}

	// ��ʼ����
	if( !begin_tran() )
	{
		//pCha->SystemNotice( "��׼�������ݲ���ʧ��!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00045) );
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(sql,"update guild\
					set try_total =try_total -1,\
						member_total =member_total +1\
						where guild_id =%d and member_total <%d and try_total > 0"
					,pCha->GetGuildID(),g_Config.m_sGuildNum);
	SQLRETURN l_sqlret =exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//pCha->SystemNotice( "��׼�������ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00046) );
		return false;
	}

	sprintf(sql,	"update character set guild_stat =0,guild_permission =%d\
						where atorID =%d and guild_id =%d and guild_stat =1 and delflag =0",
						emGldPermDefault,chaid, pCha->GetGuildID());
	l_sqlret =exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//pCha->SystemNotice( "��׼�������ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00046) );
		return false;
	}

	if( !commit_tran() )
	{
		this->rollback();
		//pCha->SystemNotice( "��׼�������ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00046) );
		return false;
	}

	WPACKET	l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MM_GUILD_APPROVE);
	WRITE_LONG(l_wpk,chaid);
	WRITE_LONG(l_wpk,pCha->GetGuildID());
	WRITE_STRING(l_wpk,pCha->GetValidGuildName());
	WRITE_STRING(l_wpk,pCha->GetValidGuildMotto());
	pCha->ReflectINFof(pCha,l_wpk);

	l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MP_GUILD_APPROVE);
	WRITE_LONG(l_wpk,chaid);
	pCha->ReflectINFof(pCha,l_wpk);

	const std::string cha_name = game_db.GetChaNameByID(chaid);

	char msg[SQL_MAXLEN];
	sprintf(msg, "%s has been accepted to the guild!", cha_name.c_str());
	DWORD guildID = pCha->GetGuildID();
	g_pGameApp->GuildNotice(guildID, msg);

	return true;
}
bool CTableGuild::Reject(CCharacter* pCha, uLong chaid)
{
	if (!pCha || !pCha->HasGuild())
	{
		return false;
	}
	//printf_s("Reject %lu", chaid);
	string buf[3];
	char filter[80];

	const char* param = "c.atorID";
	string	l_tbl_name = _tbl_name;
	_tbl_name = "character c";
	sprintf(filter, "c.atorID =%d and c.guild_id =%d and c.guild_permission & %d =%d", pCha->GetID(), pCha->GetGuildID(), emGldPermRecruit, emGldPermRecruit);
	int retrow = 0;
	bool l_ret = _get_row(buf, 3, param, filter, &retrow);
	_tbl_name = l_tbl_name;
	if (!l_ret)
	{
		//pCha->SystemNotice("�ܾ������������ʧ��.");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00047));
		return false;
	}
	if (!retrow)
	{
		//pCha->SystemNotice("��û�й���Ĺ���Ȩ��.");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00048));
		return false;
	}

	// ��ʼ����
	if (!begin_tran())
	{
		//pCha->SystemNotice( "��׼�������ݲ���ʧ��!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00045));
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(sql, "update character set guild_id =0 ,guild_stat =0,guild_permission =0\
						where atorID =%d and guild_id =%d and guild_stat =1",
		chaid, pCha->GetGuildID());
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0)
	{
		this->rollback();
		//pCha->SystemNotice( "�����ɫ���빫������ʧ��!��ȷ�ϸý�ɫ�����Ƿ��ѱ�ȡ��!" );
		//printf_s("Reject %lu failed ", chaid);
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00049));
		return false;
	}

	sprintf(sql, "update guild set try_total =try_total -1 where guild_id =%d and try_total > 0"
		, pCha->GetGuildID());
	l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0)
	{
		this->rollback();
		//pCha->SystemNotice( "�����ɫ���빫������ʧ��!��ȷ�ϸý�ɫ�����Ƿ��ѱ�ȡ��!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00049));
		return false;
	}

	if (!commit_tran())
	{
		this->rollback();
		//pCha->SystemNotice( "�����ɫ���빫������ʧ��!��ȷ�ϸý�ɫ�����Ƿ��ѱ�ȡ��!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00049));
		return false;
	}

	WPACKET	l_wpk = GETWPACKET();
	WRITE_CMD(l_wpk, CMD_MM_GUILD_REJECT);
	WRITE_LONG(l_wpk, chaid);
	WRITE_STRING(l_wpk, pCha->GetGuildName());
	pCha->ReflectINFof(pCha, l_wpk);
	return true;
}
bool CTableGuild::Kick(CCharacter* pCha, uLong chaid)
{
	if(!pCha || !pCha->HasGuild())
	{
		return false;
	}

	string buf[3];
	char filter[80];

	const char*param	="c.atorID";
	string	l_tbl_name =_tbl_name;
	_tbl_name	="character c";
	sprintf(filter, "c.atorID =%d and c.guild_id =%d and c.guild_permission & %d =%d", pCha->GetID(), pCha->GetGuildID(), emGldPermKick, emGldPermKick);
	int retrow =0;
	bool l_ret = _get_row(buf, 3, param, filter, &retrow);
	_tbl_name	=l_tbl_name;
	if(!l_ret)
	{
		//pCha->SystemNotice("���˲���ʧ��.");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00050));
		return false;
	}
	if(!retrow)
	{
		//pCha->SystemNotice("��û�й���Ĺ���Ȩ��.");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00048));
		return false;
	}

	if( chaid == pCha->GetID() )
	{
		//pCha->SystemNotice( "���ǻ᳤�������߳��Լ�!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00051) );
		return false;
	}

	// ��ʼ����
	if( !begin_tran() )
	{
		//pCha->SystemNotice( "��������Ա�������ݲ���ʧ��!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00052)
 );
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(sql,	"update character set guild_id =0 ,guild_stat =0,guild_permission =0\
						where atorID =%d and guild_id =%d and guild_stat =0 and\
								atorID not in(select leader_id from guild where guild_id =%d)",
				chaid, pCha->GetGuildID(), pCha->GetGuildID());
	SQLRETURN l_sqlret =exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//pCha->SystemNotice( "�����ɫ�����Ա����ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00053) );
		return false;
	}

	sprintf(sql,	"update guild set member_total =member_total -1 where guild_id =%d",pCha->GetGuildID());
	l_sqlret = exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//pCha->SystemNotice( "�����ɫ�����Ա����ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00053) );
		return false;
	}

	if( !commit_tran() )
	{
		this->rollback();
		//pCha->SystemNotice( "�����ɫ�����Ա����ʧ�ܣ����Ժ�����!" );
        pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00053) );		
		return false;
	}

	WPACKET	l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MM_GUILD_KICK);
	WRITE_LONG(l_wpk,chaid);
	WRITE_STRING(l_wpk,pCha->GetGuildName());
	pCha->ReflectINFof(pCha,l_wpk);

	l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MP_GUILD_KICK);
	WRITE_LONG(l_wpk,chaid);
	pCha->ReflectINFof(pCha,l_wpk);

	l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MC_GUILD_KICK);
	pCha->ReflectINFof(pCha,l_wpk);

	return true;
}
bool CTableGuild::Leave(CCharacter* pCha)
{
	if(!pCha || !pCha->HasGuild())
	{
		return false;
	}

	// ��ʼ����
	if( !begin_tran() )
	{
		//pCha->SystemNotice( "�뿪�������ݲ���ʧ��!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00054) );
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(sql,	"update character set guild_id =0 ,guild_stat =0,guild_permission =0\
						where atorID =%d and guild_id =%d and guild_stat =0 and\
								atorID not in(select leader_id from guild where guild_id =%d)",
				pCha->GetID(), pCha->GetGuildID(), pCha->GetGuildID());
	SQLRETURN l_sqlret =exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//pCha->SystemNotice( "�뿪��ǰ����ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00055) );
		return false;
	}

	sprintf(sql,	"update guild set member_total =member_total -1 where guild_id =%d",pCha->GetGuildID());
	l_sqlret = exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//pCha->SystemNotice( "�뿪��ǰ����ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00055) );
		return false;
	}

	if( !commit_tran() )
	{
		this->rollback();
		//pCha->SystemNotice( "�뿪��ǰ����ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00055) );
		return false;
	}

	char msg[SQL_MAXLEN];
	sprintf(msg, "%s has left the guild!", pCha->GetName());
	DWORD guildID = pCha->GetGuildID();
	g_pGameApp->GuildNotice(guildID, msg);

	pCha->SetGuildID( 0 );
	pCha->SetGuildName( "" );
	pCha->SetGuildMotto( "" );
	pCha->SyncGuildInfo();
	//pCha->SystemNotice("�Ѿ��뿪����!");
	pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00056));

	WPACKET l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MP_GUILD_LEAVE);
	pCha->ReflectINFof(pCha,l_wpk);

	l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MC_GUILD_LEAVE);
	pCha->ReflectINFof(pCha,l_wpk);
	return true;
}
bool CTableGuild::Disband(CCharacter* pCha,cChar *passwd)
{ 
	if(!pCha || !pCha->HasGuild())
	{
		return false;
	}

	string buf[6];
	char filter[80];
	const char*param	="challlevel";
	sprintf(filter, "guild_id =%d", pCha->GetValidGuildID());
	int	 l_retrow =0;
	bool l_ret = _get_row(buf, 6, param, filter, &l_retrow);
	if(l_retrow ==1)
	{
		if( atoi(buf[0].c_str()) > 0 )
		{
			//pCha->SystemNotice( "���ǹ�����ս����֮һ�����Խ�ɢ!" );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00057) );
			return false;
		}
		else
		{
			l_retrow = 0;
			sprintf(filter, "challid =%d", pCha->GetValidGuildID());
			bool l_ret = _get_row(buf, 6, param, filter, &l_retrow);
			if( !l_ret )
			{
				//pCha->SystemNotice( "��ȡ��Ĺ�����ս��Ϣʧ��!" );
				pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00058) );
				return false;
			}
			if( l_retrow >= 1 )
			{
				//pCha->SystemNotice( "������˹�����ս�����Խ�ɢ!" );
				pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00059) );
				return false;
			}
		}
	}
	else
	{
		//pCha->SystemNotice( "��ȡ��Ĺ�����ս������Ϣʧ��!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00060) );
		return false;
	}
	
	// ��ʼ����
	if( !begin_tran() )
	{
		//pCha->SystemNotice( "��ɢ�������ݲ���ʧ��!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00061) );
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(sql,	"update guild set level = 0, gold = 0,bank = '', motto ='',passwd ='',leader_id =0, exp =0,member_total =0,try_total =0 \
						where guild_id =%d and passwd ='%s' ",
						pCha->GetGuildID(), passwd);

	SQLRETURN l_sqlret =exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//pCha->SystemNotice( "��ɢ�������ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00062) );
		return false;
	}

	sprintf(sql,	"update character set guild_id =0 ,guild_stat =0,guild_permission =0\
						where guild_id =%d",
				pCha->GetGuildID());
	l_sqlret = exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//pCha->SystemNotice( "��ɢ�������ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00062) );
		return false;
	}

	if( !commit_tran() )
	{
		this->rollback();
		//pCha->SystemNotice( "��ɢ�������ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00062) );
		return false;
	}
	pCha->guildPermission = 0;
	WPACKET l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MP_GUILD_DISBAND);
	pCha->ReflectINFof(pCha,l_wpk);

	int guildID = pCha->GetGuildID();

	l_wpk = GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MM_GUILD_DISBAND);
	WRITE_LONG(l_wpk, guildID);
	pCha->ReflectINFof(pCha,l_wpk);

	return true;
}
bool CTableGuild::Motto(CCharacter* pCha,cChar *motto)
{
	if(!pCha || !pCha->HasGuild())
	{
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(sql,	"update guild set motto ='%s'\
						where guild_id =%d",
				motto,pCha->GetGuildID());
	SQLRETURN l_sqlret =exec_sql_direct(sql);
	if(!DBOK(l_sqlret))
	{
		//pCha->SystemNotice("�޸Ĺ�������������ʧ��.");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00063));
		return false;	//��ͨSQL����
	}
	if(get_affected_rows() !=1)
	{
		//pCha->SystemNotice("ֻ�л᳤�����޸Ĺ���������.");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00064));
		return false;
	}

	WPACKET	l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MM_GUILD_MOTTO);
	WRITE_LONG(l_wpk,pCha->GetGuildID());
	WRITE_STRING(l_wpk,motto);
	pCha->ReflectINFof(pCha,l_wpk);

	l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MP_GUILD_MOTTO);
	WRITE_STRING(l_wpk,motto);
	pCha->ReflectINFof(pCha,l_wpk);

	char msg[SQL_MAXLEN];
	sprintf(msg, "Guild Motto: %s", motto);

	DWORD guildID = pCha->GetGuildID();
	g_pGameApp->GuildNotice(guildID, msg);

	return true;
}

bool CTableGuild::GetGuildName(long lGuildID, std::string& strGuildName)
{
	char filter[80];

	const char*param	="guild_name";
	sprintf(filter, "guild_id =%d",lGuildID);
	int	 l_retrow =0;
	return _get_row(&strGuildName, 1, param, filter, &l_retrow);
}

bool CTableGuild::Leizhu( CCharacter* pCha, BYTE byLevel, DWORD dwMoney )
{
	if(!pCha || !pCha->HasGuild() || byLevel < 1 || byLevel > 3 )
	{
		return false;
	}

	if( dwMoney == 0 )
	{
		//pCha->SystemNotice( "����������Ͷ����������0��!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00065) );
		return false;
	}

	string buf[6];
	char filter[80];
	const char*param1	="guild_id, guild_name, challid, challmoney, leader_id, challstart";
	if( pCha->GetValidGuildID() > 0 )
	{
		sprintf(filter, "guild_id =%d", pCha->GetValidGuildID());
		int	 l_retrow =0;
		bool l_ret = _get_row(buf, 6, param1, filter, &l_retrow);
		if(l_retrow ==1)
		{
			if( pCha->GetID() == atoi(buf[4].c_str()) )
			{
				// �ǹ���᳤��ѯ					
			}
			else
			{
				return false;
			}
		}
		else
		{
			//pCha->SystemNotice( "��ѯ���Ĺ�����Ϣʧ��!���Ժ�����!" );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00066) );
			return false;
		}

		sprintf(filter, "challid =%d", pCha->GetValidGuildID());
		l_ret = _get_row(buf, 6, param1, filter, &l_retrow);
		if(l_retrow >=1)
		{
			//pCha->SystemNotice( "��ÿ��ֻ����սһ�����ᣬ���Ѿ��ǹ��ᡶ%s������ս��^_^!", buf[1].c_str() );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00067), buf[1].c_str() );
			return false;
		}
	}
	else
	{
		return false;
	}

	// ��ʼ����
	if( !begin_tran() )
	{
		//pCha->SystemNotice( "��ս�������ݲ���ʧ��!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00068));
		return false;
	}

	char sql[SQL_MAXLEN];
	char  szGuild[64];
	memset( szGuild, 0, 64 );
	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	const char*param	="guild_id, guild_name, challid, challmoney";
	sprintf(filter, "challlevel =%d",byLevel);
	int	 l_retrow =0;
	bool l_ret = _get_row(buf, 4, param, filter, &l_retrow);
	if(l_retrow ==1)
	{
		//pCha->SystemNotice( "���ᡶ%s���Ѿ���������%d������!", buf[1].c_str(), byLevel );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00069), buf[1].c_str(), byLevel );
		return false;
	}
	else
	{
		const char*param1	="challlevel";
		sprintf(filter, "guild_id =%d",pCha->GetValidGuildID());
		bool l_ret = _get_row(buf, 4, param1, filter, &l_retrow);
		if(l_retrow ==1)
		{
			if( atoi(buf[0].c_str()) > 0 )
			{
				//pCha->SystemNotice( "���Ĺ����Ѿ���������%d��������!", atoi(buf[0].c_str()) );
				pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00070), atoi(buf[0].c_str()) );
				return false;
			}
		}

		DWORD dwMoneyArray[3] = { 5000000, 3000000, 1000000 };
		if( dwMoney < dwMoneyArray[byLevel-1] || !pCha->HasMoney( dwMoney ) )
		{
			//pCha->SystemNotice( "����������%d�Ĺ���ƺ���Ҫ��%uG��!", byLevel, dwMoneyArray[byLevel-1] );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00071), byLevel, dwMoneyArray[byLevel-1] );
			return false;
		}

		sprintf(sql,	"update guild set challid = 0, challstart = 0, challmoney = 0, challlevel = %d where guild_id =%d",
			byLevel, pCha->GetValidGuildID() );
		SQLRETURN l_sqlret =exec_sql_direct(sql);
		if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
		{
			this->rollback();
			//LG( "��ս����", "��ս��������������ܣ�����ʧ�ܹ���ս�ù������ݲ���ʧ��!���᲻����!����ID = %d.����������%d", pCha->GetValidGuildID(), byLevel );
			LG( "challenge consortia", "challenge consortia over,leizhu failed:update lost consortia data operater failed! consortiaID = %d.consortia level:%d", pCha->GetValidGuildID(), byLevel );
			return false;
		}

		if( !commit_tran() )
		{
			this->rollback();
			//pCha->SystemNotice( "�������ݲ���ʧ�ܣ����Ժ�����!" );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00072) );
			return false;
		}
		//if( pCha->TakeMoney( "ϵͳ", dwMoney ) )
		if( pCha->TakeMoney( RES_STRING(GM_GAMEDB_CPP_00073), dwMoney ) )
		{
			//pCha->SystemNotice( "��ϲ��Ĺ��ᡶ%s�������������%d����ĳƺ�!", pCha->GetGuildName(), byLevel );			
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00074), pCha->GetGuildName(), byLevel );			
		}
		this->ListChallenge( pCha );		
	}
	return true;
}

bool CTableGuild::Challenge( CCharacter* pCha, BYTE byLevel, DWORD dwMoney )
{
	if(!pCha || !pCha->HasGuild() || byLevel < 1 || byLevel > 3 )
	{
		return false;
	}

	if( dwMoney == 0 )
	{
		//pCha->SystemNotice( "��ս����Ͷ����������0��!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00075) );
		return false;
	}

	string buf[6];
	char filter[80];
	const char*param1	="guild_id, guild_name, challid, challmoney, leader_id, challstart";
	if( pCha->GetValidGuildID() > 0 )
	{
		sprintf(filter, "guild_id =%d", pCha->GetValidGuildID());
		int	 l_retrow =0;
		bool l_ret = _get_row(buf, 6, param1, filter, &l_retrow);
		if(l_retrow ==1)
		{
			if( pCha->GetID() == atoi(buf[4].c_str()) )
			{
				// �ǹ���᳤��ѯ					
			}
			else
			{
				return false;
			}
		}
		else
		{
			//pCha->SystemNotice( "��ѯ���Ĺ�����Ϣʧ��!���Ժ�����!" );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00066));
			return false;
		}

		sprintf(filter, "challid =%d", pCha->GetValidGuildID());
		l_ret = _get_row(buf, 6, param1, filter, &l_retrow);
		if(l_retrow >=1)
		{
			//pCha->SystemNotice( "��ÿ��ֻ����սһ�����ᣬ���Ѿ��ǹ��ᡶ%s������ս��^_^!", buf[1].c_str() );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00067), buf[1].c_str() );
			return false;
		}
	}
	else
	{
		return false;
	}

	// ��ʼ����
	if( !begin_tran() )
	{
		//pCha->SystemNotice( "��ս�������ݲ���ʧ��!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00068));
		return false;
	}

	char sql[SQL_MAXLEN];
	char  szGuild[64];
	memset( szGuild, 0, 64 );
	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	const char*param	="guild_id, guild_name, challid, challmoney";
	sprintf(filter, "challlevel =%d",byLevel);
	int	 l_retrow =0;
	bool l_ret = _get_row(buf, 4, param, filter, &l_retrow);
	if(l_retrow ==1)
	{
		dwGuildID = atoi(buf[0].c_str());
		strncpy( szGuild, buf[1].c_str(), 63 );
		dwChallID = atoi(buf[2].c_str());
		dwChallMoney = atoi(buf[3].c_str());
	}else
	{
		DWORD dwMoneyArray[3] = { 5000000, 3000000, 1000000 };
		if( dwMoney < dwMoneyArray[byLevel-1] || !pCha->HasMoney( dwMoney ) )
		{
			//pCha->SystemNotice( "����������%d�Ĺ���ƺ���Ҫ��%uG��!", byLevel, dwMoneyArray[byLevel-1] );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00077), byLevel, dwMoneyArray[byLevel-1] );
			return false;
		}

		sprintf(sql,	"update guild set challid = 0, challstart = 0, challmoney = 0, challlevel = %d where guild_id =%d",
			byLevel, pCha->GetValidGuildID() );
		SQLRETURN l_sqlret =exec_sql_direct(sql);
		if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
		{
			this->rollback();
			//LG( "��ս����", "��ս��������������ܣ�����ʧ�ܹ���ս�ù������ݲ���ʧ��!���᲻����!����ID = %d.����������%d", pCha->GetValidGuildID(), byLevel );
			LG( "challenge consortia", "challenge consortia over,leizhu failed:update lost consortia data operater failed! consortiaID = %d.consortia level:%d", pCha->GetValidGuildID(), byLevel );
			return false;
		}

		if( !commit_tran() )
		{
			this->rollback();
			//pCha->SystemNotice( "�������ݲ���ʧ�ܣ����Ժ�����!" );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00072) );
			return false;
		}
		//if( pCha->TakeMoney( "ϵͳ", dwMoney ) )
		if( pCha->TakeMoney( RES_STRING(GM_GAMEDB_CPP_00073), dwMoney ) )
		{
			//pCha->SystemNotice( "��ϲ��Ĺ��ᡶ%s�������������%d����ĳƺ�!", pCha->GetGuildName(), byLevel );			
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00074), pCha->GetGuildName(), byLevel );			
		}
		this->ListChallenge( pCha );
		return true;
	}

	BYTE byLvData = 0;
	const char*param2	="challlevel";
	sprintf(filter, "guild_id =%d", pCha->GetValidGuildID() );
	l_ret = _get_row(buf, 4, param2, filter, &l_retrow);
	if(l_retrow ==1)
	{
		byLvData = (BYTE)atoi(buf[0].c_str());
	}else
	{
		//pCha->SystemNotice( "��ѯ��Ĺ���������Ϣʧ��!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00078) );
		return false;
	}

	if( dwGuildID == 0 )
	{
		//pCha->SystemNotice( "������������!GID = %d, LV = %d", dwGuildID, byLevel );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00079), dwGuildID, byLevel );
		return false;
	}

	if( byLvData != 0 && byLevel > byLvData )
	{
		//pCha->SystemNotice( "��������ս���������ǹ���͵Ĺ���!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00080) );
		return false;
	}

	if( pCha->GetPlayer()->GetDBChaId() == dwChallID )
	{
		//pCha->SystemNotice( "���Ѿ��ǹ���ս����ս����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00081) );
		return false;
	}
	else if( pCha->GetValidGuildID() == dwGuildID )
	{
		//pCha->SystemNotice( "��������ս�Լ��Ĺ���!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00082) );
		return false;
	}
	else if( dwMoney < dwChallMoney + 50000 )
	{
		//pCha->SystemNotice( "������ս����!��%u��", dwMoney );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00083), dwMoney );
		return false;
	}

	if( !pCha->HasMoney( dwMoney ) )
	{
		//pCha->SystemNotice( "��Ͷ��ʧ�ܣ�û���㹻�ý�Ǯ!��%u��", dwMoney );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00084), dwMoney );
		return false;
	}

	// �����µ���ս����	
	sprintf(sql,	"update guild set challid =%d,challmoney =%d where guild_id =%d \
					and challmoney < %d and challstart = 0",
				pCha->GetGuildID(), dwMoney, dwGuildID, dwMoney );
	SQLRETURN l_sqlret =exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//pCha->SystemNotice( "��ս�������ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00085) );
		return false;
	}

	if( !commit_tran() )
	{
		this->rollback();
		//pCha->SystemNotice( "��ս�������ʧ�ܣ����Ժ�����!" );
		pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00085) );
		return false;
	}

	// ��Ǯ
	//pCha->TakeMoney( "ϵͳ", dwMoney );
	pCha->TakeMoney(RES_STRING(GM_GAMEDB_CPP_00073), dwMoney );
	// ��֮ǰ����ս�����Ǯ����ȥ
	if( dwChallID > 0 && dwChallMoney > 0 )
	{
		WPACKET	l_wpk	=GETWPACKET();
		WRITE_CMD(l_wpk,CMD_MP_GUILD_CHALLMONEY);
		WRITE_LONG(l_wpk, dwChallID);
		WRITE_LONG(l_wpk, dwChallMoney);
		WRITE_STRING( l_wpk, szGuild );
		WRITE_STRING( l_wpk, pCha->GetGuildName() );
		pCha->ReflectINFof(pCha,l_wpk);		
	}

	ListChallenge( pCha );
	return true;
}

void CTableGuild::ListChallenge( CCharacter* pCha )
{
	string buf1[6];
	string buf2[6];
	char filter[80];

	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	DWORD dwLeaderID = 0;
	BYTE  byStart = 0;

	WPACKET l_wpk	=GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MC_GUILD_LISTCHALL);

	const char*param	="guild_id, guild_name, challid, challmoney, leader_id, challstart";
	if( pCha->GetValidGuildID() > 0 )
	{
		sprintf(filter, "guild_id =%d", pCha->GetValidGuildID());
		int	 l_retrow =0;
		bool l_ret = _get_row(buf1, 6, param, filter, &l_retrow);
		if(l_retrow ==1)
		{
			if( pCha->GetID() == atoi(buf1[4].c_str()) )
			{
				// �ǹ���᳤��ѯ
				WRITE_CHAR( l_wpk, 1 );
			}
			else
			{
				WRITE_CHAR( l_wpk, 0 );
			}
		}
		else
		{
			//pCha->SystemNotice( "��ѯ���Ĺ�����Ϣʧ��!���Ժ�����!" );
			pCha->SystemNotice( RES_STRING(GM_GAMEDB_CPP_00066) );
			return;
		}
	}
	else
	{
		WRITE_CHAR( l_wpk, 0 );
	}

	for( int i = 1; i <= 3; ++i )
	{
		sprintf(filter, "challlevel =%d", i);
		int	 l_retrow =0;
		bool l_ret = _get_row(buf1, 6, param, filter, &l_retrow);
		if(l_retrow ==1)
		{			
			dwGuildID = atoi(buf1[0].c_str());
			dwChallID = atoi(buf1[2].c_str());
			dwChallMoney = atoi(buf1[3].c_str());
			byStart = (BYTE)atoi(buf1[5].c_str());
			
			if( dwChallID != 0 )
			{
				sprintf(filter, "guild_id =%d", dwChallID);
				bool l_ret = _get_row(buf2, 6, param, filter, &l_retrow);
				if(l_retrow ==1)
				{
					WRITE_CHAR( l_wpk, i );
					WRITE_CHAR( l_wpk, byStart );
					WRITE_STRING( l_wpk, buf1[1].c_str() );
					WRITE_STRING( l_wpk, buf2[1].c_str() );
					WRITE_LONG( l_wpk, dwChallMoney );
				}
				else
				{
					WRITE_CHAR( l_wpk, 0 );
				}
			}
			else
			{
				WRITE_CHAR( l_wpk, i );
				WRITE_CHAR( l_wpk, byStart );
				WRITE_STRING( l_wpk, buf1[1].c_str() );
				WRITE_STRING( l_wpk, "" );
				WRITE_LONG( l_wpk, dwChallMoney );
			}
		}
		else
		{
			WRITE_CHAR( l_wpk, 0 );
		}
	}
	pCha->ReflectINFof(pCha,l_wpk);
}

bool CTableGuild::HasGuildLevel( CCharacter* pChar, BYTE byLevel )
{
	if( !pChar->HasGuild() )
	{
		return false;
	}

	string buf[1];
	char filter[80];
	BYTE byData = 0;
	const char*param	="challlevel";
	sprintf(filter, "guild_id =%d",pChar->GetValidGuildID());
	int	 l_retrow =0;
	bool l_ret = _get_row(buf, 1, param, filter, &l_retrow);
	if(l_retrow ==1)
	{
		byData = (BYTE)atoi(buf[0].c_str());
		return byLevel == byData;
	}
	return false;
}

bool CTableGuild::HasCall( BYTE byLevel )
{
	string buf[5];
	char filter[80];

	char  szGuild[64];
	memset( szGuild, 0, 64 );
	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	const char*param	="guild_id, guild_name, challid, challmoney, challstart";
	sprintf(filter, "challlevel =%d",byLevel);
	int	 l_retrow =0;
	BYTE byStart = 0;
	bool l_ret = _get_row(buf, 5, param, filter, &l_retrow);
	if(l_retrow ==1)
	{
		dwGuildID = atoi(buf[0].c_str());
		strncpy( szGuild, buf[1].c_str(), 63 );
		dwChallID = atoi(buf[2].c_str());
		dwChallMoney = atoi(buf[3].c_str());
		byStart = (BYTE)atoi(buf[4].c_str());
		return dwChallID != 0 && byStart == 1;
	}
	return false;
}

bool CTableGuild::StartChall( BYTE byLevel )
{
	//LG( "��ս����", "���������%d��ս����ʼ����...\n", byLevel ); 
	LG( "challenge consortia", "range level %d challenge start treat with....\n", byLevel ); 
	string buf[4];
	char filter[80];

	char  szGuild[64];
	memset( szGuild, 0, 64 );
	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	const char*param	="guild_id, guild_name, challid, challmoney";
	sprintf(filter, "challlevel =%d",byLevel);
	int	 l_retrow =0;
	bool l_ret = _get_row(buf, 4, param, filter, &l_retrow);
	if(l_retrow ==1)
	{
		dwGuildID = atoi(buf[0].c_str());
		strncpy( szGuild, buf[1].c_str(), 63 );
		dwChallID = atoi(buf[2].c_str());
		dwChallMoney = atoi(buf[3].c_str());
	}else
	{
		return false;
	}

	if( dwGuildID == 0 )
	{
		return false;
	}

	// �����µ���ս����
	char sql[SQL_MAXLEN];
	sprintf(sql,	"update guild set challstart = 1 where guild_id =%d and challstart = 0",
				dwGuildID );
	SQLRETURN l_sqlret =exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		//LG( "��ս����", "��ս�������ݲ���ʧ��!����ս�Ѿ���ʼ���߹��᲻����!" );
		LG( "challenge consortia", "challenge consortia data operator failed!consortia battle already start or inexistence!" );
		return false;
	}

	//LG( "��ս����", "������%d������ս�ɹ���ʼ!GUILD1 = %d, GUILD2 = %d, Money = %u.\n", byLevel, dwGuildID, dwChallID, dwChallMoney );
	LG( "challenge consortia", "range level %d challenge start succeed !GUILD1 = %d, GUILD2 = %d, Money = %u.\n", byLevel, dwGuildID, dwChallID, dwChallMoney );
	return true;
}

void CTableGuild::EndChall( DWORD dwGuild1, DWORD dwGuild2, BOOL bChall )
{
	//LG( "��ս����", "����������ս����ʼ��������GUILD1 = %d, GUILD2 = %d...\n", dwGuild1, dwGuild2 ); 
	LG( "challenge consortia", "arranger level consortia game start operator finish GUILD1 = %d, GUILD2 = %d...\n", dwGuild1, dwGuild2 ); 
	string buf[5];
	char filter[80];

	char  szGuild[64];
	memset( szGuild, 0, 64 );
	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	BYTE  byLevel = 0;
	BYTE  byStart = 0;
	const char*param	="challstart, guild_name, challid, challmoney, challlevel";
	sprintf(filter, "guild_id =%d",dwGuild1);
	int	 l_retrow =0;
	bool l_ret = _get_row(buf, 5, param, filter, &l_retrow);
	if(l_retrow ==1)
	{
		byStart = (BYTE)atoi(buf[0].c_str());
		strncpy( szGuild, buf[1].c_str(), 63 );
		dwChallID = atoi(buf[2].c_str());
		dwChallMoney = atoi(buf[3].c_str());
		byLevel = (BYTE)atoi(buf[4].c_str());
		if( dwChallID == dwGuild2 )
		{
			ChallMoney( byLevel, bChall, dwGuild1, dwGuild2, dwChallMoney );
			//LG( "��ս����", "������%d������ս����!GUILD1 = %d, GUILD2 = %d, Money = %u.\n", byLevel, dwGuild1, dwGuild2, dwChallMoney );
			LG( "challenge consortia", "range level %d consortia challenge over!GUILD1 = %d, GUILD2 = %d, Money = %u.\n", byLevel, dwGuild1, dwGuild2, dwChallMoney );	
			return;
		}
	}

	sprintf(filter, "guild_id =%d",dwGuild2);
	l_ret = _get_row(buf, 5, param, filter, &l_retrow);
	if(l_retrow ==1)
	{
		byStart = (BYTE)atoi(buf[0].c_str());
		strncpy( szGuild, buf[1].c_str(), 63 );
		dwChallID = atoi(buf[2].c_str());
		dwChallMoney = atoi(buf[3].c_str());
		byLevel = (BYTE)atoi(buf[4].c_str());
		if( dwChallID == dwGuild1 )
		{
			ChallMoney( byLevel, !bChall, dwGuild2, dwGuild1, dwChallMoney );
			//LG( "��ս����", "������%d������ս����!GUILD1 = %d, GUILD2 = %d, Money = %u.\n", byLevel, dwGuild2, dwGuild1, dwChallMoney );
			LG( "challenge consortia", "range level %d consortia challenge over!GUILD1 = %d, GUILD2 = %d, Money = %u.\n", byLevel, dwGuild2, dwGuild1, dwChallMoney );
			return;
		}
	}

	//LG( "��ս����", "������ս�������ʧ��!GUILD1 = %d, GUILD2 = %d, ChallFlag = %d.\n", dwGuild1, dwGuild2, ( bChall ) ? 1 : 0 ); 
	LG( "challenge consortia", "consortia challenge result disposal failed!GUILD1 = %d, GUILD2 = %d, ChallFlag = %d.\n", dwGuild1, dwGuild2, ( bChall ) ? 1 : 0 ); 
}

bool CTableGuild::ChallWin( BOOL bUpdate, BYTE byLevel, DWORD dwWinGuildID, DWORD dwFailerGuildID )
{
	// ��ʼ����
	if( !begin_tran() )
	{
		//LG( "��ս����", "��ս������������¹������ݿ�ʼ����ʧ��!" );
		LG( "challenge consortia", "challenge consortia finish,update consortia data start affair failed!" );
		return false;
	}

	// �����µ���ս����
	char sql[SQL_MAXLEN];
	SQLRETURN l_sqlret;
	if( !bUpdate )
	{
		//sprintf(sql,	"update guild set challid = 0, challstart = 0, challmoney = 0, challlevel = 0 where guild_id =%d",
		//	dwFailerGuildID );
		//l_sqlret =exec_sql_direct(sql);
		//if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
		//{
		//	this->rollback();
		//	LG( "��ս����", "��ս�������������ʤ������ʧ�ܹ���ս�ù������ݲ���ʧ��!���᲻����!����ID = %d.����������%d", dwFailerGuildID, byLevel );
		//	return false;
		//}
	}
	else
	{
		string buf[5];
		char filter[80];

		BYTE  byLvData = 0;
		const char*param	="challlevel";
		sprintf(filter, "guild_id =%d",dwWinGuildID);
		int	 l_retrow =0;
		bool l_ret = _get_row(buf, 5, param, filter, &l_retrow);
		if(l_retrow ==1)
		{
			byLvData = (BYTE)atoi(buf[0].c_str());
		}
		else
		{
			//LG( "��ս����", "��ս��������������ܣ���ѯʧ�ܹ����������Ϣʧ��!GUILDID = %d, WINID = %d.\n", dwFailerGuildID, dwWinGuildID );
			LG( "challenge consortia", "finish challenge consortia��leizhu failed:inquire about failed consortia level info failed!GUILDID = %d, WINID = %d.\n", dwFailerGuildID, dwWinGuildID );
			return false;
		}

		if( byLvData > 0 )
		{
			// ��������
			if( byLvData < byLevel )
			{
				BYTE byTemp = byLevel;
				byLevel = byLvData;
				byLvData = byTemp;
			}

			sprintf(sql,	"update guild set challid = 0, challstart = 0, challmoney = 0, challlevel = %d where guild_id =%d",
				byLvData, dwFailerGuildID );
			l_sqlret =exec_sql_direct(sql);
			if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
			{
				this->rollback();
				//LG( "��ս����", "��ս��������������ܣ�����ʧ�ܹ���ս�ù������ݲ���ʧ��!���᲻����!����ID = %d.����������%d.\n", dwFailerGuildID, byLevel );
				LG( "challenge consortia", "challenge consortia over,leizhu failed:update lost consortia data operater failed! consortiaID = %d.consortia level:%d.\n",dwFailerGuildID, byLevel );
				return false;
			}
		}
		else
		{
			sprintf(sql,	"update guild set challid = 0, challstart = 0, challmoney = 0, challlevel = 0 where guild_id =%d",
				dwFailerGuildID );
			l_sqlret =exec_sql_direct(sql);
			if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
			{
				this->rollback();
				//LG( "��ս����", "��ս��������������ܣ�����ʧ�ܹ���ս�ù������ݲ���ʧ��!���᲻����!����ID = %d.����������%d.\n", dwFailerGuildID, byLevel );
				LG( "challenge consortia", "challenge consortia over,leizhu failed:update lost consortia data operater failed! consortiaID = %d.consortia level:%d.\n",dwFailerGuildID, byLevel );
				return false;
			}
		}
	}

	sprintf(sql,	"update guild set challid = 0, challstart = 0, challmoney = 0, challlevel = %d where guild_id =%d",
				byLevel, dwWinGuildID  );
	l_sqlret =exec_sql_direct(sql);
	if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
	{
		this->rollback();
		//LG( "��ս����", "��ս�������������Ӯ�ù���ս�ù������ݲ���ʧ��!���᲻����!����ID = %d.����������%d.\n", dwWinGuildID, byLevel );
		LG( "challenge consortia", "challenge consortia over,update winner consortia data operator failed!inexistence consortia!consortiaID = %d.consortia level��%d.\n", dwWinGuildID, byLevel );
		return false;
	}

	if( !commit_tran() )
	{
		this->rollback();
		//LG( "��ս����", "��ս���������ύʧ�ܣ��Ժ�����!.\n" );
		LG( "challenge consortia", "challenge consortia data referring failed,retry later on\n" );
		return false;
	}
	return true;
}

void CTableGuild::ChallMoney( BYTE byLevel, BOOL bChall, DWORD dwGuildID, DWORD dwChallID, DWORD dwMoney )
{
	if( bChall )
	{
		//LG( "��ս������", "��սʧ�ܣ�ʤ����ID = %d, �ܷ���ID = %d, ��Ǯ��%u, ������%d.\n", dwGuildID, dwChallID, dwMoney, byLevel  );
		LG( "challenge consortia result", "challenge failed: winner:ID = %d,loser:ID = %d, money = %u,level:%d.\n", dwGuildID, dwChallID, dwMoney, byLevel  );
		if( !ChallWin( FALSE, byLevel, dwGuildID, dwChallID ) )
		{
			return;
		}

		if( dwChallID != 0 )
		{
			dwMoney = DWORD(float(dwMoney*80)/100);
			WPACKET	l_wpk	=GETWPACKET();
			WRITE_CMD(l_wpk,CMD_MP_GUILD_CHALL_PRIZEMONEY);
			WRITE_LONG(l_wpk, dwGuildID);
			WRITE_LONG(l_wpk, dwMoney);
			SENDTOGROUP(l_wpk);
		}
	}
	else
	{
		//LG( "��ս������", "��ս�ɹ���ʤ����ID = %d, �ܷ���ID = %d, ��Ǯ��%u, ������%d.\n", dwChallID, dwGuildID, dwMoney, byLevel  );
		LG( "challenge consortia result", "challenge succeed��winner:ID = %d,loser:ID = %d, money = %u,level:%d.\n", dwChallID, dwGuildID, dwMoney, byLevel  );
		if( !ChallWin( TRUE, byLevel, dwChallID, dwGuildID ) )
		{
			return;
		}

		dwMoney = DWORD(float(dwMoney*80)/100);
		WPACKET	l_wpk	=GETWPACKET();
		WRITE_CMD(l_wpk,CMD_MP_GUILD_CHALL_PRIZEMONEY);
		WRITE_LONG(l_wpk, dwChallID);
		WRITE_LONG(l_wpk, dwMoney);
		SENDTOGROUP(l_wpk);
	}
}

bool CTableGuild::GetChallInfo( BYTE byLevel, DWORD& dwGuildID1, DWORD& dwGuildID2, DWORD& dwMoney )
{
	string buf[3];
	char filter[80];

	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	const char*param	="guild_id, challid, challmoney";
	sprintf(filter, "challlevel =%d",byLevel);
	int	 l_retrow =0;
	bool l_ret = _get_row(buf, 3, param, filter, &l_retrow);
	if( l_retrow == 1 )
	{
		dwGuildID1 = atoi(buf[0].c_str());
		dwGuildID2 = atoi(buf[1].c_str());
		dwMoney = atoi(buf[2].c_str());

		return true;
	}
	return false;
}

bool CTableCha::SetGuildPermission(int atorID, unsigned long permission, int guild_id)
{
	_snprintf_s(g_sql, sizeof(g_sql), _TRUNCATE, "update %s set guild_permission='%d' where atorID=%d and guild_id = %d",
																_get_table(), permission, atorID,guild_id);

	if (strlen(g_sql) >= SQL_MAXLEN)
		return false;

	short sExec = exec_sql_direct(g_sql);
	if (!DBOK(sExec))
		return false;

	if (DBNODATA(sExec))
		return false;

	return true;
}

bool CTableCha::SetChaAddr(DWORD atorID, Long addr)
{
	_snprintf_s(g_sql, sizeof(g_sql), _TRUNCATE, "update %s set \
		endeMem='%d'\
		where atorID=%d", \
		_get_table(), -1, atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
		return false;

	short sExec =  exec_sql_direct(g_sql);
	if (!DBOK(sExec))
		return false;

	if (DBNODATA(sExec))
		return false;

	return true;
}

//===============CTableGuild End===========================================
//	2008-7-28	yyy	add	function	begin!


bool	CTableItem::LockItem(	SItemGrid*	sig,	int	iChaId	)
{
	char param[80];
	sprintf(param, "TOP 1 id");

	char filter[80]; 
	sprintf(filter, "ORDER BY id DESC");

	std::string buf[1];

	int			r1 = 0;
	int			r = _get_rowOderby(buf, 1, param, filter, &r1);
	DWORD		dwDropertyID;

	if (DBOK(r) && r1 > 0)
	{
		dwDropertyID = atol( buf[0].c_str() ) + 1;
	}
	else
	{
		dwDropertyID = 1;
	}
	if(	sig	&&	!sig->dwDBID	)
	{
		sig->dwDBID	=	dwDropertyID;
		string	s;
		long	lnCheckSum	=	0;
			
		if(	SItemGrid2String(	s,	lnCheckSum,	sig,	0	)	)
		{
			sprintf( g_sql, "insert into property (	id,	atorID,	context,	sum,	time	) values (	%d,	%d, '%s', %d,	getdate())",
				dwDropertyID,
				iChaId, 
				s.c_str(),
				lnCheckSum
				);
				short sExec = exec_sql_direct( g_sql );
				return	DBOK(sExec);
		};
	};
	return	false;
};

bool CTableItem::UnlockItem( SItemGrid* sig, int iChaId )
{
	if(sig)
	{
		_snprintf_s(g_sql,sizeof(g_sql),_TRUNCATE, "delete from property where id =%d and atorID =%d",sig->dwDBID, iChaId );

		short sExec = exec_sql_direct( g_sql );
		sig->dwDBID = 0;
		return DBOK(sExec);
	}
	return false;
}

CGameDB game_db;