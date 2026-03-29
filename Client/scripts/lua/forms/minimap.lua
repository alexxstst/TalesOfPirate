----------------
-- Minimap UI --
----------------
frmMinimap = UI_CreateForm( "frmMinimap", FALSE, 140, 155, 656, 15, TRUE, FALSE )
UI_ShowForm( frmMinimap, TRUE )
UI_AddFormToTemplete( frmMinimap, FORM_MAIN )
UI_SetFormStyle( frmMinimap, 9 )--- top-right
UI_FormSetIsEscClose( frmMinimap, FALSE )

imgMinimapT = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapT", 140, 9, 0, 0 )
UI_LoadImage( imgMinimapT, "texture/ui/ShipBuild4.dds", NORMAL, 140, 9, 0, 191 )

imgMinimapN = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapN", 13, 15, 65, 2 )
UI_LoadImage( imgMinimapN, "texture/ui/main800.tga", NORMAL, 13, 15, 6, 184 )

imgMinimapD = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapD", 140, 22, 0, 133 )
UI_LoadImage( imgMinimapD, "texture/ui/ShipBuild4.dds", NORMAL, 140, 22, 0, 223 )

imgMinimapL = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapL", 8, 156, 0, 0 )
UI_LoadImage( imgMinimapL, "texture/ui/ShipBuild4.dds", NORMAL, 8, 156, 220, 88 )

imgMinimapR = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapR", 8, 156, 132, 0 )
UI_LoadImage( imgMinimapR, "texture/ui/ShipBuild4.dds", NORMAL, 8, 156, 245, 88 )

imgMinimapRect = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapRect", 128, 128, 6, 8 )

labMapName = UI_CreateCompent( frmMinimap, LABELEX_TYPE, "labMapName", 51, 12, 2, 137 )
UI_SetCaption( labMapName, "��������������" )
UI_SetTextColor( labMapName, COLOR_BLACK )
UI_SetLabelExFont( labMapName, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Map coordinates (multiple labels for anti-cheat, only one shown)
n  = 0
while 1 do
	local name = "labMapPos"..n
	labMapPos = UI_CreateCompent( frmMinimap, LABELEX_TYPE, name, 79, 12, 2, 157 )
	UI_SetCaption( labMapPos, "2200, 2200" )
	UI_SetTextColor( labMapPos, COLOR_WHITE )
	UI_SetLabelExFont( labMapPos, FONT14, TRUE, COLOR_BLACK )
	
	if n~=0  then UI_SetIsShow( labMapPos, FALSE ) end
	
	n = n + 1
	if n==10 then break end	
end

-- Server Time
labClock = UI_CreateCompent( frmMinimap, LABELEX_TYPE, "labClock", 79, 12, 2, 155+15 )
UI_SetCaption( labClock, "00:00:00" )
UI_SetTextColor( labClock, COLOR_WHITE)
UI_SetLabelExFont( labClock, FONT20, TRUE, COLOR_BLACK )

-- Minimap Buttons
btnOpen = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnOpen", 20, 20, 115, 112 )
UI_LoadButtonImage( btnOpen, "texture/ui/corsairs/botton3.png", 20, 20, 0, 85, FALSE )
UI_SetHint( btnOpen, "World Map" )

btnSearch = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnSearch", 20, 20, 95, 112 )
UI_LoadButtonImage( btnSearch, "texture/ui/corsairs/botton3.png", 20, 20, 26, 85, FALSE )
UI_SetHint( btnSearch, "Radar" )

btnPosSearch = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnPosSearch", 20, 20, 75, 112 )
UI_LoadButtonImage( btnPosSearch, "texture/ui/corsairs/botton3.png", 20, 20, 74, 85, FALSE )
UI_SetHint( btnPosSearch, "Search NPC & Monsters" )

btnteam = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnteam", 20, 20, 55, 112)
UI_LoadButtonImage( btnteam, "texture/ui/corsairs/botton3.png", 20, 20, 52, 85, FALSE)
UI_SetHint( btnteam, "Search Party" )

-- Item Mall button
btnOpenStore = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnOpenStore", 41, 19, 87, 158 )
UI_LoadButtonImage( btnOpenStore, "texture/ui/storenewbutton.tga", 41, 19, 0, 0, TRUE )
UI_SetHint( btnOpenStore, "Accessing Item Mall" )
------------------------------Daily Buff -----------------------------------------
btnDailyBuff = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnDailyBuff", 41, 19, 87, 158+40 )
UI_LoadButtonImage( btnDailyBuff, "texture/ui/storenewbutton.tga", 41, 19, 0, 0, TRUE )
UI_SetHint( btnOpenStore, "Open Daily Buff" )

DailyBuffFrm = UI_CreateForm( "DailyBuffFrm", FALSE, 591, 467, 0, 0, TRUE, FALSE )
UI_ShowForm( DailyBuffFrm, FALSE )
UI_AddFormToTemplete( DailyBuffFrm, FORM_MAIN )
UI_FormSetIsEscClose( DailyBuffFrm, FALSE )
--UI_FormSetHotKey( DailyBuffFrm, ALT_KEY, HOTKEY_V )
UI_SetIsDrag( DailyBuffFrm, TRUE )

DailyBuffBK = UI_CreateCompent( DailyBuffFrm, IMAGE_TYPE, "DailyBuffBK", 591, 467, 0, 0 )
UI_LoadImage( DailyBuffBK, "texture/ui/new/DailyBuff/dailybuff.png", NORMAL, 591, 467, 0, 0 )

btnClose = UI_CreateCompent( DailyBuffFrm, BUTTON_TYPE, "btnClose", 14, 14, 582, 4 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )
--load image of curren buff placeholder
DBufImageType = UI_CreateCompent( DailyBuffFrm, IMAGE_TYPE, "DBufImageType", 256, 256, 180, 281 )
UI_LoadImage( DBufImageType, "texture/ui/new/DailyBuff/Combining_gems.png", NORMAL, 736, 626, 0, 0 )
--load first label 
DailyBufInfo = UI_CreateCompent( DailyBuffFrm, LABELEX_TYPE, "DailyBufInfo", 51, 12, 81, 88 )
UI_SetCaption( DailyBufInfo, "Info About current daily buff " )
UI_SetTextColor( DailyBufInfo, COLOR_BLACK )
UI_SetLabelExFont( DailyBufInfo, DEFAULT_FONT, TRUE, COLOR_WHITE )

-----------------------------------------------------------------------
-- Temporary bag button
--btnOpenTempBag = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnOpenTempBag", 76, 19, 3, 158 )
--UI_LoadButtonImage( btnOpenTempBag, "texture/ui/storebutton.tga", 76, 19, 0, 0, FALSE )
--UI_SetHint( btnSearch, "Open temporary inventory" )

-- Bulletin board, temp
--	btnteam = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnteam", 48, 19, 87, 178 )
--	UI_LoadButtonImage( btnteam, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

-----------------------------------------------------------------------
-- Radar form
-----------------------------------------------------------------------	
-- Create radar form
frmSearch = UI_CreateForm( "frmSearch", FALSE, 212, 129, 300, 200, TRUE, FALSE )
UI_ShowForm( frmSearch, FALSE )
UI_AddFormToTemplete( frmSearch, FORM_MAIN )
UI_FormSetIsEscClose( frmSearch, FALSE )
UI_FormSetHotKey( frmSearch, ALT_KEY, HOTKEY_R )
UI_SetIsDrag( frmSearch, TRUE )

-- Background image
imgSearch = UI_CreateCompent( frmSearch, IMAGE_TYPE, "imgSearch", 212, 129, 0, 0 )
UI_LoadImage( imgSearch, "texture/ui/search.tga", NORMAL, 212, 129, 0, 0 )

-- Close button
btnClose = UI_CreateCompent( frmSearch, BUTTON_TYPE, "btnClose", 14, 14, 194, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-- Coordinate input fields
edtLeft = UI_CreateCompent( frmSearch, EDIT_TYPE, "edtLeft", 45, 11, 94, 70 )
UI_SetTextColor( edtLeft, COLOR_BLACK )
UI_SetEditMaxNum( edtLeft, 4 )
UI_SetEditMaxNumVisible( edtLeft, 4 )

edtRight = UI_CreateCompent( frmSearch, EDIT_TYPE, "edtRight", 25, 11, 145, 70 )
UI_SetTextColor( edtRight, COLOR_BLACK )
UI_SetEditMaxNum( edtRight, 4 )
UI_SetEditMaxNumVisible( edtRight, 4 )


-- Confirm button
btnYes = UI_CreateCompent( frmSearch, BUTTON_TYPE, "btnYes", 41, 19, 102, 102 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetEditEnterButton( edtRight, btnYes )

-- Cancel button
btnNo = UI_CreateCompent( frmSearch, BUTTON_TYPE, "btnNo", 41, 19, 150, 102 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )


-- Map selection dropdown
cboMap  = UI_CreateCompent( frmSearch, COMBO_TYPE, "cboMap", 98, 20, 88, 38 )
UI_ComboSetStyle( cboMap, TRUE )
UI_LoadComboImage( cboMap, "", 81, 20, 106, 39, "texture/ui/search.tga", 1, 1, 130, 140, FALSE )
UI_ComboSetTextColor( cboMap, COLOR_BLACK )
lstMap = UI_GetList( cboMap )
UI_SetListFontColor( lstMap, COLOR_BLACK, COLOR_BLACK )
UI_SetSize( lstMap, 51, 60 )
UI_SetMargin( lstMap, 16,6,16,0 )
UI_SetListRowHeight( lstMap, 18 )
UI_LoadImage( lstMap, "texture/ui/search.tga", COMPENT_BACK, 51, 60, 0, 133 )
UI_AddListText( lstMap, "Ascaron" )
UI_AddListText( lstMap, "Magical Ocean" )
UI_AddListText( lstMap, "Deep Blue" )
UI_SetListIsMouseFollow( lstMap, TRUE)
UI_ListLoadSelectImage( lstMap, "texture/ui/SystemBotton3.tga", 79, 18, 0, 227  )
UI_ComboSetTextColor( lstMap, COLOR_WHITE )

-- Add by Mdr May 2020 - FPO beta
chkID = UI_CreateCompent( frmSearch, CHECK_TYPE, "chkID", 10, 10, 25, 106 )
UI_LoadImage( chkID, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkID, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )
UI_SetHint( chkID, "Auto-trace or not?" )----------------
-- Minimap UI --
----------------
-- Create minimap form
frmMinimap = UI_CreateForm( "frmMinimap", FALSE, 140, 155, 656, 15, TRUE, FALSE )
UI_ShowForm( frmMinimap, TRUE )
UI_AddFormToTemplete( frmMinimap, FORM_MAIN )
UI_SetFormStyle( frmMinimap, 9 )--- top-right
UI_FormSetIsEscClose( frmMinimap, FALSE )

-- Background images
imgMinimapT = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapT", 140, 9, 0, 0 )
UI_LoadImage( imgMinimapT, "texture/ui/ShipBuild4.dds", NORMAL, 140, 9, 0, 191 )

imgMinimapN = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapN", 13, 15, 65, 2 )
UI_LoadImage( imgMinimapN, "texture/ui/main800.tga", NORMAL, 13, 15, 6, 184 )

imgMinimapD = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapD", 140, 22, 0, 133 )
UI_LoadImage( imgMinimapD, "texture/ui/ShipBuild4.dds", NORMAL, 140, 22, 0, 223 )

imgMinimapL = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapL", 8, 156, 0, 0 )
UI_LoadImage( imgMinimapL, "texture/ui/ShipBuild4.dds", NORMAL, 8, 156, 220, 88 )

imgMinimapR = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapR", 8, 156, 132, 0 )
UI_LoadImage( imgMinimapR, "texture/ui/ShipBuild4.dds", NORMAL, 8, 156, 245, 88 )

-- Minimap display area
imgMinimapRect = UI_CreateCompent( frmMinimap, IMAGE_TYPE, "imgMinimapRect", 128, 128, 6, 8 )


-- Map name display
labMapName = UI_CreateCompent( frmMinimap, LABELEX_TYPE, "labMapName", 51, 12, 2, 137 )
UI_SetCaption( labMapName, "��������������" )
UI_SetTextColor( labMapName, COLOR_BLACK )
UI_SetLabelExFont( labMapName, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Map coordinates (multiple labels for anti-cheat, only one shown)
n  = 0
while 1 do
	local name = "labMapPos"..n
	labMapPos = UI_CreateCompent( frmMinimap, LABELEX_TYPE, name, 79, 12, 2, 157 )
	UI_SetCaption( labMapPos, "2200, 2200" )
	UI_SetTextColor( labMapPos, COLOR_BLACK )
	UI_SetLabelExFont( labMapPos, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	if n~=0  then UI_SetIsShow( labMapPos, FALSE ) end
	
	n = n + 1
	if n==10 then break end	
end

-- Minimap Buttons
btnOpen = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnOpen", 20, 20, 115, 112 )
UI_LoadButtonImage( btnOpen, "texture/ui/corsairs/botton3.png", 20, 20, 0, 85, FALSE )
UI_SetHint( btnOpen, "World Map" )

btnSearch = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnSearch", 20, 20, 95, 112 )
UI_LoadButtonImage( btnSearch, "texture/ui/corsairs/botton3.png", 20, 20, 26, 85, FALSE )
UI_SetHint( btnSearch, "Radar" )

btnPosSearch = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnPosSearch", 20, 20, 75, 112 )
UI_LoadButtonImage( btnPosSearch, "texture/ui/corsairs/botton3.png", 20, 20, 74, 85, FALSE )
UI_SetHint( btnPosSearch, "Search NPC & Monsters" )

btnteam = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnteam", 20, 20, 35, 112 )
UI_LoadButtonImage( btnteam, "texture/ui/corsairs/botton3.png", 20, 20, 119, 85, FALSE )
UI_SetHint( btnteam, "Search Party" )

------------------------------------------------------------------------------------------
-- Discord button
------------------------------------------------------------------------------------------
btnDiscord = UI_CreateCompent(frmMinimap, BUTTON_TYPE, "btnDiscord", 96, 24, 35, 210 )
UI_LoadButtonImage(btnDiscord, "texture/ui/discordbtn.png", 110, 26, 0, 0, TRUE )
------------------------------------------------------------------------------------------

-- Item Mall button
btnOpenStore = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnOpenStore", 98, 30, 35, 175 ) -- 41, 19, 87, 158
UI_LoadButtonImage( btnOpenStore, "texture/ui/storenewbutton.png", 98, 35, 0, 0, TRUE ) -- 41, 19
UI_SetHint( btnOpenStore, "Accessing Item Mall" )


-- Temporary bag button
--btnOpenTempBag = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnOpenTempBag", 76, 19, 3, 158 )
--UI_LoadButtonImage( btnOpenTempBag, "texture/ui/storebutton.tga", 76, 19, 0, 0, FALSE )
--UI_SetHint( btnSearch, "Open temporary inventory" )

-- Bulletin board, temp
--	btnteam = UI_CreateCompent( frmMinimap, BUTTON_TYPE, "btnteam", 48, 19, 87, 178 )
--	UI_LoadButtonImage( btnteam, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

-----------------------------------------------------------------------
-- Radar form
-----------------------------------------------------------------------	
-- Create radar form
frmSearch = UI_CreateForm( "frmSearch", FALSE, 212, 129, 300, 200, TRUE, FALSE )
UI_ShowForm( frmSearch, FALSE )
UI_AddFormToTemplete( frmSearch, FORM_MAIN )
UI_FormSetIsEscClose( frmSearch, FALSE )
UI_FormSetHotKey( frmSearch, ALT_KEY, HOTKEY_R )
UI_SetIsDrag( frmSearch, TRUE )

-- Background image
imgSearch = UI_CreateCompent( frmSearch, IMAGE_TYPE, "imgSearch", 212, 129, 0, 0 )
UI_LoadImage( imgSearch, "texture/ui/search.tga", NORMAL, 212, 129, 0, 0 )

-- Close button
btnClose = UI_CreateCompent( frmSearch, BUTTON_TYPE, "btnClose", 14, 14, 194, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-- Coordinate input fields
edtLeft = UI_CreateCompent( frmSearch, EDIT_TYPE, "edtLeft", 45, 11, 94, 70 )
UI_SetTextColor( edtLeft, COLOR_BLACK )
UI_SetEditMaxNum( edtLeft, 4 )
UI_SetEditMaxNumVisible( edtLeft, 4 )

edtRight = UI_CreateCompent( frmSearch, EDIT_TYPE, "edtRight", 25, 11, 145, 70 )
UI_SetTextColor( edtRight, COLOR_BLACK )
UI_SetEditMaxNum( edtRight, 4 )
UI_SetEditMaxNumVisible( edtRight, 4 )


-- Confirm button
btnYes = UI_CreateCompent( frmSearch, BUTTON_TYPE, "btnYes", 41, 19, 102, 102 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetEditEnterButton( edtRight, btnYes )

-- Cancel button
btnNo = UI_CreateCompent( frmSearch, BUTTON_TYPE, "btnNo", 41, 19, 150, 102 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )


-- Map selection dropdown
cboMap  = UI_CreateCompent( frmSearch, COMBO_TYPE, "cboMap", 98, 20, 88, 38 )
UI_ComboSetStyle( cboMap, TRUE )
UI_LoadComboImage( cboMap, "", 81, 20, 106, 39, "texture/ui/search.tga", 1, 1, 130, 140, FALSE )
UI_ComboSetTextColor( cboMap, COLOR_BLACK )
lstMap = UI_GetList( cboMap )
UI_SetListFontColor( lstMap, COLOR_BLACK, COLOR_BLACK )
UI_SetSize( lstMap, 51, 60 )
UI_SetMargin( lstMap, 16,6,16,0 )
UI_SetListRowHeight( lstMap, 18 )
UI_LoadImage( lstMap, "texture/ui/search.tga", COMPENT_BACK, 51, 60, 0, 133 )
UI_AddListText( lstMap, "Ascaron" )
UI_AddListText( lstMap, "Magical Ocean" )
UI_AddListText( lstMap, "Deep Blue" )
UI_SetListIsMouseFollow( lstMap, TRUE)
UI_ListLoadSelectImage( lstMap, "texture/ui/SystemBotton3.tga", 79, 18, 0, 227  )
UI_ComboSetTextColor( lstMap, COLOR_WHITE )

-- Add by Mdr May 2020 - FPO beta
chkID = UI_CreateCompent( frmSearch, CHECK_TYPE, "chkID", 10, 10, 25, 106 )
UI_LoadImage( chkID, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkID, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )
UI_SetHint( chkID, "Auto-trace or not?" )
