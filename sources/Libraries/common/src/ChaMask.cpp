#include ".\chamask.h"

char	szBase64[defBASE54_CONV_SIZE];

void	CMaskData::AddMap(const char* pszName, int w,int h)
{
	//pMapMask[iMapNum] = new sMapMask;
	int lenx = (w / SHOWSIZE) + 1;
	int leny = (h / SHOWSIZE) + 1;

	int len = ((lenx * leny) / 8 + 1)+ sizeof(sMapMask);
	sMapMask*	pMapMask;
	pMapData[iMapNum] = new BYTE[ len ];

	pMapMask = (sMapMask*)pMapData[iMapNum];
	lstrcpy(pMapMask->pszName,pszName);

	pMapMask->lenx = lenx;
	pMapMask->leny = leny; 

	pMapMask->llen = len;
	//memset(pMapData[iMapNum], 0, pMapMask[iMapNum].llen + sizeof(sMapMask) );
	iMapNum++;

	pMapMask->lpos  = 0;//lResultSize;
	//lResultSize += len;

	//pResultMask = new BYTE[lResultSize];
	//memset(pResultMask, 0, lResultSize);
}

bool CMaskData::InitMaskData(char* pszMapName, const char* szBase64Data, long llen)
{
	sMapMask*	pMask;
	BYTE* p;

	if(szBase64Data[0] == '0')
	{
		for ( int n = 0; n < iMapNum; n++)
		{
			pMask = (sMapMask*)pMapData[n];
			if(!lstrcmp(pMask->pszName, pszMapName) == 0)
				continue;

			p = pMapData[n];
			p += sizeof(sMapMask);
			memset(p ,0 , pMask->llen - sizeof(sMapMask));
		}
		return true;
	}

	int n = 0; 
	for (; n < iMapNum; n++)
	{
		pMask = (sMapMask*)pMapData[n];

		if(lstrcmp(pMask->pszName, pszMapName) == 0)
		{
			break;
		}
	}
	pMask = (sMapMask*)pMapData[n];

	unsigned int unTarLen;
	ibase64(szBase64Data, (unsigned int)strlen(szBase64Data), szBase64, &unTarLen);
	if (pMask->llen != unTarLen)
		return false;

	memcpy(pMapData[n],szBase64,unTarLen);

	//sMapMask*	pMask = (sMapMask*)pMaskData;
	//memcpy(&pMapMask,p, sizeof(sMapMask) * iMapNum);
	//p += sizeof(sMapMask) * iMapNum;

	//for (int n = 0; n < iMapNum; n++)
	//{
	//	memcpy(&pMapMask[n],p, sizeof(sMapMask));
	//	p += sizeof(sMapMask);
	//}

	////BYTE* p;
	////for ( int n = 0; n < iMapNum; n++)
	////{
	////	pMask = (sMapMask*)pMapData[n];

	////	p = pMapData[n];
	////	memcpy(p ,&szBase64[pMask->lpos] , pMask->llen);
	////}
	return true;
}

//void	CMaskData::ColResultMask()
//{
//	//lResultSize = sizeof(sMapMask) * iMapNum;
//	//for (int n = 0; n < iMapNum; n++)
//	//{
//	//	pMapMask[n].lpos  = lResultSize;
//	//	lResultSize += pMapMask[n].llen;
//	//}
//	////long size = 0;
//	//pResultMask = new BYTE[lResultSize];
//	if(!pResultMask)
//		pResultMask = new BYTE[lResultSize];
//	BYTE* p = pResultMask;
//	//memcpy(p, &iMapNum, sizeof(int));
//	//p += sizeof(int);
//	//size += sizeof(int);
//	//memcpy(p, &pMapMask, sizeof(sMapMask) * iMapNum);
//
//	//for ( int n = 0; n < iMapNum; n++)
//	//{
//	//	//size += sizeof(sMapMask);
//	//	memcpy(p, &pMapMask[n], sizeof(sMapMask));
//	//	p += sizeof(sMapMask);
//	//}
//	//p += sizeof(sMapMask) * iMapNum;
//
//	sMapMask*	pMask;
//
//	for ( int n = 0; n < iMapNum; n++)
//	{
//		pMask = (sMapMask*)pMapData[n];
//
//		p = pResultMask + pMask->lpos;
//		memcpy(p, pMapData[n], pMask->llen);
//	}
//	//pResultMask[lResultSize] = 0;
//}

BYTE*	CMaskData::GetMapMask(const char* pszMapName, long& size)
{
	if(iMapNum == 0)
		return NULL;
	//sMapMask* pmap;
	sMapMask*	pMask;

	for (int n = 0; n < iMapNum; n++)
	{
		pMask = (sMapMask*)pMapData[n];

		if(lstrcmp(pMask->pszName, pszMapName) == 0)
		{
			//pmap = &pMapMask[n];
			size = pMask->llen;
			return pMapData[n];
		}
	}
	return NULL;
}

float	CMaskData::GetMapMaskOpenScale(const char* pszMapName)
{
	long	lSize;
	BYTE*	pbMaskData = GetMapMask(pszMapName, lSize);
	if (!pbMaskData)
		return 0;

	float	fScale = 0;
	int ty = ((sMapMask *)pbMaskData)->leny;
	int tx = ((sMapMask *)pbMaskData)->lenx;
	for (int m = 0; m <  ty; m++)
	{
		for (int n = 0; n <  tx; n++)
		{
			BYTE* p = pbMaskData;
			p += sizeof(sMapMask);
			int pos = m * tx + n;
			int potion = pos / 8;

			pos = pos - potion * 8;

			if(p[potion] & (1 << pos))
			{
				fScale += 1;
			}
		}
	}
	fScale /= ty * tx;
	fScale *= 100;

	return fScale;
}

bool	CMaskData::UpdateMapMask(char* pszMapName, int x, int y, int cha_range)
{
	sMapMask* pmap = NULL;
	BYTE* pData = NULL;
	for (int n = 0; n < iMapNum; n++)
	{
		pmap = (sMapMask*)pMapData[n];

		if(lstrcmp(pmap->pszName, pszMapName) == 0)
		{
			pData = pMapData[n];
			break;
		}
	}
	if(!pmap || !pData)
		return false;

	pData += sizeof(sMapMask);

	int nx = x / SHOWSIZE;
	int ny = y / SHOWSIZE;
	int pos = ny * pmap->lenx + nx;
	int potion = pos / 8;
	pos = pos - potion * 8;
	int value = (1 << pos);
	if(!(pData[potion] & value))
	{
		pData[potion] |= value;
		return true;
	}
	return false;


	//int nx1 = (x - cha_range) / SHOWSIZE;
	//int nx2 = (x + cha_range) / SHOWSIZE;
	//int ny1 = (y - cha_range) / SHOWSIZE;
	//int ny2 = (y + cha_range) / SHOWSIZE;

	//for(int m = ny1; m <=  ny2; ++m)
	//{
	//	for(int n = nx1; n <= nx2; ++n)
	//	{
	//		if(n < 0 || m < 0 || n >= pmap->lenx || m >= pmap->leny)
	//			continue;

	//		int pos = m * pmap->lenx + n;
	//		int potion = pos / 8;

	//		pos = pos - potion * 8;

	//		int value = (1 << pos);
	//		if(!(pData[potion] & value))
	//			pData[potion] |= value;
	//	}
	//}
}
