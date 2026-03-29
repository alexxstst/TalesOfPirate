#include "StdAfx.h"
#include "uiitemcommand.h"
#include "ItemRecord.h"
#include "uicompent.h"
#include "uigoodsgrid.h"
#include "Character.h"
#include "GameApp.h"
#include "uifastcommand.h"
#include "PacketCmd.h"
#include "CommFunc.h"
#include "uiequipform.h"
#include <strstream> 
#include "StringLib.h"
#include "SkillRecord.h"
#include "uiboatform.h"
#include "shipset.h"
#include "itempreset.h"
#include "stpose.h"
#include "stnpctalk.h"
#include "stoneset.h"
#include "ItemRefineSet.h"
#include "ItemRefineEffectSet.h"
#include "elfskillset.h"
#include "STAttack.h"
#include "STMove.h"

#include "uibankform.h"
#include "isskilluse.h"
#include "smallmap.h"

#include "UIDoublePwdForm.h"
#include "UIStoreForm.h"
#include "UIGlobalVar.h"
#include "UIMakeEquipForm.h"
#include <ctime>
#include "time.h"
using namespace std;
using namespace GUI;
//---------------------------------------------------------------------------
// class CItemCommand
//---------------------------------------------------------------------------
static char buf[256] = { 0 };

const DWORD VALID_COLOR = COLOR_RED;
const DWORD GENERIC_COLOR = COLOR_WHITE;
const DWORD ADVANCED_COLOR = 0xFF9CCFFF;
const DWORD GLOD_COLOR = 0xFFFFFF00;

const unsigned int ITEM_HEIGHT = 32;
const unsigned int ITEM_WIDTH = 32;

std::map<int, DWORD>	CItemCommand::_mapCoolDown;	// 


CItemCommand::CItemCommand( CItemRecord* pItem )
: _pItem(pItem), _dwColor(COLOR_WHITE), _pBoatHint(NULL),
_pAniClock(NULL), _pSkill(NULL), _dwPlayTime(0),_canDrag(true)
{
    if( !_pItem )  ToLogService("errors", LogLevel::Error, "CItemCommand::CItemCommand(CItemRecord* pItem) pItem is NULL");

    _pImage = new CGuiPic;

    const char* file = pItem->GetIconFile();

    // 
    HANDLE hFile = CreateFile(file,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if( INVALID_HANDLE_VALUE == hFile ) 
    {
        _pImage->LoadImage( "texture/icon/error.png", ITEM_WIDTH, ITEM_HEIGHT, 0 );
    }
    else
    {
        CloseHandle(hFile);

        _pImage->LoadImage( file, ITEM_WIDTH, ITEM_HEIGHT, 0 );
    }

	int nSkillID = 0;
	if ( _pItem->sType == 71 )
	{
		nSkillID = atoi(GetItemInfo()->szAttrEffect);
		_pSkill = GetSkillRecordInfo(nSkillID);
		if ( !_pSkill || !_pSkill->GetIsUse() )
			_pSkill = 0;
 	}

    memset( &_ItemData, 0, sizeof(_ItemData) );
	_ItemData.sID = (short)pItem->lID;
	_ItemData.SetValid();

	_nPrice = _pItem->lPrice;
}

CItemCommand::CItemCommand( const CItemCommand& rhs )
: _pImage( new CGuiPic(*rhs._pImage) ), _pBoatHint(NULL),
_pAniClock(NULL), _dwPlayTime(0)
{
    _Copy( rhs );
}

CItemCommand& CItemCommand::operator=( const CItemCommand& rhs )
{
    *_pImage = *rhs._pImage;
	_pAniClock = NULL;
    _Copy( rhs );
    return *this;
}

void CItemCommand::_Copy( const CItemCommand& rhs )
{
    memcpy( &_ItemData, &rhs._ItemData, sizeof(_ItemData) );
	SetBoatHint( rhs._pBoatHint );

	_pItem = rhs._pItem;
	_pSkill = rhs._pSkill;
	_dwColor = rhs._dwColor;
	_nPrice = rhs._nPrice;
}

CItemCommand::~CItemCommand()
{
    //delete _pImage;
	SAFE_DELETE(_pImage); // UI

	if( _pBoatHint )
	{
		delete _pBoatHint;
		_pBoatHint = NULL;
	}
}

void CItemCommand::PUSH_HINT( const char* str, int value, DWORD color )
{
	if( value==0 ) return;

    sprintf( buf, str, value );
    PushHint( buf, color );
}

void CItemCommand::SaleRender( int x, int y, int nWidth, int nHeight )
{
	static int nX, nY;
    static int w, h;
	nX = x + ( nWidth - ITEM_WIDTH ) / 2;
	nY = y + ( nHeight -  ITEM_HEIGHT ) / 2;
	int xOffset = 25;
	int yOffset = 25;
	
	_pImage->Render( nX, nY, _ItemData.IsValid() ? _dwColor : (DWORD)0xff757575);

	short sType = _pItem->sType;
	// render gem level on icon
	if( sType == 49 || sType == 50){
		static SItemHint item;
		memset( &item, 0, sizeof(SItemHint) );
		item.Convert( _ItemData, _pItem );
		sprintf(buf, "Lv%d", item.sEnergy[1] );
		static int w, h;
		CGuiFont::s_Font.GetSize( buf, w, h );
		GetRender().FillFrame( x+xOffset, y+h+8+yOffset, x + w+xOffset, y + h+ h + 8+yOffset, 0xE0ADF6F7 );
        CGuiFont::s_Font.Render( buf, x+xOffset, y+h+8+yOffset, COLOR_BLACK );
	}
	
	bool renderText = false;
	if( _ItemData.sNum>1 ){
		sprintf(buf, "%d", _ItemData.sNum );
		renderText = true;
	}else if(_ItemData.sEndure[1] == 25000 && _ItemData.sEnergy[1] == 0){
		//render "App" on apparels.
		sprintf(buf, "App" );
		renderText = true;
	}else if(_ItemData.sEndure[1] < 25000 && _ItemData.sEnergy[1] >= 1000 && sType != 59 && sType < 29  ){
		//render "EQP" on items.
		sprintf(buf, "EQP" );
		renderText = true;
	}else if( (sType < 29 && sType != 12 && sType != 13 && sType != 17 && sType != 18 && sType != 19 /*&& sType != 20*/ && sType != 21) || (sType == 81 && sType == 82 && sType == 83) )
	{// rendering forge level on icons
		SItemForge& Forge = GetForgeInfo();
		if( Forge.IsForge ){
			if( Forge.nLevel>0 ){
				sprintf(buf, "+%d", Forge.nLevel );
				renderText = true;
			}
		}
	}else if( sType == 59 && _ItemData.sEndure[1] !=25000 )
	{// render fairy level on icon
		static SItemHint item;
		memset( &item, 0, sizeof(SItemHint) );
		item.Convert( _ItemData, _pItem );

		int nLevel = item.sInstAttr[ITEMATTR_VAL_STR]
					+ item.sInstAttr[ITEMATTR_VAL_AGI] 
					+ item.sInstAttr[ITEMATTR_VAL_DEX] 
					+ item.sInstAttr[ITEMATTR_VAL_CON] 
					+ item.sInstAttr[ITEMATTR_VAL_STA];
					
		sprintf(buf, "Lv%d", nLevel );
		renderText = true;
	}
	
	if(renderText){
		static int w, h;
		static int xNum, yNum;
		CGuiFont::s_Font.GetSize( buf, w, h );
		xNum = nX + ITEM_WIDTH - w;
		yNum = nY + ITEM_HEIGHT - h;
		GetRender().FillFrame( xNum, yNum, xNum + w, yNum + h, 0xE0ADF6F7 );
		CGuiFont::s_Font.Render( buf, xNum, yNum, COLOR_BLACK );
	}
	
	

	CGuiFont::s_Font.GetSize( _pItem->szName, w, h );	
	if( w > nWidth )
	{
		static char szBuf1[128] = { 0 };
		static char szBuf2[128] = { 0 };
		static int nEnter = 0;
		strncpy( szBuf1, _pItem->szName, sizeof(szBuf1) );
		nEnter = (int)strlen( szBuf1 ) / 2;
		if( _ismbslead( (unsigned char*)szBuf1, (unsigned char*)&szBuf1[nEnter] ) )
		{
			nEnter--;
		}
		if( nEnter<0 ) return;

		nEnter++;
		szBuf1[nEnter] = '\0';
		strncpy( szBuf2, &_pItem->szName[nEnter], sizeof(szBuf2) );

		CGuiFont::s_Font.GetSize( szBuf1, w, h );	
		CGuiFont::s_Font.Render( szBuf1, x + ( nWidth - w ) / 2, nY - h - h - 2, COLOR_BLACK );

		CGuiFont::s_Font.GetSize( szBuf2, w, h );	
		CGuiFont::s_Font.Render( szBuf2, x + ( nWidth - w ) / 2, nY - h - 2, COLOR_BLACK );
	}
	else
	{
		CGuiFont::s_Font.Render( _pItem->szName, x + ( nWidth - w ) / 2, nY - h - 2, COLOR_BLACK );
	}

	if(_nPrice > 2000000000){
		int price = _nPrice - 2000000000;
		int quantity = price/100000;
		int itemID = price - (quantity*100000);
		CItemRecord* pInfo = GetItemRecordInfo( itemID );
		if (pInfo){
			sprintf( buf, "%dx %s", quantity,pInfo->szName );
		}else{
			sprintf( buf, "%dx Invalid ID [ %d]", quantity,itemID);
		}
	}else{
		sprintf( buf, "$%s", StringSplitNum(_nPrice) );
	}
	CGuiFont::s_Font.GetSize( buf, w, h );
	CGuiFont::s_Font.Render( buf, x + ( nWidth - w ) / 2, nY + ITEM_HEIGHT + 2, COLOR_BLACK );
}

void CItemCommand::Render( int x, int y )
{
	if ((_ItemData.expiration - std::time(0)) <= 0 && _ItemData.expiration != 0 && _ItemData.sID != 32767) {
	    _pImage->Render(x, y, (DWORD) 0xff757575);
	} else {
	    _pImage->Render(x, y, _ItemData.IsValid() ? _dwColor : (DWORD) 0xff757575);
	}

    if( _pAniClock )
    {
        _pAniClock->Render(x, y);
        if( (CGameApp::GetCurTick()>_dwPlayTime) || _pAniClock->IsEnd() )
        {
            _pAniClock = NULL;
        }
    }
	
	short sType = _pItem->sType;
	// render gem level on icon
	if( sType == 49 || sType == 50){
		static SItemHint item;
		memset( &item, 0, sizeof(SItemHint) );
		item.Convert( _ItemData, _pItem );

		sprintf(buf, "Lv%d", item.sEnergy[1] );
		static int w, h;
		CGuiFont::s_Font.GetSize( buf, w, h );
	
		GetRender().FillFrame( x, y+h+8, x + w, y + h+ h + 8, 0xE0ADF6F7 );
        CGuiFont::s_Font.Render( buf, x, y+h+8, COLOR_BLACK );
	}
		
	if( _ItemData.dwDBID )
    {
		sprintf(buf, "Lock");
        static int w, h;
        CGuiFont::s_Font.GetSize( buf, w, h );

		GetRender().FillFrame( x, y, x + w, y + h, 0xAA000000 );
		CGuiFont::s_Font.Render( buf, x, y, 0xFFFFA500 );
    }
	
    if( _ItemData.sNum>1 )
    {
        sprintf(buf, "%d", _ItemData.sNum );
        static int w, h;
        CGuiFont::s_Font.GetSize( buf, w, h );

		x += ITEM_WIDTH - w;
		y += ITEM_HEIGHT - h;
		GetRender().FillFrame( x, y, x + w, y + h, 0xE0ADF6F7 );
        CGuiFont::s_Font.Render( buf, x, y, COLOR_BLACK );
    }

	// rendering forge level on icons
	if( (sType < 29 && sType != 12 && sType != 13 && sType != 17 && sType != 18 && sType != 19 /*&& sType != 20*/ && sType != 21) || (sType == 81 && sType == 82 && sType == 83) )
	{
		SItemForge& Forge = GetForgeInfo();
		if( Forge.IsForge )
		{
			if( Forge.nLevel>0 )
			{
				sType != 26 && sType != 25 ? sprintf(buf, "EQP+%d", Forge.nLevel ):sprintf(buf, "+%d", Forge.nLevel );
				static int w, h;
				CGuiFont::s_Font.GetSize( buf, w, h );

				x += ITEM_WIDTH - w;
				y += ITEM_HEIGHT - h;
				GetRender().FillFrame( x, y, x + w, y + h, 0xE0ADF6F7 );
				CGuiFont::s_Font.Render( buf, x, y, COLOR_BLACK );
			}
		}
		/*
		//render hammer on near-broken equips
		if(_ItemData.sEndure[0] <= 0.1*_ItemData.sEndure[1]){
			CGuiPic* repairIcon = new CGuiPic;
			repairIcon->LoadImage( "Texture/UI/Corsairs/Repair.png", 32, 32, 0, 0, 0, 0.8f, 0.8f );
			repairIcon->Render(x+8,y+8);
		}*/
			//render "Eqp" on Eqp. Forge.nLevel
		if(_ItemData.sEndure[1] < 25000 && _ItemData.sEnergy[1] >= 1000 && sType < 29 && sType != 59 && sType != 25 && sType != 26   && Forge.nLevel == 0 ){
			sprintf(buf, "EQP" );
			static int w, h;
			CGuiFont::s_Font.GetSize( buf, w, h );
			x += ITEM_WIDTH - w;
			y += ITEM_HEIGHT - h;
			GetRender().FillFrame( x, y, x + w, y + h, 0xE0ADF6F7 );
			CGuiFont::s_Font.Render( buf, x, y, COLOR_BLACK );
		}

	}

	// render fairy level on icon
	if( sType == 59 && _ItemData.sEndure[1] != 25000  )
	{
		static SItemHint item;
		memset( &item, 0, sizeof(SItemHint) );
		item.Convert( _ItemData, _pItem );

		int nLevel = item.sInstAttr[ITEMATTR_VAL_STR]
					+ item.sInstAttr[ITEMATTR_VAL_AGI] 
					+ item.sInstAttr[ITEMATTR_VAL_DEX] 
					+ item.sInstAttr[ITEMATTR_VAL_CON] 
					+ item.sInstAttr[ITEMATTR_VAL_STA];
					
		sprintf(buf, "Lv%d", nLevel );
		static int w, h;
		CGuiFont::s_Font.GetSize( buf, w, h );

		x += ITEM_WIDTH - w;
		y += ITEM_HEIGHT - h;
		GetRender().FillFrame( x, y, x + w, y + h, 0xE0ADF6F7 );
		CGuiFont::s_Font.Render( buf, x, y, COLOR_BLACK );
	}

	//render "App" on apparels.
	if(_ItemData.sEndure[1] == 25000 && _ItemData.sEnergy[1] == 0){
		sprintf(buf, "App" );
		static int w, h;
		CGuiFont::s_Font.GetSize( buf, w, h );
		x += ITEM_WIDTH - w;
		y += ITEM_HEIGHT - h;
		GetRender().FillFrame( x, y, x + w, y + h, 0xE0ADF6F7 );
		CGuiFont::s_Font.Render( buf, x, y, COLOR_BLACK );
	}
}

void CItemCommand::OwnDefRender( int x, int y, int nWidth, int nHeight )
{
	static int nX, nY;
    static int w, h;
	nX = x + ( nWidth - ITEM_WIDTH ) / 2;
	nY = y + ( nHeight -  ITEM_HEIGHT ) / 2;

	_pImage->Render( nX, nY, _ItemData.IsValid() ? _dwColor : (DWORD)0xff757575 );

	if( _ItemData.sNum>=0 )
	{
		static int xNum, yNum;
        sprintf(buf, "%d", _ItemData.sNum );
        CGuiFont::s_Font.GetSize( buf, w, h );

		xNum = nX + ITEM_WIDTH - w;
		yNum = nY + ITEM_HEIGHT - h;
		GetRender().FillFrame( xNum, yNum, xNum + w, yNum + h, 0xE0ADF6F7 );
		CGuiFont::s_Font.Render( buf, xNum, yNum, COLOR_BLACK );
	}

	CGuiFont::s_Font.GetSize( _pItem->szName, w, h );	
	if( w > nWidth )
	{
		static char szBuf1[128] = { 0 };
		static char szBuf2[128] = { 0 };
		static int nEnter = 0;
		strncpy( szBuf1, _pItem->szName, sizeof(szBuf1) );
		nEnter = (int)strlen( szBuf1 ) / 2;
		if( _ismbslead( (unsigned char*)szBuf1, (unsigned char*)&szBuf1[nEnter] ) )
		{
			nEnter--;
		}
		if( nEnter<0 ) return;

		nEnter++;
		szBuf1[nEnter] = '\0';
		strncpy( szBuf2, &_pItem->szName[nEnter], sizeof(szBuf2) );

		CGuiFont::s_Font.GetSize( szBuf1, w, h );	
		CGuiFont::s_Font.Render( szBuf1, x + ( nWidth - w ) / 2, nY - h - h - 2, COLOR_BLACK );

		CGuiFont::s_Font.GetSize( szBuf2, w, h );	
		CGuiFont::s_Font.Render( szBuf2, x + ( nWidth - w ) / 2, nY - h - 2, COLOR_BLACK );
	}
	else
	{
		CGuiFont::s_Font.Render( _pItem->szName, x + ( nWidth - w ) / 2, nY - h - 2, COLOR_BLACK );
	}

	CGuiFont::s_Font.GetSize( _OwnDefText.c_str(), w, h );
	CGuiFont::s_Font.Render ( _OwnDefText.c_str(), x + ( nWidth - w ) / 2, nY + ITEM_HEIGHT + 2, COLOR_BLACK );
}

void CItemCommand::RenderEnergy(int x, int y)
{
	if( _pItem->sType==29 && _ItemData.sEnergy[1]!=0 )
	{
		float fLen = (float)_ItemData.sEnergy[0] / (float)_ItemData.sEnergy[1] * (float)ITEM_HEIGHT;
		int yb = y+ITEM_HEIGHT;
		GetRender().FillFrame( x, y, x+2, yb, COLOR_BLUE );
		GetRender().FillFrame( x, yb-(int)fLen, x+2, yb, COLOR_RED );
	}
}

void CItemCommand::_AddDescriptor()
{
	StringNewLine( buf, 40, _pItem->szDescriptor, (unsigned int)strlen(_pItem->szDescriptor) );
    PushHint( buf );
}

void CItemCommand::AddHint( int x, int y ){
	bool isMain = false;

	if (GetParent()) {
		std::string name = GetParent()->GetForm()->GetName();
		if (name == "frmPlayertrade" || name == "frmItem" || name == "frmNPCstorage" || name == "frmTempBag" ||
			name == "frmBreak" || name == "frmCooking" || name == "frmFound" || name == "frmBreak" ||
			name == "frmStore" || name == "frmViewAll" ||
			name == "frmSpiritMarry" || name == "frmSpiritErnie" || name == "frmEquipPurify") {
			isMain = true;
		}
	}
	bool isStore = false;
	if( GetParent() )
	{
		string name = GetParent()->GetForm()->GetName(); 
		if( name == "frmStore" ) 
		{
			isStore = true;
		}
	}

    SGameAttr* pAttr = NULL;
	if( g_stUIBoat.GetHuman() )
    {
        pAttr = g_stUIBoat.GetHuman()->getGameAttr();
	}
    if( !pAttr ) return;

    SetHintIsCenter( true );

	static SItemHint item;
	memset( &item, 0, sizeof(SItemHint) );
	CItemRecord* pEquipItem(0);		
	item.Convert( _ItemData, _pItem );
	
	if(_ItemData.sEndure[1] == 25000 && _ItemData.sEnergy[1] == 0){
		PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );
		_ShowBody();
		return;
	}
	
	bool isWeapon = (_pItem->sType >= 1 && _pItem->sType <= 10);
	bool isDefenceType = (_pItem->sType == 22 || _pItem->sType == 11 || _pItem->sType == 27);
	bool isJewelery = (_pItem->sType == 25 || _pItem->sType == 26 || _pItem->sType == 81 || _pItem->sType == 82 || _pItem->sType == 83);
	bool isEquip = (_pItem->sType == 20 || _pItem->sType == 23 || _pItem->sType == 24 || _pItem->sType == 88 || _pItem->sType == 84);
	if (isWeapon || isDefenceType || isJewelery || isEquip) {
		if ( _ItemData.GetItemLevel() > 0 ){
			sprintf( buf, "Lv%d %s",_ItemData.GetItemLevel(), GetName());
		}else{
			sprintf( buf, "%s", GetName() );
		}
		PushHint( buf, COLOR_WHITE, 5, 1 ); //0xFF000000
		//PushHint( buf, (DWORD)(COLOR_WHITE ^ 0xFF000000), 5, 1, -1, true, -16777216);
		
		if( _pItem->lID == 1034 ){
			sprintf(buf, g_oLangRec.GetString(862), _ItemData.sEndure[0] * 10 - 1000, _ItemData.sEndure[1] * 10 - 1000);// 
			PushHint( buf, COLOR_WHITE, 5, 1 );
			//return;
		}
		
		if( _pItem->sType==2 ){
			PushHint( g_oLangRec.GetString(624), COLOR_WHITE, 5, 1 );
		}
		
        AddHintHeight();

		if(isWeapon){
			sprintf( buf, g_oLangRec.GetString(625), _GetValue( ITEMATTR_VAL_MNATK, item ), _GetValue( ITEMATTR_VAL_MXATK, item ) );
			PushHint( buf, GENERIC_COLOR );
		}else if(isDefenceType || isEquip){
			_PushValue( g_oLangRec.GetString(629), ITEMATTR_VAL_DEF, item );
		}
		
		
		
		if(! isStore && !isJewelery){
			sprintf( buf, g_oLangRec.GetString(626), item.sEndure[0], item.sEndure[1] );
			PushHint( buf, GENERIC_COLOR );
		}
		
		if(isDefenceType){
			_PushValue( g_oLangRec.GetString(630), ITEMATTR_VAL_PDEF, item );
		}else if(_pItem->sType == 23){
			_PushValue( g_oLangRec.GetString(631), ITEMATTR_VAL_HIT, item );
		}else if(_pItem->sType == 24){
			_PushValue( g_oLangRec.GetString(632), ITEMATTR_VAL_FLEE, item );
		}
		
		
		//if ( _ItemData.GetItemLevel() > 0 ){
		if (_pItem->nID != 1034) {
			sprintf(buf, "Effectiveness (%d%%)", _ItemData.GetItemLevel());
			PushHint(buf, GENERIC_COLOR);
		}
		//}

        AddHintHeight();

		
		if (_ItemData.sNeedLv)
			PUSH_HINT( g_oLangRec.GetString(628), _ItemData.sNeedLv, pAttr->get(ATTR_LV)>=_ItemData.sNeedLv ? GENERIC_COLOR : VALID_COLOR );
		else
			PUSH_HINT( g_oLangRec.GetString(628), _pItem->sNeedLv, pAttr->get(ATTR_LV)>=_pItem->sNeedLv ? GENERIC_COLOR : VALID_COLOR );
		
		_ShowBody();
		_ShowWork( _pItem, pAttr );
	
    }else if(_pItem->sType == 67 || _pItem->sType == 59  )
	{
		int nLevel = item.sInstAttr[ITEMATTR_VAL_STR]
					+ item.sInstAttr[ITEMATTR_VAL_AGI] 
					+ item.sInstAttr[ITEMATTR_VAL_DEX] 
					+ item.sInstAttr[ITEMATTR_VAL_CON] 
					+ item.sInstAttr[ITEMATTR_VAL_STA];

		sprintf( buf, "Lv%d %s", nLevel, GetName() );
        PushHint( buf, COLOR_WHITE, 5, 1 );

		AddHintHeight();

		PUSH_HINT( g_oLangRec.GetString(657), item.sInstAttr[ITEMATTR_VAL_STR] );
		PUSH_HINT( g_oLangRec.GetString(658), item.sInstAttr[ITEMATTR_VAL_AGI] );
		PUSH_HINT( g_oLangRec.GetString(659), item.sInstAttr[ITEMATTR_VAL_CON] );
		PUSH_HINT( g_oLangRec.GetString(660), item.sInstAttr[ITEMATTR_VAL_DEX] );
		PUSH_HINT( g_oLangRec.GetString(661), item.sInstAttr[ITEMATTR_VAL_STA] );

		item.sInstAttr[ITEMATTR_VAL_STR] = 0;
		item.sInstAttr[ITEMATTR_VAL_AGI] = 0;
		item.sInstAttr[ITEMATTR_VAL_DEX] = 0;
		item.sInstAttr[ITEMATTR_VAL_CON] = 0;
		item.sInstAttr[ITEMATTR_VAL_STA] = 0;

		AddHintHeight();

		if(! isStore)	// 
		{
			sprintf( buf, g_oLangRec.GetString(662), _ItemData.sEndure[0] / 50, _ItemData.sEndure[1] / 50 );
			PushHint( buf );

			sprintf( buf, g_oLangRec.GetString(663), _ItemData.sEnergy[0], _ItemData.sEnergy[1] );
			PushHint( buf );
		}

		AddHintHeight();

		// ,
		AddHintHeight();
		for( int i=0; i<ITEMATTR_CLIENT_MAX; i++ )
		{
			if( item.sInstAttr[i]!=0 )
			{
				_PushItemAttr( i, item, ADVANCED_COLOR );
			}
		}

		int array[3][2]= { 0 };
		g_pGameApp->GetScriptMgr()->DoString( "GetElfSkill", "u-dddddd", _ItemData.lDBParam[0]
					, &array[0][0], &array[0][1]
					, &array[1][0], &array[1][1]
					, &array[2][0], &array[2][1]
				);

		CElfSkillInfo* pInfo = NULL;
		for( int i=0; i<3; i++ )
		{
			pInfo = GetElfSkillInfo( array[i][0], array[i][1] );
			if( pInfo )
			{
				PushHint( pInfo->szDataName );				
			}
		}
		_AddDescriptor();

		if ( _ItemData.dwDBID )
		{
			PushHint( "Locked", (DWORD)(0xFFA500 ^ 0xFF000000), 5, 1, -1, true, -16777216);
		}

		if(_ItemData.expiration != 0){
		char buf [64];
		char buf2[64];
		DWORD color;
		const time_t timeNow = std::time(0);
		time_t timedif = timeNow - _ItemData.expiration;
		time_t seconds = abs(timedif%86400);
		int days = abs(timedif/86400);
	
		if(_ItemData.expiration){
		if(timedif < 0 && days >= 1){
			strftime(buf, sizeof(buf), "%H:%M:%S", gmtime(&seconds));
			sprintf(buf2, "%d day(s), %s remaining", days, buf);
			color = COLOR_GREEN;
		}
		else if(timedif < 0 && days < 1){
			strftime(buf, sizeof(buf), "%H:%M:%S", gmtime(&seconds));
			sprintf(buf2, "%s remaining", buf);
			color = COLOR_ORANGE;
		}
		else if(timedif >= 0){
			sprintf(buf2, "Item expired");
			color = COLOR_RED;
		}
		}
		
		PUSH_HINT(buf2, -1, color);

	}



		return;
	}
    
    else if( _pItem->sType>=31 && _pItem->sType<=33 )
    {
		

        SetHintIsCenter( false );

        PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

		if(5786 == _pItem->lID || 5787 == _pItem->lID || 5788 == _pItem->lID || 5789 == _pItem->lID)
		{
			sprintf( buf, g_oLangRec.GetString(644),  item.sEndure[0],  item.sEndure[1]);
			PushHint( buf );
		}

		if( _ItemData.sNum>0 )
		{
			AddHintHeight();
			sprintf( buf, g_oLangRec.GetString(633), _ItemData.sNum );
			PushHint( buf );
		}

		_AddDescriptor();

    }
	 
    else if( _pItem->sType==42 )    // 
    {
        PushHint( _pItem->szName, COLOR_BLUE, 5, 1 );

        SetHintIsCenter( false );
		_AddDescriptor();
    }
    else if( _pItem->sType==18 || _pItem->sType==19 )	// ,
    {
        PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

		if( _pItem->nID==3908 || _pItem->nID==3108 ) // 3108 add by Philip  2005-05-30
		{
			sprintf( buf, g_oLangRec.GetString(626), item.sEndure[0], item.sEndure[1] );
			PushHint( buf, GENERIC_COLOR );
		}

		if ( _ItemData.GetItemLevel() > 0 ){
			sprintf( buf, g_oLangRec.GetString(627), _ItemData.GetItemLevel() * 2 + 100 );	//  0-80% 1-82% ...
			PushHint( buf, GENERIC_COLOR );
		}

		PUSH_HINT( g_oLangRec.GetString(628), _pItem->sNeedLv, pAttr->get(ATTR_LV)>=_pItem->sNeedLv ? GENERIC_COLOR : VALID_COLOR );

		_AddDescriptor();
    }
	else if( _pItem->sType==43 )    // 
	{
        PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

		if( _pBoatHint )
		{
			PushHint( _pBoatHint->szName );

			xShipInfo* pInfo = GetShipInfo( _pBoatHint->sBoatID );

			if( pInfo )
			{
				int nNeedLv = pInfo->sLvLimit;
				if( nNeedLv>0 )
				{
					sprintf( buf, g_oLangRec.GetString(628), nNeedLv );
					PushHint( buf, g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_LV)>=nNeedLv ? GENERIC_COLOR : VALID_COLOR );
				}
			}

			sprintf( buf, g_oLangRec.GetString(634), _pBoatHint->sLevel );
			PushHint( buf );
			
			sprintf( buf, g_oLangRec.GetString(635), _pBoatHint->dwExp );
			PushHint( buf );

			AddHintHeight();

			sprintf( buf, g_oLangRec.GetString(636), _pBoatHint->dwHp,(int)(_pBoatHint->dwMaxHp) );
			PushHint( buf );

			if( pInfo )
			{
				_ShowWork( pInfo, g_stUIBoat.GetHuman()->getGameAttr() );
			}

			sprintf( buf, g_oLangRec.GetString(637), _pBoatHint->dwSp, (int)(_pBoatHint->dwMaxSp) );
			PushHint( buf );

			sprintf( buf, g_oLangRec.GetString(638), _pBoatHint->dwMinAttack,(int)(_pBoatHint->dwMaxAttack) );
			PushHint( buf );

			sprintf( buf, g_oLangRec.GetString(639), _pBoatHint->dwDef );
			PushHint( buf );

			sprintf( buf, g_oLangRec.GetString(640), _pBoatHint->dwSpeed );
			PushHint( buf );

			sprintf( buf, g_oLangRec.GetString(641), _pBoatHint->dwShootSpeed );
			PushHint( buf );

			sprintf( buf, g_oLangRec.GetString(642), _pBoatHint->byHasItem, _pBoatHint->byCapacity );
			PushHint( buf );	
			
			sprintf( buf, g_oLangRec.GetString(643), StringSplitNum(_pBoatHint->dwPrice / 2) );
			PushHint( buf );
		}
		else
		{
			CBoat* pBoat = g_stUIBoat.FindBoat( _ItemData.GetDBParam(enumITEMDBP_INST_ID) );
			if( pBoat )
			{
				CCharacter* pCha = pBoat->GetCha();
				PushHint( pCha->getName().c_str() );

				int nNeedLv = pCha->GetShipInfo()->sLvLimit;
				if( nNeedLv>0 )
				{
					sprintf( buf, g_oLangRec.GetString(628), nNeedLv );
					PushHint( buf, g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_LV)>=nNeedLv ? GENERIC_COLOR : VALID_COLOR );
				}

				SGameAttr* pAttr = pCha->getGameAttr();
				sprintf( buf, g_oLangRec.GetString(634), pAttr->get(ATTR_LV) );
				PushHint( buf );
				
				sprintf( buf, g_oLangRec.GetString(635), pAttr->get(ATTR_CEXP) );
				PushHint( buf );

				AddHintHeight();

				sprintf( buf, g_oLangRec.GetString(636), pAttr->get(ATTR_HP), pAttr->get(ATTR_MXHP) );
				PushHint( buf );

				_ShowWork( pCha->GetShipInfo(), g_stUIBoat.GetHuman()->getGameAttr() );

				sprintf( buf, g_oLangRec.GetString(637), pAttr->get(ATTR_SP), pAttr->get(ATTR_MXSP) );
				PushHint( buf );

				sprintf( buf, g_oLangRec.GetString(638), pAttr->get(ATTR_BMNATK), pAttr->get(ATTR_BMXATK) );
				PushHint( buf );

				sprintf( buf, g_oLangRec.GetString(639), pAttr->get(ATTR_BDEF) );
				PushHint( buf );

				sprintf( buf, g_oLangRec.GetString(640), pAttr->get(ATTR_BMSPD) );
				PushHint( buf );

				sprintf( buf, g_oLangRec.GetString(641), pAttr->get(ATTR_BASPD) );
				PushHint( buf );

				CGoodsGrid* pGoods = pBoat->GetGoodsGrid();
				sprintf( buf, g_oLangRec.GetString(642), pGoods->GetCurNum(), pGoods->GetMaxNum() );
				PushHint( buf );	

				sprintf( buf, g_oLangRec.GetString(643), StringSplitNum( pAttr->get(ATTR_BOAT_PRICE) / 2 ) );
				PushHint( buf );
			}
		}

		_AddDescriptor();
		return;
	}
	else if( _pItem->sType==29 )    // 
	{
        PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

		PUSH_HINT( g_oLangRec.GetString(628), _pItem->sNeedLv, pAttr->get(ATTR_LV)>=_pItem->sNeedLv ? GENERIC_COLOR : VALID_COLOR );

		_ShowWork( _pItem, pAttr );
	
        sprintf( buf, g_oLangRec.GetString(644), _ItemData.sEnergy[0], _ItemData.sEnergy[1] );
        PushHint( buf );

		_AddDescriptor();
	}
	else if( _pItem->sType==45 )    // 
	{
        PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

		PUSH_HINT( g_oLangRec.GetString(634), _ItemData.sEnergy[0] );
		
		float fB320 = (float)_ItemData.sEnergy[1];
		float fRate = 0.0f;
		if( _ItemData.sEnergy[1]==0 )
		{
			fRate = 30.0f;
		}
		else
		{
			fRate = max( 0.0f, 30 - pow( fB320, 0.5f ) ) + pow( fB320, -0.5f );

			if( fRate > 30.0f ) fRate = 30.0f;
			if( fRate < 0.0f ) fRate = 0.0f;
		}
		sprintf( buf, g_oLangRec.GetString(645), fRate );
		PushHint( buf );

	}
	else if( _pItem->sType==34 )	// 
	{
        PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

		CSkillRecord* pSkill = GetSkillRecordInfo( _pItem->szName );
		if( pSkill )
		{
			if( pSkill->chJobSelect[0][0]>=0 )	
			{
				std::ostrstream str;
				str << g_oLangRec.GetString(646);
				str << " ";

				for (char i = 0; i<defSKILL_JOB_SELECT_NUM; i++ )
				{
					if( pSkill->chJobSelect[i][0]<0 )
						break;

					if( i>0 && (i % 2)==0 )
					{
						str << g_oLangRec.GetString(647);
					}
					str << g_GetJobName( pSkill->chJobSelect[i][0] );
					str << " ";
				}
				str << '\0';
				
				PushHint( str.str(), pSkill->IsJobAllow( pAttr->get(ATTR_JOB) ) ? GENERIC_COLOR : VALID_COLOR );
			}

			if( pSkill->sLevelDemand!=-1 )
			{
				sprintf( buf, g_oLangRec.GetString(648), pSkill->sLevelDemand );
		        PushHint( buf, pAttr->get(ATTR_LV)>=pSkill->sLevelDemand ? GENERIC_COLOR : VALID_COLOR );
			}

			CSkillRecord* p = NULL;
			CSkillRecord* pSelfSkill = NULL;
			for( int i=0; i<defSKILL_PRE_SKILL_NUM; i++ )
			{
				if( pSkill->sPremissSkill[i][0] < 0 )
					break;

				p = GetSkillRecordInfo( pSkill->sPremissSkill[i][0] );
				if( p )
				{
					pSelfSkill = g_stUIEquip.FindSkill( p->nID );
					sprintf( buf, g_oLangRec.GetString(649), p->szName, pSkill->sPremissSkill[i][1] );
					if( pSelfSkill && pSelfSkill->GetSkillGrid().chLv >= pSkill->sPremissSkill[i][1] )
						PushHint( buf );
					else
						PushHint( buf, VALID_COLOR );
				}
			}
			SetHintIsCenter( false );
			StringNewLine( buf, 40, pSkill->szExpendHint, (unsigned int)strlen(pSkill->szExpendHint) );
    		PushHint( buf );
			StringNewLine( buf, 40, pSkill->szEffectHint, (unsigned int)strlen(pSkill->szEffectHint) );
    		PushHint( buf, ADVANCED_COLOR );
			StringNewLine( buf, 40, pSkill->szDescribeHint, (unsigned int)strlen(pSkill->szDescribeHint) );
    		PushHint( buf );
		}
		else
		{
			_AddDescriptor();
		}
	}
	else if( _pItem->sType == 46 )	// 
	{
        PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

		int show_attr[] = { ITEMATTR_VAL_STR, ITEMATTR_VAL_AGI, ITEMATTR_VAL_DEX, ITEMATTR_VAL_CON, ITEMATTR_VAL_STA };
		string show_text[] = { g_oLangRec.GetString(650), g_oLangRec.GetString(651), g_oLangRec.GetString(652), g_oLangRec.GetString(653), g_oLangRec.GetString(654) };
		int value = 0;
		const int count = sizeof(show_attr)/sizeof(show_attr[0]);
		for( int i=0; i<count; i++ )
		{
			value = item.sInstAttr[show_attr[i]];
			item.sInstAttr[show_attr[i]] = 0;
			sprintf( buf, "%s:%d", show_text[i].c_str(), value );
			PushHint( buf, GENERIC_COLOR );
		}

		sprintf( buf, g_oLangRec.GetString(655), _ItemData.sEndure[1] );
		PushHint( buf, GENERIC_COLOR );

		sprintf( buf, "%s:%d", g_oLangRec.GetString(848), _ItemData.sEnergy[1] );	// 
		PushHint( buf, GENERIC_COLOR );

		_AddDescriptor();
	}
	else if( _pItem->sType==49 )
	{
		CStoneInfo* pStoneInfo = NULL;
		DWORD color = -1;
		int nCount = CStoneSet::I()->GetLastID() + 1;
		for( int i=0; i<nCount; i++ )
		{
			pStoneInfo = ::GetStoneInfo( i );
			if( !pStoneInfo )
				continue;
			if( pStoneInfo->nItemID == _pItem->nID )
			{
				color = (DWORD)pStoneInfo->nItemRgb; //| 0xFF000000;
				break;
			}
		}

		sprintf( buf, g_oLangRec.GetString(656), ConvertNumToChinese( item.sEnergy[1] ).c_str(), _pItem->szName );
        //PushHint( buf, color, 5, 1, -1, true, -16777216);
		PushHint( buf, (DWORD)(color ^ 0xFF000000), 5, 1, -1, true, -16777216);
		PushHint( GetStoneHint(1).c_str() );	// 1
		_AddDescriptor();
	}
	else if( _pItem->sType==50 )
	{
		sprintf( buf, g_oLangRec.GetString(656), ConvertNumToChinese( item.sEnergy[1] ).c_str(), _pItem->szName );
        PushHint( buf, COLOR_WHITE, 5, 1 );
		_AddDescriptor();
	}
	else if( _pItem->sType == 65 )	//   add by Philip.Wu  2006-06-19
	{
		// Add by lark.li 20080320 begin
		if(5724 == _pItem->lID)
		{
			sprintf( buf, "Prison Term Remaining : %d mins",  item.sEnergy[0] / 60);
			PushHint( buf );

			SetHintIsCenter( false );
			_AddDescriptor();

			return;
		}
		// End

		//	Modify by alfred.shi 20080822
		if( 3279 <= _pItem->lID && _pItem->lID <= 3282||_pItem->lID == 6370||_pItem->lID == 6371||_pItem->lID == 6376||
				_pItem->lID == 6377||_pItem->lID == 6378||(_pItem->lID>=5882&&_pItem->lID<=5893)||_pItem->lID == 5895||
				_pItem->lID == 5897||(_pItem->lID>=6383&&_pItem->lID<=6385))	// 
		{
			PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

			SetHintIsCenter( false );
			_AddDescriptor();

			return;
		}


		SetHintIsCenter(true);

		PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );
		//_AddDescriptor();
		AddHintHeight();

		if( _pItem->lID == 2911 || _pItem->lID == 2912 || _pItem->lID == 2952 || 
			_pItem->lID == 3066 || _pItem->lID == 3078 )	// 
		{
			int nMonth  = 0;
			int nDay    = 0;
			int nHour   = 0;
			int nMinute = 0;
			int nSecond = 0;

			for(int i = 0; i < 5; ++i)
			{
				switch(GetData().sInstAttr[i][0])
				{
				case ITEMATTR_VAL_STA:
					nMonth = GetData().sInstAttr[i][1];
					break;
				case ITEMATTR_VAL_STR:
					nDay = GetData().sInstAttr[i][1];
					break;
				case ITEMATTR_VAL_CON:
					nHour = GetData().sInstAttr[i][1];
					break;
				case ITEMATTR_VAL_DEX:
					nMinute = GetData().sInstAttr[i][1];
					break;
				case ITEMATTR_VAL_AGI:
					nSecond = GetData().sInstAttr[i][1];
					break;
				}
			}

			if(_pItem->lID == 2911 || _pItem->lID == 2952 || _pItem->lID == 3066 || _pItem->lID == 3078)
			{
				sprintf( buf, "%s: %d", g_oLangRec.GetString(916), nMonth );
				PushHint( buf );

				sprintf( buf, "%s: %d", g_oLangRec.GetString(917), nDay );
				PushHint( buf );

				sprintf( buf, "%s: %d", g_oLangRec.GetString(918), nHour );
				PushHint( buf );

				sprintf( buf, "%s: %d", g_oLangRec.GetString(919), nMinute );
				PushHint( buf );
			}

			if(_pItem->lID != 3066 && _pItem->lID != 3078)
			{
				sprintf( buf, "%s: %d", g_oLangRec.GetString(920), nSecond );
				PushHint( buf );
			}

			AddHintHeight();
			_AddDescriptor();
			AddHintHeight();

			return;
		}
		else if(_pItem->lID == 2954)	// 
		{
			int nCount = 0;
			for(int i = 0; i < 5; ++i)
			{
				if(GetData().sInstAttr[i][0] == ITEMATTR_VAL_STR)
				{
					nCount = GetData().sInstAttr[i][1];
					break;
				}
			}

			sprintf( buf, "%s: %d", g_oLangRec.GetString(933), nCount );	// ""
			PushHint( buf );

			AddHintHeight();
			_AddDescriptor();
			AddHintHeight();

			return;
		}
        else if(_pItem->lID == 579)     //  
        {
            SetHintIsCenter( false );
		    _AddDescriptor();

            return;
        }
		//Add by sunny.sun 20080528 
		//Begin
		else if( _pItem->nID==5803 || _pItem->nID == 6373 )
		{
			if( _pItem->nID == 5803 )
			{
				sprintf(buf,"%s:%d",g_oLangRec.GetString(651),item.sInstAttr[ITEMATTR_VAL_STR]);
			}
			if(_pItem->nID == 6373 )
			{
				int nCount = 0;
				for(int i = 0; i < 5; ++i)
				{
					if(GetData().sInstAttr[i][0] == ITEMATTR_VAL_STR)
					{
						nCount = GetData().sInstAttr[i][1];
						break;
					}
				}

				sprintf(buf,"%d",nCount);
			}

			PushHint( buf, GENERIC_COLOR );
			_AddDescriptor();

			return;
		}
		//End

		sprintf( buf, g_oLangRec.GetString(664), 5 - item.sInstAttr[ITEMATTR_VAL_AGI] );
        PushHint( buf );

		sprintf( buf, g_oLangRec.GetString(665), 5 - item.sInstAttr[ITEMATTR_VAL_STR] );
        PushHint( buf );

		sprintf( buf, g_oLangRec.GetString(666), 5 - item.sInstAttr[ITEMATTR_VAL_DEX] );
        PushHint( buf );

		sprintf( buf, g_oLangRec.GetString(667), 5 - item.sInstAttr[ITEMATTR_VAL_CON] );
        PushHint( buf );

		AddHintHeight();

		switch( item.sInstAttr[ITEMATTR_VAL_STA] )
		{
		case 1:		// item.sID = 866
			PushHint( g_oLangRec.GetString(668), COLOR_RED );
			break;

		case 2:		// item.sID = 865
			PushHint( g_oLangRec.GetString(669), COLOR_RED );
			break;

		case 3:		// item.sID = 864
			PushHint( g_oLangRec.GetString(670), COLOR_RED );
			break;

		default:
			PushHint( g_oLangRec.GetString(671), COLOR_RED );
			break;
		}

		AddHintHeight();

		sprintf( buf, g_oLangRec.GetString(672), _ItemData.sEnergy[0]);
		PushHint( buf );

		return;
	}
	else if(_pItem->sType == 69)	// XXX
	{
        int iItem = 0;
        long lForge = 0;
        CItemRecord*     pCItemRec = NULL;

        PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

        sprintf(buf, g_oLangRec.GetString(869), _ItemData.sEndure[0]);
        PushHint(buf, GENERIC_COLOR);

        iItem = item.sInstAttr[ITEMATTR_VAL_AGI];
        if(iItem)
        {
            pCItemRec = GetItemRecordInfo(iItem);
            if(pCItemRec)
            {
                sprintf(buf, g_oLangRec.GetString(870), pCItemRec->szName);
                PushHint(buf, GENERIC_COLOR);
            }
        }

        sprintf(buf, g_oLangRec.GetString(871), _ItemData.sEnergy[1] - 100);
        PushHint(buf, GENERIC_COLOR);

        AddHintHeight();

        lForge = _ItemData.GetForgeParam();

        iItem = item.sInstAttr[ITEMATTR_VAL_STR];
        if(iItem)
        {
            pCItemRec = GetItemRecordInfo(iItem);
            if(pCItemRec)
            {
                sprintf(buf, g_oLangRec.GetString(872), pCItemRec->szName, (lForge / 10000000));
                PushHint(buf, GENERIC_COLOR);
            }
        }

        lForge %= 10000000;
        iItem = item.sInstAttr[ITEMATTR_VAL_CON];
        if(iItem)
        {
            pCItemRec = GetItemRecordInfo(iItem);
            if(pCItemRec)
            {
                sprintf(buf, g_oLangRec.GetString(873), pCItemRec->szName, (lForge / 10000));
                PushHint(buf, GENERIC_COLOR);
            }
        }

        lForge %= 1000;
        iItem = item.sInstAttr[ITEMATTR_VAL_DEX];
        if(iItem)
        {
            pCItemRec = GetItemRecordInfo(iItem);
            if(pCItemRec)
            {
                sprintf(buf, g_oLangRec.GetString(874), pCItemRec->szName, (lForge / 10));
                PushHint(buf, GENERIC_COLOR);
            }
        }

        AddHintHeight();

        sprintf(buf, g_oLangRec.GetString(875), item.sInstAttr[ITEMATTR_VAL_STA]);
        PushHint(buf, GENERIC_COLOR);

        sprintf(buf, g_oLangRec.GetString(876), _ItemData.sEnergy[0] * 10);
        PushHint(buf, GENERIC_COLOR);

        sprintf(buf, g_oLangRec.GetString(877), _ItemData.sEndure[1]);
        PushHint(buf, GENERIC_COLOR);

        AddHintHeight();

		if (_nPrice > 2000000000){
			int price = _nPrice -  2000000000;
			int num = price / 100000;
			int ID = price - (num * 100000);
			
			CItemRecord* pInfo= GetItemRecordInfo( ID );
				
			if(pInfo){
				sprintf(buf, "Trade Value: %dx %s",num,pInfo->szName);
			}else{
				sprintf(buf, "Trade Value: %dx [Unknown]",num);
			}
			AddHintHeight();
			PushHint( buf, COLOR_WHITE );
		}else if( _nPrice != 0 )
		{
			AddHintHeight();
			sprintf( buf, g_oLangRec.GetString(674), StringSplitNum( isMain ? _nPrice / 2 : _nPrice ) );
			PushHint( buf, COLOR_WHITE );
		}

        return;
	}
    else if(_pItem->sType == 70)
    {
        PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

		if(_pItem->lID == 2902 || _pItem->lID == 2903)	// , 
		{
			sprintf(buf, g_oLangRec.GetString(909), item.sInstAttr[ITEMATTR_VAL_STR]); // "%d"
	        PushHint(buf, GENERIC_COLOR);

			AddHintHeight();
			_AddDescriptor();
			AddHintHeight();

			return;
		}

		sprintf(buf, g_oLangRec.GetString(869), item.sInstAttr[ITEMATTR_VAL_STR]);
        PushHint(buf, GENERIC_COLOR);

        if(_pItem->lID != 2236)
        {
            sprintf(buf, g_oLangRec.GetString(878), _ItemData.sEndure[0] / 50);
            PushHint(buf, GENERIC_COLOR);

            sprintf(buf, g_oLangRec.GetString(897), _ItemData.sEnergy[0]); // "%i"
            PushHint(buf, GENERIC_COLOR);
        }

        AddHintHeight();

        _AddDescriptor();

        AddHintHeight();

 		if (_nPrice > 2000000000){
			int price = _nPrice -  2000000000;
			int num = price / 100000;
			int ID = price - (num * 100000);
			
			CItemRecord* pInfo= GetItemRecordInfo( ID );
				
			if(pInfo){
				sprintf(buf, "Trade Value: %dx %s",num,pInfo->szName);
			}else{
				sprintf(buf, "Trade Value: %dx [Unknown]",num);
			}
			AddHintHeight();
			PushHint( buf, COLOR_WHITE );
		}else if( _nPrice != 0 )
		{
			AddHintHeight();
			sprintf( buf, g_oLangRec.GetString(674), StringSplitNum( isMain ? _nPrice / 2 : _nPrice ) );
			PushHint( buf, COLOR_WHITE );
		}

        return;
    }
	else if(_pItem->sType == 71 && _pItem->lID == 3010)
	{
        PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

        sprintf( buf, g_oLangRec.GetString(644), _ItemData.sEnergy[0], _ItemData.sEnergy[1] );
        PushHint( buf );

		SetHintIsCenter( true );
		_AddDescriptor();
	}
	else if( _pItem->sType==71 && _pItem->lID == 3289 )   //
    {
		//add by ALLEN 2007-10-16

		// 
		PushHint(_pItem->szName, COLOR_WHITE, 5, 1 ); // 

		int nLevel = item.chForgeLv;
		const char* arShowName[5] = { g_oLangRec.GetString(944), g_oLangRec.GetString(945), g_oLangRec.GetString(946), g_oLangRec.GetString(947), g_oLangRec.GetString(948) };// "", "", "", "", "" };
		if(0 <= nLevel && nLevel <= 4)
		{
			sprintf(buf, g_oLangRec.GetString(949), arShowName[nLevel]);	// ":%s"
			PushHint(buf, COLOR_WHITE, 5, 1 );
		}

		sprintf(buf, g_oLangRec.GetString(950), item.sEndure[0], item.sEndure[1]);	// "(%d/%d)"
		PushHint(buf, COLOR_WHITE, 5, 1 );

		sprintf(buf, g_oLangRec.GetString(951), item.sEnergy[0] * 1000, item.sEnergy[1] * 1000);	// "(%d/%d)"
		PushHint(buf, COLOR_WHITE, 5, 1 );

		return;
    }
	//	Add by alfred.shi 20080922 begin. 
	else if( _pItem->sType==71 && _pItem->lID == 6377 )
	{
		
		PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

		SetHintIsCenter( false );
		_AddDescriptor();

		return;
		
	}
	//	End.
	else if( _pItem->sType==36 && _pItem->lID == 15006 )
	{
		DWORD color = 0xDC143C;
		short active = _ItemData.GetAttr(55);
		if (active)
		{
			color = 0x00FF00;
			sprintf( buf, "%s (%s)", _pItem->szName, "enable");
		}
		else
		{
			color = 0xDC143C;
			sprintf( buf, "%s (%s)", _pItem->szName, "disable");
		}

		PushHint( buf, (DWORD)(color ^ 0xFF000000), 5, 1, -1, true, -16777216);
		SetHintIsCenter( false );
		_AddDescriptor();
		return;
	}
    else if(_pItem->sType == 41)
    {
		if(_pItem->lID == 58) // 
		{
			PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

			sprintf( buf, g_oLangRec.GetString(626), item.sEnergy[0], item.sEnergy[1] );
			PushHint( buf, GENERIC_COLOR );

			SetHintIsCenter( true );
			_AddDescriptor();

			return;
		}
		else if(_pItem->lID == 171) // bragi
		{
			PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

			static const char* pszText[5] = {	g_oLangRec.GetString(952), 
												g_oLangRec.GetString(953), 
												g_oLangRec.GetString(954), 
												g_oLangRec.GetString(955), 
												g_oLangRec.GetString(956)	};

			int nIndex = item.sEndure[0];

			sprintf( buf, g_oLangRec.GetString(957), 0 <= nIndex && nIndex <= 4 ? pszText[nIndex] : "Not Valid");
			PushHint( buf, GENERIC_COLOR );

			SetHintIsCenter( true );
			_AddDescriptor();

			return;
		}
		else if(_pItem->lID == 2967) // 
		{
			PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

			sprintf( buf, g_oLangRec.GetString(626), item.sEnergy[0], item.sEnergy[1] );
			PushHint( buf, GENERIC_COLOR );

			SetHintIsCenter( true );
			_AddDescriptor();

			return;
		}
		else    // 
		{
			PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

			SetHintIsCenter( false );
			_AddDescriptor();
		}
	}
    else    // 
    {
        PushHint( _pItem->szName, COLOR_WHITE, 5, 1 );

        SetHintIsCenter( false );

		if(_pItem->sType != 44)
			_AddDescriptor();
    }

	if(_pItem->sType != 75)	// 
	{
		// ,
		AddHintHeight();
		for( int i=0; i<ITEMATTR_CLIENT_MAX; i++ )
		{
			if( item.sInstAttr[i]!=0 )
			{
				_PushItemAttr( i, item, ADVANCED_COLOR );
			}
		}
	}

	if( _hints.GetCount()>0 && (_pItem->sType<=27 || _pItem->sType==44) && _pItem->sType!=18 && _pItem->sType!=19 && _pItem->sType!=21) 
	{
		// ,
		char szBuf[16] = { 0 };
		sprintf( szBuf, "%09d", _ItemData.sEnergy[1]/10 );		// 4,
		char szHundred[2] = { szBuf[6], 0 };
		int nHundred = atoi( szHundred );
		int nTen = atoi( &szBuf[7] );

		DWORD dwNameColor = COLOR_BLACK;
		switch( nHundred )
		{		// ITEM RARITY - MDR MAY 2020 - FPO ALPHA
		case 0:   dwNameColor=0xffC1C1C1; break;		// Gray
		case 1:   dwNameColor=0xffFFFFFF; break;		// White, duh
		case 2:   dwNameColor=0xffFFFFFF; break;		// White, duh	
		case 3:   dwNameColor=0xffA2E13E; break;		// Green
		case 4:   dwNameColor=0xffA2E13E; break;		// Green
		case 5:   dwNameColor=0xffd68aff; break;		// Purple
		case 6:   dwNameColor=0xffd68aff; break;		// Purple
		case 7:   dwNameColor=0xffff6440; break; 		// Red/ Orange
		case 8:   dwNameColor=0xffff6440; break;		// Red/ Orange
		case 9:   dwNameColor=0xffffcc12; break;		// Gold
		}

		CItemPreInfo* pInfo = GetItemPreInfo( nTen );
		if( pInfo && strcmp( pInfo->szDataName, "0" )!=0 )
		{
			//if (_pItem->lID >= CItemRecord::enumItemFusionStart && _pItem->lID < CItemRecord::enumItemFusionEnd)
			//{
			//if ( CItemRecord::IsVaildFusionID(_pItem)
			//&& _ItemData.GetFusionItemID() > 0 ){
			if ( _ItemData.GetItemLevel() > 0 ){
				sprintf( buf, "Lv%d %s%s",_ItemData.GetItemLevel(), pInfo->szDataName, GetName());

				//if (_ItemData.dwDBID)
					//_hints.GetHint(1)->hint = buf;
				//else
					_hints.GetHint(0)->hint = buf;

			}
			else
			{
				//if (_ItemData.dwDBID)
					//_hints.GetHint(1)->hint = pInfo->szDataName + _hints.GetHint(1)->hint;
				//else
					_hints.GetHint(0)->hint = pInfo->szDataName + _hints.GetHint(0)->hint;
			}
		}

		//if (_ItemData.dwDBID)
			//_hints.GetHint(1)->color = dwNameColor;
		//else
			_hints.GetHint(0)->color = dwNameColor;
	}

	// 
	SItemForge& Forge = GetForgeInfo();
	if( _hints.GetCount()>0 && Forge.IsForge )
	{
		if( Forge.nHoleNum>0 )
		{
			sprintf( buf, g_oLangRec.GetString(673), Forge.nHoleNum );
			PushHint( buf, ADVANCED_COLOR );
		}

		for( int i=0; i<Forge.nStoneNum && i<Forge.nHoleNum; i++ )
		{
			sprintf( buf, g_oLangRec.GetString(656), ConvertNumToChinese( Forge.nStoneLevel[i] ).c_str(), Forge.pStoneInfo[i]->szDataName );
			//PushHint( buf, (DWORD)((Forge.pStoneInfo[i]->nItemRgb) ^ 0xFF000000) );
			PushHint( buf, (DWORD)((Forge.pStoneInfo[i]->nItemRgb) ^ 0xFF000000), 5, 1, -1, true, -16777216);
		} //COLOR_RED

		if( Forge.nStoneNum>0 )
		{
			AddHintHeight();
			for( int i=0; i<Forge.nStoneNum && i<Forge.nHoleNum; i++ )
			{
				PushHint( Forge.szStoneHint[i], ADVANCED_COLOR );
			}
		}

		if( Forge.nLevel>0 )
		{
			/*if ( _ItemData.dwDBID )
			{
				sprintf( buf, "%s +%d", _hints.GetHint(1)->hint.c_str(), Forge.nLevel );
				_hints.GetHint(1)->hint = buf;
			}
			else*/
			{
				sprintf( buf, "%s +%d", _hints.GetHint(0)->hint.c_str(), Forge.nLevel );
				_hints.GetHint(0)->hint = buf;
			}
		}
	}
	//mothannakh trade/throw/stack/delete hint 
	if (_ItemData.sNum != 0)
	{
	//PUSH_HINT("Bounded",-1, _pItem->chIsTrade && _pItem->chIsThrow && _pItem->chIsDel ==1 ? COLOR_GREEN : COLOR_RED);
	DWORD color;
	if(_ItemData.GetInstAttr(ITEMATTR_TRADABLE) && _pItem->chIsTrade){
		color = COLOR_GREEN;
	}else{
		color = COLOR_RED;
	}
	PUSH_HINT("Trade",-1, color);

	if(_ItemData.GetInstAttr(ITEMATTR_TRADABLE) && _pItem->chIsThrow){
		color = COLOR_GREEN;
	}else{
		color = COLOR_RED;
	}

	PUSH_HINT("Throw", -1, color);
	PUSH_HINT("Delete",-1, _pItem->chIsDel != 0 ? COLOR_GREEN : COLOR_RED);

	// max stack for item 
	sprintf(buf, "Stack: %d", _pItem->nPileMax);
	PushHint(buf, COLOR_WHITE);
	}
	// end 
	if (_nPrice > 2000000000){
		int price = _nPrice -  2000000000;
		int num = price / 100000;
		int ID = price - (num * 100000);
		
		CItemRecord* pInfo= GetItemRecordInfo( ID );
			
		if(pInfo){
			sprintf(buf, "Trade Value: %dx %s",num,pInfo->szName);
		}else{
			sprintf(buf, "Trade Value: %dx [Unknown]",num);
		}
		AddHintHeight();
		PushHint( buf, COLOR_WHITE );
	}else if( _nPrice != 0 )
	{
		AddHintHeight();
		sprintf( buf, g_oLangRec.GetString(674), StringSplitNum(  _ItemData.sNum != 0 ? _nPrice / 2 : _nPrice ) );
		PushHint( buf, COLOR_WHITE );	//item price hint 
	}

//price fix 
/*
	if( _nPrice != 0 )
		{
			AddHintHeight();
			sprintf( buf, g_oLangRec.GetString(674), StringSplitNum( _ItemData.sNum != 0  ? _nPrice/ 2 : _nPrice ) );
			PushHint( buf, COLOR_WHITE );	//item price hint 
		}
	//end
	*/
	if ( _ItemData.dwDBID )
	{
		//PushHint( "Locked", COLOR_RED, 5, 0 );
		PushHint( "Locked", (DWORD)(0xFFA500 ^ 0xFF000000), 5, 1, -1, true, -16777216);
		//_hints.GetHint(0)->color = 0xff888888;
	}

}

string CItemCommand::GetStoneHint(int nLevel)
{
	string hint = "error";
	if( _pItem->sType==49 )
	{
		CStoneInfo* pInfo = NULL;
		int nCount = CStoneSet::I()->GetLastID() + 1;
		for( int i=0; i<nCount; i++ )
		{
			pInfo = ::GetStoneInfo( i );
			if( !pInfo ) continue;

			if( pInfo->nItemID != _pItem->nID ) continue;

			if( nLevel<0 )
				g_pGameApp->GetScriptMgr()->DoString( pInfo->szHintFunc, "u-s", _ItemData.sEnergy[1], &hint );
			else
				g_pGameApp->GetScriptMgr()->DoString( pInfo->szHintFunc, "u-s", 1, &hint );
			return hint;
		}
		
	}
	return hint;
}

//void CItemCommand::_PushItemAttr( int attr, SItemGrid& item, DWORD color )
//{
//    for( int i=0; i<defITEM_INSTANCE_ATTR_NUM; i++ )
//    {
//        if( item.sInstAttr[i][0]==attr )
//        {
//			if ( item.sInstAttr[i][1]==0 ) 
//			{
//				return;
//			}
//			else
//			{
//				if( attr <= ITEMATTR_COE_PDEF )
//				{
//					if( !(item.sInstAttr[i][1] % 10) )
//					{						
//					}
//					else
//					{
//						float f = (float)item.sInstAttr[i][1] / 10.0f;
//					}
//				}
//				else
//				{
//				}
//				PushHint( buf, color );
//
//				item.sInstAttr[i][0] = 0;
//				return;
//			}
//        }
//    }
//}


void CItemCommand::_PushValue( const char* szFormat, int attr, SItemHint& item, DWORD color )
{
	if( attr<=0 || attr>=ITEMATTR_CLIENT_MAX ) 
		return;

	if( item.sInstAttr[attr]==0 ) 
		return;

	PUSH_HINT( szFormat, item.sInstAttr[attr], color );
	item.sInstAttr[attr] = 0;
}

void CItemCommand::_PushItemAttr( int attr, SItemHint& item, DWORD color )
{
	if( attr<=0 || attr>=ITEMATTR_CLIENT_MAX ) 
		return;

	if( item.sInstAttr[attr]==0 ) 
		return;

	if( attr <= ITEMATTR_COE_PDEF )
	{
		if( !(item.sInstAttr[attr] % 10) )
		{						
			sprintf( buf, "%s:%+d%%", g_GetItemAttrExplain( attr ), item.sInstAttr[attr] / 10 );
		}
		else
		{
			float f = (float)item.sInstAttr[attr] / 10.0f;
			sprintf( buf, "%s:%+.1f%%", g_GetItemAttrExplain( attr ), f );
		}
	}
	else
	{
		sprintf( buf, "%s:%+d", g_GetItemAttrExplain( attr ), item.sInstAttr[attr] );
	}
	PushHint( buf, color );

	item.sInstAttr[attr] = 0;
}

bool CItemCommand::IsDragFast()
{
	return true;
	//try allowing all items on bar.
	return (_pItem->sType>=31 && _pItem->sType<=33) || _pItem->sType == 71;
}

void CItemCommand::SetTotalNum( int num )
{
    if( _pItem->GetIsPile() )
    {
        if( num>=0 )
        {
            _ItemData.sNum = num;
        }
    }
    else
    {
        _ItemData.sNum = 1;
    }
}

bool CItemCommand::IsAllowThrow()
{
    return _pItem->chIsThrow!=0;
}

//	2008-9-17	yangyinyu	add	begin!
char	_lock_pos_            = NULL;	
long	_lock_item_id_        = 0;
long	_lock_grid_id_        = 0;
long	_lock_fusion_item_id_ = 0;

//extern	bool	g_yyy_add_lock_item_wait_return_state;		//	

static	void	_evtSelectYesNoEvent(	CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey	)
{
	//	
	
	
	
	pSender->GetForm()->Close();
	
	//	
	string	name	=	pSender->GetName();

	if(	!strcmp(	name.c_str(),	"btnYes"	)	)
	{
		//	
		//CS_DropLock(	_lock_pos_,	_lock_item_id_,	_lock_grid_id_,	_lock_fusion_item_id_	);
		CS_DropLock(	_lock_grid_id_	);
		//	
		CCursor::I()->SetCursor(	CCursor::stWait	);
		//	
		//g_yyy_add_lock_item_wait_return_state	=	true;

	}

	//pBox->frmDialog->SetParent(	NULL );
};

#include "uiboxform.h"
extern	CBoxMgr			g_stUIBox;
//	2008-9-17	yangyinyu	add	end!

bool CItemCommand::MouseDown()
{
	CCharacter* pCha = CGameScene::GetMainCha();
	if (!pCha) {
		return false;
	}

	if( CRepairState* pState = dynamic_cast<CRepairState*>(pCha->GetActor()->GetCurState()) )
	{
		// 
		if( _pItem->sType>=31 ) 
		{
			g_pGameApp->SysInfo( g_oLangRec.GetString(675), _pItem->szName );
			return false;
		}

		CGoodsGrid* pOwn = dynamic_cast<CGoodsGrid*>(GetParent());
		if( pOwn )
		{
			if( pOwn==g_stUIEquip.GetGoodsGrid() )
			{
				int GridID = pOwn->FindCommand( this );
				if( GridID==-1 ) return false;

				CS_ItemRepairAsk( pCha->getAttachID(), pCha->lTag, 2, GridID );
				return true;
			}
		}

		COneCommand* pOne = dynamic_cast<COneCommand*>(GetParent());
		if( pOne )
		{
			CS_ItemRepairAsk( pCha->getAttachID(), pCha->lTag, 1, pOne->nTag );
			return true;
		}
		return false;
	}
	else if( CFeedState* pState = dynamic_cast<CFeedState*>(pCha->GetActor()->GetCurState()) )
	{
		// 
		if( _pItem->sType!=enumItemTypePet )
		{
			g_pGameApp->SysInfo( g_oLangRec.GetString(676) );
			return false;
		}

		CGoodsGrid* pOwn = dynamic_cast<CGoodsGrid*>(GetParent());
		if( pOwn )
		{
			if( pOwn==g_stUIEquip.GetGoodsGrid() )
			{
				int GridID = pOwn->FindCommand( this );
				if( GridID==-1 ) return false;

				stNetUseItem param;
				param.sGridID = pState->GetFeedGridID();
				param.sTarGridID = GridID;
				CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_USE, (void*)&param );

				pState->PopState();
				return true;
			}
		}
		return false;
	}
	else if( CFeteState* pState = dynamic_cast<CFeteState*>(pCha->GetActor()->GetCurState()) )
	{
		CGoodsGrid* pOwn = dynamic_cast<CGoodsGrid*>(GetParent());
		if( pOwn )
		{
			if( pOwn==g_stUIEquip.GetGoodsGrid() )
			{
				int GridID = pOwn->FindCommand( this );
				if( GridID==-1 ) return false;

				stNetUseItem param;
				param.sGridID = pState->GetFeteGridID();
				param.sTarGridID = GridID;
				CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_USE, (void*)&param );

				pState->PopState();
				return true;
			}
		}
		return false;
	}
	return false;
}

bool CItemCommand::UseCommand(bool isRightClick)
{
	
	/*
	if(isRightClick){
		CFormMgr &mgr = CFormMgr::s_Mgr;
		CForm* frmMakeEquip = mgr.Find("frmMakeEquip");
		if(frmMakeEquip->GetIsShow()){
			
			if(_pItem->lID == 456){//str crystal
				g_stUIMakeEquip.PushItem(1,*this,1);
			}else if(_pItem->lID == 455){//str scroll
				g_stUIMakeEquip.PushRouleau(*this);
			}else{//other (equip)
				g_stUIMakeEquip.PushItem(0,*this,1);
			}
			g_stUIMakeEquip.SetMakeEquipUI();
			SetIsValid(false);
			return true;
			
		}
	}*/
	
	
	static DWORD dwTime = 0;
	/*
	CItemRecord* pEquipItem = GetItemRecordInfo( _pItem->lID);
	if(!pEquipItem){
		return false;
	}
	
	if( CGameApp::GetCurTick() < dwTime && pEquipItem->sType !=1 )
	{
		return false;
	}*/
	dwTime = CGameApp::GetCurTick() + 1000;

    if( !GetIsValid() ){
		return false;
	} 

	CCharacter* pCha = CGameScene::GetMainCha();
	if (!pCha) {
		return false;
	}

	
	if( pCha->GetChaState()->IsFalse(enumChaStateUseItem) )
	{
		g_pGameApp->SysInfo( g_oLangRec.GetString(678) );
		return false;
	}
	
    CGoodsGrid* pOwn = dynamic_cast<CGoodsGrid*>(GetParent());
    if( pOwn )
    {
		CCharacter* pCha = g_stUIBoat.FindCha(pOwn);
		if (!pCha) {
			return false;
		}

        int GridID = pOwn->FindCommand( this );
        if( GridID==-1 ) {
			return false;
		}
		
		/*
		if( _pItem->sType==enumItemTypePetFodder || _pItem->sType==enumItemTypePetSock )
		{
			CActor* pActor = CGameScene::GetMainCha()->GetActor();

			CFeedState* pState = new CFeedState( pActor );
			pState->SetFeedGridID( GridID );
			pActor->SwitchState( pState );
			return false;
		}
		else */
			
		if( _pItem->sType == 66 || ( 864 <= _pItem->lID && _pItem->lID <= 866 ) )	//   add by Philip.Wu  2006-06-20
		{
			CActor* pActor = CGameScene::GetMainCha()->GetActor();

			CFeteState* pState = new CFeteState( pActor );
			pState->SetFeteGridID( GridID );
			pActor->SwitchState( pState );
			return false;
		}
		else if(GetItemInfo()->sType == 71) // 
		{
			CCharacter* pCha = CGameScene::GetMainCha();
			if( !pCha ) return false;

			int nSkillID = atoi(GetItemInfo()->szAttrEffect);
			CSkillRecord* pSkill = GetSkillRecordInfo(nSkillID);

			// 
			if( !pSkill || !pSkill->GetIsUse() ) return false;

			// 
			if( pCha->IsBoat() && pSkill->chSrcType != 2)
			{
				g_pGameApp->SysInfo(g_oLangRec.GetString(879));
				return false;
			}

			// 
			if( ! pCha->IsBoat() && pSkill->chSrcType != 1)
			{
				g_pGameApp->SysInfo(g_oLangRec.GetString(880));
				return false;
			}

			int nCoolDownTime = atoi(pSkill->szFireSpeed);
			if(nCoolDownTime > 0)//  cooldown 
			{
				DWORD nCurTickCount = g_pGameApp->GetCurTick() - 500;	// 500
				map<int, DWORD>::iterator it = _mapCoolDown.find(nSkillID);

				if(it != _mapCoolDown.end() && it->second + nCoolDownTime >= nCurTickCount)
				{
					// cooldown 
					g_pGameApp->SysInfo(g_oLangRec.GetString(898), (it->second + nCoolDownTime - nCurTickCount) / 1000 + 1);//" %d "
					return false;
				}

				_mapCoolDown[nSkillID] = g_pGameApp->GetCurTick();	// 
			}

			// 
			if( pSkill->GetDistance()<=0 )
			{
				CAttackState* attack = new CAttackState(pCha->GetActor());
				attack->SetSkill( pSkill );
				attack->SetTarget( pCha );
				attack->SetCommand( this );
				return pCha->GetActor()->SwitchState(attack);
			}

			//pCha->GetActor()->Stop();
			pCha->ChangeReadySkill(nSkillID);
			return false;
		}


		if (pOwn == g_stUIEquip.GetGoodsGrid())
		{

			stNetUseItem param;
			param.sGridID = GridID;

			if (isRightClick == true) {
				param.sTarGridID = -2;
			}


			if ((_pItem->sType < 31) && pCha == CGameScene::GetMainCha() && g_stUIBoat.GetHuman() == pCha)
			{
				CActor* pActor = g_stUIBoat.GetHuman()->GetActor();

				CEquipState* pState = new CEquipState(pActor);
				pState->SetUseItemData(param);
				pActor->SwitchState(pState);
				return true;
			}
			else
			{
				if (!IsAllowUse())
					return false;
				if (!StartCommand())
					return false;
				CS_BeginAction(pCha, enumACTION_ITEM_USE, (void*)&param);
				return true;
			}
		}
		return false;
    }
	
    COneCommand* pOne = dynamic_cast<COneCommand*>(GetParent());
    if( pOne )
    {
		g_stUIEquip.UnfixToGrid( this, -1, pOne->nTag );
    }
    return false;
}

bool CItemCommand::StartCommand()
{
    if( _GetSkillTime()>0 )
    {
        _pAniClock = g_pGameApp->AddAniClock();
        if( _pAniClock )
        {
			_dwPlayTime = CGameApp::GetCurTick() + _GetSkillTime();

            _pAniClock->Play( _GetSkillTime() );
            g_pGameApp->AddTipText( "CItemCommand::Exec[%s]", _pItem->szName );
            return true;
        }
	    g_pGameApp->AddTipText( "CItemCommand::Exec[%s] Failed", _pItem->szName );
	    return false;
    }
    return true;
}

bool CItemCommand::IsAllowUse()
{
	constexpr auto lowest_reported_cooldown{ 1.0f };

	if( _pSkill)
	{
		if( !_pSkill->GetIsUse() ) return false;

		CCharacter* pCha = g_stUIBoat.GetHuman();
		if( !pCha ) return false;

		if( pCha->IsBoat())
		{
			if (_pSkill->chSrcType != 2)
			{
				g_pGameApp->SysInfo("Item cannot be used in the sea!");
				return false;
			}
			else if (_pSkill->chSrcType == 1)
			{
				if( _pSkill->GetIsActive() ) return true;

				if(g_stUIBank.GetBankGoodsGrid()->GetForm()->GetIsShow())
				{
					g_pGameApp->SysInfo( g_oLangRec.GetString(748) );
					return false;
				}

				if( pCha->GetChaState()->IsFalse(enumChaStateUseSkill) )
				{
					g_pGameApp->SysInfo( g_oLangRec.GetString(748) );
					return false;
				}

				if( !g_SkillUse.IsValid( _pSkill, pCha ) )
				{
					g_pGameApp->SysInfo( "%s", g_SkillUse.GetError() );
					return false;
				}

				if( _GetSkillTime()<=0 ) 
					return true;
					
				if (_pAniClock)
				{
					const auto t = _pAniClock->RemainingTime();
					if (t < lowest_reported_cooldown)
					{
						return false;
					}

					//g_pGameApp->SysInfo("Item:[%s] in cooldown mode, remaining time is %.1f sec(s)",
					//	_pItem->szName, t);
					return false;
				}
				return true; 
			}
			else
			{
				g_pGameApp->SysInfo("Item cannot be used on land!");
				return false;
			}
		}
	}
		
	if (_pAniClock)
	{
		const auto t = _pAniClock->RemainingTime();
		if (t < lowest_reported_cooldown)
		{
			return false;
		}

		///g_pGameApp->SysInfo("Item:[%s] in cooldown mode, remaining time is %.1f sec(s)",
			//_pItem->szName, t);
		return false;
	}


	return true;
}

bool CItemCommand::IsAtOnce()      
{
	if (_pSkill)
    	return _pSkill->GetIsActive() || _pSkill->GetDistance()<=0 || enumSKILL_TYPE_SELF==_pSkill->chApplyTarget;
	else
		return 1;
}

bool CItemCommand::ReadyUse()
{
    if (!_pSkill)
		return false;
	
	CCharacter* pCha = CGameScene::GetMainCha();
	if (!pCha)
		return false;

	if (_pSkill->GetDistance() > 0)
	{
		return pCha->ChangeReadySkill(_pSkill->nID);
	}
	else
	{
		CAttackState *attack = new CAttackState(pCha->GetActor());
        attack->SetSkill( _pSkill );
        attack->SetTarget( pCha );
		attack->SetCommand( this );
        return pCha->GetActor()->SwitchState(attack);
	}
	return false;
}

void CItemCommand::Error()
{    
	g_pGameApp->AddTipText("Item [%s] encounter an error!", _pItem->szName );
}

//int CItemCommand::_GetValue( int nItemAttrType, SItemGrid& item )
//{
//    for( int i=0; i<defITEM_INSTANCE_ATTR_NUM; i++ )
//    {
//        if( item.sInstAttr[i][0]==nItemAttrType )
//        {
//            item.sInstAttr[i][0] = 0;
//            return item.sInstAttr[i][1];
//        }
//    }
//
//    return -1;
//}

int CItemCommand::_GetValue( int nItemAttrType, SItemHint& item )
{
	if( nItemAttrType<=0 || nItemAttrType>=ITEMATTR_CLIENT_MAX ) 
		return -1;

	int nValue = 0;
	if( item.sInstAttr[nItemAttrType]!=0 )
	{
		nValue = item.sInstAttr[nItemAttrType];
		item.sInstAttr[nItemAttrType] = 0;
		return nValue;
	}

	return -1;
}

bool CItemCommand::GetIsPile()     
{ 
    return _pItem->GetIsPile(); 
}

int CItemCommand::GetPrice()
{
    return _nPrice;
}

void CItemCommand::SetData( const SItemGrid& item )  
{ 
    memcpy( &_ItemData, &item, sizeof(_ItemData) );
	int start = 0;
    for( ; start<defITEM_INSTANCE_ATTR_NUM; start++ )
    {
        if( item.sInstAttr[start][0]==0 )
        {
            break;
        }
    }
    for( int i=start; i<defITEM_INSTANCE_ATTR_NUM; i++ )
    {
        _ItemData.sInstAttr[i][0] = 0;
        _ItemData.sInstAttr[i][1] = 0;
    }
}

int CItemCommand::GetTotalNum()       
{
    return _ItemData.sNum;
}


const char* CItemCommand::GetName()   
{ 
    if( _ItemData.chForgeLv==0 )
    {
        return _pItem->szName;
    }
    else
    {
        static char szBuf[128] = { 0 };
        sprintf( szBuf, "Lv%d %s", _ItemData.chForgeLv, _pItem->szName );
        return szBuf;
    }
}

void CItemCommand::_ShowWork( CItemRecord* pItem, SGameAttr* pAttr )
{
	bool isFind = false;
	bool isSame = false;

	

	for( int i=0; i<MAX_JOB_TYPE; i++ )
	{		
		if( pItem->szWork[i]<0 )
			break;

		if( !isFind ) 
		{
			sprintf( buf, g_oLangRec.GetString(679), g_GetJobName(pItem->szWork[i]) );
			isFind = true;
		}

		if( !isSame ) 
		{
			if( pAttr->get(ATTR_JOB)==pItem->szWork[i] )
			{
				isSame = true;
			}
		}
	}

	if( isFind )
	{
		PushHint( buf, isSame ? GENERIC_COLOR : VALID_COLOR );
	}
	//else
	//{
	//	PushHint( ":", GENERIC_COLOR );
	//}
}

void CItemCommand::_ShowFusionWork(CItemRecord* pAppearItem, CItemRecord* pEquipItem, SGameAttr* pAttr)// 
{
	bool isFind = false;
	int  iAppearIndex = -1;
	int  iEquipIndex = -1;
	bool isSame = false;
	CItemRecord* pItem(0);

	if (pAppearItem->szWork[0] > pEquipItem->szWork[0])
	{
		pItem = pAppearItem;
	}
	else
	{
		pItem = pEquipItem;
	}

	for( int i=0; i<MAX_JOB_TYPE; i++ )
	{
		if( pItem->szWork[i]<0 )
			break;

		if( !isFind ) 
		{
			sprintf( buf, g_oLangRec.GetString(679), g_GetJobName(pItem->szWork[i]) );
			isFind = true;
		}

		if( !isSame ) 
		{
			if( pAttr->get(ATTR_JOB)==pItem->szWork[i] )
			{
				isSame = true;
			}
		}

	}

	if( isFind )
	{
		PushHint( buf, isSame ? GENERIC_COLOR : VALID_COLOR );
	}
	//else
	//{
	//	PushHint( ":", GENERIC_COLOR );
	//}
}


void CItemCommand::_ShowWork( xShipInfo* pInfo, SGameAttr* pAttr )
{
	bool isFind = false;
	bool isSame = false;

	for( int i=0; i<MAX_JOB_TYPE; i++ )
	{
		if( pInfo->sPfLimit[i]==(USHORT)-1 )
			break;

		/*if( !isFind ) 
		{
			sprintf( buf, g_oLangRec.GetString(679), g_GetJobName(pInfo->sPfLimit[i]) );
			isFind = true;
		}*/
		//add by alfred.shi 20080714	begin	
		/*	*/
		if( !isFind ) 
		{
			//string name(RES_STRING(CL_UIITEMCOMMAND_CPP_00010));
			string name("");
			g_GetJobName(pInfo->sPfLimit[i]);
			if(name.compare(g_GetJobName(pInfo->sPfLimit[i])) == 0)
			{
				sprintf( buf,"%s", "");
				//PushHint( buf, VALID_COLOR );
			}
			else
			{sprintf( buf, g_oLangRec.GetString(679), g_GetJobName(pInfo->sPfLimit[i]) );}
			
			isFind = true;
		}
		//	end

		if( !isSame ) 
		{
			if( pAttr->get(ATTR_JOB)==pInfo->sPfLimit[i] )
			{
				isSame = true;
			}
		}
	}

	if( isFind )
	{
		PushHint( buf, isSame ? GENERIC_COLOR : VALID_COLOR );
	}
	//else
	//{
	//	PushHint( ":", GENERIC_COLOR );
	//}
}

void CItemCommand::_ShowBody( CItemRecord* _pItem2)
{
	if(!_pItem2){
		_pItem2 = _pItem;
	}
	if( _pItem2->IsAllEquip() ) return;

	if( !g_stUIBoat.GetHuman() || !g_stUIBoat.GetHuman()->GetDefaultChaInfo() ) return;

	std::ostrstream str;
	str << g_oLangRec.GetString(680);
	for( int i=1; i<5; i++ )
	{
		if( !_pItem2->IsAllowEquip(i) )
			continue;

		switch( i )
		{
		case 1: str << g_oLangRec.GetString(681);   break;
		case 2: str << g_oLangRec.GetString(682); break;
		case 3: str << g_oLangRec.GetString(683); break;
		case 4: str << g_oLangRec.GetString(684);   break;
		}
	}
	str << '\0';

	int nBodyType = g_stUIBoat.GetHuman()->GetDefaultChaInfo()->lID;
	PushHint( str.str(), _pItem->IsAllowEquip(nBodyType) ? GENERIC_COLOR : VALID_COLOR );			
}

void CItemCommand::_ShowFusionBody(CItemRecord* pEquipItem)
{
	if( _pItem->IsAllEquip() && pEquipItem->IsAllEquip()) return;

	if( !g_stUIBoat.GetHuman() || !g_stUIBoat.GetHuman()->GetDefaultChaInfo() ) return;

	std::ostrstream str;
	str << g_oLangRec.GetString(680);
	for( int i=1; i<5; i++ )
	{
		if( _pItem->IsAllowEquip(i) && pEquipItem->IsAllowEquip(i))
		{
			switch( i )
			{
			case 1: str << g_oLangRec.GetString(681);   break;
			case 2: str << g_oLangRec.GetString(682); break;
			case 3: str << g_oLangRec.GetString(683); break;
			case 4: str << g_oLangRec.GetString(684);   break;
			}
		}
	}
	str << '\0';

	int nBodyType = g_stUIBoat.GetHuman()->GetDefaultChaInfo()->lID;
	PushHint( str.str(), _pItem->IsAllowEquip(nBodyType) ? GENERIC_COLOR : VALID_COLOR );			
}


void CItemCommand::SetBoatHint( const NET_CHARTRADE_BOATDATA* const pBoat )
{
	if( pBoat )
	{
		if( !_pBoatHint )
		{
			_pBoatHint = new NET_CHARTRADE_BOATDATA;
		}
		memcpy( _pBoatHint, pBoat, sizeof(NET_CHARTRADE_BOATDATA) );
	}
	else
	{
		if( _pBoatHint )
		{
			delete _pBoatHint;
			_pBoatHint = NULL;
		}
	}
}

SItemForge&	CItemCommand::GetForgeInfo()
{
	return SItemForge::Convert( _ItemData.lDBParam[0] );
}

//---------------------------------------------------------------------------
// class SItemHint
//---------------------------------------------------------------------------
void SItemHint::Convert( SItemGrid& ItemGrid, CItemRecord* pInfo )
{
	sID = ItemGrid.sID;
	sNum = ItemGrid.sNum;
	sEndure[0] = ItemGrid.sEndure[0] / 50;
	sEndure[1] = ItemGrid.sEndure[1] / 50;
	sEnergy[0] = ItemGrid.sEnergy[0];
	sEnergy[1] = ItemGrid.sEnergy[1];
	chForgeLv = ItemGrid.chForgeLv;
	bItemTradable = ItemGrid.bItemTradable;
	expiration = ItemGrid.expiration;

	lDBParam = ItemGrid.lDBParam;

	sInstAttr = {};
	for( int i=0; i<ITEMATTR_CLIENT_MAX; i++ )
	{
		sInstAttr[i] = pInfo->GetTypeValue( i );
	}

	// 
	int nAttr = 0;
	for( int i=0; i<defITEM_INSTANCE_ATTR_NUM; i++ )
	{
		nAttr = ItemGrid.sInstAttr[i][0];
		if( nAttr<=0 || nAttr>=ITEMATTR_CLIENT_MAX ) continue;

		sInstAttr[nAttr] = ItemGrid.sInstAttr[i][1];
	}
}

//---------------------------------------------------------------------------
// class SItemForge
//---------------------------------------------------------------------------
SItemForge& SItemForge::Convert( DWORD v, int nItemID )
{
	static SItemForge forge;
	memset( &forge, 0, sizeof(forge) );

	DWORD dwForgeValue = v;
	if( dwForgeValue==0 )
		return forge;

	forge.IsForge = true;
	forge.nHoleNum = v / 1000000000;	// 

	int nStoneData;
	CStoneInfo* pStoneInfo = NULL;
	for(int i = 0; i < 3; ++i)
	{
		nStoneData = (v / (int)(pow(1000, 2 - i))) % 1000;	// 

		pStoneInfo = GetStoneInfo(nStoneData / 10);
		if(!pStoneInfo) continue;

		forge.pStoneInfo[forge.nStoneNum]  = pStoneInfo;
		forge.nStoneLevel[forge.nStoneNum] = nStoneData % 10;

		forge.nLevel    += forge.nStoneLevel[forge.nStoneNum];

		string strHint = "";
		if( g_pGameApp->GetScriptMgr()->DoString( pStoneInfo->szHintFunc, "u-s", forge.nStoneLevel[forge.nStoneNum], &strHint ) )
		{
			strncpy( forge.szStoneHint[forge.nStoneNum], strHint.c_str(), sizeof(forge.szStoneHint[forge.nStoneNum]) );
		}

		forge.nStoneNum += 1;
	}

	if( nItemID>0 )
	{
		forge.Refresh( nItemID );
	}

	return forge;

	//////////////////////////////////////////////////////////////////////////////////
	// 

	//int Num = 0;
	//if( g_pGameApp->GetScriptMgr()->DoString( "Get_HoleNum", "u-d", dwForgeValue, &Num ) )
	//{
	//	if( Num>0 )
	//	{
	//		forge.nHoleNum = Num;
	//	}
	//}

	//// 
	//int nStone;
	//int nStoneLv;
	//CStoneInfo* pStone = NULL;
	//int StoneNum = 0;
	//string hint;
	//for( int i=0; i<3; i++ )
	//{
	//	nStone = 0;
	//	if( !g_pGameApp->GetScriptMgr()->DoString( buf, "u-d", dwForgeValue, &nStone ) )
	//		continue;

	//	pStone = GetStoneInfo( nStone );
	//	if( !pStone ) continue;

	//	forge.pStoneInfo[ StoneNum ] = pStone;

	//	nStoneLv = 0;
	//	if( g_pGameApp->GetScriptMgr()->DoString( buf, "u-d", dwForgeValue, &nStoneLv ) )
	//	{
	//		forge.nStoneLevel[ StoneNum ] = nStoneLv;
	//		forge.nLevel += nStoneLv;

	//		hint = "";
	//		if( g_pGameApp->GetScriptMgr()->DoString( pStone->szHintFunc, "u-s", nStoneLv, &hint ) )
	//		{
	//			strncpy( forge.szStoneHint[StoneNum], hint.c_str(), sizeof(forge.szStoneHint[StoneNum]) );
	//		}
	//	}
	//	StoneNum++;
	//}

	//forge.nStoneNum = StoneNum;

	//if( nItemID>0 )
	//{
	//	forge.Refresh( nItemID );
	//}
	//return forge;
}

void SItemForge::Refresh( int nItemID )
{
	for( int i=0; i<3; i++ )
	{
		if( pStoneInfo[i] )
			nStoneType[i] = pStoneInfo[i]->nType;
		else
			nStoneType[i] = -1;
	}

	int nEffectID = 0;
	if( !g_pGameApp->GetScriptMgr()->DoString( "Item_Stoneeffect", "ddd-d", nStoneType[0], nStoneType[1], nStoneType[2], &nEffectID ) )
		return;

	nEffectID--;
	if( nEffectID<0 || nEffectID>=ITEM_REFINE_NUM )
		return;

	pRefineInfo = GetItemRefineInfo( nItemID );
	if( !pRefineInfo ) return;

	pEffectInfo = GetItemRefineEffectInfo( pRefineInfo->Value[nEffectID] );
	if( !pEffectInfo ) return;

	if( nLevel>=1 ) 
	{
		nEffectLevel = ( nLevel - 1 ) / 4;
		if( nEffectLevel>3 ) nEffectLevel=3;
	}
}

float SItemForge::GetAlpha( int nTotalLevel )
{
	//static float fLevelAlpha[4] = { 150.0f, 180.0f, 220.0f, 255.0f };
	static float fLevelAlpha[4] = { 80.0f, 140.0f, 200.0f, 255.0f };
	static float fLevelBase[4] = { fLevelAlpha[1] - fLevelAlpha[0], fLevelAlpha[2] - fLevelAlpha[1], fLevelAlpha[3] - fLevelAlpha[2], 0.0f };

	if( nTotalLevel<=1 ) return fLevelAlpha[0] / 255.0f;
	if( nTotalLevel >= 13 ) return 1.0f;

	--nTotalLevel;
	int nLevel = nTotalLevel / 4;
	return ( fLevelAlpha[ nLevel ] + (float)(nTotalLevel % 4) / 4.0f * fLevelBase[ nLevel ] ) / 255.0f;
}
