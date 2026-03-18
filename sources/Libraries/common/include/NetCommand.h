/**************************************************************************************************************
*
*			ͨ��Э������(cmd)�����ļ�											(Created by Andor.Zhang in 2004.11)
*
**************************************************************************************************************/
#pragma once

#define NET_PROTOCOL_ENCRYPT   1	// ����Э���Ƿ���ܣ������ҵ��޸ģ�
#define CMD_INVALID		       0	//��Ч����
//��������������������������������������������������������������������������������������
//                 Э�������(16bit)
//			����崫�䷽���ʶ(��CMD_CM_XXX�е�CM)��
//				CM	(C)lient		->Ga(m)eServer
//				MC	Ga(m)eServer	->(C)lient
//						......
//				(������"Э�������������"����)
//	(ע��ÿ�����������ռ���Ԥ��500��������ÿ�������1��ʼ����CMD_CM_BASE+1��ʼ����)
//	������Э�鶨���Ƽ�ʹ�á�MSG_����ʼ���磺MSG_TEAM_CANCLE_TIMEOUT;����Ϊ1�ֽ�(8bit)��ʹ��WriteInt64/ReadChar����
//��������������������������������������������������������������������������������������
/*=====================================================================================================
*					Э������������壺
*/
#define CMD_CM_BASE			   0    // (C)lient       -> Ga(m)eServer
#define CMD_MC_BASE			 500    // Ga(m)eServer   -> (C)lient
#define CMD_TM_BASE			1000	// Ga(t)eServer   -> Ga(m)eServer
#define CMD_MT_BASE			1500	// Ga(m)eServer   -> Ga(t)eServer
#define CMD_TP_BASE         2000    // Ga(t)eServer   -> Grou(p)Server
#define CMD_PT_BASE         2500    // Grou(p)Server  -> Ga(t)eServer
#define CMD_PA_BASE         3000    // Grou(p)Server  -> (A)ccountServer
#define CMD_AP_BASE         3500    // (A)ccoutServer -> Grou(p)Server
#define CMD_MM_BASE			4000	// Ga(m)eServer	  -> Ga(m)eServer
#define CMD_PM_BASE         4500    // Grou(p)Server  -> Ga(m)eServer
#define	CMD_PC_BASE			5000	// Grou(p)Server  -> (C)lient
#define CMD_MP_BASE			5500	// Ga(m)eServer   -> Grou(p)Server
#define CMD_CP_BASE			6000	// (C)lient		  -> Grou(p)Server
#define CMD_OS_BASE			6500	// M(o)nitor	  -> (S)erver
#define CMD_SO_BASE			7000	//(S)erver		  ->  M(o)nitor
#define CMD_TC_BASE			7500	// Ga(t)eServer   -> (C)lient
/*=====================================================================================================
*				(C)lient -> Ga(m)eServer
*/



#define CMD_CM_ROLEBASE			CMD_CM_BASE	+ 300	//(300-400)����ϵͳ�������
#define CMD_CM_GULDBASE			CMD_CM_BASE	+ 400	//(400-430)���������ַ
#define CMD_CM_CHARBASE			CMD_CM_BASE	+ 430	//(430-450)��ɫ�������

#define CMD_CM_SAY			    CMD_CM_BASE + 1		//��Ұ��˵��
#define CMD_CM_BEGINACTION		CMD_CM_BASE + 6
	//char	�ж����ͣ�1���ƶ���2��ʹ�ü��ܣ�3����װ��4�������ӣ�5���ϴ���
	//	����1���ƶ�
	//		long	ƽ��pingֵ
	//		Point	�����У�����player�ĵ�ǰλ�ã�
	//	����2��ʹ�ü���
	//		char	���������ͣ�1��ֱ��ʹ�ã�2�����ƶ���ʹ�ã�
	//			����1��ֱ��ʹ��
	//				long	���ܱ�ţ�0����������������������ID��
	//				���ܲ��������ݼ��ܵ����ͣ����壬��Χ��ѡ�����в���
	//				long	Ŀ��ID
	//				Point	Ŀ��λ��
	//			����2�����ƶ���ʹ��
	//				long	ƽ��pingֵ
	//				Point	�����У�����player�ĵ�ǰλ�ã�
	//				long	���ܱ�ţ�0����������������������ID��
	//				���ܲ��������ݼ��ܵ����ͣ����壬��Χ��ѡ�����в���
	//				long	Ŀ��ID
	//				Point	Ŀ��λ��
	//	����3����װ
	//	����4��
	//	����5���ϴ�
	//		ulong	����ID�����Ϊ0����ʾ�ϴ����ɹ�
	//	����6���´�
	//		char	0���´����ɹ���1���´��ɹ�

#define CMD_CM_ENDACTION	    CMD_CM_BASE + 7
	//��
#define CMD_CM_SYNATTR			CMD_CM_BASE + 8		// ͬ����ɫ���ԣ�ͬ�����Ե�����ֵ��
#define CMD_CM_SYNSKILLBAG		CMD_CM_BASE + 9		// ͬ��������
#define CMD_CM_DIE_RETURN		CMD_CM_BASE + 10	// ��������
#define CMD_CM_SKILLUPGRADE		CMD_CM_BASE + 11
#define CMD_CM_PING				CMD_CM_BASE + 15
#define CMD_CM_REFRESH_DATA		CMD_CM_BASE + 16
#define CMD_CM_CHECK_PING		CMD_CM_BASE + 17	// ����Ping
#define CMD_CM_MAP_MASK			CMD_CM_BASE + 18

#define CMD_CM_UPDATEHAIR		CMD_CM_BASE + 20	// �ͻ��������������
#define CMD_CM_TEAM_FIGHT_ASK	CMD_CM_BASE + 21	// ������ս����
#define CMD_CM_TEAM_FIGHT_ASR	CMD_CM_BASE + 22	// ������սӦ��(ANSWER)
#define CMD_CM_ITEM_REPAIR_ASK	CMD_CM_BASE + 23	// ������������
#define CMD_CM_ITEM_REPAIR_ASR	CMD_CM_BASE + 24	// ��������Ӧ��
#define CMD_CM_ITEM_FORGE_ASK	CMD_CM_BASE + 25	// ���߾�������
#define CMD_CM_ITEM_FORGE_ASR	CMD_CM_BASE + 26	// ���߾���Ӧ��
#define CMD_CM_TIGER_START		CMD_CM_BASE + 27	 // �����ϻ���
#define CMD_CM_TIGER_STOP		CMD_CM_BASE + 28	 // ֹͣ�ϻ���
//�ж�
#define CMD_CM_ITEM_FORGE_CANACTION CMD_CM_BASE +29   //��������������ɣ�����������¡�
//��������
#define CMD_CM_KITBAG_LOCK      CMD_CM_BASE + 31    // ������������
#define CMD_CM_KITBAG_UNLOCK    CMD_CM_BASE + 32    // ������������
#define CMD_CM_KITBAG_CHECK     CMD_CM_BASE + 33    // ��鱳������״̬
#define CMD_CM_KITBAG_AUTOLOCK	CMD_CM_BASE + 34	// �����Զ�����
#define CMD_CM_KITBAGTEMP_SYNC	CMD_CM_BASE + 35	// ͬ����ʱ����
#define CMD_CM_KITBAGTEMPlocks	CMD_CM_BASE + 36


//�̳�ϵͳ
#define CMD_CM_STORE_OPEN_ASK	CMD_CM_BASE + 41	// ���̳�
#define CMD_CM_STORE_LIST_ASK	CMD_CM_BASE + 42	// �̳��б�
#define CMD_CM_STORE_BUY_ASK	CMD_CM_BASE + 43	// �������
#define CMD_CM_STORE_CHANGE_ASK	CMD_CM_BASE + 44	// �һ�����
#define CMD_CM_STORE_QUERY		CMD_CM_BASE + 45	// ���׼�¼��ѯ
#define CMD_CM_STORE_VIP		CMD_CM_BASE + 46	// VIP
#define CMD_CM_STORE_AFFICHE	CMD_CM_BASE + 47	// ������Ʒ
#define CMD_CM_STORE_CLOSE		CMD_CM_BASE + 48

//��������
#define CMD_CM_BLACKMARKET_EXCHANGE_REQ	CMD_CM_BASE + 51 // ���жһ�
#define CMD_CM_CHEAT_CHECK		CMD_CM_BASE + 52

//���ִ�����
#define CMD_CM_VOLUNTER_LIST	CMD_CM_BASE + 61	// ��ʾ־Ը���б�
#define CMD_CM_VOLUNTER_ADD		CMD_CM_BASE + 62	// ���ӵ�־Ը���б�
#define CMD_CM_VOLUNTER_DEL		CMD_CM_BASE + 63	// ���б���ɾ��
#define CMD_CM_VOLUNTER_SEL		CMD_CM_BASE + 64	// �������
#define CMD_CM_VOLUNTER_OPEN	CMD_CM_BASE + 65	// ��־Ը�����
#define CMD_CM_VOLUNTER_ASR		CMD_CM_BASE + 66	// ־Ը�����Ӧ��

//��ʦ
#define CMD_CM_MASTER_INVITE	CMD_CM_BASE + 71	// ��ʦ
#define CMD_CM_MASTER_ASR		CMD_CM_BASE + 72
#define CMD_CM_MASTER_DEL		CMD_CM_BASE + 73
#define CMD_CM_PRENTICE_DEL		CMD_CM_BASE + 74
#define CMD_CM_PRENTICE_INVITE	CMD_CM_BASE + 75
#define CMD_CM_PRENTICE_ASR		CMD_CM_BASE + 76

//�����
#define CMD_CM_LIFESKILL_ASR	CMD_CM_BASE + 80	//��������ˣ��ϳɣ��ֽ�ȵ�����
#define CMD_CM_LIFESKILL_ASK	CMD_CM_BASE + 81	//��������ˣ��ϳɣ��ֽ�ȵ�ȷ��

#define CMD_CM_BIDUP			CMD_CM_BASE + 86
#define CMD_CM_STALLSEARCH		CMD_CM_BASE + 87


#define CMD_CM_SAY2CAMP			CMD_CM_BASE + 91
#define CMD_CM_GM_SEND			CMD_CM_BASE + 92
#define CMD_CM_GM_RECV			CMD_CM_BASE + 93
#define CMD_CM_PK_CTRL			CMD_CM_BASE + 94

// Add by lark.li 20080514 begin
#define CMD_CM_ITEM_LOTTERY_ASK	CMD_CM_BASE + 95	// ���߾�������
#define CMD_CM_ITEM_LOTTERY_ASR	CMD_CM_BASE + 96	// ��Ʊ�趨Ӧ��
#define CMD_CM_RANK				CMD_CM_BASE + 97
#define CMD_CM_DailyBuffRequest	CMD_CM_BASE + 110

// End

// Add by lark.li 20080707 begin
#define CMD_CM_CAPTAIN_CONFIRM_ASR	CMD_CM_BASE + 97	// �������ӳ�Ӧ��
// End

#define CMD_CM_ITEM_AMPHITHEATER_ASK	CMD_CM_BASE + 98 //ͶƱ����add by sunny.sun 20080726

#define	CMD_CM_ITEM_LOCK_ASK	CMD_CM_BASE	+	99
#define	CMD_CM_ITEM_UNLOCK_ASK	CMD_CM_BASE	+	100
#define	CMD_CM_GAME_REQUEST_PIN	CMD_CM_BASE	+	101

//Begin ���������
	#define CMD_CM_GUILD_PUTNAME	CMD_CM_GULDBASE	+ 1		//�ͻ������빫����
	#define CMD_CM_GUILD_TRYFOR		CMD_CM_GULDBASE	+ 2		//������빫��
	#define CMD_CM_GUILD_TRYFORCFM	CMD_CM_GULDBASE	+ 3		//ȷ���Ƿ񸲸���ǰ����
	#define CMD_CM_GUILD_LISTTRYPLAYER	CMD_CM_GULDBASE	+ 4	//�г����д��������

	#define CMD_CM_GUILD_APPROVE	CMD_CM_GULDBASE	+ 5	//������׼�����˼��롣
	#define CMD_CM_GUILD_REJECT		CMD_CM_GULDBASE	+ 6	//�����ܾ������˼��롣
	#define	CMD_CM_GUILD_KICK		CMD_CM_GULDBASE	+ 7
	#define CMD_CM_GUILD_LEAVE		CMD_CM_GULDBASE	+ 8
	#define CMD_CM_GUILD_DISBAND	CMD_CM_GULDBASE	+ 9
	#define CMD_CM_GUILD_MOTTO		CMD_CM_GULDBASE	+ 10
	#define CMD_CM_GUILD_CHALLENGE  CMD_CM_GULDBASE + 11 // ��ս����
	#define	CMD_CM_GUILD_LEIZHU		CMD_CM_GULDBASE + 12 // ������
	#define 	CMD_CM_GUILD_PERM			CMD_CM_GULDBASE + 13
//End ���������

//Begin ��ɫ�����͵�¼����CMD_CM_CHARBASE
	#define CMD_CM_LOGIN			CMD_CM_CHARBASE + 1		//�û��ʺŵ���(�û���/�������֤)
	#define CMD_CM_LOGOUT		    CMD_CM_CHARBASE + 2		//�û��ʺŵǳ�
	#define CMD_CM_BGNPLAY		    CMD_CM_CHARBASE + 3		//�û�ѡ���ɫ��ʼ����Ϸ
	#define CMD_CM_ENDPLAY			CMD_CM_CHARBASE + 4		//�û���������Ϸ����ѡ/��/ɾ��ɫ����
	#define CMD_CM_NEWCHA			CMD_CM_CHARBASE	+ 5		//�û��½���ɫ
	#define CMD_CM_DELCHA			CMD_CM_CHARBASE	+ 6		//�û�ɾ����ɫ
	#define CMD_CM_CANCELEXIT		CMD_CM_CHARBASE + 7		//�û�Ҫ��ȡ���˳�
	#define CMD_CM_REGISTER			CMD_CM_CHARBASE + 8		//�û�Ҫ��ȡ���˳�
//End ��ɫ�����͵�¼����

//Begin����ϵͳָ��
	#define CMD_CM_REQUESTNPC		CMD_CM_ROLEBASE + 1 // �ͻ��������NPC����
	#define CMD_CM_REQUESTTALK		CMD_CM_ROLEBASE + 1	// �ͻ���������NPC�Ի�����
	#define CMD_CM_TALKPAGE			CMD_CM_ROLEBASE + 2 // �ͻ���������NPC�Ի�ҳ����
	#define CMD_CM_FUNCITEM			CMD_CM_ROLEBASE + 3 // �ͻ�������ָ������ѡ�����

	#define CMD_CM_REQUESTTRADE		CMD_CM_ROLEBASE + 8 // �ͻ���������н���
	#define CMD_CM_TRADEITEM		CMD_CM_ROLEBASE + 9 // �ͻ�����������Ʒ
	#define CMD_CM_REQUESTAGENCY	CMD_CM_ROLEBASE + 10 // �ͻ�������npc��������

	//��ɫ����ָ��
	#define CMD_CM_CHARTRADE_REQUEST		CMD_CM_ROLEBASE + 12 // ����������
	#define CMD_CM_CHARTRADE_ACCEPT			CMD_CM_ROLEBASE + 13 // ���ܽ�������
	#define CMD_CM_CHARTRADE_REJECT			CMD_CM_ROLEBASE + 14 // �ܾ���������
	#define CMD_CM_CHARTRADE_CANCEL			CMD_CM_ROLEBASE + 15 // ȡ������
	#define CMD_CM_CHARTRADE_ITEM			CMD_CM_ROLEBASE + 16 // ��ɫ������Ʒ
	#define CMD_CM_CHARTRADE_VALIDATEDATA	CMD_CM_ROLEBASE + 17 // ȷ�Ͻ�������
	#define CMD_CM_CHARTRADE_VALIDATE		CMD_CM_ROLEBASE + 18 // ȷ�Ͻ���
	#define CMD_CM_CHARTRADE_MONEY			CMD_CM_ROLEBASE + 19 // �϶���Ǯ

	// ����
	#define CMD_CM_MISSION			CMD_CM_ROLEBASE + 22 // �ͻ�������������
	#define CMD_CM_MISSIONLIST		CMD_CM_ROLEBASE + 23 // �����б�
	#define CMD_CM_MISSIONTALK		CMD_CM_ROLEBASE + 24 // ����Ի�
	#define	CMD_CM_MISLOG			CMD_CM_ROLEBASE + 25 // ������־
	#define	CMD_CM_MISLOGINFO		CMD_CM_ROLEBASE + 26 // ����������ϸ��Ϣ
	#define CMD_CM_MISLOG_CLEAR		CMD_CM_ROLEBASE + 27 // ����ɾ��һ������
	
	// ��̯
	#define CMD_CM_STALL_ALLDATA	CMD_CM_ROLEBASE + 30 // �����̯������Ϣ
	#define CMD_CM_STALL_OPEN		CMD_CM_ROLEBASE + 31 // ����򿪰�̯������Ϣ
	#define CMD_CM_STALL_BUY		CMD_CM_ROLEBASE + 32 // �������̯����
	#define CMD_CM_STALL_CLOSE		CMD_CM_ROLEBASE + 33 // ������̯

	// ����
	#define CMD_CM_FORGE			CMD_CM_ROLEBASE + 35 // ��������Ʒ

    
	// ��ֻ
	#define CMD_CM_CREATE_BOAT		CMD_CM_ROLEBASE + 38 // ���󴴽���
	#define CMD_CM_UPDATEBOAT_PART	CMD_CM_ROLEBASE + 39 // ���������ֻ����
	#define CMD_CM_BOAT_CANCEL		CMD_CM_ROLEBASE + 40 // ����ȡ�����촬ֻ
	#define CMD_CM_BOAT_LUANCH		CMD_CM_ROLEBASE + 41 // ѡ��ֻ����
	#define CMD_CM_BOAT_BAGSEL		CMD_CM_ROLEBASE + 42 // ѡ��һ�Ҵ�ֻ���մ�����л���
	#define CMD_CM_BOAT_SELECT		CMD_CM_ROLEBASE + 43 // ѡ��ֻ
	#define CMD_CM_BOAT_GETINFO		CMD_CM_ROLEBASE + 44 //	��ȡ��ֻ��Ϣ

	// ʵ��
	#define CMD_CM_ENTITY_EVENT		CMD_CM_ROLEBASE + 45 // ����ʵ���¼�

	// ��������Э��
	#define CMD_CM_CREATE_PASSWORD2	CMD_CM_ROLEBASE + 46 // ������������
	#define CMD_CM_UPDATE_PASSWORD2 CMD_CM_ROLEBASE + 47 // ���¶������� 

    //����
    #define CMD_CM_READBOOK_START	CMD_CM_ROLEBASE + 48 // ��ʼ����
	#define CMD_CM_READBOOK_CLOSE	CMD_CM_ROLEBASE + 49 // ��������

#define CMD_CM_SEND_PRIVATE_KEY		CMD_CM_ROLEBASE + 55
#define CMD_CM_REQUEST_DROP_RATE	CMD_CM_ROLEBASE + 56
#define CMD_CM_REQUEST_EXP_RATE		CMD_CM_ROLEBASE + 57
#define CMD_CM_OFFLINE_MODE			CMD_CM_ROLEBASE + 58 
//End ����ϵͳָ��

//Begin�Ҷ�����������ϵͳ
#define CMD_CM_GARNER2_REORDER		CMD_CM_ROLEBASE + 71			//�ͻ���������������
//End�Ҷ�����������ϵͳ


#define CMD_CM_ANTIINDULGENCE       CMD_CM_ROLEBASE + 72   //  �������ƽ�


/*=====================================================================================================
*		Ga(m)eServer -> (C)lient
*/
#define CMD_MC_ROLEBASE			CMD_MC_BASE	+ 300	//(300-400)����ϵͳ�����ַ
#define CMD_MC_GULDBASE			CMD_MC_BASE	+ 400	//(400-430)���������ַ
#define CMD_MC_CHARBASE			CMD_MC_BASE	+ 430	//(430-450)��ɫ�����ַ

#define CMD_MC_SAY			    CMD_MC_BASE + 1		//��Ұ��˵��
#define CMD_MC_MAPCRASH			CMD_MC_BASE	+ 3
#define CMD_MC_CHABEGINSEE		CMD_MC_BASE + 4
#define CMD_MC_CHAENDSEE	    CMD_MC_BASE + 5
#define CMD_MC_ITEMBEGINSEE		CMD_MC_BASE + 6
#define CMD_MC_ITEMENDSEE	    CMD_MC_BASE + 7

#define CMD_MC_NOTIACTION		CMD_MC_BASE + 8
	//long	��ɫID
	//char	�ж����ͣ�1���ƶ���2��ʹ�ü��ܣ�3������Ӱ�죩
	//	����1���ƶ�
	//		char	״̬��0x00���ƶ��С�0x01������Ŀ���ֹͣ��0x02�����ϰ�ֹͣ��0x04���ͻ���Ҫ��ֹͣ��0x08������Ҫ��ķ�Χ(�繥����Χ)ֹͣ����
	//		Point	�����У�����player�ĵ�ǰλ�ã�
	//	����2��ʹ�ü���
	//		char	״̬��0x00��ʹ���С�0x01������������ֹͣ��0x02�������뿪ʹ�÷�Χ��ֹͣ��0x04���ﵽ��ʹ��Ŀ�Ķ�ֹͣ����������0x08���ͻ�Ҫ��ֹͣ����
	//		long	���ܱ�ţ�0����������������������ID��
	//		���ܲ��������ݼ��ܵ����ͣ����壬��Χ��ѡ�����в���
	//		long	Ŀ��ID
	//		Point	Ŀ��λ��
	//	����3������Ӱ��
	//		long	ʹ�÷��Ľ�ɫID
	//		long	ʹ�õļ���ID
	//		�������
	//		long	Ӱ�������ID
	//		long	Ӱ�����ֵ
	//	����4����װ
#define CMD_MC_SYNATTR			CMD_MC_BASE + 9		// ͬ����ɫ���ԣ�ͬ�����Ե�����ֵ��
#define CMD_MC_SYNSKILLBAG		CMD_MC_BASE + 10	// ͬ��������
#define CMD_MC_SYNASKILLSTATE	CMD_MC_BASE + 11	// ͬ����ɫ״̬
#define CMD_MC_PING				CMD_MC_BASE + 15
#define CMD_MC_ENTERMAP			CMD_MC_BASE	+ 16	// GameServer֪ͨClient�����ͼ��������ҵ�������Ϣ���丽���ṹ
#define CMD_MC_SYSINFO			CMD_MC_BASE	+ 17	// ϵͳ��Ϣ��ʾ�������Ҳ����ߣ��㻹û�����ʲô����ȵȡ�
#define CMD_MC_ALARM			CMD_MC_BASE	+ 18
#define CMD_MC_TEAM             CMD_MC_BASE + 19    // GameServer֪ͨ�ͻ���ĳ����Ա������ˢ��(�������, HP,SP)
#define CMD_MC_FAILEDACTION		CMD_MC_BASE + 20	// ������ж�ִ��ʧ��
#define CMD_MC_MESSAGE			CMD_MC_BASE + 21	// ��ʾ��Ϣ
#define CMD_MC_ASTATEBEGINSEE	CMD_MC_BASE + 22	// ����״̬
#define CMD_MC_ASTATEENDSEE	    CMD_MC_BASE + 23
#define CMD_MC_TLEADER_ID       CMD_MC_BASE + 24    // GameServer֪ͨ�ͻ���ĳ����Ա�Ķӳ���Ϣ
#define CMD_MC_CHA_EMOTION      CMD_MC_BASE + 25    // GameServer����ָ����ɫ������   

#define CMD_MC_QUERY_CHA        CMD_MC_BASE + 26    // ��ѯ��ɫ������Ϣ
#define CMD_MC_QUERY_CHAITEM    CMD_MC_BASE + 27    // ��ѯ��ɫ����
#define CMD_MC_CALL_CHA         CMD_MC_BASE + 28    // ����ɫ�ٻ����Լ�����
#define CMD_MC_GOTO_CHA         CMD_MC_BASE + 29    // ���Լ����͵���ɫ����
#define CMD_MC_KICK_CHA         CMD_MC_BASE + 30    // ����ɫ������
#define CMD_MC_SYNDEFAULTSKILL	CMD_MC_BASE + 31	// ͬ��Ĭ�ϼ���
#define CMD_MC_ADD_ITEM_CHA		CMD_MC_BASE + 32	// ���ӵ��߽�ɫ
#define CMD_MC_DEL_ITEM_CHA		CMD_MC_BASE + 33	// ɾ�����߽�ɫ
#define CMD_MC_QUERY_CHAPING    CMD_MC_BASE + 34    // ��ѯ��ɫ��GameServer�߼����pingֵ
#define CMD_MC_QUERY_RELIVE		CMD_MC_BASE + 35	// ��ѯ�Ƿ����ԭ�ظ���
#define CMD_MC_PREMOVE_TIME		CMD_MC_BASE + 36	// Ԥ�ƶ�ʱ��
#define CMD_MC_CHECK_PING		CMD_MC_BASE + 37	// ����Ping
#define CMD_MC_MAP_MASK			CMD_MC_BASE + 38

#define CMD_MC_OPENHAIR			CMD_MC_BASE + 39	// ����������
#define CMD_MC_UPDATEHAIR_RES	CMD_MC_BASE + 40    // �������͵Ľ��
#define CMD_MC_EVENT_INFO		CMD_MC_BASE + 41	// ͬ���¼���Ϣ
#define CMD_MC_SIDE_INFO		CMD_MC_BASE + 42	// ͬ���ֱ���Ϣ
#define CMD_MC_TEAM_FIGHT_ASK	CMD_MC_BASE + 43	// ������ս����
#define CMD_MC_ITEM_REPAIR_ASK	CMD_MC_BASE + 44	// ������������
#define CMD_MC_ITEM_REPAIR_ASR	CMD_MC_BASE + 45	// ��������Ӧ��
#define CMD_MC_BEGIN_ITEM_REPAIR	CMD_MC_BASE + 46	// ��ʼ��������
#define CMD_MC_APPEND_LOOK		CMD_MC_BASE + 47	// �������
#define CMD_MC_ITEM_FORGE_ASK	CMD_MC_BASE + 48	// ���߾�������
#define CMD_MC_ITEM_FORGE_ASR	CMD_MC_BASE + 49	// ���߾���Ӧ��
#define CMD_MC_ITEM_USE_SUC		CMD_MC_BASE + 50	// ����ʹ�óɹ�


#define CMD_MC_KITBAG_CAPACITY	CMD_MC_BASE + 51	// ��������
#define CMD_MC_ESPE_ITEM		CMD_MC_BASE + 52	// �������

//��������
#define CMD_MC_KITBAG_CHECK_ASR     CMD_MC_BASE + 53    // ��������״̬��Ӧ��
#define CMD_MC_KITBAGTEMP_SYNC		CMD_MC_BASE + 54	// ͬ����ʱ����

//�̳�ϵͳ
#define CMD_MC_STORE_OPEN_ASR	CMD_MC_BASE + 61	// ���̳�
#define CMD_MC_STORE_LIST_ASR	CMD_MC_BASE + 62	// �̳��б�
#define CMD_MC_STORE_BUY_ASR	CMD_MC_BASE + 63	// �������
#define CMD_MC_STORE_CHANGE_ASR	CMD_MC_BASE + 64	// �һ�����
#define CMD_MC_STORE_QUERY		CMD_MC_BASE + 65	// ���׼�¼��ѯ
#define CMD_MC_STORE_VIP		CMD_MC_BASE + 66	// VIP
#define CMD_MC_STORE_AFFICHE	CMD_MC_BASE + 67	// ������Ʒ
#define CMD_MC_POPUP_NOTICE		CMD_MC_BASE + 68

//���н���
#define CMD_MC_BLACKMARKET_EXCHANGEDATA		CMD_MC_BASE + 71	//���жһ�����
#define CMD_MC_BLACKMARKET_EXCHANGE_ASR		CMD_MC_BASE + 72	//���жһ�Ӧ��
#define CMD_MC_BLACKMARKET_EXCHANGEUPDATE	CMD_MC_BASE + 73	//���жһ�����
#define CMD_MC_BLACKMARKET_TRADEUPDATE		CMD_MC_BASE + 74	//���н��׸���
#define CMD_MC_EXCHANGEDATA					CMD_MC_BASE + 75	//��ͨ�һ�����

//���ִ�����
#define CMD_MC_VOLUNTER_LIST	CMD_MC_BASE + 81	// ��ʾ־Ը���б�
#define CMD_MC_VOLUNTER_STATE	CMD_MC_BASE + 82	// ־Ը��״̬
#define CMD_MC_VOLUNTER_SEL		CMD_MC_BASE + 83	// �������
#define CMD_MC_VOLUNTER_OPEN	CMD_MC_BASE + 84	// ��־Ը�����
#define CMD_MC_VOLUNTER_ASK		CMD_MC_BASE + 85	// ־Ը���������

#define CMD_MC_LISTAUCTION		CMD_MC_BASE + 86

#define CMD_MC_RANK				CMD_MC_BASE + 87
#define CMD_MC_STALLSEARCH		CMD_MC_BASE + 88


//��ʦ
#define CMD_MC_MASTER_ASK		CMD_MC_BASE + 91
#define CMD_MC_PRENTICE_ASK		CMD_MC_BASE + 92
#define	CMD_MC_CHAPLAYEFFECT	CMD_MC_BASE + 93

#define CMD_MC_SAY2CAMP			CMD_MC_BASE + 96
#define CMD_MC_GM_MAIL			CMD_MC_BASE + 97
#define CMD_MC_CHEAT_CHECK		CMD_MC_BASE + 98
#define	CMD_CM_ITEM_LOCK_ASR	CMD_MC_BASE	+ 99
#define	CMD_CM_ITEM_UNLOCK_ASR	CMD_MC_BASE	+ 100

#define	CMD_MC_ITEM_UNLOCK_ASR	CMD_MC_BASE	+ 95
#define CMD_MC_REQUEST_DROP_RATE	CMD_MC_BASE + 123
#define CMD_MC_REQUEST_EXP_RATE	CMD_MC_BASE + 124

//Begin ���������
	#define CMD_MC_GUILD_GETNAME	CMD_MC_GULDBASE	+ 1		//����ͻ������빫����
	#define CMD_MC_LISTGUILD		CMD_MC_GULDBASE	+ 2		//����ͻ��˴�������봰�ڲ������л��б�
	#define CMD_MC_GUILD_TRYFORCFM	CMD_MC_GULDBASE	+ 3		//ȷ���Ƿ񸲸���ǰ����
	#define CMD_MC_GUILD_LISTTRYPLAYER	CMD_MC_GULDBASE	+ 4	//�г����д��������
	#define CMD_MC_GUILD_LISTCHALL	CMD_MC_GULDBASE + 5		//��ѯ������ս��Ϣ
	#define CMD_MC_GUILD_MOTTO		CMD_MC_GULDBASE	+ 10	//�޸�������
	#define CMD_MC_GUILD_LEAVE		CMD_MC_GULDBASE + 11    //�뿪����
	#define CMD_MC_GUILD_KICK		CMD_MC_GULDBASE + 12	//���߳�����
	#define	CMD_MC_GUILD_INFO		CMD_MC_GULDBASE + 13	//ͬ��������Ϣ
//End ���������

//Begin ��ɫ�����͵�¼����CMD_MC_CHARBASE
	#define CMD_MC_LOGIN			CMD_MC_CHARBASE + 1
	#define CMD_MC_LOGOUT		    CMD_MC_CHARBASE + 2
	#define CMD_MC_BGNPLAY		    CMD_MC_CHARBASE + 3
	#define CMD_MC_ENDPLAY			CMD_MC_CHARBASE + 4
	#define CMD_MC_NEWCHA			CMD_MC_CHARBASE	+ 5
	#define CMD_MC_DELCHA			CMD_MC_CHARBASE	+ 6
	#define CMD_MC_STARTEXIT		CMD_MC_CHARBASE + 7		// ��ʼ�˳�����ʱ
	#define CMD_MC_CANCELEXIT		CMD_MC_CHARBASE + 8		// ȡ���˳�����ʱ
	#define CMD_MC_CREATE_PASSWORD2 CMD_MC_CHARBASE + 11	// 
	#define CMD_MC_UPDATE_PASSWORD2 CMD_MC_CHARBASE + 12	// 
	#define CMD_MC_SEND_SERVER_PUBLIC_KEY CMD_MC_CHARBASE + 13
	#define CMD_MC_SEND_HANDSHAKE		CMD_MC_CHARBASE + 14
//End ��ɫ�����͵�¼����

//Begin ����ϵͳָ��
	#define CMD_MC_TALKPAGE			CMD_MC_ROLEBASE + 1 // ��������ͻ��˷��ͶԻ���Ϣ
	#define CMD_MC_FUNCPAGE			CMD_MC_ROLEBASE + 2 // ��������ͻ��˷��͹���ѡ��ҳ��Ϣ
	#define CMD_MC_CLOSETALK		CMD_MC_ROLEBASE + 3 // ��������ͻ��˷��͹رնԻ���Ϣ
	#define CMD_MC_HELPINFO			CMD_MC_ROLEBASE + 4 // ��������ͻ��˷��Ͱ�����Ϣ
	#define CMD_MC_TRADEPAGE		CMD_MC_ROLEBASE + 5 // ��������ͻ��˷��ͽ�����Ϣ
	#define CMD_MC_TRADERESULT		CMD_MC_ROLEBASE + 6 // ��������ͻ��˷��ͽ����������Ϣ
	#define	CMD_MC_TRADE_DATA		CMD_MC_ROLEBASE + 7 // NPC����������Ϣ����
	#define	CMD_MC_TRADE_ALLDATA	CMD_MC_ROLEBASE + 8 // NPC����������Ϣ����
	#define CMD_MC_TRADE_UPDATE		CMD_MC_ROLEBASE + 9 // �������˽�����Ϣ����
	#define CMD_MC_EXCHANGE_UPDATE	CMD_MC_ROLEBASE + 10// �������˶һ���Ϣ����

	//��ɫ����ָ��
	#define CMD_MC_CHARTRADE				CMD_MC_ROLEBASE + 11 // ��������ͻ��˷��ͽ�����Ϣ 
	#define CMD_MC_CHARTRADE_REQUEST		CMD_MC_ROLEBASE + 12 // ��������ͻ��˷���������ɫ���뽻����Ϣ
	#define CMD_MC_CHARTRADE_CANCEL			CMD_MC_ROLEBASE + 13 // ��������ͻ��˷��� 
	#define CMD_MC_CHARTRADE_ITEM			CMD_MC_ROLEBASE + 14 // ��������ͻ��˽�ɫ���ͽ��׵���Ʒ��Ϣ
	#define CMD_MC_CHARTRADE_PAGE			CMD_MC_ROLEBASE + 15 // ��������ͻ��˷��Ϳ�ʼ���׽�����Ϣ
	#define CMD_MC_CHARTRADE_VALIDATEDATA	CMD_MC_ROLEBASE + 16 // ȷ�Ͻ�������
	#define CMD_MC_CHARTRADE_VALIDATE		CMD_MC_ROLEBASE + 17 // ȷ�Ͻ���
	#define CMD_MC_CHARTRADE_RESULT		   	CMD_MC_ROLEBASE + 18 // ȷ�Ͻ��׳ɹ�
	#define CMD_MC_CHARTRADE_MONEY			CMD_MC_ROLEBASE + 19 // �϶���Ǯ����
	#define CMD_MC_MISSION			CMD_MC_ROLEBASE + 22// ��������ͻ��˷��������б�ҳ��Ϣ
	#define CMD_MC_MISSIONLIST		CMD_MC_ROLEBASE + 23 // �����б�
	#define CMD_MC_MISSIONTALK		CMD_MC_ROLEBASE + 24 // ����Ի�
	#define CMD_MC_NPCSTATECHG		CMD_MC_ROLEBASE + 25 // �����л�npc״̬
	#define CMD_MC_TRIGGER_ACTION	CMD_MC_ROLEBASE + 26 // ������������
	#define CMD_MC_MISPAGE			CMD_MC_ROLEBASE + 27 // ����ҳ��Ϣ
	#define	CMD_MC_MISLOG			CMD_MC_ROLEBASE + 28 // ������־
	#define	CMD_MC_MISLOGINFO		CMD_MC_ROLEBASE + 29 // ����������ϸ��Ϣ
	#define	CMD_MC_MISLOG_CHANGE	CMD_MC_ROLEBASE + 30 // ����״̬�仯
	#define CMD_MC_MISLOG_CLEAR		CMD_MC_ROLEBASE + 31 // ���һ������
	#define CMD_MC_MISLOG_ADD		CMD_MC_ROLEBASE + 32 // ����һ������

	#define CMD_MC_BEGIN_ITEM_FUSION	CMD_MC_ROLEBASE + 33	 // ��ʼ�ۺϵ���
	#define CMD_MC_BEGIN_ITEM_UPGRADE	CMD_MC_ROLEBASE + 34	 // ��ʼ��������
	#define CMD_MC_BEGIN_ITEM_FORGE		CMD_MC_ROLEBASE + 35	 // ��ʼ���߾���
	#define CMD_MC_BEGIN_ITEM_UNITE		CMD_MC_ROLEBASE + 36	 // ��ʼ���ߺϳ�
	#define CMD_MC_BEGIN_ITEM_MILLING	CMD_MC_ROLEBASE + 37	 // ��ʼ���ߴ�ĥ

	#define CMD_MC_CREATEBOAT		CMD_MC_ROLEBASE + 38 // ������ֻ
	#define CMD_MC_UPDATEBOAT		CMD_MC_ROLEBASE + 39 // ���촬ֻ
	#define CMD_MC_UPDATEBOAT_PART	CMD_MC_ROLEBASE + 40 // ���촬ֻ�Ĳ���
	#define CMD_MC_BERTH_LIST		CMD_MC_ROLEBASE + 41 // ��ֻͣ���б�
	#define CMD_MC_BOAT_LIST		CMD_MC_ROLEBASE + 42 // ͬ����ɫӵ�е����д�ֻ����
	#define CMD_MC_BOAT_ADD			CMD_MC_ROLEBASE + 43 // ����һ�Ҵ�
	#define CMD_MC_BOAT_CLEAR		CMD_MC_ROLEBASE + 44 // ���һ�Ҵ�
	#define CMD_MC_BOATINFO			CMD_MC_ROLEBASE + 45 // ͬ����ֻ������Ϣ
	#define CMD_MC_BOAT_BAGLIST		CMD_MC_ROLEBASE + 46 // ��ʾ���Դ���Ĵ�ֻ�б�

	#define CMD_MC_BEGIN_ITEM_EIDOLON_METEMPSYCHOSIS CMD_MC_ROLEBASE + 47 // ��ʼ����ת��
	#define CMD_MC_BEGIN_ITEM_EIDOLON_FUSION CMD_MC_ROLEBASE + 48 // ��ʼ�����ں�
	#define CMD_MC_BEGIN_ITEM_PURIFY 	CMD_MC_ROLEBASE + 49	 // ��ʼװ���ᴿ
	
	// �¼�ʵ�彻��Э������
	#define CMD_MC_ENTITY_BEGINESEE	CMD_MC_ROLEBASE + 50 // ͬ����Ұʵ����Ϣ
	#define CMD_MC_ENTITY_ENDSEE	CMD_MC_ROLEBASE + 51 // ʵ����Ϣ������Ұ
	#define CMD_MC_ENTITY_CHGSTATE	CMD_MC_ROLEBASE + 52 // ʵ��״̬�ı�

	// ��̯
	#define	CMD_MC_STALL_ALLDATA	CMD_MC_ROLEBASE + 54 // �鿴��̯������Ϣ
	#define CMD_MC_STALL_UPDATE		CMD_MC_ROLEBASE + 55 // ��̯���������Ϣ
	#define CMD_MC_STALL_DELGOODS	CMD_MC_ROLEBASE + 56 // ɾ��ָ��λ�õĻ���
	#define CMD_MC_STALL_CLOSE		CMD_MC_ROLEBASE + 57 // ̯����̯
	#define CMD_MC_STALL_START		CMD_MC_ROLEBASE + 58 // �ɹ���̯
	#define CMD_MC_STALL_NAME		CMD_MC_ROLEBASE + 59 // ��̯��

	// ����������Ϣ
	#define CMD_MC_BICKER_NOTICE	CMD_MC_ROLEBASE + 60 // ����������Ϣ֪ͨ

	#define CMD_MC_BEGIN_ITEM_ENERGY	CMD_MC_ROLEBASE + 71	 // ��ʼ���ǳ��
	#define CMD_MC_BEGIN_GET_STONE		CMD_MC_ROLEBASE + 72	 // ��ʼ��ȡ��ʯ
	#define CMD_MC_BEGIN_TIGER			CMD_MC_ROLEBASE + 73	 // �����ϻ�������
	#define CMD_MC_TIGER_ITEM_ID		CMD_MC_ROLEBASE + 74	 // �ϻ�������ID
	#define CMD_MC_TIGER_STOP			CMD_MC_ROLEBASE + 75	 // �ϻ�������
	#define CMD_MC_BEGIN_ITEM_FIX		CMD_MC_ROLEBASE + 76	 // �����ƹ�
	#define CMD_MC_BEGIN_GM_SEND		CMD_MC_ROLEBASE + 77	 // ����GM�ʼ�
	#define CMD_MC_BEGIN_GM_RECV		CMD_MC_ROLEBASE + 78	 // ����GM�ʼ�

	// Add by lark.li 20080514 begin
	#define CMD_MC_BEGIN_ITEM_LOTTERY		CMD_MC_ROLEBASE + 79	 // ��ʼ���ò�Ʊ
	// End
	#define CMD_MC_BEGIN_ITEM_AMPHITHEATER		CMD_MC_ROLEBASE + 80	 //�򿪾��������а�add by sunny.sun20080716	

	#define CMD_MC_COLOUR_NOTICE	CMD_MC_ROLEBASE + 92
	#define CMD_MC_UPDATEGUILDBANKGOLD	CMD_MC_ROLEBASE + 93

//�����
#define CMD_MC_LIFESKILL_BGING  CMD_MC_BASE + 100	//����ܿ�ʼ
#define CMD_MC_LIFESKILL_ASR	CMD_MC_BASE + 101	//��������ˣ��ϳɣ��ֽ�ȵ�����
#define CMD_MC_LIFESKILL_ASK	CMD_MC_BASE + 102	//��������ˣ��ϳɣ��ֽ�ȵ�ȷ��

// Add by lark.li 20080515 begin
#define CMD_MC_ITEM_LOTTERY_ASK	CMD_MC_BASE + 103	// ��Ʊ����
#define CMD_MC_ITEM_LOTTERY_ASR	CMD_MC_BASE + 104	// ��ƱӦ��
// End

// Add by lark.li 20080707 begin
#define CMD_MC_CAPTAIN_ASK	CMD_MC_BASE + 105	// �������ӳ�ȷ������
// End

#define CMD_MC_ITEM_AMPHITHEATER_ASR  CMD_MC_BASE + 106//Add by sunny.sun 20080726

//End ����ϵͳָ��


#define CMD_MC_UPDATEIMP  CMD_MC_BASE + 111

#define CMD_MC_GUILDNOTICE  CMD_MC_BASE + 112

#define CMD_MC_REQUESTPIN  CMD_MC_BASE + 113

#define CMD_MC_RecDailyBuffInfo  CMD_MC_BASE + 115

/*=====================================================================================================
*					Ga(t)eServer <->Ga(m)eServer
*/
#define CMD_TM_ENTERMAP			CMD_TM_BASE	+ 3		//����ͼ��������ɫID�������ͼ�����֡�λ��(x,y)
#define CMD_TM_GOOUTMAP			CMD_TM_BASE + 4		//����ͼ
#define CMD_TM_LOGIN_ACK		CMD_TM_BASE + 5
#define CMD_TM_CHANGE_PERSONINFO	CMD_TM_BASE	+6	//�޸ĸ�����Ϣ
#define CMD_TM_MAPENTRY			CMD_TM_BASE + 7
#define CMD_TM_MAPENTRY_NOMAP	CMD_TM_BASE + 8
#define CMD_TM_KICKCHA			CMD_TM_BASE + 9
#define CMD_TM_OFFLINE_MODE		CMD_TM_BASE + 11

//---------------------------------------------------

#define CMD_MT_LOGIN			CMD_MT_BASE + 1		//GameServer��½GateServer

#define CMD_MT_SWITCHMAP		CMD_MT_BASE + 4		//��ʼ�л���ͼ����

#define CMD_MT_KICKUSER			CMD_MT_BASE + 5		//GameServer�ߵ�ĳ���û�
#define CMD_MT_MAPENTRY			CMD_MT_BASE + 6		//��ͼ���

#define CMD_MT_PALYEREXIT		CMD_MT_BASE + 8		//֪ͨGate�ͷŸ��û���Դ

/*=====================================================================================================
*					Grou(p)eServer <->Ga(m)eServer 
*/
#define CMD_PM_TEAM             CMD_PM_BASE + 1     // GroupServer�����������Ϣ����
//CMD_PM_TEAM�����Ͷ���Begin
	#define TEAM_MSG_ADD            1                   // ������: �����¶�Ա
	#define TEAM_MSG_LEAVE          2                   // ������: ��Ա�뿪
	#define TEAM_MSG_UPDATE         3                   // ������: ˢ�¶���,ֻ�÷���������תʱ��ˢ�µ�GameServerʹ�á�
	
	#define TEAM_MSG_GROUP_ADD      4                   // ��������
	#define TEAM_MSG_GROUP_BREAK    5                   // ��ɢ����

	#define TEAM_MSG_KICK			6					// �߳�����ĳ����Ա
#define CMD_PM_GUILD_DISBAND	CMD_PM_BASE	+ 2		//��ɢ����
//CMD_PM_TEAM�����Ͷ���End

#define CMD_PM_GUILDINFO		CMD_PM_BASE + 4		// GroupServer�����Ĺ���ͬ����Ϣ
#define CMD_PM_GUILD_CHALLMONEY CMD_PM_BASE + 5     // ������սͶ���Ǯ�˸��᳤
#define CMD_PM_GUILD_CHALL_PRIZEMONEY CMD_PM_BASE + 6     // ������սս������Ǯ���᳤
//Begin�Ҷ�����������ϵͳ
#define CMD_PM_GARNER2_UPDATE	CMD_PM_BASE +7
//End�Ҷ�����������ϵͳ
#define CMD_PM_TEAM_CREATE		CMD_PM_BASE + 8
#define CMD_PM_SAY2ALL			CMD_PM_BASE + 9
#define CMD_PM_SAY2TRADE		CMD_PM_BASE + 10
#define CMD_PM_EXPSCALE         CMD_PM_BASE + 11        //  ������ϵͳ, Ӱ����Ҿ��鼰������
#define CMD_PM_GUILDBANK		CMD_PM_BASE + 15
#define CMD_PM_PUSHTOGUILDBANK		CMD_PM_BASE + 18

//---------------------------------------------------
#define CMD_MP_ENTERMAP			CMD_MP_BASE	+ 1
#define CMD_MP_GUILD_APPROVE	CMD_MP_BASE	+ 2
#define CMD_MP_GUILD_CREATE		CMD_MP_BASE	+ 3
#define CMD_MP_GUILD_KICK		CMD_MP_BASE	+ 4
#define CMD_MP_GUILD_LEAVE		CMD_MP_BASE	+ 5
#define CMD_MP_GUILD_DISBAND	CMD_MP_BASE	+ 6
#define CMD_MP_GUILD_MOTTO		CMD_MP_BASE	+ 10
#define CMD_MP_GUILD_CHALLMONEY CMD_MP_BASE + 13			//��Ͷ�깫���Ǯ�˻��᳤��ɫ
#define CMD_MP_GUILD_CHALL_PRIZEMONEY	CMD_MP_BASE + 14    //������ս�����ĵ�Ǯ���᳤��ɫ

//Begin�Ҷ�����������ϵͳ
#define CMD_MP_GARNER2_UPDATE	CMD_MP_BASE +7
#define CMD_MP_GARNER2_CGETORDER CMD_MP_BASE+15
//End�Ҷ�����������ϵͳ

#define CMD_MP_TEAM_CREATE		CMD_MP_BASE + 16

#define CMD_MP_MASTER_CREATE	CMD_MP_BASE + 17
#define CMD_MP_MASTER_DEL		CMD_MP_BASE + 18
#define CMD_MP_MASTER_FINISH	CMD_MP_BASE + 19
#define CMD_MP_SAY2ALL			CMD_MP_BASE + 20
#define CMD_MP_SAY2TRADE		CMD_MP_BASE + 21
#define CMD_MP_GM1SAY1			CMD_MP_BASE + 22//add by sunny.sun20080805
#define CMD_MP_GM1SAY			CMD_MP_BASE + 23//add by sunny.sun20080821
#define CMD_MP_GMBANACCOUNT		CMD_MP_BASE	+ 24
#define CMD_MP_GMUNBANACCOUNT	CMD_MP_BASE	+ 25
#define CMD_MP_GUILD_PERM		CMD_MP_BASE + 27
#define CMD_MP_GUILDBANK		CMD_MP_BASE + 28
#define CMD_MP_PUSHTOGUILDBANK	CMD_MP_BASE + 31
#define CMD_MP_GUILDNOTICE		CMD_MP_BASE + 32
#define CMD_MP_CANRECEIVEREQUESTS	CMD_MP_BASE + 33
#define CMD_MP_MUTE_PLAYER		CMD_MP_BASE + 34

/*=====================================================================================================
*					Ga(t)eServer <->Grou(p)Server 
*/
#define	CMD_TP_LOGIN			CMD_TP_BASE + 1		//GateServer��GroupServer���̼��½
#define CMD_TP_LOGOUT			CMD_TP_BASE + 2		//GateServer��GroupServer���̼�ǳ�
#define CMD_TP_USER_LOGIN		CMD_TP_BASE	+ 3		//�û�����
#define CMD_TP_USER_LOGOUT		CMD_TP_BASE + 4		//�û��ǳ�
#define CMD_TP_BGNPLAY			CMD_TP_BASE	+ 5		//�û�ѡ���ɫ��ʼ����Ϸ
#define CMD_TP_ENDPLAY			CMD_TP_BASE	+ 6		//�û���������Ϸ����ѡ/��/ɾ��ɫ����
#define CMD_TP_NEWCHA			CMD_TP_BASE	+ 7		//�½���ɫ
#define CMD_TP_DELCHA			CMD_TP_BASE	+ 8		//ɾ����ɫ
#define CMD_TP_PLAYEREXIT		CMD_TP_BASE + 9		//�˳���ɫ�ͷ���Դ
#define CMD_TP_REQPLYLST		CMD_TP_BASE	+ 10	//����GateServer�ϵ���ҵ�ַ�б�
#define CMD_TP_DISC				CMD_TP_BASE + 11
#define CMD_TP_ESTOPUSER_CHECK	CMD_TP_BASE + 12
#define CMD_TP_CREATE_PASSWORD2	CMD_TP_BASE + 13	// ������������
#define CMD_TP_UPDATE_PASSWORD2 CMD_TP_BASE + 14	// ���¶�������

#define CMD_TP_SYNC_PLYLST		CMD_TP_BASE + 15	// ���¶�������
#define CMD_TP_REGISTER			CMD_TP_BASE + 16		//�û��ǳ�
#define CMD_TP_CHANGEPASS		CMD_TP_BASE + 17

//---------------------------------------------------
#define CMD_PT_KICKUSER			CMD_PT_BASE	+ 11
#define CMD_PT_ESTOPUSER		CMD_PT_BASE + 12	// ���Խ�ɫ
#define CMD_PT_DEL_ESTOPUSER	CMD_PT_BASE + 13	// �������
#define CMD_PT_REGISTER			CMD_PT_BASE + 15
#define CMD_PT_KICKPLAYINGPLAYER CMD_PT_BASE + 16
/*=====================================================================================================
*					Grou(p)Server <-> (A)ccountServer
*/
#define CMD_PA_LOGIN            CMD_PA_BASE + 2
#define CMD_PA_USER_LOGIN       CMD_PA_BASE + 4
#define CMD_PA_USER_LOGOUT      CMD_PA_BASE + 5
#define CMD_PA_USER_DISABLE		CMD_PA_BASE + 6
#define CMD_PA_USER_LOGIN1		CMD_PA_BASE + 13
#define CMD_PA_USER_LOGIN2		CMD_PA_BASE + 14
#define CMD_PA_USER_BILLBGN		CMD_PA_BASE + 20
#define CMD_PA_USER_BILLEND		CMD_PA_BASE + 21
#define CMD_PA_GROUP_BILLEND_AND_LOGOUT		CMD_PA_BASE + 22
#define CMD_PA_LOGOUT			CMD_PA_BASE + 23
#define CMD_PA_GMBANACCOUNT		CMD_PA_BASE + 24
#define CMD_PA_GMUNBANACCOUNT		CMD_PA_BASE + 25
#define CMD_PA_REGISTER			CMD_PA_BASE + 26
#define CMD_PA_CHANGEPASS			CMD_PA_BASE + 27

//---------------------------------------------------
#define CMD_AP_LOGIN	        CMD_AP_BASE + 2
#define CMD_AP_USER_LOGIN		CMD_AP_BASE + 3
#define CMD_AP_RELOGIN			CMD_AP_BASE + 4
#define CMD_AP_KICKUSER			CMD_AP_BASE	+ 11
#define CMD_AP_USER_LOGIN1		CMD_AP_BASE + 12
#define CMD_AP_USER_LOGIN2		CMD_AP_BASE + 13
#define CMD_AP_EXPSCALE         CMD_AP_BASE + 14    //  ������
#define CMD_AP_REGISTER			CMD_AP_BASE + 15
/*=====================================================================================================
*					Ga(m)eServer <-> Ga(m)eServer
*
*/
#define CMD_MM_GATE_RELEASE		CMD_MM_BASE + 1
#define CMD_MM_GATE_CONNECT     CMD_MM_BASE + 2
#define CMD_MM_QUERY_CHA		CMD_MM_BASE + 3		// ��ѯ��ɫ������Ϣ
#define CMD_MM_QUERY_CHAITEM	CMD_MM_BASE + 4		// ��ѯ��ɫ������Ϣ
#define CMD_MM_CALL_CHA			CMD_MM_BASE + 5		// ����ɫ�ٻ����Լ�����
#define CMD_MM_GOTO_CHA		    CMD_MM_BASE + 6		// ���Լ����͵���ɫ����
#define CMD_MM_KICK_CHA			CMD_MM_BASE + 7		// ����ɫ������
#define CMD_MM_GUILD_REJECT		CMD_MM_BASE + 8		// �ܾ����빫������֪ͨ����
#define CMD_MM_GUILD_APPROVE	CMD_MM_BASE + 9		// �ܾ����빫������֪ͨ����
#define CMD_MM_GUILD_KICK		CMD_MM_BASE	+ 10	// �ӹ��������޳�ĳ��
#define CMD_MM_GUILD_DISBAND	CMD_MM_BASE	+ 11	// ��ɢ����
#define CMD_MM_QUERY_CHAPING	CMD_MM_BASE + 12	// ��ѯ��ɫ��GameServer�߼����pingֵ
#define CMD_MM_NOTICE			CMD_MM_BASE + 13	// ϵͳͨ��
#define CMD_MM_GUILD_MOTTO		CMD_MM_BASE + 14	// ���¹���������
#define CMD_MM_DO_STRING		CMD_MM_BASE + 15	// ִ�нű�
#define CMD_MM_CHA_NOTICE		CMD_MM_BASE + 16	// ��ָ����ҵ�ϵͳͨ��
#define CMD_MM_LOGIN			CMD_MM_BASE + 17	// ��ҵ�½��Ϸ
#define CMD_MM_GUILD_CHALL_PRIZEMONEY			CMD_MM_BASE + 18	// �˻�����ս�ķ���
#define CMD_MM_ADDCREDIT		CMD_MM_BASE + 19	// ��������
#define CMD_MM_STORE_BUY		CMD_MM_BASE + 20	// �����̳���Ʒ
#define CMD_MM_ADDMONEY			CMD_MM_BASE + 21
#define CMD_MM_AUCTION			CMD_MM_BASE + 22
#define CMD_MM_UPDATEGUILDBANK			CMD_MM_BASE + 23
#define CMD_MM_UPDATEGUILDBANKGOLD		CMD_MM_BASE + 24

/*=====================================================================================================
*					Grou(p)Server <-> (C)lient
*/
//��������BEGIN
	#define	CMD_CP_TEAM_INVITE		CMD_CP_BASE	+ 1		//�������
	#define	CMD_CP_TEAM_ACCEPT		CMD_CP_BASE	+ 2		//ȷ�����
	#define	CMD_CP_TEAM_REFUSE		CMD_CP_BASE	+ 3		//�ܾ����
	#define	CMD_CP_TEAM_LEAVE		CMD_CP_BASE	+ 4		//�뿪���
	#define CMD_CP_TEAM_KICK		CMD_CP_BASE + 5		//�߳����
//��������END
//���������BEGIN
	#define CMD_CP_FRND_INVITE		CMD_CP_BASE	+ 11	//��������
	#define CMD_CP_FRND_ACCEPT		CMD_CP_BASE	+ 12	//���ѽ���
	#define CMD_CP_FRND_REFUSE		CMD_CP_BASE	+ 13	//���Ѿܾ�
	#define CMD_CP_FRND_DELETE		CMD_CP_BASE	+ 14	//����ɾ��
	#define CMD_CP_FRND_CHANGE_GROUP	CMD_CP_BASE	+ 15	//�ı������������
	#define CMD_CP_FRND_REFRESH_INFO	CMD_CP_BASE	+ 16	//���Լ��Ŀͻ���ˢ�±���(���ѡ����ѡ�ͬ�š�·�ˡ�İ���ˣ��ĸ�����Ϣ��
	#define CMD_CP_CHANGE_PERSONINFO	CMD_CP_BASE	+ 17	//�ı�����Լ���ͷ��ȸ�����Ϣ

// Add by lark.li 20080804 begin
	#define CMD_CP_FRND_DEL_GROUP	CMD_CP_BASE	+ 18
	#define CMD_CP_FRND_ADD_GROUP	CMD_CP_BASE	+ 19
	#define CMD_CP_FRND_MOVE_GROUP	CMD_CP_BASE	+ 20
// End

// Add by lark.li 20080808 begin
	#define CMD_CP_QUERY_PERSONINFO	CMD_CP_BASE	+ 21
// End

//���������END

#define CMD_CP_PING				CMD_CP_BASE	+ 22

// ���
#define CMD_CP_REPORT_WG		CMD_CP_BASE + 23 // ���淢��ʹ�����

//��ʦ�����BEGIN
#define CMD_CP_MASTER_REFRESH_INFO		CMD_CP_BASE	+ 31	//���Լ��Ŀͻ���ˢ��ʦ���ĸ�����Ϣ
#define CMD_CP_PRENTICE_REFRESH_INFO	CMD_CP_BASE	+ 32	//���Լ��Ŀͻ���ˢ��ͽ�ܵĸ�����Ϣ

//��ʦ�����END

//���������Begin

//���������End
//Begin ����ָ��
	#define CMD_CP_GM1SAY			CMD_CP_BASE	+ 400		//GM��ȫ���㲥
	#define CMD_CP_SAY2TRADE		CMD_CP_BASE	+ 401		//����Ƶ��
	#define CMD_CP_SAY2ALL		    CMD_CP_BASE + 402		//������Ϸ����㲥
	#define CMD_CP_SAY2YOU			CMD_CP_BASE	+ 403		//P2P����(��Ҷ����)
	#define CMD_CP_SAY2TEM			CMD_CP_BASE	+ 404		//�������
	#define CMD_CP_SAY2GUD			CMD_CP_BASE	+ 405		//�л�(Guild)����

	#define CMD_CP_SESS_CREATE		CMD_CP_BASE	+ 406		//�Ự����
	#define CMD_CP_SESS_SAY			CMD_CP_BASE	+ 407		//�Ự˵��
	#define CMD_CP_SESS_ADD			CMD_CP_BASE	+ 408		//�Ự������Ա
	#define CMD_CP_SESS_LEAVE		CMD_CP_BASE	+ 409		//�뿪�Ự
	#define	CMD_CP_REFUSETOME		CMD_CP_BASE	+ 410		//�򿪽���˽�Ŀ���
	#define CMD_CP_GM1SAY1			CMD_CP_BASE	+ 411		//�����㲥 Add by sunny.sun20080804
	#define CMD_CP_GUILDBANK		CMD_CP_BASE + 414
	#define CMD_CP_CHANGEPASS		CMD_CP_BASE + 415
//End ����ָ��
//Begin�Ҷ�����������ϵͳ
//End�Ҷ�����������ϵͳ
//---------------------------------------------------
//��������BEGIN
	#define	CMD_PC_TEAM_INVITE		CMD_PC_BASE	+ 1			//֪ͨ�ͻ��������������
	#define CMD_PC_TEAM_REFRESH		CMD_PC_BASE	+ 2			//֪ͨ�ͻ�����ӱ仯����Ϣ��ʹ��CMD_PM_TEAM�����������
	#define	CMD_PC_TEAM_CANCEL		CMD_PC_BASE	+ 3			//��������Ϊ��ʱ���Ա��������ԭ��֪ͨ�����뷽ȡ����������
	//CMD_PC_TEAM_CANCEL�����Ͷ���Begin
		#define MSG_TEAM_CANCLE_BUSY		1	//�Լ���������з�æ
		#define MSG_TEAM_CANCLE_TIMEOUT		2	//�������볬ʱ
		#define MSG_TEAM_CANCLE_OFFLINE		3	//���뷽������
		#define MSG_TEAM_CANCLE_ISFULL		4	//���뷽�����Ա������
		#define MSG_TEAM_CANCLE_CANCEL		5	//�����뷽������/��������ӻ����뷽�����˱��˴�������Ӷ�ȡ���˱�������
	//CMD_PC_TEAM_CANCEL�����Ͷ���End
//��������END
//���������BEGIN
	#define CMD_PC_FRND_INVITE		CMD_PC_BASE	+ 11
	#define CMD_PC_FRND_REFRESH		CMD_PC_BASE	+ 12
	//CMD_PC_FRND_REFRESH�����Ͷ���Begin
		#define MSG_FRND_REFRESH_START          1   // ������: ������ʱ����ĺ����б�
		#define MSG_FRND_REFRESH_ADD            2   // ������: ��������
		#define MSG_FRND_REFRESH_DEL            3   // ������: ����ɾ��
		#define MSG_FRND_REFRESH_ONLINE	        4	// ������: ��������
		#define MSG_FRND_REFRESH_OFFLINE        5   // ������: ��������
	//CMD_PC_FRND_REFRESH�����Ͷ���End
	#define CMD_PC_FRND_CANCEL		CMD_PC_BASE	+ 13
	//CMD_PC_FRND_CANCEL�����Ͷ���Begin
		#define MSG_FRND_CANCLE_BUSY			1	//�Լ���������з�æ
		#define MSG_FRND_CANCLE_TIMEOUT			2	//�������볬ʱ
		#define MSG_FRND_CANCLE_OFFLINE			3	//���뷽������
		#define MSG_FRND_CANCLE_INVITER_ISFULL	4	//���뷽�ĺ��ѳ�Ա������
		#define MSG_FRND_CANCLE_SELF_ISFULL		5	//�Լ��ĺ���������
		#define MSG_FRND_CANCLE_CANCEL			6	//���뷽ȡ���˱�������
	//CMD_PC_FRND_CANCEL�����Ͷ���End
	#define	CMD_PC_FRND_CHANGE_GROUP	CMD_PC_BASE + 15	//�ı���ѷ���ɹ�
	#define	CMD_PC_FRND_REFRESH_INFO	CMD_PC_BASE	+ 16	//ˢ�º��Ѹ�����Ϣ
	#define CMD_PC_CHANGE_PERSONINFO	CMD_PC_BASE	+ 17	//�ı�����Լ���ͷ��ȸ�����Ϣ

// Add by lark.li 20080804 begin
	#define	CMD_PC_FRND_DEL_GROUP		CMD_PC_BASE + 18	//ɾ�����ѷ���ɹ�
	#define	CMD_PC_FRND_ADD_GROUP		CMD_PC_BASE + 19	//�������ѷ���ɹ�
	#define	CMD_PC_FRND_MOVE_GROUP	CMD_PC_BASE + 20	//�ı���ѷ���ɹ�
// End

// Add by lark.li 20080808 begin
#define CMD_PC_QUERY_PERSONINFO	CMD_PC_BASE	+ 21	//�ı�����Լ���ͷ��ȸ�����Ϣ
// End
	//
//���������END

#define CMD_PC_PING				CMD_PC_BASE	+ 22

//���������Begin
	#define CMD_PC_GUILD				CMD_PC_BASE	+ 30
	//CMD_PC_GUILD�����Ͷ���Begin
		#define MSG_GUILD_START					1
		#define MSG_GUILD_ADD					2
		#define MSG_GUILD_DEL					3
		#define MSG_GUILD_ONLINE				4
		#define MSG_GUILD_OFFLINE				5
		#define MSG_GUILD_STOP					6
#define CMD_PC_GUILD_PERM				CMD_PC_BASE	+ 31
#define CMD_PC_GM_INFO					CMD_PC_BASE	+ 32

	//CMD_PC_GUILD�����Ͷ���End
//���������End

//��ʦ�����BEGIN
#define CMD_PC_MASTER_REFRESH		CMD_PC_BASE	+ 41
	//CMD_PC_MASTER_REFRESH�����Ͷ���Begin
	#define MSG_MASTER_REFRESH_START          1   // ������: ������ʱ�����ʦ���б�
	#define MSG_MASTER_REFRESH_ADD            2   // ������: ʦ������
	#define MSG_MASTER_REFRESH_DEL            3   // ������: ʦ��ɾ��
	#define MSG_MASTER_REFRESH_ONLINE	      4	  // ������: ʦ������
	#define MSG_MASTER_REFRESH_OFFLINE        5   // ������: ʦ������
	#define MSG_PRENTICE_REFRESH_START        6   // ������: ������ʱ�����ͽ���б�
	#define MSG_PRENTICE_REFRESH_ADD          7   // ������: ͽ������
	#define MSG_PRENTICE_REFRESH_DEL          8   // ������: ͽ��ɾ��
	#define MSG_PRENTICE_REFRESH_ONLINE	      9   // ������: ͽ������
	#define MSG_PRENTICE_REFRESH_OFFLINE     10   // ������: ͽ������
	//CMD_PC_MASTER_REFRESH�����Ͷ���End
#define CMD_PC_MASTER_CANCEL		CMD_PC_BASE	+ 42
	//CMD_PC_MASTER_CANCEL�����Ͷ���Begin
	#define MSG_MASTER_CANCLE_BUSY				1	//�Լ��İ�ʦ���з�æ
	#define MSG_MASTER_CANCLE_TIMEOUT			2	//���ΰ�ʦ��ʱ
	#define MSG_MASTER_CANCLE_OFFLINE			3	//�Է�������
	#define MSG_MASTER_CANCLE_INVITER_ISFULL	4	//�Է���ͽ��������
	#define MSG_MASTER_CANCLE_SELF_ISFULL		5	//�Լ���ʦ��������
	#define MSG_MASTER_CANCLE_CANCEL			6	//�Է�ȡ���˱��ΰ�ʦ
	//CMD_PC_MASTER_CANCEL�����Ͷ���End
#define	CMD_PC_MASTER_REFRESH_INFO		CMD_PC_BASE	+ 43	//ˢ��ʦ��������Ϣ
#define CMD_PC_PRENTICE_REFRESH_INFO	CMD_PC_BASE	+ 44	//ˢ��ͽ�ܸ�����Ϣ
#define CMD_PC_REGISTER	CMD_PC_BASE	+ 45	//ˢ��ͽ�ܸ�����Ϣ

//
//��ʦ�����END

//Begin ����ָ��
#define CMD_PC_GM1SAY			CMD_PC_BASE	+ 400		//GM��ȫ���㲥
#define CMD_PC_SAY2TRADE		CMD_PC_BASE	+ 401		//����Ƶ��
#define CMD_PC_SAY2ALL		    CMD_PC_BASE + 402		//������Ϸ����㲥
#define CMD_PC_SAY2YOU			CMD_PC_BASE	+ 403		//P2P����(��Ҷ����)
#define CMD_PC_SAY2TEM			CMD_PC_BASE	+ 404		//�������
#define CMD_PC_SAY2GUD			CMD_PC_BASE	+ 405		//�л�(Guild)����

#define CMD_PC_SESS_CREATE		CMD_PC_BASE	+ 406		//�Ự����
#define CMD_PC_SESS_SAY			CMD_PC_BASE	+ 407		//�Ự˵��
#define CMD_PC_SESS_ADD			CMD_PC_BASE	+ 408		//�Ự������Ա
#define CMD_PC_SESS_LEAVE		CMD_PC_BASE	+ 409		//�뿪�Ự
#define CMD_PC_GM1SAY1			CMD_PC_BASE	+ 411		//�����㲥//Add by sunny.sun20080804

#define CMD_PC_ERRMSG			CMD_PC_BASE	+ 414	
#define CMD_PC_GUILDNOTICE		CMD_PC_BASE	+ 417
#define CMD_PC_REFRESH_SELECT	CMD_PC_BASE + 418

//End ����ָ��
//Begin�Ҷ�����������ϵͳ
#define CMD_PC_GARNER2_ORDER	CMD_PC_BASE +101
//End�Ҷ�����������ϵͳ
#define CMD_OS_LOGIN			CMD_OS_BASE + 1
#define CMD_OS_PING				CMD_OS_BASE + 2
#define CMD_SO_LOGIN			CMD_SO_BASE + 1
#define CMD_SO_PING				CMD_SO_BASE + 2
#define CMD_SO_WARING			CMD_SO_BASE + 3
#define CMD_SO_EXCEPTION		CMD_SO_BASE + 4

#define CMD_SO_ON_LINE			CMD_SO_BASE + 5
#define CMD_SO_OFF_LINE			CMD_SO_BASE + 6
#define CMD_SO_ENTER_MAP		CMD_SO_BASE + 7
#define CMD_SO_LEAVE_MAP		CMD_SO_BASE + 8




// Ga(t)eServer <-> (C)lient 
#define CMD_TC_DISCONNECT CMD_TC_BASE + 2

