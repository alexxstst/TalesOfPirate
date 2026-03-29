---------------------
-- Character Stats --
---------------------
frmState = UI_CreateForm( "frmState", FALSE, 198, 394, 0, 160, TRUE, FALSE )
UI_FormSetHotKey( frmState, ALT_KEY, HOTKEY_A )
UI_ShowForm( frmState, FALSE )
UI_AddFormToTemplete( frmState, 1 )
UI_SetIsDrag( frmState, TRUE )
--UI_SetFormStyle( frmState, 0 )
--UI_SetAlpha( frmState, 200 )

imgBakTop = UI_CreateCompent( frmState, IMAGE_TYPE, "imgBakTop", 198, 256, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/PublicD.png", NORMAL, 198, 256, 0, 0 )
imgBakDown = UI_CreateCompent( frmState, IMAGE_TYPE, "imgBakDown", 198, 138, 0, 256 )
UI_LoadImage( imgBakDown, "texture/ui/PublicD2.png", NORMAL, 198, 138, 0, 0 )

imgStateTop = UI_CreateCompent( frmState, IMAGE_TYPE, "imgStateTop", 184, 87, 7, 37 )
UI_LoadImage( imgStateTop, "texture/ui/corsairs/State1.png", NORMAL, 184, 87, 0, 0 )

imgStateDownT = UI_CreateCompent( frmState, IMAGE_TYPE, "imgStateDownT", 184, 169, 7, 128 )
UI_LoadImage( imgStateDownT, "texture/ui/corsairs/State1.png", NORMAL, 184, 169, 0, 87 )

imgStateDown = UI_CreateCompent( frmState, IMAGE_TYPE, "imgStateDown", 184, 17, 7, 297 )
UI_LoadImage( imgStateDown, "texture/ui/corsairs/State1.png", NORMAL, 184, 17, 0, 255 )

imgStateL = UI_CreateCompent( frmState, IMAGE_TYPE, "imgStateL", 8, 8, 5,312 )
UI_LoadImage( imgStateL, "texture/ui/PublicC.tga", NORMAL, 8, 8, 167, 0 )

imgStateR = UI_CreateCompent( frmState, IMAGE_TYPE, "imgStateR", 8, 8, 185, 312 )
UI_LoadImage( imgStateR, "texture/ui/PublicC.tga", NORMAL, 8, 8, 167, 0 )

imgStateD = UI_CreateCompent( frmState, IMAGE_TYPE, "imgStateD", 184, 43, 7, 322 )
UI_LoadImage( imgStateD, "texture/ui/ShipBuild2.tga", NORMAL, 184, 43, 0, 172 )

btnClose = UI_CreateCompent( frmState, BUTTON_TYPE, "btnClose", 14, 14, 177, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labTitle = UI_CreateCompent( frmState, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Character")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgBakName = UI_CreateCompent( frmState, IMAGE_TYPE, "imgBakName", 129, 13, 32, 22 )
UI_LoadImage( imgBakName, "texture/ui/SystemBotton3.tga", NORMAL, 129, 13, 0, 227 )

labStateName = UI_CreateCompent( frmState, LABELEX_TYPE, "labStateName", 96, 11, 49, 23 )
UI_SetCaption( labStateName, "Child of Demon what is that" )
UI_SetTextColor( labStateName, COLOR_BLACK )
UI_SetLabelExFont( labStateName, DEFAULT_FONT, TRUE, COLOR_WHITE )

labStateJob = UI_CreateCompent( frmState, LABELEX_TYPE, "labStateJob", 47, 11, 90, 46 )
--UI_SetCaption( labStateJob, "Warrior" )
UI_SetTextColor( labStateJob, COLOR_BLACK )
UI_SetLabelExFont( labStateJob, DEFAULT_FONT, TRUE, COLOR_WHITE )

labStateGuid = UI_CreateCompent( frmState, LABELEX_TYPE, "labStateGuid", 128, 11, 69, 60 )
--UI_SetCaption( labStateGuid, "game developer" )
UI_SetTextColor( labStateGuid, COLOR_BLACK )
UI_SetLabelExFont( labStateGuid, DEFAULT_FONT, TRUE, COLOR_WHITE )

labStateLevel  = UI_CreateCompent( frmState, LABELEX_TYPE, "labStateLevel", 22, 10, 40, 77 )
UI_SetCaption( labStateLevel, "22" )
UI_SetTextColor( labStateLevel, COLOR_PURPLE )
UI_SetLabelExFont( labStateLevel, DEFAULT_FONT, TRUE, COLOR_WHITE )

labStateEXP = UI_CreateCompent( frmState, LABELEX_TYPE, "labStateEXP", 45, 10, 124, 77 )
UI_SetCaption( labStateEXP, "11" )
UI_SetTextColor( labStateEXP, COLOR_PURPLE )
UI_SetLabelExFont( labStateEXP, DEFAULT_FONT, TRUE, COLOR_WHITE )

labSailLevel  = UI_CreateCompent( frmState, LABELEX_TYPE, "labSailLevel", 22, 10, 40, 92 )
--UI_SetCaption( labSailLevel, "22" )
UI_SetTextColor( labSailLevel, COLOR_RED_SP )
UI_SetLabelExFont( labSailLevel, DEFAULT_FONT, TRUE, COLOR_BLUE )
UI_SetHint(labSailLevel,"Rebirth Level")

labSailEXP = UI_CreateCompent( frmState, LABELEX_TYPE, "labSailEXP", 45, 10, 129, 92 )
--UI_SetCaption( labSailEXP, "9999999999" )
UI_SetTextColor( labSailEXP, COLOR_RED_SP )
UI_SetLabelExFont( labSailEXP, DEFAULT_FONT, TRUE, COLOR_BLUE )
UI_SetHint(labSailEXP,"Rebirth Experience points")

labStateHP  = UI_CreateCompent( frmState, LABELEX_TYPE, "labStateHP", 45, 10, 40, 106 )
UI_SetCaption( labStateHP, "9999/9999" )
UI_SetTextColor( labStateHP, COLOR_PURPLE )
UI_SetLabelExFont( labStateHP, DEFAULT_FONT, TRUE, COLOR_WHITE )

labStateSP  = UI_CreateCompent( frmState, LABELEX_TYPE, "labStateSP", 45, 10, 124, 106 )
UI_SetCaption( labStateSP, "9999/9999" )
UI_SetTextColor( labStateSP, COLOR_PURPLE )
UI_SetLabelExFont( labStateSP, DEFAULT_FONT, TRUE, COLOR_WHITE )

labStatePoint  = UI_CreateCompent( frmState, LABELEX_TYPE, "labStatePoint", 35, 13, 152, 139 )
UI_SetCaption( labStatePoint, "1500" )
UI_SetTextColor( labStatePoint, COLOR_PURPLE )
UI_SetLabelExFont( labStatePoint, DEFAULT_FONT, TRUE, COLOR_WHITE )

labSkillPoint  = UI_CreateCompent( frmState, LABELEX_TYPE, "labSkillPoint", 35, 13, 64, 139 )
UI_SetCaption( labSkillPoint, "300" )
UI_SetTextColor( labSkillPoint, COLOR_PURPLE )
UI_SetLabelExFont( labSkillPoint, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnStr = UI_CreateCompent( frmState, BUTTON_TYPE, "btnStr", 10, 11, 87, 179 )
UI_LoadButtonImage( btnStr, "texture/ui/PublicC.tga", 10, 11, 195, 238, TRUE )
UI_SetHint( btnStr, "Strength increases Melee attack power" )

labStrshow = UI_CreateCompent( frmState, LABELEX_TYPE, "labStrshow", 23, 8, 49, 175 )
UI_SetCaption( labStrshow, "9527" )
UI_SetTextColor( labStrshow, COLOR_PURPLE )
UI_SetLabelExFont( labStrshow, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnAgi = UI_CreateCompent( frmState, BUTTON_TYPE, "btnAgi", 10, 11, 175, 179 )
UI_LoadButtonImage( btnAgi, "texture/ui/PublicC.tga", 10, 11, 195, 238, TRUE )
UI_SetHint( btnAgi, "Agility increases Attack speed and Dodge rate" )

labAgishow = UI_CreateCompent( frmState, LABELEX_TYPE, "labAgishow", 23, 8, 135, 175 )
UI_SetCaption( labAgishow, "9527" )
UI_SetTextColor( labAgishow, COLOR_PURPLE )
UI_SetLabelExFont( labAgishow, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnCon = UI_CreateCompent( frmState, BUTTON_TYPE, "btnCon", 10, 11, 87, 195 )
UI_LoadButtonImage( btnCon, "texture/ui/PublicC.tga", 10, 11, 195, 238, TRUE )
UI_SetHint( btnCon, "Constitution increases Defense, Max HP and HP recovery rate" )

labConshow = UI_CreateCompent( frmState, LABELEX_TYPE, "labConshow", 23, 8, 49, 191 )
UI_SetCaption( labConshow, "9527" )
UI_SetTextColor( labConshow, COLOR_PURPLE )
UI_SetLabelExFont( labConshow, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnSta = UI_CreateCompent( frmState, BUTTON_TYPE, "btnSta", 10, 11, 175, 195 )
UI_LoadButtonImage( btnSta, "texture/ui/PublicC.tga", 10, 11, 195, 238, TRUE )
UI_SetHint( btnSta, "Spirit increases Max SP and SP recovery rate" )

labStashow = UI_CreateCompent( frmState, LABELEX_TYPE, "labStashow", 23, 8, 135, 191 )
UI_SetCaption( labStashow, "9527" )
UI_SetTextColor( labStashow, COLOR_PURPLE )
UI_SetLabelExFont( labStashow, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Create accuracy stat row
btnDex = UI_CreateCompent( frmState, BUTTON_TYPE, "btnDex", 10, 11, 87, 211 )
UI_LoadButtonImage( btnDex, "texture/ui/PublicC.tga", 10, 11, 195, 238, TRUE )
UI_SetHint( btnDex, "Accuracy increases Range attack power and Hit Rate" )

labDexshow = UI_CreateCompent( frmState, LABELEX_TYPE, "labDexshow", 23, 8, 49, 207 )
UI_SetCaption( labDexshow, "9527" )
UI_SetTextColor( labDexshow, COLOR_PURPLE )
UI_SetLabelExFont( labDexshow, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Create min attack label
labMinAtackShow  = UI_CreateCompent( frmState, LABELEX_TYPE, "labMinAtackShow", 22, 8, 75, 248 )
UI_SetCaption( labMinAtackShow, "95278" )
UI_SetTextColor( labMinAtackShow, COLOR_PURPLE )
UI_SetLabelExFont( labMinAtackShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Create max attack label
labMaxAtackShow  = UI_CreateCompent( frmState, LABELEX_TYPE, "labMaxAtackShow", 22, 8, 159, 248 )
UI_SetCaption( labMaxAtackShow, "95278" )
UI_SetTextColor( labMaxAtackShow, COLOR_PURPLE )
UI_SetLabelExFont( labMaxAtackShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labHitShow  = UI_CreateCompent( frmState, LABELEX_TYPE, "labHitShow", 22, 8, 75, 264 )
UI_SetCaption( labHitShow, "400" )
UI_SetTextColor( labHitShow, COLOR_PURPLE )
UI_SetLabelExFont( labHitShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labFleeShow  = UI_CreateCompent( frmState, LABELEX_TYPE, "labFleeShow", 22, 8, 159, 264 )
UI_SetCaption( labFleeShow, "400" )
UI_SetTextColor( labFleeShow, COLOR_PURPLE )
UI_SetLabelExFont( labFleeShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labDefenceShow  = UI_CreateCompent( frmState, LABELEX_TYPE, "labDefenceShow", 22, 8, 75, 281 )
UI_SetCaption( labDefenceShow, "4000" )
UI_SetTextColor( labDefenceShow, COLOR_PURPLE )
UI_SetLabelExFont( labDefenceShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labAspeedShow  = UI_CreateCompent( frmState, LABELEX_TYPE, "labAspeedShow", 22, 8, 159, 281 )
UI_SetCaption( labAspeedShow, "400" )
UI_SetTextColor( labAspeedShow, COLOR_PURPLE )
UI_SetLabelExFont( labAspeedShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMspeedShow  = UI_CreateCompent( frmState, LABELEX_TYPE, "labMspeedShow", 22, 8, 159, 296 )
UI_SetCaption( labMspeedShow, "400" )
UI_SetTextColor( labMspeedShow, COLOR_PURPLE )
UI_SetLabelExFont( labMspeedShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labPhysDefineShow  = UI_CreateCompent( frmState, LABELEX_TYPE, "labPhysDefineShow", 22, 8, 75, 296 )
UI_SetCaption( labPhysDefineShow, "400" )
UI_SetTextColor( labPhysDefineShow, COLOR_PURPLE )
UI_SetLabelExFont( labPhysDefineShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labFameShow  = UI_CreateCompent( frmState, LABELEX_TYPE, "labFameShow", 26, 8, 75, 347 )
UI_SetCaption( labFameShow, "0" )
UI_SetTextColor( labFameShow, COLOR_PURPLE )
UI_SetLabelExFont( labFameShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labBattlepoints  = UI_CreateCompent( frmState, LABELEX_TYPE, "labBattlepoints", 71, 11, 111, 348 )
--UI_SetCaption( labBattlepoints, "0" )
UI_SetTextColor( labBattlepoints, COLOR_BLACK )
UI_SetLabelExFont( labBattlepoints, DEFAULT_FONT, TRUE, COLOR_WHITE )
