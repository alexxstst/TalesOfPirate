----------------
--Settings UI --
----------------
function addSystemOption(frm,compName,cap,hint,cap1,cap2,y)
	labGame = UI_CreateCompent( frm, LABELEX_TYPE, "labGame", 30, 10, 12, y )
	UI_SetCaption( labGame, cap )
	UI_SetTextColor( labGame, COLOR_BLACK )
	UI_SetLabelExFont( labGame, DEFAULT_FONT, TRUE, COLOR_WHITE )
	UI_SetHint(labGame,hint)
	
	labGame = UI_CreateCompent( frm, LABELEX_TYPE, "labGame", 30, 10, 95, y )
	UI_SetCaption( labGame, cap1 )
	UI_SetTextColor( labGame, COLOR_BLACK )
	UI_SetLabelExFont( labGame, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	chkHelpmodel1 = UI_CreateCompent( frm, CHECK_TYPE, "chkHelpmodel1", 10, 10, 122, y )
	UI_LoadImage( chkHelpmodel1, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
	UI_LoadImage( chkHelpmodel1, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )
	
	labGame = UI_CreateCompent( frm, LABELEX_TYPE, "labGame", 30, 10, 140, y )
	UI_SetCaption( labGame, cap2 )
	UI_SetTextColor( labGame, COLOR_BLACK )
	UI_SetLabelExFont( labGame, DEFAULT_FONT, TRUE, COLOR_WHITE )
	
	
	chkHelpmodel2 = UI_CreateCompent( frm, CHECK_TYPE, "chkHelpmodel2", 10, 10, 167, y )
	UI_LoadImage( chkHelpmodel2, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
	UI_LoadImage( chkHelpmodel2, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )
	
	cbxEffmodel = UI_CreateCompent( frm, CHECK_GROUP_TYPE, compName, 0, 0, 0, 0 )
	UI_AddGroupBox( cbxEffmodel, chkHelpmodel1 )
	UI_AddGroupBox( cbxEffmodel, chkHelpmodel2 )
end

-----------------------------------------------------------------------
-- settings menu (esc)
-----------------------------------------------------------------------
frmSystem = UI_CreateForm( "frmSystem", FALSE, 190, 274, 150, 200, TRUE, FALSE )
UI_FormSetHotKey( frmSystem, ALT_KEY, HOTKEY_O ) --'O'
UI_ShowForm( frmSystem, TRUE )
UI_AddFormToTemplete( frmSystem, 1 )
UI_SetIsDrag( frmSystem, TRUE )
UI_SetFormStyle( frmSystem, 0 )
--UI_SetAlpha( frmSystem, 200 )

imgSystemT = UI_CreateCompent( frmSystem, IMAGE_TYPE, "imgSystemT", 190, 128, 0, 0 )
UI_LoadImage( imgSystemT, "texture/ui/system1.png", NORMAL, 190, 128, 0, 0 )
imgSystemT2 = UI_CreateCompent( frmSystem, IMAGE_TYPE, "imgSystemT2", 190, 163, 0, 128 )
UI_LoadImage( imgSystemT2, "texture/ui/system1.dds", NORMAL, 190, 163, 0, 93 )
imgSystemD = UI_CreateCompent( frmSystem, IMAGE_TYPE, "imgSystemD", 190, 18, 0, 291 )
UI_LoadImage( imgSystemD, "texture/ui/sure.dds", NORMAL, 190, 18, 0, 101 )

btnClose = UI_CreateCompent( frmSystem, BUTTON_TYPE, "btnClose", 14, 14, 169, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

btnVideo = UI_CreateCompent( frmSystem, BUTTON_TYPE, "btnVideo", 121, 26, 35, 61 )
UI_LoadButtonImage( btnVideo, "texture/ui/SysBotton1.dds", 121, 26, 0, 0, FALSE )

btnAudio = UI_CreateCompent( frmSystem, BUTTON_TYPE, "btnAudio", 121, 26, 35, 92 )
UI_LoadButtonImage( btnAudio, "texture/ui/SysBotton1.dds", 121, 26, 121, 0, FALSE )

btnGame = UI_CreateCompent( frmSystem, BUTTON_TYPE, "btnGame", 121, 26, 35, 123 )
UI_LoadButtonImage( btnGame, "texture/ui/forge3.tga", 121, 26, 0, 133, FALSE )

btnChange = UI_CreateCompent( frmSystem, BUTTON_TYPE, "btnChange", 121, 26, 35, 154 )
UI_LoadButtonImage( btnChange, "texture/ui/SysBotton1.dds", 121, 26, 0, 105, FALSE )

btnRelogin = UI_CreateCompent( frmSystem, BUTTON_TYPE, "btnRelogin", 121, 26, 35, 185 )
UI_LoadButtonImage( btnRelogin, "texture/ui/SysBotton1.dds", 121, 26, 121, 105, FALSE )

btnExit = UI_CreateCompent( frmSystem, BUTTON_TYPE, "btnExit", 121, 26, 35, 216 )
UI_LoadButtonImage( btnExit, "texture/ui/SysBotton2.dds", 121, 26, 0, 0, FALSE )

btnOfflineMode = UI_CreateCompent( frmSystem, BUTTON_TYPE, "btnOfflineMode", 121, 26, 35, 247 )
UI_LoadButtonImage( btnOfflineMode, "texture/ui/corsairs/SysBotton5.png", 121, 26, 0, 0, FALSE )

---------------------
-- Volume Settings --
---------------------
frmAudio = UI_CreateForm( "frmAudio",  FALSE, 185, 100, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmAudio, "texture/ui/Sure.dds", 185, 100, 0, 0, "", 0, 0 )
UI_ShowForm( frmAudio, FALSE )
UI_AddFormToTemplete( frmAudio, FORM_MAIN )
--UI_FormSetHotKey( frmAudio, ALT_KEY, HOTKEY_X ) --'O'
UI_SetFormStyle( frmAudio, 0 )
UI_SetIsDrag( frmAudio, TRUE )
--UI_SetAlpha( frmAudio, 200 )

btnClose = UI_CreateCompent( frmAudio, BUTTON_TYPE, "btnClose", 14, 14, 165, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )


-- Volume Settings Label
labTitle = UI_CreateCompent( frmAudio, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Volume Settings")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Music Label
labMusic = UI_CreateCompent( frmAudio, LABELEX_TYPE, "labMusic", 23, 11, 33, 32 )
UI_SetCaption( labMusic, "Music" )
UI_SetTextColor( labMusic, COLOR_BLACK )
UI_SetLabelExFont( labMusic, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Sound Effect Label
labMidi = UI_CreateCompent( frmAudio, LABELEX_TYPE, "labMidi", 23, 11, 33, 52 )
UI_SetCaption( labMidi, "Sound Effect" )
UI_SetTextColor( labMidi, COLOR_BLACK )
UI_SetLabelExFont( labMidi, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgAudioMusic = UI_CreateCompent( frmAudio, IMAGE_TYPE, "imgAudioMusic", 109, 12, 64, 31 )
UI_LoadImage( imgAudioMusic, "texture/ui/Sysbotton2.dds", NORMAL, 109, 12, 88, 244 )

proAudioMusic = UI_CreateCompent( frmAudio, PROGRESS_TYPE, "proAudioMusic", 95, 9, 71, 32 )
UI_LoadScaleImage( proAudioMusic, "texture/ui/Sysbotton2.dds", PROGRESS_PROGRESS, 95, 9, 88, 234, 1.0, 1.0 )
UI_SetProgressActiveMouse( proAudioMusic, 1 )

imgAudioMidi = UI_CreateCompent( frmAudio, IMAGE_TYPE, "imgAudioMidi", 109, 12, 64, 52 )
UI_LoadImage( imgAudioMidi, "texture/ui/Sysbotton2.dds", NORMAL, 109, 12, 88, 244)

proAudioMidi = UI_CreateCompent( frmAudio, PROGRESS_TYPE, "proAudioMidi", 95, 9, 71, 53 )
UI_LoadScaleImage( proAudioMidi, "texture/ui/Sysbotton2.dds", PROGRESS_PROGRESS, 95, 9, 88, 234, 1.0, 1.0 )
UI_SetProgressActiveMouse( proAudioMidi, 1 )

-- Confirm Button
btnYes = UI_CreateCompent( frmAudio, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

-- Cancel Button
btnNo = UI_CreateCompent( frmAudio, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

-----------------------------------------------------------------------
-- Display Settings
-----------------------------------------------------------------------
frmVideo = UI_CreateForm( "frmVideo", FALSE, 190, 251, 150, 150, FALSE, FALSE )
--UI_FormSetHotKey( frmVideo, ALT_KEY, HOTKEY_O)
UI_ShowForm( frmVideo, FALSE )
UI_AddFormToTemplete( frmVideo, 1 )
UI_SetIsDrag( frmVideo, TRUE )
--UI_SetFormStyle( frmVideo, 0 )
--UI_SetAlpha( frmVideo, 200 )

imgVideoT = UI_CreateCompent( frmVideo, IMAGE_TYPE, "imgVideoT", 190, 251, 0, 0 )
UI_LoadImage( imgVideoT, "texture/ui/video.dds", NORMAL, 190, 251, 0, 0 )

btnClose = UI_CreateCompent( frmVideo, BUTTON_TYPE, "btnClose", 14, 14, 165, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-- imgVideoTitle = UI_CreateCompent( frmVideo, IMAGE_TYPE, "imgVideoTitle", 49, 13, 7, 3 )
-- UI_LoadImage( imgVideoTitle, "texture/ui/Word.dds", NORMAL, 49, 13, 90, 29 )

-- Display Settings
labTitle = UI_CreateCompent( frmVideo, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Display Settings")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmVideo, BUTTON_TYPE, "btnYes", 41, 19, 74, 206 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmVideo, BUTTON_TYPE, "btnNo", 41, 19, 122, 206 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

chkTextureHigh = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkTextureHigh", 10, 10, 102, 46 )
UI_LoadImage( chkTextureHigh, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkTextureHigh, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

chkTextureNormal = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkTextureNormal", 10, 10, 134, 46 )
UI_LoadImage( chkTextureNormal, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkTextureNormal, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

chkTextureLow = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkTextureLow", 10, 10, 165, 46 )
UI_LoadImage( chkTextureLow, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkTextureLow, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

cbxTexture = UI_CreateCompent( frmVideo, CHECK_GROUP_TYPE, "cbxTexture", 0, 0, 0, 0 )
UI_AddGroupBox( cbxTexture, chkTextureHigh )
UI_AddGroupBox( cbxTexture, chkTextureNormal )
UI_AddGroupBox( cbxTexture, chkTextureLow )

chkMovieOn = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkMovieOn", 10, 10, 102, 66 )
UI_LoadImage( chkMovieOn, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkMovieOn, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

chkMovieOff = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkMovieOff", 10, 10, 134, 66 )
UI_LoadImage( chkMovieOff, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkMovieOff, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

cbxMovie = UI_CreateCompent( frmVideo, CHECK_GROUP_TYPE, "cbxMovie", 0, 0, 0, 0 )
UI_AddGroupBox( cbxMovie, chkMovieOn )
UI_AddGroupBox( cbxMovie, chkMovieOff )

chkCameraOn = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkCameraOn", 10, 10, 102, 86)
UI_LoadImage( chkCameraOn, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkCameraOn, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

chkCameraOff = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkCameraOff", 10, 10, 134, 86 )
UI_LoadImage( chkCameraOff, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkCameraOff, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

cbxCamera = UI_CreateCompent( frmVideo, CHECK_GROUP_TYPE, "cbxCamera", 0, 0, 0, 0 )
UI_AddGroupBox( cbxCamera, chkCameraOn )
UI_AddGroupBox( cbxCamera, chkCameraOff )

chkViewFar = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkViewFar", 10, 10, 102, 106 )
--UI_LoadImage( chkViewFar, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
--UI_LoadImage( chkViewFar, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

chkViewNear = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkViewNear", 10, 10, 134, 106 )
--UI_LoadImage( chkViewNear, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
--UI_LoadImage( chkViewNear, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

cbxView = UI_CreateCompent( frmVideo, CHECK_GROUP_TYPE, "cbxView", 0, 0, 0, 0 )
UI_AddGroupBox( cbxView, chkViewFar )
UI_AddGroupBox( cbxView, chkViewNear )

chkTrailOn = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkTrailOn", 10, 10, 102, 106 )
UI_LoadImage( chkTrailOn, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkTrailOn, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

chkTrailOff = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkTrailOff", 10, 10, 134, 106 )
UI_LoadImage( chkTrailOff, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkTrailOff, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

cbxTrail = UI_CreateCompent( frmVideo, CHECK_GROUP_TYPE, "cbxTrail", 0, 0, 0, 0 )
UI_AddGroupBox( cbxTrail, chkTrailOn )
UI_AddGroupBox( cbxTrail, chkTrailOff )

chkColor32 = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkColor32", 10, 10, 102, 126 )
UI_LoadImage( chkColor32, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkColor32, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

chkColor16 = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkColor16", 10, 10, 134, 126 )
UI_LoadImage( chkColor16, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkColor16, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

cbxColor = UI_CreateCompent( frmVideo, CHECK_GROUP_TYPE, "cbxColor", 0, 0, 0, 0 )
UI_AddGroupBox( cbxColor, chkColor32 )
UI_AddGroupBox( cbxColor, chkColor16 )

cboResolution  = UI_CreateCompent( frmVideo, COMBO_TYPE, "cboResolution", 98, 20, 97, 181 ) --98,20,97,181
UI_ComboSetStyle( cboResolution, TRUE )
UI_LoadComboImage( cboResolution, "", 81, 20, 106, 39, "texture/ui/search.tga", 1, 1, 130, 180, FALSE )
UI_ComboSetTextColor( cboResolution, COLOR_BLACK )
lstMap = UI_GetList( cboResolution )
UI_SetListFontColor( lstMap, COLOR_BLACK, COLOR_BLACK )
UI_SetSize( lstMap, 97, 120 )
UI_SetMargin( lstMap, 16,6,16,0 )
UI_SetListRowHeight( lstMap, 18 )
UI_LoadImage( lstMap, "texture/ui/search.tga", COMPENT_BACK, 51, 60, 0, 133 )  --51, 60,  97, 181
UI_AddListText( lstMap, "800x600" )
UI_AddListText( lstMap, "1152x648" )
UI_AddListText( lstMap, "1280x720" )
UI_AddListText( lstMap, "1366x768" )
UI_AddListText( lstMap, "1600x900" )
UI_AddListText( lstMap, "1920x1080" )

UI_SetListIsMouseFollow( lstMap, TRUE)
UI_ListLoadSelectImage( lstMap, "texture/ui/SystemBotton3.tga", 79, 18, 0, 227  )
UI_ComboSetTextColor( lstMap, COLOR_WHITE )

chkFull = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkFull", 10, 10, 102, 166 )
UI_LoadImage( chkFull, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkFull, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

chkWindow = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkWindow", 10, 10, 134, 166 )
UI_LoadImage( chkWindow, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkWindow, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

cbxModel = UI_CreateCompent( frmVideo, CHECK_GROUP_TYPE, "cbxModel", 0, 0, 0, 0 )
UI_AddGroupBox( cbxModel, chkFull )
UI_AddGroupBox( cbxModel, chkWindow )

chkHigh = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkHigh", 10, 10, 102, 146 )
UI_LoadImage( chkHigh, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkHigh, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

chkNormal = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkNormal", 10, 10, 134, 146 )
UI_LoadImage( chkNormal, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkNormal, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

chkLow = UI_CreateCompent( frmVideo, CHECK_TYPE, "chkLow", 10, 10, 165, 146 )
UI_LoadImage( chkLow, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkLow, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

cbxQuality = UI_CreateCompent( frmVideo, CHECK_GROUP_TYPE, "cbxQuality", 0, 0, 0, 0 )
UI_AddGroupBox( cbxQuality, chkHigh )
UI_AddGroupBox( cbxQuality, chkNormal )
UI_AddGroupBox( cbxQuality, chkLow )

-----------------------------------------------------------------------
-- game settings
-----------------------------------------------------------------------
frmGame = UI_CreateForm( "frmGame", FALSE, 190, 350, 150, 150, FALSE, FALSE )
--UI_FormSetHotKey( frmGame, ALT_KEY, HOTKEY_Z )
UI_ShowForm( frmGame, FALSE )
UI_AddFormToTemplete( frmGame, 1 )
UI_SetIsDrag( frmGame, TRUE )
--UI_SetFormStyle( frmGame, 0 )
--UI_SetAlpha( frmGame, 200 )

imgVideoT = UI_CreateCompent( frmGame, IMAGE_TYPE, "imgVideoT", 190, 350, 0, 0 )
UI_LoadImage( imgVideoT, "texture/ui/Settings_Game.dds", NORMAL, 190, 350, 0, 0 )

btnClose = UI_CreateCompent( frmGame, BUTTON_TYPE, "btnClose", 14, 14, 165, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labGame = UI_CreateCompent( frmGame, LABELEX_TYPE, "labGame", 47, 11, 7, 3 )
UI_SetCaption( labGame, "Game Settings" )
UI_SetTextColor( labGame, COLOR_BLACK )
UI_SetLabelExFont( labGame, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmGame, BUTTON_TYPE, "btnYes", 41, 19, 74, 326 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetButtonModalResult( btnYes, BUTTON_CLOSE )

btnNo = UI_CreateCompent( frmGame, BUTTON_TYPE, "btnNo", 41, 19, 122, 326 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )

--------------------------------------------------------
-- Corsairs Online Alpha settings By: Mdr.st May 2020 --
--------------------------------------------------------
addSystemOption(frmGame,"cbxRunmodel","Movement","Use Server Movement","Off","On",46)
addSystemOption(frmGame,"cbxLockmodel","Auto-Lock","Lock Bag on Logout","Off","On",66)
addSystemOption(frmGame,"cbxHelpmodel","Help","Show Help","Hide","Show",86)
addSystemOption(frmGame,"cbxCameraMode_p","Camera","Camera display viewing","Top1","Top2",106)
addSystemOption(frmGame,"cbxAppmodel","Apparels","Display player's apparel","Hide","Show",126)
addSystemOption(frmGame,"cbxEffmodel","Effects","Show Effects","Hide","Show",146)
addSystemOption(frmGame,"cbxShowMounts_p","Mounts","Display player's mount","Hide","Show",166)
addSystemOption(frmGame,"cbxStatemodel","Active State","Show active states","Hide","Show",186)
addSystemOption(frmGame,"cbxEnemyNames_p","Friend/Enemy","Highlight friendly and enemy's names","Hide","Show",206)
addSystemOption(frmGame,"cbxShowBars_p","HP/SP Bars","Show HP/SP bars below characters","Hide","Show",226)
addSystemOption(frmGame,"cbxShowPercentages_p","HP/SP Values","Display mode for HP/SP bars","Raw","%",246) 
addSystemOption(frmGame,"cbxShowInfo_p","Player's Info","Show other player's information","Off","On",266) 
addSystemOption(frmGame,"cbxFramerate_p", "FPS", "Unlocks the framerate", "30","60", 286)

-----------------------------------------------------------------------
-- switch character settings
-----------------------------------------------------------------------
frmAskChange = UI_CreateForm( "frmAskChange", FALSE, 185, 100, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmAskChange, "texture/ui/Sure.dds", 185, 100, 0, 0, "", 0, 0 )

UI_ShowForm( frmAskChange, FALSE )
UI_AddFormToTemplete( frmAskChange, FORM_MAIN )
UI_SetFormStyle( frmAskChange, 0 )
--UI_SetAlpha( frmAskChange, 200 )

labchange = UI_CreateCompent( frmAskChange, LABELEX_TYPE, "labchange", 47, 11, 7, 3 )
UI_SetCaption( labchange, "Switch Character" )
UI_SetTextColor( labchange, COLOR_BLACK )
UI_SetLabelExFont( labchange, DEFAULT_FONT, TRUE, COLOR_WHITE )

labLogin = UI_CreateCompent( frmAskChange, LABELEX_TYPE, "labLogin", 59, 11, 35, 40 )
UI_SetCaption( labLogin, "Confirm to switch \ncharacter?" )
UI_SetTextColor( labLogin, COLOR_BLACK )
UI_SetLabelExFont( labLogin, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmAskChange, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmAskChange, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

-----------------------------------------------------------------------
-- Logout settings
-----------------------------------------------------------------------
frmAskRelogin = UI_CreateForm( "frmAskRelogin", FALSE, 185, 100, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmAskRelogin, "texture/ui/Sure.dds", 185, 100, 0, 0, "", 0, 0 )

UI_ShowForm( frmAskRelogin, FALSE )
UI_AddFormToTemplete( frmAskRelogin, FORM_MAIN )
UI_SetFormStyle( frmAskRelogin, 0 )
--UI_SetAlpha( frmAskRelogin, 200 )

labchange = UI_CreateCompent( frmAskRelogin, LABELEX_TYPE, "labchange", 47, 11, 7, 3 )
UI_SetCaption( labchange, "Logout" )
UI_SetTextColor( labchange, COLOR_BLACK )
UI_SetLabelExFont( labchange, DEFAULT_FONT, TRUE, COLOR_WHITE )

labLogin = UI_CreateCompent( frmAskRelogin, LABELEX_TYPE, "labLogin", 59, 11, 35, 40 )
UI_SetCaption( labLogin, "Confirm to log out?" )
UI_SetTextColor( labLogin, COLOR_BLACK )
UI_SetLabelExFont( labLogin, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmAskRelogin, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmAskRelogin, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

-----------------------------------------------------------------------
-- exit settings
-----------------------------------------------------------------------
frmAskExit = UI_CreateForm( "frmAskExit",  FALSE, 185, 100, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmAskExit, "texture/ui/Sure.dds", 185, 100, 0, 0, "", 0, 0 )

UI_ShowForm( frmAskExit, FALSE )
UI_AddFormToTemplete( frmAskExit, FORM_MAIN )
--UI_FormSetHotKey( frmAskExit, ALT_KEY, HOTKEY_X ) --'O'
UI_SetFormStyle( frmAskExit, 0 )
--UI_SetAlpha( frmAskExit, 200 )

labchange = UI_CreateCompent( frmAskExit, LABELEX_TYPE, "labchange", 47, 11, 7, 3 )
UI_SetCaption( labchange, "Exit Game" )
UI_SetTextColor( labchange, COLOR_BLACK )
UI_SetLabelExFont( labchange, DEFAULT_FONT, TRUE, COLOR_WHITE )

labLogin = UI_CreateCompent( frmAskExit, LABELEX_TYPE, "labLogin", 63, 5, 11, 45 )
UI_SetCaption( labLogin, "Do you wish to exit the game?" )
UI_SetTextColor( labLogin, COLOR_BLACK )
UI_SetLabelExFont( labLogin, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmAskExit, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmAskExit, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

-----------------------------------------------------------------------
-- offline mode settings
-----------------------------------------------------------------------
frmAskOfflineMode = UI_CreateForm( "frmAskOfflineMode",  FALSE, 185, 100, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmAskOfflineMode, "texture/ui/Sure.dds", 185, 100, 0, 0, "", 0, 0 )

UI_ShowForm( frmAskOfflineMode, FALSE )
UI_AddFormToTemplete( frmAskOfflineMode, FORM_MAIN )
UI_SetFormStyle( frmAskOfflineMode, 0 )

labTitle = UI_CreateCompent( frmAskOfflineMode, LABELEX_TYPE, "labTitle", 47, 11, 7, 3 )
UI_SetCaption( labTitle, "Offline Mode" )
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labBody = UI_CreateCompent( frmAskOfflineMode, LABELEX_TYPE, "labBody", 63, 5, 11, 45 )
UI_SetCaption( labBody, "Confirm to offline mode?" )
UI_SetTextColor( labBody, COLOR_BLACK )
UI_SetLabelExFont( labBody, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmAskOfflineMode, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmAskOfflineMode, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
