------------------------------------
-- Player Item Trade | UI by aLex --
------------------------------------
frmPlayertrade = UI_CreateForm( "frmPlayertrade", FALSE, 198, 466, 240, 468, TRUE, FALSE )
--UI_FormSetHotKey( frmPlayertrade, ALT_KEY, HOTKEY_T )
UI_ShowForm( frmPlayertrade, TRUE )
UI_SetIsDrag( frmPlayertrade, TRUE )
UI_AddFormToTemplete( frmPlayertrade, FORM_MAIN )
UI_SetFormStyle( frmPlayertrade, 1 )
UI_FormSetIsEscClose( frmPlayertrade, FALSE )

imgBakTop = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgBakTop", 198, 466, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/corsairs/trade.png", NORMAL, 198, 466, 0, 0 )

btnClose = UI_CreateCompent( frmPlayertrade, BUTTON_TYPE, "btnClose", 14, 14, 177, 1 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

labTitle = UI_CreateCompent( frmPlayertrade, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_LoadImage( labTitle, "texture/ui/word.dds", NORMAL, 48, 13, 36, 29 )

labPlayertradeName = UI_CreateCompent( frmPlayertrade, LABELEX_TYPE, "labPlayertradeName", 95, 11, 90, 29 )
UI_SetCaption( labPlayertradeName, "WuwuwuwAhTatatata" )
UI_SetTextColor( labPlayertradeName, COLOR_BLACK )
UI_SetLabelExFont( labPlayertradeName, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Grid Other
grdBuy = UI_CreateCompent( frmPlayertrade, GOODS_GRID_TYPE, "grdBuy", 184, 121, 7, 46 ) 
UI_LoadImage( grdBuy,  "texture/ui/sure.dds", NORMAL, 184, 121, 0, 119 )
UI_SetGridSpace( grdBuy, 4, 4 )
UI_SetGridContent( grdBuy, 3, 4 )
UI_SetGridUnitSize( grdBuy, 32, 32 )
UI_SetMargin( grdBuy, 21, 10, 0, 3 )
UI_SetIsEnabled( grdBuy, FALSE )

-- Other Gold Label
-- imgOtherGold = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgOtherGold", 90, 13, 6, 172 + 8 )
-- UI_LoadImage( imgOtherGold, "texture/ui/SysBotton1.dds", NORMAL, 90, 13, 14, 240 )

-- Other Gold Box
imgOtherGoldBak = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgOtherGoldBak", 87, 18, 79, 169 + 8 )
UI_LoadImage( imgOtherGoldBak, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

-- Other Gold
labOtherGold = UI_CreateCompent( frmPlayertrade, LABELEX_TYPE, "labOtherGold", 47, 11, 88, 174 + 8 )
UI_SetCaption( labOtherGold, "0" )
UI_SetTextColor( labOtherGold, COLOR_WHITE )
UI_SetLabelExFont( labOtherGold, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Other Gold Icon
imgOtherGoldBak = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgOtherGoldBak", 26, 26, 167, 167 + 8)
UI_LoadImage( imgOtherGoldBak, "texture/ui/corsairs/trade.png", NORMAL, 26, 26, 202, 0 )

-- Other IMP Label
-- imgOtherIMP = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgOtherIMP", 90, 13, 6, 172 + 36 )
-- UI_LoadImage( imgOtherIMP, "texture/ui/SysBotton1.dds", NORMAL, 90, 13, 14, 240 )

-- Other IMP Box
imgOtherIMPBak = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgOtherIMPBak", 87, 18, 79, 169 + 36 )
UI_LoadImage( imgOtherIMPBak, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

-- Other IMP Label
labOtherIMP = UI_CreateCompent( frmPlayertrade, LABELEX_TYPE, "labOtherIMP", 47, 11, 88, 174 + 36 )
UI_SetCaption( labOtherIMP, "0" )
UI_SetTextColor( labOtherIMP, COLOR_WHITE )
UI_SetLabelExFont( labOtherIMP, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Other IMP Icon
imgOtherGoldBak = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgOtherGoldBak", 26, 26, 167, 167 + 36)
UI_LoadImage( imgOtherGoldBak, "texture/ui/corsairs/trade.png", NORMAL, 26, 26, 202, 26 )

-- Trade / Confirm Indicators
-- Trade Items Label
-- imgMineItem = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgMineItem", 90, 13, 6, 202 + 30 )
-- UI_LoadImage( imgMineItem, "texture/ui/SysBotton1.dds", NORMAL, 90, 13, 116, 212 )

-- Confirm Label
labOtherYes = UI_CreateCompent( frmPlayertrade, LABELEX_TYPE, "labOtherYes", 23, 11, 79, 203 + 30 )
UI_SetCaption( labOtherYes, "Confirm" )
UI_SetTextColor( labOtherYes, COLOR_BLACK )
UI_SetLabelExFont( labOtherYes, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Confirm Indicator
chkYes = UI_CreateCompent( frmPlayertrade, CHECK_TYPE, "chkYes", 11, 12, 124, 202 + 30)
UI_LoadImage( chkYes, "texture/ui/PublicC.tga", UNCHECKED, 11, 12, 227, 47 )
UI_LoadImage( chkYes, "texture/ui/PublicC.tga", CHECKED, 11, 12, 214, 47 )
UI_SetIsEnabled( chkYes, FALSE )

-- Trade Label
labOtherTrade = UI_CreateCompent( frmPlayertrade, LABELEX_TYPE, "labOtherTrade", 23, 11, 136, 203 + 30 )
UI_SetCaption( labOtherTrade, "Trade" )
UI_SetTextColor( labOtherTrade, COLOR_WHITE )
UI_SetLabelExFont( labOtherTrade, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Trade Indicator
chkTrade = UI_CreateCompent( frmPlayertrade, CHECK_TYPE, "chkTrade", 11, 12, 168, 202 + 30 )
UI_LoadImage( chkTrade, "texture/ui/PublicC.tga", UNCHECKED, 11, 12, 227, 47 )
UI_LoadImage( chkTrade, "texture/ui/PublicC.tga", CHECKED, 11, 12, 214, 47 )
UI_SetIsEnabled( chkTrade, FALSE )


-- Grid
grdSale = UI_CreateCompent( frmPlayertrade, GOODS_GRID_TYPE, "grdSale", 184, 121, 11 - 4, 220 + 36 ) 
UI_LoadImage( grdSale,  "texture/ui/sure.dds", NORMAL, 184, 121, 0, 119 )
UI_SetGridSpace( grdSale, 4, 4 )
UI_SetGridContent( grdSale, 3, 4 )
UI_SetGridUnitSize( grdSale, 32, 32 )
UI_SetMargin( grdSale, 21, 10, 0, 3 )

-- Gold Label
-- imgMineGold = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgMineGold", 90, 13, 6, 347 + 43 )
-- UI_LoadImage( imgMineGold, "texture/ui/SysBotton1.dds", NORMAL, 90, 13, 117, 226 )

-- Gold Box
imgMineGoldBak = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgMineGoldBak", 87, 18, 79, 344 + 43 )
UI_LoadImage( imgMineGoldBak, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

-- Gold
labSelfGold = UI_CreateCompent( frmPlayertrade, LABELEX_TYPE, "labSelfGold", 47, 11, 88, 349 + 43 )
UI_SetCaption( labSelfGold, "0" )
UI_SetTextColor( labSelfGold, COLOR_WHITE )
UI_SetLabelExFont( labSelfGold, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Gold Button
btnGold = UI_CreateCompent( frmPlayertrade, BUTTON_TYPE, "btnGold", 26, 26, 167, 341 + 43 )
UI_LoadButtonImage( btnGold, "texture/ui/corsairs/trade.png", 26, 26, 202, 0, TRUE )
UI_SetHint( btnGold, "Click here to input gold for trade" )

-- IMP Label
-- imgMineIMP = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgMineIMP", 90, 13, 6, 347 + 71 )
-- UI_LoadImage( imgMineIMP, "texture/ui/SysBotton1.dds", NORMAL, 90, 13, 117, 226 )

-- IMP Box
imgMineIMPBak = UI_CreateCompent( frmPlayertrade, IMAGE_TYPE, "imgMineIMPBak", 87, 18, 79, 344 + 71 )
UI_LoadImage( imgMineIMPBak, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

-- IMP
labSelfIMP = UI_CreateCompent( frmPlayertrade, LABELEX_TYPE, "labSelfIMP", 47, 11, 88, 349 + 71 )
UI_SetCaption( labSelfIMP, "0" )
UI_SetTextColor( labSelfIMP, COLOR_WHITE )
UI_SetLabelExFont( labSelfIMP, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- IMP Button
btnIMP = UI_CreateCompent( frmPlayertrade, BUTTON_TYPE, "btnIMP", 26, 26, 167, 341 + 71 )
UI_LoadButtonImage( btnIMP, "texture/ui/corsairs/trade.png", 26, 26, 202, 26, TRUE )
UI_SetHint( btnIMP, "Click here to input IMP for trade" )

-- Confirm Button
btnYes = UI_CreateCompent( frmPlayertrade, BUTTON_TYPE, "btnYes", 41, 19, 55, 368 + 72 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

-- Trade Button
btnTrade = UI_CreateCompent( frmPlayertrade, BUTTON_TYPE, "btnTrade", 41, 19, 102, 368 + 72 )
UI_LoadButtonImage( btnTrade, "texture/ui/PublicC.tga", 41, 19, 0, 40, TRUE )

-- Cancel Button
btnNo = UI_CreateCompent( frmPlayertrade, BUTTON_TYPE, "btnNo", 41, 19, 149, 368 + 72 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )



--------------------
-- Friend Request --
--------------------
frmAcceptFrnd = UI_CreateForm( "frmAcceptFrnd",  FALSE, 185, 101, 0, 0, FALSE, FALSE )
UI_LoadFormImage( frmAcceptFrnd, "texture/ui/sure.dds", 185, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmAcceptFrnd, FALSE )
UI_AddFormToTemplete( frmAcceptFrnd, FORM_MAIN )
UI_SetFormStyle( frmAcceptFrnd, 0 )
UI_SetIsDrag( frmAcceptFrnd, TRUE )
UI_FormSetIsEscClose( frmAcceptFrnd, FALSE )

btnClose = UI_CreateCompent( frmAcceptFrnd, BUTTON_TYPE, "btnClose", 14, 14, 164, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

labFrndName = UI_CreateCompent( frmAcceptFrnd, LABELEX_TYPE, "labFrndName", 23, 11, 44, 32 )
UI_SetCaption( labFrndName, "" )
UI_SetTextColor( labFrndName, COLOR_BLACK )
UI_SetLabelExFont( labFrndName, DEFAULT_FONT, TRUE, COLOR_WHITE )

labFrnd = UI_CreateCompent( frmAcceptFrnd, LABELEX_TYPE, "labFrnd", 23, 11, 44, 54 )
UI_SetCaption( labFrnd, "" )
UI_SetTextColor( labFrnd, COLOR_BLACK )
UI_SetLabelExFont( labFrnd, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmAcceptFrnd, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/SystemBotton3.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmAcceptFrnd, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )


--------------------------
-- Player Trade Request --
--------------------------
frmAcceptGroup = UI_CreateForm( "frmAcceptGroup",  FALSE, 185, 101, 0, 0, FALSE, FALSE )
UI_LoadFormImage( frmAcceptGroup, "texture/ui/sure.dds", 185, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmAcceptGroup, FALSE )
UI_AddFormToTemplete( frmAcceptGroup, FORM_MAIN )
UI_SetFormStyle( frmAcceptGroup, 0 )
UI_SetIsDrag( frmAcceptGroup, TRUE )
UI_FormSetIsEscClose( frmAcceptGroup, FALSE )
--UI_SetAlpha( frmAcceptGroup, 200 )
--UI_FormSetHotKey( frmAcceptGroup, ALT_KEY, HOTKEY_V )

btnClose = UI_CreateCompent( frmAcceptGroup, BUTTON_TYPE, "btnClose", 14, 14, 164, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

labGroupName = UI_CreateCompent( frmAcceptGroup, LABELEX_TYPE, "labGroupName", 23, 11, 44, 32 )
UI_SetCaption( labGroupName, "" )
UI_SetTextColor( labGroupName, COLOR_BLACK )
UI_SetLabelExFont( labGroupName, DEFAULT_FONT, TRUE, COLOR_WHITE )

labGroup = UI_CreateCompent( frmAcceptGroup, LABELEX_TYPE, "labGroup", 23, 11, 44, 54 )
UI_SetCaption( labGroup, "" )
UI_SetTextColor( labGroup, COLOR_BLACK )
UI_SetLabelExFont( labGroup, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmAcceptGroup, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/SystemBotton3.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmAcceptGroup, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )


--------------------
-- TeamMember4 UI --
--------------------
frmTeamMenber4 = UI_CreateForm( "frmTeamMenber4", FALSE, 166, 33, 6, 220, TRUE, FALSE )
--UI_FormSetHotKey( frmTeamMenber4, ALT_KEY, HOTKEY_U ) 

imgBackDropTeam4= UI_CreateCompent( frmTeamMenber4, IMAGE_TYPE, "imgBackDropTeam4", 40, 30, 3, 2 )
UI_LoadImage( imgBackDropTeam4, "texture/ui/Corsairs/npcBackDrop/0.tga", NORMAL, 55, 44, 0, 0 )

imgTeamMember4 = UI_CreateCompent( frmTeamMenber4, IMAGE_TYPE, "imgTeamMember4", 166, 33, 0, 0 )
UI_LoadImage( imgTeamMember4, "texture/ui/corsairs/new3.tga", NORMAL, 166, 33, 0, 84 )

--UI_LoadFormImage( frmTeamMenber1, "texture/ui/corsairs/new3.tga", 166, 33, 0, 84, "", 0, 0 )
--UI_LoadFormImage( frmTeamMenber4, "texture/ui/corsairs/new3.tga", 166, 33, 0, 84, "", 0, 0 )
UI_ShowForm( frmTeamMenber4, FALSE )
UI_SetIsDrag( frmTeamMenber4, FALSE )
UI_AddFormToTemplete( frmTeamMenber4, FORM_MAIN )
UI_FormSetIsEscClose( frmTeamMenber4, FALSE )

labMenber4Name = UI_CreateCompent( frmTeamMenber4, LABELEX_TYPE, "labMenber4Name", 1, 1, 61, 5 )
UI_SetCaption( labMenber4Name, "" )
UI_SetTextColor( labMenber4Name, COLOR_WHITE )
UI_SetLabelExFont( labMenber4Name, DEFAULT_FONT, TRUE, COLOR_BLACK )

proTeamMenber4HP = UI_CreateCompent( frmTeamMenber4, PROGRESS_TYPE, "proTeamMenber4HP", 111, 8, 51, 21 )
UI_LoadScaleImage( proTeamMenber4HP, "texture/ui/corsairs/new3.tga", PROGRESS_PROGRESS, 111, 8, 0, 129, 1.0, 1.0 )
UI_SetHint( proTeamMenber4HP, "HP" )
UI_SetProgressHintStyle( proTeamMenber4HP, PROGRESS_HINT_NUM )

proTeamMenber4SP = UI_CreateCompent( frmTeamMenber4, PROGRESS_TYPE, "proTeamMenber4SP", 50, 4, 1, 59 )
--UI_LoadScaleImage( proTeamMenber4SP, "texture/ui/StartF.dds", PROGRESS_PROGRESS, 50, 4, 124, 16, 1.0, 1.0 )
UI_SetHint( proTeamMenber4SP, "SP" )
UI_SetProgressHintStyle( proTeamMenber4SP, PROGRESS_HINT_NUM )

d3dHead = UI_CreateCompent( frmTeamMenber4, UI3D_COMPENT_TYPE, "d3dHead", 24, 24, 6, 5 )

d3dDown = UI_CreateCompent( frmTeamMenber4, UI3D_COMPENT_TYPE, "d3dDown", 166, 33, 0, 0 )

labFLv = UI_CreateCompent( frmTeamMenber4, LABELEX_TYPE, "labFLv", 14, 9, 36, 6 )
UI_SetTextColor( labFLv, COLOR_WHITE )
UI_SetLabelExFont( labFLv, DEFAULT_FONT, TRUE, COLOR_BLACK )
UI_SetCaption( labFLv, "99" )

imgobj = UI_CreateCompent( frmTeamMenber4, IMAGE_TYPE, "imgobj", 9, 9, 23, 21 )
UI_SetMaxImage( imgobj, 19 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 0, 9, 9, 0, 0 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 1, 9, 9, 0, 9 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 2, 9, 9, 0, 18 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 3, 9, 9, 0, 27 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 4, 9, 9, 0, 36 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 5, 9, 9, 0, 45 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 6, 9, 9, 0, 54 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 7, 9, 9, 0, 63 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 8, 9, 9, 0, 72 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 9, 9, 9, 0, 81 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 10, 9, 9, 0, 90 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 11, 9, 9, 0, 99 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 12, 9, 9, 0, 108 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 13, 9, 9, 0, 117 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 14, 9, 9, 9, 0 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 15, 9, 9, 9, 9 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 16, 9, 9, 9, 18 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 17, 9, 9, 9, 27 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 18, 9, 9, 9, 36 )

imgLeader = UI_CreateCompent( frmTeamMenber4, IMAGE_TYPE, "imgLeader", 9, 9, 7, 5 )
UI_LoadImage( imgLeader, "texture/ui/corsairs/new3.tga", NORMAL, 9, 9, 0, 161 )


-----------------------------
-- Player Right Click Menu -- 
-----------------------------
mainMouseRight = UI_CreateCompent( frmMain800, MENU_TYPE, "mainMouseRight", 90, 97, 240, 468 )
UI_MenuLoadImage( mainMouseRight, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( mainMouseRight, 10, 5, 5, 5 )

UI_MenuLoadSelect( mainMouseRight, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )

UI_MenuAddText( mainMouseRight, "Trade" )
UI_MenuAddText( mainMouseRight, "Add Friend" )
UI_MenuAddText( mainMouseRight, "Party Invite" )
UI_MenuAddText( mainMouseRight, "Leave Party" )
UI_MenuAddText( mainMouseRight, "PM" )
UI_MenuAddText( mainMouseRight, "Cargo Trade" )
UI_MenuAddText( mainMouseRight, "View Stall" )
UI_MenuAddText( mainMouseRight, "1 VS 1" )
UI_MenuAddText( mainMouseRight, "M VS M" )
UI_MenuAddText( mainMouseRight, "Mentor" )
UI_MenuAddText( mainMouseRight, "Disciple" )
--UI_MenuAddText( mainMouseRight, "Check Eq" )
UI_MenuAddText( mainMouseRight, "Block" )
UI_MenuAddText( mainMouseRight, "Unblock" )
UI_MenuAddText( mainMouseRight, "Follow")

UI_MenuAddText( mainMouseRight, "Manage" )
UI_MenuAddText( mainMouseRight, "View Info") -- add by Mdrst May 2020 FPO Alpha
UI_MenuAddText( mainMouseRight, "View Drops") -- add by Mdrst

--------------------
-- TeamMember3 UI --
--------------------
frmTeamMenber3 = UI_CreateForm( "frmTeamMenber3", FALSE, 166, 33, 6, 180, TRUE, FALSE )
--UI_FormSetHotKey( frmTeamMenber3, ALT_KEY, HOTKEY_Y ) 
--UI_LoadFormImage( frmTeamMenber3, "texture/ui/corsairs/new3.tga", 166, 33, 0, 84, "", 0, 0 )

imgBackDropTeam3 = UI_CreateCompent( frmTeamMenber3, IMAGE_TYPE, "imgBackDropTeam3", 40, 30, 3, 2 )
UI_LoadImage( imgBackDropTeam3, "texture/ui/Corsairs/npcBackDrop/0.tga", NORMAL, 55, 44, 0, 0 )

imgTeamMember3 = UI_CreateCompent( frmTeamMenber3, IMAGE_TYPE, "imgTeamMember3", 166, 33, 0, 0 )
UI_LoadImage( imgTeamMember3, "texture/ui/corsairs/new3.tga", NORMAL, 166, 33, 0, 84 )


UI_ShowForm( frmTeamMenber3, FALSE )
UI_SetIsDrag( frmTeamMenber3, FALSE )
UI_AddFormToTemplete( frmTeamMenber3, FORM_MAIN )
UI_FormSetIsEscClose( frmTeamMenber3, FALSE )

labMenber3Name = UI_CreateCompent( frmTeamMenber3, LABELEX_TYPE, "labMenber3Name", 1, 1, 61, 5 )
UI_SetCaption( labMenber3Name, "" )
UI_SetTextColor( labMenber3Name, COLOR_WHITE )
UI_SetLabelExFont( labMenber3Name, DEFAULT_FONT, TRUE, COLOR_BLACK )

proTeamMenber3HP = UI_CreateCompent( frmTeamMenber3, PROGRESS_TYPE, "proTeamMenber3HP", 111, 8, 51, 21 )
UI_LoadScaleImage( proTeamMenber3HP, "texture/ui/corsairs/new3.tga", PROGRESS_PROGRESS, 111, 8, 0, 129, 1.0, 1.0 )
UI_SetHint( proTeamMenber3HP, "HP" )
UI_SetProgressHintStyle( proTeamMenber3HP, PROGRESS_HINT_NUM )

proTeamMenber3SP = UI_CreateCompent( frmTeamMenber3, PROGRESS_TYPE, "proTeamMenber3SP", 50, 4, 1, 59 )
--UI_LoadScaleImage( proTeamMenber3SP, "texture/ui/StartF.dds", PROGRESS_PROGRESS, 50, 4, 124, 16, 1.0, 1.0 )
UI_SetHint( proTeamMenber3SP, "SP" )
UI_SetProgressHintStyle( proTeamMenber3SP, PROGRESS_HINT_NUM )

d3dHead = UI_CreateCompent( frmTeamMenber3, UI3D_COMPENT_TYPE, "d3dHead", 24, 24, 6, 5 )

d3dDown = UI_CreateCompent( frmTeamMenber3, UI3D_COMPENT_TYPE, "d3dDown", 166, 33, 0, 0 )

labFLv = UI_CreateCompent( frmTeamMenber3, LABELEX_TYPE, "labFLv", 14, 9, 36, 6 )
UI_SetTextColor( labFLv, COLOR_WHITE )
UI_SetLabelExFont( labFLv, DEFAULT_FONT, TRUE, COLOR_BLACK )
UI_SetCaption( labFLv, "99" )

imgobj = UI_CreateCompent( frmTeamMenber3, IMAGE_TYPE, "imgobj", 9, 9, 23, 21 )
UI_SetMaxImage( imgobj, 19 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 0, 9, 9, 0, 0 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 1, 9, 9, 0, 9 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 2, 9, 9, 0, 18 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 3, 9, 9, 0, 27 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 4, 9, 9, 0, 36 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 5, 9, 9, 0, 45 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 6, 9, 9, 0, 54 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 7, 9, 9, 0, 63 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 8, 9, 9, 0, 72 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 9, 9, 9, 0, 81 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 10, 9, 9, 0, 90 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 11, 9, 9, 0, 99 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 12, 9, 9, 0, 108 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 13, 9, 9, 0, 117 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 14, 9, 9, 9, 0 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 15, 9, 9, 9, 9 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 16, 9, 9, 9, 18 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 17, 9, 9, 9, 27 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 18, 9, 9, 9, 36 )

imgLeader = UI_CreateCompent( frmTeamMenber3, IMAGE_TYPE, "imgLeader", 9, 9, 7, 5 )
UI_LoadImage( imgLeader, "texture/ui/corsairs/new3.tga", NORMAL, 9, 9, 0, 161 )


--------------------
-- TeamMember2 UI --
--------------------
frmTeamMenber2 = UI_CreateForm( "frmTeamMenber2", FALSE, 166, 33, 6, 140, TRUE, FALSE )
--UI_FormSetHotKey( frmTeamMenber2, ALT_KEY, HOTKEY_T ) 

imgBackDropTeam2 = UI_CreateCompent( frmTeamMenber2, IMAGE_TYPE, "imgBackDropTeam2", 40, 30, 3, 2 )
UI_LoadImage( imgBackDropTeam2, "texture/ui/Corsairs/npcBackDrop/0.tga", NORMAL, 55, 44, 0, 0 )

imgTeamMember2 = UI_CreateCompent( frmTeamMenber2, IMAGE_TYPE, "imgTeamMember2", 166, 33, 0, 0 )
UI_LoadImage( imgTeamMember2, "texture/ui/corsairs/new3.tga", NORMAL, 166, 33, 0, 84 )

UI_ShowForm( frmTeamMenber2, FALSE )
UI_SetIsDrag( frmTeamMenber2, FALSE )
UI_AddFormToTemplete( frmTeamMenber2, FORM_MAIN )
UI_FormSetIsEscClose( frmTeamMenber2, FALSE )

labMenber2Name = UI_CreateCompent( frmTeamMenber2, LABELEX_TYPE, "labMenber2Name", 1, 1, 61, 5 )
UI_SetCaption( labMenber2Name, "" )
UI_SetTextColor( labMenber2Name, COLOR_WHITE )
UI_SetLabelExFont( labMenber2Name, DEFAULT_FONT, TRUE, COLOR_BLACK )

proTeamMenber2HP = UI_CreateCompent( frmTeamMenber2, PROGRESS_TYPE, "proTeamMenber2HP", 111, 8, 51, 21 )
UI_LoadScaleImage( proTeamMenber2HP, "texture/ui/corsairs/new3.tga", PROGRESS_PROGRESS, 111, 8, 0, 129, 1.0, 1.0 )
UI_SetHint( proTeamMenber2HP, "HP" )
UI_SetProgressHintStyle( proTeamMenber2HP, PROGRESS_HINT_NUM )

proTeamMenber2SP = UI_CreateCompent( frmTeamMenber2, PROGRESS_TYPE, "proTeamMenber2SP", 50, 4, 1, 59 )
--UI_LoadScaleImage( proTeamMenber2SP, "texture/ui/StartF.dds", PROGRESS_PROGRESS, 50, 4, 124, 16, 1.0, 1.0 )
UI_SetHint( proTeamMenber2SP, "SP" )
UI_SetProgressHintStyle( proTeamMenber2SP, PROGRESS_HINT_NUM )

d3dHead = UI_CreateCompent( frmTeamMenber2, UI3D_COMPENT_TYPE, "d3dHead", 24, 24, 6, 5 )

d3dDown = UI_CreateCompent( frmTeamMenber2, UI3D_COMPENT_TYPE, "d3dDown", 166, 33, 0, 0 )

labFLv = UI_CreateCompent( frmTeamMenber2, LABELEX_TYPE, "labFLv", 14, 9, 36, 6 )
UI_SetTextColor( labFLv, COLOR_WHITE )
UI_SetLabelExFont( labFLv, DEFAULT_FONT, TRUE, COLOR_BLACK )
UI_SetCaption( labFLv, "99" )

imgobj = UI_CreateCompent( frmTeamMenber2, IMAGE_TYPE, "imgobj", 9, 9, 23, 21 )
UI_SetMaxImage( imgobj, 19 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 0, 9, 9, 0, 0 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 1, 9, 9, 0, 9 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 2, 9, 9, 0, 18 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 3, 9, 9, 0, 27 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 4, 9, 9, 0, 36 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 5, 9, 9, 0, 45 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 6, 9, 9, 0, 54 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 7, 9, 9, 0, 63 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 8, 9, 9, 0, 72 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 9, 9, 9, 0, 81 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 10, 9, 9, 0, 90 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 11, 9, 9, 0, 99 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 12, 9, 9, 0, 108 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 13, 9, 9, 0, 117 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 14, 9, 9, 9, 0 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 15, 9, 9, 9, 9 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 16, 9, 9, 9, 18 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 17, 9, 9, 9, 27 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 18, 9, 9, 9, 36 )

imgLeader = UI_CreateCompent( frmTeamMenber2, IMAGE_TYPE, "imgLeader", 9, 9, 7, 5 )
UI_LoadImage( imgLeader, "texture/ui/corsairs/new3.tga", NORMAL, 9, 9, 0, 161 )

--------------------
-- TeamMember1 UI --
--------------------
frmTeamMenber1 = UI_CreateForm( "frmTeamMenber1", FALSE, 166, 33, 6, 100, TRUE, FALSE )
UI_FormSetHotKey( frmTeamMenber1, ALT_KEY, HOTKEY_P ) 

imgBackDropTeam1 = UI_CreateCompent( frmTeamMenber1, IMAGE_TYPE, "imgBackDropTeam1", 40, 30, 3, 2 )
UI_LoadImage( imgBackDropTeam1, "texture/ui/Corsairs/npcBackDrop/0.tga", NORMAL, 55, 44, 0, 0 )

imgTeamMember1 = UI_CreateCompent( frmTeamMenber1, IMAGE_TYPE, "imgTeamMember1", 166, 33, 0, 0 )
UI_LoadImage( imgTeamMember1, "texture/ui/corsairs/new3.tga", NORMAL, 166, 33, 0, 84 )

--UI_LoadFormImage( frmTeamMenber1, "texture/ui/corsairs/new3.tga", 166, 33, 0, 84, "", 0, 0 )
UI_ShowForm( frmTeamMenber1, FALSE )
UI_SetIsDrag( frmTeamMenber1, FALSE )
UI_AddFormToTemplete( frmTeamMenber1, FORM_MAIN )
UI_FormSetIsEscClose( frmTeamMenber1, FALSE )

labMenber1Name = UI_CreateCompent( frmTeamMenber1, LABELEX_TYPE, "labMenber1Name", 1, 1, 61, 5 )
UI_SetCaption( labMenber1Name, "" )
UI_SetTextColor( labMenber1Name, COLOR_WHITE )
UI_SetLabelExFont( labMenber1Name, DEFAULT_FONT, TRUE, COLOR_BLACK )

proTeamMenber1HP = UI_CreateCompent( frmTeamMenber1, PROGRESS_TYPE, "proTeamMenber1HP", 111, 8, 51, 21 )
UI_LoadScaleImage( proTeamMenber1HP, "texture/ui/corsairs/new3.tga", PROGRESS_PROGRESS, 111, 8, 0, 129, 1.0, 1.0 )
UI_SetHint( proTeamMenber1HP, "HP" )
UI_SetProgressHintStyle( proTeamMenber1HP, PROGRESS_HINT_NUM )

proTeamMenber1SP = UI_CreateCompent( frmTeamMenber1, PROGRESS_TYPE, "proTeamMenber1SP", 50, 4, 1, 59 )
--UI_LoadScaleImage( proTeamMenber1SP, "texture/ui/StartF.dds", PROGRESS_PROGRESS, 50, 4, 124, 16, 1.0, 1.0 )
UI_SetHint( proTeamMenber1SP, "SP" )
UI_SetProgressHintStyle( proTeamMenber1SP, PROGRESS_HINT_NUM )

d3dHead = UI_CreateCompent( frmTeamMenber1, UI3D_COMPENT_TYPE, "d3dHead", 24, 24, 6, 5 )

d3dDown = UI_CreateCompent( frmTeamMenber1, UI3D_COMPENT_TYPE, "d3dDown", 166, 33, 0, 0 )

labFLv = UI_CreateCompent( frmTeamMenber1, LABELEX_TYPE, "labFLv", 14, 9, 36, 6 )
UI_SetTextColor( labFLv, COLOR_WHITE )
UI_SetLabelExFont( labFLv, DEFAULT_FONT, TRUE, COLOR_BLACK )
UI_SetCaption( labFLv, "99" )

imgobj = UI_CreateCompent( frmTeamMenber1, IMAGE_TYPE, "imgobj", 9, 9, 23, 21 )
UI_SetMaxImage( imgobj, 19 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 0, 9, 9, 0, 0 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 1, 9, 9, 0, 9 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 2, 9, 9, 0, 18 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 3, 9, 9, 0, 27 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 4, 9, 9, 0, 36 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 5, 9, 9, 0, 45 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 6, 9, 9, 0, 54 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 7, 9, 9, 0, 63 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 8, 9, 9, 0, 72 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 9, 9, 9, 0, 81 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 10, 9, 9, 0, 90 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 11, 9, 9, 0, 99 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 12, 9, 9, 0, 108 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 13, 9, 9, 0, 117 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 14, 9, 9, 9, 0 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 15, 9, 9, 9, 9 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 16, 9, 9, 9, 18 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 17, 9, 9, 9, 27 )
--UI_LoadImage( imgobj, "texture/ui/obj1.tga", 18, 9, 9, 9, 36 )

imgLeader = UI_CreateCompent( frmTeamMenber1, IMAGE_TYPE, "imgLeader", 9, 9, 7, 5 )
UI_LoadImage( imgLeader, "texture/ui/corsairs/new3.tga", NORMAL, 9, 9, 0, 161 )

---------------
-- Revive ui --
---------------
frmRelive = UI_CreateForm( "frmRelive",  FALSE, 185, 101, 397, 500, TRUE, FALSE )
UI_LoadFormImage( frmRelive, "texture/ui/sure.dds", 185, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmRelive, FALSE )
UI_AddFormToTemplete( frmRelive, FORM_MAIN )
UI_SetFormStyle( frmRelive, 0 )
UI_SetIsDrag( frmRelive, TRUE )
 UI_FormSetIsEscClose( frmRelive, FALSE )
--UI_SetAlpha( frmRelive, 200 )
--UI_FormSetHotKey( frmRelive, ALT_KEY, HOTKEY_X ) 

labReCity = UI_CreateCompent( frmRelive, LABELEX_TYPE, "labReCity", 1, 1, 38, 40 )
UI_SetCaption( labReCity, "Do you want to Revive in town" )
UI_SetTextColor( labReCity, COLOR_BLACK)
UI_SetLabelExFont( labReCity, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTitle = UI_CreateCompent( frmRelive, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Revive")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnReCity = UI_CreateCompent( frmRelive, BUTTON_TYPE, "btnReCity", 41, 19, 74, 78 )
UI_LoadButtonImage( btnReCity, "texture/ui/SystemBotton3.tga", 41, 19, 169, 39, FALSE )

-----------------------------------------------------------------------
-- revive ui
-----------------------------------------------------------------------
frmTime = UI_CreateForm( "frmTime",  FALSE, 185, 101, 397, 500, TRUE, FALSE )
UI_LoadFormImage( frmTime, "texture/ui/sure.dds", 185, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmTime, FALSE )
UI_AddFormToTemplete( frmTime, FORM_MAIN )
UI_SetFormStyle( frmTime, 0 )
UI_SetIsDrag( frmTime, TRUE )
--UI_SetAlpha( frmTime, 200 )
--UI_FormSetHotKey( frmTime, ALT_KEY, HOTKEY_X ) 

-- Query result display
labTime = UI_CreateCompent( frmTime, LABELEX_TYPE, "labTime", 1, 1, 58, 40 )
--UI_SetCaption( labTime, "Do you want to Revive in town" )
UI_SetTextColor( labTime, COLOR_BLACK)
UI_SetLabelExFont( labTime, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Title label
imgTime = UI_CreateCompent( frmTime, IMAGE_TYPE, "imgTime", 36, 13, 8, 4 )
UI_LoadImage( imgTime, "texture/ui/Word.dds", NORMAL, 36, 13, 211, 161 )

-- Cancel button
btnNo = UI_CreateCompent( frmTime, BUTTON_TYPE, "btnNo", 41, 19, 74, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )


------------------
-- Player Stall --
------------------
frmBooth = UI_CreateForm( "frmBooth", FALSE, 288, 298, 100, 100, TRUE, FALSE )
UI_ShowForm( frmBooth, FALSE )
UI_AddFormToTemplete( frmBooth, FORM_MAIN )
--UI_FormSetHotKey( frmBooth, ALT_KEY, HOTKEY_T )
UI_SetIsDrag( frmBooth, TRUE )
UI_FormSetIsEscClose( frmFound, FALSE )

imgBak = UI_CreateCompent( frmBooth, IMAGE_TYPE, "imgBak", 256, 256, 0, 0 )
UI_LoadImage( imgBak, "texture/ui/baitan1.tga", NORMAL, 256, 256, 0, 0 )

imgBak1 = UI_CreateCompent( frmBooth, IMAGE_TYPE, "imgBak1", 32, 256, 256, 0 )
UI_LoadImage( imgBak1, "texture/ui/baitan2.tga", NORMAL, 32, 256, 0, 0 )

imgBak2 = UI_CreateCompent( frmBooth, IMAGE_TYPE, "imgBak2", 256, 34, 0, 256 )
UI_LoadImage( imgBak2, "texture/ui/baitan3.tga", NORMAL, 256, 34, 0, 0 )

imgBak3 = UI_CreateCompent( frmBooth, IMAGE_TYPE, "imgBak3", 32, 34, 256, 256 )
UI_LoadImage( imgBak3, "texture/ui/baitan2.tga", NORMAL, 32, 34, 41, 0 )

btnClose = UI_CreateCompent( frmBooth, BUTTON_TYPE, "btnClose", 14, 14, 271, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

lblOwnerName = UI_CreateCompent( frmBooth, LABELEX_TYPE, "lblOwnerName", 94, 31, 111, 23 )
UI_SetCaption( lblOwnerName, "" )
UI_SetTextColor( lblOwnerName, COLOR_BLACK )
UI_SetLabelExFont( lblOwnerName, DEFAULT_FONT, TRUE, COLOR_WHITE )

edtBoothName = UI_CreateCompent( frmBooth, EDIT_TYPE, "edtBoothName", 264, 13, 54, 44 )
UI_SetEditMaxNum( edtBoothName, 32 )

grdBoothItem = UI_CreateCompent( frmBooth, GOODS_GRID_TYPE, "grdBoothItem", 267, 178, 11, 75 ) 
UI_SetGridSpace( grdBoothItem, 1, 1 )
UI_SetGridContent( grdBoothItem, 4, 3 )
UI_SetGridUnitSize( grdBoothItem, 87, 84 )
UI_SetMargin( grdBoothItem, 0, 0, 0, 0 )
UI_GoodGridLoadUnitImage( grdBoothItem, "texture/ui/baitan2.tga", 87, 84, 172, 173 )

scrollid = UI_GetScroll( grdBoothItem)
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

btnSetupBooth = UI_CreateCompent( frmBooth, BUTTON_TYPE, "btnSetupBooth", 41, 19, 119, 269 )
UI_LoadButtonImage( btnSetupBooth, "texture/ui/SystemBotton3.tga", 41, 19, 0, 38, TRUE )

btnPullStakes = UI_CreateCompent( frmBooth, BUTTON_TYPE, "btnPullStakes", 41, 19, 119, 269 )
UI_LoadButtonImage( btnPullStakes, "texture/ui/SystemBotton3.tga", 41, 19, 0, 57, TRUE )

----------------------
-- Hairstlylist NPC --
----------------------
frmHead = UI_CreateForm( "frmHead", FALSE, 454, 373, 100, 100, TRUE, FALSE )
UI_ShowForm( frmHead, FALSE )
UI_AddFormToTemplete( frmHead, FORM_MAIN )
--UI_FormSetHotKey( frmHead, ALT_KEY, HOTKEY_T )
UI_FormSetIsEscClose( frmHead, FALSE )

imgFoundTop = UI_CreateCompent( frmHead, IMAGE_TYPE, "imgFoundTop", 256, 256, 0, 0 )
UI_LoadImage( imgFoundTop, "texture/ui/head1.tga", NORMAL, 256, 256, 0, 0 )

imgFoundDown = UI_CreateCompent( frmHead, IMAGE_TYPE, "imgFoundDown", 256, 117, 0, 256 )
UI_LoadImage( imgFoundDown, "texture/ui/head3.tga", NORMAL, 256, 117, 0, 0 )

imgFoundTopR = UI_CreateCompent( frmHead, IMAGE_TYPE, "imgFoundTopR", 198, 256, 256, 0 )
UI_LoadImage( imgFoundTopR, "texture/ui/head2.tga", NORMAL, 198, 256, 0, 0 )

imgFoundDownR = UI_CreateCompent( frmHead, IMAGE_TYPE, "imgFoundDownR", 198, 117, 256, 256 )
UI_LoadImage( imgFoundDownR, "texture/ui/head3.tga", NORMAL, 198, 117, 0, 117 )

labHairColor = UI_CreateCompent( frmHead, LABELEX_TYPE, "labHairColor", 61, 12, 330, 200 )
UI_SetCaption( labHairColor, "Long Hair" )
UI_SetTextColor( labHairColor, COLOR_BLACK )
UI_SetLabelExFont( labHairColor, DEFAULT_FONT, TRUE, COLOR_WHITE )

labHair = UI_CreateCompent( frmHead, LABELEX_TYPE, "labHair", 61, 12, 330, 243 )
UI_SetCaption( labHair, "1 1 Golden" )
UI_SetTextColor( labHair, COLOR_BLACK )
UI_SetLabelExFont( labHair, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnLeftColor = UI_CreateCompent( frmHead, BUTTON_TYPE, "btnLeftColor", 40, 24, 283, 191 )
UI_LoadButtonImage( btnLeftColor, "texture/ui/select4.tga", 40, 24, 110, 24, TRUE )

btnLeftHair = UI_CreateCompent( frmHead, BUTTON_TYPE, "btnLeftHair", 40, 24, 283, 234 )
UI_LoadButtonImage( btnLeftHair, "texture/ui/select4.tga", 40, 24, 110, 24, TRUE )

btnRightColor = UI_CreateCompent( frmHead, BUTTON_TYPE, "btnRightColor", 40, 24, 392, 191 )
UI_LoadButtonImage( btnRightColor, "texture/ui/select4.tga", 40, 24, 110, 0, TRUE )

btnRightHair = UI_CreateCompent( frmHead, BUTTON_TYPE, "btnRightHair", 40, 24, 392, 234 )
UI_LoadButtonImage( btnRightHair, "texture/ui/select4.tga", 40, 24, 110, 0, TRUE )

ui3dCreateCha = UI_CreateCompent( frmHead, UI3D_COMPENT, "ui3dCreateCha", 160, 300, 52, 35 )

btnLeft3d = UI_CreateCompent( frmHead, BUTTON_TYPE, "btnLeft3d", 32, 38, 28, 291 )
UI_LoadButtonImage( btnLeft3d, "texture/ui/select4.tga", 32, 38, 0, 38, TRUE )

btnRight3d = UI_CreateCompent( frmHead, BUTTON_TYPE, "btnRight3d", 32, 38, 204, 291 )
UI_LoadButtonImage( btnRight3d, "texture/ui/select4.tga",  32, 38, 0, 0, TRUE )

labHairMoney = UI_CreateCompent( frmHead, LABELEX_TYPE, "labHairMoney", 65, 12, 335, 287 )
UI_SetCaption( labHairMoney, "99999999" )
UI_SetTextColor( labHairMoney, COLOR_BLACK )
UI_SetLabelExFont( labHairMoney, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmHead, BUTTON_TYPE, "btnYes", 41, 19, 293, 321 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetButtonModalResult( btnYes, BUTTON_YES )

btnNo = UI_CreateCompent( frmHead, BUTTON_TYPE, "btnNo", 41, 19, 379, 321 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_NO )

cmdHead1 = UI_CreateCompent( frmHead, COMMAND_ONE_TYPE, "cmdHead1", 32, 32, 0, 0 )
UI_SetIsShow( cmdHead1, FALSE )

UI_CreateCompent( frmHead, COMMAND_ONE_TYPE, "cmdHead2", 32, 32, 298, 78 )
UI_CreateCompent( frmHead, COMMAND_ONE_TYPE, "cmdHead3", 32, 32, 344, 78 )
UI_CreateCompent( frmHead, COMMAND_ONE_TYPE, "cmdHead4", 32, 32, 390, 78 )

labTicket = UI_CreateCompent( frmHead, LABELEX_TYPE, "labTicket", 65, 12, 298, 138 )
UI_SetCaption( labTicket, "Hairstyling Voucher required" )
UI_SetTextColor( labTicket, COLOR_BLACK )
UI_SetLabelExFont( labTicket, DEFAULT_FONT, TRUE, COLOR_WHITE )

--------------
-- Team PK  --
--------------
frmTeamPK = UI_CreateForm( "frmTeamPK", FALSE, 376, 431, 159, 468, TRUE, FALSE )
--UI_FormSetHotKey( frmTeamPK, ALT_KEY, HOTKEY_Z )
UI_ShowForm( frmTeamPK, TRUE )
UI_SetIsDrag( frmTeamPK, TRUE )
UI_AddFormToTemplete( frmTeamPK, FORM_MAIN )
UI_SetFormStyle( frmTeamPK, 1 )--- top-left
UI_FormSetIsEscClose( frmTeamPK, TRUE )

imgBakTop1 = UI_CreateCompent( frmTeamPK, IMAGE_TYPE, "imgBakTop1", 256, 256, 0, 0 )
UI_LoadImage( imgBakTop1, "texture/ui/PK1.tga", NORMAL, 256, 256, 0, 0 )

imgBakTop2 = UI_CreateCompent( frmTeamPK, IMAGE_TYPE, "imgBakTop2", 120, 256, 256, 0 )
UI_LoadImage( imgBakTop2, "texture/ui/PK2.tga", NORMAL, 120, 256, 0, 0 )

--imgBakTop3 = UI_CreateCompent( frmTeamPK, IMAGE_TYPE, "imgBakTop3", 46, 256, 512, 0 )
--UI_LoadImage( imgBakTop3, "texture/ui/PK4.tga", NORMAL, 46, 256, 0, 0 )

imgBakDown1 = UI_CreateCompent( frmTeamPK, IMAGE_TYPE, "imgBakDown1", 256, 175, 0, 256 )
UI_LoadImage( imgBakDown1, "texture/ui/PK3.tga", NORMAL, 256, 175, 0, 0 )

imgBakDown2 = UI_CreateCompent( frmTeamPK, IMAGE_TYPE, "imgBakDown2", 120, 175, 256, 256 )
UI_LoadImage( imgBakDown2, "texture/ui/PK4.tga", NORMAL, 120, 175, 0, 0 )

--imgBakDown3 = UI_CreateCompent( frmTeamPK, IMAGE_TYPE, "imgBakDown3", 46, 26, 512, 256 )
--UI_LoadImage( imgBakDown3, "texture/ui/PK3.tga", NORMAL, 46, 26, 210, 52 )

labPKtitle = UI_CreateCompent( frmTeamPK, LABELEX_TYPE, "labPKtitle", 207, 12, 87, 7 )
UI_SetCaption( labPKtitle, "" )
UI_SetTextColor( labPKtitle, COLOR_BLACK )
UI_SetLabelExFont( labPKtitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmTeamPK, BUTTON_TYPE, "btnClose", 14, 14, 340, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

labPKmap = UI_CreateCompent( frmTeamPK, LABELEX_TYPE, "labPKmap", 100, 12, 256, 31 )
UI_SetCaption( labPKmap, "" )
UI_SetTextColor( labPKmap, COLOR_BLACK )
UI_SetLabelExFont( labPKmap, DEFAULT_FONT, TRUE, COLOR_WHITE )

lstTeam0 = UI_CreateListView( frmTeamPK, "lstTeam0", 296, 115, 44, 87, 5, 2 )
UI_SetListRowHeight( lstTeam0, 21 )
UI_SetListFontColor( lstTeam0, COLOR_BLACK, COLOR_BLACK )
UI_SetListIsMouseFollow( lstTeam0, TRUE)
--UI_SetMargin( lstTeam0, 2,2,2,2 )
UI_SetIsShow( lstTeam0, TRUE )
id = UI_GetScroll( lstTeam0 )
UI_SetIsEnabled( id, FALSE )
UI_ListViewSetTitle( lstTeam0, 0, 105, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstTeam0, 1, 76, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstTeam0, 2, 39, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstTeam0, 3, 34, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstTeam0, 4, 12, "", 0, 0, 0, 0 )
UI_AddListText( lstTeam0, "Eight characters or therefore" )
UI_AddListText( lstTeam0, "Eight characters or therefore" )
UI_AddListText( lstTeam0, "Eight characters or therefore" )
UI_AddListText( lstTeam0, "Eight characters or therefore" )
UI_AddListText( lstTeam0, "Eight characters or therefore" )

--- Second Team ---
lstTeam1 = UI_CreateListView( frmTeamPK, "lstTeam1", 296, 115, 44, 264, 5, 2 )
UI_SetListRowHeight( lstTeam1, 21 )
UI_SetListFontColor( lstTeam1, COLOR_BLACK, COLOR_BLACK )
UI_SetListIsMouseFollow( lstTeam1, TRUE)
--UI_SetMargin( lstTeam1, 2,2,2,2 )
UI_SetIsShow( lstTeam1, TRUE )
id = UI_GetScroll( lstTeam1 )
UI_SetIsEnabled( id, FALSE )
UI_ListViewSetTitle( lstTeam1, 0, 105, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstTeam1, 1, 76, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstTeam1, 2, 39, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstTeam1, 3, 34, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstTeam1, 4, 12, "", 0, 0, 0, 0 )
UI_AddListText( lstTeam1, "Eight characters or therefore" )
UI_AddListText( lstTeam1, "Eight characters or therefore" )
UI_AddListText( lstTeam1, "Eight characters or therefore" )
UI_AddListText( lstTeam1, "Eight characters or therefore" )
UI_AddListText( lstTeam1, "Eight characters or therefore" )

btnYes = UI_CreateCompent( frmTeamPK, BUTTON_TYPE, "btnYes", 41, 19, 224, 382 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmTeamPK, BUTTON_TYPE, "btnNo", 41, 19, 279, 382 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )


-----------------------------
-- Target Info
-----------------------------
frmTargetInfo = UI_CreateForm("frmTargetInfo", FALSE, 166, 33, 200, 4, TRUE, FALSE)

imgBackDropTarget = UI_CreateCompent(frmTargetInfo, IMAGE_TYPE, "imgBackDropTarget", 40, 30, 3, 0)
UI_LoadImage(imgBackDropTarget, "texture/ui/backdrop/0.tga", NORMAL, 55, 44, 0, 0)

imgTargetInfo = UI_CreateCompent(frmTargetInfo, IMAGE_TYPE, "imgTargetInfo", 166, 33, 0, 0)
UI_LoadImage(imgTargetInfo, "texture/ui/TargetInfo.png", NORMAL, 166, 33, 0, 84)

UI_ShowForm(frmTargetInfo, TRUE)
UI_SetIsDrag(frmTargetInfo, FALSE)
UI_AddFormToTemplete(frmTargetInfo, FORM_MAIN)
UI_FormSetIsEscClose(frmTargetInfo, FALSE)

frmTargetInfoHP = UI_CreateCompent(frmTargetInfo, PROGRESS_TYPE, "frmTargetInfoHP", 111, 8, 51, 21)
UI_LoadScaleImage(frmTargetInfoHP, "texture/ui/new3.tga", PROGRESS_PROGRESS, 111, 8, 0, 129, 1.0, 1.0)
UI_SetHint(frmTargetInfoHP, "HP")

frmTargetInfoName = UI_CreateCompent(frmTargetInfo, LABELEX_TYPE, "frmTargetInfoName", 95, 11, 56, 5)
UI_SetTextColor(frmTargetInfoName, COLOR_WHITE)
UI_SetLabelExFont(frmTargetInfoName, DEFAULT_FONT, TRUE, COLOR_BLACK)

labTargetLv = UI_CreateCompent(frmTargetInfo, LABELEX_TYPE, "labTargetLv", 14, 9, 36, 6)
UI_SetTextColor(labTargetLv, COLOR_WHITE)
UI_SetLabelExFont(labTargetLv, DEFAULT_FONT, TRUE, COLOR_BLACK)
UI_SetCaption(labFLv, "99")

-- Render 3D Model
d3dTarget = UI_CreateCompent(frmTargetInfo, UI3D_COMPENT_TYPE, "d3dTarget", 23, 23, 6, 6)

-- Monster Info Button
btnMonsterInfo = UI_CreateCompent(frmTargetInfo, BUTTON_TYPE, "btnMonsterInfo", 67, 18, 100, 33)
UI_LoadButtonImage(btnMonsterInfo, "texture/ui/corsairs/frmMonsterInfo.png", 67, 18, 244, 488, TRUE)

-- Monster Drop Information Tab
frmMonsterInfo = UI_CreateForm("frmMonsterInfo", FALSE, 202, 473, 0, 0, TRUE, TRUE)
imgfrmMobDrops = UI_CreateCompent(frmMonsterInfo, IMAGE_TYPE, "imgfrmMobDrops", 202, 473, 0, 0)
UI_LoadImage(imgfrmMobDrops, "texture/ui/corsairs/frmMonsterInfo.png", NORMAL, 202, 473, 0, 0)

-- Drop Data Columns
imgfrmMobDrops = UI_CreateCompent(frmMonsterInfo, IMAGE_TYPE, "imgfrmMobDrops", 152, 402, 22, 54)
UI_LoadImage(imgfrmMobDrops, "texture/ui/corsairs/frmMonsterInfo.png", NORMAL, 152, 402, 542, 11)

-- Break Lines
imgfrmMobInfoLines = UI_CreateCompent(frmMonsterInfo, IMAGE_TYPE, "imgfrmMobInfoLines", 173, 403, 16, 51)
UI_LoadImage(imgfrmMobInfoLines, "texture/ui/corsairs/frmMonsterInfo.png", NORMAL, 173, 403, 326, 10)

-- Show UI
UI_ShowForm(frmMonsterInfo, TRUE)
UI_SetIsDrag(frmMonsterInfo, TRUE)
UI_AddFormToTemplete(frmMonsterInfo, FORM_MAIN)
UI_FormSetIsEscClose(frmMonsterInfo, FALSE)
UI_SetFormStyleEx(frmMonsterInfo, 9, 180, 59)
UI_FormSetIsEscClose(frmMonsterInfo, TRUE)

-- Close Button
btnClose = UI_CreateCompent(frmMonsterInfo, BUTTON_TYPE, "btnClose", 14, 14, 185, 3)
UI_LoadButtonImage(btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE)
UI_SetButtonModalResult(btnClose, BUTTON_CLOSE)


for i = 0, 14 do
	listMobDrops = UI_CreateCompent(frmMonsterInfo, COMMAND_ONE_TYPE, "listMobDrops"..i, 170, 37, 25, 56 + (i * 40))
	UI_SetIsDrag(listMobDrops, TRUE)

	LabMobItems = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobItems"..i, 85, 9, 64, 60 + (i * 40))
	UI_SetTextColor(LabMobItems, COLOR_BLACK)
	UI_SetLabelExFont(LabMobItems, DEFAULT_FONT, TRUE, COLOR_WHITE)

	LabMobRates = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobRates"..i, 85, 9, 64, 74 + (i * 40))
	UI_SetTextColor(LabMobRates, COLOR_BLACK)
	UI_SetLabelExFont(LabMobRates, DEFAULT_FONT, TRUE, COLOR_WHITE)
	
	checkDropFilter = UI_CreateCompent(frmMonsterInfo, CHECK_TYPE, "checkDropFilter"..i, 10, 10,  160, 68 + (i * 40))
	UI_LoadImage(checkDropFilter, "texture/ui/corsairs/frmMonsterInfo.png", UNCHECKED, 10, 10, 412, 473)
	UI_LoadImage(checkDropFilter, "texture/ui/corsairs/frmMonsterInfo.png", CHECKED, 10, 10, 393, 473)
end

-- Show Monster's Level
LabMobLevel = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobLevel", 50, 20, 130, 125)
UI_SetCaption(LabMobLevel, "0")
UI_SetTextColor(LabMobLevel, COLOR_PURPLE)
UI_SetLabelExFont(LabMobLevel, DEFAULT_FONT, TRUE, COLOR_WHITE)

-- Show Monster's Experience
LabMobexp = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobexp", 50, 20, 130, 147)
UI_SetCaption(LabMobexp, "0")
UI_SetTextColor(LabMobexp, COLOR_PURPLE)
UI_SetLabelExFont(LabMobexp, DEFAULT_FONT, TRUE, COLOR_WHITE)

-- Show Monster's HP
LabMobHP = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobHP", 50, 20, 130, 166)
UI_SetCaption(LabMobHP, "0")
UI_SetTextColor(LabMobHP, COLOR_PURPLE)
UI_SetLabelExFont(LabMobHP, DEFAULT_FONT, TRUE, COLOR_WHITE)

-- Show Monster's Damage
LabMobAttack = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobAttack", 50, 20, 130, 361)
UI_SetCaption(LabMobAttack, "0")
UI_SetTextColor(LabMobAttack, COLOR_PURPLE)
UI_SetLabelExFont(LabMobAttack, DEFAULT_FONT, TRUE, COLOR_WHITE)

-- Show Monster's Hit Rate
LabMobHitRate = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobHitRate", 50, 20, 130, 210)
UI_SetCaption(LabMobHitRate, "0")
UI_SetTextColor(LabMobHitRate, COLOR_PURPLE)
UI_SetLabelExFont(LabMobHitRate, DEFAULT_FONT, TRUE, COLOR_WHITE)

-- Show Monster's Dodge
LabMobDodge = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobDodge", 50, 20, 130, 230) --258
UI_SetCaption(LabMobDodge, "0")
UI_SetTextColor(LabMobDodge, COLOR_PURPLE)
UI_SetLabelExFont(LabMobDodge, DEFAULT_FONT, TRUE, COLOR_WHITE)

-- Show Monster's Defense
LabMobDef = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobDef", 50, 20, 130, 250)
UI_SetCaption(LabMobDef, "0")
UI_SetTextColor(LabMobDef, COLOR_PURPLE)
UI_SetLabelExFont(LabMobDef, DEFAULT_FONT, TRUE, COLOR_WHITE)

-- Show Monster's Physical Defense
LabMobPR = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobPR", 50, 20, 130, 270)
UI_SetCaption(LabMobPR, "0")
UI_SetTextColor(LabMobPR, COLOR_PURPLE)
UI_SetLabelExFont(LabMobPR, DEFAULT_FONT, TRUE, COLOR_WHITE)

-- Show Monster's Attack Speed
LabMobAtSpeed = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobAtSpeed", 50, 20, 130, 340)
UI_SetCaption(LabMobAtSpeed, "0")
UI_SetTextColor(LabMobAtSpeed, COLOR_PURPLE)
UI_SetLabelExFont(LabMobAtSpeed, DEFAULT_FONT, TRUE, COLOR_WHITE)

-- Show Monster's Movement Speed
LabMobMSpeed = UI_CreateCompent(frmMonsterInfo, LABELEX_TYPE, "LabMobMSpeed", 50, 20, 130, 290)
UI_SetCaption(LabMobMSpeed, "0")
UI_SetTextColor(LabMobMSpeed, COLOR_PURPLE)
UI_SetLabelExFont(LabMobMSpeed, DEFAULT_FONT, TRUE, COLOR_WHITE)

-- Form Tab
pgeSkill = UI_CreateCompent(frmMonsterInfo, PAGE_TYPE, "pgeSkill", 172, 25, 0, 0)
UI_SetPageButton(pgeSkill, PAGE_BUTTON_CUSTOM, 48, 16)

-- Drop Info Tab
DropsInfo = UI_CreatePageItem(pgeSkill)
ttlDrop = UI_GetPageItemObj(DropsInfo, PAGE_ITEM_TITLE)
UI_LoadImage(ttlDrop, "texture/ui/corsairs/frmMonsterInfo.png", PAGE_ITEM_TITLE_NORMAL, 67, 24, 246, 440)
UI_LoadImage(ttlDrop, "texture/ui/corsairs/frmMonsterInfo.png", PAGE_ITEM_TITLE_ACTIVE, 67, 24, 311, 440)
UI_SetPos(ttlDrop, 32, 30)
UI_SetSize(ttlDrop, 67, 24)

-- Display Monster Drops
UI_AddCompent(DropsInfo, listMobDrops)

-- Monster Attributes Tab
MobsInfo = UI_CreatePageItem(pgeSkill)
ttlInfo = UI_GetPageItemObj(MobsInfo, PAGE_ITEM_TITLE)
UI_LoadImage(ttlInfo, "texture/ui/corsairs/frmMonsterInfo.png", PAGE_ITEM_TITLE_NORMAL, 67, 24, 246, 464)
UI_LoadImage(ttlInfo, "texture/ui/corsairs/frmMonsterInfo.png", PAGE_ITEM_TITLE_ACTIVE, 67, 24, 311, 464)
UI_SetPos(ttlInfo, 86, 30)
UI_SetSize(ttlInfo, 67, 24)

-- Display Monster Attributes
UI_AddCompent(MobsInfo, imgfrmMobInfoLines)
--UI_AddCompent(MobsInfo, lstMobInfo)
UI_AddCompent(MobsInfo, LabMobLevel)
UI_AddCompent(MobsInfo, LabMobexp)
UI_AddCompent(MobsInfo, LabMobHP)
UI_AddCompent(MobsInfo, LabMobAttack)
UI_AddCompent(MobsInfo, LabMobHitRate)
UI_AddCompent(MobsInfo, LabMobDodge)
UI_AddCompent(MobsInfo, LabMobDef)
UI_AddCompent(MobsInfo, LabMobPR)
UI_AddCompent(MobsInfo, LabMobAtSpeed)
UI_AddCompent(MobsInfo, LabMobMSpeed)
