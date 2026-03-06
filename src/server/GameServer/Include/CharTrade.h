// CharTrade.h Created by knight-gongjian 2004.12.7.
//---------------------------------------------------------
#pragma once

#ifndef _CHARTRADE_H_
#define _CHARTRADE_H_


#include "Character.h"
//---------------------------------------------------------
_DBC_USING

namespace mission
{
	typedef struct _TRADE_DATA
	{
		struct TRADE_ITEM_DATA
		{
			BYTE byType : 2;
			BYTE byIndex: 6;
			BYTE byCount;
			USHORT sItemID;			
		};
		TRADE_ITEM_DATA ItemArray[ROLE_MAXNUM_TRADEDATA];
		BYTE  byItemCount;						// 
		DWORD dwMoney;							// 
		DWORD dwIMP;

	} TRADE_DATA, *PTRADE_DATA;

	class CTradeData : public dbc::PreAllocStru
	{
	public:
		CTradeData(dbc::uLong lSize);
		virtual ~CTradeData();

		void Clear()
		{
			pRequest	= NULL;
			pAccept		= NULL;
			byValue		= 0;

			memset( &ReqTradeData, 0, sizeof(TRADE_DATA) );
			memset( &AcpTradeData, 0, sizeof(TRADE_DATA) );
		}

		CCharacter *pRequest, *pAccept;
		union
		{
			struct
			{
				BYTE  bTradeStart : 1;		// 
				BYTE  bReqTrade : 1;		// 
				BYTE  bAcpTrade : 1;		// 
				BYTE  bReqOk : 1;			// 
				BYTE  bAcpOk : 1;			// 
				BYTE  byParam : 3;			// 
			};
			BYTE byValue;
		};
		//USHORT sxPos, syPos;		// 
		TRADE_DATA ReqTradeData;	// 
		TRADE_DATA AcpTradeData;	// 
		
		DWORD dwTradeTime;			// ()
	};

	class CTradeSystem
	{
	public:
		CTradeSystem();
		~CTradeSystem();

		// 
		BOOL Request( BYTE byType, CCharacter& character, DWORD dwAcceptID );
		BOOL Accept( BYTE byType, CCharacter& character,  DWORD dwRequestID );
		BOOL Cancel( BYTE byType, CCharacter& character,  DWORD dwCharID );
		
		// 
		BOOL Clear( BYTE byType, CCharacter& character );

		// 
		BOOL ValidateItemData( BYTE byType, CCharacter& character, DWORD dwCharID );
		BOOL ValidateTrade( BYTE byType, CCharacter& character, DWORD dwCharID );

		// 
		BOOL AddItem( BYTE byType, CCharacter& character, DWORD dwCharID, BYTE byOpType, BYTE byIndex, BYTE byItemIndex, BYTE byCount );
		BOOL AddMoney( BYTE byType, CCharacter& charactar, DWORD dwCharID, BYTE byOpType, DWORD dwMoney );
		BOOL AddIMP(BYTE byType, CCharacter& charactar, DWORD dwCharID, BYTE byOpType, DWORD dwMoney);
		BOOL IsTradeDist( CCharacter& Char1, CCharacter& Char2, DWORD dwDist );
	private:
		void ResetItemState( CCharacter& character, CTradeData& TradeData );

	};

}

extern mission::CTradeSystem g_TradeSystem;

//---------------------------------------------------------

#endif // _CHARTRADE_H_
