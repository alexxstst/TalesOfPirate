--------------------
-- Build Ship NPC --
--------------------
frmShipBiuld = UI_CreateForm( "frmShipBiuld", FALSE, 382, 410, 294, 193, FALSE, FALSE )
UI_ShowForm( frmShipBiuld, FALSE )
UI_AddFormToTemplete( frmShipBiuld, FORM_MAIN )
UI_SetIsDrag( frmShipBiuld, TRUE )
--UI_FormSetHotKey( frmShipBiuld, ALT_KEY, HOTKEY_T ) 

imgBakLeftTop = UI_CreateCompent( frmShipBiuld, IMAGE_TYPE, "imgBakLeftTop", 256, 256, 0, 0 )
UI_LoadImage( imgBakLeftTop, "texture/ui/ShipBuild1.tga", NORMAL, 256, 256, 0, 0 )

imgBakRightTop = UI_CreateCompent( frmShipBiuld, IMAGE_TYPE, "imgBakRightTop", 126, 256, 256, 0 )
UI_LoadImage( imgBakRightTop, "texture/ui/ShipBuildT.tga", NORMAL, 126, 256, 0, 0 )

imgBakLeftButtom = UI_CreateCompent( frmShipBiuld, IMAGE_TYPE, "imgBakLeftButtom", 256, 118, 0, 256 )
UI_LoadImage( imgBakLeftButtom, "texture/ui/ShipBuildD.tga", NORMAL, 256, 118, 0, 0 )

imgBakRightButtom = UI_CreateCompent( frmShipBiuld, IMAGE_TYPE, "imgBakRightButtom", 126, 118, 256, 256 )
UI_LoadImage( imgBakRightButtom, "texture/ui/ShipBuildT.tga", NORMAL, 126, 118, 130, 0 )

btnClose = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnClose", 14, 14, 360, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

ui3dship = UI_CreateCompent( frmShipBiuld, UI3D_COMPENT, "ui3dship", 318, 147, 31, 49 )

labShipLvName = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labShipLvName", 40, 13, 31, 182 )
UI_SetCaption( labShipLvName, "Lv:" )
UI_SetTextColor( labShipLvName, COLOR_BLACK )
UI_SetLabelExFont( labShipLvName, DEFAULT_FONT, TRUE, COLOR_WHITE )

labShipLv = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labShipLv", 65, 8, 88, 182 )
UI_SetCaption( labShipLv, "1" )
UI_SetTextColor( labShipLv, COLOR_BLACK )
UI_SetLabelExFont( labShipLv, DEFAULT_FONT, TRUE, COLOR_WHITE )

labShipExpName = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labShipExpName", 40, 13, 31, 196 )
UI_SetCaption( labShipExpName, "EXP:" )
UI_SetTextColor( labShipExpName, COLOR_BLACK )
UI_SetLabelExFont( labShipExpName, DEFAULT_FONT, TRUE, COLOR_WHITE )

labShipExp = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labShipExp", 65, 8, 88, 196 )
UI_SetCaption( labShipExp, "0" )
UI_SetTextColor( labShipExp, COLOR_BLACK )
UI_SetLabelExFont( labShipExp, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnLeft = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnLeft", 15, 39, 20, 105 )
UI_LoadButtonImage( btnLeft, "texture/ui/SysBotton4.tga", 15, 39, 165, 39, TRUE )

btnRight = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnRight", 15, 39, 345, 105 )
UI_LoadButtonImage( btnRight, "texture/ui/SysBotton4.tga", 15, 39, 165, 0, TRUE )

btnTop = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnTop", 39, 15, 172, 38 )
UI_LoadButtonImage( btnTop, "texture/ui/ShipBuildT.tga", 39, 15, 132, 185, FALSE )

btnDown = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnDown", 39, 15, 172, 192 )
UI_LoadButtonImage( btnDown, "texture/ui/ShipBuildT.tga", 39, 15, 132, 123, FALSE )

edtShipName = UI_CreateCompent( frmShipBiuld, EDIT_TYPE, "edtShipName", 103, 13, 62, 225 )
UI_SetEditMaxNum( edtShipName, 16 )

labShipBody = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labShipBody", 103, 13, 62, 255 )
UI_SetCaption( labShipBody, "Fool" )
UI_SetTextColor( labShipBody, COLOR_BLACK )
UI_SetLabelExFont( labShipBody, DEFAULT_FONT, TRUE, COLOR_WHITE )

labShipPower = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labShipPower", 103, 13, 62, 275 )
UI_SetCaption( labShipPower, "Wheel of Fate" )
UI_SetTextColor( labShipPower, COLOR_BLACK )
UI_SetLabelExFont( labShipPower, DEFAULT_FONT, TRUE, COLOR_WHITE )
btnPowerLeft = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnPowerLeft", 13, 13, 45, 274 )
UI_LoadButtonImage( btnPowerLeft, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE )
btnPowerRight = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnPowerRight", 13, 13, 163, 274 )
UI_LoadButtonImage( btnPowerRight, "texture/ui/SystemBotton3.tga", 13, 13, 0, 211, TRUE )

labShipHead = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labShipHead", 103, 13, 62, 295 )
UI_SetCaption( labShipHead, "Queen" )
UI_SetTextColor( labShipHead, COLOR_BLACK )
UI_SetLabelExFont( labShipHead, DEFAULT_FONT, TRUE, COLOR_WHITE )
btnHeadLeft = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnHeadLeft", 13, 13, 45, 294 )
UI_LoadButtonImage( btnHeadLeft, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE )
btnHeadRight = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnHeadRight", 13, 13, 163, 294 )
UI_LoadButtonImage( btnHeadRight, "texture/ui/SystemBotton3.tga", 13, 13, 0, 211, TRUE )

labShipSign = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labShipSign", 103, 13, 62, 315 )
UI_SetCaption( labShipSign, "Skull" )
UI_SetTextColor( labShipSign, COLOR_BLACK )
UI_SetLabelExFont( labShipSign, DEFAULT_FONT, TRUE, COLOR_WHITE )
btnSignLeft = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnSignLeft", 13, 13, 45, 314 )
UI_LoadButtonImage( btnSignLeft, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE )
btnSignRight = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnSignRight", 13, 13, 163, 314 )
UI_LoadButtonImage( btnSignRight, "texture/ui/SystemBotton3.tga", 13, 13, 0, 211, TRUE )


--
labShipCanon = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labShipCanon", 103, 13, 62, 335 )
UI_SetCaption( labShipCanon, "Cannon" )
UI_SetTextColor( labShipCanon, COLOR_BLACK )
UI_SetLabelExFont( labShipCanon, DEFAULT_FONT, TRUE, COLOR_WHITE )
btnCanonLeft = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnCanonLeft", 13, 13, 45, 334 )
UI_LoadButtonImage( btnCanonLeft, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE )
btnCanonRight = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnCanonRight", 13, 13, 163, 334 )
UI_LoadButtonImage( btnCanonRight, "texture/ui/SystemBotton3.tga", 13, 13, 0, 211, TRUE )

labAtackShow = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labAtackShow", 62, 12, 216, 254 )
UI_SetCaption( labAtackShow, "11111/11111" )
UI_SetTextColor( labAtackShow, COLOR_BLACK )
UI_SetLabelExFont( labAtackShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTimeShow = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labTimeShow", 27, 12, 330, 254 )
UI_SetCaption( labTimeShow, "11111" )
UI_SetTextColor( labTimeShow, COLOR_BLACK )
UI_SetLabelExFont( labTimeShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labSailorShow = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labSailorShow", 62, 12, 216, 274 )
UI_SetCaption( labSailorShow, "22222/22222" )
UI_SetTextColor( labSailorShow, COLOR_BLACK )
UI_SetLabelExFont( labSailorShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labFlyspeedShow = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labFlyspeedShow", 27, 12, 330, 294)
UI_SetCaption( labFlyspeedShow, "22222" )
UI_SetTextColor( labFlyspeedShow, COLOR_BLACK )
UI_SetLabelExFont( labFlyspeedShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labDefenceShow = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labDefenceShow", 27, 12, 330, 314 )
UI_SetCaption( labDefenceShow, "33333" )
UI_SetTextColor( labDefenceShow, COLOR_BLACK )
UI_SetLabelExFont( labDefenceShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labCanonShow = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labCanonShow", 62, 12, 216, 294 )
UI_SetCaption( labCanonShow, "33333/33333" )
UI_SetTextColor( labCanonShow, COLOR_BLACK )
UI_SetLabelExFont( labCanonShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labContentShow = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labContentShow", 62, 12, 216, 314 )
UI_SetCaption( labContentShow, "44444/44444" )
UI_SetTextColor( labContentShow, COLOR_BLACK )
UI_SetLabelExFont( labContentShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labSpeedShow = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labSpeedShow", 27, 12, 330, 274 )
UI_SetCaption( labSpeedShow, "44444" )
UI_SetTextColor( labSpeedShow, COLOR_BLACK )
UI_SetLabelExFont( labSpeedShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgMomeyTitle = UI_CreateCompent( frmShipBiuld, IMAGE_TYPE, "imgMomeyTitle", 25, 13, 182, 333 )
UI_LoadImage( imgMomeyTitle, "texture/ui/ShipBuildD.tga", NORMAL, 25, 13, 4, 124 )

imgMoney = UI_CreateCompent( frmShipBiuld, IMAGE_TYPE, "imgMoney", 152, 18, 224, 331 )
UI_LoadImage( imgMoney, "texture/ui/word.dds", NORMAL, 152, 18, 2, 197 )

labMenShow = UI_CreateCompent( frmShipBiuld, LABELEX_TYPE, "labMenShow", 62, 12, 248, 336 )
UI_SetCaption( labMenShow, "55555555555" )
UI_SetTextColor( labMenShow, COLOR_BLACK )
UI_SetLabelExFont( labMenShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnYes", 41, 19, 106, 353 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnNo", 41, 19, 226, 353 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )

imgShipo = UI_CreateCompent( frmShipBiuld, IMAGE_TYPE, "imgShipo", 73, 13, 184, 225 )
UI_LoadImage( imgShipo, "texture/ui/ShipR2.tga", NORMAL, 73, 13, 62, 212 )

chkShip = UI_CreateCompent( frmShipBiuld, CHECK_TYPE, "chkShip", 10, 10, 260, 227 )
UI_LoadImage( chkShip, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkShip, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

imgSpace = UI_CreateCompent( frmShipBiuld, IMAGE_TYPE, "imgSpace", 42, 13, 182, 333 )
UI_LoadImage( imgSpace, "texture/ui/shipR2.tga", NORMAL, 42, 13, 63, 192 )

btnROpen = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnROpen", 92, 23, 270, 220 )
UI_LoadButtonImage( btnROpen, "texture/ui/botton2.tga", 92, 23, 62, 58, FALSE )

imgRoomLeft = UI_CreateCompent( frmShipBiuld, IMAGE_TYPE, "imgRoomLeft", 256, 168, 0, 0 )
--UI_LoadImage( imgRoomLeft, "texture/ui/shipR2.tga", NORMAL, 256, 168, 0, 0 )

imgRoomRight = UI_CreateCompent( frmShipBiuld, IMAGE_TYPE, "imgRoomRight", 126, 168, 256, 0 )
--UI_LoadImage( imgRoomRight, "texture/ui/shipR.tga", NORMAL, 126, 168, 0, 0 )

--grdRoom = UI_CreateCompent( frmShipBiuld, GOODS_GRID_TYPE, "grdRoom", 320, 110, 42, 270 ) 
--UI_SetGridSpace( grdRoom, 3, 3 )
--UI_SetGridContent( grdRoom, 4, 8 )
--UI_SetGridUnitSize( grdRoom, 32, 32 )
--UI_SetMargin( grdRoom, 3, 2, 0, 0 )

--UI_GridLoadSelectImage( grdRoom, "texture/ui/StartF.dds", 32, 32, 123, 99 )

--scrollid = UI_GetScroll( grdRoom )
--UI_SetSize( scrollid, 11, 1 )
--UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

--id = UI_GetScrollObj( scrollid, SCROLL_UP )
--UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
--UI_SetSize( id, 11, 9 )

--id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
--UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
--UI_SetSize( id, 11, 43 )

--id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
--UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
--UI_SetSize( id, 11, 9 )

btnRClose = UI_CreateCompent( frmShipBiuld, BUTTON_TYPE, "btnRClose", 92, 23, 270, 220 )
UI_LoadButtonImage( btnRClose, "texture/ui/Botton2.tga", 92, 23, 155, 58, FALSE )

-------------------
-- Cargo storage -- 
-------------------
frmShipRoom = UI_CreateForm( "frmShipRoom", FALSE, 382, 374, 294, 193, FALSE, FALSE )
UI_ShowForm( frmShipRoom, FALSE )
UI_AddFormToTemplete( frmShipRoom, FORM_MAIN )
UI_SetIsDrag( frmShipRoom, TRUE )
--UI_FormSetHotKey( frmShipRoom, ALT_KEY, HOTKEY_T ) 

imgRoomLeft = UI_CreateCompent( frmShipRoom, IMAGE_TYPE, "imgRoomLeft", 256, 168, 0, 0 )
UI_LoadImage( imgRoomLeft, "texture/ui/shipR2.tga", NORMAL, 256, 168, 0, 0 )

imgRoomRight = UI_CreateCompent( frmShipRoom, IMAGE_TYPE, "imgRoomRight", 126, 168, 256, 0 )
UI_LoadImage( imgRoomRight, "texture/ui/shipR.tga", NORMAL, 126, 168, 0, 0 )

grdRoom = UI_CreateCompent( frmShipRoom, GOODS_GRID_TYPE, "grdRoom", 320, 110, 46, 29 ) 
UI_SetGridSpace( grdRoom, 3, 3 )
UI_SetGridContent( grdRoom, 4, 8 )
UI_SetGridUnitSize( grdRoom, 32, 32 )
UI_SetMargin( grdRoom, 3, 2, 0, 0 )

UI_GridLoadSelectImage( grdRoom, "texture/ui/StartF.dds", 32, 32, 123, 99 )

scrollid = UI_GetScroll( grdRoom )
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



-------------
-- NPCchat --
-------------
frmNPCShip = UI_CreateForm( "frmNPCShip", FALSE, 320, 241, 240, 70, TRUE, FALSE )
UI_ShowForm( frmNPCShip, TRUE )
UI_AddFormToTemplete( frmNPCShip, FORM_MAIN )
--UI_FormSetHotKey( frmNPCShip, ALT_KEY, HOTKEY_V )
UI_SetFormStyle( frmNPCShip, 2 )---
UI_SetIsDrag( frmNPCShip, TRUE )

imgBakLeft = UI_CreateCompent( frmNPCShip, IMAGE_TYPE, "imgBakLeft", 320, 241, 0, 0 )
UI_LoadImage( imgBakLeft, "texture/ui/corsairs/NPCchat.png", NORMAL, 320, 241, 0, 0 )

--imgBakRight = UI_CreateCompent( frmNPCShip, IMAGE_TYPE, "imgBakRight", 64, 241, 256, 0 )
--UI_LoadImage( imgBakRight, "texture/ui/shipbuild3.dds", NORMAL, 64, 241, 189, 0 )

--imgchatTitle = UI_CreateCompent( frmNPCShip, IMAGE_TYPE, "imgchatTitle", 45, 15, 10, 4 )
--UI_LoadImage( imgchatTitle, "texture/ui/Word.dds", NORMAL, 45, 15, 0, 0 )

-- Dialog Label
labTitle = UI_CreateCompent( frmNPCShip, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Dialog")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Exit Button
btnClose = UI_CreateCompent( frmNPCShip, BUTTON_TYPE, "btnClose", 14, 14, 299, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

labNPCCity = UI_CreateCompent( frmNPCShip, LABELEX_TYPE, "labNPCCity", 34, 11, 53, 29 )
UI_SetCaption( labNPCCity, "" )
UI_SetTextColor( labNPCCity, COLOR_WHITE )
UI_SetLabelExFont( labNPCCity, DEFAULT_FONT, TRUE, COLOR_BLACK )

labNPCName = UI_CreateCompent( frmNPCShip, LABELEX_TYPE, "labNPCName", 155, 11, 53, 44 )
UI_SetCaption( labNPCName, "" )
UI_SetTextColor( labNPCName, COLOR_WHITE )
UI_SetLabelExFont( labNPCName, DEFAULT_FONT, TRUE, COLOR_BLACK )

memCtrl = UI_CreateCompent( frmNPCShip, MEMO_TYPE, "memCtrl", 280, 134, 33, 68 )
UI_SetTextColor( memCtrl, COLOR_BLACK )
UI_SetCaption( memCtrl, "" ) 
UI_SetMemoMaxNumPerRow(memCtrl, 42 )
UI_SetMemoPageShowNum(memCtrl, 6 )
UI_SetMemoRowHeight( memCtrl , 21 )

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

btnNo = UI_CreateCompent( frmNPCShip, BUTTON_TYPE, "btnNo", 41, 19, 257, 208 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 214, TRUE )

-----------------------------------------------------------------------
-- 
-----------------------------------------------------------------------
--
frmShipsail = UI_CreateForm( "frmShipsail", FALSE, 305, 46, 294, 0, FALSE, FALSE )
UI_ShowForm( frmShipsail, FALSE )
UI_AddFormToTemplete( frmShipsail, FORM_MAIN )
UI_SetIsDrag( frmShipsail, TRUE )
UI_SetFormStyleEx( frmShipsail, 2, 0, 0 )
--UI_FormSetHotKey( frmShipsail, ALT_KEY, HOTKEY_T ) 
UI_FormSetIsEscClose( frmShipsail, FALSE )

-- 
imgBakLeft = UI_CreateCompent( frmShipsail, IMAGE_TYPE, "imgBakLeft", 143, 41, -32, 14 )
UI_LoadImage( imgBakLeft, "texture/ui/Shipdetail.tga", NORMAL, 143, 41, 0, 0 )

imgBakRight = UI_CreateCompent( frmShipsail, IMAGE_TYPE, "imgBakRight", 143, 41, 194, 14 )
UI_LoadImage( imgBakRight, "texture/ui/Shipdetail.tga", NORMAL, 143, 41, 0, 46 )

imgBakLeftTop = UI_CreateCompent( frmShipsail, IMAGE_TYPE, "imgBakLeftTop", 256, 46, 0, 0 )
UI_LoadImage( imgBakLeftTop, "texture/ui/shipinfo.tga", NORMAL, 256, 46, 0, 0 )

imgBakRightTop = UI_CreateCompent( frmShipsail, IMAGE_TYPE, "imgBakRightTop", 49, 28, 256, 7 )
UI_LoadImage( imgBakRightTop, "texture/ui/shipinfo.tga", NORMAL, 49, 28, 0, 51 )

--
labSailorShow1 = UI_CreateCompent( frmShipsail, LABELEX_TYPE, "labSailorShow1", 62, 12, 25, 9 )
UI_SetCaption( labSailorShow1, "22222/22222" )
UI_SetTextColor( labSailorShow1, COLOR_WHITE )
UI_SetLabelExFont( labSailorShow1, DEFAULT_FONT, TRUE, COLOR_BLACK )

--
labCanonShow1 = UI_CreateCompent( frmShipsail, LABELEX_TYPE, "labCanonShow1", 62, 12, 217, 9 )
UI_SetCaption( labCanonShow1, "33333/33333" )
UI_SetTextColor( labCanonShow1, COLOR_WHITE )
UI_SetLabelExFont( labCanonShow1, DEFAULT_FONT, TRUE, COLOR_BLACK )

--
labExpship = UI_CreateCompent( frmShipsail, LABELEX_TYPE, "labExpship", 62, 12, 298, 37 )
UI_SetCaption( labExpship, "10000" )
UI_SetTextColor( labExpship, COLOR_WHITE )
UI_SetLabelExFont( labExpship, DEFAULT_FONT, TRUE, COLOR_BLACK )

--
labLvship = UI_CreateCompent( frmShipsail, LABELEX_TYPE, "labLvship", 62, 12, 40, 37 )
UI_SetCaption( labLvship, "100" )
UI_SetTextColor( labLvship, COLOR_WHITE )
UI_SetLabelExFont( labLvship, DEFAULT_FONT, TRUE, COLOR_BLACK )

--
proSailor = UI_CreateCompent( frmShipsail, PROGRESS_TYPE, "proSailor", 98, 4, 8, 22 )
UI_LoadScaleImage( proSailor, "texture/ui/shipinfo.tga", PROGRESS_PROGRESS, 98, 4, 8, 86, 1.0, 1.0 )

--
proCanon = UI_CreateCompent( frmShipsail, PROGRESS_TYPE, "proCanon", 98, 4, 199, 22 )
UI_LoadScaleImage( proCanon, "texture/ui/shipinfo.tga", PROGRESS_PROGRESS, 98, 4, 8, 93, 1.0, 1.0 )

--
btnShip = UI_CreateCompent( frmShipsail, BUTTON_TYPE, "btnShip", 28, 25, 139, 9 )
UI_LoadButtonImage( btnShip, "texture/ui/shipinfo.tga", 28, 25, 4, 102, TRUE )
UI_SetHint( btnShip, "Check ship attribute" )
