#include "stdafx.h"
#include "Friend.h"
#include "Master.h"
#include "DBConnect.h"
#include "Guild.h"
#include "GroupServerApp.h"
#include "GameCommon.h"

using namespace std;


SQLRETURN Exec_sql_direct(const char *pszSQL, cfl_rs *pTable)
{
	//LG("group_sql", "[%s], SQL[%s]\n", pTable->get_table(), pszSQL);
	ToLogService("group_sql", "Table [{}], begin execute SQL [{}]", pTable->get_table(), pszSQL);
	SQLRETURN r = pTable->exec_sql_direct(pszSQL);
	if(DBOK(r))
	{
		//LG("group_sql", "SQL!\n");
		ToLogService("group_sql", "execute SQL success!");
	}
	else if(DBNODATA(r))
	{
		//LG("group_sql", "SQL, \n");
		ToLogService("group_sql", "execute SQL, no result");
	}
	else
	{
		//LG("group_sql", "SQL, !\n");
		ToLogService("group_sql", "execute SQL, failed!");
	}
	return r;
}

//==========TBLSystem===============================
bool TBLAccounts::IsReady()
{
	char sql[SQL_MAXLEN];
	strcpy(sql,"drop trigger [TR_D_Character_Friends]");
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	if(!DBOK(l_ret))
	{
			//l_line<<newln<<...;
			ToLogService("Database", "SQL:{} execute failed !", sql);
	}
	strcpy(sql,"drop trigger [TR_I_Character]");
	l_ret =Exec_sql_direct(sql, this);
	if(!DBOK(l_ret))
	{
			//l_line<<newln<<...;
			ToLogService("Database", "SQL:{} execute failed !", sql);
	}
	strcpy(sql,"CREATE TRIGGER TR_D_Character_Friends ON character \n\
				FOR DELETE \n\
				AS\n\
				BEGIN\n\
					declare @@stat tinyint\n\
					declare @@gid  int\n\
					select @@stat =guild_stat,@@gid =guild_id from deleted\n\
					DELETE friends where friends.cha_id1 IN(select atorID from deleted)\n\
					if(@@gid >0)\n\
					BEGIN\n\
						update guild set try_total =try_total -(case when @@stat>0 then 1 else 0 end),\n\
								member_total =member_total -(case when @@stat >0 then 0 else 1 end)\n\
							where guild_id >0 and guild_id =@@gid\n\
					END\n\
				END\n\
		");
	l_ret =Exec_sql_direct(sql, this);
	if(!DBOK(l_ret))
	{
			//l_line<<newln<<...;
			ToLogService("Database", "SQL:{} execute failed !", sql);
		return false;
	}
	strcpy(sql,"CREATE TRIGGER TR_I_Character ON character\n\
				FOR INSERT\n\
				AS\n\
				BEGIN\n\
					declare @l_icon smallint\n\
					select @l_icon =convert(smallint,SUBSTRING(inserted.olhe,5,1)) from inserted\n\
					update character set icon =@l_icon where atorID in (select atorID from inserted)\n\
				END\n\
		");
	l_ret =Exec_sql_direct(sql, this);
	if(!DBOK(l_ret))
	{
		ToLogService("Database", "SQL:{} execute failed !", sql);
		return false;
	}
	return true;
}/*
int TBLSystem::Increment()
{
   char sql[SQL_MAXLEN];

    // account_save  id 
    sprintf(sql, "update %s set group_startup =group_startup +1",
            _get_table());
    Exec_sql_direct(sql, this);

	int l_retrow	=0;
	char* param = "group_startup";
	char filter[80];
	sprintf(filter, "group_startup = group_startup");
	if(_get_row(m_buf, 1, param, filter,&l_retrow) && l_retrow ==1 && get_affected_rows() ==1)
	{
		return atoi(m_buf[0].c_str());
	}else
	{
		return -1;
	}

}
void TBLSystem::Decrement()
{
   char sql[SQL_MAXLEN];

    // account_save  id 
    sprintf(sql, "update %s set group_startup =group_startup -1",
            _get_table());
    Exec_sql_direct(sql, this);
}
*/
//==========TBLAccounts===============================
void TBLAccounts::AddStatLog(long login,long play,long wgplay)
{
    char sql[SQL_MAXLEN];

    // account_save  id 
    sprintf(sql, "insert stat_log (login_num , play_num, wgplay_num) values (%d, %d, %d)", login , play, wgplay);
    Exec_sql_direct(sql, this);
}
bool TBLAccounts::SetDiscInfo(int actid,const char *cli_ip,const char *reason)
{
	char sql[SQL_MAXLEN];

	// account_save  id 
	sprintf(sql, "update %s set last_ip='%s',disc_reason ='%s',last_leave =getdate() where ato_id =%d",
			_get_table(), cli_ip, reason, actid);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return  (DBOK(l_ret))?true:false;
}
bool TBLAccounts::InsertRow(int ato_id,const char *ato_nome,const char *ator_ids)
{
    char sql[SQL_MAXLEN];

	DWORD dwActID = 0;
	std::string buf[1];
	char param[80];
	sprintf(param, "TOP 1 ato_id");
	char filter[80]; 
	sprintf(filter, "ORDER BY ato_id DESC");
	int r1 = 0;
	int r = _get_rowOderby(buf, 1, param, filter, &r1);
	if (DBOK(r) && r1 > 0)
	{
		dwActID = atol( buf[0].c_str() ) + 1;
	}
	else
	{
		dwActID = 1;
	}

    // account_save  id 
    sprintf(sql, "insert %s (ato_id, ato_nome, ator_ids) values (%d, '%s', '%s')",
            _get_table(), dwActID, ato_nome, ator_ids);
    SQLRETURN l_ret =Exec_sql_direct(sql, this);
    return (DBOK(l_ret))?true:false;

}
bool TBLAccounts::UpdateRow(int ato_id,const char *ator_ids)
{
	char sql[SQL_MAXLEN];

	// account_save  id 
	sprintf(sql, "update %s set ator_ids='%s' where ato_id=%d",
			_get_table(), ator_ids, ato_id);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}
bool TBLAccounts::UpdatePassword( int ato_id, const char szPassword[] )
{
	char sql[SQL_MAXLEN];

	// account_save  id 
	sprintf(sql, "update %s set password='%s' where ato_id=%d",
			_get_table(), szPassword, ato_id);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

int TBLAccounts::FetchRowByActName(const char szAccount[])
{
	int l_retrow	=0;
	const char* param = "ato_id,jmes,ator_ids,password,last_ip,disc_reason,convert(varchar(20),last_leave,120)";
	char filter[200];
	sprintf(filter, "ato_nome='%s'", szAccount);
	if(_get_row(m_buf, 7, param, filter,&l_retrow))
	{
		if(l_retrow ==1 && get_affected_rows() ==1)
		{
			return l_retrow;
		}else
		{
			return 0;
		}
	}else
	{
		return -1;
	}
}

int TBLAccounts::FetchRowByActID(int ato_id)
{
	int l_retrow	=0;
	const char* param = "ato_nome,jmes,ator_ids,password,last_ip,disc_reason,convert(varchar(20),last_leave,120)";
	char filter[200];
	sprintf(filter, "ato_id=%d", ato_id);
	if(_get_row(m_buf, 7, param, filter,&l_retrow))
	{
		if(l_retrow ==1 && get_affected_rows() ==1)
		{
			return l_retrow;
		}else
		{
			return 0;
		}
	}else
	{
		return -1;
	}
}

//==========TBLCharacters===============================
bool TBLCharacters::ZeroAddr()
{
	char sql[SQL_MAXLEN];

	// account_save  id 
	sprintf(sql, "update %s set endeMem =0 where endeMem != 0",_get_table());
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

//bool TBLCharacters::ZeroAddr()
//{
//	char sql[SQL_MAXLEN];
//
//	for( int i = 0; i < 200; i++ )
//	{
//		// account_save  id 
//		int nMinID = i * 10000;
//		int nMaxID = (i + 1) * 10000;
//		sprintf(sql, "update %s set mem_addr = 0 where cha_id > %d and cha_id < %d and mem_addr != 0",_get_table(), nMinID, nMaxID );
//		SQLRETURN l_ret =Exec_sql_direct(sql, this, 60);
//		if( !DBOK(l_ret) )
//		{
//			return false;
//		}
//	}
//	return true;
//}

bool TBLCharacters::SetAddr(long atorID,long addr)
{
	char sql[SQL_MAXLEN];

	// account_save  id 
	sprintf(sql, "update %s set endeMem =%d where atorID =%d",_get_table(),addr,atorID);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}
bool TBLCharacters::InsertRow(const char *atorNome,int ato_id,const char *birth,const char *map,const char *olhe)
{
	char sql[SQL_MAXLEN];

	// account_save  id 
	sprintf(sql, "insert %s (atorNome, ato_id, birth,map,olhe) values ('%s', %d, '%s','%s', '%s')",
			_get_table(), atorNome, ato_id, birth, map,olhe);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}
bool TBLCharacters::UpdateInfo(unsigned long atorID,unsigned short icon,const char * motto)
{
	char sql[SQL_MAXLEN];

	// account_save  id 
	sprintf(sql, "update %s set icon =%d,motto ='%s' where atorID =%d",_get_table(),icon,motto,atorID);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

int TBLCharacters::FetchRowByChaName(const char *atorNome)
{
	int l_retrow	=0;
	const char* param = "atorID,motto,icon";
	char filter[200];
	sprintf(filter, "atorNome='%s'", atorNome);
	if(_get_row(m_buf, 3, param, filter,&l_retrow))
	{
		if(l_retrow ==1 && get_affected_rows() ==1)
		{
			return l_retrow;
		}else
		{
			return 0;
		}
	}else
	{
		return -1;
	}
}
bool TBLCharacters::FetchAccidByChaName(const char *atorNome, int& cha_accid )
{
	int l_retrow	=0;
	const char* param = "ato_id";
	char filter[200];
	sprintf(filter, "atorNome='%s'", atorNome);
	if(_get_row(m_buf, 1, param, filter,&l_retrow))
	{
		if(l_retrow ==1 && get_affected_rows() ==1)
		{
			cha_accid = atoi( m_buf[0].c_str() );
			return true;
		}else
		{
			return false;
		}
	}
	return false;	
}

bool TBLCharacters::StartEstopTime( int atorID )
{
	char sql[SQL_MAXLEN];
	sprintf(sql, "update %s set estop = getdate() where atorID =%d",_get_table(), atorID);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

bool TBLCharacters::EndEstopTime( int atorID )
{
	char sql[SQL_MAXLEN];
	sprintf(sql, "update %s set estoptime = estoptime - datediff(minute, estop, getdate()) where atorID =%d",_get_table(), atorID);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

bool TBLCharacters::IsEstop(int atorID)
{
	int l_retrow	=0;
	const char* param = "estop";
	char filter[200];
	sprintf(filter, "atorID = %d and dateadd(minute, estoptime, estop) > getdate()", atorID);
	if(_get_row(m_buf, 1, param, filter,&l_retrow))
	{
		if(l_retrow ==1 && get_affected_rows() ==1)
		{
			return true;
		}else
		{
			return false;
		}
	}
	return true;
}

bool TBLCharacters::Estop( const char *atorNome, uLong lTimes )
{
	char sql[SQL_MAXLEN];
	sprintf(sql, "update %s set estop = getdate(), estoptime = %d where atorNome ='%s'",_get_table(), lTimes, atorNome);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

bool TBLCharacters::AddMoney( int atorID, DWORD dwMoney )
{
	char sql[SQL_MAXLEN];
	sprintf(sql, "update %s set bomd = bomd + %d where atorID ='%d'",_get_table(), dwMoney, atorID);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

bool TBLCharacters::DelEstop( const char *atorNome )
{
	char sql[SQL_MAXLEN];
	sprintf(sql, "update %s set estoptime = %d where atorNome ='%s'",_get_table(), 0, atorNome);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

int TBLCharacters::FetchChaIDByCharName(cChar* atorNome)
{
	int l_retrow = 0;
	string param = string("c.atorID");
	char filter[200];
	sprintf(filter, "c.atorNome='%s'", atorNome);
	std::string	l_tblname = _tbl_name;
	_tbl_name = "character c";
	bool l_bret = false;
	try{
		if (_get_row(m_buf, CHA_MAXCOL, const_cast<char*>(param.c_str()), filter, &l_retrow)){
			return atoi(m_buf[0].c_str());
		}
	}
	catch (...)
	{
		ToLogService("group_sql", "TBLCharacters::FetchChaIDByCharName execute SQL, failed!,atorID ={}", atorNome);
	}
	return 0;
}

int TBLCharacters::FetchActIDByCharName(cChar* atorNome)
{
	int l_retrow = 0;
	string param = string("c.ato_id");
	char filter[200];
	sprintf(filter, "c.atorNome='%s'", atorNome);
	std::string	l_tblname = _tbl_name;
	_tbl_name = "character c";
	bool l_bret = false;
	try{
		if (_get_row(m_buf, CHA_MAXCOL, const_cast<char*>(param.c_str()), filter, &l_retrow)){
			return atoi(m_buf[0].c_str());
		}
	}
	catch (...)
	{
		ToLogService("group_sql", "TBLCharacters::FetchActIDByCharName execute SQL, failed!,atorID ={}", atorNome);
	}
	return 0;
}

int TBLCharacters::FetchRowByChaID(int atorID)
{
	int l_retrow	=0;
/*
	char* param = "c.atorNome,c.motto,c.icon,\
				  case when c.guild_stat =0 then c.guild_id else 0 end,\
				  case when c.guild_stat <>0 or c.guild_id =0 then '[]' else g.guild_name end,\
				  c.job,c.degree,c.map,c.map_x,c.map_y,c.look,c.str,c.dex,c.agi,c.con,c.sta,c.luk\
				  ";
*/
	string param = string("c.atorNome,c.motto,c.icon,\
				  case when c.guild_stat =0 then c.guild_id else 0 end,\
				  case when c.guild_stat <>0 or c.guild_id =0 then '")
				  +	string(RES_STRING(GP_DBCONNECT_CPP_00001)) +
				  string("' else g.guild_name end,\
				  c.job,c.degree,c.map,c.map_x,c.map_y,c.olhe,c.str,c.dex,c.agi,c.con,c.sta,c.luk,c.guild_permission,c.chatColour");
	char filter[200];
	sprintf(filter, "c.guild_id =g.guild_id and c.atorID=%d", atorID);
	std::string	l_tblname	=_tbl_name;
	_tbl_name	="character c,guild g";
	bool l_bret =false;
	try{
		l_bret	=_get_row(m_buf, CHA_MAXCOL, const_cast<char*>(param.c_str()), filter,&l_retrow);
	}catch(...)
	{
		//LG("group_sql", "TBLCharacters::FetchRowByChaIDSQL, !,atorID =%d\n", atorID);
		ToLogService("group_sql", "TBLCharacters::FetchRowByChaID execute SQL, failed!,atorID ={}", atorID);
	}
	_tbl_name	=l_tblname;
	if(l_bret)
	{
		if(l_retrow ==1 && get_affected_rows() ==1)
		{
            return l_retrow;
		}else
		{
			return 0;
		}
	}else
	{
		return -1;
	}
}
bool TBLCharacters::BackupRow(int atorID)
{
	char sql[SQL_MAXLEN];
	//insert into character_log (atorID, atorNome, ato_id, guild_id, job, degree, exp, hp, sp, ap, tp, gd, str, dex, agi, con, sta, luk, map, map_x, map_y, radius, look)
	//	select * from character where atorID =11

	string buf[3];
	char filter[80];
	const char*param	="guild_id, guild_stat";
	sprintf(filter, "atorID =%d",atorID);
	int	 l_retrow =0;
	DWORD dwGuildID;
	BYTE  byType;
	bool bret = _get_row(buf, 2, param, filter, &l_retrow);
	if(l_retrow ==1)
	{
		dwGuildID = atoi(buf[0].c_str());
		byType = atoi(buf[1].c_str());
		if( dwGuildID > 0 )
		{
			// 
			if( byType == emGldMembStatNormal )
			{
				// 
				sprintf(sql,"update guild set member_total =member_total -1 where guild_id =%d and member_total > 0", dwGuildID );
				SQLRETURN l_sqlret = Exec_sql_direct(sql, this);
				if( !DBOK(l_sqlret) )
				{
					//LG( "", "1>Reject:sql.ret = ", l_sqlret );
		ToLogService("GuildSystem", "1>Reject:delete chaupdate guild count failed! database sql failed .ret = {}", l_sqlret);
					return false;
				}
				else
				{
					if(get_affected_rows() !=1)
					{
						//LG( "", "2>Reject:sql.ret = ", l_sqlret );
		ToLogService("GuildSystem", "2>Reject:delete chaupdate guild count failed! database sql failed .ret = {}", l_sqlret);
						return false;

					}
					else
					{
					}
				}
			}
			else
			{
				// 
				sprintf(sql,"update guild set try_total =try_total -1 where guild_id =%d and try_total > 0", dwGuildID);
				SQLRETURN l_sqlret = Exec_sql_direct(sql, this);
				if( !DBOK(l_sqlret) )
				{
					//LG( "", "1>BackupRow:sql.ret = ", l_sqlret );
		ToLogService("GuildSystem", "1>BackupRow:delete chaupdate guild count failed! database sql failed .ret = {}", l_sqlret);
					return false;
				}
				else
				{
					if(get_affected_rows() !=1)
					{
						//LG( "", "2>BackupRow:sql.ret = ", l_sqlret );
		ToLogService("GuildSystem", "2>BackupRow:delete chaupdate guild count failed! database sql failed .ret = {}", l_sqlret);
						return false;
					}
					else
					{

					}
				}
			}
		}
	}else
	{
		//LG( "", "BackupRow:sql.atorID = ", atorID );
		ToLogService("GuildSystem", "BackupRow:delete chaget guild info failed! database sql failed.atorID = {}", atorID);
		return false;
	}


	//sprintf(sql, "delete from %s where atorID=%d",_get_table(), atorID);
	sprintf(sql, "update %s set delflag =1,deldate =getdate() where atorID=%d",_get_table(), atorID);   //  
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

//==========TBLFriends===============================
int TBLFriends::GetFriendsCount(long cha_id1,long cha_id2)
{
	int l_retrow	=0;
	char filter[200];

	const char* param1 = "count(*) num";
	sprintf(filter, "(cha_id1=%d AND cha_id2 =%d)OR(cha_id1=%d AND cha_id2 =%d)", cha_id1,cha_id2,cha_id2,cha_id1);

	if(_get_row(m_buf, FRD_MAXCOL, param1, filter,&l_retrow) && l_retrow ==1 && get_affected_rows() ==1)
	{
		return atoi(m_buf[0].c_str());
	}else
	{
		return -1;
	}
}

int TBLFriends::GetGroupCount(long cha_id1)
{
	int l_retrow	=0;
	char filter[200];
	char buffer[255];
	memset(buffer, 0, sizeof(buffer));

	const char* param1 = "count(*) num";
	sprintf(filter, "1=1");
	_tbl_name	="(select distinct friends.relation relation from friends\
						where friends.cha_id1 =%d and friends.cha_id2 = -1) cc";

	sprintf(buffer, _tbl_name.c_str(), cha_id1);

	_tbl_name = buffer;

	if(_get_row(m_buf, FRD_MAXCOL, param1, filter,&l_retrow) && l_retrow ==1 && get_affected_rows() ==1)
	{
		_tbl_name ="friends";
		return atoi(m_buf[0].c_str());
	}else
	{
		_tbl_name ="friends";
		return -1;
	}
}
unsigned long TBLFriends::GetFriendAddr(long cha_id1,long cha_id2)
{
	int l_retrow	=0;
	char filter[200];

	const char* param = "character.endeMem addr";
	sprintf(filter, "(friends.cha_id1=%d AND friends.cha_id2 =%d)", cha_id1,cha_id2);
	_tbl_name	="character (nolock) INNER JOIN friends ON character.atorID = friends.cha_id2";
	if(_get_row(m_buf, FRD_MAXCOL, param, filter,&l_retrow) && l_retrow ==1 && get_affected_rows() ==1)
	{
		_tbl_name ="friends";
		return atoi(m_buf[0].c_str());
	}else
	{
		_tbl_name ="friends";
		return 0;
	}
}

bool TBLFriends::UpdateGroup(long cha_id1, long cha_id2, const char *newgroup)
{
	char sql[SQL_MAXLEN];

	sprintf(sql,  "update %s set relation ='%s' where cha_id1=%d AND cha_id2 =%d",
			_get_table(),newgroup,cha_id1,cha_id2);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

bool TBLFriends::UpdateGroup(long cha_id1,const char *oldgroup,const char *newgroup)
{
	char sql[SQL_MAXLEN];

	sprintf(sql, "update %s set relation ='%s' where cha_id1=%d AND relation = '%s'",
			_get_table(),newgroup,cha_id1,oldgroup);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

bool TBLFriends::AddFriend(long cha_id1,long cha_id2)
{
	char sql[SQL_MAXLEN];

	sprintf(sql, "insert %s (cha_id1,cha_id2,relation) values(%d,%d,'%s')",
			_get_table(),cha_id1,cha_id2,gc_standard_group);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);

	if(DBOK(l_ret) && get_affected_rows() ==1)
	{
		sprintf(sql, "insert %s (cha_id1,cha_id2,relation) values(%d,%d,'%s')",
				_get_table(),cha_id2,cha_id1,gc_standard_group);
		SQLRETURN l_ret =Exec_sql_direct(sql, this);
		return (DBOK(l_ret))?true:false;
	}
	return false;
}
bool TBLFriends::DelFriend(long cha_id1,long cha_id2)
{
	char sql[SQL_MAXLEN];

	sprintf(sql, "delete %s where (cha_id1=%d AND cha_id2 =%d)OR(cha_id1=%d AND cha_id2 =%d)",
			_get_table(),cha_id1,cha_id2,cha_id2,cha_id1);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

//==========TBLMaster===============================
int TBLMaster::GetMasterCount(long atorID)
{
	int l_retrow	=0;
	char filter[200];

	const char* param1 = "count(*) num";
	sprintf(filter, "(cha_id1=%d)", atorID);
	if(_get_row(m_buf, MASTER_MAXCOL, param1, filter,&l_retrow) && l_retrow ==1 && get_affected_rows() ==1)
	{
		return atoi(m_buf[0].c_str());
	}else
	{
		return -1;
	}
}

int TBLMaster::GetPrenticeCount(long atorID)
{
	int l_retrow	=0;
	char filter[200];

	const char* param1 = "count(*) num";
	sprintf(filter, "(cha_id2=%d AND finish=0)", atorID);
	if(_get_row(m_buf, MASTER_MAXCOL, param1, filter,&l_retrow) && l_retrow ==1 && get_affected_rows() ==1)
	{
		return atoi(m_buf[0].c_str());
	}else
	{
		return -1;
	}
}

int TBLMaster::HasMaster(long cha_id1,long cha_id2)
{
	int l_retrow	=0;
	char filter[200];

	const char* param1 = "count(*) num";
	sprintf(filter, "(cha_id1=%d AND cha_id2=%d)", cha_id1, cha_id2);
	if(_get_row(m_buf, MASTER_MAXCOL, param1, filter,&l_retrow) && l_retrow ==1 && get_affected_rows() ==1)
	{
		return atoi(m_buf[0].c_str());
	}else
	{
		return -1;
	}
}

bool TBLMaster::AddMaster(long cha_id1,long cha_id2)
{
	char sql[SQL_MAXLEN];

	sprintf(sql, "insert %s (cha_id1,cha_id2,finish) values(%d,%d,%d)",
		_get_table(),cha_id1,cha_id2,0);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

bool TBLMaster::DelMaster(long cha_id1,long cha_id2)
{
	char sql[SQL_MAXLEN];

	sprintf(sql, "delete %s where (cha_id1=%d AND cha_id2 =%d)",
		_get_table(),cha_id1,cha_id2);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

bool TBLMaster::FinishMaster(long atorID)
{
	char sql[SQL_MAXLEN];

	sprintf(sql, "update %s set finish=1 where cha_id1=%d",
		_get_table(),atorID);
	SQLRETURN l_ret =Exec_sql_direct(sql, this);
	return (DBOK(l_ret))?true:false;
}

bool TBLMaster::InitMasterRelation(map<uLong, uLong> &mapMasterRelation)
{
	static char const query_master_format[SQL_MAXLEN] =
		"select cha_id1 cha_id1,cha_id2 cha_id2 from %s";

	bool ret = false;
	char sql[SQL_MAXLEN];
	sprintf(sql, query_master_format,_get_table());

	// 
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try
	{
		do
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

			// Fetch each Row
			for (int i = 0; ((sqlret = SQLFetch(hstmt)) == SQL_SUCCESS) || (sqlret == SQL_SUCCESS_WITH_INFO); ++ i)
			{
				if (sqlret != SQL_SUCCESS) handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);

				uLong ulPID = atoi((char const *)_buf[0]);
				uLong ulMID = atoi((char const *)_buf[1]);

				mapMasterRelation[ulPID] = ulMID;
			}

			SQLFreeStmt(hstmt, SQL_CLOSE);
			SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
			SQLFreeStmt(hstmt, SQL_UNBIND);
			ret = true;

		} while (0);
	}
	catch (...)
	{
		ToLogService("Master", "Unknown Exception raised when InitMasterRelation()");
	}

	if (hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}

bool TBLMaster::GetMasterData(master_dat* farray, int& array_num, unsigned int atorID)
{
	//NOTE(Ogge): Remember the size of farray before resetting array_num to be used as a return value
	const auto array_size{ array_num };
	array_num = 0;

	constexpr const char query_master_format[] =
		"select '' relation,count(*) addr,0 atorID,'' atorNome,0 icon,'' motto from ( \
		select distinct master.relation relation from character INNER JOIN \
		master ON character.atorID = master.cha_id2 where master.cha_id1 = %d \
		) cc union select master.relation relation,count(character.endeMem) addr,0 \
		atorID,'' atorNome,1 icon,'' motto from character INNER JOIN master ON \
		character.atorID = master.cha_id2 where master.cha_id1 = %d group by relation \
		union select master.relation relation,character.endeMem addr,character.atorID \
		atorID,character.atorNome atorNome,character.icon icon,character.motto motto \
		from character INNER JOIN master ON character.atorID = master.cha_id2 \
		where master.cha_id1 = %d order by relation,atorID,icon";

	if (!farray || array_size <= 0 || atorID == 0) return false;

	bool ret = false;
	char sql[SQL_MAXLEN];
	sprintf(sql, query_master_format, atorID,atorID, atorID);

	// 
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try
	{
		do
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

			// Fetch each Row
			int i = 0;
			for (; ((sqlret = SQLFetch(hstmt)) == SQL_SUCCESS) || (sqlret == SQL_SUCCESS_WITH_INFO); ++ i)
			{
				if (i >= array_size)
				{
					break;
				}

				if (sqlret != SQL_SUCCESS) handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);

				farray[i].relation = (char const *)_buf[0];
				farray[i].memaddr = atoi((char const *)_buf[1]);
				farray[i].atorID = atoi((char const *)_buf[2]);
				farray[i].atorNome = (char const *)_buf[3];
				farray[i].icon_id = atoi((char const *)_buf[4]);
				farray[i].motto = (char const *)_buf[5];
			}

			array_num = i; //NOTE(Ogge): Return number of rows read through referenced argument

			SQLFreeStmt(hstmt, SQL_CLOSE);
			SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
			SQLFreeStmt(hstmt, SQL_UNBIND);
			ret = true;

		} while (0);
	}
	catch (...)
	{
		ToLogService("Master", "Unknown Exception raised when GetMasterData()");
	}

	if (hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}

bool TBLMaster::GetPrenticeData(master_dat* farray, int& array_num, unsigned int atorID)
{
	const auto array_size{ array_num };
	array_num = 0;

	constexpr const char query_prentice_format[] =
		"select '' relation,count(*) addr,0 atorID,'' atorNome,0 icon,'' motto from ( \
		select distinct master.relation relation from character INNER JOIN \
		master ON character.atorID = master.cha_id1 where master.cha_id2 = %d \
		) cc union select master.relation relation,count(character.endeMem) addr,0 \
		atorID,'' atorNome,1 icon,'' motto from character INNER JOIN master ON \
		character.atorID = master.cha_id1 where master.cha_id2 = %d group by relation \
		union select master.relation relation,character.endeMem addr,character.atorID \
		atorID,character.atorNome atorNome,character.icon icon,character.motto motto \
		from character INNER JOIN master ON character.atorID = master.cha_id1 \
		where master.cha_id2 = %d order by relation,atorID,icon";

	if (!farray || array_size <= 0 || atorID == 0)
	{
		return false;
	}

	bool ret = false;
	char sql[SQL_MAXLEN];
	sprintf(sql, query_prentice_format, atorID, atorID, atorID);

	// 
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try
	{
		do
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

			// Fetch each Row
			int i = 0;
			for (; ((sqlret = SQLFetch(hstmt)) == SQL_SUCCESS) || (sqlret == SQL_SUCCESS_WITH_INFO); ++ i)
			{
				if (i >= array_size)
				{
					break;
				}

				if (sqlret != SQL_SUCCESS) handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);

				farray[i].relation = (char const *)_buf[0];
				farray[i].memaddr = atoi((char const *)_buf[1]);
				farray[i].atorID = atoi((char const *)_buf[2]);
				farray[i].atorNome = (char const *)_buf[3];
				farray[i].icon_id = atoi((char const *)_buf[4]);
				farray[i].motto = (char const *)_buf[5];
			}

			array_num = i; // 

			SQLFreeStmt(hstmt, SQL_CLOSE);
			SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
			SQLFreeStmt(hstmt, SQL_UNBIND);
			ret = true;

		} while (0);
	}
	catch (...)
	{
		ToLogService("Master", "Unknown Exception raised when GetPrenticeData()");
	}

	if (hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}

//==========TBLGuilds===============================
bool TBLGuilds::IsReady()
{
	int l_retrow	=0;
	const char* param = "count(*)";
	if(_get_row(m_buf, 1, param, 0,&l_retrow))
	{
		if (l_retrow == 1 &&
			get_affected_rows() == 1 &&
			atoi(m_buf[0].c_str()) == 199) // NOTE: 199 is the maximum guild count
		{
			return true;
		}
	}
	return false;
}

int TBLGuilds::FetchRowByName(const char *guild_name)
{
	int l_retrow	=0;
	const char* param = "guild_id";
	char filter[200];
	sprintf(filter, "guild_name='%s'", guild_name);
	if(_get_row(m_buf, 1, param, filter,&l_retrow))
	{
		if(l_retrow ==1 && get_affected_rows() ==1)
		{
			return l_retrow;
		}else
		{
			return 0;
		}
	}else
	{
		return -1;
	}
}
bool TBLGuilds::Disband(uLong gldid)
{
	char sql[SQL_MAXLEN];
	sprintf(sql,	"update guild set motto ='',passwd ='',leader_id =0, money =0,exp =0,member_total =0,try_total =0\
						where guild_id =%d",gldid);
	SQLRETURN l_sqlret =Exec_sql_direct(sql, this);
	if(!DBOK(l_sqlret))
	{
		if(DBNODATA(l_sqlret))
		{
		//l_line<<newln<<...;
		ToLogService("Guild", "dismiss guild SQL failed2! guild ID:{}", gldid);
			return false;
		}else
		{
		//l_line<<newln<<...;
		ToLogService("Guild", "dismiss guild SQL failed1! guild ID:{}", gldid);
			return false;	//SQL
		}
	}
	sprintf(sql,	"update character set guild_id =0 ,guild_stat =0,guild_permission =0\
						where guild_id =%d",
				gldid);
	l_sqlret =Exec_sql_direct(sql, this);
	if(!DBOK(l_sqlret))
	{
		//l_line<<newln<<...;
		ToLogService("Guild", "dismiss guild SQL failed3! guild ID:{}", gldid);
		return false;	//SQL
	}

	return true;
}
bool TBLGuilds::InitAllGuilds(char disband_days)
{
	string sql_syntax ="";
	if(disband_days<1)
	{
		return false;
	}else
	{
		/*
		sql_syntax =
			"	select g.guild_id, g.guild_name, g.motto, g.leader_id,g.type,g.stat,\
						g.money, g.exp, g.member_total, g.try_total,g.disband_date,\
						case when g.stat>0 then DATEDIFF(mi,g.disband_date,GETDATE()) else 0 end  ,\
						case when g.stat>0 then %d*24*60 -DATEDIFF(mi,g.disband_date,GETDATE()) else 0 end \
					from guild As g\
					where (g.guild_id >0)\
			";
		*/	
		sql_syntax =
			string("	select g.guild_id, g.guild_name, g.motto, g.leader_id,\
						g.exp, g.member_total, g.try_total,g.disband_date")
						+ string(",g.level from guild As g where (g.guild_id >0) ");
	}

	bool l_ret = false;
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax.c_str(), disband_days);

	// 
	SQLRETURN sqlret;
	SQLHSTMT hstmt	= SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found		= true;

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

		// Fetch each Row	int i; // 
		for (int f_row = 1; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO; ++ f_row)
		{
			if (sqlret != SQL_SUCCESS)
			{
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			}
			Guild* l_gld = Guild::Alloc();
			l_gld->m_id = atol((cChar*) _buf[0]);			//ID
			strcpy(l_gld->m_name, (cChar*)_buf[1]);			//
			strcpy(l_gld->m_motto,(cChar*)_buf[2]);			//
			l_gld->m_leaderID = atol((cChar*)_buf[3]);		//ID
			l_gld->m_remain_minute = atol((cChar*)_buf[7]);	//
			l_gld->m_tick = GetTickCount();

			l_gld->BeginRun();
		}

		SQLFreeStmt(hstmt, SQL_UNBIND);
		l_ret = true;
	}catch(int&e)
	{
		//l_line<<newln<<...;
		ToLogService("Guild", "init guild ODBC interface failed, InitAllGuilds() error:{}", e);
	}catch (...)
	{
		ToLogService("Guild", "Unknown Exception raised when InitAllGuilds()");
	}

	if (hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return l_ret;
}
bool TBLGuilds::SendGuildInfo(Player *ply)
{
	WPacket l_togmSelf  =g_gpsvr->GetWPacket();
	l_togmSelf.WriteCmd( CMD_PM_GUILDINFO );
	l_togmSelf.WriteLong( ply->m_chaid[ply->m_currcha] ); // DBID
	l_togmSelf.WriteLong(ply->m_guild[ply->m_currcha]);	//ID	
	l_togmSelf.WriteLong(ply->GetGuild()->m_leaderID);	//ID	
	l_togmSelf.WriteString(ply->GetGuild()->m_name);	//name
	l_togmSelf.WriteString(ply->GetGuild()->m_motto);	//
	ply->m_gate->GetDataSock()->SendData( l_togmSelf );
	return true;
}
bool TBLGuilds::InitGuildMember(Player *ply,uLong chaid,uLong gldid,int mode)
{
	bool l_ret = false;
	if(ply && gldid ==0)
	{
		WPacket l_toSelf  =g_gpsvr->GetWPacket();
		l_toSelf.WriteCmd(CMD_PC_GUILD);
		l_toSelf.WriteChar(MSG_GUILD_START);

		l_toSelf.WriteLong(0);
		l_toSelf.WriteChar(0);

		g_gpsvr->SendToClient(ply,l_toSelf);
	}else
	{
		const char *sql_syntax =0;
		char sql[SQL_MAXLEN];
		sql_syntax =
			"	select c.endeMem,c.atorID, c.atorNome, c.motto, c.job, c.degree, c.icon, c.guild_permission\
					from character As c\
					where (c.guild_stat =0) and (c.guild_id =%d) and (c.delflag = 0)\
			";
		sprintf(sql, sql_syntax, gldid);
		// 
		SQLRETURN sqlret;
		SQLHSTMT hstmt	= SQL_NULL_HSTMT;
		SQLSMALLINT col_num = 0;
		bool found		= true;

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
			WPacket l_toGuild =g_gpsvr->GetWPacket();
			l_toGuild.WriteCmd(CMD_PC_GUILD);
			if(mode)
			{
				l_toGuild.WriteChar(MSG_GUILD_ADD);
			}else
			{
				l_toGuild.WriteChar(MSG_GUILD_ONLINE);
				l_toGuild.WriteLong(chaid);
			}

			WPacket l_toSelf,l_wpk0;
			if(ply)
			{
				l_wpk0  =g_gpsvr->GetWPacket();
				l_wpk0.WriteCmd(CMD_PC_GUILD);
				l_wpk0.WriteChar(MSG_GUILD_START);
			}
			bool	l_hrd	=false;

			Player *l_plylst[10240];
			short	l_plynum	=0;

			long lPacketNum = 0;

			// Fetch each Row	int i; // 
			int f_row = 1;
			for (; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO; ++ f_row)
			{
				if (sqlret != SQL_SUCCESS)
				{
					handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
				}
				if(ply && (f_row %20) ==1)
				{
					l_toSelf	=l_wpk0;
				}
				if(ply && !l_hrd)
				{
					l_hrd	=true;
					l_toSelf.WriteLong(ply->m_guild[ply->m_currcha]);	//ID
					l_toSelf.WriteString(ply->GetGuild()->m_name);		//name
					l_toSelf.WriteLong(ply->GetGuild()->m_leaderID);	//ID
				}
				uLong l_memaddr		=atol((cChar *)_buf[0]);
				if(l_memaddr)
				{
					l_plylst[l_plynum] = ToPointer<Player>(l_memaddr);
					l_plynum++;
				}
				if(mode && chaid ==atol((cChar*)_buf[1]))
				{
					l_toGuild.WriteChar(l_memaddr?1:0);			//online
					l_toGuild.WriteLong(atol((cChar*)_buf[1]));	//chaid
					l_toGuild.WriteString(	(cChar*)_buf[2]);	//chaname
					l_toGuild.WriteString(	(cChar*)_buf[3]);	//motto
					l_toGuild.WriteString(	(cChar*)_buf[4]);	//job
					l_toGuild.WriteShort(atoi((cChar*)_buf[5]));//degree
					l_toGuild.WriteShort(atoi((cChar*)_buf[6]));//icon
					l_toGuild.WriteLong(stoull((cChar*)_buf[7]));//permission
				}
				if(ply)
				{
					l_toSelf.WriteChar(l_memaddr?1:0);			//online
					l_toSelf.WriteLong(atol((cChar*)_buf[1]));	//chaid
					l_toSelf.WriteString(	(cChar*)_buf[2]);	//chaname
					l_toSelf.WriteString(	(cChar*)_buf[3]);	//motto
					l_toSelf.WriteString(	(cChar*)_buf[4]);	//job
					l_toSelf.WriteShort(atoi((cChar*)_buf[5]));	//degree
					l_toSelf.WriteShort(atoi((cChar*)_buf[6]));	//icon
					l_toSelf.WriteLong(stoull((cChar*)_buf[7]));	//permission
				}
				if(ply && !(f_row %20))
				{
					l_toSelf.WriteLong(lPacketNum);
					lPacketNum++;
					l_toSelf.WriteChar(((f_row-1)%20)+1);	//
					g_gpsvr->SendToClient(ply,l_toSelf);
				}
			}
			if(ply && (f_row%20) ==1)
			{
				l_toSelf	=l_wpk0;
			}
			if(ply && !l_hrd)
			{
				l_hrd	=true;
				l_toSelf.WriteLong(ply->m_guild[ply->m_currcha]);	//ID
				l_toSelf.WriteString(ply->GetGuild()->m_name);		//name
				l_toSelf.WriteLong(ply->GetGuild()->m_leaderID);	//ID
			}
			if(ply)
			{
				l_toSelf.WriteLong(lPacketNum);
				lPacketNum++;
				l_toSelf.WriteChar((f_row -1)%20);
				g_gpsvr->SendToClient(ply,l_toSelf);
			}
	//l_line<<newln<<...;
	ToLogService("Guild", "online guild num:{}", l_plynum);
			g_gpsvr->SendToClient(l_plylst,l_plynum,l_toGuild);

			SQLFreeStmt(hstmt, SQL_UNBIND);
			l_ret = true;
		}catch(int&e)
		{
		//l_line<<newln<<...;
		ToLogService("Guild", "init guild ODBC interface failed, InitGuildMember() error:{} sql:{}", e, sql);
		}catch (...)
		{
		ToLogService("Guild", "Unknown Exception raised when InitGuildMember()");
		}

		if (hstmt != SQL_NULL_HSTMT)
		{
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
			hstmt = SQL_NULL_HSTMT;
		}
	}

	return l_ret;
}


bool TBLParam::InitParam(void)
{
	string strSQL = "select param1,param2,param3,param4,param5,param6,param7,param8,param9,param10 from param where id = 1";
//	string strSQL = "select param1 from param where id = 1";

	SQLRETURN sqlret;
	SQLHSTMT hstmt	= SQL_NULL_HSTMT;
	SQLINTEGER buf_len[MAXORDERNUM+MAXORDERNUM];
//	SQLINTEGER		nID = 0,nlen;
	bool found		= true;

	try
	{
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO))
		{
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret);
			throw 1;
		}
		sqlret = SQLSetStmtAttr( hstmt,SQL_ATTR_ROW_ARRAY_SIZE,(SQLPOINTER)1,0);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO))
		{
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret);
			throw 1;
		}
		SQLBindCol(hstmt, 1, SQL_C_LONG, &m_nOrder[0].nid, 0, &(buf_len[0]));
		SQLBindCol(hstmt, 2, SQL_C_LONG, &m_nOrder[1].nid, 0, &buf_len[1]);
		SQLBindCol(hstmt, 3, SQL_C_LONG, &m_nOrder[2].nid, 0, &buf_len[2]);
		SQLBindCol(hstmt, 4, SQL_C_LONG, &m_nOrder[3].nid, 0, &buf_len[3]);
		SQLBindCol(hstmt, 5, SQL_C_LONG, &m_nOrder[4].nid, 0, &buf_len[4]);

		SQLBindCol(hstmt, 6, SQL_C_LONG, &m_nOrder[0].nfightpoint, 0, &buf_len[5]);
		SQLBindCol(hstmt, 7, SQL_C_LONG, &m_nOrder[1].nfightpoint, 0, &buf_len[6]);
		SQLBindCol(hstmt, 8, SQL_C_LONG, &m_nOrder[2].nfightpoint, 0, &buf_len[7]);
		SQLBindCol(hstmt, 9, SQL_C_LONG, &m_nOrder[3].nfightpoint, 0, &buf_len[8]);
		SQLBindCol(hstmt, 10,SQL_C_LONG, &m_nOrder[4].nfightpoint, 0, &buf_len[9]);
		sqlret = SQLExecDirect(hstmt,(SQLCHAR *)const_cast<char*>(strSQL.c_str()),SQL_NTS);
		if (sqlret != SQL_SUCCESS)
		{
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}

		sqlret = SQLFetch(hstmt);
		if (sqlret != SQL_SUCCESS)
		{
			// Modfi by lark.li 20080714 begin
			if(sqlret != SQL_NO_DATA)
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			// End
		}
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SQLFreeStmt(hstmt, SQL_UNBIND);	}
	catch(int&e)
	{
		//l_line<<newln<<...;
		ToLogService("Garner2", "init guild ODBC interface failed, InitParam() error:{}", e);
	}catch (...)
	{
		ToLogService("Garner2", "Unknown Exception raised when InitParam()");
	}

	char buff[255];
	int nlev;
	try
	{
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO))
		{
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret);
			throw 1;
		}
		SQLBindCol(hstmt, 1, SQL_C_CHAR, _buf[0], MAX_DATALEN, &buf_len[0]);
		SQLBindCol(hstmt, 2, SQL_C_CHAR, _buf[1], MAX_DATALEN, &buf_len[1]);
		SQLBindCol(hstmt, 3, SQL_C_ULONG, &nlev, 0, &buf_len[2]);
		for(int n = 0;n<MAXORDERNUM;n++)
		{
			sprintf(buff,"select atorNome,job,degree from character where atorID = %d ",m_nOrder[n].nid);

			sqlret = SQLExecDirect(hstmt,(SQLCHAR *)buff,SQL_NTS);
			if (sqlret != SQL_SUCCESS)
			{
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
				if (sqlret != SQL_SUCCESS_WITH_INFO)
					throw 2;
			}
			int i =0;
			if((sqlret = SQLFetch(hstmt)) != SQL_NO_DATA) 
			{
				if(sqlret == SQL_NO_DATA)
				{
		//l_line<<newln<<...;
		ToLogService("Garner2", "cha name query failed .cha ID{}", m_nOrder[n].nid);
					continue;

				}
				if (sqlret != SQL_SUCCESS)
				{
					handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
				}
				if(buf_len[0] >20 )
				{
		//l_line<<newln<<...;
		ToLogService("Garner2", "cha name query failed.");
					return false;
				}
				memcpy(m_nOrder[n].strname,_buf[0],buf_len[0]);
				m_nOrder[n].strname[buf_len[0]]='\0';
				memcpy(m_nOrder[n].strjob,_buf[1],buf_len[1]);
				m_nOrder[n].strjob[buf_len[1]]='\0';
				m_nOrder[n].nlev = nlev;
			}
			SQLFreeStmt(hstmt, SQL_CLOSE);
		}
		SQLFreeStmt(hstmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	}
	catch(int&e)
	{
		//l_line<<newln<<...;
		ToLogService("Garner2", "init guild ODBC interface failed, InitParam() erro :{}", e);
	}catch (...)
	{
		ToLogService("Garner2", "Unknown Exception raised when InitParam()");
	}
	return true;
}

bool TBLParam::SaveParam(void)
{
	char buff[255];
	sprintf(buff,"update %s set param1 = %d,param2 = %d,param3 = %d,param4 = %d,param5 = %d,param6 = %d,param7 = %d,param8 = %d,param9 = %d,param10 = %d where id = 1",_get_table(),m_nOrder[0].nid,m_nOrder[1].nid,m_nOrder[2].nid,m_nOrder[3].nid,m_nOrder[4].nid,m_nOrder[0].nfightpoint,m_nOrder[1].nfightpoint,m_nOrder[2].nfightpoint,m_nOrder[3].nfightpoint,m_nOrder[4].nfightpoint);
	SQLRETURN sqlret;
	sqlret = _db->exec_sql_direct(buff);
	if(sqlret != SQL_SUCCESS)
	{
		ToLogService("ParamErr", "Save Param Error SQL = {}", buff);
	}
	return true;
}

bool TBLParam::IsReady()
{
	int l_retrow	=0;
	const char* param = "count(*)";
	if(_get_row(m_buf, 1, param, 0,&l_retrow))
	{
		if(l_retrow ==1 && get_affected_rows() ==1 && atoi(m_buf[0].c_str()) >=199)
		{
			return true;
		}
	}
	return false;
}

void TBLParam::UpdateOrder(ORDERINFO &Order)
{
	ORDERINFO ordertemp[MAXORDERNUM];

	memcpy(ordertemp,m_nOrder,sizeof(ORDERINFO)*MAXORDERNUM);


	int i = 0;
	int oldid = 0;
	for(i = 0;i< MAXORDERNUM;i++)
	{
		if(ordertemp[i].nfightpoint >= Order.nfightpoint)
		{
			if(ordertemp[i].nid == Order.nid)
				break;
			continue;
		}
		else
		{
			oldid = i;
			if(ordertemp[i].nid == Order.nid)
			{
				m_nOrder[i].nfightpoint = Order.nfightpoint;
				break;
			}
			memcpy(&m_nOrder[i++],&Order,sizeof(ORDERINFO));

			int n=-1;
			for(int a = i;a<MAXORDERNUM;)
			{
				if(ordertemp[a+n].nid == Order.nid)
				{
					n++;
					continue;
				}

				if(a+n < MAXORDERNUM)
					memcpy(&m_nOrder[a],&ordertemp[a+n],sizeof(ORDERINFO));
				else
				{
					strcpy(m_nOrder[a].strjob,"");
					strcpy(m_nOrder[a].strname,"");
					m_nOrder[a].nid = -1;
					m_nOrder[a].nlev = 0;
					m_nOrder[a].nfightpoint=0;
				}
				a++;
			}

			SaveParam();
			WPacket l_wpk = g_gpsvr->GetWPacket();
			l_wpk.WriteCmd(CMD_PM_GARNER2_UPDATE);
			for(i = 0;i < MAXORDERNUM;i++)
			{
				l_wpk.WriteLong(m_nOrder[i].nid);
			}
			l_wpk.WriteLong(oldid);
			l_wpk.WriteLong(0);

			for (auto& gate : g_gpsvr->m_gate)
			{
				if (gate.GetDataSock())
				{
					gate.GetDataSock()->SendData(l_wpk);
					break;
				}
			}
		//l_line<<newln<<...;
		ToLogService("Garner2", "order chaned");
			break;
		}
	}
}
