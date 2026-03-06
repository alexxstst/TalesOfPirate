#pragma once

#include <lwITypes.h>

enum EActionNumber
{
    POSE_WAITING =       01,//	waiting	
    POSE_SHOW =          02,//	pose	
    POSE_SLEEP =         03,//	
    POSE_WAITING2 =      04,//	waiting	
    POSE_RUN =           05,//		
    POSE_RUN2 =          06,//		
    POSE_ATTACK =        07,//	1
    POSE_ATTACK1 =       8, //	2
    POSE_ATTACK2 =       9, //	3
    POSE_POWER_ATTACK =  10,//  
    POSE_SKILL1 =        11,//	
    POSE_SKILL2 =        12,//	
    POSE_SKILL3 =        13,//	
    POSE_SKILL4 =        14,//		011	
    POSE_SKILL5 =        15,//		012	
    POSE_SEAT =          16,//		
    POSE_DIE =           17,//	()	
    POSE_WAVE =          18,//	()	
    POSE_CRY =           19,//	()	
    POSE_JUMP =          20,//	()	
    POSE_ANGER =         21,//	()	
    POSE_DARE =          22,//	()	
    POSE_GAY =           23,//	()
    POSE_EMOTE7 =        24,//	7	
    POSE_EMOTE8 =        25,//	8	
    POSE_EMOTE9 =        26,//	9	
    POSE_EMOTE10 =       27,//	10	
    POSE_MINE =          28,//	(
    POSE_COLLECT =       29,//	(
    POSE_SEAT2 =         30,//	(
    POSE_LEAN =          31,//	(
    POSE_LEAN2 =         32,//	(
    POSE_HAPPY =         33,//	()
    POSE_HAPPY7 =        34,//	7
    POSE_HAPPY8 =        35,//	8
    POSE_HAPPY9 =        36,//	9
    POSE_HAPPY10 =       37,//	10
    POSE_RESERVED1 =     38,//	1	
    POSE_FALLDOWN =      39,//	2	
    POSE_SKILL6 =        40,//	6	
    POSE_SKILL7 =        41,//	7	
    POSE_FLY_WAITING =   42,//					//POSE_SKILL8 =        42,//	8	
    POSE_FLY_RUN =       43,//				//POSE_SKILL9 =        43,//	9	
    POSE_FLY_SHOW =      44,//				//POSE_SKILL10 =       44,//	10	
    POSE_FLY_SEAT =      45,//				//POSE_SKILL11 =       45,//	11	
    POSE_SKILL12 =       46,//	12	
    POSE_SKILL13 =       47,//	13	
    POSE_SKILL14 =       48,//	14	
    POSE_SKILL15 =       49,//	15	
    POSE_SKILL16 =       50,//	16	
    POSE_SKILL17 =       51,//	17	
    POSE_SKILL18 =       52,//	18	
    POSE_SKILL19 =       53,//	19	
    POSE_SKILL20 =       54,//	20	
};

struct SActionInfo
{
	short		m_sActionNO;	// 
	//short		m_sStartFrame;	// 
	//short		m_sEndFrame;	// 

	//short		m_sKeyFrameNum;	// 
	//short		*m_sKeyFrame;	// 
    lwPoseInfo info;
};

struct SCharacterAction
{
	short		m_iCharacterType;	// 
	short		m_iMaxActionNum;	// ()
	short		m_iActualActionNum;	// ()
	SActionInfo	*m_SActionInfo;		// 
};

class	CGameCharacterAction
{
public:
	CGameCharacterAction();
	~CGameCharacterAction();

	bool		Init(const char* ptcsFileName);
	void		Free(void);

	void		GetMaxCharType(int *iMaxType) {*iMaxType = m_iMaxCharacterType;};
	void		GetActualCharType(int *iActualType) {*iActualType = m_iActualCharacterType;};
	bool		GetCharMaxActNum(int iCharType, int *iMaxActNum);
	bool		GetCharActualActNum(int iCharType, int *iActualActNum);
	bool		GetCharFrameInfo(int iCharType, int iActionNO, int *iStartFrame, int *iEndFrame);
	bool		GetCharAction(int iCharType, SCharacterAction *SCharAct);

protected:
	short		m_iMaxCharacterType;		// ()
	short		m_iActualCharacterType;		// ()
	SCharacterAction	*m_SCharacterAction;
};

long StringGet(char* out, long out_max, char* in, long* in_from, const char* end_list, long end_len);
void StringSkipCompartment(char* in, long* in_from, const char* skip_list, long skip_len);


extern CGameCharacterAction g_PoseData;
extern void InitPoseData();
extern bool GetCharacterAction(int nTypeID, SCharacterAction *SCharAct);
