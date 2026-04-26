--------------
-- NPC TALK --
--------------
frmNPCchat = UI_CreateForm( "frmNPCchat", FALSE, 320, 285, 240, 70, TRUE, FALSE )
UI_ShowForm( frmNPCchat, TRUE )
UI_AddFormToTemplete( frmNPCchat, FORM_MAIN )
--UI_FormSetHotKey( frmNPCchat, ALT_KEY, HOTKEY_V )
UI_SetFormStyle( frmNPCchat, 2 )
UI_SetIsDrag( frmNPCchat, TRUE )

imgBakLeft = UI_CreateCompent( frmNPCchat, IMAGE_TYPE, "imgBakLeft", 256, 256, 0, 0 )
UI_LoadImage( imgBakLeft, "texture/ui/NPC1.tga", NORMAL, 256, 256, 0, 0 )

imgBakRight = UI_CreateCompent( frmNPCchat, IMAGE_TYPE, "imgBakRight", 64, 256, 256, 0 )
UI_LoadImage( imgBakRight, "texture/ui/NPC2.tga", NORMAL, 64, 256, 0, 0 )

imgBakLeftD = UI_CreateCompent( frmNPCchat, IMAGE_TYPE, "imgBakLeftD", 256, 29, 0, 256 )
UI_LoadImage( imgBakLeftD, "texture/ui/NPC3.tga", NORMAL, 256, 29, 0, 0 )

imgBakRightD = UI_CreateCompent( frmNPCchat, IMAGE_TYPE, "imgBakRightD", 64, 29, 256, 256 )
UI_LoadImage( imgBakRightD, "texture/ui/NPC2.tga", NORMAL, 64, 29, 64, 0 )

labTitle = UI_CreateCompent( frmNPCchat, LABELEX_TYPE, "labTitle", 400, 150, 130, 4 )
UI_SetCaption( labTitle, "Dialogue")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmNPCchat, BUTTON_TYPE, "btnClose", 14, 14, 299, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

labNPCCity = UI_CreateCompent( frmNPCchat, LABELEX_TYPE, "labNPCCity", 34, 11, 53, 29 )
UI_SetCaption( labNPCCity, "" )
UI_SetTextColor( labNPCCity, COLOR_WHITE )
UI_SetLabelExFont( labNPCCity, DEFAULT_FONT, TRUE, COLOR_BLACK )

labNPCName = UI_CreateCompent( frmNPCchat, LABELEX_TYPE, "labNPCName", 155, 11, 53, 44 )
UI_SetCaption( labNPCName, "" )
UI_SetTextColor( labNPCName, COLOR_WHITE )
UI_SetLabelExFont( labNPCName, DEFAULT_FONT, TRUE, COLOR_BLACK )

memCtrl = UI_CreateCompent( frmNPCchat, MEMO_TYPE, "memCtrl", 287, 210, 27, 48 )
UI_SetTextColor( memCtrl, COLOR_BLACK )
UI_SetCaption( memCtrl, "" ) 
UI_SetMemoMaxNumPerRow(memCtrl, 42 )
UI_SetMemoPageShowNum(memCtrl, 8 )
UI_SetMemoRowHeight( memCtrl , 22 )

scrollid = UI_GetScroll( memCtrl )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

btnNo = UI_CreateCompent( frmNPCchat, BUTTON_TYPE, "btnNo", 41, 19, 138, 258 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 214, TRUE )

---------------
-- Trade NPC --
---------------
frmNPCtrade = UI_CreateForm( "frmNPCtrade", FALSE, 198, 394, 240, 168, TRUE, FALSE )
--UI_FormSetHotKey( frmNPCtrade, ALT_KEY, HOTKEY_V )
--UI_LoadFormImage( frmNPCtrade, "texture/ui/Npctrade.dds", 233, 357, 0, 0, "", 0, 0 )
UI_ShowForm( frmNPCtrade, TRUE )
UI_AddFormToTemplete( frmNPCtrade, FORM_MAIN )
UI_SetIsDrag( frmNPCtrade, TRUE )
--UI_SetFormStyle( frmNPCtrade, 2 )--- top-left

imgBakTop = UI_CreateCompent( frmNPCtrade, IMAGE_TYPE, "imgBakTop", 198, 256, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/PublicD.png", NORMAL, 198, 256, 0, 0 )
imgBakDown = UI_CreateCompent( frmNPCtrade, IMAGE_TYPE, "imgBakDown", 198, 138, 0, 256 )
UI_LoadImage( imgBakDown, "texture/ui/PublicD2.png", NORMAL, 198, 138, 0, 0 )

btnClose = UI_CreateCompent( frmNPCtrade, BUTTON_TYPE, "btnClose", 14, 14, 177, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labTitle = UI_CreateCompent( frmNPCtrade, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Item Trade")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgBakTop1 = UI_CreateCompent( frmNPCtrade, IMAGE_TYPE, "imgBakTop1", 177, 256, 7, 55 )
UI_LoadImage( imgBakTop1, "texture/ui/PublicD3.dds", NORMAL, 177, 256, 0, 0 )
imgBakDown1 = UI_CreateCompent( frmNPCtrade, IMAGE_TYPE, "imgBakDown1", 177, 57, 7, 311 )
UI_LoadImage( imgBakDown1, "texture/ui/PublicD2.png", NORMAL, 177, 57, 0, 138 )

grdNPCtradeWeapon = UI_CreateCompent( frmNPCtrade, GOODS_GRID_TYPE, "grdNPCtradeWeapon", 189, 313, 0, 0 ) 
UI_SetGridSpace( grdNPCtradeWeapon, 4, 4 )
UI_SetGridContent( grdNPCtradeWeapon, 30, 4 )
UI_SetGridUnitSize( grdNPCtradeWeapon, 32, 32 )
UI_SetMargin( grdNPCtradeWeapon, 17, 6, 0, 4 )

scrollid = UI_GetScroll( grdNPCtradeWeapon )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

grdNPCtradeEquip = UI_CreateCompent( frmNPCtrade, GOODS_GRID_TYPE, "grdNPCtradeEquip", 189, 313, 0, 0 ) 
UI_SetGridSpace( grdNPCtradeEquip, 4, 4 )
UI_SetGridContent( grdNPCtradeEquip, 30, 4 )
UI_SetGridUnitSize( grdNPCtradeEquip, 32, 32 )
UI_SetMargin( grdNPCtradeEquip, 17, 6, 0, 4 )

scrollid = UI_GetScroll( grdNPCtradeEquip )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

grdNPCtradeOther = UI_CreateCompent( frmNPCtrade, GOODS_GRID_TYPE, "grdNPCtradeOther", 189, 313, 0, 0) 
UI_SetGridSpace( grdNPCtradeOther, 4, 4 )
UI_SetGridContent( grdNPCtradeOther, 30, 4 )
UI_SetGridUnitSize( grdNPCtradeOther, 32, 32 )
UI_SetMargin( grdNPCtradeOther, 17, 6, 0, 4 )

scrollid = UI_GetScroll( grdNPCtradeOther )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

pgeNPCtrade = UI_CreateCompent( frmNPCtrade, PAGE_TYPE, "pgeNPCtrade", 187, 313, 7, 56 )
UI_SetPageButton( pgeNPCtrade, PAGE_BUTTON_CUSTOM, 39, 19 )

itemid = UI_CreatePageItem( pgeNPCtrade )
ttlWeapon = UI_GetPageItemObj( itemid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlWeapon, "texture/ui/PublicC.tga", PAGE_ITEM_TITLE_NORMAL, 55, 35, 58, 105 )
UI_LoadImage( ttlWeapon, "texture/ui/PublicC.tga", PAGE_ITEM_TITLE_ACTIVE, 55, 35, 0, 105 )
UI_SetPos( ttlWeapon, 13, -30 )
UI_SetSize( ttlWeapon, 55, 35 )
UI_AddCompent( itemid, grdNPCtradeWeapon )

itemid = UI_CreatePageItem( pgeNPCtrade )
ttlEquip = UI_GetPageItemObj( itemid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlEquip, "texture/ui/PublicC.tga", PAGE_ITEM_TITLE_NORMAL, 55, 35, 57, 140 )
UI_LoadImage( ttlEquip, "texture/ui/PublicC.tga", PAGE_ITEM_TITLE_ACTIVE, 55, 35, 0, 140 )
UI_SetPos( ttlEquip, 65, -30 )
UI_SetSize( ttlEquip, 55, 35 )
UI_AddCompent( itemid, grdNPCtradeEquip )

itemid = UI_CreatePageItem( pgeNPCtrade )
ttlOther = UI_GetPageItemObj( itemid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlOther, "texture/ui/PublicC.tga", PAGE_ITEM_TITLE_NORMAL, 55, 35, 57, 175 )
UI_LoadImage( ttlOther, "texture/ui/PublicC.tga", PAGE_ITEM_TITLE_ACTIVE, 55, 35, 0, 175)
UI_SetPos( ttlOther, 111, -30 )
UI_SetSize( ttlOther, 55, 35 )
UI_AddCompent( itemid, grdNPCtradeOther )

-----------------------------------------------------------------------
-- NPC trade quantity dialog
-----------------------------------------------------------------------	
-- Create NPC trade quantity form
frmNPCTradeNumber = UI_CreateForm( "frmNPCTradeNumber", FALSE, 185, 101, 240, 268, TRUE, FALSE )
--UI_FormSetHotKey( frmNPCTradeNumber, ALT_KEY, HOTKEY_V ) 
UI_LoadFormImage( frmNPCTradeNumber, "texture/ui/sure.dds", 185, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmNPCTradeNumber, TRUE )
UI_AddFormToTemplete( frmNPCTradeNumber, FORM_MAIN )
UI_SetIsDrag( frmNPCTradeNumber, TRUE )
UI_SetFormStyle( frmNPCTradeNumber, 2 )--- top-left
UI_SetAlpha( frmNPCTradeNumber, 255 )


-- Close button
btnClose = UI_CreateCompent( frmNPCTradeNumber, BUTTON_TYPE, "btnClose", 14, 14, 164, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

-- Title
--labtitle = UI_CreateCompent( frmNPCTradeNumber, LABELEX_TYPE, "labtitle", 47, 11, 70, 4 )
--UI_SetCaption( labtitle, "Trade Quantity" )
--UI_SetTextColor( labtitle, COLOR_WHITE )
--UI_SetLabelExFont( labtitle, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Quantity label
labNumber = UI_CreateCompent( frmNPCTradeNumber, LABELEX_TYPE, "labNumber", 23, 11, 44, 32 )
UI_SetCaption( labNumber, "Quantity" )
UI_SetTextColor( labNumber, COLOR_BLACK )
UI_SetLabelExFont( labNumber, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Total price label
labSum = UI_CreateCompent( frmNPCTradeNumber, LABELEX_TYPE, "labSum", 23, 11, 44, 54 )
UI_SetCaption( labSum, "Total Price" )
UI_SetTextColor( labSum, COLOR_BLACK )
UI_SetLabelExFont( labSum, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Quantity and total display background
imgNumber = UI_CreateCompent( frmNPCTradeNumber, IMAGE_TYPE, "imgNumber", 87, 18, 71, 28 )
UI_LoadImage( imgNumber, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )
imgSum = UI_CreateCompent( frmNPCTradeNumber, IMAGE_TYPE, "imgSum", 87, 18, 71, 50 )
UI_LoadImage( imgSum, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )


-- Quantity input field
edtNPCTradeNumber = UI_CreateCompent( frmNPCTradeNumber, EDIT_TYPE, "edtNPCTradeNumber", 87, 11, 79, 32 )
UI_SetTextColor( edtNPCTradeNumber, COLOR_BLACK )
UI_SetEditMaxNum( edtNPCTradeNumber, 3 )
UI_SetEditMaxNumVisible( edtNPCTradeNumber, 3 )

-- Total price display
labNPCTradeNumber = UI_CreateCompent( frmNPCTradeNumber, LABELEX_TYPE, "labNPCTradeNumber", 87, 11, 79, 54 )
UI_SetCaption( labNPCTradeNumber, "0" )
UI_SetTextColor( labNPCTradeNumber, COLOR_WHITE )
UI_SetLabelExFont( labNPCTradeNumber, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Trade button
btnYes = UI_CreateCompent( frmNPCTradeNumber, BUTTON_TYPE, "btnYes", 41, 19, 85, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 40, TRUE )

-- Cancel button
btnNo = UI_CreateCompent( frmNPCTradeNumber, BUTTON_TYPE, "btnNo", 41, 19, 130, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, FALSE )

--------------
-- NPC Bank --
-------------------------
frmNPCstorage = UI_CreateForm( "frmNPCstorage", FALSE, 198, 394, 100, 100, TRUE, FALSE )
--UI_FormSetHotKey( frmNPCstorage, ALT_KEY, HOTKEY_V ) 
UI_ShowForm( frmNPCstorage, TRUE )
UI_AddFormToTemplete( frmNPCstorage, FORM_MAIN )
UI_SetIsDrag( frmNPCstorage, TRUE )

imgBakTop = UI_CreateCompent( frmNPCstorage, IMAGE_TYPE, "imgBakTop", 198, 256, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/save1.tga", NORMAL, 198, 256, 0, 0 )
imgBakDown = UI_CreateCompent( frmNPCstorage, IMAGE_TYPE, "imgBakDown", 198, 138, 0, 256 )
UI_LoadImage( imgBakDown, "texture/ui/save2.tga", NORMAL, 198, 138, 0, 0 )

--title
labTitle = UI_CreateCompent( frmNPCstorage, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Bank")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmNPCstorage, BUTTON_TYPE, "btnClose", 14, 14, 178, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labOwnerName = UI_CreateCompent( frmNPCstorage, LABELEX_TYPE, "labOwnerName", 119, 12, 54, 42 )
UI_SetCaption( labOwnerName, "Personal Storage" )
UI_SetTextColor( labOwnerName, COLOR_BLACK )
UI_SetLabelExFont( labOwnerName, DEFAULT_FONT, TRUE, COLOR_WHITE )

grdNPCstorage = UI_CreateCompent( frmNPCstorage, GOODS_GRID_TYPE, "grdNPCstorage", 184, 305, 7, 71 ) 
UI_SetGridSpace( grdNPCstorage, 4, 4 )
UI_SetGridContent( grdNPCstorage, 8, 4 )
UI_SetGridUnitSize( grdNPCstorage, 32, 32 )
UI_SetMargin( grdNPCstorage, 21, 4, 0, 0 )

scrollid = UI_GetScroll( grdNPCstorage )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

-----------------
-- NPC Mission --
-----------------
frmNPCMission = UI_CreateForm( "frmNPCMission", FALSE, 235, 394, 235, 150, TRUE, FALSE )
--UI_FormSetHotKey( frmNPCMission, ALT_KEY, HOTKEY_V ) 
UI_ShowForm( frmNPCMission, FALSE )
UI_AddFormToTemplete( frmNPCMission, FORM_MAIN )
UI_SetIsDrag( frmNPCMission, TRUE )
--UI_SetFormStyle( frmNPCMission, 0 )
--UI_SetAlpha( frmNPCMission, 200 )

imgBakTop = UI_CreateCompent( frmNPCMission, IMAGE_TYPE, "imgBakTop", 235, 256, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/QuestT.tga", NORMAL, 235, 256, 0, 0 )
imgBakDown = UI_CreateCompent( frmNPCMission, IMAGE_TYPE, "imgBakDown", 235, 138, 0, 256 )
UI_LoadImage( imgBakDown, "texture/ui/QuestNew.tga", NORMAL, 235, 138, 0, 0 )

labTitle = UI_CreateCompent( frmNPCMission, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Quest")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmNPCMission, BUTTON_TYPE, "btnClose", 14, 14, 214, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

labNPCCity = UI_CreateCompent( frmNPCMission, LABELEX_TYPE, "labNPCCity", 34, 11, 53, 29 )
UI_SetCaption( labNPCCity, "" )
UI_SetTextColor( labNPCCity, COLOR_WHITE )
UI_SetLabelExFont( labNPCCity, DEFAULT_FONT, TRUE, COLOR_BLACK )

labNPCName = UI_CreateCompent( frmNPCMission, LABELEX_TYPE, "labNPCName", 155, 11, 53, 44 )
UI_SetCaption( labNPCName, "" )
UI_SetTextColor( labNPCName, COLOR_WHITE )
UI_SetLabelExFont( labNPCName, DEFAULT_FONT, TRUE, COLOR_BLACK )

memMission = UI_CreateCompent( frmNPCMission, MEMOEX_TYPE, "memMission", 215, 326, 17, 40 )
UI_SetTextColor( memMission, COLOR_BLACK )
UI_SetCaption( memMission, "" ) 
UI_SetMemoMaxNumPerRow(memMission, 30 )
UI_SetMemoPageShowNum(memMission, 17 )
UI_SetMemoRowHeight( memMission, 16 )

scrollid = UI_GetScroll( memMission )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

btnYes = UI_CreateCompent( frmNPCMission, BUTTON_TYPE, "btnYes", 41, 19, 91, 370 )
UI_LoadButtonImage( btnYes, "texture/ui/SystemBotton3.tga", 41, 19, 0, 0, TRUE )

btnComplete = UI_CreateCompent( frmNPCMission, BUTTON_TYPE, "btnComplete", 41, 19, 91, 370 )
UI_LoadButtonImage( btnComplete, "texture/ui/SystemBotton3.tga", 41, 19, 0, 76, TRUE )

btnNo = UI_CreateCompent( frmNPCMission, BUTTON_TYPE, "btnNo", 41, 19, 139, 370 )
UI_LoadButtonImage( btnNo, "texture/ui/Quest1.tga", 41, 19, 82, 199, TRUE )

labYes = UI_CreateCompent( frmNPCMission, LABELEX_TYPE, "labYes", 42, 10, 29, 409 )
UI_SetCaption( labYes, "" )
UI_SetTextColor( labYes, COLOR_WHITE )
UI_SetLabelExFont( labYes, DEFAULT_FONT, TRUE, COLOR_BLACK )

labComplete = UI_CreateCompent( frmNPCMission, LABELEX_TYPE, "labComplete", 42, 10, 29, 409 )
UI_SetCaption( labComplete, "" )
UI_SetTextColor( labComplete, COLOR_WHITE )
UI_SetLabelExFont( labComplete, DEFAULT_FONT, TRUE, COLOR_BLACK )

labNo = UI_CreateCompent( frmNPCMission, LABELEX_TYPE, "labNo", 42, 10, 180, 409 )
UI_SetCaption( labNo, "" )
UI_SetTextColor( labNo, COLOR_WHITE )
UI_SetLabelExFont( labNo, DEFAULT_FONT, TRUE, COLOR_BLACK )


--------------------------
-- Forge npc ui By aLex --
--------------------------	
frmNPCforge = UI_CreateForm( "frmNPCforge", FALSE, 407, 275, 240, 70, TRUE, FALSE )
UI_ShowForm( frmNPCforge, TRUE )
UI_AddFormToTemplete( frmNPCforge, FORM_MAIN )
--UI_FormSetHotKey( frmNPCforge, ALT_KEY, HOTKEY_Z )
UI_SetFormStyle( frmNPCforge, 2 )--- top-left
UI_SetIsDrag( frmNPCforge, TRUE )
--UI_SetAlpha( frmNPCforge, 200 )

imgBakTopLeft = UI_CreateCompent( frmNPCforge, IMAGE_TYPE, "imgBakTopLeft", 256, 256, 0, 0 )
UI_LoadImage( imgBakTopLeft, "texture/ui/forge1.tga", NORMAL, 256, 256, 0, 0 )
imgBakTopRight = UI_CreateCompent( frmNPCforge, IMAGE_TYPE, "imgBakTopRight", 151, 256, 256, 0 )
UI_LoadImage( imgBakTopRight, "texture/ui/forge2.tga", NORMAL, 151, 256, 0, 0 )
imgBakDownLeft = UI_CreateCompent( frmNPCforge, IMAGE_TYPE, "imgBakDownLeft", 256, 18, 0, 256 )
UI_LoadImage( imgBakDownLeft, "texture/ui/forge3.tga", NORMAL, 256, 18, 0, 0 )
imgBakDownRight = UI_CreateCompent( frmNPCforge, IMAGE_TYPE, "imgBakDownRight", 151, 18, 256, 256 )
UI_LoadImage( imgBakDownRight, "texture/ui/forge3.tga", NORMAL, 151, 18, 105, 18 )

imgMillingTitle = UI_CreateCompent( frmNPCforge, IMAGE_TYPE, "imgMillingTitle", 53, 20, 180, 0 )
UI_LoadImage( imgMillingTitle, "texture/ui/forge3.tga", NORMAL, 53, 20, 164, 44 )

--labTitle = UI_CreateCompent( frmNPCforge, LABELEX_TYPE, "labTitle", 400, 150, 182, 4 )
--UI_SetCaption( labTitle, "Item Forging")
--UI_SetTextColor( labTitle, COLOR_BLACK )
--UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmNPCforge, BUTTON_TYPE, "btnClose", 14, 14, 386, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

cmdForgeItem1 = UI_CreateCompent( frmNPCforge, COMMAND_ONE_TYPE, "cmdForgeItem1", 32, 32, 20, 100 )	-- Material 1
cmdForgeItem2 = UI_CreateCompent( frmNPCforge, COMMAND_ONE_TYPE, "cmdForgeItem2", 32, 32, 20, 151 )	-- Material 2
cmdForgeItem0 = UI_CreateCompent( frmNPCforge, COMMAND_ONE_TYPE, "cmdForgeItem0", 32, 32, 20, 49 )	-- Forge target
UI_SetHint( cmdForgeItem0, "Forge item" )
UI_SetHint( cmdForgeItem1, "Attribute Gem" )
UI_SetHint( cmdForgeItem2, "Refining Gem" )

labForgeGold = UI_CreateCompent( frmNPCforge, LABELEX_TYPE, "labForgeGold", 140, 12, 55, 247 )
UI_SetCaption( labForgeGold, "" )
UI_SetTextColor( labForgeGold, COLOR_BLACK )
UI_SetLabelExFont( labForgeGold, DEFAULT_FONT, TRUE, COLOR_WHITE )

proNPCforge = UI_CreateCompent( frmNPCforge, PROGRESS_TYPE, "proNPCforge", 354, 8, 25, 210 )
UI_LoadImage( proNPCforge, "texture/ui/forge3.tga", PROGRESS_PROGRESS, 192, 8, 0, 36 )

memForgeItemState = UI_CreateCompent( frmNPCforge, MEMO_TYPE, "memForgeItemState", 294, 124, 82, 54 )
UI_SetTextColor( memForgeItemState, COLOR_BLACK )
UI_SetMemoMaxNumPerRow( memForgeItemState, 24 )
UI_SetMemoPageShowNum( memForgeItemState, 7 )
UI_SetMemoRowHeight( memForgeItemState, 17 )

lstForgeItemState = UI_CreateCompent( frmNPCforge, LIST_TYPE, "lstForgeItemState", 294, 124, 82, 54 )
UI_SetListRowHeight( lstForgeItemState, 17 )
UI_SetListFontColor( lstForgeItemState, 4293591039, 4293591039 )
UI_SetMargin( lstForgeItemState, 8,3,6,0 )
UI_SetIsShow( lstForgeItemState, TRUE )
UI_AddListText( lstForgeItemState, "" )
UI_AddListText( lstForgeItemState, "" )
UI_AddListText( lstForgeItemState, "" )
UI_AddListText( lstForgeItemState, "" )
UI_AddListText( lstForgeItemState, "" )
UI_AddListText( lstForgeItemState, "" )
UI_AddListText( lstForgeItemState, "" )

btnForgeYes = UI_CreateCompent( frmNPCforge, BUTTON_TYPE, "btnForgeYes", 41, 19, 249, 241 )
UI_LoadButtonImage( btnForgeYes, "texture/ui/forge3.tga", 41, 19, 0, 63, TRUE )

btnMillingYes = UI_CreateCompent( frmNPCforge, BUTTON_TYPE, "btnMillingYes", 41, 19, 249, 241 )
UI_LoadButtonImage( btnMillingYes, "texture/ui/forge3.tga", 41, 19, 0, 44, TRUE )

btnForgeNo = UI_CreateCompent( frmNPCforge, BUTTON_TYPE, "btnForgeNo", 41, 19, 316, 241 )
UI_LoadButtonImage( btnForgeNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnForgeNo, BUTTON_CLOSE )

-----------------------------------------------------------------------
-- NPC polishing
-----------------------------------------------------------------------	
--- Create NPC polishing form
frmNPCshine = UI_CreateForm( "frmNPCshine", FALSE, 237, 217, 240, 70, TRUE, FALSE )
UI_LoadFormImage( frmNPCshine, "texture/ui/forge4.tga", 237, 217, 0, 0, "", 0, 0 )
UI_ShowForm( frmNPCshine, TRUE )
UI_AddFormToTemplete( frmNPCshine, FORM_MAIN )
--UI_FormSetHotKey( frmNPCshine, ALT_KEY, HOTKEY_X )
UI_SetFormStyle( frmNPCshine, 2 )--- top-left
UI_SetIsDrag( frmNPCshine, TRUE )
--UI_SetAlpha( frmNPCshine, 200 )

--title
--labTitle = UI_CreateCompent( frmNPCforge, LABELEX_TYPE, "labTitle", 400, 150, 182, 4 )
--UI_SetCaption( labTitle, "Item Forging")
--UI_SetTextColor( labTitle, COLOR_BLACK )
--UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Close button
btnClose = UI_CreateCompent( frmNPCshine, BUTTON_TYPE, "btnClose", 14, 14, 216, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

-- Polish item and catalyst slot
cmdShineItem = UI_CreateCompent( frmNPCshine, COMMAND_ONE_TYPE, "cmdShineItem", 32, 32, 16, 61 )	-- Result

-- Gold cost display
labShineGold = UI_CreateCompent( frmNPCshine, LABELEX_TYPE, "labShineGold", 110, 12, 68, 139 )
UI_SetCaption( labShineGold, "" )
UI_SetTextColor( labShineGold, COLOR_BLACK )
UI_SetLabelExFont( labShineGold, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Polish progress bar
proNPCshine = UI_CreateCompent( frmNPCshine, PROGRESS_TYPE, "proNPCshine", 198, 8, 20, 170 )
UI_LoadScaleImage( proNPCshine, "texture/ui/forge3.tga", PROGRESS_PROGRESS, 192, 8, 0, 56, 1.0, 1.0 )

-- Item state display
memForgeItemState = UI_CreateCompent( frmNPCshine, MEMO_TYPE, "memForgeItemState", 152, 75, 67, 36 )
UI_SetTextColor( memForgeItemState, COLOR_BLACK )
UI_SetMemoMaxNumPerRow( memForgeItemState, 16 )
UI_SetMemoPageShowNum( memForgeItemState, 4 )
UI_SetMemoRowHeight( memForgeItemState, 17 )

-- Dead code: lstForgeItemState is created later after frmConAsk is defined (line 573+)
-- lstForgeItemState = UI_CreateListView( frmConAsk, "lstForgeItemState", 152, 75, 67, 36, 1, 2 )


-- Polish confirm/cancel buttons
btnShineYes = UI_CreateCompent( frmNPCshine, BUTTON_TYPE, "btnShineYes", 41, 19, 54, 190 )
UI_LoadButtonImage( btnShineYes, "texture/ui/forge3.tga", 41, 19, 0, 63, TRUE )

btnShineNo = UI_CreateCompent( frmNPCshine, BUTTON_TYPE, "btnShineNo", 41, 19, 146, 190 )
UI_LoadButtonImage( btnShineNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

------------------------------------
-- Gem combine / Item upgrade NPC --
------------------------------------
frmMakeEquip = UI_CreateForm( "frmMakeEquip", FALSE, 226, 440, 240, 70, TRUE, FALSE )
UI_ShowForm( frmMakeEquip, TRUE )
UI_AddFormToTemplete( frmMakeEquip, FORM_MAIN )
--UI_FormSetHotKey( frmMakeEquip, ALT_KEY, HOTKEY_V )
UI_SetFormStyle( frmMakeEquip, 2 )--- top-left
UI_SetIsDrag( frmMakeEquip, TRUE )

imgBakTop = UI_CreateCompent( frmMakeEquip, IMAGE_TYPE, "imgBakTop", 226, 256, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/makeequip1.tga", NORMAL, 226, 256, 0, 0 ) 
imgBakDown = UI_CreateCompent( frmMakeEquip, IMAGE_TYPE, "imgBakDown", 226, 184, 0, 256 )
UI_LoadImage( imgBakDown, "texture/ui/makeequip2.tga", NORMAL, 226, 184, 0, 0 )

--title
--labTitle = UI_CreateCompent( frmMakeEquip, LABELEX_TYPE, "labTitle", 400, 150, 87, 6 )
--UI_SetCaption( labTitle, "Item combining")
--UI_SetTextColor( labTitle, COLOR_BLACK )
--UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmMakeEquip, BUTTON_TYPE, "btnClose", 14, 14, 205, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

cmdRouLeau = UI_CreateCompent( frmMakeEquip, COMMAND_ONE_TYPE, "cmdRouLeau", 32, 32, 97, 67 )	-- Scroll
cmdItem0 = UI_CreateCompent( frmMakeEquip, COMMAND_ONE_TYPE, "cmdItem0", 32, 32, 25, 122 )	-- Material 1
cmdItem1 = UI_CreateCompent( frmMakeEquip, COMMAND_ONE_TYPE, "cmdItem1", 32, 32, 172, 122 )	-- Material 2
cmdItem2 = UI_CreateCompent( frmMakeEquip, COMMAND_ONE_TYPE, "cmdItem2", 32, 32, 49, 207 )	-- Material 3
cmdItem3 = UI_CreateCompent( frmMakeEquip, COMMAND_ONE_TYPE, "cmdItem3", 32, 32, 146, 207 )	-- Material 4
cmdLastEquip = UI_CreateCompent( frmMakeEquip, COMMAND_ONE_TYPE, "cmdLastEquip", 32, 32, 97, 142 )	-- Result
UI_SetHint( cmdLastEquip, "Combine item" )
UI_SetHint( cmdItem0, "Ingredient 1" )
UI_SetHint( cmdItem1, "Ingredient 2" )
UI_SetHint( cmdItem2, "Ingredient 3" )
UI_SetHint( cmdItem3, "Ingredient 4" )
UI_SetHint( cmdRouLeau, "Scroll" )

labForgeGold = UI_CreateCompent( frmMakeEquip, LABELEX_TYPE, "labForgeGold", 112, 12, 50, 386 )
UI_SetCaption( labForgeGold, "" )
UI_SetTextColor( labForgeGold, COLOR_BLACK )
UI_SetLabelExFont( labForgeGold, DEFAULT_FONT, TRUE, COLOR_WHITE )

memForgeItemState = UI_CreateCompent( frmMakeEquip, MEMO_TYPE, "memForgeItemState", 180, 77, 24, 286 )
UI_SetTextColor( memForgeItemState, COLOR_BLACK )
UI_SetMemoMaxNumPerRow( memForgeItemState, 16 )
UI_SetMemoPageShowNum( memForgeItemState, 4 )
UI_SetMemoRowHeight( memForgeItemState, 18 )

-- Dead code: frmConAsk is not defined yet at this point (defined on line 586+)
-- lstForgeItemState = UI_CreateListView( frmConAsk, "lstForgeItemState", 180, 77, 24, 286, 1, 2 )
-- UI_SetListRowHeight( lstForgeItemState, 18 )
-- UI_SetListFontColor( lstForgeItemState, 4293591039, 4293591039 )
-- UI_SetMargin( lstForgeItemState, 8,5,0,0 )
-- UI_SetIsShow( lstForgeItemState, TRUE )
-- UI_AddListText( lstForgeItemState, "" )
-- UI_AddListText( lstForgeItemState, "" )
-- UI_AddListText( lstForgeItemState, "" )
-- UI_AddListText( lstForgeItemState, "" )

btnForgeYes = UI_CreateCompent( frmMakeEquip, BUTTON_TYPE, "btnForgeYes", 41, 19, 52, 411 )
UI_LoadButtonImage( btnForgeYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnForgeNo = UI_CreateCompent( frmMakeEquip, BUTTON_TYPE, "btnForgeNo", 41, 19, 135, 411 )
UI_LoadButtonImage( btnForgeNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )UI_SetButtonModalResult( btnForgeNo, BUTTON_CLOSE )


--------------------
-- guild list npc --
--------------------
frmConAsk = UI_CreateForm( "frmConAsk", FALSE, 525, 274, 100, 200, TRUE, FALSE )
--UI_FormSetHotKey( frmConAsk, ALT_KEY, HOTKEY_V ) 
UI_ShowForm( frmConAsk, TRUE )
UI_AddFormToTemplete( frmConAsk, FORM_MAIN )
UI_SetIsDrag( frmConAsk, TRUE )
UI_SetFormStyle( frmConAsk, 2 )
UI_SetAlpha( frmConAsk, 255 )

imgBakAsk2 = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgBakAsk2", 13, 256, 512, 0 )
UI_LoadImage( imgBakAsk2, "texture/ui/ShipD2.tga", NORMAL, 13, 256, 147, 0 )

imgBakAsk3 = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgBakAsk3", 256, 18, 0, 256 )
UI_LoadImage( imgBakAsk3, "texture/ui/ShipD.tga", NORMAL, 256, 18, 0, 210 )

imgBakAsk4 = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgBakAsk4", 256, 18, 256, 256 )
UI_LoadImage( imgBakAsk4, "texture/ui/ShipD.tga", NORMAL, 256, 18, 0, 228 )

imgBakAsk5 = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgBakAsk5", 13, 18, 512, 256 )
UI_LoadImage( imgBakAsk5, "texture/ui/ShipD2.tga", NORMAL, 13, 18, 160, 0 )

imgMaxim = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgMaxim", 187, 26, 117, 37 )
UI_LoadImage( imgMaxim, "texture/ui/help3.tga", NORMAL, 187, 26, 0, 145 )


imgBakAskH = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgBakAskH", 256, 256, 0, 0 )
UI_LoadImage( imgBakAskH, "texture/ui/ConAsk1.tga", NORMAL, 256, 256, 0, 0 )

imgBakAskH1 = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgBakAskH1", 256, 256, 256, 0 )
UI_LoadImage( imgBakAskH1, "texture/ui/ConAsk2.tga", NORMAL, 256, 256, 0, 0 )

imgName1 = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgName1", 102, 26, 410, 37 )
UI_LoadImage( imgName1, "texture/ui/help3.tga", NORMAL, 102, 26, 0, 180 )

--imgbackJ = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgbackJ", 256, 256, 165, 50 )
--UI_LoadImage( imgbackJ, "texture/ui/HJback.tga", NORMAL, 256, 256, 0, 0 )
--UI_SetImageAlpha( imgbackJ, 144 )

--imgTitle
imgTitleJ = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgTitleJ", 61, 13, 8, 4 )
--UI_LoadImage( imgTitleJ, "texture/ui/word.dds", NORMAL, 61, 13, 172, 204 )

--labTitle
labTitleJ = UI_CreateCompent( frmConAsk, LABELEX_TYPE, "labTitleJ", 100, 11, 8, 4 )
UI_SetCaption( labTitleJ, "Navy Recruitment Station" )
UI_SetTextColor( labTitleJ, COLOR_BLACK )
UI_SetLabelExFont( labTitleJ, DEFAULT_FONT, TRUE, COLOR_WHITE )

--labInfo
labInfoJ = UI_CreateCompent( frmConAsk, LABELEX_TYPE, "labInfoJ", 100, 11, 80, 5 )
UI_SetCaption( labInfoJ, "Can only apply for occupied Navy division" )
UI_SetTextColor( labInfoJ, COLOR_BLACK )
UI_SetLabelExFont( labInfoJ, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgBakAsk = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgBakAsk", 256, 256, 0, 0 )
UI_LoadImage( imgBakAsk, "texture/ui/ConAsk1D.tga", NORMAL, 256, 256, 0, 0 )

imgBakAsk1 = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgBakAsk1", 256, 256, 256, 0 )
UI_LoadImage( imgBakAsk1, "texture/ui/ConAsk2D.tga", NORMAL, 256, 256, 0, 0 )

imgName2 = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgName2", 102, 26, 410, 37 )
UI_LoadImage( imgName2, "texture/ui/help3.tga", NORMAL, 102, 26, 102, 180 )

--imgbackD = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgbackD", 256, 256, 185, 80 )
--UI_LoadImage( imgbackD, "texture/ui/HDback.tga", NORMAL, 256, 256, 0, 0 )
--UI_SetImageAlpha( imgbackD, 134 )


imgTitleD = UI_CreateCompent( frmConAsk, IMAGE_TYPE, "imgTitleD", 61, 13, 8, 4 )
--UI_LoadImage( imgTitleD, "texture/ui/word.dds", NORMAL, 61, 13, 166, 218 )

labTitleD = UI_CreateCompent( frmConAsk, LABELEX_TYPE, "labTitleD", 100, 11, 8, 4 )
UI_SetCaption( labTitleD, "Join the Pirates" )
UI_SetTextColor( labTitleD, COLOR_BLACK )
UI_SetLabelExFont( labTitleD, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnApply = UI_CreateCompent( frmConAsk, BUTTON_TYPE, "btnApply", 41, 19, 220, 250 )
UI_LoadButtonImage( btnApply, "texture/ui/SysBotton4.tga", 41, 19, 0, 0, TRUE )

btnName = UI_CreateCompent( frmConAsk, BUTTON_TYPE, "btnName", 102, 26, 14, 37 )
UI_LoadImage( btnName, "texture/ui/SysBotton4.tga", 0, 102, 26, 24, 173)
UI_LoadImage( btnName, "texture/ui/SysBotton4.tga", 1, 102, 26, 24, 173)
UI_LoadImage( btnName, "texture/ui/SysBotton4.tga", 2, 102, 26, 126, 173)

btnNum = UI_CreateCompent( frmConAsk, BUTTON_TYPE, "btnNum", 37, 26, 306, 37 )
UI_LoadImage( btnNum, "texture/ui/SysBotton4.tga", 0, 37, 26, 24, 200 )
UI_LoadImage( btnNum, "texture/ui/SysBotton4.tga", 1, 37, 26, 24, 200 )
UI_LoadImage( btnNum, "texture/ui/SysBotton4.tga", 2, 37, 26, 61, 200)

btnExp = UI_CreateCompent( frmConAsk, BUTTON_TYPE, "btnExp", 66, 26, 343, 37 )
UI_LoadImage( btnExp, "texture/ui/SysBotton4.tga", 0, 66, 26, 24, 228 )
UI_LoadImage( btnExp, "texture/ui/SysBotton4.tga", 1, 66, 26, 24, 228 )
UI_LoadImage( btnExp, "texture/ui/SysBotton4.tga", 2, 66, 26, 90, 228 )

btnClose = UI_CreateCompent( frmConAsk, BUTTON_TYPE, "btnClose", 14, 14, 506, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

btnNo = UI_CreateCompent( frmConAsk, BUTTON_TYPE, "btnNo", 41, 19, 265, 250 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_NO )

lstAsk = UI_CreateListView( frmConAsk, "lstAsk", 510, 178, 12, 65, 5, 2 )
UI_ListViewSetTitle( lstAsk, 0, 102, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstAsk, 1, 188, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstAsk, 2, 38, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstAsk, 3, 67, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstAsk, 4, 101, "", 0, 0, 0, 0 )

scrollid = UI_GetScroll( lstAsk )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )


-----------------------------------------------------------------------
-- Guild password input dialog
-----------------------------------------------------------------------
frmCode = UI_CreateForm( "frmCode",  FALSE, 185, 101, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmCode, "texture/ui/sure.dds", 185, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmCode, FALSE )
UI_AddFormToTemplete( frmCode, FORM_MAIN )
--UI_FormSetHotKey( frmCode, ALT_KEY, HOTKEY_V ) 
UI_SetFormStyle( frmCode, 0 )
UI_SetIsDrag( frmCode, TRUE )

-- Title label
labTitle = UI_CreateCompent( frmCode, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Input password")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Password label
labInfo = UI_CreateCompent( frmCode, LABELEX_TYPE, "labInfo", 23, 11, 50, 30 )
UI_SetCaption( labInfo, "Please input password" )
UI_SetTextColor( labInfo, COLOR_BLACK )
UI_SetLabelExFont( labInfo, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Password input background
imgNumber = UI_CreateCompent( frmCode, IMAGE_TYPE, "imgNumber", 87, 18, 50, 50 )
UI_LoadImage( imgNumber, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

-- Password input field
edtCode = UI_CreateCompent( frmCode, EDIT_TYPE, "edtCode", 71, 11, 58, 54 )
UI_SetTextColor( edtCode, COLOR_BLACK )
UI_SetEditMaxNum( edtCode, 8 )
UI_SetEditMaxNumVisible( edtCode, 8 )

-- Confirm button
btnYes = UI_CreateCompent( frmCode, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetButtonModalResult( btnYes, BUTTON_YES )
UI_SetEditEnterButton( edtCode, btnYes )

-- Cancel button
btnNo = UI_CreateCompent( frmCode, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_NO )

------------------
-- Create guild --
------------------
frmName = UI_CreateForm( "frmName",  FALSE, 185, 136, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmName, "texture/ui/QuestD2.dds", 185, 136, 0, 111, "", 0, 0 )
UI_ShowForm( frmName, FALSE )
UI_AddFormToTemplete( frmName, FORM_MAIN )
--UI_FormSetHotKey( frmName, ALT_KEY, HOTKEY_V ) 
UI_SetFormStyle( frmName, 0 )
UI_SetIsDrag( frmName, TRUE )

labTitle = UI_CreateCompent( frmName, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Input guild name, password")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labNameInfo = UI_CreateCompent( frmName, LABELEX_TYPE, "labNameInfo", 40, 11, 15, 45 )
UI_SetCaption( labNameInfo, "Name:" )
UI_SetTextColor( labNameInfo, COLOR_BLACK )
UI_SetLabelExFont( labNameInfo, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgName = UI_CreateCompent( frmName, IMAGE_TYPE, "imgName", 109, 19, 68, 41 )
UI_LoadImage( imgName, "texture/ui/PublicC.tga", NORMAL, 109, 19, 47, 68 )

edtName = UI_CreateCompent( frmName, EDIT_TYPE, "edtName", 96, 11, 75, 45 )
UI_SetTextColor( edtName, COLOR_BLACK )
UI_SetEditMaxNum( edtName, 16 )
UI_SetEditMaxNumVisible( edtName, 16 )

labCodeInfo = UI_CreateCompent( frmName, LABELEX_TYPE, "labCodeInfo", 40, 11, 15, 70 )
UI_SetCaption( labCodeInfo, "Password:" )
UI_SetTextColor( labCodeInfo, COLOR_BLACK )
UI_SetLabelExFont( labCodeInfo, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgCode = UI_CreateCompent( frmName, IMAGE_TYPE, "imgCode", 109, 19, 68, 65 )
UI_LoadImage( imgCode, "texture/ui/PublicC.tga", NORMAL, 109, 19, 47, 68 )

imgCode2 = UI_CreateCompent( frmName, IMAGE_TYPE, "imgCode2", 109, 19, 68, 87 )
UI_LoadImage( imgCode2, "texture/ui/PublicC.tga", NORMAL, 109, 19, 47, 68 )

edtPCode2 = UI_CreateCompent( frmName, EDIT_TYPE, "edtPCode2", 71, 11, 75, 92 )
UI_SetTextColor( edtPCode2, COLOR_BLACK )
UI_SetEditMaxNum( edtPCode2, 8 )
UI_SetEditMaxNumVisible( edtPCode2, 8 )

edtPCode = UI_CreateCompent( frmName, EDIT_TYPE, "edtPCode", 71, 11, 75, 70 )
UI_SetTextColor( edtPCode, COLOR_BLACK )
UI_SetEditMaxNum( edtPCode, 8 )
UI_SetEditMaxNumVisible( edtPCode, 8 )

labCodeInfo2 = UI_CreateCompent( frmName, LABELEX_TYPE, "labCodeInfo2", 40, 11, 15, 92 )
UI_SetCaption( labCodeInfo2, "Re-Enter:" )
UI_SetTextColor( labCodeInfo2, COLOR_BLACK )
UI_SetLabelExFont( labCodeInfo2, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmName, BUTTON_TYPE, "btnYes", 41, 19, 74, 112 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetButtonModalResult( btnYes, BUTTON_YES )
UI_SetEditEnterButton( edtName, btnYes )

btnNo = UI_CreateCompent( frmName, BUTTON_TYPE, "btnNo", 41, 19, 122, 112 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_NO )

UI_SetEditEnterButton( edtPCode2, btnYes )

-----------------------------------------------------------------------
-- NPC Guild War board
-----------------------------------------------------------------------	
--- Create NPC Guild War form
frmGuildPK = UI_CreateForm( "frmGuildPK", FALSE, 558, 387, 240, 70, TRUE, FALSE )
UI_ShowForm( frmGuildPK, TRUE )
UI_AddFormToTemplete( frmGuildPK, FORM_MAIN )
--UI_FormSetHotKey( frmGuildPK, ALT_KEY, HOTKEY_V )
UI_SetFormStyle( frmGuildPK, 2 )--- top-left
UI_SetIsDrag( frmGuildPK, TRUE )

-- Load background images
imgBakLeft = UI_CreateCompent( frmGuildPK, IMAGE_TYPE, "imgBakLeft", 256, 256, 0, 0 )
UI_LoadImage( imgBakLeft, "texture/ui/GuildPK1.tga", NORMAL, 256, 256, 0, 0 )

imgBakMidle = UI_CreateCompent( frmGuildPK, IMAGE_TYPE, "imgBakMidle", 256, 256, 256, 0 )
UI_LoadImage( imgBakMidle, "texture/ui/GuildPK2.tga", NORMAL, 256, 256, 0, 0 )

imgBakRight = UI_CreateCompent( frmGuildPK, IMAGE_TYPE, "imgBakRight", 46, 256, 512, 0 )
UI_LoadImage( imgBakRight, "texture/ui/GuildPK3.tga", NORMAL, 46, 256, 0, 0 )

imgBakLeftD = UI_CreateCompent( frmGuildPK, IMAGE_TYPE, "imgBakLeftD", 256, 131, 0, 256 )
UI_LoadImage( imgBakLeftD, "texture/ui/GuildPK4.tga", NORMAL, 256, 131, 0, 0 )

imgBakMidleD = UI_CreateCompent( frmGuildPK, IMAGE_TYPE, "imgBakMidleD", 256, 131, 256, 256 )
UI_LoadImage( imgBakMidleD, "texture/ui/GuildPK5.tga", NORMAL, 256, 131, 0, 0 )

imgBakRightD = UI_CreateCompent( frmGuildPK, IMAGE_TYPE, "imgBakRightD", 46, 131, 512, 256 )
UI_LoadImage( imgBakRightD, "texture/ui/GuildPK6.tga", NORMAL, 46, 131, 0, 0 )

--title
labTitle = UI_CreateCompent( frmGuildPK, LABELEX_TYPE, "labTitle", 60, 14, 208, 4 )
UI_SetCaption( labTitle, "Guild Ranking Board")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )
--title

-- Close button
btnClose = UI_CreateCompent( frmGuildPK, BUTTON_TYPE, "btnClose", 14, 14, 537, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-- Guild name labels
labGuildName0 = UI_CreateCompent( frmGuildPK, LABELEX_TYPE, "labGuildName0", 108, 11, 40, 124 )
UI_SetCaption( labGuildName0, "" )
UI_SetTextColor( labGuildName0, COLOR_BLACK )
UI_SetLabelExFont( labGuildName0, DEFAULT_FONT, TRUE, COLOR_WHITE )

labGuildName1 = UI_CreateCompent( frmGuildPK, LABELEX_TYPE, "labGuildName1", 108, 11, 40, 171 )
UI_SetCaption( labGuildName1, "" )
UI_SetTextColor( labGuildName1, COLOR_BLACK )
UI_SetLabelExFont( labGuildName1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labGuildName2 = UI_CreateCompent( frmGuildPK, LABELEX_TYPE, "labGuildName2", 108, 11, 40, 216 )
UI_SetCaption( labGuildName2, "" )
UI_SetTextColor( labGuildName2, COLOR_BLACK )
UI_SetLabelExFont( labGuildName2, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Challenger labels
labChallenger0 = UI_CreateCompent( frmGuildPK, LABELEX_TYPE, "labChallenger0", 108, 11, 194, 124 )
UI_SetCaption( labChallenger0, "" )
UI_SetTextColor( labChallenger0, COLOR_BLACK )
UI_SetLabelExFont( labChallenger0, DEFAULT_FONT, TRUE, COLOR_WHITE )

labChallenger1 = UI_CreateCompent( frmGuildPK, LABELEX_TYPE, "labChallenger1", 108, 11, 194, 171 )
UI_SetCaption( labChallenger1, "" )
UI_SetTextColor( labChallenger1, COLOR_BLACK )
UI_SetLabelExFont( labChallenger1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labChallenger2 = UI_CreateCompent( frmGuildPK, LABELEX_TYPE, "labChallenger2", 108, 11, 194, 216 )
UI_SetCaption( labChallenger2, "" )
UI_SetTextColor( labChallenger2, COLOR_BLACK )
UI_SetLabelExFont( labChallenger2, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Prize money labels
labMoney0 = UI_CreateCompent( frmGuildPK, LABELEX_TYPE, "labMoney0", 92, 11, 376, 124 )
UI_SetCaption( labMoney0, "" )
UI_SetTextColor( labMoney0, COLOR_BLACK )
UI_SetLabelExFont( labMoney0, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMoney1 = UI_CreateCompent( frmGuildPK, LABELEX_TYPE, "labMoney1", 92, 11, 376, 171 )
UI_SetCaption( labMoney1, "" )
UI_SetTextColor( labMoney1, COLOR_BLACK )
UI_SetLabelExFont( labMoney1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMoney2 = UI_CreateCompent( frmGuildPK, LABELEX_TYPE, "labMoney2", 92, 11, 376, 216 )
UI_SetCaption( labMoney2, "" )
UI_SetTextColor( labMoney2, COLOR_BLACK )
UI_SetLabelExFont( labMoney2, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Bid buttons
btnCharge0 = UI_CreateCompent( frmGuildPK, BUTTON_TYPE, "btnCharge0", 41, 19, 497, 120 )
UI_LoadButtonImage( btnCharge0, "texture/ui/forge3.tga", 41, 19, 0, 82, TRUE )
UI_SetButtonModalResult( btnCharge0, BUTTON_YES )

btnCharge1 = UI_CreateCompent( frmGuildPK, BUTTON_TYPE, "btnCharge1", 41, 19, 497, 167 )
UI_LoadButtonImage( btnCharge1, "texture/ui/forge3.tga", 41, 19, 0, 82, TRUE )
UI_SetButtonModalResult( btnCharge1, BUTTON_YES )

btnCharge2 = UI_CreateCompent( frmGuildPK, BUTTON_TYPE, "btnCharge2", 41, 19, 497, 212 )
UI_LoadButtonImage( btnCharge2, "texture/ui/forge3.tga", 41, 19, 0, 82, TRUE )
UI_SetButtonModalResult( btnCharge2, BUTTON_YES )



-- Info display list
lstGuildPK = UI_CreateCompent( frmGuildPK, LIST_TYPE, "lstGuildPK", 527, 76, 14, 289 )
UI_SetListRowHeight( lstGuildPK, 13 )
UI_SetListFontColor( lstGuildPK, COLOR_BLACK, COLOR_BLACK )
UI_SetMargin( lstGuildPK, 12,12,12,12 )
UI_SetIsShow( lstGuildPK, TRUE )
UI_AddListText( lstGuildPK, "1st place challenge: Every Sunday at 0001 GMT+8. Bids will end at Saturday 0001 GMT+8" )
UI_AddListText( lstGuildPK, "http://top.igg.com/guild/guild_wars.html" )
UI_AddListText( lstGuildPK, "3rd place challenge: Every Sunday 1201 GMT+8. Bids will end at Saturday 1201 GMT+8" )
UI_AddListText( lstGuildPK, "Note: After each match ended, a new round will begin. Please check out the official website for details." )



