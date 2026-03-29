------------------------
-- Background Loading --
------------------------
frmLoading = UI_CreateForm("frmLoading", FALSE, 1920, 1080, 0, 0, FALSE, FALSE)
UI_SetFormStyle(frmLoading, 2)
UI_ShowForm(frmLoading, TRUE)
UI_AddFormToTemplete(frmLoading, FORM_SWITCH_SCENE)
UI_FormSetIsEscClose(frmLoading, FALSE)

imgLoadingBg = UI_CreateCompent(frmLoading, IMAGE_TYPE, "imgLoadingBg", 1920, 1080, 0, 0)
UI_LoadImage(imgLoadingBg, "texture/ui/LOADING/LoadBgScene.jpg", NORMAL, 1920, 1080, 0, 0)

imgProgressBar = UI_CreateCompent(frmLoading, IMAGE_TYPE, "imgProgressBar", 854, 180, 294, 122)
UI_LoadImage(imgProgressBar, "texture/ui/imgProgressBar.png", NORMAL, 854, 180, 0, 0)

imgProgressLoad = UI_CreateCompent(frmLoading, IMAGE_TYPE, "imgProgressLoad", 1338, 180, 294, 122)
UI_LoadImage(imgProgressLoad, "texture/ui/imgProgressLoad.png", NORMAL, 854, 180, 0, 37)

labTip = UI_CreateCompent(frmLoading, LABELEX_TYPE, "labTip", 800, 13, 220, 750)
UI_SetCaption(labTip, "2. This is second tip, you can click ALT + E to open inventory")
UI_SetTextColor(labTip, COLOR_WHITE)
UI_SetLabelExFont(labTip, TIPFONT, FALSE, COLOR_WHITE)

------------------------
-- Connecting Loading --
------------------------
frmConnect = UI_CreateForm("frmConnect", FALSE, 198, 72, 396, 80, FALSE, FALSE)
UI_LoadFormImage(frmConnect, "texture/ui/SysBotton2.dds", 198, 72, 0, 111, "", 0, 0)
--UI_FormSetHotKey( frmConnect, ALT_KEY, HOTKEY_L )
UI_SetFormStyle(frmConnect, 1)
UI_AddFormToTemplete(frmConnect, FORM_SWITCH_SCENE)
UI_FormSetIsEscClose(frmConnect, FALSE)

labConnect = UI_CreateCompent(frmConnect, LABELEX_TYPE, "labConnect", 52, 13, 70, 24)
UI_SetCaption(labConnect, "Please wait")
UI_SetTextColor(labConnect, COLOR_BLACK)
UI_SetLabelExFont(labConnect, DEFAULT_FONT, TRUE, COLOR_WHITE)

imgLoading = UI_CreateCompent(frmConnect, IMAGE_FLASH_TYPE, "imgLoading", 135, 8, 20, 51)
UI_LoadScaleImage(imgLoading, "texture/ui/SysBotton2.dds", 0, 26, 8, 20, 185, 1.0, 1.0)
