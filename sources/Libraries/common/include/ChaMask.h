#pragma once
#include "util.h"
#define SHOWSIZE	80
#define SHOWRANGE	80

#define defBASE54_CONV_SIZE	100 * 1024
char	szBase64[];

#define MAX_MAP_COUNT	10
class CMaskData
{
public:
	CMaskData()
	{
		//pData = NULL;
		//lResultSize = 0;
		//pResultMask = NULL;
		iMapNum = 0;
		//memset(pMapData,0,MAX_MAP_COUNT * sizeof(BYTE*));
		for (int n = 0; n < MAX_MAP_COUNT; n++)
		{
			pMapData[n] = NULL;
				
		}
		//memset(pMapMask,0,sizeof(sMapMask) * MAX_MAP_COUNT);
	}
	~CMaskData()
	{
		//lResultSize = 0;
		iMapNum = 0;

		for (int n = 0; n < MAX_MAP_COUNT; n++)
		{
			if(pMapData[n])
				SAFE_DELETE_ARRAY(pMapData[n]);
		}

		//SAFE_DELETE_ARRAY(pResultMask);
	}
	struct sMapMask
	{
		char pszName[32];
		int lenx,leny;
		long lpos;
		long llen;
	};
	//
	void	AddMap(const char* pszName, int w,int h);

	//mask
	bool	InitMaskData(char* pszMapName, const char* szBase64Data, long llen = 0);

	//mask
	BYTE*	GetMapMask(const char* pszMapName, long& size);

	// ()
	float	GetMapMaskOpenScale(const char* pszMapName);

	//mask, x,yw,h
	bool	UpdateMapMask(char* pszMapName, int x, int y, int cha_range = SHOWRANGE);

	//
	//void	SetChaRange(int cha_range){ iChaRange = cha_range;}
	//int 	GetChaRange(int cha_range){ iChaRange = cha_range;}

	//
	//long	GetResultLength(){ return lResultSize;}
	//const char*	GetResultMask(const char* pszMapName)
	//{ 
	//	//ColResultMask();
	//	unsigned int nLen;
	//	BYTE	*pbyOneMask = GetMapMask(pszMapName, nLen);

	//	base64((const char*)pbyOneMask, lResultSize, szBase64, defBASE54_CONV_SIZE, &nLen);
	//	return (const char *)szBase64;
	//}
	const char*	GetResultOneMask(const char* pszMapName)
	{
		unsigned int nLen;
		long	lOneLen;
		BYTE	*pbyOneMask = GetMapMask(pszMapName, lOneLen);
		base64((const char*)pbyOneMask, lOneLen, szBase64, defBASE54_CONV_SIZE, &nLen);
		return (const char *)szBase64;
	}
protected:
	//
	//void	ColResultMask();

	//bool	GetMask(int x, int y)
	//{
	//	//int x  = posx;//((posx / 100) / 40 );
	//	//int y  = posy;//((posy / 100) / 40 );
	//	if(x < 0 || y < 0 ||x >= iNumX || y >= iNumY)
	//		return false;

	//	int pos = y * iNumX + x;
	//	int potion = pos / 8;

	//	pos = pos - potion * 8;

	//	if(pData[potion] & (1 << pos))
	//		return true;
	//	return false;
	//}
	//void	SetMask(int x, int y)
	//{
	//	if(x < 0 || y < 0 ||x >= iNumX || y >= iNumY)
	//		return;
	//	//int x  = posx;//((posx / 100) / 40 );
	//	//int y  = posy;//((posy / 100) / 40 );

	//	int pos = y * iNumX + x;
	//	int potion = pos / 8;

	//	pos = pos - potion * 8;

	//	int value = (1 << pos);
	//	if(!(pData[potion] & value))
	//		pData[potion] |= value;
	//}
protected:
	//sMapMask	pMapMask[MAX_MAP_COUNT];
	//BYTE* pData;

	BYTE* pMapData[MAX_MAP_COUNT];
	int iMapNum;

	//long  lResultSize;
	//BYTE* pResultMask;

	//int		iChaRange;
};
