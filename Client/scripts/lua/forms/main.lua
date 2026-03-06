UI_LoadSkillActiveImage( "texture/ui/StartF.dds", 2,  32, 32, 124, 155 )

 UI_LoadChargeImage( enumEQUIP_HEAD, "texture/ui/charge.tga", 6,  32, 32, 0, 0 )
 UI_LoadChargeImage( enumEQUIP_FACE, "texture/ui/charge.tga", 6,  32, 32, 0, 0 ) 
 UI_LoadChargeImage( enumEQUIP_BODY, "texture/ui/charge.tga", 6,  32, 32, 0, 0 ) 
 UI_LoadChargeImage( enumEQUIP_GLOVE, "texture/ui/charge.tga", 6,  32, 32, 0, 0 ) 
 UI_LoadChargeImage( enumEQUIP_SHOES, "texture/ui/charge.tga", 6,  32, 32, 0, 0 ) 
 UI_LoadChargeImage( enumEQUIP_LHAND, "texture/ui/charge.tga", 6,  32, 32, 0, 0 ) 
 UI_LoadChargeImage( enumEQUIP_RHAND, "texture/ui/charge.tga", 6,  32, 32, 0, 0 ) 
 UI_LoadChargeImage( enumEQUIP_NECK, "texture/ui/charge.tga", 6,  32, 32, 0, 0 ) 
 UI_LoadChargeImage( enumEQUIP_HAND1, "texture/ui/charge.tga", 6,  32, 32, 0, 0 ) 
 UI_LoadChargeImage( enumEQUIP_HAND2, "texture/ui/charge.tga", 6,  32, 32, 0, 0 )


 -----------------------------------------------------------------------
-- 
-- ,:,,,,GM,,,
UI_SetChatColor( 4294967295, 4290505961, 4281597696, 4278255615,  4294967040, 4294967040 ,4287943679, 4294939894 )
UI_SetHeadSayBkgColor( 2568384662 )


----------------------
-- Charater profile --
----------------------
frmDetail = UI_CreateForm( "frmDetail", FALSE, 189, 51, 4, 3, TRUE, FALSE )
--UI_FormSetHotKey( frmDetail, ALT_KEY, HOTKEY_X ) 
UI_ShowForm( frmDetail, TRUE )
UI_AddFormToTemplete( frmDetail, FORM_MAIN )
UI_FormSetIsEscClose( frmDetail, FALSE )


imgBackDropPlayer = UI_CreateCompent( frmDetail, IMAGE_TYPE, "imgBackDropPlayer", 55, 44, 5, 5 )
UI_LoadImage( imgBackDropPlayer, "texture/ui/Corsairs/npcBackDrop/0.tga", NORMAL, 55, 44, 0, 0 )

imgMain5 = UI_CreateCompent( frmDetail, IMAGE_TYPE, "imgMain5", 189, 51, 0, 0 )
UI_LoadImage( imgMain5, "texture/ui/corsairs/new3.tga", NORMAL, 189, 51, 0, 0 )

proMainHP1 = UI_CreateCompent( frmDetail, PROGRESS_TYPE, "proMainHP1", 111, 8, 70, 22 )
UI_LoadScaleImage( proMainHP1, "texture/ui/corsairs/new3.tga", PROGRESS_PROGRESS, 111, 8, 0, 129, 1.0, 1.0 )
UI_SetHint( proMainHP1, "hp" )
UI_SetProgressHintStyle( proMainHP1, PROGRESS_HINT_NUM )

proMainSP = UI_CreateCompent( frmDetail, PROGRESS_TYPE, "proMainSP", 111, 8, 70, 31 )
UI_LoadScaleImage( proMainSP, "texture/ui/corsairs/new3.tga", PROGRESS_PROGRESS, 111, 8, 0, 138, 1.0, 1.0 )
UI_SetHint( proMainSP, "sp" )
UI_SetProgressHintStyle( proMainSP, PROGRESS_HINT_NUM )

btnHPSP = UI_CreateCompent( frmDetail, BUTTON_TYPE, "btnHPSP", 111, 8, 102, 22 )

proMainEXP = UI_CreateCompent( frmDetail, PROGRESS_TYPE, "proMainEXP", 111, 8, 70, 40 )
UI_LoadScaleImage( proMainEXP, "texture/ui/corsairs/new3.tga", PROGRESS_PROGRESS, 111, 8, 0, 147, 1.0, 1.0 )
UI_SetHint( proMainEXP, "EXP" )

labMainID = UI_CreateCompent( frmDetail, LABELEX_TYPE, "labMainID", 95, 11, 77, 7 )
UI_SetTextColor( labMainID, COLOR_WHITE )
UI_SetLabelExFont( labMainID, DEFAULT_FONT, TRUE, COLOR_BLACK )
--UI_SetCaption( labMainID, "" )

labMainLv = UI_CreateCompent( frmDetail, LABELEX_TYPE, "labMainLv", 14, 9, 51, 7 )
UI_SetTextColor( labMainLv, COLOR_WHITE )
UI_SetLabelExFont( labMainLv, DEFAULT_FONT, TRUE, COLOR_BLACK )
UI_SetCaption( labMainLv, "99" )


d3dSelf = UI_CreateCompent( frmDetail, UI3D_COMPENT_TYPE, "d3dSelf", 36, 36, 6, 10 )

d3dSelfDown = UI_CreateCompent( frmDetail, UI3D_COMPENT_TYPE, "d3dSelfDown", 189, 51, 0, 0 )

imgLeader = UI_CreateCompent( frmDetail, IMAGE_TYPE, "imgLeader", 9, 9, 8, 9 )
UI_LoadImage( imgLeader, "texture/ui/corsairs/new3.tga", NORMAL, 9, 9, 0, 161 )
UI_SetIsShow( imgLeader, FALSE )

-----------------------------------------------------------------------
-- Define 800*600 main interface
-----------------------------------------------------------------------	
frmMain800 = UI_CreateForm( "frmMain800", FALSE, 288, 30, 0, 570, TRUE, FALSE )
--UI_FormSetHotKey( frmMain800, ALT_KEY, HOTKEY_X ) 
UI_ShowForm( frmMain800, TRUE )
UI_AddFormToTemplete( frmMain800, FORM_MAIN )
UI_SetFormStyle( frmMain800, 7 )---
UI_FormSetIsEscClose( frmMain800, FALSE )
--UI_SetAlpha( frmMinimap, 200 )

tlCity = UI_CreateCompent( frmMain800, TITLE_TYPE, "tlCity", 256, 45, 0, 0 )
UI_LoadImage( tlCity, "texture/ui/City.tga", 0, 166, 105, 0, 0 )
UI_LoadImage( tlCity, "texture/ui/City.tga", 1, 1, 69, 86, 105 )
UI_LoadImage( tlCity, "texture/ui/City.tga", 2, 166, 105, 90, 105 )
UI_SetTitleFont( tlCity, BIGFONT, COLOR_BLACK, 10 )

tlField = UI_CreateCompent( frmMain800, TITLE_TYPE, "tlField", 256, 45, 0, 0 )
UI_LoadImage( tlField, "texture/ui/Field.tga", 0, 124, 115, 0, 0 )
UI_LoadImage( tlField, "texture/ui/Field.tga", 1, 1, 72, 127, 0 )
UI_LoadImage( tlField, "texture/ui/Field.tga", 2, 124, 115, 132, 0 )
UI_SetTitleFont( tlField, BIGFONT, COLOR_BLACK, 15 )


--------------------
-- Chat interface --
--------------------

imgMain1 = UI_CreateCompent( frmMain800, IMAGE_TYPE, "imgMain1", 256, 30, 0, 0 )
UI_LoadImage( imgMain1, "texture/ui/new.tga", NORMAL, 256, 30, 0, 0 )
imgMain2 = UI_CreateCompent( frmMain800, IMAGE_TYPE, "imgMain2", 32, 30, 256, 0 )
UI_LoadImage( imgMain2, "texture/ui/new4.tga", NORMAL, 32, 30, 0, 0 )


-- 
edtSay = UI_CreateCompent( frmMain800, EDIT_TYPE, "edtSay", 150, 20, 64, 9 )
UI_SetTextColor( edtSay, COLOR_BLACK )
UI_SetEditMaxNum( edtSay, 64)
UI_SetEditMaxNumVisible( edtSay, 24)
UI_SetEditCursorColor( edtSay, COLOR_BLACK )

-- 
lstOnSay = UI_CreateCompent( frmMain800, LIST_TYPE, "lstOnSay", 288, 100, 0, -98 )
UI_LoadImage( lstOnSay, "texture/ui/Nbotton.tga", COMPENT_BACK, 1, 1, 100, 171 )
UI_SetImageAlpha( lstOnSay, 153 )
UI_SetMargin( lstOnSay, 5, 3, 0, 0 )


--  
drpTitle = UI_CreateCompent( frmMain800, DRAG_TITLE_TYPE, "drpTitle", 43, 7, 0, -100 )
UI_LoadImage( drpTitle, "texture/ui/SysBotton4.tga", NORMAL, 43, 7, 158, 100 )

--
scrollid = UI_GetScroll( lstOnSay)
UI_SetSize( scrollid, 18, 18 )
--UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 18, 18, 194, 13 )

--
id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/Nbotton.tga", 18, 18, 138, 4, TRUE )
UI_SetSize( id, 18, 18 )

--
id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/Nbotton.tga", COMPENT_BACK, 18, 27, 195, 10 )
UI_SetSize( id, 18, 27 )

--
id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/Nbotton.tga", 18, 18, 138, 25, TRUE )
UI_SetSize( id, 18, 18 )

-- 
lstOnSaySystem = UI_CreateCompent( frmMain800, LIST_TYPE, "lstOnSaySystem", 288, 60, 0, -158 )
UI_LoadImage( lstOnSaySystem, "texture/ui/Nbotton.tga", COMPENT_BACK, 1, 1, 100, 171 )
UI_SetImageAlpha( lstOnSaySystem, 153 )
UI_SetMargin( lstOnSaySystem, 5, 3, 0, 0 )


--  
drpTitleSystem = UI_CreateCompent( frmMain800, DRAG_TITLE_TYPE, "drpTitleSystem", 43, 7, 0, -160 )
UI_LoadImage( drpTitleSystem, "texture/ui/SysBotton4.tga", NORMAL, 43, 7, 158, 100 )

--
scrollid = UI_GetScroll( lstOnSaySystem )
UI_SetSize( scrollid, 18, 18 )
--UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 18, 18, 194, 13 )

--
id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/Nbotton.tga", 18, 18, 138, 4, TRUE )
UI_SetSize( id, 18, 18 )

--
id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/Nbotton.tga", COMPENT_BACK, 18, 27, 195, 10 )
UI_SetSize( id, 18, 27 )

--
id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/Nbotton.tga", 18, 18, 138, 25, TRUE )
UI_SetSize( id, 18, 18 )

----------        
chkChatOn = UI_CreateCompent( frmMain800, CHECK_TYPE, "chkChatOn", 21, 20, 35, 5 )
UI_LoadImage( chkChatOn, "texture/ui/forge3.tga", UNCHECKED, 21, 20, 121, 133 )
UI_LoadImage( chkChatOn, "texture/ui/forge3.tga", CHECKED, 21, 20, 142, 133 )
UI_SetHint( chkChatOn , "Channel Display Button" )

----------
btnCard = UI_CreateCompent( frmMain800, BUTTON_TYPE, "btnCard", 6, 22, 56, 4 )
UI_LoadButtonImage( btnCard, "texture/ui/main800.tga", 6,22,135,139, TRUE )
--UI_LoadImage( btnCard, "texture/ui/chat/card.bmp", UNCHECKED, 7, 21, 0, 0 )
UI_SetHint( btnCard , "Whisper list" )

----------
lstCard = UI_CreateCompent( frmMain800, LIST_TYPE, "lstCard", 100, 80, 56, -79 )
UI_SetListRowHeight( lstCard, 14 )
UI_SetListFontColor( lstCard, COLOR_BLACK, COLOR_BLACK )
UI_LoadImage( lstCard, "texture/ui/QQ.dds", COMPENT_BACK, 35, 100, 212, 57 )
UI_SetListIsMouseFollow( lstCard, TRUE)
UI_ListLoadSelectImage( lstCard, "texture/ui/SystemBotton3.tga", 79, 14, 0, 227  )
UI_SetMargin( lstCard, 8,6,5,1 )
UI_SetIsShow( lstCard, FALSE )
--
scrollid = UI_GetScroll( lstCard )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )
--
id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )
--
id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 33, 166, 10 )
UI_SetSize( id, 11, 33 )
--
id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )


--
cboChannel  = UI_CreateCompent( frmMain800, COMBO_TYPE, "cboChannel", 45, 26, 2, 5 )
UI_ComboSetStyle( cboChannel, FALSE )
UI_LoadComboImage( cboChannel, "", 33, 26, 0, 0, "texture/ui/Quest1.tga", 1, 1, 30, 195, TRUE )
UI_ComboSetTextColor( cboChannel, COLOR_BLACK )
lstChannel = UI_GetList( cboChannel )
UI_SetListRowHeight( lstChannel, 14 )
UI_SetListFontColor( lstChannel, COLOR_BLACK, COLOR_BLACK )
UI_SetSize( lstChannel, 35, 120 )
UI_SetMargin( lstChannel, 5,8,5,4 )
UI_SetListRowHeight( lstChannel, 18 )
UI_LoadImage( lstChannel, "texture/ui/QQ.dds", COMPENT_BACK, 35, 100, 212, 57 )
UI_AddListText( lstChannel, "Local" )
UI_AddListText( lstChannel, "Party" )
UI_AddListText( lstChannel, "Guild" )
UI_AddListText( lstChannel, "Trade" )
UI_AddListText( lstChannel, "World" )
UI_AddListText( lstChannel, "Camp" )
UI_SetListIsMouseFollow( lstChannel, TRUE)
UI_ListLoadSelectImage( lstChannel, "texture/ui/SystemBotton3.tga", 79, 18, 0, 227  )
UI_ComboSetTextColor( lstChannel, COLOR_WHITE )

-- 
lstChannel = UI_CreateCompent( frmMain800, LIST_TYPE, "lstChannel", 75, 84, 0, -200 )
listid = UI_GetList( lstChannel )
UI_SetListRowHeight( listid, 15 )
--UI_LoadImage( listid, "texture/ui/selectcha.dds", NORMAL, 75, 85, 944, 1    )
UI_SetMargin( lstChannel, 12, 5, 6, 2 )
--scrollid = UI_GetScroll( listid )
--UI_SetSize( scrollid, 13, 13 )
--UI_LoadImage( scrollid, "texture/ui/chat/chat.dds", COMPENT_BACK, 9, 9, 0, 51 )
--scrollid = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
--UI_LoadImage( scrollid, "texture/ui/item.dds", COMPENT_BACK, 13, 17, 13, 374 )
--UI_SetSize( scrollid, 13, 17 )
--UI_SetAlpha( lstChannel, 100 )
--UI_SetIsKeyFocus( lstChannel, FALSE )
UI_SetIsShow( lstChannel, FALSE )
UI_SetListIsMouseFollow( lstChannel, TRUE )

--UI_SetFrameSelectAtrribute( lstChannel, 57, 15, 6, 3 )

UI_AddListText( lstChannel, "Local" )
UI_AddListText( lstChannel, "Party" )
UI_AddListText( lstChannel, "Guild" )
UI_AddListText( lstChannel, "Trade" )
UI_AddListText( lstChannel, "World" )
UI_AddListText( lstChannel, "Camp" )


-- 
btnChatFace = UI_CreateCompent( frmMain800, BUTTON_TYPE, "btnChatFace", 23, 23, 217, 3 )
UI_LoadButtonImage( btnChatFace, "texture/ui/Nbotton.tga", 23, 23, 0, 48, TRUE )
UI_SetHint( btnChatFace , "Emoticon" )

-- 
btnAction = UI_CreateCompent( frmMain800, BUTTON_TYPE, "btnAction", 23, 23, 240, 3 )
UI_LoadButtonImage( btnAction, "texture/ui/Nbotton.tga", 23, 23, 0, 23, TRUE )
UI_SetHint( btnAction, "Action" )

-- 
btnBrow = UI_CreateCompent( frmMain800, BUTTON_TYPE, "btnBrow", 23, 23, 263, 3 )
UI_LoadButtonImage( btnBrow, "texture/ui/Nbotton.tga", 23, 23, 0, 73, TRUE )
UI_SetHint( btnBrow, "Mood" )

-- 
grdFace = UI_CreateCompent( frmMain800, GRID_TYPE, "grdFace", 142, 170, 243, -168 ) 
UI_LoadImage( grdFace,  "texture/ui/QQ.dds", NORMAL, 142, 170, 0, 0 )
UI_SetGridUnitSize( grdFace, 20, 20 )
UI_SetIsShow( grdFace, FALSE )
UI_SetMargin( grdFace, 11, 5, 5, 5 )
UI_SetGridSpace( grdFace, 4, 4 )
UI_SetGridIsDragSize( grdFace, FALSE )
n=0
while 1 do
	name = "texture/ui/face/"..(n+1)..".tga"   	
    	UI_AddFaceToGrid( grdFace, name, 16, 16, 0, 0, 1, 0 )
	UI_SetTextParse( n , name, 16, 16 , 0 , 0 ,1 )
	n = n + 1    	
    	if n>=21 then break end
end
UI_GridLoadSelectImage( grdFace, "texture/ui/StartF.dds", 32, 32, 123, 99 )



-- 
grdAction = UI_CreateCompent( frmMain800, GRID_TYPE, "grdAction", 142, 170, 263, -168 ) 
UI_LoadImage( grdAction,  "texture/ui/QQ.dds", NORMAL, 142, 170, 0, 0 )
UI_SetGridUnitSize( grdAction, 32, 32 )
UI_SetIsShow( grdAction, FALSE )
UI_SetMargin( grdAction, 5, 4, 0, 0 )
UI_SetGridSpace( grdAction, 1, 1 )
UI_SetGridIsDragSize( grdAction, FALSE )

n=2
while 1 do
	name = "texture/ui/Action/"..(n)..".tga"   	
    	UI_AddFaceToGrid( grdAction, name, 32, 32, 0, 0, 1, n )	
	n = n + 1    	
    	if n>=10 then break end
end
n=16
while 1 do
	name = "texture/ui/Action/"..(n)..".tga"   	
    	UI_AddFaceToGrid( grdAction, name, 32, 32, 0, 0, 1, n )	
	n = n + 1    	
    	if n>=24 then break end
end
UI_GridLoadSelectImage( grdAction, "texture/ui/StartF.dds", 32, 32, 157, 99 )


-- 
grdHeart = UI_CreateCompent( frmMain800, GRID_TYPE, "grdHeart", 142, 170, 283, -168 ) 
UI_LoadImage( grdHeart,  "texture/ui/QQ.dds", NORMAL, 142, 170, 0, 0 )
UI_SetGridUnitSize( grdHeart, 32, 32 )
UI_SetIsShow( grdHeart, FALSE )
UI_SetMargin( grdHeart, 5, 4, 0, 0 )
UI_SetGridSpace( grdHeart, 0, 0 )
UI_SetGridIsDragSize( grdHeart, FALSE )
n=0
while 1 do
	name = "texture/ui/brow/"..(n)..".tga"   	
    	UI_AddFaceToGrid( grdHeart, name, 64, 64, 0, 0, 2, 0 )	
	n = n + 1    	
   	if n>=9 then break end
end
UI_GridLoadSelectImage( grdHeart, "texture/ui/StartF.dds", 32, 32, 123, 99 )

-- 
btnFastUp = UI_CreateCompent( frmMain800, BUTTON_TYPE, "btnFastUp", 7, 8, 298, 11 )
--UI_LoadButtonImage( btnFastUp, "texture/ui/Nbotton.tga", 7, 8, 82, 2, TRUE )

-- 
btnFastDown = UI_CreateCompent( frmMain800, BUTTON_TYPE, "btnFastDown", 7, 8, 298, 19 )
--UI_LoadButtonImage( btnFastDown, "texture/ui/Nbotton.tga", 7, 8, 82, 12, TRUE )


---
selfMouseRight = UI_CreateCompent( frmMain800, MENU_TYPE, "selfMouseRight", 68, 97, 240, 468 )
UI_MenuLoadImage( selfMouseRight, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( selfMouseRight, 10, 5, 5, 5 )

-- 
UI_MenuLoadSelect( selfMouseRight, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )

-- 
UI_MenuAddText( selfMouseRight, "Leave Party" )


-----------------
--- Skill Slot --
-----------------
frmFast = UI_CreateForm( "frmFast", FALSE, 432, 43, 290, 557, TRUE, FALSE )
--UI_FormSetHotKey( , ALT_KEY, HOTKEY_T ) 
UI_ShowForm( frmFast, FALSE )
UI_SetFormStyleEx( frmFast, FORM_BOTTOM, 0, 0)
UI_AddFormToTemplete( frmFast, FORM_MAIN )
UI_SetIsDrag( frmFast, TRUE )
UI_FormSetIsEscClose( frmFast, FALSE )

imgMain1 = UI_CreateCompent( frmFast, IMAGE_TYPE, "imgMain1", 256, 43, 0, 0 )
UI_LoadImage( imgMain1, "texture/ui/new4.tga", NORMAL, 256, 43, 0, 33 )

imgMain2 = UI_CreateCompent( frmFast, IMAGE_TYPE, "imgMain2", 176, 43, 256, 0 )
UI_LoadImage( imgMain2, "texture/ui/new4.tga", NORMAL, 176, 43, 52, 79 )

fscMainF0  = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF0",  32, 32, 5, 5 )
fscMainF1  = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF1",  32, 32, 39, 5 )
fscMainF2  = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF2",  32, 32, 73, 5 )
fscMainF3  = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF3",  32, 32, 107, 5 )
fscMainF4  = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF4",  32, 32, 141, 5 )
fscMainF5  = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF5",  32, 32, 175, 5 )
fscMainF6  = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF6",  32, 32, 209, 5 )
fscMainF7  = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF7",  32, 32, 243, 5 )
fscMainF8  = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF8",  32, 32, 277, 5 )
fscMainF9  = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF9",  32, 32, 311, 5 )
fscMainF10 = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF10", 32, 32, 345, 5 )
fscMainF11 = UI_CreateCompent( frmFast, FAST_COMMANG_TYPE, "fscMainF11", 32, 32, 379, 5 )

btnFastUp = UI_CreateCompent( frmFast, BUTTON_TYPE, "btnFastUp", 7, 8, 417, 12 )
UI_LoadButtonImage( btnFastUp, "texture/ui/Nbotton.tga", 7, 8, 82, 2, TRUE )
UI_SetHint( btnFastUp, "+" )

btnFastDown = UI_CreateCompent( frmFast, BUTTON_TYPE, "btnFastDown", 7, 8, 417, 22 )
UI_LoadButtonImage( btnFastDown, "texture/ui/Nbotton.tga", 7, 8, 82, 12, TRUE )
UI_SetHint( btnFastDown, "-" )

-----------------------------------------------------------------------
frmFast2 = UI_CreateForm( "frmFast2", FALSE, 432, 43, 290, 514, TRUE, FALSE )
UI_ShowForm( frmFast2, FALSE )
UI_SetFormStyleEx( frmFast2, FORM_BOTTOM, 0, 43)
UI_AddFormToTemplete( frmFast2, FORM_MAIN )
UI_SetIsDrag( frmFast2, TRUE ) --true or false 
UI_FormSetIsEscClose( frmFast2, FALSE )

imgMain1 = UI_CreateCompent( frmFast2, IMAGE_TYPE, "imgMain1", 256, 43, 0, 0 )
UI_LoadImage( imgMain1, "texture/ui/new4.tga", NORMAL, 256, 43, 0, 125 )

imgMain2 = UI_CreateCompent( frmFast2, IMAGE_TYPE, "imgMain2", 176, 43, 256, 0 )
UI_LoadImage( imgMain2, "texture/ui/new4.tga", NORMAL, 176, 43, 52, 171 )

fscMainF212  = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF212",  32, 32, 5, 5 )
fscMainF213  = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF213",  32, 32, 39, 5 )
fscMainF214  = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF214",  32, 32, 73, 5 )
fscMainF215  = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF215",  32, 32, 107, 5 )
fscMainF216  = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF216",  32, 32, 141, 5 )
fscMainF217 = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF217",  32, 32, 175, 5 )
fscMainF218  = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF218",  32, 32, 209, 5 )
fscMainF219  = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF219",  32, 32, 243, 5 )
fscMainF220  = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF220",  32, 32, 277, 5 )
fscMainF221  = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF221",  32, 32, 311, 5 )
fscMainF222 = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF222", 32, 32, 345, 5 )
fscMainF223 = UI_CreateCompent( frmFast2, FAST_COMMANG_TYPE, "fscMainF223", 32, 32, 379, 5 )

---------------
-- Quickslot --
---------------
frmMainFun = UI_CreateForm( "frmMainFun", FALSE, 215,  30, 585, 570, TRUE, FALSE )
--UI_FormSetHotKey( frmMainFun, ALT_KEY, HOTKEY_D ) 
UI_ShowForm( frmMainFun, TRUE )
UI_AddFormToTemplete( frmMainFun, FORM_MAIN )
UI_FormSetIsEscClose( frmMainFun, FALSE )
UI_SetFormStyle( frmMainFun, 11 )---

imgMainFun1 = UI_CreateCompent( frmMainFun, IMAGE_TYPE, "imgMainFun1", 183, 30, 0, 0 )
UI_LoadImage( imgMainFun1, "texture/ui/new.tga", NORMAL, 183, 30, 73, 92 )

imgMainFun2 = UI_CreateCompent( frmMainFun, IMAGE_TYPE, "imgMainFun2", 32, 30, 183, 0 )
UI_LoadImage( imgMainFun2, "texture/ui/new.tga", NORMAL, 32, 30, 0, 132 )

btnState = UI_CreateCompent( frmMainFun, BUTTON_TYPE, "btnState", 23, 22, 7, 4 )
UI_LoadButtonImage( btnState, "texture/ui/new6.tga", 23, 22, 0, 0,TRUE )
UI_SetHint( btnState , "Stats (Alt+A)" )

btnOpenBag = UI_CreateCompent( frmMainFun, BUTTON_TYPE, "btnOpenBag", 23, 22, 31, 4 )
UI_LoadButtonImage( btnOpenBag, "texture/ui/new6.tga", 23, 22, 0, 22,TRUE )
UI_SetHint( btnOpenBag , "Item" )

btnSkill = UI_CreateCompent( frmMainFun, BUTTON_TYPE, "btnSkill", 23, 22, 55, 4  )
UI_LoadButtonImage( btnSkill, "texture/ui/new6.tga", 23, 22, 0, 44,TRUE )
UI_SetHint( btnSkill , "Skill(Alt+S)" )

btnMission = UI_CreateCompent( frmMainFun, BUTTON_TYPE, "btnMission", 23, 22, 79, 4  )
UI_LoadButtonImage( btnMission, "texture/ui/new6.tga", 23, 22, 0, 66,TRUE )
UI_SetHint( btnMission , "Quest(Alt+Q)" )

btnGuild = UI_CreateCompent( frmMainFun, BUTTON_TYPE, "btnGuild", 23, 22, 103, 4  )
UI_LoadButtonImage( btnGuild, "texture/ui/new6.tga", 23, 22, 0, 88,TRUE )
UI_SetHint( btnGuild , "Guild(Alt+C)" )

btnOpenSociliaty = UI_CreateCompent( frmMainFun, BUTTON_TYPE, "btnOpenSociliaty", 23, 22, 103, 4  )
UI_LoadButtonImage( btnOpenSociliaty, "texture/ui/new6.tga", 23, 22, 69, 0, TRUE )
UI_SetHint( btnOpenSociliaty , "Social Skill" )

btnShip = UI_CreateCompent( frmMainFun, BUTTON_TYPE, "btnShip", 23, 22, 127, 4  )
UI_LoadButtonImage( btnShip, "texture/ui/new6.tga",  23, 22, 0, 110,TRUE )
UI_SetHint( btnShip , "Help(Alt+H)" )

btnSystem = UI_CreateCompent( frmMainFun, BUTTON_TYPE, "btnSystem", 23, 22, 151, 4  )
UI_LoadButtonImage( btnSystem, "texture/ui/new6.tga",  23, 22, 0, 132, TRUE )
UI_SetHint( btnSystem , "Setting(Alt+O)" )

btnQQ = UI_CreateCompent( frmMainFun, BUTTON_TYPE, "btnQQ", 37, 22, 175, 4 )
UI_LoadButtonImage( btnQQ, "texture/ui/new6.tga", 37, 22, 0, 154, TRUE )
UI_SetHint( btnQQ , "Friend (Alt+F)" )

btnLevelUpHelp = UI_CreateCompent( frmMainFun, BUTTON_TYPE, "btnLevelUpHelp", 48, 48 , 160, -110 )
UI_LoadButtonImage( btnLevelUpHelp, "texture/ui/newbutton.tga", 41, 41, 0, 0, FALSE )
UI_SetIsShow(btnLevelUpHelp,FALSE)

btnInfoCenter = UI_CreateCompent( frmMainFun, BUTTON_TYPE, "btnInfoCenter",24, 24, 172, -60 )
UI_LoadButtonImage( btnInfoCenter, "texture/ui/newbutton.tga", 41, 41, 41, 0, FALSE )
UI_SetIsShow(btnInfoCenter,FALSE)

-------------------------------------------------------------------------
---- 			--          btnOpenBag
-------------------------------------------------------------------------	
frmBag = UI_CreateForm( "frmBag", FALSE, 89,  30, 450, 250, TRUE, FALSE )
--UI_FormSetHotKey( frmItem, ALT_KEY, HOTKEY_D ) 
UI_ShowForm( frmBag, TRUE )
UI_AddFormToTemplete( frmBag, FORM_MAIN )
UI_FormSetIsEscClose( frmBag, FALSE )

--
imgMainFun1 = UI_CreateCompent( frmBag, IMAGE_TYPE, "imgMainFun1", 32, 30, 57, 0 )
UI_LoadImage( imgMainFun1, "texture/ui/new4.tga", NORMAL, 32, 30, 0, 0 )

imgMainFun2 = UI_CreateCompent( frmBag, IMAGE_TYPE, "imgMainFun2", 32, 30, 0, 0 )
UI_LoadImage( imgMainFun2, "texture/ui/new4.tga", NORMAL, 32, 30, 32, 0 )

imgMainFun2 = UI_CreateCompent( frmBag, IMAGE_TYPE, "imgMainFun2", 25, 30, 32, 0 )
UI_LoadImage( imgMainFun2, "texture/ui/new4.tga", NORMAL, 2, 30, 0, 0 )

--
--
btnOpenItem = UI_CreateCompent( frmBag, BUTTON_TYPE, "btnOpenItem", 37, 22, 7, 4 )
UI_LoadButtonImage( btnOpenItem, "texture/ui/new6.tga", 37, 22, 0, 198,TRUE )
UI_SetHint( btnOpenItem , "Inventory(Alt+E)" )

--
btnOpenTempBag = UI_CreateCompent( frmBag, BUTTON_TYPE, "btnOpenTempBag", 37, 22, 45, 4 )
UI_LoadButtonImage( btnOpenTempBag, "texture/ui/new6.tga", 37, 22, 0, 220,TRUE )
UI_SetHint( btnOpenTempBag , "Temporary Bag" )

-- btnOpenStore = UI_CreateCompent( frmBag, BUTTON_TYPE, "btnOpenStore", 37, 22, 83, 4 )
-- UI_LoadButtonImage( btnOpenStore, "texture/ui/new6.tga", 37, 22, 0, 176, TRUE )
-- UI_SetHint( btnOpenStore , "Item Mall" )
-------------------------------------------------------------------------
---- 			--		btnSociliaty
-------------------------------------------------------------------------	
frmSociliaty = UI_CreateForm( "frmSociliaty", FALSE, 37,  30, 400, 300, TRUE, FALSE )
--UI_FormSetHotKey( frmItem, ALT_KEY, HOTKEY_D ) 
UI_ShowForm( frmSociliaty, TRUE )
UI_AddFormToTemplete( frmSociliaty, FORM_MAIN )
UI_FormSetIsEscClose( frmSociliaty, FALSE )

--
imgMainFun1 = UI_CreateCompent( frmSociliaty, IMAGE_TYPE, "imgMainFun1", 25, 30, 0, 0 )
UI_LoadImage( imgMainFun1, "texture/ui/new4.tga", NORMAL, 25, 30, 32, 0 )

imgMainFun2 = UI_CreateCompent( frmSociliaty, IMAGE_TYPE, "imgMainFun2", 25, 30, 12, 0 )
UI_LoadImage( imgMainFun2, "texture/ui/new4.tga", NORMAL, 25, 30, 7, 0 )

--
--  btnGuild
btnOpenGuild = UI_CreateCompent( frmSociliaty, BUTTON_TYPE, "btnOpenGuild", 23, 22, 7, 4  )
UI_LoadButtonImage( btnOpenGuild, "texture/ui/new6.tga", 23, 22, 0, 88,TRUE )
UI_SetHint( btnOpenGuild , "Guild(Alt+C)" )

--   
-- btnOpenTeam = UI_CreateCompent( frmSociliaty, BUTTON_TYPE, "btnOpenTeam", 23, 22, 31, 4 )
-- UI_LoadButtonImage( btnOpenTeam, "texture/ui/new6.tga", 23, 22, 69, 22, TRUE )
-- UI_SetHint( btnOpenTeam , "Party Volunteer" )

---------------------------
-- main main (world map) --
---------------------------
frmBigmap = UI_CreateForm( "frmBigmap", FALSE, 569, 430, 30, 80, TRUE, FALSE )
UI_ShowForm( frmBigmap, TRUE )
UI_AddFormToTemplete( frmBigmap, FORM_MAIN )
UI_FormSetIsEscClose( frmBigmap, FALSE )
UI_FormSetHotKey( frmBigmap, ALT_KEY, HOTKEY_W )
UI_FormSetIsEscClose( frmBigmap, FALSE )

imgBigmap1 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap1", 256, 256, 0, 0 )
UI_LoadImage( imgBigmap1, "texture/ui/bigmap1.tga", NORMAL, 256, 256, 0, 0 )

imgBigmap2 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap2", 256, 256, 256, 0 )
UI_LoadImage( imgBigmap2, "texture/ui/bigmap2.tga", NORMAL, 256, 256, 0, 0 )

imgBigmap3 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap3", 256, 256, 512, 0 )
UI_LoadImage( imgBigmap3, "texture/ui/bigmap3.tga", NORMAL, 256, 256, 0, 0 )

imgBigmap4 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap4", 256, 256, 0, 256 )
UI_LoadImage( imgBigmap4, "texture/ui/bigmap4.tga", NORMAL, 256, 256, 0, 0 )

imgBigmap5 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap5", 256, 256, 256, 256 )
UI_LoadImage( imgBigmap5, "texture/ui/bigmap5.tga", NORMAL, 256, 256, 0, 0 )

imgBigmap6 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap6", 256, 256, 512, 256 )
UI_LoadImage( imgBigmap6, "texture/ui/bigmap6.tga", NORMAL, 256, 256, 0, 0 )

UI_CreateCompent( frmBigmap, UI3D_COMPENT_TYPE, "d3dCompent", 530, 390, 140, 52 )


imgBigmap11 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap11", 256, 256, 0, 0 )
UI_LoadImage( imgBigmap11, "texture/ui/bigmap8.tga", NORMAL, 256, 256, 0, 0 )

imgBigmap21 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap21", 256, 97, 256, 10 )
UI_LoadImage( imgBigmap21, "texture/ui/bigmap9.tga", NORMAL, 256, 97, 0, 0 )

imgBigmap31 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap31", 256, 256, 512, 0 )
UI_LoadImage( imgBigmap31, "texture/ui/bigmap10.tga", NORMAL, 256, 256, 0, 0 )

imgBigmap41 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap41", 256, 256, 0, 256 )
UI_LoadImage( imgBigmap41, "texture/ui/bigmap11.tga", NORMAL, 256, 256, 0, 0 )

imgBigmap51 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap51", 256, 97, 256, 373 )
UI_LoadImage( imgBigmap51, "texture/ui/bigmap9.tga", NORMAL, 256, 97, 0, 159 )

imgBigmap61 = UI_CreateCompent( frmBigmap, IMAGE_TYPE, "imgBigmap61", 256, 256, 512, 256 )
UI_LoadImage( imgBigmap61, "texture/ui/bigmap12.tga", NORMAL, 256, 256, 0, 0 )


btnLeft = UI_CreateCompent( frmBigmap, BUTTON_TYPE, "btnLeft", 15, 39, 160, 230 )
UI_LoadButtonImage( btnLeft, "texture/ui/SysBotton4.tga", 15, 39, 165, 39, TRUE )

btnRight = UI_CreateCompent( frmBigmap, BUTTON_TYPE, "btnRight", 15, 39, 640, 230 )
UI_LoadButtonImage( btnRight, "texture/ui/SysBotton4.tga", 15, 39, 165, 0, TRUE )

btnTop = UI_CreateCompent( frmBigmap, BUTTON_TYPE, "btnTop", 39, 15, 387, 60 )
UI_LoadButtonImage( btnTop, "texture/ui/ShipBuildT.tga", 39, 15, 132, 185, FALSE )

btnDown = UI_CreateCompent( frmBigmap, BUTTON_TYPE, "btnDown", 39, 15, 387, 420 )
UI_LoadButtonImage( btnDown, "texture/ui/ShipBuildT.tga", 39, 15, 132, 123, FALSE )

btnZoomIn = UI_CreateCompent(frmBigmap, BUTTON_TYPE, "btnZoomIn", 15, 15, 526, 371)
UI_LoadButtonImage(btnZoomIn, "texture/ui/new/frmBigmap.png", 10, 10, 756, 256, FALSE )

btnZoomOut = UI_CreateCompent(frmBigmap, BUTTON_TYPE, "btnZoomOut", 15, 15, 543, 375)
UI_LoadButtonImage(btnZoomOut, "texture/ui/new/frmBigmap.png", 10, 10, 771, 259, FALSE )

btnNo = UI_CreateCompent( frmBigmap, BUTTON_TYPE, "btnNo", 111, 36, 615, 415 )
UI_LoadButtonImage( btnNo, "texture/ui/Close.tga", 111, 36, 0, 0, FALSE )
UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )

-----------------------------------------------------------------------
-- 
-----------------------------------------------------------------------	
-- 
frmFollow = UI_CreateForm( "frmFollow", FALSE, 0, 0, 350, 280, FALSE, FALSE )
UI_ShowForm( frmFollow, TRUE )
UI_AddFormToTemplete( frmFollow, FORM_MAIN )
UI_FormSetIsEscClose( frmFollow, FALSE )

labFollow = UI_CreateCompent( frmFollow, LABELEX_TYPE, "labFollow", 100, 40, 0, 0 )
UI_SetCaption( labFollow, "" )
UI_SetTextColor( labFollow, COLOR_BLACK )
UI_SetLabelExFont( labFollow, DEFAULT_FONT, TRUE, COLOR_WHITE )

------------------------------------
-- Gm Panel --
-------------------------------------
frmGM = UI_CreateForm( "frmGM", FALSE, 646, 106, 80, 545, TRUE, FALSE )
UI_FormSetHotKey( frmGM, ALT_KEY, HOTKEY_P ) 
UI_ShowForm( frmGM, FALSE )
UI_AddFormToTemplete( frmGM, FORM_MAIN )
UI_FormSetIsEscClose( frmGM, FALSE )
UI_SetIsDrag( frmGM, TRUE )
UI_SetFormStyleEx( frmGM, FORM_BOTTOM, 0, 45)

imgMain3 = UI_CreateCompent( frmGM, IMAGE_TYPE, "imgMain3", 646, 106, 0, -15 )
UI_LoadImage( imgMain3, "texture/ui/corsairs/gmsay.png", NORMAL, 646, 106, 0, 0 )

btnClose = UI_CreateCompent( frmGM, BUTTON_TYPE, "btnClose", 41, 19, 300, 68 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labTitle = UI_CreateCompent( frmGM, LABELEX_TYPE, "labTitle", 283, 16, 10, -11 )
UI_SetCaption( labTitle, "GM Panel")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- 
edtGMSay = UI_CreateCompent( frmGM, EDIT_TYPE, "edtGMSay", 579, 19, 43, 39 )
UI_SetTextColor( edtGMSay, COLOR_BLACK )
UI_SetEditMaxNum( edtGMSay, 800)
UI_SetEditMaxNumVisible( edtGMSay, 800)
UI_SetEditCursorColor( edtGMSay, COLOR_BLACK )

-----------------------------------------------------------------------
-- Enable / Disable Chat
-----------------------------------------------------------------------	
-- 
frmForbid = UI_CreateForm( "frmForbid", FALSE, 33, 160, 34, 555, FALSE, FALSE )
UI_ShowForm( frmForbid, FALSE )
UI_AddFormToTemplete( frmForbid, FORM_MAIN )
UI_FormSetIsEscClose( frmForbid, TRUE )
--UI_FormSetHotKey( frmForbid, ALT_KEY, HOTKEY_Z )
imgForbid = UI_CreateCompent( frmForbid, IMAGE_TYPE, "imgForbid", 33, 160, 0, 0 )
--UI_LoadImage( imgForbid, "texture/ui/corsairs/forge3.png", NORMAL, 33, 160, 233, 40 ) 

--
chkPersonal = UI_CreateCompent( frmForbid, CHECK_TYPE, "chkPersonal", 33, 16, -33, 0 )
UI_LoadImage( chkPersonal, "texture/ui/corsairs/forge3.png", UNCHECKED, 33, 16, 0, 101 )
UI_LoadImage( chkPersonal, "texture/ui/corsairs/forge3.png", CHECKED, 33, 16, 0, 117 )

--
chkNear = UI_CreateCompent( frmForbid, CHECK_TYPE, "chkNear", 33, 16, 0, 0 )
UI_LoadImage( chkNear, "texture/ui/corsairs/forge3.png", UNCHECKED, 33, 16, 33, 101 )
UI_LoadImage( chkNear, "texture/ui/corsairs/forge3.png", CHECKED, 33, 16, 33, 117 )

--
chkSystem = UI_CreateCompent( frmForbid, CHECK_TYPE, "chkSystem", 33, 16, 33, 0 )
UI_LoadImage( chkSystem, "texture/ui/corsairs/forge3.png", UNCHECKED, 33, 16, 66, 101 )
UI_LoadImage( chkSystem, "texture/ui/corsairs/forge3.png", CHECKED, 33, 16, 66, 117 )

--
chkTeam = UI_CreateCompent( frmForbid, CHECK_TYPE, "chkTeam", 33, 16, 33*2, 0 )
UI_LoadImage( chkTeam, "texture/ui/corsairs/forge3.png", UNCHECKED, 33, 16, 99, 101 )
UI_LoadImage( chkTeam, "texture/ui/corsairs/forge3.png", CHECKED, 33, 16, 99, 117 )

--
chkGuild = UI_CreateCompent( frmForbid, CHECK_TYPE, "chkGuild", 33, 16, 33*3, 0 )
UI_LoadImage( chkGuild, "texture/ui/corsairs/forge3.png", UNCHECKED, 33, 16, 132, 101 )
UI_LoadImage( chkGuild, "texture/ui/corsairs/forge3.png", CHECKED, 33, 16, 132, 117 )

--
chkWorld = UI_CreateCompent( frmForbid, CHECK_TYPE, "chkWorld", 33, 16, 33*4, 0 )
UI_LoadImage( chkWorld, "texture/ui/corsairs/forge3.png", UNCHECKED, 33, 16, 165, 101 )
UI_LoadImage( chkWorld, "texture/ui/corsairs/forge3.png", CHECKED, 33, 16, 165, 117 )

--
chkTrade = UI_CreateCompent( frmForbid, CHECK_TYPE, "chkTrade", 33, 16, 33*5, 0 )
UI_LoadImage( chkTrade, "texture/ui/corsairs/forge3.png", UNCHECKED, 33, 16, 198, 101 )
UI_LoadImage( chkTrade, "texture/ui/corsairs/forge3.png", CHECKED, 33, 16, 198, 117 )

chkVIP = UI_CreateCompent( frmForbid, CHECK_TYPE, "chkVIP", 33, 16, 33*6, 0 )
--UI_LoadImage( chkVIP, "texture/ui/corsairs/forge3.png", UNCHECKED, 33, 16, 165, 133 )
--UI_LoadImage( chkVIP, "texture/ui/corsairs/forge3.png", CHECKED, 33, 16, 165, 149 )

-----------------
-- Pet Profile --
-----------------
frmMainPet = UI_CreateForm( "frmMainPet", FALSE, 171, 31, 4, 53, FALSE, FALSE )
UI_ShowForm( frmMainPet, FALSE )
UI_AddFormToTemplete( frmMainPet, FORM_MAIN )
UI_FormSetIsEscClose( frmMainPet, FALSE )
--UI_FormSetHotKey( frmMainPet, ALT_KEY, HOTKEY_V )

imgMainPet = UI_CreateCompent( frmMainPet, IMAGE_TYPE, "imgMainPet", 171, 31, 0, 0 )
UI_LoadImage( imgMainPet, "texture/ui/corsairs/new3.tga", NORMAL, 171, 31, 0, 52 )

imgPetHead = UI_CreateCompent( frmMainPet, IMAGE_TYPE, "imgPetHead", 23, 23, 33, 4 )

labPetLv = UI_CreateCompent( frmMainPet, LABELEX_TYPE, "labPetLv", 14, 9, 64, 5 )
UI_SetTextColor( labPetLv, COLOR_WHITE )
UI_SetLabelExFont( labPetLv, DEFAULT_FONT, TRUE, COLOR_BLACK )
UI_SetCaption( labPetLv, "99" )

proPetHP = UI_CreateCompent( frmMainPet, PROGRESS_TYPE, "proPetHP", 64, 8, 102, 2 )
UI_LoadScaleImage( proPetHP, "texture/ui/corsairs/new3.tga", PROGRESS_PROGRESS, 64, 8, 0, 120, 1.0, 1.0 )
UI_SetHint( proPetHP, "Stamina" )
UI_SetProgressHintStyle( proPetHP, PROGRESS_HINT_NUM )

proPetSP = UI_CreateCompent( frmMainPet, PROGRESS_TYPE, "proPetSP", 64, 8, 102, 12 )
UI_LoadScaleImage( proPetSP, "texture/ui/corsairs/new3.tga", PROGRESS_PROGRESS, 64, 8, 66, 120, 1.0, 1.0 )
UI_SetHint( proPetSP, "Growth" )
UI_SetProgressHintStyle( proPetSP, PROGRESS_HINT_NUM )


-------------------
---- Help Menu ----
-------------------
frmHelpSystem = UI_CreateForm( "frmHelpSystem", FALSE, 480, 350, 300, 100, TRUE, FALSE )
UI_ShowForm( frmHelpSystem, TRUE )
UI_AddFormToTemplete( frmHelpSystem, FORM_MAIN )
UI_FormSetIsEscClose( frmHelpSystem, TRUE )
UI_SetIsDrag(frmHelpSystem,TRUE)
UI_SetFormStyle(frmHelpSystem,1)
--UI_FormSetHotKey( frmHelpSystem, ALT_KEY, HOTKEY_Z )

imgBackGround1 = UI_CreateCompent( frmHelpSystem, IMAGE_TYPE, "imgBackGround1", 256, 256, 0, 0 )
UI_LoadImage( imgBackGround1, "texture/ui/newshelp/newhelp01.tga", NORMAL, 256,256, 0, 0 )

imgBackGround2 = UI_CreateCompent( frmHelpSystem, IMAGE_TYPE, "imgBackGround2", 256, 256, 256, 0 )
UI_LoadImage( imgBackGround2, "texture/ui/newshelp/newhelp02.tga", NORMAL, 256,256, 0, 0 )

imgBackGround3 = UI_CreateCompent( frmHelpSystem, IMAGE_TYPE, "imgBackGround3", 256, 206, 0, 256 )
UI_LoadImage( imgBackGround3, "texture/ui/newshelp/newhelp03.tga", NORMAL, 256,206, 0, 0 )

imgBackGround4 = UI_CreateCompent( frmHelpSystem, IMAGE_TYPE, "imgBackGround4", 256, 206, 256, 256 )
UI_LoadImage( imgBackGround4, "texture/ui/newshelp/newhelp04.tga", NORMAL, 256,206, 0, 0 )

imgBackGround5 = UI_CreateCompent( frmHelpSystem, IMAGE_TYPE, "imgBackGround5", 93, 256, 512, 0 )
UI_LoadImage( imgBackGround5, "texture/ui/newshelp/newhelp05.tga", NORMAL, 93,256, 0, 0 )

imgBackGround6 = UI_CreateCompent( frmHelpSystem, IMAGE_TYPE, "imgBackGround6", 93, 206, 512, 256 )
UI_LoadImage( imgBackGround6, "texture/ui/newshelp/newhelp05.tga", NORMAL, 93,206,93,0  )

labTitle = UI_CreateCompent( frmHelpSystem, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Help System")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

lstHelpList = UI_CreateCompent( frmHelpSystem, LIST_TYPE, "lstHelpList", 170, 390, 19,42 )

lstHelpButton = UI_GetList(lstHelpList)
UI_SetListRowHeight(lstHelpButton,15)
UI_SetListIsMouseFollow( lstHelpList, TRUE )

	scrollid = UI_GetScroll( lstHelpButton )
	UI_SetSize( scrollid, 11, 1 )
	UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )
	
	id = UI_GetScrollObj( scrollid, SCROLL_UP )
	UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
	UI_SetSize( id, 11, 9 )

	id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
	UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 33, 166, 10 )
	UI_SetSize( id, 11, 33 )

	id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
	UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
	UI_SetSize( id, 11, 9 )

local tabImage1 = {}
local tabImage2 = {}
local tabImage3 = {}
local tabImage4 = {}
for i=1,68,1 do
	tabImage1[i]  = UI_CreateCompent( frmHelpSystem, IMAGE_TYPE, "imgHelpShow"..i.."_1", 256, 256, 223, 43 )
	UI_LoadImage( tabImage1[i], "texture/ui/newshelp/"..i.."_1.tga", NORMAL, 256,256, 0, 0 )
	UI_SetIsShow(tabImage1[i],FALSE)

	tabImage2[i]  = UI_CreateCompent( frmHelpSystem, IMAGE_TYPE, "imgHelpShow"..i.."_2",256, 132, 223, 299 )
	UI_LoadImage( tabImage2[i], "texture/ui/newshelp/"..i.."_2.tga", NORMAL, 256,132, 0, 0 )
	UI_SetIsShow(tabImage2[i],FALSE)

	tabImage3[i]  = UI_CreateCompent( frmHelpSystem, IMAGE_TYPE, "imgHelpShow"..i.."_3", 104, 256, 479, 43 )
	UI_LoadImage( tabImage3[i], "texture/ui/newshelp/"..i.."_3.tga", NORMAL, 104,256, 0, 0 )
	UI_SetIsShow(tabImage3[i],FALSE)

	tabImage4[i]  = UI_CreateCompent( frmHelpSystem, IMAGE_TYPE, "imgHelpShow"..i.."_4", 104, 132, 479, 299)
	UI_LoadImage( tabImage4[i], "texture/ui/newshelp/"..i.."_3.tga", NORMAL, 104, 132, 104, 0 )
	UI_SetIsShow(tabImage4[i],FALSE)
end

btnClose = UI_CreateCompent( frmHelpSystem, BUTTON_TYPE, "btnClose", 14, 14, 585, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

UI_AddListText( lstHelpList,"Attribute")
UI_AddListText( lstHelpList,"Classes")
UI_AddListText( lstHelpList,"Swordsman")
UI_AddListText( lstHelpList,"Hunter")
UI_AddListText( lstHelpList,"Herbalist")
UI_AddListText( lstHelpList,"Explorer")
UI_AddListText( lstHelpList,"Crusader")
UI_AddListText( lstHelpList,"Champion")
UI_AddListText( lstHelpList,"Cleric")
UI_AddListText( lstHelpList,"Seal Master")
UI_AddListText( lstHelpList,"Sharpshooter")
UI_AddListText( lstHelpList,"Voyager")
UI_AddListText( lstHelpList,"Item")
UI_AddListText( lstHelpList,"Skill")
UI_AddListText( lstHelpList,"Quest")
UI_AddListText( lstHelpList,"Friends/Guild")
UI_AddListText( lstHelpList,"Help Interface")
UI_AddListText( lstHelpList,"System Setting")
UI_AddListText( lstHelpList,"Chat System")
UI_AddListText( lstHelpList,"Movement")
UI_AddListText( lstHelpList,"Battle Mode")
UI_AddListText( lstHelpList,"NPC")
UI_AddListText( lstHelpList,"Camera View")
UI_AddListText( lstHelpList,"Party")
UI_AddListText( lstHelpList,"Trade")
UI_AddListText( lstHelpList,"World Map")
UI_AddListText( lstHelpList,"Account Secrecy")
UI_AddListText( lstHelpList,"Game Master")
UI_AddListText( lstHelpList,"Shortcut Keys")
UI_AddListText( lstHelpList,"Radar Guide")
UI_AddListText( lstHelpList,"Equipment")
UI_AddListText( lstHelpList,"Class Advancement")
UI_AddListText( lstHelpList,"Item Pickup")
UI_AddListText( lstHelpList,"HP/SP Recovery")
UI_AddListText( lstHelpList,"Special Chest")
UI_AddListText( lstHelpList,"Help!")
UI_AddListText( lstHelpList,"Advancement Quest I")
UI_AddListText( lstHelpList,"Advancement Quest II")
UI_AddListText( lstHelpList,"Learning Skill")
UI_AddListText( lstHelpList,"Official Guide")
UI_AddListText( lstHelpList,"Leveling")
UI_AddListText( lstHelpList,"Story Quest")
UI_AddListText( lstHelpList,"Teleportation")
UI_AddListText( lstHelpList,"Lifeskill")
UI_AddListText( lstHelpList,"Emoticons")
UI_AddListText( lstHelpList,"Spawn Point")
UI_AddListText( lstHelpList,"Storage")
UI_AddListText( lstHelpList,"Accessory")
UI_AddListText( lstHelpList,"Sailing")
UI_AddListText( lstHelpList,"Party Benefits")
UI_AddListText( lstHelpList,"Whisper")
UI_AddListText( lstHelpList,"Decorative Wings")
UI_AddListText( lstHelpList,"Follow Mode")
UI_AddListText( lstHelpList,"Equipment Attribute")
UI_AddListText( lstHelpList,"Experience Cap")
UI_AddListText( lstHelpList,"Sea Commerce")
UI_AddListText( lstHelpList,"Virtual Bag")
UI_AddListText( lstHelpList,"Pet Fairy")
UI_AddListText( lstHelpList,"System Notice")
UI_AddListText( lstHelpList,"Training Area")
UI_AddListText( lstHelpList,"Discussion Forum")
UI_AddListText( lstHelpList,"Events")
UI_AddListText( lstHelpList,"Class Advancement II")
UI_AddListText( lstHelpList,"Mazes")
UI_AddListText( lstHelpList,"Runestones")
UI_AddListText( lstHelpList,"Guild")
UI_AddListText( lstHelpList,"Advancement I")
UI_AddListText( lstHelpList,"Advancement II")



-----------------
-- Ingame Mall --
-----------------
frmStore = UI_CreateForm( "frmStore", FALSE, 600, 550, 0, 0, TRUE, FALSE )
UI_ShowForm( frmStore, FALSE )
UI_AddFormToTemplete( frmStore, FORM_MAIN )
UI_FormSetIsEscClose( frmStore, FALSE )
UI_SetIsDrag( frmStore,FALSE)
UI_SetFormStyle( frmStore,1)

--UI_FormSetHotKey( frmStore, ALT_KEY, HOTKEY_Z )

--
imgBackGround1 = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBackGround1", 256, 256, 0, 0 )
UI_LoadImage( imgBackGround1, "texture/ui/store1.tga", NORMAL, 256,256, 0, 0 )

imgBackGround2 = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBackGround2", 256, 256, 256, 0 )
UI_LoadImage( imgBackGround2, "texture/ui/store2.tga", NORMAL, 256,256, 0, 0 )

imgBackGround3 = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBackGround3", 88, 256, 512, 0 )
UI_LoadImage( imgBackGround3, "texture/ui/store5.tga", NORMAL, 88,256, 0, 0 )

imgBackGround4 = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBackGround4", 256, 256, 0, 256 )
UI_LoadImage( imgBackGround4, "texture/ui/store3.tga", NORMAL, 256,256, 0, 0 )

imgBackGround5 = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBackGround5", 256, 256, 256, 256 )
UI_LoadImage( imgBackGround5, "texture/ui/store4.tga", NORMAL, 256,256, 0, 0 )

imgBackGround5 = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBackGround5", 256, 5, 256, 507 )
UI_LoadImage( imgBackGround5, "texture/ui/store3.tga", NORMAL, 5,5, 189, 251 )

imgBackGround6 = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBackGround6", 88, 256, 512, 256 )
UI_LoadImage( imgBackGround6, "texture/ui/store5.tga", NORMAL, 88,256, 88, 0 )

imgBackGround7 = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBackGround7", 256, 38, 0, 512 )
UI_LoadImage( imgBackGround7, "texture/ui/store6.tga", NORMAL, 255,38, 0, 0 )

imgBackGround8 = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBackGround8", 256, 38, 256, 512 )
UI_LoadImage( imgBackGround8, "texture/ui/store6.tga", NORMAL, 255,38, 0, 38 )

imgBackGround9 = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBackGround9", 88, 38, 512, 512 )
UI_LoadImage( imgBackGround9, "texture/ui/store6.tga", NORMAL, 88,38, 0, 76 )

--
imgblack = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgblack", 550, 17, 42, 29 )
UI_LoadImage( imgblack, "texture/ui/store1.tga", NORMAL, 5,17, 27, 29 )

	--title
	labTitle = UI_CreateCompent( frmStore, LABELEX_TYPE, "labTitle", 10, 150, 7 ,3  )
	UI_SetCaption( labTitle, "Item Mall")
	UI_SetTextColor( labTitle, COLOR_BLACK )
	UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

--Notice
		labNotice = UI_CreateCompent( frmStore, LABELEX_TYPE, "labNotice", 10, 150, 48 ,29  )
		UI_SetCaption( labNotice, "Welcome to the InGame Item Mall: We have many items that will help you in your journeys!")
		UI_SetTextColor( labNotice, COLOR_RED )
		UI_SetLabelExFont( labNotice, DEFAULT_FONT, FALSE, COLOR_WHITE )

----begin
	--
	trvStore = UI_CreateCompent( frmStore, TREE_TYPE, "trvStore", 142, 370, 6, 56)
	
	UI_TreeLoadImage( trvStore, enumTreeNodeImage, "texture/ui/store6.tga", 127, 20, 30, 114, 127, 20)
	
	UI_TreeLoadImage( trvStore, enumTreeAddImage, "texture/ui/store6.tga", 30, 20, 0, 114, 30, 20)
	UI_TreeLoadImage( trvStore, enumTreeSubImage, "texture/ui/store6.tga", 30, 20, 0, 134, 30, 20)
	UI_SetIsDrag( trvStore, FALSE )

		--
	btnReceiveMoney = UI_CreateCompent( frmStore, BUTTON_TYPE, "btnReceiveMoney", 64, 19, 293, 67 )
	UI_LoadButtonImage( btnReceiveMoney, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

	labReceiveMoney = UI_CreateCompent( frmStore, LABELEX_TYPE, "labReceiveMoney", 400, 150, 301, 71 )
	UI_SetCaption( labReceiveMoney, "Redeem Crystal")
	UI_SetTextColor( labReceiveMoney, COLOR_BLACK )
	UI_SetLabelExFont( labReceiveMoney, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	--
	btnReceiveMoDou= UI_CreateCompent( frmStore, BUTTON_TYPE, "btnReceiveMoDou", 64, 19, 368, 67 )
	UI_LoadButtonImage( btnReceiveMoDou, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

	labReceiveMoDou= UI_CreateCompent( frmStore, LABELEX_TYPE, "labReceiveMoDou", 400, 150, 376, 71 )
	UI_SetCaption( labReceiveMoDou, "Top Up Point")
	UI_SetTextColor( labReceiveMoDou, COLOR_BLACK )
	UI_SetLabelExFont( labReceiveMoDou, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	labNotice = UI_CreateCompent( frmStore, LABELEX_TYPE, "labNotice", 10, 150, 190 ,100  )
	UI_SetCaption( labNotice, "Crystal Replenishment")
	UI_SetTextColor( labNotice, COLOR_RED )
	UI_SetLabelExFont( labNotice, DEFAULT_FONT, FALSE, COLOR_WHITE )
	
	labNotice = UI_CreateCompent( frmStore, LABELEX_TYPE, "labNotice", 10, 150, 190 ,120  )
	UI_SetCaption( labNotice, "Purchase of items here requires Crystal.You can exchange crystal")
	UI_SetTextColor( labNotice, COLOR_BLACK )
	UI_SetLabelExFont( labNotice, DEFAULT_FONT, FALSE, COLOR_WHITE )
	
	labNotice = UI_CreateCompent( frmStore, LABELEX_TYPE, "labNotice", 10, 150, 190 ,140  )
	UI_SetCaption( labNotice, "with item mall points. Exchange ratio 1:1.")
	UI_SetTextColor( labNotice, COLOR_BLACK )
	UI_SetLabelExFont( labNotice, DEFAULT_FONT, FALSE, COLOR_WHITE )
	--

	--
	imgBackGround10 = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBackGround10", 405, 440, 182, 60 )
	UI_LoadImage( imgBackGround10, "texture/ui/store1.tga", NORMAL, 20,20, 30, 70 )

	--memo
	memStoreHelp  = UI_CreateCompent( frmStore, MEMO_TYPE, "memStoreHelp", 397, 430, 190, 67 )
	UI_SetCaption( memStoreHelp, "400" )
	UI_SetTextColor( memStoreHelp, COLOR_BLACK )
	UI_SetLabelExFont( memStoreHelp, DEFAULT_FONT, TRUE, COLOR_WHITE )
	UI_SetMemoPageShowNum( memStoreHelp, 21 )
	UI_SetMemoMaxNumPerRow( memStoreHelp, 62 )

	--
	scrollid = UI_GetScroll( memStoreHelp )
	UI_SetSize( scrollid, 11, 1 )
	UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )
	
	--
	id = UI_GetScrollObj( scrollid, SCROLL_UP )
	UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
	UI_SetSize( id, 11, 9 )
	
	--
	id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
	UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 33, 166, 10 )
	UI_SetSize( id, 11, 33 )
	
	--
	id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
	UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
	UI_SetSize( id, 11, 9 )
	
	--
	labNameTitle = UI_CreateCompent( frmStore, LABELEX_TYPE, "labNameTitle0", 10, 150, 235, 64 )
	UI_SetCaption( labNameTitle, "Item")
	UI_SetTextColor( labNameTitle, COLOR_BLACK )
	UI_SetLabelExFont( labNameTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

	labPriceTitle = UI_CreateCompent( frmStore, LABELEX_TYPE, "labPriceTitle", 10, 150, 380, 64)
	UI_SetCaption( labPriceTitle, "Price")
	UI_SetTextColor( labPriceTitle, COLOR_BLACK )
	UI_SetLabelExFont( labPriceTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	labLeftTimeTitle = UI_CreateCompent( frmStore, LABELEX_TYPE, "labLeftTimeTitle", 10, 150, 440, 64 )
	UI_SetCaption( labLeftTimeTitle, "Time Left")
	UI_SetTextColor( labLeftTimeTitle, COLOR_BLACK )
	UI_SetLabelExFont( labLeftTimeTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

	labLeftNumTitle = UI_CreateCompent( frmStore, LABELEX_TYPE, "labLeftNumTitle", 10, 150, 510, 64 )
	UI_SetCaption( labLeftNumTitle, "Amount Left")
	UI_SetTextColor( labLeftNumTitle, COLOR_BLACK )
	UI_SetLabelExFont( labLeftNumTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )		
	
	
	--
	local labName = {}
	local labPrice = {}
	local labRemark = {}
	local labLeftTime = {}
	local labLeftNum = {}
	local labRightClickView = {}
	
	local btnBlue = {}
	local imgBlue = {}
	
	--
	local imgNew = {}
	local imgHot = {}
	
	--9icon frame
	local imgSquare = {}
	--9icon command
	local cmdStore = {}

	local imgCutLine = {}

	
	for i=0,8,1 do
		--
		imgCutLine[i] = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgCutLine_"..i, 355, 2, 230, i*42+78 )
		UI_LoadImage( imgCutLine[i], "texture/ui/store6.tga",NORMAL, 253 ,2, 0, 51 )
		--UI_SetAlpha(imgCutLine[i],150)
		
		--icon
		imgSquare[i] =  UI_CreateCompent( frmStore, IMAGE_TYPE, "imgSquare_"..i, 36, 36, 190, i*42+82 )
		UI_LoadImage( imgSquare[i], "texture/ui/store6.tga",NORMAL, 36 ,36, 157, 76 )
		
		--icon
		cmdStore[i] = UI_CreateCompent( frmStore, COMMAND_ONE_TYPE, "cmdStore_"..i, 32, 32, 192, i*42+84 )	
		
		--
		btnBlue[i] = UI_CreateCompent( frmStore, BUTTON_TYPE, "btnBlue_"..i, 410, 40, 180, i*42+80 )
		--UI_LoadButtonImage( btnBlue[i], "texture/ui/store7.tga", 256 ,51, 0, 0, FALSE )
		UI_SetAlpha(btnBlue[i],0)
		
		--  
		imgBlue[i] = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgBlue_"..i, 355, 40, 230, i*42+80 )
		UI_LoadImage( imgBlue[i], "texture/ui/store7.tga",NORMAL, 253 ,48, 0, 51 )
		UI_SetAlpha(imgBlue[i],200)
		UI_SetIsShow(imgBlue[i],FALSE)
		
		labName[i] = UI_CreateCompent( frmStore, LABELEX_TYPE, "labName_"..i, 10, 150, 235, i*42+84 )
		UI_SetCaption( labName[i], "Item"..i)
		UI_SetTextColor( labName[i], COLOR_BLUE )
		UI_SetLabelExFont( labName[i], DEFAULT_FONT, TRUE, COLOR_WHITE )
		UI_SetIsShow(labName[i],FALSE)

		labPrice[i] = UI_CreateCompent( frmStore, LABELEX_TYPE, "labPrice_"..i, 10, 150, 381, i*42+84)
		UI_SetCaption( labPrice[i], "Price"..i)
		UI_SetTextColor( labPrice[i], COLOR_RED)
		UI_SetLabelExFont( labPrice[i], DEFAULT_FONT, TRUE, COLOR_WHITE )
		UI_SetIsShow(labPrice[i],FALSE)

		--  : 
		labLeftTime[i] = UI_CreateCompent( frmStore, LABELEX_TYPE, "labLeftTime_"..i, 10, 150, 440, i*42+84 )
		UI_SetCaption( labLeftTime[i], "Remaining time"..i)
		UI_SetTextColor( labLeftTime[i], COLOR_PINK )
		UI_SetLabelExFont( labLeftTime[i], DEFAULT_FONT, TRUE, COLOR_WHITE )
		UI_SetIsShow(labLeftTime[i],FALSE)
		
		--  : 
		labLeftNum[i] = UI_CreateCompent( frmStore, LABELEX_TYPE, "labLeftNum_"..i, 10, 150, 510, i*42+84 )
		UI_SetCaption( labLeftNum[i], "Remaining amount"..i)
		UI_SetTextColor( labLeftNum[i], COLOR_PINK )
		UI_SetLabelExFont( labLeftNum[i], DEFAULT_FONT, TRUE, COLOR_WHITE )		
		UI_SetIsShow(labLeftNum[i],FALSE)
		
		labRemark[i] = UI_CreateCompent( frmStore, LABELEX_TYPE, "labRemark_"..i, 10, 150, 235, i*42+98 )
		UI_SetCaption( labRemark[i], "Details"..i)
		UI_SetTextColor( labRemark[i], COLOR_BLACK )
		UI_SetLabelExFont( labRemark[i], DEFAULT_FONT, TRUE, COLOR_WHITE )
		
		labRightClickView[i] = UI_CreateCompent( frmStore, LABELEX_TYPE, "labRightClickView_"..i, 10, 150, 440, i*42+98 )
		UI_SetCaption( labRightClickView[i], "Right click for details")
		UI_SetTextColor( labRightClickView[i], COLOR_BLACK)
		UI_SetLabelExFont( labRightClickView[i], DEFAULT_FONT, TRUE, COLOR_WHITE )
		
		--
		imgNew[i] = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgNew_"..i, 29, 11, 196, i*42+87 )
		UI_LoadImage( imgNew[i], "texture/ui/store6.tga", NORMAL, 29 ,11, 0, 174 )
		UI_SetIsShow(imgNew[i],FALSE)
		--
		imgHot[i] = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgHot_"..i, 22, 15, 196, i*42+87 )
		UI_LoadImage( imgHot[i], "texture/ui/store6.tga", NORMAL, 22 ,15, 0, 186 )
		UI_SetIsShow(imgHot[i],FALSE)
	end
		
	--
	labMoneyNotice = UI_CreateCompent( frmStore, LABELEX_TYPE, "labMoneyNotice", 400, 150,24, 445 )
	UI_SetCaption( labMoneyNotice, "Crystal")
	UI_SetTextColor( labMoneyNotice, COLOR_BLACK )
	UI_SetLabelExFont( labMoneyNotice, DEFAULT_FONT, TRUE, COLOR_WHITE )

	imgMoney = UI_CreateCompent( frmStore, IMAGE_TYPE, "imgMoney", 18, 24, 76, 435 )
	UI_LoadImage( imgMoney, "texture/ui/store6.tga", NORMAL, 24,31, 0, 200 )

	labMoneyLeft = UI_CreateCompent( frmStore, LABELEX_TYPE, "labMoneyLeft", 400, 150,110 , 445 )
	UI_SetCaption( labMoneyLeft, "99999")
	UI_SetTextColor( labMoneyLeft, COLOR_BLACK )
	UI_SetLabelExFont( labMoneyLeft, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	--
	labMoneyNotice = UI_CreateCompent( frmStore, LABELEX_TYPE, "labMoneyNotice", 400, 150,24, 465 )
	UI_SetCaption( labMoneyNotice, "Points")
	UI_SetTextColor( labMoneyNotice, COLOR_BLACK )
	UI_SetLabelExFont( labMoneyNotice, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	labBeanLeft = UI_CreateCompent( frmStore, LABELEX_TYPE, "labBeanLeft", 400, 150, 76, 465 )
	UI_SetCaption( labBeanLeft, "0")
	UI_SetTextColor( labBeanLeft, COLOR_BLACK )
	UI_SetLabelExFont( labBeanLeft, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
		--
	labMoneyNotice = UI_CreateCompent( frmStore, LABELEX_TYPE, "labMoneyNotice", 400, 150,24, 485 )
	UI_SetCaption( labMoneyNotice, "Type")
	UI_SetTextColor( labMoneyNotice, COLOR_BLACK )
	UI_SetLabelExFont( labMoneyNotice, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	labMemberStyle = UI_CreateCompent( frmStore, LABELEX_TYPE, "labMemberStyle", 400, 150, 76, 485 )
	UI_SetCaption( labMemberStyle, "Ordinary Member")
	UI_SetTextColor( labMemberStyle, COLOR_BLACK )
	UI_SetLabelExFont( labMemberStyle, DEFAULT_FONT, TRUE, COLOR_WHITE )

	
	--
	btnLeftPage = UI_CreateCompent( frmStore, BUTTON_TYPE, "btnLeftPage", 13, 13, 505, 485 )
	UI_LoadButtonImage( btnLeftPage, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE )
	UI_SetIsShow(btnLeftPage,FALSE)

	--
	labListPage = UI_CreateCompent( frmStore, LABELEX_TYPE, "labListPage", 400, 150, 525, 485 )
	UI_SetCaption( labListPage, "")
	UI_SetTextColor( labListPage, COLOR_BLACK )
	UI_SetLabelExFont( labListPage, DEFAULT_FONT, TRUE, COLOR_WHITE )
	UI_SetIsShow(labListPage,FALSE)

	--
	btnRightPage = UI_CreateCompent( frmStore, BUTTON_TYPE, "btnRightPage", 13, 13, 555, 485 )
	UI_LoadButtonImage( btnRightPage, "texture/ui/SystemBotton3.tga", 13, 13, 0, 211, TRUE )
	UI_SetIsShow(btnRightPage,FALSE)

	--
	btnTrade = UI_CreateCompent( frmStore, BUTTON_TYPE, "btnTrade", 48, 19, 200, 480 )
	UI_LoadButtonImage( btnTrade, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

	labTrade = UI_CreateCompent( frmStore, LABELEX_TYPE, "labTrade", 400, 150, 216, 484 )
	UI_SetCaption( labTrade, "Buy")
	UI_SetTextColor( labTrade, COLOR_BLACK )
	UI_SetLabelExFont( labTrade, DEFAULT_FONT, TRUE, COLOR_WHITE )

	--
	btnTryon = UI_CreateCompent( frmStore, BUTTON_TYPE, "btnTryon", 48, 19, 260, 480 )
	UI_LoadButtonImage( btnTryon, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

	labTryon = UI_CreateCompent( frmStore, LABELEX_TYPE, "labTryon", 400, 150, 276, 484 )
	UI_SetCaption( labTryon, "Try")
	UI_SetTextColor( labTryon, COLOR_BLACK )
	UI_SetLabelExFont( labTryon, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	--
	btnViewAll = UI_CreateCompent( frmStore, BUTTON_TYPE, "btnViewAll", 68, 19, 320, 480 )
	UI_LoadButtonImage( btnViewAll, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

	labViewAll = UI_CreateCompent( frmStore, LABELEX_TYPE, "labViewAll", 400, 150, 340, 484 )
	UI_SetCaption( labViewAll, "Check")
	UI_SetTextColor( labViewAll, COLOR_BLACK )
	UI_SetLabelExFont( labViewAll, DEFAULT_FONT, TRUE, COLOR_WHITE )

	--II
	btnCloseall = UI_CreateCompent( frmStore, BUTTON_TYPE, "btnCloseall", 68, 19, 400, 480 )
	UI_LoadButtonImage( btnCloseall, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )
	UI_SetButtonModalResult( btnCloseall, BUTTON_CLOSE )
	
	labCloseAll = UI_CreateCompent( frmStore, LABELEX_TYPE, "labCloseAll", 400, 150, 405, 484 )
	UI_SetCaption( labCloseAll, "Close Mall")
	UI_SetTextColor( labCloseAll, COLOR_BLACK )
	UI_SetLabelExFont( labCloseAll, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	--
	btnClose = UI_CreateCompent( frmStore, BUTTON_TYPE, "btnClose", 14, 14, 570, 3 )
	UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
	UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

---------------------------
--  IGS item description --
---------------------------
frmViewAll = UI_CreateForm( "frmViewAll", FALSE, 250, 184, 100, 100, TRUE, FALSE )
UI_ShowForm( frmViewAll, FALSE )
UI_AddFormToTemplete( frmViewAll, FORM_MAIN )
UI_FormSetIsEscClose( frmViewAll, FALSE )
UI_SetFormStyle(frmViewAll,1)

--UI_FormSetHotKey( frmViewAll, ALT_KEY, HOTKEY_Z )

imgBackGround1 = UI_CreateCompent( frmViewAll, IMAGE_TYPE, "imgBackGround1", 250, 85, 0, 0 )
UI_LoadImage( imgBackGround1, "texture/ui/player1.tga", NORMAL, 171,85, 0, 0 )

imgBackGround2 = UI_CreateCompent( frmViewAll, IMAGE_TYPE, "imgBackGround2", 250, 29, 0, 155 )
UI_LoadImage( imgBackGround2, "texture/ui/player2.tga", NORMAL, 171,29, 0, 23)

imgBackGround3 = UI_CreateCompent( frmViewAll, IMAGE_TYPE, "imgBackGround3", 250, 70, 0, 85 )
UI_LoadImage( imgBackGround3, "texture/ui/player1.tga", NORMAL, 171,2, 0, 85)

--title
labTitle = UI_CreateCompent(  frmViewAll, LABELEX_TYPE, "labTitle", 49, 14, 7, 3 )
UI_SetCaption( labTitle, "Investigate")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

--icon img
	local imgSquareViewAll = {}
	local cmdSquareIcon = {}
	
	for i=0 , 5 , 1 do
		imgSquareViewAll[i] = UI_CreateCompent( frmViewAll, IMAGE_TYPE, "imgSquareViewAll_"..i, 36, 36, i*40+7, 30 )
		UI_LoadImage( imgSquareViewAll[i], "texture/ui/store6.tga",NORMAL, 36 ,36, 157, 76 )

		cmdSquareIcon[i] = UI_CreateCompent( frmViewAll, COMMAND_ONE_TYPE, "cmdSquareIcon_"..i, 32, 32, i*40+7, 31 )	
	end

	memViewAll  = UI_CreateCompent( frmViewAll, MEMO_TYPE, "memViewAll", 220, 100, 12, 70)
	UI_SetCaption( memViewAll, "400" )
	UI_SetTextColor( memViewAll, COLOR_BLACK )
	UI_SetLabelExFont( memViewAll, DEFAULT_FONT, TRUE, COLOR_WHITE )
	UI_SetMemoPageShowNum( memViewAll, 4 )
	UI_SetMemoMaxNumPerRow( memViewAll, 38 )
	
	btnReBack = UI_CreateCompent( frmViewAll, BUTTON_TYPE, "btnReBack", 48, 19,185, 151 )
	UI_LoadButtonImage( btnReBack, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )
	UI_SetButtonModalResult( btnReBack, BUTTON_CLOSE )
	
	labReBack = UI_CreateCompent( frmViewAll, LABELEX_TYPE, "labReBack", 400, 15, 192, 155 )
	UI_SetCaption( labReBack, "Return")
	UI_SetTextColor( labReBack, COLOR_BLACK )
	UI_SetLabelExFont( labReBack, DEFAULT_FONT, TRUE, COLOR_WHITE )


	--
	btnClose = UI_CreateCompent( frmViewAll, BUTTON_TYPE, "btnClose", 14, 14, 230, 3 )
	UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
	UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

----------------
-- IGS TRY ON --
----------------
frmTryon = UI_CreateForm( "frmTryon", FALSE, 200, 318, 0, 0, TRUE, FALSE )
UI_ShowForm( frmTryon, FALSE )
UI_AddFormToTemplete( frmTryon, FORM_MAIN )
UI_FormSetIsEscClose( frmTryon, FALSE )
UI_SetIsDrag( frmTryon,FALSE)

--UI_FormSetHotKey( frmTryon, ALT_KEY, HOTKEY_Z )

imgBackGround1 = UI_CreateCompent( frmTryon, IMAGE_TYPE, "imgBackGround1", 200, 265, 0, 0 )
UI_LoadImage( imgBackGround1, "texture/ui/player1.tga", NORMAL, 171,256, 0, 0 )

imgBackGround2 = UI_CreateCompent( frmTryon, IMAGE_TYPE, "imgBackGround2", 200, 53, 0, 265 )
UI_LoadImage( imgBackGround2, "texture/ui/player2.tga", NORMAL, 171,51, 0, 0 )

labTitle = UI_CreateCompent( frmTryon, LABELEX_TYPE, "labTitle", 10, 150, 7 ,3  )
UI_SetCaption( labTitle, "Try")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

ui3dplayer = UI_CreateCompent( frmTryon, UI3D_COMPENT, "ui3dplayer", 120, 240, 40, 30 )

btnTurnleft3d = UI_CreateCompent( frmTryon, BUTTON_TYPE, "btnTurnleft3d", 32, 38, 34, 264 )
UI_LoadButtonImage( btnTurnleft3d, "texture/ui/select4.tga", 32, 38, 0, 38, TRUE )

btnTurnright3d = UI_CreateCompent( frmTryon, BUTTON_TYPE, "btnTurnright3d", 32, 38, 134, 264 )
UI_LoadButtonImage( btnTurnright3d, "texture/ui/select4.tga",  32, 38, 0, 0, TRUE )

	btnClearAll = UI_CreateCompent( frmTryon, BUTTON_TYPE, "btnClearAll", 68, 19, 66, 30)
	UI_LoadButtonImage( btnClearAll, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

	labClearAll = UI_CreateCompent( frmTryon, LABELEX_TYPE, "labClearAll", 400, 150, 85, 34 )
	UI_SetCaption( labClearAll, "Clear")
	UI_SetTextColor( labClearAll, COLOR_BLACK )
	UI_SetLabelExFont( labClearAll, DEFAULT_FONT, TRUE, COLOR_WHITE )



-----------------------------------------------------------------------
-- Bag of Holding
-----------------------------------------------------------------------
frmBagOfHolding = UI_CreateForm( "frmBagOfHolding", FALSE, 200, 232, 0, 0, TRUE, FALSE )
UI_ShowForm( frmBagOfHolding, FALSE )
UI_AddFormToTemplete( frmBagOfHolding, FORM_MAIN )
UI_FormSetIsEscClose( frmBagOfHolding, TRUE )
UI_SetIsDrag( frmBagOfHolding,FALSE)
UI_SetFormStyle( frmBagOfHolding,1)


imgBakTop = UI_CreateCompent( frmBagOfHolding, IMAGE_TYPE, "imgBakTop", 200, 232, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/corsairs/tempbag.png", NORMAL, 198, 232, 0, 0 )

labTitle = UI_CreateCompent( frmBagOfHolding, LABELEX_TYPE, "labTitle", 400, 150, 10, 5 )
UI_SetCaption( labTitle, "Bag of Holding")
UI_SetTextColor( labTitle, COLOR_BLACK )

UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )
btnClose = UI_CreateCompent( frmBagOfHolding, BUTTON_TYPE, "btnClose", 14, 14, 178, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

grdBagOfHolding = UI_CreateCompent( frmBagOfHolding, GOODS_GRID_TYPE, "grdBagOfHolding", 150, 150, 31, 41 ) 
UI_SetGridSpace( grdBagOfHolding, 3, 3 )
UI_SetGridContent( grdBagOfHolding, 4, 4 )
UI_SetGridUnitSize( grdBagOfHolding, 32, 32 )
UI_SetMargin( grdBagOfHolding, 0,0, 0, 0 )

btnRename = UI_CreateCompent( frmBagOfHolding, BUTTON_TYPE, "btnRename", 14, 14, 28, 191)
UI_LoadButtonImage( btnRename, "texture/ui/corsairs/coButtons.png", 14, 14, 217, 51, TRUE )
UI_SetHint(btnRename,"Rename")

btnRemoveAll = UI_CreateCompent( frmBagOfHolding, BUTTON_TYPE, "btnRemoveAll", 14, 14, 49, 191)
UI_LoadButtonImage( btnRemoveAll, "texture/ui/corsairs/coButtons.png", 14, 14, 217, 67, TRUE )
UI_SetHint(btnRemoveAll,"Remove All Items")



-------------------------------------
-- 2006.8.29 by Leo Fairy Marriage --
-------------------------------------
frmSpiritMarry = UI_CreateForm( "frmSpiritMarry", FALSE, 226, 426, 0, 0, TRUE, FALSE )
UI_ShowForm( frmSpiritMarry, FALSE )
UI_AddFormToTemplete( frmSpiritMarry, FORM_MAIN )
UI_FormSetIsEscClose( frmSpiritMarry, TRUE )
UI_SetIsDrag( frmSpiritMarry,TRUE)
UI_SetFormStyle( frmSpiritMarry,1)

--UI_FormSetHotKey( frmSpiritMarry, ALT_KEY, HOTKEY_Z )
imgBakTop = UI_CreateCompent( frmSpiritMarry, IMAGE_TYPE, "imgBakTop", 226, 256, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/marry1.tga", NORMAL, 226, 256, 0, 0 )

imgBakButtom = UI_CreateCompent( frmSpiritMarry, IMAGE_TYPE, "imgBakButtom", 226, 170, 0, 256 )
UI_LoadImage( imgBakButtom, "texture/ui/marry2.tga", NORMAL, 226, 170, 0, 0 )

labTitle = UI_CreateCompent( frmSpiritMarry, LABELEX_TYPE, "labTitle", 49, 14, 7, 3 )
UI_SetCaption( labTitle, "Fairy Marriage")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

cmdSpiritOne = UI_CreateCompent( frmSpiritMarry, COMMAND_ONE_TYPE, "cmdSpiritOne", 32, 32, 52, 196 )	-- 
cmdSpiritTwo = UI_CreateCompent( frmSpiritMarry, COMMAND_ONE_TYPE, "cmdSpiritTwo", 32, 32, 138, 196 )	-- 
cmdSpiritItem = UI_CreateCompent( frmSpiritMarry, COMMAND_ONE_TYPE, "cmdSpiritItem", 32, 32, 96, 264 )	-- 

UI_SetTag( cmdSpiritOne, enumSPIRIT_ONE )
UI_SetTag( cmdSpiritTwo, enumSPIRIT_TWO )
UI_SetTag( cmdSpiritItem, enumSPIRIT_OUT )

labNotice = UI_CreateCompent( frmSpiritMarry, LABELEX_TYPE, "labNotice", 112, 14, 18, 384 )
UI_SetCaption( labNotice, "Demonic Fruit has bonus effect!")
UI_SetTextColor( labNotice, COLOR_RED )
UI_SetLabelExFont( labNotice, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMoneyShow = UI_CreateCompent( frmSpiritMarry, LABELEX_TYPE, "labMoneyShow", 112, 14, 68, 334 )
UI_SetCaption( labMoneyShow, "$$$")
UI_SetTextColor( labMoneyShow, COLOR_BLACK )
UI_SetLabelExFont( labMoneyShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmSpiritMarry, BUTTON_TYPE, "btnClose", 14, 14, 206, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

btnForgeYes = UI_CreateCompent( frmSpiritMarry, BUTTON_TYPE, "btnForgeYes", 50, 44, 22, 275 )
UI_LoadButtonImage( btnForgeYes, "texture/ui/marryyesno.tga", 60, 53, 0, 0, FALSE )

btnForgeNo = UI_CreateCompent( frmSpiritMarry, BUTTON_TYPE, "btnForgeNo", 50, 44, 150, 275)
UI_LoadButtonImage( btnForgeNo, "texture/ui/marryyesno.tga", 60, 53, 60, 0, FALSE )
UI_SetButtonModalResult( btnForgeNo, BUTTON_CLOSE )

---------------------------------------------------------
-- 2006.8.29 by Leo gem extraction / item purification --
---------------------------------------------------------
frmEquipPurify = UI_CreateForm( "frmEquipPurify", FALSE, 226, 255, 0, 0, TRUE, FALSE )
UI_ShowForm( frmEquipPurify, FALSE )
UI_AddFormToTemplete( frmEquipPurify, FORM_MAIN )
UI_FormSetIsEscClose( frmEquipPurify, TRUE )
UI_SetIsDrag( frmEquipPurify,TRUE)
UI_SetFormStyle( frmEquipPurify,1)

--UI_FormSetHotKey( frmEquipPurify, ALT_KEY, HOTKEY_X )

imgBakTop = UI_CreateCompent( frmEquipPurify, IMAGE_TYPE, "imgBakTop", 226, 255, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/EquipPurify.tga", NORMAL, 226, 255, 0, 0 )

--title
labTitle1 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labTitle1", 49, 14, 7, 3 )
UI_SetCaption( labTitle1, "Purification")
UI_SetTextColor( labTitle1, COLOR_BLACK )
UI_SetLabelExFont( labTitle1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTitle2 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labTitle2", 49, 14, 7, 3 )
UI_SetCaption( labTitle2, "Coral recharge")
UI_SetTextColor( labTitle2, COLOR_BLACK )
UI_SetLabelExFont( labTitle2, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTitle3 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labTitle3", 49, 14, 7, 3 )
UI_SetCaption( labTitle3, "Gem purification")
UI_SetTextColor( labTitle3, COLOR_BLACK )
UI_SetLabelExFont( labTitle3, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTitle4 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labTitle4", 49, 14, 7, 3 )
UI_SetCaption( labTitle4, "Repair Lifeskill tools")
UI_SetTextColor( labTitle4, COLOR_BLACK )
UI_SetLabelExFont( labTitle4, DEFAULT_FONT, TRUE, COLOR_WHITE )

cmdEquipOne = UI_CreateCompent( frmEquipPurify, COMMAND_ONE_TYPE, "cmdEquipOne", 32, 32, 46, 65 )	-- 

labHintleft1 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labHintleft1", 112, 14, 40, 110 )
UI_SetCaption( labHintleft1, "Main")
UI_SetTextColor( labHintleft1, COLOR_BLACK )
UI_SetLabelExFont( labHintleft1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labHintleft2 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labHintleft2", 112, 14,40, 110 )
UI_SetCaption( labHintleft2, "Conch")
UI_SetTextColor( labHintleft2, COLOR_BLACK )
UI_SetLabelExFont( labHintleft2, DEFAULT_FONT, TRUE, COLOR_WHITE )

labHintleft3 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labHintleft3", 112, 14, 40, 110 )
UI_SetCaption( labHintleft3, "Equipment")
UI_SetTextColor( labHintleft3, COLOR_BLACK )
UI_SetLabelExFont( labHintleft3, DEFAULT_FONT, TRUE, COLOR_WHITE )

labHintleft4 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labHintleft4", 112, 14, 40, 110 )
UI_SetCaption( labHintleft4, "Lifeskill tool")
UI_SetTextColor( labHintleft4, COLOR_BLACK )
UI_SetLabelExFont( labHintleft4, DEFAULT_FONT, TRUE, COLOR_WHITE )

cmdEquipTwo = UI_CreateCompent( frmEquipPurify, COMMAND_ONE_TYPE, "cmdEquipTwo", 32, 32, 146, 65 )	-- 

--lab
labHintright1 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labHintright1", 112, 14, 144, 110 )
UI_SetCaption( labHintright1, "Catalyst")
UI_SetTextColor( labHintright1, COLOR_BLACK )
UI_SetLabelExFont( labHintright1, DEFAULT_FONT, TRUE, COLOR_WHITE )

--lab
labHintright2 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labHintright2", 112, 14, 144, 110 )
UI_SetCaption( labHintright2, "Battery")
UI_SetTextColor( labHintright2, COLOR_BLACK )
UI_SetLabelExFont( labHintright2, DEFAULT_FONT, TRUE, COLOR_WHITE )

labHintright3 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labHintright3", 112, 14,144, 110 )
UI_SetCaption( labHintright3, "Pliers")
UI_SetTextColor( labHintright3, COLOR_BLACK )
UI_SetLabelExFont( labHintright3, DEFAULT_FONT, TRUE, COLOR_WHITE )

labHintright4 = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labHintright4", 112, 14,144, 110 )
UI_SetCaption( labHintright4, "Repair Tool")
UI_SetTextColor( labHintright4, COLOR_BLACK )
UI_SetLabelExFont( labHintright4, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMoneyShow = UI_CreateCompent( frmEquipPurify, LABELEX_TYPE, "labMoneyShow", 112, 14, 64, 148 )
UI_SetCaption( labMoneyShow, "")
UI_SetTextColor( labMoneyShow, COLOR_BLACK )
UI_SetLabelExFont( labMoneyShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmEquipPurify, BUTTON_TYPE, "btnClose", 14, 14, 206, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

btnForgeNo = UI_CreateCompent( frmEquipPurify, BUTTON_TYPE, "btnForgeNo", 41, 19, 135, 220 )
UI_LoadButtonImage( btnForgeNo, "texture/ui/PublicC.tga", 41, 19, 0, 20 ,TRUE )
UI_SetButtonModalResult( btnForgeNo, BUTTON_CLOSE )

btnForgeYes = UI_CreateCompent( frmEquipPurify, BUTTON_TYPE, "btnForgeYes", 41, 19, 52, 220 )
UI_LoadButtonImage( btnForgeYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

-----------------------------------------------------------------------
--   2006.8.29 by Leo
-----------------------------------------------------------------------
frmSpiritErnie = UI_CreateForm( "frmSpiritErnie", FALSE, 423, 357, 0, 0, TRUE, FALSE )
UI_AddFormToTemplete( frmSpiritErnie, FORM_MAIN )
UI_FormSetIsEscClose( frmSpiritErnie, TRUE )
UI_SetIsDrag( frmSpiritErnie,TRUE)
UI_SetFormStyle( frmSpiritErnie,1)

--UI_FormSetHotKey( frmSpiritErnie, ALT_KEY, HOTKEY_V )

--
imgBakTopLeft = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgBakTopLeft", 256, 256, 0, 0 )
UI_LoadImage( imgBakTopLeft, "texture/ui/SpiritErnie1.tga", NORMAL, 256, 256, 0, 0 )

imgBakTopRight = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgBakTopRight", 167, 256, 256, 0 )
UI_LoadImage( imgBakTopRight, "texture/ui/SpiritErnie2.tga", NORMAL, 167, 256, 0, 0 )

imgBakBottomLeft = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgBakBottomLeft", 256, 101, 0, 256 )
UI_LoadImage( imgBakBottomLeft, "texture/ui/SpiritErnie3.tga", NORMAL, 256, 101, 0, 0 )

imgBakBottomRight = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgBakBottomRight", 167, 101, 256, 256 )
UI_LoadImage( imgBakBottomRight, "texture/ui/SpiritErnie3.tga", NORMAL, 167, 101, 0, 101 )

--
	--
		--
		imgLine1_1 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine1_1", 32, 32, 180, 86 )
		UI_LoadImage( imgLine1_1, "texture/ui/tiger.tga", NORMAL, 32, 32, 0, 0 )

		imgLine1_2 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine1_2", 32, 32, 180, 86 )
		UI_LoadImage( imgLine1_2, "texture/ui/tiger.tga", NORMAL, 32, 32, 32, 0 )

		imgLine1_3 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine1_3", 32, 32, 180, 86 )
		UI_LoadImage( imgLine1_3, "texture/ui/tiger.tga", NORMAL, 32, 32, 64, 0 )

		imgLine1_4 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine1_4", 32, 32, 180, 86 )
		UI_LoadImage( imgLine1_4, "texture/ui/tiger.tga", NORMAL, 32, 32, 96, 0 )

		imgLine1_5 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine1_5", 32, 32, 180, 86 )
		UI_LoadImage( imgLine1_5, "texture/ui/tiger.tga", NORMAL, 32, 32, 128, 0 )
		--
		imgLine2_1 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine2_1", 32, 32, 180, 143 )
		UI_LoadImage( imgLine2_1, "texture/ui/tiger.tga", NORMAL, 32, 32, 0, 32 )

		imgLine2_2 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine2_2", 32, 32, 180, 143 )
		UI_LoadImage( imgLine2_2, "texture/ui/tiger.tga", NORMAL, 32, 32, 32, 32 )

		imgLine2_3 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine2_3", 32, 32, 180, 143 )
		UI_LoadImage( imgLine2_3, "texture/ui/tiger.tga", NORMAL, 32, 32, 64, 32 )

		imgLine2_4 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine2_4", 32, 32, 180, 143 )
		UI_LoadImage( imgLine2_4, "texture/ui/tiger.tga", NORMAL, 32, 32, 96, 32 )

		imgLine2_5 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine2_5", 32, 32, 180, 143 )
		UI_LoadImage( imgLine2_5, "texture/ui/tiger.tga", NORMAL, 32, 32, 128, 32 )
		--
		imgLine3_1 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine3_1", 32, 32, 180, 199 )
		UI_LoadImage( imgLine3_1, "texture/ui/tiger.tga", NORMAL, 32, 32, 0, 64 )

		imgLine3_2 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine3_2", 32, 32, 180, 199 )
		UI_LoadImage( imgLine3_2, "texture/ui/tiger.tga", NORMAL, 32, 32, 32, 64 )

		imgLine3_3 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine3_3", 32, 32, 180, 199 )
		UI_LoadImage( imgLine3_3, "texture/ui/tiger.tga", NORMAL, 32, 32, 64, 64 )

		imgLine3_4 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine3_4", 32, 32, 180, 199 )
		UI_LoadImage( imgLine3_4, "texture/ui/tiger.tga", NORMAL, 32, 32, 96, 64 )

		imgLine3_5 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine3_5", 32, 32, 180, 199 )
		UI_LoadImage( imgLine3_5, "texture/ui/tiger.tga", NORMAL, 32, 32, 128, 64 )
	-------------------------------------------------------------------------------------------------
	--
		--
		imgLine4_1 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine4_1", 32, 32, 239,86 )
		UI_LoadImage( imgLine4_1, "texture/ui/tiger.tga", NORMAL, 32, 32, 0, 96 )

		imgLine4_2 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine4_2", 32, 32, 239,86 )
		UI_LoadImage( imgLine4_2, "texture/ui/tiger.tga", NORMAL, 32, 32, 32, 96 )

		imgLine4_3 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine4_3", 32, 32, 239,86 )
		UI_LoadImage( imgLine4_3,"texture/ui/tiger.tga", NORMAL, 32, 32, 64, 96 )

		imgLine4_4 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine4_4", 32, 32, 239,86 )
		UI_LoadImage( imgLine4_4, "texture/ui/tiger.tga", NORMAL, 32, 32, 96, 96 )

		imgLine4_5 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine4_5", 32, 32, 239,86 )
		UI_LoadImage( imgLine4_5, "texture/ui/tiger.tga", NORMAL, 32, 32, 128, 96 )
		--
		imgLine5_1 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine5_1", 32, 32, 239,143 )
		UI_LoadImage( imgLine5_1, "texture/ui/tiger.tga", NORMAL, 32, 32, 0, 128 )

		imgLine5_2 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine5_2", 32, 32, 239,143 )
		UI_LoadImage( imgLine5_2, "texture/ui/tiger.tga", NORMAL, 32, 32, 32, 128 )

		imgLine5_3 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine5_3", 32, 32, 239,143 )
		UI_LoadImage( imgLine5_3,"texture/ui/tiger.tga", NORMAL, 32, 32, 64, 128 )

		imgLine5_4 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine5_4", 32, 32, 239,143 )
		UI_LoadImage( imgLine5_4, "texture/ui/tiger.tga", NORMAL, 32, 32, 96, 128 )

		imgLine5_5 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine5_5", 32, 32, 239,143 )
		UI_LoadImage( imgLine5_5, "texture/ui/tiger.tga", NORMAL, 32, 32, 128, 128 )
		--
		imgLine6_1 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine6_1", 32, 32, 239,199 )
		UI_LoadImage( imgLine6_1, "texture/ui/tiger.tga", NORMAL, 32, 32, 0, 160 )

		imgLine6_2 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine6_2", 32, 32, 239,199 )
		UI_LoadImage( imgLine6_2, "texture/ui/tiger.tga", NORMAL, 32, 32, 32, 160 )

		imgLine6_3 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine6_3", 32, 32, 239,199 )
		UI_LoadImage( imgLine6_3,"texture/ui/tiger.tga", NORMAL, 32, 32, 64, 160 )

		imgLine6_4 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine6_4", 32, 32, 239,199 )
		UI_LoadImage( imgLine6_4, "texture/ui/tiger.tga", NORMAL, 32, 32, 96, 160 )

		imgLine6_5 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine6_5", 32, 32, 239,199 )
		UI_LoadImage( imgLine6_5, "texture/ui/tiger.tga", NORMAL, 32, 32, 128, 160 )
	-------------------------------------------------------------------------------------------------
	--
		--
		imgLine7_1 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine7_1", 32, 32, 297, 86 )
		UI_LoadImage( imgLine7_1, "texture/ui/tiger.tga", NORMAL, 32, 32, 160, 0 )

		imgLine7_2 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine7_2", 32, 32, 297, 86 )
		UI_LoadImage( imgLine7_2, "texture/ui/tiger.tga", NORMAL, 32, 32, 192, 0 )

		imgLine7_3 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine7_3", 32, 32, 297, 86 )
		UI_LoadImage( imgLine7_3,"texture/ui/tiger.tga", NORMAL, 32, 32, 224, 0 )

		imgLine7_4 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine7_4", 32, 32, 297, 86 )
		UI_LoadImage( imgLine7_4, "texture/ui/tiger.tga", NORMAL, 32, 32, 160, 96 )

		imgLine7_5 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine7_5", 32, 32, 297, 86 )
		UI_LoadImage( imgLine7_5, "texture/ui/tiger.tga", NORMAL, 32, 32, 192, 96 )
		--
		imgLine8_1 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine8_1", 32, 32, 297, 143 )
		UI_LoadImage( imgLine8_1, "texture/ui/tiger.tga", NORMAL, 32, 32, 160, 32 )

		imgLine8_2 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine8_2", 32, 32, 297, 143 )
		UI_LoadImage( imgLine8_2, "texture/ui/tiger.tga", NORMAL, 32, 32, 192, 32 )

		imgLine8_3 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine8_3", 32, 32, 297, 143 )
		UI_LoadImage( imgLine8_3,"texture/ui/tiger.tga", NORMAL, 32, 32, 224, 32 )

		imgLine8_4 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine8_4", 32, 32, 297, 143 )
		UI_LoadImage( imgLine8_4, "texture/ui/tiger.tga", NORMAL, 32, 32, 160, 128 )

		imgLine8_5 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine8_5", 32, 32, 297, 143 )
		UI_LoadImage( imgLine8_5, "texture/ui/tiger.tga", NORMAL, 32, 32, 192, 128 )
		--
		imgLine9_1 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine9_1", 32, 32, 297, 199 )
		UI_LoadImage( imgLine9_1, "texture/ui/tiger.tga", NORMAL, 32, 32, 160, 64 )

		imgLine9_2 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine9_2", 32, 32, 297, 199 )
		UI_LoadImage( imgLine9_2, "texture/ui/tiger.tga", NORMAL, 32, 32, 192, 64 )

		imgLine9_3 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine9_3", 32, 32, 297, 199 )
		UI_LoadImage( imgLine9_3,"texture/ui/tiger.tga", NORMAL, 32, 32, 224, 64 )

		imgLine9_4 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine9_4", 32, 32, 297, 199 )
		UI_LoadImage( imgLine9_4, "texture/ui/tiger.tga", NORMAL, 32, 32, 160, 160 )

		imgLine9_5 = UI_CreateCompent( frmSpiritErnie, IMAGE_TYPE, "imgLine9_5", 32, 32, 297, 199 )
		UI_LoadImage( imgLine9_5, "texture/ui/tiger.tga", NORMAL, 32, 32, 192, 160 )

--9
cmdItemTiger1 = UI_CreateCompent( frmSpiritErnie, COMMAND_ONE_TYPE, "cmdItemTiger1", 32, 32, 180, 86 )
cmdItemTiger2 = UI_CreateCompent( frmSpiritErnie, COMMAND_ONE_TYPE, "cmdItemTiger2", 32, 32, 180, 143 )
cmdItemTiger3 = UI_CreateCompent( frmSpiritErnie, COMMAND_ONE_TYPE, "cmdItemTiger3", 32, 32, 180, 199 )
cmdItemTiger4 = UI_CreateCompent( frmSpiritErnie, COMMAND_ONE_TYPE, "cmdItemTiger4", 32, 32, 239, 86 )
cmdItemTiger5 = UI_CreateCompent( frmSpiritErnie, COMMAND_ONE_TYPE, "cmdItemTiger5", 32, 32, 239, 143 )
cmdItemTiger6 = UI_CreateCompent( frmSpiritErnie, COMMAND_ONE_TYPE, "cmdItemTiger6", 32, 32, 239, 199 )
cmdItemTiger7 = UI_CreateCompent( frmSpiritErnie, COMMAND_ONE_TYPE, "cmdItemTiger7", 32, 32, 297, 86 )
cmdItemTiger8 = UI_CreateCompent( frmSpiritErnie, COMMAND_ONE_TYPE, "cmdItemTiger8", 32, 32, 297, 143 )
cmdItemTiger9 = UI_CreateCompent( frmSpiritErnie, COMMAND_ONE_TYPE, "cmdItemTiger9", 32, 32, 297, 199 )

--
btnStart = UI_CreateCompent( frmSpiritErnie, BUTTON_TYPE, "btnStart", 48, 19, 80, 252 )
UI_LoadButtonImage( btnStart, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

labStart = UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labStart", 400, 150, 92, 255 )
UI_SetCaption( labStart, "Start")
UI_SetTextColor( labStart, COLOR_BLACK )
UI_SetLabelExFont( labStart, DEFAULT_FONT, TRUE, COLOR_WHITE )

--
btnClose = UI_CreateCompent( frmSpiritErnie, BUTTON_TYPE, "btnClose", 14, 14, 403, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

--
labMoneyShow = UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labMoneyShow", 112, 14, 84, 42 )
UI_SetCaption( labMoneyShow, "Note: Minimum try cost 5 Fairy Coins!")
UI_SetTextColor( labMoneyShow, COLOR_RED )
UI_SetLabelExFont( labMoneyShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

--
labMoneyShow = UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labMoneyShow", 112, 14, 84, 60 )
UI_SetCaption( labMoneyShow, "Note: Do not have than 1900000000G in your inventory!")
UI_SetTextColor( labMoneyShow, COLOR_RED )
UI_SetLabelExFont( labMoneyShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

--label1
labSetmoney1= UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labSetmoney1", 112, 14, 100, 85 )
UI_SetCaption( labSetmoney1, "Bet")
UI_SetTextColor( labSetmoney1, COLOR_BLACK )
UI_SetLabelExFont( labSetmoney1, DEFAULT_FONT, TRUE, COLOR_WHITE )

--1
chkSetmoney1 = UI_CreateCompent( frmSpiritErnie, CHECK_TYPE, "chkSetmoney1", 10, 10, 130, 85 )
UI_LoadImage( chkSetmoney1, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkSetmoney1, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )
UI_SetHint( chkSetmoney1, "Bet" )

--label
labUsemoney1= UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labUsemoney1", 112, 14, 78, 106 )
UI_SetCaption( labUsemoney1, "")
UI_SetTextColor( labUsemoney1, COLOR_BLACK )
UI_SetLabelExFont( labUsemoney1, DEFAULT_FONT, TRUE, COLOR_WHITE )

--label2
labSetmoney2= UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labSetmoney2", 112, 14, 100, 142 )
UI_SetCaption( labSetmoney2, "Bet")
UI_SetTextColor( labSetmoney2, COLOR_BLACK )
UI_SetLabelExFont( labSetmoney2, DEFAULT_FONT, TRUE, COLOR_WHITE )

--2
chkSetmoney2 = UI_CreateCompent( frmSpiritErnie, CHECK_TYPE, "chkSetmoney2", 10, 10, 130, 142 )
UI_LoadImage( chkSetmoney2, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkSetmoney2, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )
UI_SetHint( chkSetmoney2, "Bet" )
--label

labUsemoney2= UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labUsemoney2", 112, 14, 78, 163 )
UI_SetCaption( labUsemoney2, "")
UI_SetTextColor( labUsemoney2, COLOR_BLACK )
UI_SetLabelExFont( labUsemoney2, DEFAULT_FONT, TRUE, COLOR_WHITE )

--label3
labSetmoney3 = UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labSetmoney3", 112, 14, 100, 198 )
UI_SetCaption( labSetmoney3, "Bet")
UI_SetTextColor( labSetmoney3, COLOR_BLACK )
UI_SetLabelExFont( labSetmoney3, DEFAULT_FONT, TRUE, COLOR_WHITE )

--3
chkSetmoney3 = UI_CreateCompent( frmSpiritErnie, CHECK_TYPE, "chkSetmoney3", 10, 10, 130, 198 )
UI_LoadImage( chkSetmoney3, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkSetmoney3, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )
UI_SetHint( chkSetmoney3, "Bet" )
--label

labUsemoney3= UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labUsemoney3", 112, 14, 78, 219 )
UI_SetCaption( labUsemoney3, "")
UI_SetTextColor( labUsemoney3, COLOR_BLACK )
UI_SetLabelExFont( labUsemoney3, DEFAULT_FONT, TRUE, COLOR_WHITE )

--
btnStop1 = UI_CreateCompent( frmSpiritErnie, BUTTON_TYPE, "btnStop1", 48, 19, 175, 252 )
UI_LoadButtonImage( btnStop1, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

labStop1 = UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labTryon", 400, 150, 187, 255 )
UI_SetCaption( labStop1, "Stop")
UI_SetTextColor( labStop1, COLOR_BLACK )
UI_SetLabelExFont( labStop1, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnStop2 = UI_CreateCompent( frmSpiritErnie, BUTTON_TYPE, "btnStop2", 48, 19, 234, 252 )
UI_LoadButtonImage( btnStop2, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

labStop2 = UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labTryon", 400, 150, 246, 255 )
UI_SetCaption( labStop2, "Stop")
UI_SetTextColor( labStop2, COLOR_BLACK )
UI_SetLabelExFont( labStop2, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnStop3 = UI_CreateCompent( frmSpiritErnie, BUTTON_TYPE, "btnStop3", 48, 19, 292, 252 )
UI_LoadButtonImage( btnStop3, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

labStop3 = UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labStop3", 400, 150, 304, 255 )
UI_SetCaption( labStop3, "Stop")
UI_SetTextColor( labStop3, COLOR_BLACK )
UI_SetLabelExFont( labStop3, DEFAULT_FONT, TRUE, COLOR_WHITE )

--label
labLastshow1= UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labLastshow1", 112, 14, 40, 300 )
UI_SetCaption( labLastshow1, "This is the first result explanation")
UI_SetTextColor( labLastshow1, COLOR_BLACK )
UI_SetLabelExFont( labLastshow1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labLastshow2= UI_CreateCompent( frmSpiritErnie, LABELEX_TYPE, "labLastshow2", 112, 14, 40, 320 )
UI_SetCaption( labLastshow2, "This is the second line outcome explaination")
UI_SetTextColor( labLastshow2, COLOR_BLACK )
UI_SetLabelExFont( labLastshow2, DEFAULT_FONT, TRUE, COLOR_WHITE )


-------------------
-- chaos ranking --
-------------------
frmPKSilver = UI_CreateForm( "frmPKSilver", FALSE, 548, 386, 0, 0, TRUE, FALSE )
UI_ShowForm( frmPKSilver, FALSE )
UI_AddFormToTemplete( frmPKSilver, FORM_MAIN )
UI_FormSetIsEscClose( frmPKSilver, TRUE )
UI_SetIsDrag( frmPKSilver,TRUE)
UI_SetFormStyle( frmPKSilver,2)

--UI_FormSetHotKey( frmPKSilver, ALT_KEY, HOTKEY_Z )

imgBackGround1 = UI_CreateCompent( frmPKSilver, IMAGE_TYPE, "imgBackGround1", 256, 256, 0, 0 )
UI_LoadImage( imgBackGround1, "texture/ui/fighting1.tga", NORMAL, 256,256, 0, 0 )

imgBackGround2 = UI_CreateCompent( frmPKSilver, IMAGE_TYPE, "imgBackGround2", 256, 256, 256, 0 )
UI_LoadImage( imgBackGround2, "texture/ui/fighting2.tga", NORMAL, 256,256, 0, 0 )

imgBackGround3 = UI_CreateCompent( frmPKSilver, IMAGE_TYPE, "imgBackGround3", 46, 256, 512, 0 )
UI_LoadImage( imgBackGround3, "texture/ui/fighting5.tga", NORMAL, 46, 256, 0, 0 )

imgBackGround4 = UI_CreateCompent( frmPKSilver, IMAGE_TYPE, "imgBackGround4", 256, 130, 0, 256 )
UI_LoadImage( imgBackGround4, "texture/ui/fighting3.tga", NORMAL, 256,130, 0, 0 )

imgBackGround5 = UI_CreateCompent( frmPKSilver, IMAGE_TYPE, "imgBackGround5", 256, 130, 256, 256 )
UI_LoadImage( imgBackGround5, "texture/ui/fighting4.tga", NORMAL, 256,130, 0, 0 )

imgBackGround6 = UI_CreateCompent( frmPKSilver, IMAGE_TYPE, "imgBackGround6", 46, 130, 512, 256 )
UI_LoadImage( imgBackGround6, "texture/ui/fighting5.tga", NORMAL, 46,130, 46, 0 )

btnClose = UI_CreateCompent( frmPKSilver, BUTTON_TYPE, "btnClose", 14, 14, 531, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labTtitle1 = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labTtitle1", 400, 150,60, 104 )
UI_SetCaption( labTtitle1, "Character ")
UI_SetTextColor( labTtitle1, COLOR_RED )
UI_SetLabelExFont( labTtitle1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTtitle2 = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labTtitle2", 400, 150,175, 104 )
UI_SetCaption( labTtitle2, "Level")
UI_SetTextColor( labTtitle2, COLOR_RED )
UI_SetLabelExFont( labTtitle2, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTtitle3 = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labTtitle3", 400, 150,289, 104 )
UI_SetCaption( labTtitle3, "Class")
UI_SetTextColor( labTtitle3, COLOR_RED )
UI_SetLabelExFont( labTtitle3, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTtitle4 = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labTtitle4", 400, 150,406, 104 )
UI_SetCaption( labTtitle4, "Chaos Points")
UI_SetTextColor( labTtitle4, COLOR_RED )
UI_SetLabelExFont( labTtitle4, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTtitle5 = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labTtitle5", 400, 150,27, 104 )
UI_SetCaption( labTtitle5, "NO")
UI_SetTextColor( labTtitle5, COLOR_RED )
UI_SetLabelExFont( labTtitle5, DEFAULT_FONT, TRUE, COLOR_WHITE )

local labName = {}
local labLevel = {}
local labJob = {}
local labData = {}
local labNO = {}

for i=0 ,4 ,1 do
	labName[i] = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labName_"..i, 10, 150, 60, i*32+136 )
	UI_SetCaption( labName[i], "User Name"..i)
	UI_SetTextColor( labName[i], COLOR_BLACK )
	UI_SetLabelExFont( labName[i], DEFAULT_FONT, TRUE, COLOR_WHITE )

	labLevel[i] = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labLevel_"..i, 10, 150, 175, i*32+136 )
	UI_SetCaption( labLevel[i], "Player Lv"..i)
	UI_SetTextColor( labLevel[i], COLOR_BLACK )
	UI_SetLabelExFont( labLevel[i], DEFAULT_FONT, TRUE, COLOR_WHITE )

	labJob[i] = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labJob_"..i, 10, 150, 289, i*32+136 )
	UI_SetCaption( labJob[i], "Player class"..i)
	UI_SetTextColor( labJob[i], COLOR_BLACK )
	UI_SetLabelExFont( labJob[i], DEFAULT_FONT, TRUE, COLOR_WHITE )

	labData[i] = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labData_"..i, 10, 150, 406, i*32+136 )
	UI_SetCaption( labData[i], "time"..i)
	UI_SetTextColor( labData[i], COLOR_BLACK )
	UI_SetLabelExFont( labData[i], DEFAULT_FONT, TRUE, COLOR_WHITE )

	labNO[i] = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labTtitle5", 400, 150, 27, i*32+136 )
	UI_SetCaption( labNO[i], ""..(i+1))
	UI_SetTextColor( labNO[i], COLOR_RED )
	UI_SetLabelExFont( labNO[i], DEFAULT_FONT, TRUE, COLOR_WHITE )
end
	labNotice1 = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labNotice1", 10, 150, 50, 305)
	UI_SetCaption( labNotice1, "Assemble and fuse the Chaos stone set: Framestone, Clawstone and Pawstone")
	UI_SetTextColor( labNotice1, COLOR_BLACK )
	UI_SetLabelExFont( labNotice1, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	labNotice2 = UI_CreateCompent( frmPKSilver, LABELEX_TYPE, "labNotice2", 10, 150, 50, 320)
	UI_SetCaption( labNotice2, "To receive hidden powers of the full set based on ranking on this list")
	UI_SetTextColor( labNotice2, COLOR_BLACK )
	UI_SetLabelExFont( labNotice2, DEFAULT_FONT, TRUE, COLOR_WHITE )

--------------------------------------
-- Party Search by 2006.10.17 by Leo --
--------------------------------------
frmFindTeam = UI_CreateForm( "frmFindTeam", FALSE, 548, 386, 0, 0, TRUE, FALSE )
UI_ShowForm( frmFindTeam, FALSE )
UI_AddFormToTemplete( frmFindTeam, FORM_MAIN )
UI_FormSetIsEscClose( frmFindTeam, TRUE )
UI_SetIsDrag( frmFindTeam,TRUE)
UI_SetFormStyle( frmFindTeam,2)

--
imgBackGround1 = UI_CreateCompent( frmFindTeam, IMAGE_TYPE, "imgBackGround1", 256, 256, 0, 0 )
UI_LoadImage( imgBackGround1, "texture/ui/plist1.tga", NORMAL, 256,256, 0, 0 )

imgBackGround2 = UI_CreateCompent( frmFindTeam, IMAGE_TYPE, "imgBackGround2", 256, 256, 256, 0 )
UI_LoadImage( imgBackGround2, "texture/ui/plist2.tga", NORMAL, 256,256, 0, 0 )

imgBackGround3 = UI_CreateCompent( frmFindTeam, IMAGE_TYPE, "imgBackGround3", 256, 130, 0, 256 )
UI_LoadImage( imgBackGround3, "texture/ui/plist3.tga", NORMAL, 256, 130, 0, 0 )

imgBackGround4 = UI_CreateCompent( frmFindTeam, IMAGE_TYPE, "imgBackGround4", 256, 130, 256, 256 )
UI_LoadImage( imgBackGround4, "texture/ui/plist4.tga", NORMAL, 256,130, 0, 0 )

imgBackGround5 = UI_CreateCompent( frmFindTeam, IMAGE_TYPE, "imgBackGround5", 46, 256, 512, 0 )
UI_LoadImage( imgBackGround5, "texture/ui/plist5.tga", NORMAL, 46,256, 0, 0 )

imgBackGround6 = UI_CreateCompent( frmFindTeam, IMAGE_TYPE, "imgBackGround6", 46, 130, 512, 256 )
UI_LoadImage( imgBackGround6, "texture/ui/plist5.tga", NORMAL, 46,130, 46, 0 )

labTitle = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labTtitle", 400, 150,89, 27 )
UI_SetCaption( labTitle, "Look for suitable players to form a party")
UI_SetTextColor( labTitle, COLOR_PURPLE )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTitle = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labTitle", 400, 150,7, 3 )
UI_SetCaption( labTitle, "Party Search")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTtitle1 = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labTtitle1", 400, 150,35, 55 )
UI_SetCaption( labTtitle1, "Character")
UI_SetTextColor( labTtitle1, COLOR_RED )
UI_SetLabelExFont( labTtitle1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTtitle2 = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labTtitle2", 400, 150,164, 55 )
UI_SetCaption( labTtitle2, "Level")
UI_SetTextColor( labTtitle2, COLOR_RED )
UI_SetLabelExFont( labTtitle2, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTtitle3 = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labTtitle3", 400, 150,269, 55 )
UI_SetCaption( labTtitle3, "Class")
UI_SetTextColor( labTtitle3, COLOR_RED )
UI_SetLabelExFont( labTtitle3, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTtitle4 = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labTtitle4", 400, 150,376, 55 )
UI_SetCaption( labTtitle4, "Location")
UI_SetTextColor( labTtitle4, COLOR_RED )
UI_SetLabelExFont( labTtitle4, DEFAULT_FONT, TRUE, COLOR_WHITE )

local labName = {}
local labLevel = {}
local labJob = {}
local labPlace = {}
local btnSubmit = {}

for i=0 ,9 ,1 do
	labName[i] = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labName_"..i, 10, 150, 35, i*28+83 )
	UI_SetCaption( labName[i], "User Name"..i)
	UI_SetTextColor( labName[i], COLOR_BLACK )
	UI_SetLabelExFont( labName[i], DEFAULT_FONT, TRUE, COLOR_WHITE )

	labLevel[i] = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labLevel_"..i, 10, 150, 164, i*28+83 )
	UI_SetCaption( labLevel[i], "Player Lv"..i)
	UI_SetTextColor( labLevel[i], COLOR_BLACK )
	UI_SetLabelExFont( labLevel[i], DEFAULT_FONT, TRUE, COLOR_WHITE )

	labJob[i] = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labJob_"..i, 10, 150, 269, i*28+83 )
	UI_SetCaption( labJob[i], "Player class"..i)
	UI_SetTextColor( labJob[i], COLOR_BLACK )
	UI_SetLabelExFont( labJob[i], DEFAULT_FONT, TRUE, COLOR_WHITE )

	labPlace[i] = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labPlace_"..i, 10, 150, 376, i*28+83 )
	UI_SetCaption( labPlace[i], "Location"..i)
	UI_SetTextColor( labPlace[i], COLOR_BLACK )
	UI_SetLabelExFont( labPlace[i], DEFAULT_FONT, TRUE, COLOR_WHITE )

	btnSubmit[i] =  UI_CreateCompent( frmFindTeam, BUTTON_TYPE, "btnSubmit_"..i, 41, 19, 470, i*28+80 )
	UI_LoadButtonImage( btnSubmit[i], "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
end

	btnLeftPage = UI_CreateCompent( frmFindTeam, BUTTON_TYPE, "btnLeftPage", 13, 13, 30, 365 )
	UI_LoadButtonImage( btnLeftPage, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE )

	labListPage = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labListPage", 400, 150, 55, 365 )
	UI_SetCaption( labListPage, "")
	UI_SetTextColor( labListPage, COLOR_BLACK )
	UI_SetLabelExFont( labListPage, DEFAULT_FONT, TRUE, COLOR_WHITE )

	btnRightPage = UI_CreateCompent( frmFindTeam, BUTTON_TYPE, "btnRightPage", 13, 13, 80, 365 )
	UI_LoadButtonImage( btnRightPage, "texture/ui/SystemBotton3.tga", 13, 13, 0, 211, TRUE )

	btnAddme = UI_CreateCompent( frmFindTeam, BUTTON_TYPE, "btnAddme", 76, 19, 370, 360 )
	UI_LoadButtonImage( btnAddme, "texture/ui/store8.tga", 76, 19, 0, 0, FALSE )

	labAddme = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labAddme", 400, 150, 378, 364 )
	UI_SetCaption( labAddme, "Enlist")
	UI_SetTextColor( labAddme, COLOR_BLACK )
	UI_SetLabelExFont( labAddme, DEFAULT_FONT, TRUE, COLOR_WHITE )


	btnDelme = UI_CreateCompent( frmFindTeam, BUTTON_TYPE, "btnDelme", 76, 19, 460, 360 )
	UI_LoadButtonImage( btnDelme, "texture/ui/store8.tga", 76, 19, 0, 0, FALSE )

	labDelme = UI_CreateCompent( frmFindTeam, LABELEX_TYPE, "labAddme", 400, 150, 468, 364 )
	UI_SetCaption( labDelme, "Withdraw")
	UI_SetTextColor( labDelme, COLOR_BLACK )
	UI_SetLabelExFont( labDelme, DEFAULT_FONT, TRUE, COLOR_WHITE )

	btnClose = UI_CreateCompent( frmFindTeam, BUTTON_TYPE, "btnClose", 14, 14, 528, 3 )
	UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
	UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-------------------------------------
-- 2006.10.19 by Leo Manufacturing --
-------------------------------------
frmCompose = UI_CreateForm( "frmCompose", FALSE, 305, 480, 0, 0, TRUE, FALSE )
UI_ShowForm( frmCompose, FALSE )
UI_AddFormToTemplete( frmCompose, FORM_MAIN )
UI_FormSetIsEscClose( frmCompose, TRUE )
UI_SetIsDrag( frmCompose,TRUE)
UI_SetFormStyle( frmCompose,2)

--UI_FormSetHotKey( frmCompose, ALT_KEY, HOTKEY_Z )

imgBakTopLeft = UI_CreateCompent( frmCompose, IMAGE_TYPE, "imgBakTopLeft", 256, 256, 0, 0 )
UI_LoadImage( imgBakTopLeft, "texture/ui/Compose1.tga", NORMAL, 256, 256, 0, 0 )

imgBakTopRight = UI_CreateCompent( frmCompose, IMAGE_TYPE, "imgBakTopRight", 49, 256, 256, 0 )
UI_LoadImage( imgBakTopRight, "texture/ui/Compose3.tga", NORMAL, 49, 256, 0, 0 )

imgBakBottomLeft = UI_CreateCompent( frmCompose, IMAGE_TYPE, "imgBakBottomLeft", 256, 224, 0, 256 )
UI_LoadImage( imgBakBottomLeft, "texture/ui/Compose2.tga", NORMAL, 256, 224, 0, 0 )

imgBakBottomRight = UI_CreateCompent( frmCompose, IMAGE_TYPE, "imgBakBottomRight", 49, 224, 256, 256 )
UI_LoadImage( imgBakBottomRight, "texture/ui/Compose3.tga", NORMAL, 49, 224, 49, 0 )

imgCover = UI_CreateCompent( frmCompose, IMAGE_TYPE, "imgCover", 181, 23, 26, 434 )
UI_LoadImage( imgCover, "texture/ui/Compose2.tga", NORMAL, 30, 23, 26, 146)

labTitle = UI_CreateCompent( frmCompose, LABELEX_TYPE, "labTitle", 49, 14, 7, 3 )
UI_SetCaption( labTitle, "Manufacturing")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

cmdItemBase4 = UI_CreateCompent( frmCompose, COMMAND_ONE_TYPE, "cmdItemBase4", 32, 32, 46, 87 )
UI_SetHint(cmdItemBase4,"Material 1")

cmdItemBase7 = UI_CreateCompent( frmCompose, COMMAND_ONE_TYPE, "cmdItemBase7", 32, 32, 142, 143 )
UI_SetHint(cmdItemBase7,"Product")

cmdItemBase5 = UI_CreateCompent( frmCompose, COMMAND_ONE_TYPE, "cmdItemBase5", 32, 32, 47, 205 )
UI_SetHint(cmdItemBase5,"Ingredient 2")
cmdItemBase6 = UI_CreateCompent( frmCompose, COMMAND_ONE_TYPE, "cmdItemBase6", 32, 32, 229, 205 )
UI_SetHint(cmdItemBase6,"Ingredient 3")

cmdItemBase3 = UI_CreateCompent( frmCompose, COMMAND_ONE_TYPE, "cmdItemBase3", 32, 32, 101, 246 )
UI_SetHint(cmdItemBase3,"Tool")
cmdItemBase2 = UI_CreateCompent( frmCompose, COMMAND_ONE_TYPE, "cmdItemBase2", 32, 32, 179, 246 )
UI_SetHint(cmdItemBase2,"Blueprint")

cmdItemBase1 = UI_CreateCompent( frmCompose, COMMAND_ONE_TYPE, "cmdItemBase1", 32, 32, 142, 319 )
UI_SetHint(cmdItemBase1,"Fairy")

labCharOutput = UI_CreateCompent( frmCompose, LABELEX_TYPE, "labCharOutput", 49, 14, 54, 414 )
UI_SetCaption( labCharOutput, "ABCDEFGH")
UI_SetTextColor( labCharOutput, COLOR_YELLOW)
UI_SetLabelExFont( labCharOutput, FONT28, FALSE, COLOR_WHITE )

imgComposeFrame = UI_CreateCompent( frmCompose, IMAGE_TYPE, "imgComposeFrame", 278, 13, 13, 369 )
UI_LoadImage( imgComposeFrame, "texture/ui/Progress.tga", NORMAL, 157, 13, 0, 52 )

proCompose = UI_CreateCompent( frmCompose, PROGRESS_TYPE, "proCompose", 278, 13, 13, 369 )
UI_LoadImage( proCompose, "texture/ui/Progress.tga", PROGRESS_PROGRESS, 157, 13, 0, 65 )

labNumInput = UI_CreateCompent( frmCompose, LABELEX_TYPE, "labNumInput", 49, 14, 41, 318 )
UI_SetCaption( labNumInput, "Quantity")
UI_SetTextColor( labNumInput, COLOR_RED )
UI_SetLabelExFont( labNumInput, DEFAULT_FONT, FALSE, COLOR_WHITE )

btnLeftPage = UI_CreateCompent( frmCompose, BUTTON_TYPE, "btnLeftPage", 13, 13, 22, 335 )
UI_LoadButtonImage( btnLeftPage, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE )

edtNumInput = UI_CreateCompent(frmCompose, EDIT_TYPE,"edtNumInput", 49, 14, 42, 335)
UI_SetEditMaxNum(edtNumInput, 8 )
UI_SetEditMaxNumVisible(edtNumInput,8)
UI_SetIsKeyFocus(edtNumInput,FALSE)
btnRightPage = UI_CreateCompent( frmCompose, BUTTON_TYPE, "btnRightPage", 13, 13,  97, 335 )
UI_LoadButtonImage( btnRightPage, "texture/ui/SystemBotton3.tga", 13, 13, 0, 211, TRUE )

btnForgeYes = UI_CreateCompent( frmCompose, BUTTON_TYPE, "btnForgeYes", 57, 26, 230, 389 )
UI_LoadButtonImage( btnForgeYes, "texture/ui/Progress.tga", 57, 26, 47, 78, FALSE )

labInput = UI_CreateCompent( frmCompose, LABELEX_TYPE, "labInput", 49, 14, 246, 396 )
UI_SetCaption( labInput, "Start")
UI_SetTextColor( labInput, COLOR_BLACK )
UI_SetLabelExFont( labInput, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnForgeNo = UI_CreateCompent( frmCompose, BUTTON_TYPE, "btnForgeNo", 57, 26, 230, 441 )
UI_LoadButtonImage( btnForgeNo, "texture/ui/Progress.tga", 57, 26, 47, 78, FALSE )

labInput = UI_CreateCompent( frmCompose, LABELEX_TYPE, "labInput", 49, 14, 246, 448 )
UI_SetCaption( labInput, "Stop")
UI_SetTextColor( labInput, COLOR_BLACK )
UI_SetLabelExFont( labInput, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmCompose, BUTTON_TYPE, "btnClose", 14, 14, 285, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

------------------------------------------
-- 2006.10.24 by Leo Analyze ui by aLex --
------------------------------------------
frmBreak = UI_CreateForm( "frmBreak", FALSE, 305, 480, 0, 0, TRUE, FALSE )
UI_ShowForm( frmBreak, FALSE )
UI_AddFormToTemplete( frmBreak, FORM_MAIN )
UI_FormSetIsEscClose( frmBreak, TRUE )
UI_SetIsDrag( frmBreak,TRUE)
UI_SetFormStyle( frmBreak,2)

--UI_FormSetHotKey( frmBreak, ALT_KEY, HOTKEY_Z )
imgBakTopLeft = UI_CreateCompent( frmBreak, IMAGE_TYPE, "imgBakTopLeft", 256, 256, 0, 0 )
UI_LoadImage( imgBakTopLeft, "texture/ui/Break1.tga", NORMAL, 256, 256, 0, 0 )

imgBakTopRight = UI_CreateCompent( frmBreak, IMAGE_TYPE, "imgBakTopRight", 49, 256, 256, 0 )
UI_LoadImage( imgBakTopRight, "texture/ui/Break3.tga", NORMAL, 49, 256, 0, 0 )

imgBakBottomLeft = UI_CreateCompent( frmBreak, IMAGE_TYPE, "imgBakBottomLeft", 256, 224, 0, 256 )
UI_LoadImage( imgBakBottomLeft, "texture/ui/Break2.tga", NORMAL, 256, 224, 0, 0 )

imgBakBottomRight = UI_CreateCompent( frmBreak, IMAGE_TYPE, "imgBakBottomRight", 49, 224, 256, 256 )
UI_LoadImage( imgBakBottomRight, "texture/ui/Break3.tga", NORMAL, 49, 224, 49, 0 )

labTitle = UI_CreateCompent( frmBreak, LABELEX_TYPE, "labTitle", 49, 14, 7, 3 )
UI_SetCaption( labTitle, "Analyze")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

cmdItemBase4 = UI_CreateCompent( frmBreak, COMMAND_ONE_TYPE, "cmdItemBase4", 32, 32, 135, 43 )
UI_SetHint(cmdItemBase4,"Catalyst")

cmdItemBase3 = UI_CreateCompent( frmBreak, COMMAND_ONE_TYPE, "cmdItemBase3", 32, 32, 135, 137 )
UI_SetHint(cmdItemBase3,"Ingredients")

cmdItemBase2 = UI_CreateCompent( frmBreak, COMMAND_ONE_TYPE, "cmdItemBase2", 32, 32, 135, 226 )
UI_SetHint(cmdItemBase2,"Tool")

cmdItemBase1 = UI_CreateCompent( frmBreak, COMMAND_ONE_TYPE, "cmdItemBase1", 32, 32, 134, 318 )
UI_SetHint(cmdItemBase1,"Fairy")

imgBreakFrame = UI_CreateCompent( frmBreak, IMAGE_TYPE, "imgBreakFrame", 278, 13, 13, 369 )
UI_LoadImage( imgBreakFrame, "texture/ui/Progress.tga", NORMAL, 157, 13, 0, 52 )

proBreak = UI_CreateCompent( frmBreak, PROGRESS_TYPE, "proBreak", 278, 13, 13, 369 )
UI_LoadImage( proBreak, "texture/ui/Progress.tga", PROGRESS_PROGRESS, 157, 13, 0, 65)

imgBreakOver = UI_CreateCompent( frmBreak, IMAGE_TYPE, "imgBreakOver", 110, 25,170, 395 )
UI_LoadImage( imgBreakOver, "texture/ui/Break2.tga", NORMAL, 90, 15, 30, 150 )

labNumOutputLeft = UI_CreateCompent( frmBreak, LABELEX_TYPE, "labNumOutputLeft", 10, 145, 31, 402 )
UI_SetCaption( labNumOutputLeft, "")
UI_SetTextColor( labNumOutputLeft, COLOR_YELLOW )
UI_SetLabelExFont( labNumOutputLeft, FONT20, FALSE, COLOR_YELLOW )

labNumOutputRight = UI_CreateCompent( frmBreak, LABELEX_TYPE, "labNumOutputRight", 10, 145, 179, 402 )
UI_SetCaption( labNumOutputRight, "")
UI_SetTextColor( labNumOutputRight, COLOR_YELLOW )
UI_SetLabelExFont( labNumOutputRight, FONT20, FALSE, COLOR_YELLOW )

btnForgeYes = UI_CreateCompent( frmBreak, BUTTON_TYPE, "btnForgeYes", 57, 26, 43, 441 )
UI_LoadButtonImage( btnForgeYes, "texture/ui/Progress.tga", 57, 26, 47, 78, FALSE )

labInput = UI_CreateCompent( frmBreak, LABELEX_TYPE, "labInput", 49, 14, 59, 448 )
UI_SetCaption( labInput, "Start")
UI_SetTextColor( labInput, COLOR_BLACK )
UI_SetLabelExFont( labInput, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnForgeNo = UI_CreateCompent( frmBreak, BUTTON_TYPE, "btnForgeNo", 57, 26, 196, 441 )
UI_LoadButtonImage( btnForgeNo, "texture/ui/Progress.tga", 57, 26, 47, 78, FALSE )

labInput = UI_CreateCompent( frmBreak, LABELEX_TYPE, "labInput", 49, 14, 212, 448 )
UI_SetCaption( labInput, "Stop")
UI_SetTextColor( labInput, COLOR_BLACK )
UI_SetLabelExFont( labInput, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmBreak, BUTTON_TYPE, "btnClose", 14, 14, 285, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

--------------------------------
-- 2006.10.24 by Leo crafting --
--------------------------------
frmFound = UI_CreateForm( "frmFound", FALSE, 305, 480, 0, 0, TRUE, FALSE )
UI_ShowForm( frmFound, FALSE )
UI_AddFormToTemplete( frmFound, FORM_MAIN )
UI_FormSetIsEscClose( frmFound, TRUE )
UI_SetIsDrag( frmFound,TRUE)
UI_SetFormStyle( frmFound,2)

--UI_FormSetHotKey( frmFound, ALT_KEY, HOTKEY_Z )

imgBakTopLeft = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgBakTopLeft", 256, 256, 0, 0 )
UI_LoadImage( imgBakTopLeft, "texture/ui/Found1.tga", NORMAL, 256, 256, 0, 0 )

imgBakTopRight = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgBakTopRight", 49, 256, 256, 0 )
UI_LoadImage( imgBakTopRight, "texture/ui/Found3.tga", NORMAL, 49, 256, 0, 0 )

imgBakBottomLeft = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgBakBottomLeft", 256, 224, 0, 256 )
UI_LoadImage( imgBakBottomLeft, "texture/ui/Found2.tga", NORMAL, 256, 224, 0, 0 )

imgBakBottomRight = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgBakBottomRight", 49, 224, 256, 256 )
UI_LoadImage( imgBakBottomRight, "texture/ui/Found3.tga", NORMAL, 49, 224, 49, 0 )

labTitle = UI_CreateCompent( frmFound, LABELEX_TYPE, "labTitle", 49, 14, 7, 3 )
UI_SetCaption( labTitle, "Crafting")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

cmdItemBase5 = UI_CreateCompent( frmFound, COMMAND_ONE_TYPE, "cmdItemBase5", 32, 32, 136, 79 )
UI_SetHint(cmdItemBase5,"Ingredient 2")

cmdItemBase4 = UI_CreateCompent( frmFound, COMMAND_ONE_TYPE, "cmdItemBase4", 32, 32, 51, 168 )
UI_SetHint(cmdItemBase4,"Material 1")
cmdItemBase7 = UI_CreateCompent( frmFound, COMMAND_ONE_TYPE, "cmdItemBase7", 32, 32, 136, 167 )
UI_SetHint(cmdItemBase7,"Product")
cmdItemBase6 = UI_CreateCompent( frmFound, COMMAND_ONE_TYPE, "cmdItemBase6", 32, 32, 220, 168 )
UI_SetHint(cmdItemBase6,"Ingredient 3")

cmdItemBase3 = UI_CreateCompent( frmFound, COMMAND_ONE_TYPE, "cmdItemBase3", 32, 32, 136, 233 )
UI_SetHint(cmdItemBase3,"Tool")

cmdItemBase1 = UI_CreateCompent( frmFound, COMMAND_ONE_TYPE, "cmdItemBase1", 32, 32, 92, 290 )
UI_SetHint(cmdItemBase1,"Fairy")
cmdItemBase2 = UI_CreateCompent( frmFound, COMMAND_ONE_TYPE, "cmdItemBase2", 32, 32, 179, 290 )
UI_SetHint(cmdItemBase2,"Blueprint")

imgShaiZi1_1 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi1_1", 32, 32, 24, 388 )
UI_LoadImage( imgShaiZi1_1, "texture/ui/Progress.tga", NORMAL, 36, 36, 0, 182 )
-- UI_SetIsShow(imgShaiZi1_1,FALSE)

imgShaiZi1_2 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi1_2", 32, 32, 24, 388 )
UI_LoadImage( imgShaiZi1_2, "texture/ui/Progress.tga", NORMAL, 36, 36, 36, 182 )
UI_SetIsShow(imgShaiZi1_2,FALSE)

imgShaiZi1_3 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi1_3", 32, 32, 24, 388 )
UI_LoadImage( imgShaiZi1_3, "texture/ui/Progress.tga", NORMAL, 36, 36, 72, 182 )
UI_SetIsShow(imgShaiZi1_3,FALSE)

imgShaiZi1_4 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi1_4", 32, 32, 24, 388 )
UI_LoadImage( imgShaiZi1_4, "texture/ui/Progress.tga", NORMAL, 36, 36, 108, 182 )
UI_SetIsShow(imgShaiZi1_4,FALSE)

imgShaiZi1_5 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi1_5", 32, 32, 24, 388 )
UI_LoadImage( imgShaiZi1_5, "texture/ui/Progress.tga", NORMAL, 36, 36, 144, 182 )
UI_SetIsShow(imgShaiZi1_5,FALSE)

imgShaiZi1_6 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi1_6", 32, 32, 24, 388 )
UI_LoadImage( imgShaiZi1_6, "texture/ui/Progress.tga", NORMAL, 36, 36, 180, 182 )
UI_SetIsShow(imgShaiZi1_6,FALSE)

imgShaiZi2_1 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi2_1", 32, 32, 61, 388 )
UI_LoadImage( imgShaiZi2_1, "texture/ui/Progress.tga", NORMAL, 36, 36, 0, 182 )
-- UI_SetIsShow(imgShaiZi2_1,FALSE)

imgShaiZi2_2 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi2_2", 32, 32, 61, 388 )
UI_LoadImage( imgShaiZi2_2, "texture/ui/Progress.tga", NORMAL, 36, 36, 36, 182 )
UI_SetIsShow(imgShaiZi2_2,FALSE)

imgShaiZi2_3 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi2_3", 32, 32, 61, 388 )
UI_LoadImage( imgShaiZi2_3, "texture/ui/Progress.tga", NORMAL, 36, 36, 72, 182 )
UI_SetIsShow(imgShaiZi2_3,FALSE)

imgShaiZi2_4 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi2_4", 32, 32, 61, 388 )
UI_LoadImage( imgShaiZi2_4, "texture/ui/Progress.tga", NORMAL, 36, 36, 108, 182 )
UI_SetIsShow(imgShaiZi2_4,FALSE)

imgShaiZi2_5 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi2_5", 32, 32, 61, 388 )
UI_LoadImage( imgShaiZi2_5, "texture/ui/Progress.tga", NORMAL, 36, 36, 144, 182 )
UI_SetIsShow(imgShaiZi2_5,FALSE)

imgShaiZi2_6 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi2_6", 32, 32, 61, 388 )
UI_LoadImage( imgShaiZi2_6, "texture/ui/Progress.tga", NORMAL, 36, 36, 180, 182 )
UI_SetIsShow(imgShaiZi2_6,FALSE)

imgShaiZi3_1 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi3_1", 32, 32, 98, 388 )
UI_LoadImage( imgShaiZi3_1, "texture/ui/Progress.tga", NORMAL, 36, 36, 0, 182 )
-- UI_SetIsShow(imgShaiZi3_1,FALSE)

imgShaiZi3_2 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi3_2", 32, 32, 98, 388 )
UI_LoadImage( imgShaiZi3_2, "texture/ui/Progress.tga", NORMAL, 36, 36, 36, 182 )
UI_SetIsShow(imgShaiZi3_2,FALSE)

imgShaiZi3_3 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi3_3", 32, 32, 98, 388 )
UI_LoadImage( imgShaiZi3_3, "texture/ui/Progress.tga", NORMAL, 36, 36, 72, 182 )
UI_SetIsShow(imgShaiZi3_3,FALSE)

imgShaiZi3_4 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi3_4", 32, 32, 98, 388 )
UI_LoadImage( imgShaiZi3_4, "texture/ui/Progress.tga", NORMAL, 36, 36, 108, 182 )
UI_SetIsShow(imgShaiZi3_4,FALSE)

imgShaiZi3_5 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi3_5", 32, 32, 98, 388 )
UI_LoadImage( imgShaiZi3_5, "texture/ui/Progress.tga", NORMAL, 36, 36, 144, 182 )
UI_SetIsShow(imgShaiZi3_5,FALSE)

imgShaiZi3_6 = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgShaiZi3_6", 32, 32, 98, 388 )
UI_LoadImage( imgShaiZi3_6, "texture/ui/Progress.tga", NORMAL, 36, 36, 180, 182 )
UI_SetIsShow(imgShaiZi3_6,FALSE)

btnBigBig = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnBigBig", 47, 26, 170, 391 )
UI_LoadButtonImage( btnBigBig, "texture/ui/Progress.tga", 47, 26, 0, 78, FALSE )

labInput = UI_CreateCompent( frmFound, LABELEX_TYPE, "labInput", 49, 14, 185, 398 )
UI_SetCaption( labInput, "Big")
UI_SetTextColor( labInput, COLOR_BLACK )
UI_SetLabelExFont( labInput, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnSmallSmall = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnSmallSmall", 47, 26, 227, 391 )
UI_LoadButtonImage( btnSmallSmall, "texture/ui/Progress.tga", 47, 26, 0, 78, FALSE )

labInput = UI_CreateCompent( frmFound, LABELEX_TYPE, "labInput", 49, 14, 235, 398 )
UI_SetCaption( labInput, "Small")
UI_SetTextColor( labInput, COLOR_BLACK )
UI_SetLabelExFont( labInput, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgFoundFrame = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgFoundFrame", 278, 13, 13, 352 )
UI_LoadImage( imgFoundFrame, "texture/ui/Progress.tga", NORMAL, 157, 13, 0, 52 )

proFound = UI_CreateCompent( frmFound, PROGRESS_TYPE, "proFound", 278, 13, 13, 352 )
UI_LoadImage( proFound, "texture/ui/Progress.tga", PROGRESS_PROGRESS, 157, 13, 0, 65 )

btnForgeYes = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnForgeYes", 57, 26, 124, 439 )
UI_LoadButtonImage( btnForgeYes, "texture/ui/Progress.tga", 57, 26, 47, 78, FALSE )

labInput = UI_CreateCompent( frmFound, LABELEX_TYPE, "labInput", 49, 14, 140, 446 )
UI_SetCaption( labInput, "Start")
UI_SetTextColor( labInput, COLOR_BLACK )
UI_SetLabelExFont( labInput, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnClose", 14, 14, 285, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-------------------------------
-- 2006.10.24 by Leo Cooking --
-------------------------------
frmCooking = UI_CreateForm( "frmCooking", FALSE, 305, 480, 0, 0, TRUE, FALSE )
UI_ShowForm( frmCooking, FALSE )
UI_AddFormToTemplete( frmCooking, FORM_MAIN )
UI_FormSetIsEscClose( frmCooking, TRUE )
UI_SetIsDrag( frmCooking,TRUE)
UI_SetFormStyle( frmCooking,2)

--UI_FormSetHotKey( frmCooking, ALT_KEY, HOTKEY_Z )

imgBakTopLeft = UI_CreateCompent( frmCooking, IMAGE_TYPE, "imgBakTopLeft", 256, 256, 0, 0 )
UI_LoadImage( imgBakTopLeft, "texture/ui/Cooking1.tga", NORMAL, 256, 256, 0, 0 )

imgBakTopRight = UI_CreateCompent( frmCooking, IMAGE_TYPE, "imgBakTopRight", 49, 256, 256, 0 )
UI_LoadImage( imgBakTopRight, "texture/ui/Cooking3.tga", NORMAL, 49, 256, 0, 0 )

imgBakBottomLeft = UI_CreateCompent( frmCooking, IMAGE_TYPE, "imgBakBottomLeft", 256, 224, 0, 256 )
UI_LoadImage( imgBakBottomLeft, "texture/ui/Cooking2.tga", NORMAL, 256, 224, 0, 0 )

imgBakBottomRight = UI_CreateCompent( frmCooking, IMAGE_TYPE, "imgBakBottomRight", 49, 224, 256, 256 )
UI_LoadImage( imgBakBottomRight, "texture/ui/Cooking3.tga", NORMAL, 49, 224, 49, 0 )

labTitle = UI_CreateCompent( frmCooking, LABELEX_TYPE, "labTitle", 49, 14, 7, 3 )
UI_SetCaption( labTitle, "Cooking")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

cmdItemBase5 = UI_CreateCompent( frmCooking, COMMAND_ONE_TYPE, "cmdItemBase5", 32, 32, 134, 54 )
UI_SetHint(cmdItemBase5,"Material 2")

cmdItemBase4 = UI_CreateCompent( frmCooking, COMMAND_ONE_TYPE, "cmdItemBase4", 32, 32, 41, 149 )
UI_SetHint(cmdItemBase4,"Material 1")
cmdItemBase7 = UI_CreateCompent( frmCooking, COMMAND_ONE_TYPE, "cmdItemBase7", 32, 32, 136, 149 )
cmdItemBase6 = UI_CreateCompent( frmCooking, COMMAND_ONE_TYPE, "cmdItemBase6", 32, 32, 230, 149 )
UI_SetHint(cmdItemBase6,"Material 3")

cmdItemBase3 = UI_CreateCompent( frmCooking, COMMAND_ONE_TYPE, "cmdItemBase3", 32, 32,	84, 249 )
UI_SetHint(cmdItemBase3,"Tool")
cmdItemBase2 = UI_CreateCompent( frmCooking, COMMAND_ONE_TYPE, "cmdItemBase2", 32, 32, 194, 249 )
UI_SetHint(cmdItemBase2,"Blueprint")

cmdItemBase1 = UI_CreateCompent( frmCooking, COMMAND_ONE_TYPE, "cmdItemBase1", 32, 32, 136, 326 )
UI_SetHint(cmdItemBase1,"Fairy")

imgCookingOver = UI_CreateCompent( frmCooking, IMAGE_TYPE, "imgCookingOver", 240, 10, 15, 411 )
UI_LoadImage( imgCookingOver, "texture/ui/Cooking2.tga", NORMAL, 14,10,100, 155 )

imgCookingFrame = UI_CreateCompent( frmCooking, IMAGE_TYPE, "imgCookingFrame", 217, 13, 43, 426 )
UI_LoadImage( imgCookingFrame, "texture/ui/Progress.tga", NORMAL, 157, 13, 0, 52 )

proCooking = UI_CreateCompent( frmCooking, PROGRESS_TYPE, "proCooking", 217, 13,43, 426 )
UI_LoadImage( proCooking, "texture/ui/Progress.tga", PROGRESS_PROGRESS, 157, 13, 0, 0 )

btnForgeYes = UI_CreateCompent( frmCooking, BUTTON_TYPE, "btnForgeYes", 57, 26, 40, 385)
UI_LoadButtonImage( btnForgeYes, "texture/ui/Progress.tga", 57, 26, 47, 78, FALSE )

labInput = UI_CreateCompent( frmCooking, LABELEX_TYPE, "labInput", 49, 14, 56, 392 )
UI_SetCaption( labInput, "Start")
UI_SetTextColor( labInput, COLOR_BLACK )
UI_SetLabelExFont( labInput, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnForgeNo = UI_CreateCompent( frmCooking, BUTTON_TYPE, "btnForgeNo", 57, 26, 207, 385)
UI_LoadButtonImage( btnForgeNo, "texture/ui/Progress.tga", 57, 26, 47, 78, FALSE )

labInput = UI_CreateCompent( frmCooking, LABELEX_TYPE, "labInput", 49, 14, 223, 392 )
UI_SetCaption( labInput, "Stop")
UI_SetTextColor( labInput, COLOR_BLACK )
UI_SetLabelExFont( labInput, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmCooking, BUTTON_TYPE, "btnClose", 14, 14, 285, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )



-----------------------------------------------------------------------
--   2007.1.17 by Leo
-----------------------------------------------------------------------
frmQuestion = UI_CreateForm( "frmQuestion", FALSE, 235, 317, 0, 0, TRUE, FALSE )
UI_ShowForm( frmQuestion, FALSE )
UI_AddFormToTemplete( frmQuestion, FORM_MAIN  )
UI_FormSetIsEscClose( frmQuestion, TRUE )
UI_SetIsDrag( frmQuestion,TRUE)
UI_SetFormStyle( frmQuestion,2)

--UI_FormSetHotKey( frmQuestion, ALT_KEY, HOTKEY_X )

--
imgMissonT = UI_CreateCompent( frmQuestion, IMAGE_TYPE, "imgMissonT", 235, 111, 0, 0 )
UI_LoadImage( imgMissonT, "texture/ui/answer.tga", NORMAL, 235, 111, 0, 0 )
imgMissonD = UI_CreateCompent( frmQuestion, IMAGE_TYPE, "imgMissonD", 235, 206, 0, 111 )
UI_LoadImage( imgMissonD, "texture/ui/question.tga", NORMAL, 235, 206, 0, 0 )

--title
labMisson = UI_CreateCompent( frmQuestion, LABELEX_TYPE, "labMisson", 47, 11, 3, 3 )
UI_SetCaption( labMisson, "Submit question" )
UI_SetTextColor( labMisson, COLOR_BLACK )
UI_SetLabelExFont( labMisson, DEFAULT_FONT, TRUE, COLOR_WHITE )

--
--
edtQuestionTitle= UI_CreateCompent(frmQuestion, EDIT_TYPE,"edtQuestionTitle",197, 21, 23, 43)
UI_SetEditMaxNum( edtQuestionTitle, 256)
UI_SetEditMaxNumVisible(edtQuestionTitle,30)

--
memCentent  = UI_CreateCompent( frmQuestion, MEMO_TYPE, "memCentent", 140, 210, 21, 95 )
UI_SetCaption( memCentent, "400" )
UI_SetTextColor( memCentent, COLOR_BLACK )
UI_SetLabelExFont( memCentent, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetMemoPageShowNum( memCentent, 9 )
UI_SetMemoMaxNumPerRow( memCentent, 31 )

	labNotice = UI_CreateCompent( frmQuestion, LABELEX_TYPE, "labNotice", 400, 150, 15 ,73 )
	UI_SetCaption( labNotice, "Please do not disclose your security code in the question.")
	UI_SetTextColor( labNotice, COLOR_RED)
	UI_SetLabelExFont( labNotice, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
--
	btnSubmit = UI_CreateCompent( frmQuestion, BUTTON_TYPE, "btnSubmit", 48, 19, 170, 287 )
	UI_LoadButtonImage( btnSubmit, "texture/ui/store8.tga", 48, 19, 0, 76, FALSE )

	labSubmit = UI_CreateCompent( frmQuestion, LABELEX_TYPE, "labSubmit", 400 , 150 , 179 , 289 )
	UI_SetCaption( labSubmit, "Submit")
	UI_SetTextColor( labSubmit, COLOR_BLACK )
	UI_SetLabelExFont( labSubmit, DEFAULT_FONT, TRUE, COLOR_WHITE )

--
btnClose = UI_CreateCompent( frmQuestion, BUTTON_TYPE, "btnClose", 14, 14, 215, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-----------------------------------------------------------------------
--   2007.1.17  by Leo
-----------------------------------------------------------------------
frmAnswer = UI_CreateForm( "frmAnswer", FALSE, 235, 317, 0, 0, TRUE, FALSE )
UI_ShowForm( frmAnswer, FALSE )
UI_AddFormToTemplete( frmAnswer, FORM_MAIN  )
UI_FormSetIsEscClose( frmAnswer, TRUE )
UI_SetIsDrag( frmAnswer,TRUE)
UI_SetFormStyle( frmAnswer,2)

--UI_FormSetHotKey( frmAnswer, ALT_KEY, HOTKEY_Z )

--
imgMissonT = UI_CreateCompent( frmAnswer, IMAGE_TYPE, "imgMissonT", 235, 111, 0, 0)
UI_LoadImage( imgMissonT, "texture/ui/answer.tga", NORMAL, 235, 111, 0,111 )
imgMissonD = UI_CreateCompent( frmAnswer, IMAGE_TYPE, "imgMissonD", 235, 206, 0, 111 )
UI_LoadImage( imgMissonD, "texture/ui/question.tga", NORMAL, 235, 206, 0, 0 )

--title
labMisson = UI_CreateCompent( frmAnswer, LABELEX_TYPE, "labMisson", 47, 11, 3, 3 )
UI_SetCaption( labMisson, "Check browser" )
UI_SetTextColor( labMisson, COLOR_BLACK )
UI_SetLabelExFont( labMisson, DEFAULT_FONT, TRUE, COLOR_WHITE )

--
memMiss  = UI_CreateCompent( frmAnswer, MEMO_TYPE, "memMiss", 197, 230, 20, 44 )
UI_SetCaption( memMiss, "400" )
UI_SetTextColor( memMiss, COLOR_BLACK )
UI_SetLabelExFont( memMiss, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetMemoPageShowNum( memMiss, 9 )
UI_SetMemoMaxNumPerRow( memMiss, 31 )

--
btnClose = UI_CreateCompent( frmAnswer, BUTTON_TYPE, "btnClose", 14, 14, 215, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-----------------------------------------------------------------------
--   2007.1.29  by Leo
-----------------------------------------------------------------------
frmAlphaMatte = UI_CreateForm( "frmAlphaMatte", FALSE, 800, 600, 0, 0, TRUE, FALSE )
UI_ShowForm( frmAlphaMatte, FALSE )
UI_AddFormToTemplete( frmAlphaMatte, FORM_MAIN  )
UI_FormSetIsEscClose( frmAlphaMatte, FALSE )
UI_SetIsDrag( frmAlphaMatte,FALSE)

-----------------------------------------------------------------------
--   2007.02.12  by Leo
-----------------------------------------------------------------------
frmStoreLoad = UI_CreateForm( "frmStoreLoad", FALSE, 200, 80, 0, 0, TRUE, FALSE )
UI_ShowForm( frmStoreLoad, FALSE )
UI_AddFormToTemplete( frmStoreLoad, FORM_MAIN  )
UI_FormSetIsEscClose( frmStoreLoad, TRUE )
UI_SetIsDrag( frmStoreLoad,FALSE)

proStoreLoad= UI_CreateCompent( frmStoreLoad, PROGRESS_TYPE, "proStoreLoad", 160, 13,20, 10 )
UI_LoadImage( proStoreLoad, "texture/ui/Progress.tga", PROGRESS_PROGRESS, 157, 13, 0, 0 )

labStoreLoad = UI_CreateCompent( frmStoreLoad, LABELEX_TYPE, "labStoreLoad", 400, 150, 0,30 )
UI_SetCaption( labStoreLoad, "Loading... ESC to Close.")
UI_SetTextColor( labStoreLoad, COLOR_BLACK )
UI_SetLabelExFont( labStoreLoad, BIGFONT, FALSE, COLOR_WHITE )

-----------------------------------------------------------------------
-- 
-----------------------------------------------------------------------
-- 
frmStoreDark = UI_CreateForm( "frmStoreDark", FALSE, 98, 36, 380, 507, TRUE, FALSE )
UI_ShowForm( frmStoreDark, TRUE )
UI_AddFormToTemplete( frmStoreDark, FORM_MAIN )
UI_SetFormStyleEx( frmStoreDark, 7, 0, 2 )
UI_FormSetIsEscClose( frmStoreDark, FALSE )

-- 
imgBack = UI_CreateCompent( frmStoreDark, IMAGE_TYPE, "imgBack", 1198, 768, -380, -675 )
UI_LoadImage( imgBack, "texture/ui/select4.tga", NORMAL, 1, 1, 232, 1 )

-- logo
imgLogo = UI_CreateCompent( frmStoreDark, IMAGE_TYPE, "imgLogo", 242, 115, -100, -410 )
--UI_LoadImage( imgLogo, "texture/ui/logot.tga", NORMAL, 242, 115, 0, 0 )


-----------------------------------------------------------------------
-- 
-----------------------------------------------------------------------
frmChurchSale = UI_CreateForm( "frmChurchSale", FALSE, 558, 291, 380, 507, TRUE, FALSE )
UI_ShowForm( frmChurchSale, TRUE )
UI_AddFormToTemplete( frmChurchSale, FORM_MAIN)
UI_FormSetIsEscClose( frmChurchSale, FALSE )
UI_SetIsDrag( frmChurchSale,TRUE)
UI_SetFormStyle(frmChurchSale,1)

--UI_FormSetHotKey( frmChurchSale, ALT_KEY, HOTKEY_Z )


-- 
imgBack01 = UI_CreateCompent( frmChurchSale, IMAGE_TYPE, "imgBack01", 256,256,0,0 )
UI_LoadImage( imgBack01, "texture/ui/church01.tga", NORMAL, 256, 256, 0, 0 )

imgBack02 = UI_CreateCompent( frmChurchSale, IMAGE_TYPE, "imgBack02", 256,256,256,0 )
UI_LoadImage( imgBack02, "texture/ui/church02.tga", NORMAL, 256, 256, 0, 0 )

imgBack03 = UI_CreateCompent( frmChurchSale, IMAGE_TYPE, "imgBack03", 46,256,512,0 )
UI_LoadImage( imgBack03, "texture/ui/church04.tga", NORMAL, 46, 256, 0, 0 )

imgBack04 = UI_CreateCompent( frmChurchSale, IMAGE_TYPE, "imgBack04", 256,35,0,256 )
UI_LoadImage( imgBack04, "texture/ui/church03.tga", NORMAL, 256, 35, 0, 0 )

imgBack05 = UI_CreateCompent( frmChurchSale, IMAGE_TYPE, "imgBack05", 256,35,256,256 )
UI_LoadImage( imgBack05, "texture/ui/church03.tga", NORMAL, 256, 35, 0, 74 )

imgBack06 = UI_CreateCompent( frmChurchSale, IMAGE_TYPE, "imgBack06", 46,35,512,256 )
UI_LoadImage( imgBack06, "texture/ui/church04.tga", NORMAL, 46, 35, 46, 0 )

btnPriceUp = UI_CreateCompent( frmChurchSale, BUTTON_TYPE, "btnPriceUp", 41, 19, 492, 118)
UI_LoadButtonImage( btnPriceUp, "texture/ui/church03.tga", 41, 19, 1, 55, TRUE )

--label
labInfoShow1 = UI_CreateCompent( frmChurchSale, LABELEX_TYPE, "labInfoShow1", 124, 16,24,122 )
UI_SetCaption( labInfoShow1, "Program writes here")
UI_SetTextColor( labInfoShow1, COLOR_BLACK )
UI_SetLabelExFont( labInfoShow1, DEFAULT_FONT, FALSE, COLOR_WHITE )

labInfoShow2 = UI_CreateCompent( frmChurchSale, LABELEX_TYPE, "labInfoShow2", 124, 16, 186,122 )
UI_SetCaption( labInfoShow2, "Program writes here")
UI_SetTextColor( labInfoShow2, COLOR_BLACK )
UI_SetLabelExFont( labInfoShow2, DEFAULT_FONT, FALSE, COLOR_WHITE )

labInfoShow3 = UI_CreateCompent( frmChurchSale, LABELEX_TYPE, "labInfoShow3", 98, 16,373,122  )
UI_SetCaption( labInfoShow3, "Program writes here")
UI_SetTextColor( labInfoShow3, COLOR_BLACK )
UI_SetLabelExFont( labInfoShow3, DEFAULT_FONT, FALSE, COLOR_WHITE )

labTittle = UI_CreateCompent( frmChurchSale, LABELEX_TYPE, "labTittle", 496,16,23,35 )
UI_SetCaption( labTittle, "Please make sure your inventory have two empty spaces")
UI_SetTextColor( labTittle, COLOR_BLACK )
UI_SetLabelExFont( labTittle, DEFAULT_FONT, FALSE, COLOR_WHITE )


--label
labTextShow = UI_CreateCompent( frmChurchSale, LABELEX_TYPE, "labTextShow", 496,16,23,202 )
UI_SetCaption( labTextShow, "Bid for renting Saturday 1st timing for Church usage is Every Monday 06:00pm-06:30 pm")
UI_SetTextColor( labTextShow, COLOR_BLACK )
UI_SetLabelExFont( labTextShow, DEFAULT_FONT, FALSE, COLOR_WHITE )

labTextShow = UI_CreateCompent( frmChurchSale, LABELEX_TYPE, "labTextShow", 496,16,23,216 )
UI_SetCaption( labTextShow, "Bid for renting Saturday 2nd timing for Church usage is Every Monday 06:40pm-07:10 pm")
UI_SetTextColor( labTextShow, COLOR_BLACK )
UI_SetLabelExFont( labTextShow, DEFAULT_FONT, FALSE, COLOR_WHITE )

labTextShow = UI_CreateCompent( frmChurchSale, LABELEX_TYPE, "labTextShow", 496,16,23,230 )
UI_SetCaption( labTextShow, "Bid for renting Sunday 1st timing for Church usage is Every Monday 07:20pm-07:50 pm")
UI_SetTextColor( labTextShow, COLOR_BLACK )
UI_SetLabelExFont( labTextShow, DEFAULT_FONT, FALSE, COLOR_WHITE )

labTextShow = UI_CreateCompent( frmChurchSale, LABELEX_TYPE, "labTextShow", 496,16,23,244 )
UI_SetCaption( labTextShow, "Bid for renting Sunday 2nd timing for Church usage is Every Monday 08:00pm-08:30 pm")
UI_SetTextColor( labTextShow, COLOR_BLACK )
UI_SetLabelExFont( labTextShow, DEFAULT_FONT, FALSE, COLOR_WHITE )

--
btnClose = UI_CreateCompent( frmChurchSale, BUTTON_TYPE, "btnClose", 14, 14, 538, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

--------------
-- Temp Bag --
--------------
frmTempBag = UI_CreateForm( "frmTempBag", FALSE, 200, 232, 0, 0, TRUE, FALSE )
UI_ShowForm( frmTempBag, FALSE )
UI_AddFormToTemplete( frmTempBag, FORM_MAIN )
UI_FormSetIsEscClose( frmTempBag, TRUE )
UI_SetIsDrag( frmTempBag, FALSE )
UI_SetFormStyle( frmTempBag,1)

imgBakTop = UI_CreateCompent( frmTempBag, IMAGE_TYPE, "imgBakTop", 200, 232, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/tempbag.tga", NORMAL, 198, 232, 0, 0 )

--Title
labTitle = UI_CreateCompent( frmTempBag, LABELEX_TYPE, "labTitle", 400, 150, 10, 5 )
UI_SetCaption( labTitle, "Temporary Bag")
UI_SetTextColor( labTitle, COLOR_BLACK )

UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmTempBag, BUTTON_TYPE, "btnClose", 14, 14, 178, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

grdTempBag = UI_CreateCompent( frmTempBag, GOODS_GRID_TYPE, "grdTempBag", 184, 305, 7, 36 ) 
UI_SetGridSpace( grdTempBag, 4, 4 )
UI_SetGridContent( grdTempBag, 4, 4 )
UI_SetGridUnitSize( grdTempBag, 32, 32 )
UI_SetMargin( grdTempBag, 21, 4, 0, 0 )
