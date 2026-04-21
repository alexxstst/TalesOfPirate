//
#pragma once

#include <tchar.h>
#include "util.h"
#include "TableData.h"

// 
enum ID_BTN_BOAT
{
    ID_BTN_HEADLEFT =      0,
    ID_BTN_HEADRIGHT,
    ID_BTN_POWERLEFT,
    ID_BTN_POWERRIGHT,
    ID_BTN_WEAPONLEFT,
    ID_BTN_WEAPONRIGHT,

    ID_BTN_ITEM_NUM,
};
#define BOAT_MAXSIZE_MINNAME	2   // 
#define BOAT_MAXSIZE_BOATNAME	17	// 
#define BOAT_MAXSIZE_NAME		64	// 
#define BOAT_MAXSIZE_DESP		128 // 
#define BOAT_MAXSIZE_PART		3   // 
#define BOAT_MAXNUM_PARTITEM	16  // 
#define BOAT_MAXNUM_MOTOR		4	// 
#define BOAT_MAXNUM_MODEL		8	// 
#define MAX_CHAR_BOAT			3	// 

// 
struct BOAT_BERTH_DATA
{
	BYTE byID[MAX_CHAR_BOAT];
	BYTE byState[MAX_CHAR_BOAT];
	char szName[MAX_CHAR_BOAT][BOAT_MAXSIZE_BOATNAME + BOAT_MAXSIZE_DESP];
};

enum BOAT_STATE
{
	BS_GOOD		= 0,				// 
	BS_NOSP		= 1,				// SP
	BS_NOHP		= 2,				// 
	BS_DEAD		= 3,				// 
};

// 
struct BOAT_DATA
{
	char szName[BOAT_MAXSIZE_BOATNAME];		// 
	USHORT sBoat;				// ID
	USHORT sHeader;				// 
	USHORT sBody;				// 
	USHORT sEngine;				// 
	USHORT sCannon;				// 
	USHORT sEquipment;			// 
	USHORT sBerth;				// 
	USHORT sCapacity;			// 
	DWORD  dwOwnerID;			// ID
};

// 
struct BOAT_LOAD_INFO
{
	DWORD  dwID;				// ID
	char   szName[BOAT_MAXSIZE_BOATNAME];			// 
	USHORT sHeader;				// 
	USHORT sBody;				// 
	USHORT sEngine;				// 
	USHORT sCannon;				// 
	USHORT sEquipment;			// 
	USHORT sBerth;				// 
};

struct xShipBuildInfo
{
	char szName[BOAT_MAXSIZE_NAME];			// 
	char szDesp[BOAT_MAXSIZE_DESP];			// 
	char szBerth[BOAT_MAXSIZE_NAME];		// 
	char szHeader[BOAT_MAXSIZE_NAME];		// 
	char szBody[BOAT_MAXSIZE_NAME];			// 
	char szEngine[BOAT_MAXSIZE_NAME];		// 
	char szCannon[BOAT_MAXSIZE_NAME];		// 
	char szEquipment[BOAT_MAXSIZE_NAME];	// 

	// ID	
	USHORT sPosID;				// ID
	union
	{
		struct
		{
			DWORD dwBody;		// 		
			DWORD dwHeader;		// 	
			DWORD dwEngine;		// 
			DWORD dwFlag;		// 
			DWORD dwMotor[BOAT_MAXNUM_MOTOR];
		};

		DWORD dwBuf[BOAT_MAXNUM_MODEL];
	};

	BYTE  byHeader;	   // 
	BYTE  byEngine;	   // 
	BYTE  byCannon;	   // 
	BYTE  byEquipment; // 
	BYTE  byIsUpdate;  // 
	DWORD dwMoney;	   // 
    DWORD dwMinAttack; // 
    DWORD dwMaxAttack; // 
    DWORD dwCurEndure; // 
    DWORD dwMaxEndure; // 
    DWORD dwSpeed;	   // 
    DWORD dwDistance;  // 
    DWORD dwDefence;   // 
	DWORD dwCurSupply; // 
	DWORD dwMaxSupply; // 
	DWORD dwConsume;   // 
    DWORD dwAttackTime;// 
	USHORT	sCapacity; // 
};

struct xShipAttrInfo
{
	// 
	DWORD dwMoney;	   // 
    DWORD dwMinAttack; // 
    DWORD dwMaxAttack; // 
    DWORD dwCurEndure; // 
    DWORD dwMaxEndure; // 
    DWORD dwSpeed;	   // 
    DWORD dwDistance;  // 
    DWORD dwDefence;   // 
	DWORD dwCurSupply; // 
	DWORD dwMaxSupply; // 
    DWORD dwAttackTime;// 
	USHORT	sCapacity; // 

	// 	
	DWORD dwResume;			// 
	DWORD dwResist;	        // 
	DWORD dwScope;			// 
	DWORD dwConsume;		// 
	DWORD dwCannonSpeed;	// 

	// 
};

struct xShipInfo: public EntityData
{
	char szName[BOAT_MAXSIZE_NAME];	// 
	char szDesp[BOAT_MAXSIZE_DESP];	// 
	USHORT sItemID;					// ID
	USHORT sCharID;					// ID
	USHORT sPosID;					// ID
	BYTE byIsUpdate;				// 	
	USHORT sNumHeader;
	USHORT sNumEngine;
	USHORT sNumCannon;
	USHORT sNumEquipment;
	USHORT sHeader[BOAT_MAXNUM_PARTITEM];		// 	
	USHORT sEngine[BOAT_MAXNUM_PARTITEM];		// 
	USHORT sCannon[BOAT_MAXNUM_PARTITEM];		// 
	USHORT sEquipment[BOAT_MAXNUM_PARTITEM];	// 
	USHORT sBody;			// 	
	USHORT sLvLimit;		// 
	USHORT sNumPfLimit;
	USHORT sPfLimit[BOAT_MAXNUM_PARTITEM];		// 

	USHORT sEndure;			// 
	USHORT sResume;			// 
	USHORT sDefence;		// 
	USHORT sResist;			// 
    USHORT sMinAttack;		// 
    USHORT sMaxAttack;		// 
	USHORT sDistance;		// 
	USHORT sTime;			// 
	USHORT sScope;			// 
	USHORT sCapacity;		// 
	USHORT sSupply;			// 
	USHORT sConsume;		// 
	USHORT sCannonSpeed;	// 
	USHORT sSpeed;			// 
	USHORT sParam;			// 

	xShipInfo()
	{
		memset( szName, 0, BOAT_MAXSIZE_NAME );
		memset( szDesp, 0, BOAT_MAXSIZE_DESP );
		sItemID = 0;
		sCharID = 0;
		sPosID = 0;
		byIsUpdate = 0;
		sNumHeader = 0;
		sNumEngine = 0;
		sNumCannon = 0;
		sNumEquipment = 0;
		memset( sHeader, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		memset( sEngine, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		memset( sCannon, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		memset( sEquipment, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		sBody = 0;
		sLvLimit = 0;		// 
		sNumPfLimit = 0;
		memset( sPfLimit, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );	


		sEndure = 0;
		sResume = 0;
		sDefence = 0;
		sResist = 0;
		sMinAttack = 0;
		sMaxAttack = 0;
		sDistance = 0;
		sTime = 0;
		sScope = 0;
		sCapacity = 0;
		sSupply = 0;			
		sConsume = 0;		
		sCannonSpeed = 0;	
		sSpeed = 0;
		sParam = 0;
	}
};

struct xShipPartInfo: public EntityData
{
	char szName[BOAT_MAXSIZE_NAME];	// 
	char szDesp[BOAT_MAXSIZE_DESP];	// 
    DWORD dwModel;			// ID
	USHORT sMotor[BOAT_MAXNUM_MOTOR];	// 
	DWORD  dwPrice;			// 

	USHORT sEndure;			// 
	USHORT sResume;			// 
	USHORT sDefence;		// 
	USHORT sResist;			// 
    USHORT sMinAttack;		// 
    USHORT sMaxAttack;		// 
	USHORT sDistance;		// 
	USHORT sTime;			// 
	USHORT sScope;			// 
	USHORT sCapacity;		// 
	USHORT sSupply;			// 
	USHORT sConsume;		// 
	USHORT sCannonSpeed;	// 
	USHORT sSpeed;			// 
	USHORT sParam;			// 
	
	xShipPartInfo()
	{
		memset( szName, 0, BOAT_MAXSIZE_NAME );
		memset( szDesp, 0, BOAT_MAXSIZE_DESP );
		dwModel = 0;
		memset( sMotor, 0, sizeof(USHORT)*BOAT_MAXNUM_MOTOR );
		dwPrice = 0;
		sEndure = 0;
		sResume = 0;
		sDefence = 0;
		sResist = 0;
		sMinAttack = 0;
		sMaxAttack = 0;
		sDistance = 0;
		sTime = 0;
		sScope = 0;
		sCapacity = 0;
		sSupply = 0;			
		sConsume = 0;		
		sCannonSpeed = 0;	
		sSpeed = 0;
		sParam = 0;
	}
};

// GetShipInfo() / GetShipPartInfo() определены в сторах
#include "ShipRecordStore.h"
#include "ShipPartRecordStore.h"
