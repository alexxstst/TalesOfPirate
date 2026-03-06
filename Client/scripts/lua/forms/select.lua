---------------------------------------
-- Character Selection Screen Button --
---------------------------------------
frmSelect = UI_CreateForm( "frmSelect", FALSE, 407, 36, 200, 200, TRUE, FALSE )
UI_ShowForm( frmSelect, TRUE )
UI_AddFormToTemplete( frmSelect, FORM_SELECT )
--UI_SetFormStyleEx( frmSelect, 7, 0, 2 )
UI_FormSetIsEscClose( frmSelect, FALSE )
UI_SetFormStyle( frmSelect, 2 )

imgSelect = UI_CreateCompent( frmSelect, IMAGE_TYPE, "imgSelect",407, 36, -40, 0 )
UI_LoadImage( imgSelect, "texture/ui/corsairs/a001.png", NORMAL, 407, 36, 0, 129 )

btnChangePass = UI_CreateCompent( frmSelect, BUTTON_TYPE, "btnChangePass", 66, 21, 269, 8 )
UI_LoadButtonImage( btnChangePass, "texture/ui/corsairs/a001.png", 66, 21, 162, 165, FALSE )

-- Create
btnCreate = UI_CreateCompent( frmSelect, BUTTON_TYPE, "btnCreate", 46, 22, -16, 7 )
UI_LoadButtonImage( btnCreate, "texture/ui/select4.tga", 46, 22, 92, 113, FALSE )

-- Delete
btnDel = UI_CreateCompent( frmSelect, BUTTON_TYPE, "btnDel", 46, 22, 35, 7 )
UI_LoadButtonImage( btnDel, "texture/ui/select4.tga", 46, 22, 138, 113, FALSE )

-- Accept
btnYes = UI_CreateCompent( frmSelect, BUTTON_TYPE, "btnYes", 46, 22, 86, 7 )
UI_LoadButtonImage( btnYes, "texture/ui/select4.tga", 46, 22, 184, 113, FALSE )

-- Back
btnNo = UI_CreateCompent( frmSelect, BUTTON_TYPE, "btnNo", 46, 22, 137, 7 )
UI_LoadButtonImage( btnNo, "texture/ui/select4.tga", 46, 22, 0, 203, TRUE )

-- Edit
btnAlter = UI_CreateCompent( frmSelect, BUTTON_TYPE, "btnAlter", 66, 21, 198 , 8 )
UI_LoadButtonImage( btnAlter, "texture/ui/corsairs/a001.png", 66, 21, 96, 165, FALSE )

-- Previous Page
btnPrevPage = UI_CreateCompent (frmSelect, BUTTON_TYPE, "btnPrevPage", 20,  20 , 148, 11  )
UI_LoadButtonImage (btnPrevPage, "texture/ui/new/a001.png", 20,20, 228,124, TRUE)
UI_SetIsShow(btnPrevPage, FALSE)

-- Next Page
btnNextPage = UI_CreateCompent (frmSelect, BUTTON_TYPE, "btnNextPage", 20,  20 , 208, 11 )
UI_LoadButtonImage (btnNextPage, "texture/ui/new/a001.png", 20,20,228,104, TRUE)
UI_SetIsShow(btnNextPage, FALSE)

-- Page label
labPage = UI_CreateCompent (frmSelect, LABEL_TYPE, "labPage", 2,2, 180, 14)
UI_SetTextColor( labPage, COLOR_WHITE)
UI_SetIsShow(labPage, FALSE)

-----------------------------
-- Create race quit button --
-----------------------------
frmQuit = UI_CreateForm( "frmQuit", FALSE, 98, 36, 380, 507, TRUE, FALSE )
UI_ShowForm( frmQuit, TRUE )
UI_AddFormToTemplete( frmQuit, FORM_CREATE_CHA )
UI_SetFormStyleEx( frmQuit, 7, 0, 2 )
UI_FormSetIsEscClose( frmQuit, FALSE )

imgQuit = UI_CreateCompent( frmQuit, IMAGE_TYPE, "imgQuit", 98, 36, 0, 0 )
UI_LoadImage( imgQuit, "texture/ui/ShipBuildD.tga", NORMAL, 98, 36, 0, 179 )

--logo
--imgLogo = UI_CreateCompent( frmQuit, IMAGE_TYPE, "imgLogo", 242, 115, -312, -510 )
--UI_LoadImage( imgLogo, "texture/ui/logot.tga", NORMAL, 242, 115, 0, 0 )

btnNo = UI_CreateCompent( frmQuit, BUTTON_TYPE, "btnNo", 46, 22, 26, 7 )
UI_LoadButtonImage( btnNo, "texture/ui/select4.tga", 46, 22, 0, 203, TRUE )

imgBack = UI_CreateCompent( frmQuit, IMAGE_TYPE, "imgBack", 1198, 768, -380, -675 )
UI_LoadImage( imgBack, "texture/ui/select4.tga", NORMAL, 1, 1, 232, 1 )

-----------------
-- Create Race --
-----------------
frmFound = UI_CreateForm( "frmFound", FALSE, 454, 373, 173, 112, TRUE, FALSE )
UI_ShowForm( frmFound, TRUE )
UI_AddFormToTemplete( frmFound, FORM_CREATE_CHA )
--UI_AddFormToTemplete( frmFound, 1 )
UI_FormSetIsEscClose( frmFound, FALSE )
--UI_FormSetHotKey( frmFound, ALT_KEY, HOTKEY_I )

--logo
--imgLogo = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgLogo", 242, 115, -134, -98 )
--UI_LoadImage( imgLogo, "texture/ui/logot.tga", NORMAL, 242, 115, 0, 0 )

--imgBack = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgBack", 1198, 768, -173, -112 )
--UI_LoadImage( imgBack, "texture/ui/select4.tga", NORMAL, 1, 1, 232, 1 )

imgFoundTop = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgFoundTop", 256, 256, 0, 0 )
UI_LoadImage( imgFoundTop, "texture/ui/select1.tga", NORMAL, 256, 256, 0, 0 )
imgFoundDown = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgFoundDown", 256, 117, 0, 256 )
UI_LoadImage( imgFoundDown, "texture/ui/select3.tga", NORMAL, 256, 117, 0, 0 )
imgFoundTopR = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgFoundTopR", 198, 256, 256, 0 )
UI_LoadImage( imgFoundTopR, "texture/ui/select2.tga", NORMAL, 198, 256, 0, 0 )
imgFoundDownR = UI_CreateCompent( frmFound, IMAGE_TYPE, "imgFoundDownR", 198, 117, 256, 256 )
UI_LoadImage( imgFoundDownR, "texture/ui/select3.tga", NORMAL, 198, 117, 0, 117 )

memChaDescribe  = UI_CreateCompent( frmFound, MEMO_TYPE, "memChaDescribe", 162, 154, 286, 29 )
UI_SetTextColor( memChaDescribe, COLOR_BLACK )
UI_SetLabelExFont( memChaDescribe, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetMemoMaxNumPerRow( memChaDescribe, 24 )
UI_SetMemoPageShowNum( memChaDescribe, 7 )

edtName = UI_CreateCompent( frmFound, EDIT_TYPE, "edtName", 117, 12, 310, 198 )
UI_SetTextColor( edtName, COLOR_BLACK )
UI_SetEditMaxNum( edtName, 16 )
UI_SetEditMaxNumVisible( edtName, 16 )

labHairShow = UI_CreateCompent( frmFound, LABELEX_TYPE, "labHairShow", 65, 12, 341, 242 )
UI_SetCaption( labHairShow, "Long Hair" )
UI_SetTextColor( labHairShow, COLOR_BLACK )
UI_SetLabelExFont( labHairShow, DEFAULT_FONT, TRUE, COLOR_WHITE )
labFaceShow = UI_CreateCompent( frmFound, LABELEX_TYPE, "labFaceShow", 65, 12, 341, 284 )
UI_SetCaption( labFaceShow, "Nightmare" )
UI_SetTextColor( labFaceShow, COLOR_BLACK )
UI_SetLabelExFont( labFaceShow, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnLeftHair = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnLeftHair", 40, 24, 282, 234 )
UI_LoadButtonImage( btnLeftHair, "texture/ui/select4.tga", 40, 24, 110, 24, TRUE )

btnLeftFace = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnLeftFace", 40, 24, 282, 277 )
UI_LoadButtonImage( btnLeftFace, "texture/ui/select4.tga", 40, 24, 110, 24, TRUE )

btnLeft3d = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnLeft3d", 32, 38, 28, 291 )
UI_LoadButtonImage( btnLeft3d, "texture/ui/select4.tga", 32, 38, 0, 38, TRUE )

btnRightHair = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnRightHair", 40, 24, 393, 234 )
UI_LoadButtonImage( btnRightHair, "texture/ui/select4.tga", 40, 24, 110, 0, TRUE )

btnRightFace = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnRightFace", 40, 24, 393, 277 )
UI_LoadButtonImage( btnRightFace, "texture/ui/select4.tga", 40, 24, 110, 0, TRUE )

btnRight3d = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnRight3d", 32, 38, 204, 291 )
UI_LoadButtonImage( btnRight3d, "texture/ui/select4.tga",  32, 38, 0, 0, TRUE )

ui3dCreateCha = UI_CreateCompent( frmFound, UI3D_COMPENT, "ui3dCreateCha", 160, 300, 52, 35 )

btnYes = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnYes", 46, 22, 303, 319 )
UI_LoadButtonImage( btnYes, "texture/ui/select4.tga", 46, 22, 0, 113, FALSE )

btnNo = UI_CreateCompent( frmFound, BUTTON_TYPE, "btnNo", 46, 22, 376, 319 )
UI_LoadButtonImage( btnNo, "texture/ui/select4.tga", 46, 22, 46, 113, FALSE )

-------------------
-- Starting City --
-------------------
frmCity = UI_CreateForm( "frmCity", FALSE, 461, 385, 173, 112, TRUE, FALSE )
UI_ShowForm( frmCity, TRUE )
UI_AddFormToTemplete( frmCity, FORM_CREATE_CHA )
UI_AddFormToTemplete( frmCity, 1 )
UI_FormSetIsEscClose( frmCity, FALSE )
--UI_FormSetHotKey( frmCity, ALT_KEY, HOTKEY_V )

imgCity00 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity00", 256, 256, 0, 0 )
UI_LoadImage( imgCity00, "texture/ui/s1.tga", NORMAL, 256, 256, 0, 0 )
imgCity01 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity01", 203, 256, 256, 0 )
UI_LoadImage( imgCity01, "texture/ui/s2.tga", NORMAL, 203, 256, 0, 0 )
imgCity02 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity02", 256, 129, 0, 256 )
UI_LoadImage( imgCity02, "texture/ui/s3.tga", NORMAL, 256, 129, 0, 0 )
imgCity03 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity03", 203, 129, 256, 256 )
UI_LoadImage( imgCity03, "texture/ui/s4.tga", NORMAL, 203, 129, 0, 0 )

imgCity1 = UI_CreateCompent( frmCity, BUTTON_TYPE, "imgCity1", 157, 198, 8, 157 )

imgCity10 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity10", 256, 256, 0, 0 )
UI_LoadImage( imgCity10, "texture/ui/s5.tga", NORMAL, 256, 256, 0, 0 )
imgCity11 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity11", 203, 256, 256, 0 )
UI_LoadImage( imgCity11, "texture/ui/s6.tga", NORMAL, 203, 256, 0, 0 )
imgCity12 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity12", 256, 129, 0, 256 )
UI_LoadImage( imgCity12, "texture/ui/s7.tga", NORMAL, 256, 129, 0, 0 )
imgCity13 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity13", 203, 129, 256, 256 )
UI_LoadImage( imgCity13, "texture/ui/s8.tga", NORMAL, 203, 129, 0, 0 )

imgCity2 = UI_CreateCompent( frmCity, BUTTON_TYPE, "imgCity2", 226, 152, 218, 244 )

imgCity20= UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity20", 256, 256, 0, 0 )
UI_LoadImage( imgCity20, "texture/ui/s9.tga", NORMAL, 256, 256, 0, 0 )
imgCity21 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity21", 203, 256, 256, 0 )
UI_LoadImage( imgCity21, "texture/ui/s10.tga", NORMAL, 203, 256, 0, 0 )
imgCity22 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity22", 256, 129, 0, 256 )
UI_LoadImage( imgCity22, "texture/ui/s11.tga", NORMAL, 256, 129, 0, 0 )
imgCity23 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity23", 203, 129, 256, 256 )
UI_LoadImage( imgCity23, "texture/ui/s12.tga", NORMAL, 203, 129, 0, 0 )

imgCity3 = UI_CreateCompent( frmCity, BUTTON_TYPE, "imgCity3", 391, 92, 52, 61 )

imgCity30 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity30", 256, 256, 0, 0 )
UI_LoadImage( imgCity30, "texture/ui/s13.tga", NORMAL, 256, 256, 0, 0 )
imgCity31 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity31", 203, 256, 256, 0 )
UI_LoadImage( imgCity31, "texture/ui/s14.tga", NORMAL, 203, 256, 0, 0 )
imgCity32 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity32", 256, 129, 0, 256 )
UI_LoadImage( imgCity32, "texture/ui/s15.tga", NORMAL, 256, 129, 0, 0 )
imgCity33 = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgCity33", 203, 129, 256, 256 )
UI_LoadImage( imgCity33, "texture/ui/s16.tga", NORMAL, 203, 129, 0, 0 )

imgQuit = UI_CreateCompent( frmCity, IMAGE_TYPE, "imgQuit", 98, 36, 180, 390 )
UI_LoadImage( imgQuit, "texture/ui/ShipBuildD.tga", NORMAL, 98, 36, 0, 179 )

btnNo = UI_CreateCompent( frmCity, BUTTON_TYPE, "btnNo", 46, 22, 206, 397 )
UI_LoadButtonImage( btnNo, "texture/ui/select4.tga", 46, 22, 0, 203, TRUE )

----------------------------
-- Character Introduction --
----------------------------
frmRoleInfo = UI_CreateForm("frmRoleInfo", TRUE, 600, 450, 50 , 450, TRUE,TRUE)
UI_ShowForm(frmRoleInfo, FALSE )
UI_SetIsDrag(frmRoleInfo, FALSE)
UI_FormSetIsEscClose(frmRoleInfo, FALSE)
UI_SetFormStyle(frmRoleInfo,1)
UI_AddFormToTemplete( frmRoleInfo, FORM_CREATE_CHA )

--UI_FormSetHotKey( frmRoleInfo, ALT_KEY, HOTKEY_Z ) 

imgRoleInfo1 = UI_CreateCompent(frmRoleInfo, IMAGE_TYPE,"imgRoleInfo1", 256, 256, 0, 0)
UI_LoadImage( imgRoleInfo1, "texture/ui/newshelp/create_role_info_01.tga", NORMAL, 256,256, 0, 0)

imgRoleInfo2 = UI_CreateCompent(frmRoleInfo, IMAGE_TYPE,"imgRoleInfo2", 256, 256, 256, 0)
UI_LoadImage( imgRoleInfo2, "texture/ui/newshelp/create_role_info_02.tga", NORMAL, 256,256, 0, 0)

imgRoleInfo3 = UI_CreateCompent(frmRoleInfo, IMAGE_TYPE,"imgRoleInfo3", 88, 256, 512, 0)
UI_LoadImage( imgRoleInfo3, "texture/ui/newshelp/create_role_info_05.tga", NORMAL, 88,256, 0, 0)

imgRoleInfo4 = UI_CreateCompent(frmRoleInfo, IMAGE_TYPE,"imgRoleInfo4", 256, 194, 0, 256)
UI_LoadImage( imgRoleInfo4, "texture/ui/newshelp/create_role_info_03.tga", NORMAL, 256,194, 0, 0)

imgRoleInfo5 = UI_CreateCompent(frmRoleInfo, IMAGE_TYPE,"imgRoleInfo5", 256, 194, 256, 256)
UI_LoadImage( imgRoleInfo5, "texture/ui/newshelp/create_role_info_04.tga", NORMAL, 256,194, 0, 0)

imgRoleInfo6 = UI_CreateCompent(frmRoleInfo, IMAGE_TYPE,"imgRoleInfo6", 88, 194, 512, 256)
UI_LoadImage( imgRoleInfo6, "texture/ui/newshelp/create_role_info_05.tga", NORMAL, 88,194, 88, 0)

labTitle = UI_CreateCompent( frmRoleInfo, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Character Introduction")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnNextStep = UI_CreateCompent(frmRoleInfo, BUTTON_TYPE,"btnNextStep", 229, 39, 178, 400)
UI_LoadButtonImage( btnNextStep,"texture/ui/newshelp/create_role_info_button.tga", 229, 39, 0, 0, FALSE )

---------------------------
-- lance available class --
---------------------------
frmLanchInfo = UI_CreateForm("frmLanchInfo", TRUE, 600, 500, 50 , 450, TRUE,TRUE)
UI_ShowForm(frmLanchInfo, FALSE )
UI_SetIsDrag(frmLanchInfo, FALSE)
UI_FormSetIsEscClose(frmLanchInfo, FALSE)
UI_SetFormStyle(frmLanchInfo,1)
UI_AddFormToTemplete( frmLanchInfo, FORM_CREATE_CHA )

--UI_FormSetHotKey( frmLanchInfo, ALT_KEY, HOTKEY_X ) 

imgLanchInfo1 = UI_CreateCompent(frmLanchInfo, IMAGE_TYPE,"imgLanchInfo1", 256, 256, 0, 0)
UI_LoadImage( imgLanchInfo1, "texture/ui/newshelp/frmLanch01.tga", NORMAL, 256,256, 0, 0)

imgLanchInfo2 = UI_CreateCompent(frmLanchInfo, IMAGE_TYPE,"imgLanchInfo2", 256, 256, 256, 0)
UI_LoadImage( imgLanchInfo2, "texture/ui/newshelp/frmLanch02.tga", NORMAL, 256, 256, 0, 0)

imgLanchInfo3 = UI_CreateCompent(frmLanchInfo, IMAGE_TYPE,"imgLanchInfo3", 256, 194, 0, 256)
UI_LoadImage( imgLanchInfo3, "texture/ui/newshelp/frmLanch03.tga", NORMAL, 256, 194, 0, 0)

imgLanchInfo4 = UI_CreateCompent(frmLanchInfo, IMAGE_TYPE,"imgLanchInfo4", 256, 194, 256, 256)
UI_LoadImage( imgLanchInfo4, "texture/ui/newshelp/frmLanch04.tga", NORMAL, 256, 194, 0, 0)

imgLanchInfo5 = UI_CreateCompent(frmLanchInfo, IMAGE_TYPE,"imgLanchInfo5", 88, 256, 512, 0)
UI_LoadImage( imgLanchInfo5, "texture/ui/newshelp/frmLanch05.tga", NORMAL, 88,256, 0, 0)

imgLanchInfo6 = UI_CreateCompent(frmLanchInfo, IMAGE_TYPE,"imgLanchInfo6", 88, 194, 512, 256)
UI_LoadImage( imgLanchInfo6, "texture/ui/newshelp/frmLanch05.tga", NORMAL, 88,194, 88, 0)

labTitle = UI_CreateCompent( frmLanchInfo, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Available class for Lance")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_1 = UI_CreateCompent(frmLanchInfo, BUTTON_TYPE,"btnViewPlayer_1", 76, 96, 72, 174)
UI_ButtonSetHint(btnViewPlayer_1,"Lance Newbie")

labTitle1 = UI_CreateCompent( frmLanchInfo, LABELEX_TYPE, "labTitle1", 400, 150, 77, 274 )
UI_SetCaption( labTitle1, "Lance Newbie")
UI_SetTextColor( labTitle1, COLOR_BLACK )
UI_SetLabelExFont( labTitle1, DEFAULT_FONT, TRUE, COLOR_WHITE )


btnViewPlayer_2 = UI_CreateCompent(frmLanchInfo, BUTTON_TYPE,"btnViewPlayer_2", 76, 96, 251, 51)
UI_ButtonSetHint(btnViewPlayer_2,"Lance Swordsman")

labTitle2 = UI_CreateCompent( frmLanchInfo, LABELEX_TYPE, "labTitle2", 400, 150, 256, 151 )
UI_SetCaption( labTitle2, "Lance Swordsman")
UI_SetTextColor( labTitle2, COLOR_BLACK )
UI_SetLabelExFont( labTitle2, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_3 = UI_CreateCompent(frmLanchInfo, BUTTON_TYPE,"btnViewPlayer_3", 76, 96, 251, 174)
UI_ButtonSetHint(btnViewPlayer_3,"Lance Hunter")

labTitle3 = UI_CreateCompent( frmLanchInfo, LABELEX_TYPE, "labTitle3", 400, 150, 256, 274 )
UI_SetCaption( labTitle3, "Lance Hunter")
UI_SetTextColor( labTitle3, COLOR_BLACK )
UI_SetLabelExFont( labTitle3, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_4 = UI_CreateCompent(frmLanchInfo, BUTTON_TYPE,"btnViewPlayer_4", 76, 96, 251, 301)
UI_ButtonSetHint(btnViewPlayer_4,"Lance Explorer")

labTitle4 = UI_CreateCompent( frmLanchInfo, LABELEX_TYPE, "labTitle4", 400, 150, 256, 402 )
UI_SetCaption( labTitle4, "Lance Explorer")
UI_SetTextColor( labTitle4, COLOR_BLACK )
UI_SetLabelExFont( labTitle4, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_5 = UI_CreateCompent(frmLanchInfo, BUTTON_TYPE,"btnViewPlayer_5", 76, 96, 434, 41)
UI_ButtonSetHint(btnViewPlayer_5,"Lance Crusader")

labTitle5 = UI_CreateCompent( frmLanchInfo, LABELEX_TYPE, "labTitle5", 400, 150, 439, 151 )
UI_SetCaption( labTitle5, "Lance Crusader")
UI_SetTextColor( labTitle5, COLOR_BLACK )
UI_SetLabelExFont( labTitle5, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_6 = UI_CreateCompent(frmLanchInfo, BUTTON_TYPE,"btnViewPlayer_6", 76, 96, 434, 174)
UI_ButtonSetHint(btnViewPlayer_6,"Lance Sharpshooter")

labTitle6 = UI_CreateCompent( frmLanchInfo, LABELEX_TYPE, "labTitle6", 400, 150, 439, 274 )
UI_SetCaption( labTitle6, "Lance Sharpshooter")
UI_SetTextColor( labTitle6, COLOR_BLACK )
UI_SetLabelExFont( labTitle6, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_7 = UI_CreateCompent(frmLanchInfo, BUTTON_TYPE,"btnViewPlayer_7", 76, 96, 434, 301)
UI_ButtonSetHint(btnViewPlayer_7,"Lance Voyager")

labTitle7 = UI_CreateCompent( frmLanchInfo, LABELEX_TYPE, "labTitle7", 400, 150, 439, 402 )
UI_SetCaption( labTitle7, "Lance Voyager")
UI_SetTextColor( labTitle7, COLOR_BLACK )
UI_SetLabelExFont( labTitle7, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnNextStep = UI_CreateCompent(frmLanchInfo, BUTTON_TYPE,"btnNextStep", 100, 41, 252, 468)
UI_LoadButtonImage( btnNextStep, "texture/ui/newshelp/buton_next_prev.tga", 100, 41, 0, 0, FALSE )
UI_ButtonSetHint(btnNextStep,"Next")


---------------------------------
-- available class for carsise --
---------------------------------
frmCaxiusInfo = UI_CreateForm("frmCaxiusInfo", TRUE, 600, 500, 50 , 450, TRUE,TRUE)
UI_ShowForm(frmCaxiusInfo, FALSE )
UI_SetIsDrag(frmCaxiusInfo, FALSE)
UI_FormSetIsEscClose(frmCaxiusInfo, FALSE)
UI_SetFormStyle(frmCaxiusInfo,1)
UI_AddFormToTemplete( frmCaxiusInfo, FORM_CREATE_CHA )

--UI_FormSetHotKey( frmCaxiusInfo, ALT_KEY, HOTKEY_X ) 

imgLanchInfo1 = UI_CreateCompent(frmCaxiusInfo, IMAGE_TYPE,"imgLanchInfo1", 256, 256, 0, 0)
UI_LoadImage( imgLanchInfo1, "texture/ui/newshelp/frmCaxius01.tga", NORMAL, 256,256, 0, 0)

imgLanchInfo2 = UI_CreateCompent(frmCaxiusInfo, IMAGE_TYPE,"imgLanchInfo2", 256, 256, 256, 0)
UI_LoadImage( imgLanchInfo2, "texture/ui/newshelp/frmCaxius02.tga", NORMAL, 256, 256, 0, 0)

imgLanchInfo3 = UI_CreateCompent(frmCaxiusInfo, IMAGE_TYPE,"imgLanchInfo3", 256, 194, 0, 256)
UI_LoadImage( imgLanchInfo3, "texture/ui/newshelp/frmCaxius03.tga", NORMAL, 256, 194, 0, 0)

imgLanchInfo4 = UI_CreateCompent(frmCaxiusInfo, IMAGE_TYPE,"imgLanchInfo4", 256, 194, 256, 256)
UI_LoadImage( imgLanchInfo4, "texture/ui/newshelp/frmCaxius04.tga", NORMAL, 256, 194, 0, 0)

imgLanchInfo5 = UI_CreateCompent(frmCaxiusInfo, IMAGE_TYPE,"imgLanchInfo5", 88, 256, 512, 0)
UI_LoadImage( imgLanchInfo5, "texture/ui/newshelp/frmCaxius05.tga", NORMAL, 88,256, 0, 0)

imgLanchInfo6 = UI_CreateCompent(frmCaxiusInfo, IMAGE_TYPE,"imgLanchInfo6", 88, 194, 512, 256)
UI_LoadImage( imgLanchInfo6, "texture/ui/newshelp/frmCaxius05.tga", NORMAL, 88,194, 88, 0)

labTitle = UI_CreateCompent( frmCaxiusInfo, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Available class for Carsise")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_8 = UI_CreateCompent(frmCaxiusInfo, BUTTON_TYPE,"btnViewPlayer_8", 76, 96, 72, 174)
UI_ButtonSetHint(btnViewPlayer_8,"Carsise Newbie")

labTitle8 = UI_CreateCompent( frmCaxiusInfo, LABELEX_TYPE, "labTitle8", 400, 150, 77, 274 )
UI_SetCaption( labTitle8, "Carsise Newbie")
UI_SetTextColor( labTitle8, COLOR_BLACK )
UI_SetLabelExFont( labTitle8, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_9 = UI_CreateCompent(frmCaxiusInfo, BUTTON_TYPE,"btnViewPlayer_9", 76, 96, 251, 174)
UI_ButtonSetHint(btnViewPlayer_9,"Carsise Swordsman")

labTitle9 = UI_CreateCompent( frmCaxiusInfo, LABELEX_TYPE, "labTitle9", 400, 150, 256, 274 )
UI_SetCaption( labTitle9, "Carsise Swordsman")
UI_SetTextColor( labTitle9, COLOR_BLACK )
UI_SetLabelExFont( labTitle9, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_10 = UI_CreateCompent(frmCaxiusInfo, BUTTON_TYPE,"btnViewPlayer_10", 76, 96, 433, 174)
UI_ButtonSetHint(btnViewPlayer_10,"Carsise Champion")

labTitle10 = UI_CreateCompent( frmCaxiusInfo, LABELEX_TYPE, "labTitle10", 400, 150, 438, 274 )
UI_SetCaption( labTitle10, "Carsise Champion")
UI_SetTextColor( labTitle10, COLOR_BLACK )
UI_SetLabelExFont( labTitle10, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnNextStep = UI_CreateCompent(frmCaxiusInfo, BUTTON_TYPE,"btnNextStep", 100, 41, 252, 468)
UI_LoadButtonImage( btnNextStep, "texture/ui/newshelp/buton_next_prev.tga", 100, 41, 0, 0, FALSE )
UI_ButtonSetHint(btnNextStep,"Next")

-----------------------------
-- phyllis available class --
-----------------------------
frmFelierInfo = UI_CreateForm("frmFelierInfo", TRUE, 600, 500, 50 , 450, TRUE,TRUE)
UI_ShowForm(frmFelierInfo, FALSE )
UI_SetIsDrag(frmFelierInfo, FALSE)
UI_FormSetIsEscClose(frmFelierInfo, FALSE)
UI_SetFormStyle(frmFelierInfo,1)
UI_AddFormToTemplete( frmFelierInfo, FORM_CREATE_CHA )

--UI_FormSetHotKey( frmFelierInfo, ALT_KEY, HOTKEY_X ) 

imgFelierInfo1 = UI_CreateCompent(frmFelierInfo, IMAGE_TYPE,"imgFelierInfo1", 256, 256, 0, 0)
UI_LoadImage( imgFelierInfo1, "texture/ui/newshelp/frmFelier01.tga", NORMAL, 256,256, 0, 0)

imgFelierInfo2 = UI_CreateCompent(frmFelierInfo, IMAGE_TYPE,"imgFelierInfo2", 256, 256, 256, 0)
UI_LoadImage( imgFelierInfo2, "texture/ui/newshelp/frmFelier02.tga", NORMAL, 256, 256, 0, 0)

imgFelierInfo3 = UI_CreateCompent(frmFelierInfo, IMAGE_TYPE,"imgFelierInfo3", 256, 194, 0, 256)
UI_LoadImage( imgFelierInfo3, "texture/ui/newshelp/frmFelier03.tga", NORMAL, 256, 194, 0, 0)

imgFelierInfo4 = UI_CreateCompent(frmFelierInfo, IMAGE_TYPE,"imgFelierInfo4", 256, 194, 256, 256)
UI_LoadImage( imgFelierInfo4, "texture/ui/newshelp/frmFelier04.tga", NORMAL, 256, 194, 0, 0)

imgFelierInfo5 = UI_CreateCompent(frmFelierInfo, IMAGE_TYPE,"imgFelierInfo5", 88, 256, 512, 0)
UI_LoadImage( imgFelierInfo5, "texture/ui/newshelp/frmFelier05.tga", NORMAL, 88,256, 0, 0)

imgFelierInfo6 = UI_CreateCompent(frmFelierInfo, IMAGE_TYPE,"imgFelierInfo6", 88, 194, 512, 256)
UI_LoadImage( imgFelierInfo6, "texture/ui/newshelp/frmFelier05.tga", NORMAL, 88,194, 88, 0)

labTitle = UI_CreateCompent( frmFelierInfo, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Available class for Phyllis")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_11 = UI_CreateCompent(frmFelierInfo, BUTTON_TYPE,"btnViewPlayer_11", 76, 96, 72, 174)
UI_ButtonSetHint(btnViewPlayer_11,"Phyllis Newbie")

labTitle11 = UI_CreateCompent( frmFelierInfo, LABELEX_TYPE, "labTitle11", 400, 150, 77, 274 )
UI_SetCaption( labTitle11, "Phyllis Newbie")
UI_SetTextColor( labTitle11, COLOR_BLACK )
UI_SetLabelExFont( labTitle11, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_12 = UI_CreateCompent(frmFelierInfo, BUTTON_TYPE,"btnViewPlayer_12", 76, 96, 251, 51)
UI_ButtonSetHint(btnViewPlayer_12,"Phyllis Hunter")

labTitle12 = UI_CreateCompent( frmFelierInfo, LABELEX_TYPE, "labTitle12", 400, 150, 256, 151 )
UI_SetCaption( labTitle12, "Phyllis Hunter")
UI_SetTextColor( labTitle12, COLOR_BLACK )
UI_SetLabelExFont( labTitle12, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_13 = UI_CreateCompent(frmFelierInfo, BUTTON_TYPE,"btnViewPlayer_13", 76, 96, 251, 174)
UI_ButtonSetHint(btnViewPlayer_13,"Phyllis Herbalist")

labTitle13 = UI_CreateCompent( frmFelierInfo, LABELEX_TYPE, "labTitle13", 400, 150, 256, 274 )
UI_SetCaption( labTitle13, "Phyllis Herbalist")
UI_SetTextColor( labTitle13, COLOR_BLACK )
UI_SetLabelExFont( labTitle13, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_14 = UI_CreateCompent(frmFelierInfo, BUTTON_TYPE,"btnViewPlayer_14", 76, 96, 251, 301)
UI_ButtonSetHint(btnViewPlayer_14,"Phyllis Explorer")

labTitle14 = UI_CreateCompent( frmFelierInfo, LABELEX_TYPE, "labTitle14", 400, 150, 256, 401 )
UI_SetCaption( labTitle14, "Phyllis Explorer")
UI_SetTextColor( labTitle14, COLOR_BLACK )
UI_SetLabelExFont( labTitle14, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_15 = UI_CreateCompent(frmFelierInfo, BUTTON_TYPE,"btnViewPlayer_15", 76, 96, 434, 31)
UI_ButtonSetHint(btnViewPlayer_15,"Phyllis Sharpshooter")

labTitle15 = UI_CreateCompent( frmFelierInfo, LABELEX_TYPE, "labTitle15", 400, 150, 435, 111 )
UI_SetCaption( labTitle15, "Phyllis Sharpshooter")
UI_SetTextColor( labTitle15, COLOR_WHITE )
UI_SetLabelExFont( labTitle15, DEFAULT_FONT, TRUE, COLOR_BLACK )

btnViewPlayer_16 = UI_CreateCompent(frmFelierInfo, BUTTON_TYPE,"btnViewPlayer_16", 76, 96, 433, 135)
UI_ButtonSetHint(btnViewPlayer_16,"Phyllis Cleric")

labTitle16 = UI_CreateCompent( frmFelierInfo, LABELEX_TYPE, "labTitle16", 400, 150, 435, 215 )
UI_SetCaption( labTitle16, "Phyllis Cleric")
UI_SetTextColor( labTitle16, COLOR_WHITE )
UI_SetLabelExFont( labTitle16, DEFAULT_FONT, TRUE, COLOR_BLACK )

btnViewPlayer_17 = UI_CreateCompent(frmFelierInfo, BUTTON_TYPE,"btnViewPlayer_17", 76, 96, 434, 239)
UI_ButtonSetHint(btnViewPlayer_17,"Phyllis Seal Master")

labTitle17 = UI_CreateCompent( frmFelierInfo, LABELEX_TYPE, "labTitle17", 400, 150, 435, 319 )
UI_SetCaption( labTitle17, "Phyllis Seal Master")
UI_SetTextColor( labTitle17, COLOR_WHITE )
UI_SetLabelExFont( labTitle17, DEFAULT_FONT, TRUE, COLOR_BLACK )

btnViewPlayer_18 = UI_CreateCompent(frmFelierInfo, BUTTON_TYPE,"btnViewPlayer_18", 76, 96, 434, 345)
UI_ButtonSetHint(btnViewPlayer_18,"Phyllis Voyager")

labTitle18 = UI_CreateCompent( frmFelierInfo, LABELEX_TYPE, "labTitle18", 400, 150, 435, 425 )
UI_SetCaption( labTitle18, "Phyllis Voyager")
UI_SetTextColor( labTitle18, COLOR_WHITE)
UI_SetLabelExFont( labTitle18, DEFAULT_FONT, TRUE, COLOR_BLACK )

btnNextStep = UI_CreateCompent(frmFelierInfo, BUTTON_TYPE,"btnNextStep", 100, 41, 252, 468)
UI_LoadButtonImage( btnNextStep, "texture/ui/newshelp/buton_next_prev.tga", 100, 41, 0, 0, FALSE )
UI_ButtonSetHint(btnNextStep,"Next")


-------------------------
-- ami available class --
-------------------------
frmAimiInfo = UI_CreateForm("frmAimiInfo", TRUE, 600, 500, 50 , 450, TRUE,TRUE)
UI_ShowForm(frmAimiInfo, FALSE )
UI_SetIsDrag(frmAimiInfo, FALSE)
UI_FormSetIsEscClose(frmAimiInfo, FALSE)
UI_SetFormStyle(frmAimiInfo,1)
UI_AddFormToTemplete( frmAimiInfo, FORM_CREATE_CHA )

--UI_FormSetHotKey( frmAimiInfo, ALT_KEY, HOTKEY_X ) 

imgAimiInfo1 = UI_CreateCompent(frmAimiInfo, IMAGE_TYPE,"imgAimiInfo1", 256, 256, 0, 0)
UI_LoadImage( imgAimiInfo1, "texture/ui/newshelp/frmAimi01.tga", NORMAL, 256,256, 0, 0)

imgAimiInfo2 = UI_CreateCompent(frmAimiInfo, IMAGE_TYPE,"imgAimiInfo2", 256, 256, 256, 0)
UI_LoadImage( imgAimiInfo2, "texture/ui/newshelp/frmAimi02.tga", NORMAL, 256, 256, 0, 0)

imgAimiInfo3 = UI_CreateCompent(frmAimiInfo, IMAGE_TYPE,"imgAimiInfo3", 256, 194, 0, 256)
UI_LoadImage( imgAimiInfo3, "texture/ui/newshelp/frmAimi03.tga", NORMAL, 256, 194, 0, 0)

imgAimiInfo4 = UI_CreateCompent(frmAimiInfo, IMAGE_TYPE,"imgAimiInfo4", 256, 194, 256, 256)
UI_LoadImage( imgAimiInfo4, "texture/ui/newshelp/frmAimi04.tga", NORMAL, 256, 194, 0, 0)

imgAimiInfo5 = UI_CreateCompent(frmAimiInfo, IMAGE_TYPE,"imgAimiInfo5", 88, 256, 512, 0)
UI_LoadImage( imgAimiInfo5, "texture/ui/newshelp/frmAimi05.tga", NORMAL, 88,256, 0, 0)

imgAimiInfo6 = UI_CreateCompent(frmAimiInfo, IMAGE_TYPE,"imgAimiInfo6", 88, 194, 512, 256)
UI_LoadImage( imgAimiInfo6, "texture/ui/newshelp/frmAimi05.tga", NORMAL, 88,194, 88, 0)

labTitle = UI_CreateCompent( frmAimiInfo, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Available class for Ami")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_19 = UI_CreateCompent(frmAimiInfo, BUTTON_TYPE,"btnViewPlayer_19", 76, 96, 72, 174)
UI_ButtonSetHint(btnViewPlayer_19,"Ami Newbie")

labTitle19 = UI_CreateCompent( frmAimiInfo, LABELEX_TYPE, "labTitle19", 400, 150, 77, 274 )
UI_SetCaption( labTitle19, "Ami Newbie")
UI_SetTextColor( labTitle19, COLOR_BLACK )
UI_SetLabelExFont( labTitle19, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_20 = UI_CreateCompent(frmAimiInfo, BUTTON_TYPE,"btnViewPlayer_20", 76, 96, 251, 96)
UI_ButtonSetHint(btnViewPlayer_20,"Ami Herbalist")

labTitle20 = UI_CreateCompent( frmAimiInfo, LABELEX_TYPE, "labTitle20", 400, 150, 255, 196 )
UI_SetCaption( labTitle20, "Ami Herbalist")
UI_SetTextColor( labTitle20, COLOR_BLACK )
UI_SetLabelExFont( labTitle20, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_21 = UI_CreateCompent(frmAimiInfo, BUTTON_TYPE,"btnViewPlayer_21", 76, 96, 251, 252)
UI_ButtonSetHint(btnViewPlayer_21,"Ami Explorer")

labTitle21 = UI_CreateCompent( frmAimiInfo, LABELEX_TYPE, "labTitle21", 400, 150, 255, 352 )
UI_SetCaption( labTitle21, "Ami Explorer")
UI_SetTextColor( labTitle21, COLOR_BLACK )
UI_SetLabelExFont( labTitle21, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_22 = UI_CreateCompent(frmAimiInfo, BUTTON_TYPE,"btnViewPlayer_22", 76, 96, 434, 51)
UI_ButtonSetHint(btnViewPlayer_22,"Ami Cleric")

labTitle22 = UI_CreateCompent( frmAimiInfo, LABELEX_TYPE, "labTitle22", 400, 150, 439, 151 )
UI_SetCaption( labTitle22, "Ami Cleric")
UI_SetTextColor( labTitle22, COLOR_BLACK )
UI_SetLabelExFont( labTitle22, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_23 = UI_CreateCompent(frmAimiInfo, BUTTON_TYPE,"btnViewPlayer_23", 76, 96, 433, 174)
UI_ButtonSetHint(btnViewPlayer_23,"Ami Seal Master")

labTitle23 = UI_CreateCompent( frmAimiInfo, LABELEX_TYPE, "labTitle23", 400, 150, 439, 274 )
UI_SetCaption( labTitle23, "Ami Seal Master")
UI_SetTextColor( labTitle23, COLOR_BLACK )
UI_SetLabelExFont( labTitle23, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnViewPlayer_24 = UI_CreateCompent(frmAimiInfo, BUTTON_TYPE,"btnViewPlayer_24", 76, 96, 434, 301)
UI_ButtonSetHint(btnViewPlayer_24,"Ami Voyager")

labTitle24 = UI_CreateCompent( frmAimiInfo, LABELEX_TYPE, "labTitle24", 400, 150, 439, 401 )
UI_SetCaption( labTitle24, "Ami Voyager")
UI_SetTextColor( labTitle24, COLOR_BLACK )
UI_SetLabelExFont( labTitle24, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnNextStep = UI_CreateCompent(frmAimiInfo, BUTTON_TYPE,"btnNextStep", 100, 41, 252, 468)
UI_LoadButtonImage( btnNextStep, "texture/ui/newshelp/buton_next_prev.tga", 100, 41, 0, 0, FALSE )
UI_ButtonSetHint(btnNextStep,"Next")


-------------------------------------------------------------------------
--     
-------------------------------------------------------------------------
frmRoleAllInfo = UI_CreateForm("frmRoleAllInfo", TRUE, 447, 373, 50 , 450, TRUE,TRUE)
UI_ShowForm(frmRoleAllInfo, FALSE )
UI_SetIsDrag(frmRoleAllInfo, FALSE)
UI_FormSetIsEscClose(frmRoleAllInfo, FALSE)
UI_SetFormStyle(frmRoleAllInfo,1)
UI_AddFormToTemplete( frmRoleAllInfo, FORM_CREATE_CHA )

UI_FormSetHotKey( frmRoleAllInfo, ALT_KEY, HOTKEY_X ) 

--

-- 
imgFoundTop = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgFoundTop", 256, 256, 0, 0 )
UI_LoadImage( imgFoundTop, "texture/ui/newshelp/frmRoleInfo01.tga", NORMAL, 256, 256, 0, 0 )

imgFoundDown = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgFoundDown", 191, 256, 256, 0 )
UI_LoadImage( imgFoundDown, "texture/ui/newshelp/frmRoleInfo02.tga", NORMAL, 191, 256, 0, 0 )

imgFoundTopR = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgFoundTopR", 256, 117, 0, 256 )
UI_LoadImage( imgFoundTopR, "texture/ui/newshelp/frmRoleInfo03.tga", NORMAL, 256, 117, 0, 0 )

imgFoundDownR = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgFoundDownR", 191, 117, 256, 256 )
UI_LoadImage( imgFoundDownR, "texture/ui/newshelp/frmRoleInfo03.tga", NORMAL, 191, 117, 0, 117 )


--
memChaDescribeUp  = UI_CreateCompent( frmRoleAllInfo, MEMO_TYPE, "memChaDescribeUp", 162, 154, 287, 26 )
UI_SetTextColor( memChaDescribeUp, COLOR_BLACK )
UI_SetLabelExFont( memChaDescribeUp, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetMemoMaxNumPerRow( memChaDescribeUp, 24 )
UI_SetMemoPageShowNum( memChaDescribeUp, 7 )

memChaDescribeDown  = UI_CreateCompent( frmRoleAllInfo, MEMO_TYPE, "memChaDescribeDown", 162, 154, 287, 198 )
UI_SetTextColor( memChaDescribeDown, COLOR_BLACK )
UI_SetLabelExFont( memChaDescribeDown, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetMemoMaxNumPerRow( memChaDescribeDown, 24 )
UI_SetMemoPageShowNum( memChaDescribeDown, 7 )

--
local tabImage = {}
--
tabImage[1]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView1", 220, 300, 30, 30 )
UI_LoadImage( tabImage[1], "texture/ui/newshelp/char03.tga", NORMAL, 82,102, 0, 0 )

tabImage[2]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView2", 220, 300, 30, 30 )
UI_LoadImage( tabImage[2], "texture/ui/newshelp/char03.tga", NORMAL, 82,102, 82, 0 )

tabImage[3]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView3", 220, 300, 30, 30 )
UI_LoadImage( tabImage[3], "texture/ui/newshelp/char03.tga", NORMAL, 82,102, 164, 0 )

tabImage[4]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView4", 220, 300, 30, 30 )
UI_LoadImage( tabImage[4], "texture/ui/newshelp/char03.tga", NORMAL, 82,102, 0, 102 )

tabImage[5]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView5", 220, 300, 30, 30 )
UI_LoadImage( tabImage[5], "texture/ui/newshelp/char03.tga", NORMAL, 82,102, 82, 102 )

tabImage[6]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView6", 220, 300, 30, 30 )
UI_LoadImage( tabImage[6], "texture/ui/newshelp/char03.tga", NORMAL, 82,102, 164, 102 )

tabImage[7]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView7", 220, 300, 30, 30 )
UI_LoadImage( tabImage[7], "texture/ui/newshelp/char02.tga", NORMAL, 82,102, 0, 0 )

--
tabImage[8]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView8", 220, 300, 30, 30 )
UI_LoadImage( tabImage[8], "texture/ui/newshelp/char02.tga", NORMAL, 82,102, 0, 102 )

tabImage[9]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView9", 220, 300, 30, 30 )
UI_LoadImage( tabImage[9], "texture/ui/newshelp/char02.tga", NORMAL, 82,102, 82, 102 )

tabImage[10]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView10", 220, 300, 30, 30 )
UI_LoadImage( tabImage[10], "texture/ui/newshelp/char02.tga", NORMAL, 82,102, 164, 102 )

--
tabImage[11]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView11", 220, 300, 30, 30 )
UI_LoadImage( tabImage[11], "texture/ui/newshelp/char02.tga", NORMAL, 82,102, 82, 0 )

tabImage[12]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView12", 220, 300, 30, 30 )
UI_LoadImage( tabImage[12], "texture/ui/newshelp/char02.tga", NORMAL, 82,102, 164, 0 )

tabImage[13]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView13", 220, 300, 30, 30 )
UI_LoadImage( tabImage[13], "texture/ui/newshelp/char04.tga", NORMAL, 82,102, 0, 0 )

tabImage[14]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView14", 220, 300, 30, 30 )
UI_LoadImage( tabImage[14], "texture/ui/newshelp/char04.tga", NORMAL, 82,102, 82, 0 )

tabImage[15]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView15", 220, 300, 30, 30 )
UI_LoadImage( tabImage[15], "texture/ui/newshelp/char04.tga", NORMAL, 82,102, 164, 0 )

tabImage[16]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView16", 220, 300, 30, 30 )
UI_LoadImage( tabImage[16], "texture/ui/newshelp/char04.tga", NORMAL, 82,102, 0, 102 )

tabImage[17]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView17", 220, 300, 30, 30 )
UI_LoadImage( tabImage[17], "texture/ui/newshelp/char04.tga", NORMAL, 82,102, 82, 102 )

tabImage[18]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView18", 220, 300, 30, 30 )
UI_LoadImage( tabImage[18], "texture/ui/newshelp/char04.tga", NORMAL, 82,102, 164, 102 )

--
tabImage[19]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView19", 220, 300, 30, 30 )
UI_LoadImage( tabImage[19], "texture/ui/newshelp/char01.tga", NORMAL, 82,102, 0, 0 )

tabImage[20]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView20", 220, 300, 30, 30 )
UI_LoadImage( tabImage[20], "texture/ui/newshelp/char01.tga", NORMAL, 82,102, 82, 0 )

tabImage[21]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView21", 220, 300, 30, 30 )
UI_LoadImage( tabImage[21], "texture/ui/newshelp/char01.tga", NORMAL, 82,102, 164, 0 )

tabImage[22]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView22", 220, 300, 30, 30 )
UI_LoadImage( tabImage[22], "texture/ui/newshelp/char01.tga", NORMAL, 82,102, 0, 102 )

tabImage[23]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView23", 220, 300, 30, 30 )
UI_LoadImage( tabImage[23], "texture/ui/newshelp/char01.tga", NORMAL, 82,102, 82, 102 )

tabImage[24]  = UI_CreateCompent( frmRoleAllInfo, IMAGE_TYPE, "imgChaView24", 220, 300, 30, 30 )
UI_LoadImage( tabImage[24], "texture/ui/newshelp/char01.tga", NORMAL, 82,102, 164, 102 )

for i=1,24,1 do
	UI_SetIsShow(tabImage[i],FALSE)
end

--
btnPrevStep = UI_CreateCompent(frmRoleAllInfo, BUTTON_TYPE,"btnPrevStep", 100,41, 173, 408)
UI_LoadButtonImage( btnPrevStep, "texture/ui/newshelp/buton_next_prev.tga", 100, 41, 100, 0, FALSE )
UI_ButtonSetHint(btnPrevStep,"Previous")




----------------------------------------------------------------
-- Change Password | Coded by Billy, GUI by Foxseiz
----------------------------------------------------------------

frmUpdPas = UI_CreateForm( "frmUpdPas", TRUE, 250, 170, 0 , 0, TRUE, TRUE )
UI_ShowForm( frmUpdPas, FALSE )
UI_AddFormToTemplete( frmUpdPas, FORM_SELECT )
UI_SetIsDrag( frmUpdPas, TRUE )
UI_FormSetIsEscClose( frmUpdPas, TRUE)
UI_SetFormStyle(frmUpdPas,1)

imgBackground = UI_CreateCompent( frmUpdPas, IMAGE_TYPE, "imgBackground", 230, 148, 0, 0 )
UI_LoadImage( imgBackground, "texture/ui/corsairs/changepw.png", NORMAL, 230, 148, 0, 0 )

-- Title Bar Password Label
labTiPass = UI_CreateCompent( frmUpdPas, LABELEX_TYPE, "labTiPass", 95, 12, 10, 6 )
UI_SetCaption( labTiPass, "Change Password" )
UI_SetTextColor( labTiPass, COLOR_BLACK )
UI_SetLabelExFont( labTiPass, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Password Label
labPass = UI_CreateCompent( frmUpdPas, LABELEX_TYPE, "labPass", 95, 13, 10, 50 )
UI_SetCaption( labPass, "New Password" )
UI_SetTextColor( labPass, COLOR_BLACK )
UI_SetLabelExFont( labPass, DEFAULT_FONT, TRUE, COLOR_WHITE )

edtPassword = UI_CreateCompent( frmUpdPas, EDIT_TYPE, "edtPassword", 80, 12, 116, 50 )
UI_SetTextColor( edtPassword, COLOR_BLACK )
UI_SetEditMaxNum( edtPassword, 20 )
UI_SetEditMaxNumVisible( edtPassword, 12 )
UI_SetCaption(edtPassword,"")

-- Password 2 Label
labPass2 = UI_CreateCompent( frmUpdPas, LABELEX_TYPE, "labPass2", 95, 13, 10, 75 )
UI_SetCaption( labPass2, "Confirm Password" )
UI_SetTextColor( labPass2, COLOR_BLACK )
UI_SetLabelExFont( labPass2, DEFAULT_FONT, TRUE, COLOR_WHITE )

edtPassword2 = UI_CreateCompent( frmUpdPas, EDIT_TYPE, "edtPassword2", 80, 12, 116, 75 )
UI_SetTextColor( edtPassword2, COLOR_BLACK )
UI_SetEditMaxNum( edtPassword2, 20 )
UI_SetEditMaxNumVisible( edtPassword2, 12 )
UI_SetCaption(edtPassword2,"")

-- PIN Label
labPIN = UI_CreateCompent( frmUpdPas, LABELEX_TYPE, "labPIN", 95, 13, 10, 100 )
UI_SetCaption( labPIN, "Confirm PIN" )
UI_SetTextColor( labPIN, COLOR_BLACK )
UI_SetLabelExFont( labPIN, DEFAULT_FONT, TRUE, COLOR_WHITE )

confirmPIN = UI_CreateCompent( frmUpdPas, EDIT_TYPE, "confirmPIN", 80, 12, 116, 100 )
UI_SetTextColor( confirmPIN, COLOR_BLACK )
UI_SetEditMaxNum( confirmPIN, 20 )
UI_SetEditMaxNumVisible( confirmPIN, 12 )
UI_SetCaption(confirmPIN,"")

-- Confirm Button
btnChangePassConf = UI_CreateCompent( frmUpdPas, BUTTON_TYPE, "btnChangePassConf", 41, 19, 60, 126 )
UI_LoadButtonImage( btnChangePassConf, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetButtonModalResult( btnChangePassConf, BUTTON_CLOSE )

-- Cancel Button
btnCancel = UI_CreateCompent( frmUpdPas, BUTTON_TYPE, "btnCancel", 41, 19, 130, 126 )
UI_LoadButtonImage( btnCancel, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnCancel, BUTTON_CLOSE )
