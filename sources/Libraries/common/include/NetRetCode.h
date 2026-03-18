/**************************************************************************************************************
*
*			������÷����붨���ļ�											(Created by Andor.Zhang in 2004.12)
*
**************************************************************************************************************/
#ifndef NETRETCODE_H
#define NETRETCODE_H

#define ERR_SUCCESS	0				//���еĳɹ����÷��ؾ�Ϊ 0����0��ʾ���ֲ��ɹ�ԭ���־

//��������������������������������������������������������������������������������������
//					������÷����붨��(16bit)
//			�����붨�崫�䷽���ʶ(��ERR_CM_XXX�е�CM)��
//				CM	(C)lient		->Ga(m)eServer
//				MC	Ga(m)eServer	->(C)lient
//						......
//				(������"�������������"����)
//	(ע��ÿ�����ذ��ķ������Ϊ���Ŀ�ʼ��16bit,����һ��WriteInt64/ReadShort�Ĳ���)
//	(ע��ÿ������ķ�����ռ���Ԥ��500��������ÿ�������1��ʼ����ERR_MC_BASE+1��ʼ����)
//��������������������������������������������������������������������������������������
/*=====================================================================================================
*					������������壺
*/
#define ERR_MC_BASE			   0				//GameServer/GateServer->Client���صĴ�����ռ�1��500
#define ERR_PT_BASE			 500				//GroupServer->GateServer���صĴ�����ռ�501��1000
#define ERR_AP_BASE			1000				//AccountServer->GroupServer���صĴ�����ռ�1001��1500
#define ERR_MT_BASE			1500				//GameServer->GateServer���صĴ�����ռ�1501��2000
#define ERR_TM_BASE         2000                //GateServer->GameServer���صĴ�����ռ�2000��2500
#define ERR_OS_BASE         2500                //M(o)nitorServer->(S)erver·µ»ØµÄ´íÎóÂë¿Õ¼ä2500£­3000

/*=====================================================================================================
*				AccountServer->GroupServer
*/
#define ERR_AP_INVALIDUSER	ERR_AP_BASE	+ 1		//�û���������
#define ERR_AP_INVALIDPWD	ERR_AP_BASE	+ 2		//�������
#define ERR_AP_ACTIVEUSER   ERR_AP_BASE + 3     //�����û�ʧ��
#define ERR_AP_LOGGED		ERR_AP_BASE + 4		//���û��ѵ�¼
#define ERR_AP_DISABLELOGIN ERR_AP_BASE + 5     //���û����ڱ���ֹLogin״̬
#define ERR_AP_BANUSER      ERR_AP_BASE + 6     //�ٷ���ͣ�ʺ�
#define ERR_AP_PBANUSER     ERR_AP_BASE + 7     //¸öÈËÃÜ±£¿¨ÕÊºÅ
#define ERR_AP_SBANUSER		ERR_AP_BASE + 8	

#define ERR_AP_GPSLOGGED    ERR_AP_BASE + 11    //��GroupServer�ѵ�¼
#define ERR_AP_GPSAUTHFAIL  ERR_AP_BASE + 12    //��GroupServer��֤ʧ��

#define ERR_AP_UNKNOWN		ERR_AP_BASE + 100	//δ֪ԭ��
#define ERR_AP_OFFLINE		ERR_AP_BASE + 101	//���û�����������״̬
#define ERR_AP_LOGIN1		ERR_AP_BASE + 102	//���û���������֤1�׶�
#define ERR_AP_LOGIN2		ERR_AP_BASE + 103	//���û���������֤2״̬
#define ERR_AP_ONLINE		ERR_AP_BASE + 104	//���û�������
#define ERR_AP_SAVING		ERR_AP_BASE + 105	//���û��˺������ڴ���״̬
#define ERR_AP_LOGINTWICE	ERR_AP_BASE + 106   //���û��˺���Զ���ٴε�¼
#define ERR_AP_DISCONN		ERR_AP_BASE + 107	//group�Ѷϵ�
#define ERR_AP_UNKNOWNCMD	ERR_AP_BASE + 108	//δ֪Э��
#define ERR_AP_TLSWRONG		ERR_AP_BASE + 109	//�̱߳��ش洢����
#define ERR_AP_NOBILL		ERR_AP_BASE + 110	//�˺���Ҫ��ֵ
#define ERR_AP_NOALT        ERR_AP_BASE + 111   // No alts
#define ERR_AP_3ACCS        ERR_AP_BASE + 112   // max 3 accs

/*=====================================================================================================
*				GroupServer->GateServer
*/
#define	ERR_PT_LOGFAIL		ERR_PT_BASE	+ 1		//GateServer��GroupServer�ĵ�¼ʧ��
#define ERR_PT_SAMEGATENAME	ERR_PT_BASE	+ 2		//GateServer���ѵ�¼GateServer����

#define ERR_PT_INVALIDDAT	ERR_PT_BASE	+ 20	//��Ч�����ݸ�ʽ
#define ERR_PT_INERR		ERR_PT_BASE + 21	//������֮��Ĳ��������Դ���
#define ERR_PT_NETEXCP		ERR_PT_BASE	+ 22	//GroupServer���ֵĵ�AccuntServer���������
#define ERR_PT_DBEXCP		ERR_PT_BASE + 23	//GroupServer���ֵĵ�Database�Ĺ���
#define ERR_PT_INVALIDCHA	ERR_PT_BASE + 24	//��ǰ�ʺŲ�ӵ������(ѡ��/ɾ��)�Ľ�ɫ
#define ERR_PT_TOMAXCHA		ERR_PT_BASE	+ 25	//�Ѿ��ﵽ����ܴ����Ľ�ɫ����
#define ERR_PT_SAMECHANAME	ERR_PT_BASE	+ 26	//�ظ��Ľ�ɫ��
#define ERR_PT_INVALIDBIRTH	ERR_PT_BASE	+ 27	//�Ƿ���ְҵ
#define ERR_PT_TOOBIGCHANM	ERR_PT_BASE	+ 28	//��ɫ��̫��
#define ERR_PT_KICKUSER		ERR_PT_BASE	+ 29
#define ERR_PT_ISGLDLEADER	ERR_PT_BASE	+ 30	//���᲻��һ���޳������Ƚ�ɢ������ɾ����ɫ
#define ERR_PT_ERRCHANAME	ERR_PT_BASE	+ 31	//�Ƿ��Ľ�ɫ����
#define ERR_PT_SERVERBUSY   ERR_PT_BASE + 32	//������æµ�����Ժ�
#define ERR_PT_TOOBIGPW2	ERR_PT_BASE + 33	//��������̫��
#define ERR_PT_INVALID_PW2  ERR_PT_BASE + 34	//��Ч�ö������룬���ܽ�����Ϸ

// Add by lark.li 20080825 begin
#define ERR_PT_BANUSER      ERR_PT_BASE + 35     //1����?��a����?��o?
#define ERR_PT_PBANUSER     ERR_PT_BASE + 36     //??��??������?��?��o?
// End

#define ERR_PT_GMISLOG     ERR_PT_BASE + 37
#define ERR_PT_MULTICHA	   ERR_PT_BASE + 38
#define ERR_PT_BONUSCHARS	   ERR_PT_BASE + 39

#define ERR_PT_BADBOY		ERR_PT_BASE + 50	//

/*=====================================================================================================
*				GameServer/GateServer->Client
*/
#define ERR_MC_NETEXCP		ERR_MC_BASE	+ 1		//GateServer���ֵ������쳣
#define ERR_MC_NOTSELCHA	ERR_MC_BASE	+ 2		//��ǰδ����ѡ���ɫ״̬
#define ERR_MC_NOTPLAY		ERR_MC_BASE + 3		//��ǰ����������Ϸ״̬�����ܷ���ENDPLAY����
#define ERR_MC_NOTARRIVE	ERR_MC_BASE + 4		//Ŀ���ͼ���ɵ���
#define ERR_MC_TOOMANYPLY	ERR_MC_BASE	+ 5		//Ŀ���������ҹ���
#define ERR_MC_NOTLOGIN		ERR_MC_BASE	+ 6		//��δ��½��
#define ERR_MC_VER_ERROR	ERR_MC_BASE + 7		//�ͻ��˵İ汾�Ŵ���
#define ERR_MC_ENTER_ERROR	ERR_MC_BASE + 8		//�����ͼʧ��
#define ERR_MC_ENTER_POS	ERR_MC_BASE + 9		//�����ͼ��λ�÷Ƿ�
#define ERR_MC_BANUSER      ERR_MC_BASE + 10     //¹Ù·½·âÍ£ÕÊºÅ
#define ERR_MC_PBANUSER     ERR_MC_BASE + 11     //¸öÈËÃÜ±£¿¨ÕÊºÅ

/*=====================================================================================================
*				GateServer->GameServer
*/
#define ERR_TM_OVERNAME     ERR_TM_BASE + 1     //GameServer���ظ�
#define ERR_TM_OVERMAP      ERR_TM_BASE + 2     //GameServer�ϵ�ͼ���ظ�
#define ERR_TM_MAPERR       ERR_TM_BASE + 3     //GameServer��ͼ�����﷨����

#define ERR_OS_NOTMATCH_VERSION	ERR_OS_BASE + 1		// ²»Æ¥Åä°æ±¾
#define ERR_OS_RELOGIN	ERR_OS_BASE + 2		// Ë«ÖØµÇÂ¼


namespace ReturnCode
{
	enum class OfflineMode : uint8_t
	{
		Success,
		Disabled,
		Refuse,
		Dead,
		Unknown
	};
}

#endif
