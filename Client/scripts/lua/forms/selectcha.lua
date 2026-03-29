-----------------------------------------------------------------------
-- Character selection related forms
-----------------------------------------------------------------------
-- Create character info form
frmChaInfo = UI_CreateForm( "frmChaInfo", FALSE, 136, 73, 0, 0, TRUE, FALSE )
--UI_FormSetHotKey( frmChaInfo, ALT_KEY, HOTKEY_H ) --'H'
--UI_LoadFormImage( frmChaInfo, "texture/ui/selectcha.tga", 164, 83, 0, 83, "", 0, 0 )
UI_FormSetIsEscClose( frmChaInfo, FALSE )
UI_ShowForm( frmChaInfo, TRUE )
UI_AddFormToTemplete( frmChaInfo, FORM_LOGIN )
UI_SetAlpha( frmChaInfo, 200 )

-- Background image
imgChaInfoUp = UI_CreateCompent( frmChaInfo, IMAGE_TYPE, "imgChaInfoUp", 136, 59, 0, 0 )
UI_LoadImage( imgChaInfoUp, "texture/ui/selectcha.tga", NORMAL, 136, 59, 261, 359 )

imgChaInfoDown = UI_CreateCompent( frmChaInfo, IMAGE_TYPE, "imgChaInfoDown", 136, 14, 0, 59 )
UI_LoadImage( imgChaInfoDown, "texture/ui/selectcha.tga", NORMAL, 136, 14, 261, 497 )

-- Character name display
labChaInfoName = UI_CreateCompent( frmChaInfo, LABELEX_TYPE, "labChaInfoName", 94, 12, 8, 10 )
UI_SetCaption( labChaInfoName, "Name Pirate Prince B.J" )
UI_SetTextColor( labChaInfoName, COLOR_WHITE )
UI_SetLabelExFont( labChaInfoName, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Character class display
labChaInfoJob = UI_CreateCompent( frmChaInfo, LABELEX_TYPE, "labChaInfoJob", 94, 12, 8, 31 )
UI_SetCaption( labChaInfoJob, "Class Sharpshooter" )
UI_SetTextColor( labChaInfoJob, COLOR_WHITE )
UI_SetLabelExFont( labChaInfoJob, DEFAULT_FONT, TRUE, COLOR_BLACK )


-- Load level
labChaInfoLevel = UI_CreateCompent( frmChaInfo, LABELEX_TYPE, "labChaInfoLevel", 94, 12, 8, 52 )
UI_SetCaption( labChaInfoLevel, "Level 300" )
UI_SetTextColor( labChaInfoLevel, COLOR_WHITE )
UI_SetLabelExFont( labChaInfoLevel, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- EXP bar
--labEXP = UI_CreateCompent( frmChaInfo, LABELEX_TYPE, "labEXP", 41, 8, 68, 61 )
--UI_SetCaption( labEXP, "100.00%" )
--UI_SetTextColor( labEXP, COLOR_WHITE )
--UI_SetLabelExFont( labEXP, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- HP bar
--labChaInfoHP = UI_CreateCompent( frmChaInfo, LABELEX_TYPE, "labChaInfoHP", 81, 8, 44, 31 )
--UI_SetCaption( labChaInfoHP, "HP 88888/88888" )
--UI_SetTextColor( labChaInfoHP, COLOR_WHITE )
--UI_SetLabelExFont( labChaInfoHP, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- SP bar
--labChaInfoSP = UI_CreateCompent( frmChaInfo, LABELEX_TYPE, "labChaInfoSP", 81, 8, 44, 45 )
--UI_SetCaption( labChaInfoSP, "SP 88888/88888" )
--UI_SetTextColor( labChaInfoSP, COLOR_WHITE )
--UI_SetLabelExFont( labChaInfoSP, DEFAULT_FONT, TRUE, COLOR_BLACK )

-----------------------------------------------------------------------
-- Character selection form
-----------------------------------------------------------------------
-- Create selection form
frmUserselect = UI_CreateForm( "frmUserselect", FALSE, 570, 356, 227, 197, TRUE, FALSE )
--UI_ShowForm( frmUserselect, TRUE )
UI_AddFormToTemplete( frmUserselect, FORM_LOGIN )
--UI_SetAlpha( frmUserselect, 200 )
--UI_SetIsDrag( frmUserselect, TRUE )
UI_SetFormStyle( frmUserselect, 2 )
UI_FormSetIsEscClose( frmUserselect, FALSE )

-- Background image
--imgUserselect = UI_CreateCompent( frmUserselect, IMAGE_TYPE, "imgUserselect", 569, 372, 0, 0 )
--UI_LoadImage( imgUserselect, "texture/ui/selectcha.tga", NORMAL, 569, 372, 0, 0 )

-- Character name display
--labUserselect = UI_CreateCompent( frmUserselect, LABELEX_TYPE, "labUserselect", 94, 12, 15, 4 )
--UI_SetCaption( labUserselect, "Select Character" )
--UI_SetTextColor( labUserselect, COLOR_WHITE )
--UI_SetLabelExFont( labUserselect, DEFAULT_FONT, TRUE, COLOR_BLACK )


-- Create 3D character 1
ui3dCha1 = UI_CreateCompent( frmUserselect, UI3D_COMPENT, "ui3dCha1", 159, 235, 24, 34 )

-- Create 3D character 2
ui3dCha2 = UI_CreateCompent( frmUserselect, UI3D_COMPENT, "ui3dCha2", 159, 235, 207, 34 )

-- Create 3D character 3
ui3dCha3 = UI_CreateCompent( frmUserselect, UI3D_COMPENT, "ui3dCha3", 159, 235, 391, 34 )

-- Character 1 name display
labCha1 = UI_CreateCompent( frmUserselect, LABELEX_TYPE, "labCha1", 96, 12, 27, 280 )
UI_SetCaption( labCha1, "Nil" )
UI_SetTextColor( labCha1, COLOR_BLACK )
UI_SetLabelExFont( labCha1, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Character 2 name display
labCha2 = UI_CreateCompent( frmUserselect, LABELEX_TYPE, "labCha2", 96, 12, 210, 280 )
UI_SetCaption( labCha2, "Nil" )
UI_SetTextColor( labCha2, COLOR_BLACK )
UI_SetLabelExFont( labCha2, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Character 3 name display
labCha3 = UI_CreateCompent( frmUserselect, LABELEX_TYPE, "labCha3", 96, 12, 394, 280 )
UI_SetCaption( labCha3, "Nil" )
UI_SetTextColor( labCha3, COLOR_BLACK )
UI_SetLabelExFont( labCha3, DEFAULT_FONT, TRUE, COLOR_WHITE )


-- Create button
btnCreate = UI_CreateCompent( frmUserselect, BUTTON_TYPE, "btnCreate", 41, 19, 144, 320 )
UI_LoadButtonImage( btnCreate, "texture/ui/SystemBotton3.tga", 41, 19, 0, 95, TRUE )

labCreate = UI_CreateCompent( frmUserselect, LABELEX_TYPE, "labCreate", 42, 10, 153, 324 )
UI_SetCaption( labCreate, "" )
UI_SetTextColor( labCreate, COLOR_WHITE )
UI_SetLabelExFont( labCreate, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Delete button
btnDel = UI_CreateCompent( frmUserselect, BUTTON_TYPE, "btnDel", 41, 19, 218, 320 )
UI_LoadButtonImage( btnDel, "texture/ui/SystemBotton3.tga", 41, 19, 0, 114, TRUE )

labDel = UI_CreateCompent( frmUserselect, LABELEX_TYPE, "labDel", 42, 10, 227, 324 )
UI_SetCaption( labDel, "" )
UI_SetTextColor( labDel, COLOR_WHITE )
UI_SetLabelExFont( labDel, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Enter button
btnYes = UI_CreateCompent( frmUserselect, BUTTON_TYPE, "btnYes", 41, 19, 292, 320 )
UI_LoadButtonImage( btnYes, "texture/ui/SystemBotton3.tga", 41, 19, 0, 133, TRUE )

labYes = UI_CreateCompent( frmUserselect, LABELEX_TYPE, "labYes", 42, 10, 301, 324 )
UI_SetCaption( labYes, "" )
UI_SetTextColor( labYes, COLOR_WHITE )
UI_SetLabelExFont( labYes, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Exit button
btnNo = UI_CreateCompent( frmUserselect, BUTTON_TYPE, "btnNo", 41, 19, 366, 320 )
UI_LoadButtonImage( btnNo, "texture/ui/SystemBotton3.tga", 41, 19, 0, 152, TRUE )

labNo = UI_CreateCompent( frmUserselect, LABELEX_TYPE, "labNo", 42, 10, 375, 324 )
UI_SetCaption( labNo, "" )
UI_SetTextColor( labNo, COLOR_WHITE )
UI_SetLabelExFont( labNo, DEFAULT_FONT, TRUE, COLOR_BLACK )

-----------------------------------------------------------------------
-- Create character form
-----------------------------------------------------------------------
-- Create form
frmUserfound = UI_CreateForm( "frmUserfound", FALSE, 394, 356, 300, 197, FALSE, FALSE )
UI_LoadFormImage( frmUserfound, "texture/ui/player.dds", 394, 356, 0, 356, "", 0, 0 )
--UI_FormSetHotKey( frmUserfound, ALT_KEY, HOTKEY_N ) --'V'
UI_ShowForm( frmUserfound, FALSE )
UI_AddFormToTemplete( frmUserfound, FORM_LOGIN )
--UI_SetIsDrag( frmUserfound, TRUE )
UI_SetFormStyle( frmUserfound, 2 )
--UI_FormSetHotKey( frmUserfound, ALT_KEY, 84 ) --'T'
UI_FormSetIsEscClose( frmUserfound, FALSE )


-- Description text area
memChaDescribe  = UI_CreateCompent( frmUserfound, MEMO_TYPE, "memChaDescribe", 167, 116, 198, 63 )
--UI_SetCaption( memChaDescribe, "" )
UI_SetTextColor( memChaDescribe, COLOR_BLACK )
UI_SetLabelExFont( memChaDescribe, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetMemoMaxNumPerRow( memChaDescribe, 26 )
UI_SetMemoPageShowNum( memChaDescribe, 6 )

-- Name background image
imgNameBak = UI_CreateCompent( frmUserfound, IMAGE_TYPE, "imgNameBak", 136, 19, 227, 187 )
UI_LoadImage( imgNameBak, "texture/ui/player.dds", NORMAL, 136, 19, 397, 393 )

-- Style background image
imgStyleBak = UI_CreateCompent( frmUserfound, IMAGE_TYPE, "imgStyleBak", 136, 19, 227, 208 )
UI_LoadImage( imgStyleBak, "texture/ui/player.dds", NORMAL, 136, 19, 397, 393 )

-- Hair background image
imgHairBak = UI_CreateCompent( frmUserfound, IMAGE_TYPE, "imgHairBak", 136, 19, 227, 230 )
UI_LoadImage( imgHairBak, "texture/ui/player.dds", NORMAL, 136, 19, 397, 393 )

-- Face background image
imgFaceBak = UI_CreateCompent( frmUserfound, IMAGE_TYPE, "imgFaceBak", 136, 19, 227, 252 )
UI_LoadImage( imgFaceBak, "texture/ui/player.dds", NORMAL, 136, 19, 397, 393 )

-- City background image
imgCityBak = UI_CreateCompent( frmUserfound, IMAGE_TYPE, "imgCityBak", 136, 19, 227, 274 )
UI_LoadImage( imgCityBak, "texture/ui/player.dds", NORMAL, 136, 19, 397, 393 )


-- Name input field
edtName = UI_CreateCompent( frmUserfound, EDIT_TYPE, "edtName", 117, 10, 236, 191 )
UI_SetTextColor( edtName, COLOR_BLACK )
UI_SetEditMaxNum( edtName, 16 )
UI_SetEditMaxNumVisible( edtName, 16 )

-- Check name button
--btnCheck = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnCheck", 15, 17, 389, 189 )
--UI_LoadButtonImage( btnCheck, "texture/ui/selectcha.tga", 15, 17, 1, 359, TRUE )



-- Style, hair, face, class display
labStyleShow = UI_CreateCompent( frmUserfound, LABELEX_TYPE, "labStyleShow", 86, 10, 250, 211 )
UI_SetCaption( labStyleShow, "Lance" )
UI_SetTextColor( labStyleShow, COLOR_BLACK )
UI_SetLabelExFont( labStyleShow, DEFAULT_FONT, TRUE, COLOR_WHITE )
labHairShow = UI_CreateCompent( frmUserfound, LABELEX_TYPE, "labHairShow", 86, 10, 250, 233 )
UI_SetCaption( labHairShow, "Long Hair" )
UI_SetTextColor( labHairShow, COLOR_BLACK )
UI_SetLabelExFont( labHairShow, DEFAULT_FONT, TRUE, COLOR_WHITE )
labFaceShow = UI_CreateCompent( frmUserfound, LABELEX_TYPE, "labFaceShow", 86, 10, 250, 255 )
UI_SetCaption( labFaceShow, "Nightmare" )
UI_SetTextColor( labFaceShow, COLOR_BLACK )
UI_SetLabelExFont( labFaceShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

labCityShow = UI_CreateCompent( frmUserfound, LABELEX_TYPE, "labCityShow", 86, 10, 250, 277 )
UI_SetCaption( labCityShow, "Argent City" )
UI_SetTextColor( labCityShow, COLOR_BLACK )
UI_SetLabelExFont( labCityShow, DEFAULT_FONT, TRUE, COLOR_WHITE )



-- Left arrow buttons
btnLeftStyle = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnLeftStyle", 13, 13, 234, 211 )
UI_LoadButtonImage( btnLeftStyle, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE )

btnLeftHair = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnLeftHair", 13, 13, 234, 233 )
UI_LoadButtonImage( btnLeftHair, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE )

btnLeftFace = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnLeftFace", 13, 13, 234, 255 )
UI_LoadButtonImage( btnLeftFace, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE )

btnLeft3d = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnLeft3d", 15, 13, 45, 280 )
UI_LoadButtonImage( btnLeft3d, "texture/ui/player.dds", 15, 13, 398, 360, TRUE )

btnLeftCity = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnLeftCity", 13, 13, 234, 277 )
UI_LoadButtonImage( btnLeftCity, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE )

-- Right arrow buttons
btnRightStyle = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnRightStyle", 13, 13, 345, 211 )
UI_LoadButtonImage( btnRightStyle, "texture/ui/SystemBotton3.tga", 13, 13, 0, 211, TRUE )

btnRightHair = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnRightHair", 13, 13, 345, 233 )
UI_LoadButtonImage( btnRightHair, "texture/ui/SystemBotton3.tga", 13, 13, 0, 211, TRUE )

btnRightFace = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnRightFace", 13, 13, 345, 255 )
UI_LoadButtonImage( btnRightFace, "texture/ui/SystemBotton3.tga", 13, 13, 0, 211, TRUE )

btnRight3d = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnRight3d", 15, 13, 150, 280 )
UI_LoadButtonImage( btnRight3d, "texture/ui/player.dds",  15, 13, 398, 373, TRUE )

btnRightCity = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnRightCity", 13, 13, 345, 277 )
UI_LoadButtonImage( btnRightCity, "texture/ui/SystemBotton3.tga",  13, 13, 0, 211, TRUE )

-- Create 3D character
ui3dCreateCha = UI_CreateCompent( frmUserfound, UI3D_COMPENT, "ui3dCreateCha", 159, 235, 24, 34 )


-- Confirm button
btnYes = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnYes", 41, 19, 235, 300 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

labYes = UI_CreateCompent( frmUserfound, LABELEX_TYPE, "labYes", 42, 10, 244, 314 )
UI_SetCaption( labYes, "" )
UI_SetTextColor( labYes, COLOR_WHITE )
UI_SetLabelExFont( labYes, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- Cancel button
btnNo = UI_CreateCompent( frmUserfound, BUTTON_TYPE, "btnNo", 41, 19, 305, 300 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

labNo = UI_CreateCompent( frmUserfound, LABELEX_TYPE, "labNo", 42, 10, 314, 314 )
UI_SetCaption( labNo, "" )
UI_SetTextColor( labNo, COLOR_WHITE )
UI_SetLabelExFont( labNo, DEFAULT_FONT, TRUE, COLOR_BLACK )




-----------------------------------------------------------------------
-- LOGO form
-----------------------------------------------------------------------
-- Create LOGO
frmLOGO = UI_CreateForm( "frmLOGO", FALSE, 686, 184, 433, 15, FALSE, FALSE )
--UI_LoadFormImage( frmLOGO, "texture/ui/logo.tga", 686, 184, 0, 0, "", 0, 0 )
UI_AddFormToTemplete( frmLOGO, FORM_LOGIN )
UI_SetFormStyle( frmLOGO, 2 )
--UI_SetIsDrag( frmLOGO, TRUE )
--UI_FormSetHotKey( frmLOGO, ALT_KEY, HOTKEY_X )
UI_FormSetIsEscClose( frmLOGO, FALSE )

-- Flash animation
--imgFlash = UI_CreateCompent( frmLOGO, IMAGE_TYPE, "imgFlash", 708, 164, -145, -8 )
--UI_LoadImage( imgFlash, "texture/ui/Flash.tga", NORMAL, 708, 164, 0, 0 )

--imgLOGO = UI_CreateCompent( frmLOGO, IMAGE_TYPE, "imgLOGO", 390, 170, 0, 0 )
--UI_LoadImage( imgLOGO, "texture/ui/logo.tga", NORMAL, 390, 170, 7, 13 )
