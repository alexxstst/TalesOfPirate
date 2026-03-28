#include "Stdafx.h"

#include "CharacterAction.h"

CGameCharacterAction g_PoseData;
void InitPoseData()
{
	g_PoseData.Init("scripts\\txt\\CharacterAction.tx");
}

bool GetCharacterAction(int nTypeID, SCharacterAction *SCharAct)
{
    return g_PoseData.GetCharAction(nTypeID, SCharAct);
}

CGameCharacterAction::CGameCharacterAction()
{
	m_iMaxCharacterType = 0;
	m_iActualCharacterType = 0;
	m_SCharacterAction = NULL;
}

CGameCharacterAction::~CGameCharacterAction()
{
	Free();
}

bool CGameCharacterAction::Init(const char* ptcsFileName)
{
	bool		bRet = true;
	char		tcsLine[1024], tcsTemp[1024];
	short		iCurType, iCurActNO, iCurKeyFrame;
	long		lIndex, lOldIndex, lFilePos;
	FILE		*fFile = NULL;

	m_iMaxCharacterType = 0;
	m_iActualCharacterType = 0;
	m_SCharacterAction = NULL;

	fFile = _tfopen(ptcsFileName, _TEXT("rb"));
	if (fFile == NULL)
	{
		ToLogService("errors", LogLevel::Error, "msgLoad Raw Data Info Txt File [{}] Fail!", ptcsFileName);
		bRet = false;
		goto end;
	}

	// ���"��ɫ�������͵����ֵ"(m_iMaxCharacterType),�Լ�"��ɫ�������͵���Ч��Ŀ"(m_iActualCharacterType);
	while (!feof(fFile))
	{
		lIndex = 0;
		_fgetts(tcsLine, 1023, fFile);
		StringSkipCompartment(tcsLine, &lIndex, " ", 1);
		if (tcsLine[lIndex] == 0x0a || tcsLine[lIndex] == 0x0d) //�س�, ����
			continue;
		if (tcsLine[lIndex] == _TEXT('/') && tcsLine[lIndex + 1] == _TEXT('/')) // ע��
			continue;
		if (tcsLine[lIndex] == _TEXT('\t')) // tab�� (�ڸ��ı�������"��ɫ���͵�һ������"�еĿ�ʼ����)
			continue;

		m_iActualCharacterType ++;

		StringGet(tcsTemp, 1023, tcsLine, &lIndex, _TEXT(" ,\x09\x0a\x0d"), 5);
		if (m_iMaxCharacterType < _ttoi(tcsTemp))
			m_iMaxCharacterType = _ttoi(tcsTemp);
	}
	//

	if (m_iActualCharacterType < 1)
	{
		bRet = true;
		goto end;
	}

	m_SCharacterAction = new (SCharacterAction[m_iMaxCharacterType]);
	if (m_SCharacterAction == NULL)
	{
		m_iMaxCharacterType = 0;
		bRet = false;
		goto end;
	}
	memset((void *)m_SCharacterAction, 0, sizeof(SCharacterAction) * m_iMaxCharacterType);

	// ��õ�����ɫ��Ϣ
	fseek(fFile, 0, SEEK_SET);
	while (!feof(fFile))
	{
		lIndex = 0;
		_fgetts(tcsLine, 1023, fFile);
		StringSkipCompartment(tcsLine, &lIndex, _TEXT(" "), 1);
		if (tcsLine[lIndex] == 0x0a || tcsLine[lIndex] == 0x0d) //�س�, ����
			continue;
		if (tcsLine[lIndex] == _TEXT('/') && tcsLine[lIndex + 1] == _TEXT('/')) // ע��
			continue;
		if (tcsLine[lIndex] == _TEXT('\t')) // tab�� (�ڸ��ı�������"��ɫ���͵�һ�������еĿ�ʼ����)
			continue;

		StringGet(tcsTemp, 1023, tcsLine, &lIndex, _TEXT(" ,\x09\x0a\x0d"), 5);
		iCurType = _ttoi(tcsTemp) - 1;
		if (iCurType < 0)
			continue;
		// ��ɫ�Ķ�����Ŀ
		m_SCharacterAction[iCurType].m_SActionInfo = NULL;
		m_SCharacterAction[iCurType].m_iActualActionNum = 0;
		m_SCharacterAction[iCurType].m_iCharacterType = iCurType + 1;
		m_SCharacterAction[iCurType].m_iMaxActionNum = 0;
		lFilePos = ftell(fFile);
		while (!feof(fFile))
		{
			lIndex = 0;
			_fgetts(tcsLine, 1023, fFile);
			StringSkipCompartment(tcsLine, &lIndex, _TEXT(" "), 1);
			if (tcsLine[lIndex] == 0x0a || tcsLine[lIndex] == 0x0d) //�س�, ����
				continue;
			if (tcsLine[lIndex] == _TEXT('/') && tcsLine[lIndex + 1] == _TEXT('/')) // ע��
				continue;
			if (tcsLine[lIndex] == _TEXT('\t')) // tab��
			{
				m_SCharacterAction[iCurType].m_iActualActionNum ++;

				StringSkipCompartment(tcsLine, &lIndex, _TEXT(" ,\x09"), 3);
				StringGet(tcsTemp, 1023, tcsLine, &lIndex, _TEXT(" ,\x09\x0a\x0d"), 5);

				// ��ʱ���룬 ������ʾ�����ļ��汾��
				if (_ttoi(tcsTemp) < 1)
				{
					MessageBox(NULL, _TEXT(g_oLangRec.GetString(24)), _TEXT(g_oLangRec.GetString(25)), 0);
					goto end;
				}
				//

				if (m_SCharacterAction[iCurType].m_iMaxActionNum < _ttoi(tcsTemp))
					m_SCharacterAction[iCurType].m_iMaxActionNum = _ttoi(tcsTemp);
			}
			else
				break;
		}
		if (m_SCharacterAction[iCurType].m_iActualActionNum < 1)
			continue;

		m_SCharacterAction[iCurType].m_SActionInfo = new (SActionInfo[m_SCharacterAction[iCurType].m_iMaxActionNum]);
		if (m_SCharacterAction[iCurType].m_SActionInfo == NULL)
		{
			m_SCharacterAction[iCurType].m_iMaxActionNum = 0;
			bRet = false;
			goto end;
		}
		memset((void *)(m_SCharacterAction[iCurType].m_SActionInfo), 0, sizeof(SActionInfo) * m_SCharacterAction[iCurType].m_iMaxActionNum);

		// ����������Ϣ
        SActionInfo* s;
		fseek(fFile, lFilePos, SEEK_SET);
		while (!feof(fFile))
		{
			lFilePos = ftell(fFile);
			lIndex = 0;
			_fgetts(tcsLine, 1023, fFile);
			StringSkipCompartment(tcsLine, &lIndex, _TEXT(" "), 1);
			if (tcsLine[lIndex] == 0x0a || tcsLine[lIndex] == 0x0d) //�س�, ����
				continue;
			if (tcsLine[lIndex] == _TEXT('/') && tcsLine[lIndex + 1] == _TEXT('/')) // ע��
				continue;
			if (tcsLine[lIndex] == _TEXT('\t')) // tab��
			{
				StringSkipCompartment(tcsLine, &lIndex, _TEXT("\x09"), 1);
				StringGet(tcsTemp, 1023, tcsLine, &lIndex, _TEXT(" ,\x09\x0a\x0d"), 5);

				iCurActNO = _ttoi(tcsTemp);
				if (iCurActNO < 1)
					continue;

                s = &m_SCharacterAction[iCurType].m_SActionInfo[iCurActNO - 1];
				//m_SCharacterAction[iCurType].m_SActionInfo[iCurActNO - 1].m_sActionNO = iCurActNO;
                s->m_sActionNO = iCurActNO;

				StringSkipCompartment(tcsLine, &lIndex, _TEXT(" ,\x09"), 3);
				StringGet(tcsTemp, 1023, tcsLine, &lIndex, _TEXT(" ,\x09\x0a\x0d"), 5);
				//m_SCharacterAction[iCurType].m_SActionInfo[iCurActNO - 1].m_sStartFrame = _ttoi(tcsTemp);
                s->info.start = _ttoi(tcsTemp);

				StringSkipCompartment(tcsLine, &lIndex, _TEXT(" ,\x09"), 3);
				StringGet(tcsTemp, 1023, tcsLine, &lIndex, _TEXT(" ,\x09\x0a\x0d"), 5);
				//m_SCharacterAction[iCurType].m_SActionInfo[iCurActNO - 1].m_sEndFrame = _ttoi(tcsTemp);
                s->info.end = _ttoi(tcsTemp); 

				StringSkipCompartment(tcsLine, &lIndex, _TEXT(" ,\x09"), 3);
				lOldIndex = lIndex;
				// �ؼ�֡��Ŀ
				iCurKeyFrame = 0;
				while (StringGet(tcsTemp, 1023, tcsLine, &lIndex, _TEXT(" ,\x09\x0a\x0d"), 5))
				{
					iCurKeyFrame ++;
					StringSkipCompartment(tcsLine, &lIndex, _TEXT(" ,\x09"), 3);
				}
				if (iCurKeyFrame < 1)
					continue;

                if(iCurKeyFrame > MAX_KEY_FRAME_NUM)
                    iCurKeyFrame = MAX_KEY_FRAME_NUM;

				//m_SCharacterAction[iCurType].m_SActionInfo[iCurActNO - 1].m_sKeyFrameNum = iCurKeyFrame;
                s->info.key_frame_num = iCurKeyFrame;

				//m_SCharacterAction[iCurType].m_SActionInfo[iCurActNO - 1].m_sKeyFrame
				//	= new (short[m_SCharacterAction[iCurType].m_SActionInfo[iCurActNO - 1].m_sKeyFrameNum]);
				//if (m_SCharacterAction[iCurType].m_SActionInfo[iCurActNO - 1].m_sKeyFrame == NULL)
				//{
				//	m_SCharacterAction[iCurType].m_SActionInfo[iCurActNO - 1].m_sKeyFrameNum = 0;
				//	bRet = false;
				//	goto end;
				//}

				lIndex = lOldIndex;
				// �ؼ�֡
				iCurKeyFrame = 0;
				while (StringGet(tcsTemp, 1023, tcsLine, &lIndex, _TEXT(" ,\x09\x0a\x0d"), 5))
				{
					//m_SCharacterAction[iCurType].m_SActionInfo[iCurActNO - 1].m_sKeyFrame[iCurKeyFrame] = _ttoi(tcsTemp);
                    s->info.key_frame_seq[iCurKeyFrame] = _ttoi(tcsTemp);
					iCurKeyFrame ++;
					StringSkipCompartment(tcsLine, &lIndex, _TEXT(" ,\x09"), 3);
				}
			}
			else
			{
				fseek(fFile, lFilePos, SEEK_SET);
				break;
			}
		}
	}
	//

end:
	if (fFile)
		fclose(fFile);
	if (!bRet)
		Free();

	return bRet;
}

void CGameCharacterAction::Free(void)
{
	//short	i, j;
	short	i;

	if (m_SCharacterAction)
	{
		for (i = 0; i < m_iMaxCharacterType; i ++)
		{
			//for (j = 0; j < m_SCharacterAction[i].m_iMaxActionNum; j ++)
			//{
			//	if (m_SCharacterAction[i].m_SActionInfo[j].m_sKeyFrame)
			//	{
			//		delete [] m_SCharacterAction[i].m_SActionInfo[j].m_sKeyFrame;
			//		m_SCharacterAction[i].m_SActionInfo[j].m_sKeyFrame = NULL;
			//	}
			//}
			
			//delete [] m_SCharacterAction[i].m_SActionInfo;
			//m_SCharacterAction[i].m_SActionInfo = NULL;
			SAFE_DELETE_ARRAY(m_SCharacterAction[i].m_SActionInfo); // UI��������
		}
		//delete [] m_SCharacterAction;
		//m_SCharacterAction = NULL;
		SAFE_DELETE_ARRAY(m_SCharacterAction); // UI��������
	}
}

bool CGameCharacterAction::GetCharMaxActNum(int iCharType, int *iMaxActNum)
{
	if (m_SCharacterAction == NULL)
		return false;
	if (iCharType < 1 || iCharType > m_iMaxCharacterType)
		return false;

	*iMaxActNum = m_SCharacterAction[iCharType - 1].m_iMaxActionNum;

	return true;
}

bool CGameCharacterAction::GetCharActualActNum(int iCharType, int *iActualActNum)
{
	if (m_SCharacterAction == NULL)
		return false;
	if (iCharType < 1 || iCharType > m_iMaxCharacterType)
		return false;

	*iActualActNum = m_SCharacterAction[iCharType - 1].m_iActualActionNum;

	return true;
}

bool CGameCharacterAction::GetCharFrameInfo(int iCharType, int iActionNO, int *iStartFrame, int *iEndFrame)
{
	if (m_SCharacterAction == NULL)
		return false;
	if (iCharType < 1 || iCharType > m_iMaxCharacterType)
		return false;
	if (m_SCharacterAction[iCharType - 1].m_iCharacterType < 1)
		return false;
	if (iActionNO < 1 || iActionNO > m_SCharacterAction[iCharType - 1].m_iMaxActionNum)
		return false;
	if (m_SCharacterAction[iCharType - 1].m_SActionInfo[iActionNO - 1].m_sActionNO < 1)
		return false;

	//*iStartFrame = m_SCharacterAction[iCharType - 1].m_SActionInfo[iActionNO - 1].m_sStartFrame;
	//*iEndFrame = m_SCharacterAction[iCharType - 1].m_SActionInfo[iActionNO - 1].m_sEndFrame;

    *iStartFrame = m_SCharacterAction[iCharType - 1].m_SActionInfo[iActionNO - 1].info.start;
    *iEndFrame = m_SCharacterAction[iCharType - 1].m_SActionInfo[iActionNO - 1].info.end;

	return true;
}

bool CGameCharacterAction::GetCharAction(int iCharType, SCharacterAction *SCharAct)
{
	if (m_SCharacterAction == NULL || SCharAct == NULL)
		return false;
	if (iCharType < 1 || iCharType > m_iMaxCharacterType)
		return false;
	if (m_SCharacterAction[iCharType - 1].m_iCharacterType < 1)
		return false;

	memcpy((void *)SCharAct, (void *)(m_SCharacterAction + iCharType - 1), sizeof(SCharacterAction));

	return true;
}

// �Ӹ����ַ���(_TCHAR *in)�ĸ���λ��(long *in_from)��ȡ�ַ��ܣ�
// ֱ�������ַ����б�(_TCHAR *end_list)�е���һ�ַ�ʱ����
long StringGet(char* out, long out_max, char* in, long* in_from, const char* end_list, long end_len)
{
    long offset=-1;    // set offset of get string to -1 for the first do process
    long i;    // temp variable

    --(*in_from);   // dec (*in_from) for the first do process
    do
    {
        out[++offset]=in[++(*in_from)];
        for(i=end_len-1;i>=0;--i)
        {
            if(out[offset]==end_list[i])
            {
                out[offset]=0x00;
                break;
            }
        }
    }while(out[offset] && offset<out_max);
    return offset;
}

// �Ӹ����ַ���(_TCHAR *in)�ĸ���λ��(long *in_from)�޳��ַ����б�(_TCHAR *end_list)�е���һ�ַ�
void StringSkipCompartment(char* in, long* in_from, const char* skip_list, long skip_len)
{
    long i;    // temp variable

    while(in[(*in_from)])
    {
        for(i=skip_len-1;i>=0;--i)
        {
            if(in[(*in_from)]==skip_list[i])    
                break;
        }
        if(i<0) break;  // dismatch skip conditions, finished
        else ++(*in_from);  // match skip conditions, skip it
    }
}
