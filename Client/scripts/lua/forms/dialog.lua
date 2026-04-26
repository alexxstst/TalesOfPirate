----------------------
-- NPC Trade Dialog --
----------------------
frmTrade = UI_CreateForm( "frmTrade", FALSE, 185, 101, 240, 268, TRUE, FALSE )
UI_LoadFormImage( frmTrade, "texture/ui/sure.dds", 185, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmTrade, FALSE )
UI_AddFormToTemplete( frmTrade, FORM_DIALOG )
UI_SetIsDrag( frmTrade, TRUE )
UI_SetFormStyle( frmTrade, 2 )
UI_SetAlpha( frmTrade, 255 )

btnClose = UI_CreateCompent( frmTrade, BUTTON_TYPE, "btnClose", 14, 14, 164, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labName = UI_CreateCompent( frmTrade, LABELEX_TYPE, "labName", 95, 13, 20, 4 )
UI_SetCaption( labName, "Trade" )
UI_SetTextColor( labName, COLOR_BLACK )
UI_SetLabelExFont( labName, DEFAULT_FONT, TRUE, COLOR_WHITE )

labNumber = UI_CreateCompent( frmTrade, LABELEX_TYPE, "labNumber", 23, 11, 44, 32 )
UI_SetCaption( labNumber, "Quantity" )
UI_SetTextColor( labNumber, COLOR_BLACK )
UI_SetLabelExFont( labNumber, DEFAULT_FONT, TRUE, COLOR_WHITE )

labSum = UI_CreateCompent( frmTrade, LABELEX_TYPE, "labSum", 23, 11, 44, 54 )
UI_SetCaption( labSum, "Total Price" )
UI_SetTextColor( labSum, COLOR_BLACK )
UI_SetLabelExFont( labSum, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgNumber = UI_CreateCompent( frmTrade, IMAGE_TYPE, "imgNumber", 87, 18, 71, 28 )
UI_LoadImage( imgNumber, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )
imgSum = UI_CreateCompent( frmTrade, IMAGE_TYPE, "imgSum", 87, 18, 71, 50 )
UI_LoadImage( imgSum, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

edtNumber = UI_CreateCompent( frmTrade, EDIT_TYPE, "edtNumber", 87, 11, 79, 32 )
UI_SetTextColor( edtNumber, COLOR_BLACK )
UI_SetEditMaxNum( edtNumber, 3 )
UI_SetEditMaxNumVisible( edtNumber, 3 )

labPrice = UI_CreateCompent( frmTrade, LABELEX_TYPE, "labPrice", 87, 11, 79, 54 )
UI_SetCaption( labPrice, "0" )
UI_SetTextColor( labPrice, COLOR_BLACK )
UI_SetLabelExFont( labPrice, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmTrade, BUTTON_TYPE, "btnYes", 41, 19, 85, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 40, TRUE )
UI_SetButtonModalResult( btnYes, BUTTON_YES )
UI_SetEditEnterButton( edtNumber, btnYes )

btnNo = UI_CreateCompent( frmTrade, BUTTON_TYPE, "btnNo", 41, 19, 130, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_NO )

-----------------
-- select form --
-----------------
frmSelect = UI_CreateForm( "frmSelect",  FALSE, 218, 101, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmSelect, "texture/ui/surenew.tga", 218, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmSelect, FALSE )
UI_AddFormToTemplete( frmSelect, FORM_DIALOG )
UI_SetFormStyle( frmSelect, 0 )
UI_SetIsDrag( frmSelect, TRUE )

btnClose = UI_CreateCompent( frmSelect, BUTTON_TYPE, "btnClose", 14, 14, 197, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labInfo = UI_CreateCompent( frmSelect, LABELEX_TYPE, "labInfo", 23, 11, 44, 32 )
UI_SetCaption( labInfo, "Search ID\n\n Search Content" )
UI_SetTextColor( labInfo, COLOR_BLACK )
UI_SetLabelExFont( labInfo, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmSelect, BUTTON_TYPE, "btnYes", 41, 19, 107, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
--UI_LoadButtonImage( btnYes, "texture/ui/SystemBotton3.tga", 41, 19, 0, 0, TRUE )
UI_SetButtonModalResult( btnYes, BUTTON_YES )
UI_FormSetEnterButton( frmSelect, btnYes )

btnNo = UI_CreateCompent( frmSelect, BUTTON_TYPE, "btnNo", 41, 19, 155, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_NO )


-------------
-- Numbers --
-------------
frmNumber = UI_CreateForm( "frmNumber",  FALSE, 185, 101, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmNumber, "texture/ui/sure.dds", 185, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmNumber, FALSE )
UI_AddFormToTemplete( frmNumber, FORM_DIALOG )
UI_SetFormStyle( frmNumber, 0 )
UI_SetIsDrag( frmNumber, TRUE )

labInfo = UI_CreateCompent( frmNumber, LABELEX_TYPE, "labInfo", 23, 11, 50, 30 )
UI_SetCaption( labInfo, "Please Input Quantity" )
UI_SetTextColor( labInfo, COLOR_BLACK )
UI_SetLabelExFont( labInfo, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgNumber = UI_CreateCompent( frmNumber, IMAGE_TYPE, "imgNumber", 87, 18, 50, 50 )
UI_LoadImage( imgNumber, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

edtNumber = UI_CreateCompent( frmNumber, EDIT_TYPE, "edtNumber", 71, 11, 58, 54 )
UI_SetTextColor( edtNumber, COLOR_BLACK )
UI_SetEditMaxNum( edtNumber, 32 )
UI_SetEditMaxNumVisible( edtNumber, 32 )

btnYes = UI_CreateCompent( frmNumber, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetButtonModalResult( btnYes, BUTTON_YES )
UI_SetEditEnterButton( edtNumber, btnYes )

btnNo = UI_CreateCompent( frmNumber, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_NO )

------------------
-- Error Dialog --
------------------
frmError = UI_CreateForm( "frmError",  TRUE, 185, 101, 397, 500, FALSE, TRUE )
UI_LoadFormImage( frmError, "texture/ui/system/error.tga", 32, 32, 64, 64, "texture/ui/system/error.tga", 32, 32 )

UI_SetFormStyle( frmError, 1 )
UI_ShowForm( frmError, FALSE )
UI_AddFormToTemplete( frmError, FORM_DIALOG )
UI_SetIsDrag( frmError, TRUE )

labError = UI_CreateCompent( frmError, LABELEX_TYPE, "labError", 59, 11, 35, 40 )
UI_SetCaption( labError, "" )
UI_SetTextColor( labError, COLOR_BLACK )
UI_SetLabelExFont( labError, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmError, BUTTON_TYPE, "btnYes", 41, 19, 80, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetButtonModalResult( btnYes, BUTTON_YES )
UI_SetAlign( btnYes, caWidthCenter )
UI_FormSetEnterButton( frmError, btnYes )

----------------------------------
-- guild disband password input --
----------------------------------
frmPass = UI_CreateForm( "frmPass", FALSE, 185, 101, 310, 410, FALSE, FALSE )
UI_AddFormToTemplete( frmPass, FORM_DIALOG )
UI_SetFormStyle( frmPass, 2 )
--UI_FormSetHotKey( frmPass, ALT_KEY, 75 ) 
UI_FormSetIsEscClose( frmPass, FALSE )

imgAccout = UI_CreateCompent( frmPass, IMAGE_TYPE, "imgAccout", 185, 101, 0, 0 )
UI_LoadImage( imgAccout, "texture/ui/sure.dds", NORMAL, 185, 101, 0, 0 )

labTitle = UI_CreateCompent( frmPass, LABELEX_TYPE, "labTitle", 49, 11, 7, 4 )
UI_SetCaption( labTitle, "Input password" )
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labPass = UI_CreateCompent( frmPass, LABELEX_TYPE, "labPass", 23, 11, 23, 44 )
UI_SetCaption( labPass, "Password" )
UI_SetTextColor( labPass, COLOR_BLACK )
UI_SetLabelExFont( labPass, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgPass = UI_CreateCompent( frmPass, IMAGE_TYPE, "imgPass", 87, 18, 70, 40 )
UI_LoadImage( imgPass, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

btnYes = UI_CreateCompent( frmPass, BUTTON_TYPE, "btnYes", 41, 19, 84, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmPass, BUTTON_TYPE, "btnNo", 41, 19, 129, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

edtPassword = UI_CreateCompent( frmPass, EDIT_TYPE, "edtPassword", 87, 12, 76, 44 )
UI_SetTextColor( edtPassword, COLOR_BLACK )
UI_SetEditMaxNum( edtPassword, 12 )
UI_SetEditMaxNumVisible( edtPassword, 12 )
UI_SetButtonModalResult( btnYes, BUTTON_YES )
UI_SetButtonModalResult( btnNo, BUTTON_NO )

UI_SetEditEnterButton( edtPassword, btnYes )


-----------------------------------------------------------------------
-- Anti-cheat verification dialog
-----------------------------------------------------------------------
frmWGAnswer = UI_CreateForm( "frmWGAnswer",  FALSE, 185, 101, 397, 500, TRUE, FALSE )
UI_ShowForm( frmWGAnswer, FALSE )
UI_SetIsDrag( frmWGAnswer, TRUE )
UI_AddFormToTemplete( frmWGAnswer, FORM_MAIN )
UI_FormSetIsEscClose( frmWGAnswer, FALSE )

--UI_FormSetHotKey( frmWGAnswer, ALT_KEY, HOTKEY_Z) 

-- Load background image
imgBackGround = UI_CreateCompent( frmWGAnswer, IMAGE_TYPE, "imgBackGround", 185, 101, 0, 0 )
UI_LoadImage( imgBackGround, "texture/ui/sure.dds", NORMAL, 185, 101, 0, 0 )

-- Notice label
	labNotice = UI_CreateCompent( frmWGAnswer, LABELEX_TYPE, "labNotice", 400, 150, 7, 3 )
	UI_SetCaption( labNotice, "Enter 4 letter code")
	UI_SetTextColor( labNotice, COLOR_BLUE)
	UI_SetLabelExFont( labNotice, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Verification code display
ui3dCheckCode = UI_CreateCompent( frmWGAnswer, UI3D_COMPENT, "ui3dCheckCode", 110, 240, 73,24 )

-- Input field background
imgSum = UI_CreateCompent( frmWGAnswer, IMAGE_TYPE, "imgSum", 57, 18, 116, 54 )
UI_LoadImage( imgSum, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

-- User input field
edtUserInput = UI_CreateCompent( frmWGAnswer, EDIT_TYPE, "edtUserInput", 87, 12, 120, 57 )
UI_SetTextColor( edtUserInput, COLOR_BLACK )
UI_SetEditMaxNum( edtUserInput, 4 )
UI_SetEditMaxNumVisible( edtUserInput, 4 )
UI_SetButtonModalResult( btnYes, BUTTON_YES )
UI_SetButtonModalResult( btnNo, BUTTON_NO )

UI_SetEditEnterButton( edtPassword, btnYes )

-- Countdown timer label
labNotice = UI_CreateCompent( frmWGAnswer, LABELEX_TYPE, "labNotice", 23, 11, 12,57 )
UI_SetCaption( labNotice, "Time" )
UI_SetTextColor( labNotice, COLOR_BLACK )
UI_SetLabelExFont( labNotice, DEFAULT_FONT, FALSE, COLOR_WHITE )

labTimeLeft = UI_CreateCompent( frmWGAnswer, LABELEX_TYPE, "labTimeLeft", 23, 11, 88, 56 )
UI_SetCaption( labTimeLeft, "Password" )
UI_SetTextColor( labTimeLeft, COLOR_RED )
UI_SetLabelExFont( labTimeLeft, FONT16, FALSE, COLOR_WHITE )

-- Confirm button
btnYes = UI_CreateCompent( frmWGAnswer, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetButtonModalResult( btnYes, BUTTON_YES )
UI_SetEditEnterButton( edtUserInput, btnYes )


-----------------------------------------------------
------ Forge attribute transfer By Xiaoyu 090310 ------
-----------------------------------------------------
-- Create forge transfer page
frmforgeMove = UI_CreateForm( "frmforgeMove", FALSE, 310, 275, 240, 70, TRUE, FALSE )
UI_ShowForm( frmforgeMove, TRUE )
UI_AddFormToTemplete( frmforgeMove, FORM_MAIN )
--UI_FormSetHotKey( frmforgeMove, ALT_KEY, HOTKEY_V )
UI_SetIsDrag( frmforgeMove, TRUE )

-- Load background images
imgBakLeft = UI_CreateCompent( frmforgeMove, IMAGE_TYPE, "imgBakLeft", 256, 256, 0, 0 )
UI_LoadImage( imgBakLeft, "texture/ui2009/tongyongX1.tga", NORMAL, 256, 256, 0, 0 )

imgBakRight = UI_CreateCompent( frmforgeMove, IMAGE_TYPE, "imgBakRight", 54, 256, 256, 0 )
UI_LoadImage( imgBakRight, "texture/ui2009/tongyongX2.tga", NORMAL, 54, 256, 0, 0 )

imgBakLeftD = UI_CreateCompent( frmforgeMove, IMAGE_TYPE, "imgBakLeftD", 202, 19, 0, 256 )
UI_LoadImage( imgBakLeftD, "texture/ui2009/tongyongX2.tga", NORMAL, 202, 19, 54, 0 )

imgBakRightD = UI_CreateCompent( frmforgeMove, IMAGE_TYPE, "imgBakRightD", 109, 19, 202, 256 )
UI_LoadImage( imgBakRightD, "texture/ui2009/tongyongX2.tga", NORMAL, 109, 19, 54, 19 )

-- Form title
labtitle = UI_CreateCompent( frmforgeMove, LABELEX_TYPE, "labtitle", 261, 12, 22, 12 )
UI_SetCaption( labtitle, "Refining Stone Transfer" )
UI_SetTextColor( labtitle, COLOR_WHITE )
UI_SetLabelExFont( labtitle, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Close button
btnClose = UI_CreateCompent( frmforgeMove, BUTTON_TYPE, "btnClose", 15, 15, 278, 12 )
UI_LoadButtonImage( btnClose, "texture/ui2009/button.tga", 15, 15, 165, 129, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-- Catalyst slot
imgElf0 = UI_CreateCompent( frmforgeMove, IMAGE_TYPE, "imgElf0", 47, 47, 134, 111 )
UI_LoadImage( imgElf0, "texture/ui2009/BakGhost3.tga", NORMAL, 47, 47, 86, 135 )

-- Item description 1
labtitle1 = UI_CreateCompent( frmforgeMove, LABELEX_TYPE, "labtitle1", 261, 12, 140, 127 )
UI_SetCaption( labtitle1, "Catalyst" )
UI_SetTextColor( labtitle1, COLOR_BLACK )
UI_SetLabelExFont( labtitle1, DEFAULT_FONT, FALSE, COLOR_BLACK )

cmdStore1 = UI_CreateCompent( frmforgeMove, COMMAND_ONE_TYPE, "cmdStore1", 32, 32, 140, 117 )	-- Catalyst
UI_SetHint( cmdStore1, "Place catalyst" )

-- Equipment 1 slot
imgStore1 = UI_CreateCompent( frmforgeMove, IMAGE_TYPE, "imgStore1", 47, 47, 24, 45 )
UI_LoadImage( imgStore1, "texture/ui2009/BakGhost3.tga", NORMAL, 47, 47, 86, 135 )

-- Item description 2
labtitle2 = UI_CreateCompent( frmforgeMove, LABELEX_TYPE, "labtitle2", 261, 12, 30, 61 )
UI_SetCaption( labtitle2, "Equipment 1" )
UI_SetTextColor( labtitle2, COLOR_BLACK )
UI_SetLabelExFont( labtitle2, DEFAULT_FONT, FALSE, COLOR_BLACK )

cmdElf0 = UI_CreateCompent( frmforgeMove, COMMAND_ONE_TYPE, "cmdElf0", 32, 32, 30, 51 )	-- Combine item
UI_SetHint( cmdElf0, "Place equipment 1" )

-- Arrow 1
imgjiantou1 = UI_CreateCompent( frmforgeMove, IMAGE_TYPE, "imgjiantou1", 84, 48, 49, 92 )
UI_LoadImage( imgjiantou1, "texture/ui2009/jiantou.tga", NORMAL, 84, 48, 0, 0 )

-- Equipment 2 slot
imgEquip2 = UI_CreateCompent( frmforgeMove, IMAGE_TYPE, "imgEquip2", 47, 47, 243, 45 )
UI_LoadImage( imgEquip2, "texture/ui2009/BakGhost3.tga", NORMAL, 47, 47, 86, 135 )

-- Item description 3
labtitle3 = UI_CreateCompent( frmforgeMove, LABELEX_TYPE, "labtitle3", 261, 12, 249, 61 )
UI_SetCaption( labtitle3, "Equipment 2" )
UI_SetTextColor( labtitle3, COLOR_BLACK )
UI_SetLabelExFont( labtitle3, DEFAULT_FONT, FALSE, COLOR_BLACK )

cmdEquip2 = UI_CreateCompent( frmforgeMove, COMMAND_ONE_TYPE, "cmdEquip2", 32, 32, 249, 51 )	-- Combine item
UI_SetHint( cmdEquip2, "Place equipment 2" )

-- Arrow 2
imgjiantou2 = UI_CreateCompent( frmforgeMove, IMAGE_TYPE, "imgjiantou2", 84, 41, 181, 96 )
UI_LoadImage( imgjiantou2, "texture/ui2009/jiantou.tga", NORMAL, 84, 41, 91, 0 )

-- Help text background
imgTxtHelp1 = UI_CreateCompent( frmforgeMove, IMAGE_TYPE, "imgTxtHelp1", 202, 60, 16, 171 )
UI_LoadImage( imgTxtHelp1, "texture/ui2009/tongyongX2.tga", NORMAL, 202, 60, 54, 38 )

imgTxtHelp2 = UI_CreateCompent( frmforgeMove, IMAGE_TYPE, "imgTxtHelp2", 78, 60, 218, 171 )
UI_LoadImage( imgTxtHelp2, "texture/ui2009/tongyongX2.tga", NORMAL, 78, 60, 54, 98 )

-- Help text block 1
labTxthelp1 = UI_CreateCompent( frmforgeMove, LABELEX_TYPE, "labTxthelp1", 261, 12, 24, 180 )
UI_SetCaption( labTxthelp1, "After transfer Equipment 1 enchants will be lost" )
UI_SetTextColor( labTxthelp1, COLOR_BLACK )
UI_SetLabelExFont( labTxthelp1, DEFAULT_FONT, FALSE, COLOR_WHITE )

-- Help text block 2
labTxthelp2 = UI_CreateCompent( frmforgeMove, LABELEX_TYPE, "labTxthelp2", 261, 12, 24, 195 )
UI_SetCaption( labTxthelp2, "Both items must be of the same equipment type" )
UI_SetTextColor( labTxthelp2, COLOR_BLACK )
UI_SetLabelExFont( labTxthelp2, DEFAULT_FONT, FALSE, COLOR_WHITE )

-- Help text block 3
labTxthelp3 = UI_CreateCompent( frmforgeMove, LABELEX_TYPE, "labTxthelp3", 261, 12, 24, 210 )
UI_SetCaption( labTxthelp3, "Transfer requires a Refining Stone (NPC item)" )
UI_SetTextColor( labTxthelp3, COLOR_BLACK )
UI_SetLabelExFont( labTxthelp3, DEFAULT_FONT, FALSE, COLOR_WHITE )

-- Confirm button
btnYes = UI_CreateCompent( frmforgeMove, BUTTON_TYPE, "btnYes", 43, 22, 58, 238 )
UI_LoadButtonImage( btnYes, "texture/ui2009/button.tga", 47, 22, 0, 0, TRUE )

-- Cancel button
btnNo = UI_CreateCompent( frmforgeMove, BUTTON_TYPE, "btnNo", 43, 22, 207, 238 )
UI_LoadButtonImage( btnNo, "texture/ui2009/BakGhost3.tga", 43, 22, 43, 135, FALSE )
UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )
