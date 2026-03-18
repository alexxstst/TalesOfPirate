#pragma once

#include "NetIF.h"
#include "compcommand.h"

// Add by lark.li 20080807 begin
struct stPersonInfo;
struct stQueryPersonInfo;
// End

/****************************************************************
		����Packet�������������
		����:������������Ա
		�޸�:������������Ա
		����:�ͻ��˿�����Ա
		�������������ļ�"NetPkXXX.cpp"(ע��:��Pk,��Ϊ�����������cpp�ļ�)
*****************************************************************/
extern void CS_GM1Say(const char *pszContent);
extern void CS_GM1Say1(const char *pszContent, DWORD color);//Add by sunny.sun20080804
extern void CS_Say2Trade(const char *pszContent);
extern void CS_Say2All(const char *pszContent);
extern void CS_Say2You(const char *you,const char *pszContent);
extern void CS_Say2Team(const char *pszContent);
extern void CS_Say2Guild( const char* pszContent);
extern void CP_RefuseToMe(bool refusetome);

extern void	CS_Sess_Create(const char *chaname[],unsigned char chanum);
extern void	CS_Sess_Say(unsigned long sessid,const char *word);
extern void	CS_Sess_Add(unsigned long sessid,const char *chaname);
extern void	CS_Sess_Leave(unsigned long sessid);

extern void	CS_Team_Invite(const char *chaname);
extern void	CS_Team_Refuse(unsigned long chaid);
extern void	CS_Team_Confirm(unsigned long chaid);
extern void	CS_Team_Leave();
extern void CS_Team_Kick( DWORD dwKickedID );

extern void	CS_Frnd_Invite(const char *chaname);
extern void	CS_Frnd_Refuse(unsigned long chaid);
extern void	CS_Frnd_Confirm(unsigned long chaid);
extern void	CS_Frnd_Delete(unsigned long chaid);

extern void	CP_Frnd_Refresh_Info(unsigned long chaid);

extern void	CP_Change_PersonInfo(const char *motto,unsigned short icon,bool refuse_sess);//refuse_sess =true,�������Զ��ܾ��Ự =false,���ջỰ.

/****************************************************************
		����Packet����õı��ؽӿ�������
		����:������������Ա
		�޸�:�ͻ��˿�����Ա
		����:�����յ�Packet�������������Ρ���Ӧ����
		�������������ļ�"NetXXX.cpp"(ע��:û��Pk,�����NetPkXXX,û�а�����)
*****************************************************************/

struct stNetSay2You			// ����˵��
{
	std::string m_src;		// ˵����
	std::string m_dst;		// ������
	std::string m_content;	// ����
};

struct stNetSay2All		// ��������˵��
{
	std::string m_src;	//˵����
	std::string m_content;//����
};

struct stNetScrollSay//��˵����˵�� ������ʾ
{
	std::string m_content;//��ʾ����
	int		setnum;//��ʾ����
	DWORD	color;
};

struct stNetTeamChaPart // ��ӳ�Ա�����
{
    struct SItem
    {
	    short	sID;		// ���߱��е�ID��0��ʾû�е��ߣ�
	    short	sNum;		// ���߸���
	    char	chForgeLv;	// �����ȼ�
		long lFuseID;
    };

	short		sTypeID;
	SItem	    SLink[enumEQUIP_NUM];
    short       sHairID;        // Ĭ�ϵ�ͷ��������

	void		Convert( const stNetChangeChaPart& stPart );
};

struct stNetTeamState
{
	unsigned long	ulID;			// ��ɫΨһID
	BYTE			byWork;			// ְҵ
	long			lHP;
	long			lSP;
	long			lLV;
    long            lMaxHP;
    long            lMaxSP;

    stNetTeamChaPart    SFace;       // ���
};

struct stNetPCTeam      // from group
{
	unsigned char kind;	// �ο�Common:netcommand.h - TEAM_MSG ��ʼ��
	unsigned char count;//����Ԫ�صĸ���;
	unsigned long cha_dbid[10];
	char		  cha_name[10][33];
	char		  motto[10][33];
	short		  cha_icon[10];
};

//struct stMemberInfo
//{
//	DWORD dwMessage;
//	DWORD dwID;
//	string sName;
//	string sMottoName;
//	DWORD dwIcon;
//	DWORD dwLv;
//	string sJobName;
//	BYTE reason;
//}S_MEMBERINFO;
extern void NetSay2You(stNetSay2You &say2you,DWORD dwColour);
extern void	NetSay2Team(unsigned long chaid,const char *word,DWORD dwColour);
extern void NetSay2Gud(const char* src, const char* word,DWORD dwColour);
extern void NetSay2All(stNetSay2All &say2all,DWORD dwColour);
extern void NetSay2Trade(stNetSay2All &say2all,DWORD dwColour);

extern void NetGM1Say(stNetSay2All &say2all);
extern void NetGM1Say1(stNetScrollSay &say2all);//Add by sunny.sun20080804

extern void NetTeamInvite(const char *inviter_name, unsigned long inviter_chaid,unsigned short icon);
extern void NetTeamCancel(unsigned long  inviter_chaid,char reason);
extern void NetPCTeam(stNetPCTeam	&pcteam);
extern void NetSynTeam(stNetTeamState *pSTeamState);

struct stNetFrndStart
{
	std::string		szGroup;	//������������
	unsigned long	lChaid;
	std::string		szChaname;
	std::string		szMotto;
	unsigned short	sIconID;
	unsigned char	cStatus;	//0-���ߣ���0-����
};

extern void NetFrndInvite(const char *inviter_name, unsigned long inviter_chaid,unsigned short icon);
extern void NetFrndCancel(unsigned long  inviter_chaid,char reason);
extern void NetFrndOnline(unsigned long cha_id);
extern void NetFrndOffline(unsigned long cha_id);
extern void NetFrndDel(unsigned long cha_id);
extern void NetFrndAdd(unsigned long cha_id,const char * cha_name,const char * motto,unsigned short icon_id,const char *grp);

extern void NetFrndStart(stNetFrndStart &self,stNetFrndStart arrCha[],unsigned short arrnum);

extern void NetGMStart(stNetFrndStart arrCha[],unsigned short arrnum);
extern void NetGMOnline(unsigned long cha_id);
extern void NetGMOffline(unsigned long cha_id);
extern void NetGMDel(unsigned long cha_id);
extern void NetGMAdd(unsigned long cha_id,const char * cha_name,const char * motto,unsigned short icon_id,const char *grp);
extern void NetFrndRefreshInfo(unsigned long cha_id,const char * motto,unsigned short icon,unsigned short degree,const char * job,const char *guildname);

// ʦ��
extern void NetMasterOnline(unsigned long cha_id);
extern void NetMasterOffline(unsigned long cha_id);
extern void NetMasterDel(unsigned long cha_id);
extern void NetMasterAdd(unsigned long cha_id,const char * cha_name,const char * motto,unsigned short icon_id,const char *grp);
extern void NetMasterStart(stNetFrndStart &self,stNetFrndStart arrCha[],unsigned short arrnum);
extern void NetMasterCancel(unsigned long  inviter_chaid,char reason);
extern void NetMasterRefreshInfo(unsigned long cha_id,const char * motto,unsigned short icon,unsigned short degree,const char * job,const char *guildname);

// ͽ��
extern void NetPrenticeOnline(unsigned long cha_id);
extern void NetPrenticeOffline(unsigned long cha_id);
extern void NetPrenticeDel(unsigned long cha_id);
extern void NetPrenticeAdd(unsigned long cha_id,const char * cha_name,const char * motto,unsigned short icon_id,const char *grp);
extern void NetPrenticeStart(stNetFrndStart &self,stNetFrndStart arrCha[],unsigned short arrnum);
extern void NetPrenticeCancel(unsigned long  inviter_chaid,char reason);
extern void NetPrenticeRefreshInfo(unsigned long cha_id,const char * motto,unsigned short icon,unsigned short degree,const char * job,const char *guildname);

extern void	NetChangePersonInfo(const char *motto,unsigned short icon,bool refuse_sess);


struct stNetSessCreate
{
	unsigned long	lChaID;
	std::string		szChaName;
	std::string		szMotto;
	unsigned short	sIconID;
};
extern void NetSessCreate(const char *chaname);
extern void NetSessCreate(unsigned long newsessid,stNetSessCreate *cha,short chanum);
extern void NetSessAdd(unsigned long sessid,stNetSessCreate *cha);
extern void NetSessLeave(unsigned long sessid,unsigned long chaid);
extern void NetSessSay(unsigned long sessid,unsigned long chaid,const char *word);
/****************************************************************
		���յ�Packet��������������
		����:������������Ա
		�޸�:������������Ա
		����:NetIF::HandlePacketMessage
		�������������ļ�"NetPkXXX.cpp"(ע��:��Pk,��Ϊ�����������cpp�ļ�)
*****************************************************************/

//˽��
extern BOOL	PC_Say2You(LPRPACKET pk);
extern BOOL	PC_GM1SAY(LPRPACKET pk);
extern BOOL	PC_GM1SAY1(LPRPACKET pk);//Add by sunny.sun20080804
extern BOOL	PC_SAY2TRADE(LPRPACKET pk);
extern BOOL	PC_Say2All(LPRPACKET pk);
extern BOOL	PC_Say2Team(LPRPACKET pk);
extern BOOL	PC_Say2Gud(LPRPACKET pk);

//��ӳ�Ա�仯��Ϣˢ��
extern BOOL	PC_SESS_CREATE(LPRPACKET pk);
extern BOOL	PC_SESS_ADD(LPRPACKET pk);
extern BOOL	PC_SESS_LEAVE(LPRPACKET pk);
extern BOOL	PC_SESS_SAY(LPRPACKET pk);

extern BOOL	PC_TEAM_INVITE(LPRPACKET pk);
extern BOOL	PC_TEAM_CANCEL(LPRPACKET pk);
extern BOOL	PC_TEAM_REFRESH(LPRPACKET pk);

extern BOOL	PC_FRND_INVITE(LPRPACKET pk);
extern BOOL	PC_FRND_CANCEL(LPRPACKET pk);
extern BOOL	PC_FRND_REFRESH(LPRPACKET pk);
extern BOOL	PC_FRND_REFRESH_INFO(LPRPACKET pk);
extern BOOL	PC_GM_INFO(LPRPACKET pk);

extern BOOL	PC_CHANGE_PERSONINFO(LPRPACKET pk);



