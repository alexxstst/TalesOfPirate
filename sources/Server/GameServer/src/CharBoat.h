// CharBoat.h created by knight 2005.4.18
//---------------------------------------------------------

#include "Character.h"

//---------------------------------------------------------

namespace mission
{
	struct BOAT_SYNC_ATTR
	{
		char szName[BOAT_MAXSIZE_NAME];	//
		USHORT	sBoatID;
		BYTE	byHeader;
		BYTE	byEngine;
		BYTE	byCannon;
		BYTE	byEquipment;
		USHORT	sHeader;
		USHORT	sEngine;
		USHORT	sCannon;
		USHORT	sEquipment;
	};

	class CCharBoat
	{
	public:
		CCharacter* SummonBoat( USHORT sBoatID );

		//
		BOOL	LoadBoat( CCharacter& owner, char chType );
		BOOL	CreateBoat( CCharacter& owner, DWORD dwBoatID, char chType );
		BOOL	Create( CCharacter& owner, USHORT sBoatID, USHORT sBerthID );
		void	Cancel( CCharacter& owner );
		BOOL	Update( CCharacter& owner, const net::msg::CmUpdateBoatMessage& msg );
		BOOL	MakeBoat( CCharacter& owner, const net::msg::CmCreateBoatMessage& msg );
		void	GetBerthName( USHORT sBerthID, char szBerth[], USHORT sLen );
		BOOL	GetBoatInfo( CCharacter& owner, DWORD dwBoatID );
		BOOL	GetTradeBoatInfo( CCharacter& viewer, CCharacter& owner, DWORD dwBoatID );
		BOOL	BoatPfLimit( CCharacter& owner, USHORT sBoatID );
		BOOL	BoatLvLimit( CCharacter& owner, USHORT sBoatID );
		BOOL	BoatLimit( CCharacter& owner, USHORT sBoatID );

	private:
		void	UpdateBoat( const BOAT_DATA& Data );
		BOOL	SyncAttr( CCharacter& owner, DWORD dwBoatID, USHORT sCmd, USHORT sBerthID,
					const BOAT_SYNC_ATTR& AttrInfo );
		BOOL	GetData( CCharacter& owner, BYTE byIsUpdate, const BOAT_DATA& Info, xShipAttrInfo& Data );
		BOOL	SetPartData( CCharacter& boat, USHORT sTypeID, const BOAT_DATA& AttrInfo );
		BOOL	SetBoatAttr( CCharacter& owner, CCharacter& boat, const xShipInfo& ShipInfo,
					const BOAT_DATA& Data, bool bFromFile = true, bool bLoadState = false );
	};

}

//
extern mission::CCharBoat g_CharBoat;
