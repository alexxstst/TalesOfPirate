-----------------
-- region list --
-----------------
frmArea = UI_CreateForm( "frmArea", FALSE, 403, 290, 396, 225, FALSE, FALSE )
UI_SetFormStyle( frmArea , 2 )
UI_AddFormToTemplete( frmArea, FORM_LOGIN )
--UI_SetAlpha( frmArea, 1 )
UI_FormSetIsEscClose( frmArea, FALSE )

imgBakTop = UI_CreateCompent( frmArea, IMAGE_TYPE, "imgBakTop", 256, 256, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/list1.tga", NORMAL, 256, 256, 0, 0 )
UI_SetImageAlpha(imgBakTop,180)

imgBakDown = UI_CreateCompent( frmArea, IMAGE_TYPE, "imgBakDown", 147, 256, 256, 0 )
UI_LoadImage( imgBakDown, "texture/ui/list2.tga", NORMAL, 147, 256, 0, 0 )
UI_SetImageAlpha(imgBakDown,180)

imgBakDown1 = UI_CreateCompent( frmArea, IMAGE_TYPE, "imgBakDown1", 256, 34, 0, 256 )
UI_LoadImage( imgBakDown1, "texture/ui/list3.tga", NORMAL, 256, 34, 0, 0 )
UI_SetImageAlpha(imgBakDown1,180)

imgBakDown1 = UI_CreateCompent( frmArea, IMAGE_TYPE, "imgBakDown1", 147, 34, 256, 256 )
UI_LoadImage( imgBakDown1, "texture/ui/list3.tga", NORMAL, 147, 34, 0, 44 )
UI_SetImageAlpha(imgBakDown1,180)

imgPapa = UI_CreateCompent( frmArea, IMAGE_TYPE, "imgPapa", 43, 32, 7, 22 )
UI_LoadImage( imgPapa, "texture/ui/PublicC.tga", NORMAL, 43, 32, 213, 15 )
UI_SetImageAlpha(imgPapa,180)

--imgServerTitle1 = UI_CreateCompent( frmArea, IMAGE_TYPE, "imgServerTitle1", 61, 13, 175, 27 )
--UI_LoadImage( imgServerTitle1, "texture/ui/Word.dds", NORMAL, 61, 13, 158, 183 )
labTitle = UI_CreateCompent( frmArea, LABELEX_TYPE, "labTitle", 400, 150, 175, 27 )
UI_SetCaption( labTitle, "Region")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

lstRegion0 = UI_CreateCompent( frmArea, LIST_TYPE, "lstRegion0", 184, 150, 16, 35 )
UI_SetMargin( lstRegion0, 19, 20, 15, 0 )
UI_ListSetItemMargin( lstRegion0, 10, 9 )
UI_SetListRowHeight( lstRegion0, 23 )
UI_SetListIsMouseFollow( lstRegion0, TRUE)
UI_ListLoadSelectImage( lstRegion0, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227 )
UI_SetImageAlpha( lstRegion0, 134 )
UI_SetListFontColor( lstRegion0, COLOR_BLACK, COLOR_RED )

lstRegion1 = UI_CreateCompent( frmArea, LIST_TYPE, "lstRegion1", 184, 150, 202, 35 )
UI_SetMargin( lstRegion1, 19, 20, 15, 0 )
UI_ListSetItemMargin( lstRegion1, 10, 9 )
UI_SetListRowHeight( lstRegion1, 23 )
UI_SetListIsMouseFollow( lstRegion1, TRUE)
UI_ListLoadSelectImage( lstRegion1, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227 )
UI_SetImageAlpha( lstRegion1, 134 )
UI_SetListFontColor( lstRegion1, COLOR_BLACK, COLOR_RED )


scrollid = UI_GetScroll( lstRegion1 )
--UI_SetIsEnabled( scrollid, FALSE )
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

btnNo = UI_CreateCompent( frmArea, BUTTON_TYPE, "btnNo", 41, 19, 185, 263 )
UI_LoadButtonImage( btnNo, "texture/ui/SystemBotton3.tga", 41, 19, 0, 152, TRUE )
UI_SetAlpha(btnNo,210)

-----------------
-- server list --
-----------------
frmServer = UI_CreateForm( "frmServer", FALSE, 403, 290, 396, 225, FALSE, FALSE )
UI_SetFormStyle( frmServer , 2 )
UI_AddFormToTemplete( frmServer, FORM_LOGIN )
UI_FormSetIsEscClose( frmServer, FALSE )

imgBakTop = UI_CreateCompent( frmServer, IMAGE_TYPE, "imgBakTop", 256, 256, 0, 0 )
UI_LoadImage( imgBakTop, "texture/ui/list1.tga", NORMAL, 256, 256, 0, 0 )
UI_SetImageAlpha(imgBakTop,220)

imgBakDown = UI_CreateCompent( frmServer, IMAGE_TYPE, "imgBakDown", 147, 256, 256, 0 )
UI_LoadImage( imgBakDown, "texture/ui/list2.tga", NORMAL, 147, 256, 0, 0 )
UI_SetImageAlpha(imgBakDown,220)

imgBakDown1 = UI_CreateCompent( frmServer, IMAGE_TYPE, "imgBakDown1", 256, 34, 0, 256 )
UI_LoadImage( imgBakDown1, "texture/ui/list3.tga", NORMAL, 256, 34, 0, 0 )
UI_SetImageAlpha(imgBakDown1,220)

imgBakDown1 = UI_CreateCompent( frmServer, IMAGE_TYPE, "imgBakDown1", 147, 34, 256, 256 )
UI_LoadImage( imgBakDown1, "texture/ui/list3.tga", NORMAL, 147, 34, 0, 44 )
UI_SetImageAlpha(imgBakDown1,220)

imgPapa = UI_CreateCompent( frmServer, IMAGE_TYPE, "imgPapa", 43, 32, 7, 22 )
UI_LoadImage( imgPapa, "texture/ui/PublicC.tga", NORMAL, 43, 32, 213, 15 )
UI_SetImageAlpha(imgPapa,220)

--imgServerTitle = UI_CreateCompent( frmServer, IMAGE_TYPE, "imgServerTitle", 61, 13, 175, 27 )
--UI_LoadImage( imgServerTitle, "texture/ui/Word.dds", NORMAL, 61, 13, 0, 86 )

labTitle = UI_CreateCompent( frmServer, LABELEX_TYPE, "labTitle", 400, 150, 175, 27 )
UI_SetCaption( labTitle, "Server List")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgServerIcon0 = UI_CreateCompent( frmServer, IMAGE_TYPE, "imgServerIcon0", 12, 12, 175, 27 )
--UI_LoadImage( imgServerIcon0, "texture/ui/list1.tga", NORMAL, 12, 12, 0, 0 )
UI_SetMaxImage( imgServerIcon0, 4 )
UI_LoadImage( imgServerIcon0, "texture/ui/forge3.tga", 0, 11, 11, 164, 76 )
UI_LoadImage( imgServerIcon0, "texture/ui/forge3.tga", 1, 11, 11, 175, 76 )
UI_LoadImage( imgServerIcon0, "texture/ui/forge3.tga", 2, 11, 11, 197, 76 )
UI_LoadImage( imgServerIcon0, "texture/ui/forge3.tga", 3, 11, 11, 186, 76 )


--imgServerIcon1 = UI_CreateCompent( frmServer, IMAGE_TYPE, "imgServerIcon1", 12, 12, 175, 27 )
--UI_LoadImage( imgServerIcon1, "texture/ui/Word.dds", NORMAL, 12, 12, 12, 86 )
--UI_SetMaxImage( imgServerIcon1, 4 )

--imgServerIcon2 = UI_CreateCompent( frmServer, IMAGE_TYPE, "imgServerIcon2", 12, 12, 175, 27 )
--UI_LoadImage( imgServerIcon2, "texture/ui/Word.dds", NORMAL, 12, 12, 24, 86 )
--UI_SetMaxImage( imgServerIcon2, 4 )

--imgServerIcon3 = UI_CreateCompent( frmServer, IMAGE_TYPE, "imgServerIcon3", 12, 12, 175, 27 )
--UI_LoadImage( imgServerIcon3, "texture/ui/Word.dds", NORMAL, 12, 12, 36, 86 )
--UI_SetMaxImage( imgServerIcon3, 4 )

lvServer0 = UI_CreateListView( frmServer, "lvServer0", 184, 150, 16, 35, 2, 2 )
UI_SetMargin( lvServer0, 19, 20, 15, 0 )
UI_ListViewSetTitle( lvServer0, 0, 15, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lvServer0, 1, 132, "", 0, 0, 0, 0 )
UI_ListSetItemMargin( lvServer0, 10, 9 )
UI_SetListRowHeight( lvServer0, 23 )
UI_SetListIsMouseFollow( lvServer0, TRUE)
UI_ListLoadSelectImage( lvServer0, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227 )
UI_SetImageAlpha( lvServer0, 134 )
UI_SetListFontColor( lvServer0, COLOR_BLACK, COLOR_RED )

lvServer1 = UI_CreateListView( frmServer, "lvServer1", 184, 150, 202, 35, 2, 2 )
UI_SetMargin( lvServer1, 19, 20, 15, 0 )
UI_ListViewSetTitle( lvServer1, 0, 15, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lvServer1, 1, 132, "", 0, 0, 0, 0 )
UI_ListSetItemMargin( lvServer1, 10, 9 )
UI_SetListRowHeight( lvServer1, 23 )
UI_SetListIsMouseFollow( lvServer1, TRUE)
UI_ListLoadSelectImage( lvServer1, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227, 2, 2 )
UI_SetImageAlpha( lvServer1, 134 )
UI_SetListFontColor( lvServer1, COLOR_BLACK, COLOR_RED )

scrollid = UI_GetScroll( lvServer1 )
--UI_SetIsEnabled( scrollid, FALSE )
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

btnNo = UI_CreateCompent( frmServer, BUTTON_TYPE, "btnNo", 41, 19, 185, 263 )
UI_LoadButtonImage( btnNo, "texture/ui/SystemBotton3.tga", 41, 19, 0, 152, TRUE )
UI_SetAlpha(btnNo,210)

----------------
-- Login Form --
----------------
frmAccount = UI_CreateForm( "frmAccount", FALSE, 185, 101, 310, 340, FALSE, FALSE )
UI_AddFormToTemplete( frmAccount, FORM_LOGIN )
UI_SetFormStyle( frmAccount, 2 )
UI_FormSetIsEscClose( frmAccount, FALSE )

imgLogo1 = UI_CreateCompent( frmAccount, IMAGE_TYPE, "imgLogo1", 256, 256, -117, -197 )

imgLogo2 = UI_CreateCompent( frmAccount, IMAGE_TYPE, "imgLogo2", 256, 256, 139, -173 )

imgAccout = UI_CreateCompent( frmAccount, IMAGE_TYPE, "imgAccout", 185, 101, 0, 0 )
UI_LoadImage( imgAccout, "texture/ui/sure.dds", NORMAL, 185, 101, 0, 0 )
UI_SetImageAlpha(imgAccout,235)

labTitle = UI_CreateCompent( frmAccount, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Account Login")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labID = UI_CreateCompent( frmAccount, LABELEX_TYPE, "labID", 23, 11, 28, 32 )
UI_SetCaption( labID, "Account" )
UI_SetTextColor( labID, COLOR_BLACK )
UI_SetLabelExFont( labID, DEFAULT_FONT, TRUE, COLOR_WHITE )

labPass = UI_CreateCompent( frmAccount, LABELEX_TYPE, "labPass", 23, 11, 22, 54 )
UI_SetCaption( labPass, "Password" )
UI_SetTextColor( labPass, COLOR_BLACK )
UI_SetLabelExFont( labPass, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgID = UI_CreateCompent( frmAccount, IMAGE_TYPE, "imgID", 87, 18, 70, 28 )
UI_LoadImage( imgID, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )
imgPass = UI_CreateCompent( frmAccount, IMAGE_TYPE, "imgPass", 87, 18, 70, 50 )
UI_LoadImage( imgPass, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

chkID = UI_CreateCompent( frmAccount, CHECK_TYPE, "chkID", 10, 10, 161, 32 )
UI_LoadImage( chkID, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkID, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )
UI_SetHint( chkID, "Remember ID" )

-- Login Button
btnYes = UI_CreateCompent( frmAccount, BUTTON_TYPE, "btnYes", 41, 19, 84, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/corsairs/coButtons.png", 41, 19, 0, 57, TRUE )

-- Exit Button
btnNo = UI_CreateCompent( frmAccount, BUTTON_TYPE, "btnNo", 41, 19, 129, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/corsairs/coButtons.png", 41, 19, 0, 76, TRUE )

labYes = UI_CreateCompent( frmAccount, LABELEX_TYPE, "labYes", 42, 10, 107, 155 )
UI_SetCaption( labYes, "" )
UI_SetTextColor( labYes, COLOR_WHITE )
UI_SetLabelExFont( labYes, DEFAULT_FONT, TRUE, COLOR_BLACK )

labNo = UI_CreateCompent( frmAccount, LABELEX_TYPE, "labNo", 42, 10, 152, 155 )
UI_SetCaption( labNo, "" )
UI_SetTextColor( labNo, COLOR_WHITE )
UI_SetLabelExFont( labNo, DEFAULT_FONT, TRUE, COLOR_BLACK )

edtID = UI_CreateCompent( frmAccount, EDIT_TYPE, "edtID", 80, 12, 76, 32 )
UI_SetTextColor( edtID, COLOR_BLACK )
UI_SetEditMaxNum( edtID, 20 )
UI_SetEditMaxNumVisible( edtID, 12 )

edtPassword = UI_CreateCompent( frmAccount, EDIT_TYPE, "edtPassword", 80, 12, 76, 54 )
UI_SetTextColor( edtPassword, COLOR_BLACK )
UI_SetEditMaxNum( edtPassword, 20 )
UI_SetEditMaxNumVisible( edtPassword, 12 )

-- Register Button
btnKeyboard = UI_CreateCompent( frmAccount, BUTTON_TYPE, "btnKeyboard", 41, 19, 39, 78 )
UI_LoadButtonImage( btnKeyboard, "texture/ui/corsairs/coButtons.png", 41, 19, 0, 38, TRUE )
--UI_LoadImage( btnKeyboard, "texture/ui/corsairs/coButtons.png", 0, 41, 19, 0, 38, )
--UI_LoadImage( btnKeyboard, "texture/ui/corsairs/coButtons.png", 2, 41, 19, 41, 38, )
--UI_LoadImage( btnKeyboard, "texture/ui/corsairs/coButtons.png", 1, 41, 19, 82, 38, )
	
--btnRegister = UI_CreateCompent( frmAccount, BUTTON_TYPE, "btnRegister", 57, 17, 21, 79 )
--UI_LoadButtonImage( btnRegister, "texture/ui/Keyboard4.tga", 57, 17, 197, 85, FALSE )

-----------------------------------------------------------------------
-- Announcement form
-----------------------------------------------------------------------	
-- Create announcement form
frmAnnounce = UI_CreateForm( "frmAnnounce", FALSE, 198, 207, 396, 190, FALSE, FALSE )
UI_LoadFormImage( frmAnnounce, "texture/ui/Game.bmp", 198, 207, 0, 0, "", 0, 0 )
UI_AddFormToTemplete( frmAnnounce, FORM_LOGIN )
UI_SetFormStyle( frmAnnounce, 2 )
--UI_SetAlpha( frmAnnounce, 200 )
UI_FormSetIsEscClose( frmAnnounce, FALSE )


-- Title label
imgAnnTitle = UI_CreateCompent( frmAnnounce, IMAGE_TYPE, "imgAnnTitle", 73, 13, 10, 4 )
UI_LoadImage( imgAnnTitle, "texture/ui/Word.dds", NORMAL, 73, 13, 0, 58 )


-- Confirm button
btnYes = UI_CreateCompent( frmAnnounce, BUTTON_TYPE, "btnYes", 41, 19, 100, 180 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetAlign( btnYes, caWidthCenter )

labYes = UI_CreateCompent( frmAnnounce, LABELEX_TYPE, "labYes", 24, 10, 109, 324 )
UI_SetCaption( labYes, "" )
UI_SetTextColor( labYes, COLOR_WHITE )
UI_SetLabelExFont( labYes, DEFAULT_FONT, TRUE, COLOR_BLACK )

-----------------------------------------------------------------------
-- Coded by Billy | GUI by Foxseiz edited by aLex
-----------------------------------------------------------------------
-----------------------------------------------------------------------
-- Coded by Billy | GUI by Foxseiz
-----------------------------------------------------------------------

frmRegister = UI_CreateForm( "frmRegister", FALSE, 268, 170, 310, 160, FALSE, FALSE )
UI_AddFormToTemplete( frmRegister, FORM_LOGIN )
UI_SetFormStyle( frmRegister, 2 )
UI_FormSetIsEscClose( frmRegister, FALSE )

imgAccout = UI_CreateCompent( frmRegister, IMAGE_TYPE, "imgAccout", 268, 170, 0, 0 )
UI_LoadImage( imgAccout, "texture/ui/corsairs/register.png", NORMAL, 268, 170, 0, 0 )
UI_SetImageAlpha(imgAccout,235)

labTitle = UI_CreateCompent( frmRegister, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Account Registration")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labID = UI_CreateCompent( frmRegister, LABELEX_TYPE, "labID", 23, 11, 18, 50 )
UI_SetCaption( labID, "Account" )
UI_SetTextColor( labID, COLOR_BLACK )
UI_SetLabelExFont( labID, DEFAULT_FONT, TRUE, COLOR_WHITE )

labPass = UI_CreateCompent( frmRegister, LABELEX_TYPE, "labPass", 23, 11, 18, 74 )
UI_SetCaption( labPass, "Password" )
UI_SetTextColor( labPass, COLOR_BLACK )
UI_SetLabelExFont( labPass, DEFAULT_FONT, TRUE, COLOR_WHITE )

labPass2 = UI_CreateCompent( frmRegister, LABELEX_TYPE, "labPass2", 23, 11, 18, 98 )
UI_SetCaption( labPass2, "Re-Password" )
UI_SetTextColor( labPass2, COLOR_BLACK )
UI_SetLabelExFont( labPass2, DEFAULT_FONT, TRUE, COLOR_WHITE )

labEmail = UI_CreateCompent( frmRegister, LABELEX_TYPE, "labEmail", 23, 11, 18, 122 )
UI_SetCaption( labEmail, "Email" )
UI_SetTextColor( labEmail, COLOR_BLACK )
UI_SetLabelExFont( labEmail, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- IMAGES FOR FIELD
-- imgID = UI_CreateCompent( frmRegister, IMAGE_TYPE, "imgID", 87, 18, 70, 28 )
-- UI_LoadImage( imgID, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

-- imgPass = UI_CreateCompent( frmRegister, IMAGE_TYPE, "imgPass", 87, 18, 70, 50 )
-- UI_LoadImage( imgPass, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

-- imgPass2 = UI_CreateCompent( frmRegister, IMAGE_TYPE, "imgPass2", 87, 18, 70, 72 )
-- UI_LoadImage( imgPass2, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

-- imgEmail = UI_CreateCompent( frmRegister, IMAGE_TYPE, "imgEmail", 87, 18, 70, 94 )
-- UI_LoadImage( imgEmail, "texture/ui/PublicC.tga", NORMAL, 87, 18, 99, 235 )

btnRegYes = UI_CreateCompent( frmRegister, BUTTON_TYPE, "btnRegYes", 41, 19, 82, 148 )
UI_LoadButtonImage( btnRegYes, "texture/ui/corsairs/coButtons.png", 41, 19, 0, 152, TRUE )

btnRegNo = UI_CreateCompent( frmRegister, BUTTON_TYPE, "btnRegNo", 41, 19, 148, 148 )
UI_LoadButtonImage( btnRegNo, "texture/ui/corsairs/coButtons.png", 41, 19, 0, 133, TRUE )

-- YES/NO BUTTONS
labYes = UI_CreateCompent( frmRegister, LABELEX_TYPE, "labYes", 42, 10, 107, 155 )
UI_SetCaption( labYes, "" )
UI_SetTextColor( labYes, COLOR_WHITE )
UI_SetLabelExFont( labYes, DEFAULT_FONT, TRUE, COLOR_BLACK )

labNo = UI_CreateCompent( frmRegister, LABELEX_TYPE, "labNo", 42, 10, 152, 155 )
UI_SetCaption( labNo, "" )
UI_SetTextColor( labNo, COLOR_WHITE )
UI_SetLabelExFont( labNo, DEFAULT_FONT, TRUE, COLOR_BLACK )

-- TYPING BOXES
edtRegID = UI_CreateCompent( frmRegister, EDIT_TYPE, "edtRegID", 130, 12, 120, 51 )
UI_SetTextColor( edtRegID, COLOR_BLACK )
UI_SetEditMaxNum( edtRegID, 20 )
UI_SetEditMaxNumVisible( edtRegID, 12 )

edtRegPassword = UI_CreateCompent( frmRegister, EDIT_TYPE, "edtRegPassword", 130, 12, 120, 75 )
UI_SetTextColor( edtRegPassword, COLOR_BLACK )
UI_SetEditMaxNum( edtRegPassword, 20 )
UI_SetEditMaxNumVisible( edtRegPassword, 12 )

edtRegPassword2 = UI_CreateCompent( frmRegister, EDIT_TYPE, "edtRegPassword2", 130, 12, 120, 99 )
UI_SetTextColor( edtRegPassword2, COLOR_BLACK )
UI_SetEditMaxNum( edtRegPassword2, 20 )
UI_SetEditMaxNumVisible( edtRegPassword2, 12 )

edtRegEmail = UI_CreateCompent( frmRegister, EDIT_TYPE, "edtRegEmail", 130, 12, 120, 123 )
UI_SetTextColor( edtRegEmail, COLOR_BLACK )
UI_SetEditMaxNum( edtRegEmail, 254 )
UI_SetEditMaxNumVisible( edtRegEmail, 12 )

-----------------------------------------------------------------------
-- Soft keyboard form
-----------------------------------------------------------------------	
-- Create keyboard form
frmKeyboard = UI_CreateForm( "frmKeyboard", FALSE, 447, 122, 396, 178, FALSE, FALSE )
--UI_LoadFormImage( frmKeyboard, "texture/ui/Game.bmp", 198, 207, 0, 0, "", 0, 0 )
UI_AddFormToTemplete( frmKeyboard, FORM_LOGIN )
UI_SetFormStyle( frmKeyboard, 2 )
--UI_SetAlpha( frmKeyboard, 200 )
UI_FormSetIsEscClose( frmKeyboard, FALSE )
--UI_FormSetHotKey( frmKeyboard, ALT_KEY, 75 ) --'K'


-- Load background images
imgKeyboard1 = UI_CreateCompent( frmKeyboard, IMAGE_TYPE, "imgKeyboard1", 256, 122, -4, 0 )
UI_LoadImage( imgKeyboard1, "texture/ui/Keyboard1.tga", NORMAL, 256, 122, 0, 0 )
UI_SetImageAlpha( imgKeyboard1, 210 )

imgKeyboard2 = UI_CreateCompent( frmKeyboard, IMAGE_TYPE, "imgKeyboard2", 191, 122, 252, 0 )
UI_LoadImage( imgKeyboard2, "texture/ui/Keyboard1.tga", NORMAL, 191, 122, 0, 123 )
UI_SetImageAlpha( imgKeyboard2, 210 )

-- Title label
labTitle1 = UI_CreateCompent( frmKeyboard, LABELEX_TYPE, "labTitle1", 24, 10, 112, 4 )
UI_SetCaption( labTitle1, "For the safety of your account" )
UI_SetTextColor( labTitle1, COLOR_BLACK )
UI_SetLabelExFont( labTitle1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTitle2 = UI_CreateCompent( frmKeyboard, LABELEX_TYPE, "labTitle2", 24, 10, 36, 16 )
UI_SetCaption( labTitle2, "please use the soft keyboard to input your password" )
UI_SetTextColor( labTitle2, COLOR_BLACK )
UI_SetLabelExFont( labTitle2, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Number buttons
btnNum1 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnNum1", 27, 17, 28, 50 )
UI_LoadButtonImage( btnNum1, "texture/ui/Keyboard2.tga", 27, 17, 108, 17, TRUE )
btnNum2 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnNum2", 27, 17, 55, 50 )
UI_LoadButtonImage( btnNum2, "texture/ui/Keyboard2.tga", 27, 17, 0, 102, TRUE )
btnNum3 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnNum3", 27, 17, 82, 50 )
UI_LoadButtonImage( btnNum3, "texture/ui/Keyboard2.tga", 27, 17, 108, 102, TRUE )
btnNum4 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnNum4", 27, 17, 109, 50 )
UI_LoadButtonImage( btnNum4, "texture/ui/Keyboard2.tga", 27, 17, 0, 187, TRUE )
btnNum5 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnNum5", 27, 17, 136, 50 )
UI_LoadButtonImage( btnNum5, "texture/ui/Keyboard2.tga", 27, 17, 108, 187, TRUE )
btnNum6 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnNum6", 27, 17, 163, 50 )
UI_LoadButtonImage( btnNum6, "texture/ui/Keyboard3.tga", 27, 17, 0, 17, TRUE )
btnNum7 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnNum7", 27, 17, 190, 50 )
UI_LoadButtonImage( btnNum7, "texture/ui/Keyboard3.tga", 27, 17, 108, 17, TRUE )
btnNum8 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnNum8", 27, 17, 217, 50 )
UI_LoadButtonImage( btnNum8, "texture/ui/Keyboard3.tga", 27, 17, 0, 102, TRUE )
btnNum9 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnNum9", 27, 17, 244, 50 )
UI_LoadButtonImage( btnNum9, "texture/ui/Keyboard3.tga", 27, 17, 108, 102, TRUE )
btnNum0 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnNum0", 27, 17, 271, 50 )
UI_LoadButtonImage( btnNum0, "texture/ui/Keyboard3.tga", 27, 17, 0, 187, TRUE )

-- Letter buttons
btnLetterA = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterA", 27, 17, 1, 84 )
UI_LoadButtonImage( btnLetterA, "texture/ui/Keyboard2.tga", 27, 17, 0, 51, TRUE )
btnLetterB = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterB", 27, 17, 109, 101 )
UI_LoadButtonImage( btnLetterB, "texture/ui/Keyboard2.tga", 27, 17, 0, 238, TRUE )
btnLetterC = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterC", 27, 17, 55, 101 )
UI_LoadButtonImage( btnLetterC, "texture/ui/Keyboard2.tga", 27, 17, 0, 153, TRUE )
btnLetterD = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterD", 27, 17, 55, 84 )
UI_LoadButtonImage( btnLetterD, "texture/ui/Keyboard2.tga", 27, 17, 0, 136, TRUE )
btnLetterE = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterE", 27, 17, 55, 67 )
UI_LoadButtonImage( btnLetterE, "texture/ui/Keyboard2.tga", 27, 17, 0, 119, TRUE )
btnLetterF = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterF", 27, 17, 82, 84 )
UI_LoadButtonImage( btnLetterF, "texture/ui/Keyboard2.tga", 27, 17, 108, 136, TRUE )
btnLetterG = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterG", 27, 17, 109, 84 )
UI_LoadButtonImage( btnLetterG, "texture/ui/Keyboard2.tga", 27, 17, 0, 221, TRUE )
btnLetterH = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterH", 27, 17, 136, 84 )
UI_LoadButtonImage( btnLetterH, "texture/ui/Keyboard2.tga", 27, 17, 108, 221, TRUE )
btnLetterI = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterI", 27, 17, 190, 67 )
UI_LoadButtonImage( btnLetterI, "texture/ui/Keyboard3.tga", 27, 17, 108, 34, TRUE )
btnLetterJ = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterJ", 27, 17, 163, 84 )
UI_LoadButtonImage( btnLetterJ, "texture/ui/Keyboard3.tga", 27, 17, 0, 51, TRUE )
btnLetterK = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterK", 27, 17, 190, 84 )
UI_LoadButtonImage( btnLetterK, "texture/ui/Keyboard3.tga", 27, 17, 108, 51, TRUE )
btnLetterL = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterL", 27, 17, 217, 84 )
UI_LoadButtonImage( btnLetterL, "texture/ui/Keyboard3.tga", 27, 17, 0, 136, TRUE )
btnLetterM = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterM", 27, 17, 163, 101 )
UI_LoadButtonImage( btnLetterM, "texture/ui/Keyboard3.tga", 27, 17, 0, 68, TRUE )
btnLetterN = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterN", 27, 17, 136, 101 )
UI_LoadButtonImage( btnLetterN, "texture/ui/Keyboard2.tga", 27, 17, 108, 238, TRUE )
btnLetterO = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterO", 27, 17, 217, 67 )
UI_LoadButtonImage( btnLetterO, "texture/ui/Keyboard3.tga", 27, 17, 0, 119, TRUE )
btnLetterP = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterP", 27, 17, 244, 67 )
UI_LoadButtonImage( btnLetterP, "texture/ui/Keyboard3.tga", 27, 17, 108, 119, TRUE )
btnLetterQ = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterQ", 27, 17, 1, 67 )
UI_LoadButtonImage( btnLetterQ, "texture/ui/Keyboard2.tga", 27, 17, 0, 34, TRUE )
btnLetterR = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterR", 27, 17, 82, 67 )
UI_LoadButtonImage( btnLetterR, "texture/ui/Keyboard2.tga", 27, 17, 108, 119, TRUE )
btnLetterS = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterS", 27, 17, 28, 84 )
UI_LoadButtonImage( btnLetterS, "texture/ui/Keyboard2.tga", 27, 17, 108, 51, TRUE )
btnLetterT = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterT", 27, 17, 109, 67 )
UI_LoadButtonImage( btnLetterT, "texture/ui/Keyboard2.tga", 27, 17, 0, 204, TRUE )
btnLetterU = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterU", 27, 17, 163, 67 )
UI_LoadButtonImage( btnLetterU, "texture/ui/Keyboard3.tga", 27, 17, 0, 34, TRUE )
btnLetterV = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterV", 27, 17, 82, 101 )
UI_LoadButtonImage( btnLetterV, "texture/ui/Keyboard2.tga", 27, 17, 108, 153, TRUE )
btnLetterW = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterW", 27, 17, 28, 67 )
UI_LoadButtonImage( btnLetterW, "texture/ui/Keyboard2.tga", 27, 17, 108, 34, TRUE )
btnLetterX = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterX", 27, 17, 28, 101 )
UI_LoadButtonImage( btnLetterX, "texture/ui/Keyboard2.tga", 27, 17, 108, 68, TRUE )
btnLetterY = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterY", 27, 17, 136, 67 )
UI_LoadButtonImage( btnLetterY, "texture/ui/Keyboard2.tga", 27, 17, 108, 204, TRUE )
btnLetterZ = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnLetterZ", 27, 17, 1, 101 )
UI_LoadButtonImage( btnLetterZ, "texture/ui/Keyboard2.tga", 27, 17, 0, 68, TRUE )

--- Function buttons
-- Confirm button
btnYes = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnYes", 83, 34, 352, 84 )
UI_LoadButtonImage( btnYes, "texture/ui/Keyboard4.tga", 83, 34, 57, 85, FALSE )
--UI_SetAlign( btnYes, caWidthCenter )
UI_SetCaption( btnYes, "" )

-- Close button
btnClose = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnClose", 57, 17, 379, 8 )
UI_LoadButtonImage( btnClose, "texture/ui/Keyboard4.tga", 57, 17, 140, 153, FALSE )



-- Caps/shift toggle
chkShift = UI_CreateCompent( frmKeyboard, CHECK_TYPE, "chkShift", 57, 17, 379, 67 )
UI_LoadImage( chkShift, "texture/ui/Keyboard4.tga", UNCHECKED, 57, 17, 140, 85 )
UI_LoadImage( chkShift, "texture/ui/Keyboard4.tga", CHECKED, 57, 17, 140, 119 )



-- Backspace button
btnDel = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnDel", 57, 34, 379, 33 )
UI_LoadButtonImage( btnDel, "texture/ui/Keyboard4.tga", 57, 34, 0, 85, FALSE )
--UI_SetAlign( btnDel, caWidthCenter )
UI_SetCaption( btnDel, "" )

-- Special character buttons
btnOther101 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther101", 27, 17, 1, 33 )
UI_LoadButtonImage( btnOther101, "texture/ui/Keyboard2.tga", 27, 17, 0, 0, TRUE )
btnOther102 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther102", 27, 17, 28, 33 )
UI_LoadButtonImage( btnOther102, "texture/ui/Keyboard2.tga", 27, 17, 108, 0, TRUE )
btnOther103 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther103", 27, 17, 55, 33 )
UI_LoadButtonImage( btnOther103, "texture/ui/Keyboard2.tga", 27, 17, 0, 85, TRUE )
btnOther104 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther104", 27, 17, 82, 33 )
UI_LoadButtonImage( btnOther104, "texture/ui/Keyboard2.tga", 27, 17, 108, 85, TRUE )
btnOther105 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther105", 27, 17, 109, 33 )
UI_LoadButtonImage( btnOther105, "texture/ui/Keyboard2.tga", 27, 17, 0, 170, TRUE )
btnOther106 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther106", 27, 17, 136, 33 )
UI_LoadButtonImage( btnOther106, "texture/ui/Keyboard2.tga", 27, 17, 108, 170, TRUE )
btnOther107 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther107", 27, 17, 163, 33 )
UI_LoadButtonImage( btnOther107, "texture/ui/Keyboard3.tga", 27, 17, 0, 0, TRUE )
btnOther108 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther108", 27, 17, 190, 33 )
UI_LoadButtonImage( btnOther108, "texture/ui/Keyboard3.tga", 27, 17, 108, 0, TRUE )
btnOther109 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther109", 27, 17, 217, 33 )
UI_LoadButtonImage( btnOther109, "texture/ui/Keyboard3.tga", 27, 17, 0, 85, TRUE )
btnOther110 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther110", 27, 17, 244, 33 )
UI_LoadButtonImage( btnOther110, "texture/ui/Keyboard3.tga", 27, 17, 108, 85, TRUE )
btnOther111 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther111", 27, 17, 271, 33 )
UI_LoadButtonImage( btnOther111, "texture/ui/Keyboard3.tga", 27, 17, 0, 170, TRUE )
btnOther112 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther112", 27, 17, 298, 33 )
UI_LoadButtonImage( btnOther112, "texture/ui/Keyboard3.tga", 27, 17, 108, 170, TRUE )
btnOther113 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther113", 27, 17, 325, 33 )
UI_LoadButtonImage( btnOther113, "texture/ui/Keyboard4.tga", 27, 17, 0, 0, TRUE )
btnOther114 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther114", 27, 17, 352, 33 )
UI_LoadButtonImage( btnOther114, "texture/ui/Keyboard4.tga", 27, 17, 108, 0, TRUE )

btnOther201 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther201", 27, 17, 1, 50 )
UI_LoadButtonImage( btnOther201, "texture/ui/Keyboard2.tga", 27, 17, 0, 17, TRUE )
btnOther202 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther202", 27, 17, 298, 50 )
UI_LoadButtonImage( btnOther202, "texture/ui/Keyboard3.tga", 27, 17, 108, 187, TRUE )
btnOther203 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther203", 27, 17, 325, 50 )
UI_LoadButtonImage( btnOther203, "texture/ui/Keyboard4.tga", 27, 17, 0, 17, TRUE )
btnOther204 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther204", 27, 17, 352, 50 )
UI_LoadButtonImage( btnOther204, "texture/ui/Keyboard4.tga", 27, 17, 108, 17, TRUE )

btnOther301 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther301", 27, 17, 271, 67 )
UI_LoadButtonImage( btnOther301, "texture/ui/Keyboard3.tga", 27, 17, 0, 204, TRUE )
btnOther302 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther302", 27, 17, 298, 67 )
UI_LoadButtonImage( btnOther302, "texture/ui/Keyboard3.tga", 27, 17, 108, 204, TRUE )
btnOther303 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther303", 27, 17, 325, 67 )
UI_LoadButtonImage( btnOther303, "texture/ui/Keyboard4.tga", 27, 17, 0, 34, TRUE )
btnOther304 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther304", 27, 17, 352, 67 )
UI_LoadButtonImage( btnOther304, "texture/ui/Keyboard4.tga", 27, 17, 108, 34, TRUE )

btnOther401 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther401", 27, 17, 244, 84 )
UI_LoadButtonImage( btnOther401, "texture/ui/Keyboard3.tga", 27, 17, 108, 136, TRUE )
btnOther402 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther402", 27, 17, 271, 84 )
UI_LoadButtonImage( btnOther402, "texture/ui/Keyboard3.tga", 27, 17, 0, 221, TRUE )
btnOther403 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther403", 27, 17, 298, 84 )
UI_LoadButtonImage( btnOther403, "texture/ui/Keyboard3.tga", 27, 17, 108, 221, TRUE )
btnOther404 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther404", 27, 17, 325, 84 )
UI_LoadButtonImage( btnOther404, "texture/ui/Keyboard4.tga", 27, 17, 0, 51, TRUE )

btnOther501 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther501", 27, 17, 190, 101 )
UI_LoadButtonImage( btnOther501, "texture/ui/Keyboard3.tga", 27, 17, 108, 68, TRUE )
btnOther502 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther502", 27, 17, 217, 101 )
UI_LoadButtonImage( btnOther502, "texture/ui/Keyboard3.tga", 27, 17, 0, 153, TRUE )
btnOther503 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther503", 27, 17, 244, 101 )
UI_LoadButtonImage( btnOther503, "texture/ui/Keyboard3.tga", 27, 17, 108, 153, TRUE )
btnOther504 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther504", 27, 17, 271, 101 )
UI_LoadButtonImage( btnOther504, "texture/ui/Keyboard3.tga", 27, 17, 0, 238, TRUE )
btnOther505 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther505", 27, 17, 298, 101 )
UI_LoadButtonImage( btnOther505, "texture/ui/Keyboard3.tga", 27, 17, 108, 238, TRUE )
btnOther506 = UI_CreateCompent( frmKeyboard, BUTTON_TYPE, "btnOther506", 27, 17, 325, 101 )
UI_LoadButtonImage( btnOther506, "texture/ui/Keyboard4.tga", 27, 17, 0, 68, TRUE )


-----------------------------------------------------------------------
-- Create FORM1 -added by Arcol
-----------------------------------------------------------------------	
frmTest1 = UI_CreateForm( "frmTest1_Arcol", FALSE, 160, 120, 600, 200, FALSE, FALSE )
UI_LoadFormImage( frmTest1, "texture/ui/Test1.bmp", 160, 120, 0, 0, "", 0, 0 )
UI_AddFormToTemplete( frmTest1, FORM_LOGIN )
UI_SetIsDrag( frmTest1, TRUE )

-----------------------------------------------------------------------
-- Create FORM2 -added by Arcol
-----------------------------------------------------------------------	
frmTest2 = UI_CreateForm( "frmTest2_Arcol", FALSE, 160, 120, 600, 400, FALSE, FALSE )
UI_LoadFormImage( frmTest2, "texture/ui/Test2.bmp", 160, 120, 0, 0, "", 0, 0 )
UI_AddFormToTemplete( frmTest2, FORM_LOGIN )
UI_SetIsDrag( frmTest2, TRUE )
labTest = UI_CreateCompent( frmTest2, LABELEX_TYPE, "labTest", 24, 10, 20, 90 )
UI_SetCaption( labTest, "Speed Test -by Arcol" )
UI_SetTextColor( labTest, COLOR_BLACK )
UI_SetLabelExFont( labTest, DEFAULT_FONT, TRUE, COLOR_BLACK )

---------------------
-- Double password --
---------------------
frmDoublePwdCreate = UI_CreateForm("frmDoublePwdCreate", FALSE, 247, 129, 200, 200, FALSE, FALSE)
UI_AddFormToTemplete( frmDoublePwdCreate, FORM_SELECT )
--UI_SetFormStyle( frmDoublePwdCreate, 2 )
UI_FormSetIsEscClose( frmDoublePwdCreate, FALSE)
UI_SetIsDrag( frmDoublePwdCreate, FALSE)
UI_ShowForm( frmDoublePwdCreate, FALSE)

UI_FormSetEnterButton(frmDoublePwdCreate,btnYes)

--UI_FormSetHotKey( frmDoublePwdCreate, ALT_KEY, HOTKEY_V ) 

imgDoublePwdCreate = UI_CreateCompent( frmDoublePwdCreate, IMAGE_TYPE,"imgDoublePwdCreate" ,247, 129, 0, 0)
UI_LoadImage(imgDoublePwdCreate, "texture/ui/a001.tga", NORMAL , 247, 129, 0, 0)

btnYes = UI_CreateCompent( frmDoublePwdCreate, BUTTON_TYPE, "btnYes", 41, 19, 130, 107 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnClear = UI_CreateCompent( frmDoublePwdCreate, BUTTON_TYPE, "btnClear", 41, 19, 175, 107 )
UI_LoadButtonImage( btnClear, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

lab1 = UI_CreateCompent( frmDoublePwdCreate, LABELEX_TYPE, "lab1", 210, 11, 17, 35)
UI_SetCaption( lab1, "Enter security code" )
UI_SetTextColor( lab1,COLOR_BLACK )
UI_SetLabelExFont( lab1, DEFAULT_FONT, TRUE, COLOR_WHITE )
edtDoublePwdCreate = UI_CreateCompent(frmDoublePwdCreate, EDIT_TYPE,"edtDoublePwdCreate" ,66, 14, 137, 35)
UI_SetEditMaxNum( edtDoublePwdCreate, 12 )
UI_SetEditMaxNumVisible(edtDoublePwdCreate,12)

lab2 = UI_CreateCompent( frmDoublePwdCreate, LABELEX_TYPE, "lab2", 210, 11, 17, 60)
UI_SetCaption( lab2, "Re-enter" )
UI_SetTextColor( lab2,COLOR_BLACK )
UI_SetLabelExFont( lab2, DEFAULT_FONT, TRUE, COLOR_WHITE )
edtDoublePwdCreateRetry = UI_CreateCompent(frmDoublePwdCreate, EDIT_TYPE,"edtDoublePwdCreateRetry", 66, 14, 137, 59)
UI_SetEditMaxNum( edtDoublePwdCreateRetry, 12 )
UI_SetEditMaxNumVisible(edtDoublePwdCreateRetry,12)

-- Title
--labID = UI_CreateCompent( frmDoublePwdCreate, LABELEX_TYPE, "labID", 210, 11, 16, 84)
--UI_SetCaption( labID, "For the safety of your account, please make a security code" )
--UI_SetTextColor( labID,COLOR_RED )
--UI_SetLabelExFont( labID, DEFAULT_FONT, TRUE, COLOR_WHITE )


------------------------
-- Edit Security Code --
------------------------
frmDoublePwdAlter = UI_CreateForm("frmDoublePwdAlter", TRUE, 247, 129, 200, 200, TRUE, TRUE)
UI_ShowForm(frmDoublePwdAlter, FALSE)
--UI_SetFormStyle( frmDoublePwdAlter, 2 )
UI_SetIsDrag(frmDoublePwdAlter, FALSE)
UI_AddFormToTemplete(frmDoublePwdAlter, FORM_SELECT )
UI_FormSetIsEscClose(frmDoublePwdAlter, TRUE)

UI_FormSetEnterButton(frmDoublePwdAlter,btnYes)
--UI_FormSetHotKey( frmDoublePwdAlter, ALT_KEY, HOTKEY_C )

imgDoublePwdAlter = UI_CreateCompent(frmDoublePwdAlter, IMAGE_TYPE,"imgDoublePwdAlter", 247, 129, 0, 0)
UI_LoadImage( imgDoublePwdAlter, "texture/ui/a002.tga", NORMAL, 247, 129, 0, 0)

btnYes = UI_CreateCompent( frmDoublePwdAlter, BUTTON_TYPE, "btnYes", 41, 19, 130, 107 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnClear = UI_CreateCompent( frmDoublePwdAlter, BUTTON_TYPE, "btnClear", 41, 19, 175, 107 )
UI_LoadButtonImage( btnClear, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

--title
labTitle = UI_CreateCompent( frmDoublePwdAlter, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Edit security code")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

lab1 = UI_CreateCompent( frmDoublePwdAlter, LABELEX_TYPE, "lab1", 210, 11, 16, 32)
UI_SetCaption( lab1, "Enter old code" )
UI_SetTextColor( lab1,COLOR_BLACK )
UI_SetLabelExFont( lab1, DEFAULT_FONT, TRUE, COLOR_WHITE )
edtDoublePwdAlterOld = UI_CreateCompent(frmDoublePwdAlter, EDIT_TYPE,"edtDoublePwdAlterOld", 66, 14, 137, 34)
UI_SetEditMaxNum( edtDoublePwdAlterOld, 12 )
UI_SetEditMaxNumVisible(edtDoublePwdAlterOld,12)

lab2 = UI_CreateCompent( frmDoublePwdAlter, LABELEX_TYPE, "lab2", 210, 11, 16, 56)
UI_SetCaption( lab2, "Enter new code" )
UI_SetTextColor( lab2,COLOR_BLACK )
UI_SetLabelExFont( lab2, DEFAULT_FONT, TRUE, COLOR_WHITE )
edtDoublePwdAlterNew = UI_CreateCompent(frmDoublePwdAlter, EDIT_TYPE,"edtDoublePwdAlterNew", 66, 14, 137, 58)
UI_SetEditMaxNum( edtDoublePwdAlterNew, 12 )
UI_SetEditMaxNumVisible(edtDoublePwdAlterNew,12)
lab3 = UI_CreateCompent( frmDoublePwdAlter, LABELEX_TYPE, "lab3", 210, 11, 16, 80)
UI_SetCaption( lab3, "Re-enter" )
UI_SetTextColor( lab3,COLOR_BLACK )
UI_SetLabelExFont( lab3, DEFAULT_FONT, TRUE, COLOR_WHITE )

edtDoublePwdAlterNewRetry = UI_CreateCompent(frmDoublePwdAlter, EDIT_TYPE,"edtDoublePwdAlterNewRetry" ,66, 14, 137, 82)
UI_SetEditMaxNum( edtDoublePwdAlterNewRetry, 12 )
UI_SetEditMaxNumVisible(edtDoublePwdAlterNewRetry,12)

---------------------
-- double password --
---------------------
frmDoublePwd = UI_CreateForm("frmDoublePwd", TRUE, 247, 129, 200, 200, TRUE, TRUE)
UI_ShowForm( frmDoublePwd, FALSE )
--UI_SetFormStyle( frmDoublePwd, 2 )
UI_AddFormToTemplete( frmDoublePwd, FORM_SELECT )
UI_SetIsDrag(frmDoublePwd, FALSE)
UI_FormSetIsEscClose( frmDoublePwd, TRUE)

UI_FormSetEnterButton(frmDoublePwd,btnYes)
--UI_FormSetHotKey( frmDoublePwd, ALT_KEY, HOTKEY_Z )

imgDoublePwd = UI_CreateCompent(frmDoublePwd, IMAGE_TYPE,"imgDoublePwd", 247, 129, 0, 0)
UI_LoadImage( imgDoublePwd, "texture/ui/a003.tga", NORMAL, 247, 129, 0, 0)

labTitle = UI_CreateCompent( frmDoublePwd, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Enter security code")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

lab1 = UI_CreateCompent( frmDoublePwd, LABELEX_TYPE, "lab1", 210, 11, 16, 46)
UI_SetCaption( lab1, "Enter security code" )
UI_SetTextColor( lab1,COLOR_BLACK )
UI_SetLabelExFont( lab1, DEFAULT_FONT, TRUE, COLOR_WHITE )


edtDoublePwd = UI_CreateCompent(frmDoublePwd, EDIT_TYPE,"edtDoublePwd", 66, 14, 137, 44)
UI_SetIsKeyFocus(edtDoublePwd,FALSE)
UI_SetEditMaxNum(edtDoublePwd, 12 )
UI_SetEditMaxNumVisible(edtDoublePwd,12)

btnYes = UI_CreateCompent( frmDoublePwd, BUTTON_TYPE, "btnYes", 41, 19, 130, 107 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnClear = UI_CreateCompent( frmDoublePwd, BUTTON_TYPE, "btnClear", 41, 19, 175, 107 )
UI_LoadButtonImage( btnClear, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

------------------------------
-- Double password keyboard -- 
------------------------------	
frmDoublePwdInput = UI_CreateForm("frmDoublePwdInput", TRUE, 131, 129, 392, 300, TRUE,TRUE)
UI_ShowForm(frmDoublePwdInput, FALSE )
UI_AddAllFormTemplete( frmDoublePwdInput)
UI_SetIsDrag(frmDoublePwdInput, FALSE)
UI_FormSetIsEscClose(frmDoublePwdInput, TRUE)

--UI_FormSetHotKey( frmDoublePwdInput, ALT_KEY, HOTKEY_X ) 

imgDoublePwdInput = UI_CreateCompent(frmDoublePwdInput, IMAGE_TYPE,"imgDoublePwdInput", 131, 129, 0, 0)
UI_LoadImage( imgDoublePwdInput, "texture/ui/a004.tga", NORMAL, 131,129, 0, 0)

btnNum1 = UI_CreateCompent( frmDoublePwdInput, BUTTON_TYPE, "btnNum1", 27, 17, 17, 26 )
UI_LoadButtonImage( btnNum1, "texture/ui/Keyboard2.tga", 27, 17, 108, 17, TRUE )
btnNum2 = UI_CreateCompent( frmDoublePwdInput, BUTTON_TYPE, "btnNum2", 27, 17, 51, 26 )
UI_LoadButtonImage( btnNum2, "texture/ui/Keyboard2.tga", 27, 17, 0, 102, TRUE )
btnNum3 = UI_CreateCompent( frmDoublePwdInput, BUTTON_TYPE, "btnNum3", 27, 17, 85, 26 )
UI_LoadButtonImage( btnNum3, "texture/ui/Keyboard2.tga", 27, 17, 108, 102, TRUE )
btnNum4 = UI_CreateCompent( frmDoublePwdInput, BUTTON_TYPE, "btnNum4", 27, 17, 17, 50 )
UI_LoadButtonImage( btnNum4, "texture/ui/Keyboard2.tga", 27, 17, 0, 187, TRUE )
btnNum5 = UI_CreateCompent( frmDoublePwdInput, BUTTON_TYPE, "btnNum5", 27, 17, 51, 50 )
UI_LoadButtonImage( btnNum5, "texture/ui/Keyboard2.tga", 27, 17, 108, 187, TRUE )
btnNum6 = UI_CreateCompent( frmDoublePwdInput, BUTTON_TYPE, "btnNum6", 27, 17, 85, 50 )
UI_LoadButtonImage( btnNum6, "texture/ui/Keyboard3.tga", 27, 17, 0, 17, TRUE )
btnNum7 = UI_CreateCompent( frmDoublePwdInput, BUTTON_TYPE, "btnNum7", 27, 17, 17, 74 )
UI_LoadButtonImage( btnNum7, "texture/ui/Keyboard3.tga", 27, 17, 108, 17, TRUE )
btnNum8 = UI_CreateCompent( frmDoublePwdInput, BUTTON_TYPE, "btnNum8", 27, 17, 51, 74 )
UI_LoadButtonImage( btnNum8, "texture/ui/Keyboard3.tga", 27, 17, 0, 102, TRUE )
btnNum9 = UI_CreateCompent( frmDoublePwdInput, BUTTON_TYPE, "btnNum9", 27, 17, 85, 74 )
UI_LoadButtonImage( btnNum9, "texture/ui/Keyboard3.tga", 27, 17, 108, 102, TRUE )
btnNum0 = UI_CreateCompent( frmDoublePwdInput, BUTTON_TYPE, "btnNum0", 27, 17, 17, 98 )
UI_LoadButtonImage( btnNum0, "texture/ui/Keyboard3.tga", 27, 17, 0, 187, TRUE )

btnClear = UI_CreateCompent( frmDoublePwdInput, BUTTON_TYPE, "btnClear", 61, 17, 51, 98 )
UI_LoadButtonImage( btnClear, "texture/ui/a002.tga", 54, 17, 0, 129, TRUE )

---------------------------------------
-- Double Security password Reminder -- 
---------------------------------------
frmDoublePwdInfo =  UI_CreateForm("frmDoublePwdInfo", TRUE, 378, 129, 200, 329, TRUE,TRUE)
UI_ShowForm(frmDoublePwdInfo, TRUE )
UI_AddFormToTemplete( frmDoublePwdInfo, FORM_SELECT )
UI_SetIsDrag(frmDoublePwdInfo, FALSE)

--UI_FormSetHotKey( frmDoublePwdInfo, ALT_KEY, HOTKEY_Z ) 

imgWelcomeNotice1 = UI_CreateCompent(frmDoublePwdInfo, IMAGE_TYPE,"imgWelcomeNotice1", 256, 122, 0, 0)
UI_LoadImage( imgWelcomeNotice1, "texture/ui/Keyboard1.tga", NORMAL, 256,122, 0, 0)
UI_SetImageAlpha(imgWelcomeNotice1,220)

imgWelcomeNotice2 = UI_CreateCompent(frmDoublePwdInfo, IMAGE_TYPE,"imgWelcomeNotice2", 122, 122, 256, 0)
UI_LoadImage( imgWelcomeNotice2, "texture/ui/Keyboard1.tga", NORMAL, 122,122, 69, 123)
UI_SetImageAlpha(imgWelcomeNotice2,220)

labText1  = UI_CreateCompent( frmDoublePwdInfo, LABELEX_TYPE, "labText1", 400, 150, 10, 5 )
labText2  = UI_CreateCompent( frmDoublePwdInfo, LABELEX_TYPE, "labText2", 400, 150, 10, 25 )
labText3  = UI_CreateCompent( frmDoublePwdInfo, LABELEX_TYPE, "labText3", 400, 150, 10, 45 )
labText4  = UI_CreateCompent( frmDoublePwdInfo, LABELEX_TYPE, "labText4", 400, 150, 10, 65 )
labText5  = UI_CreateCompent( frmDoublePwdInfo, LABELEX_TYPE, "labText5", 400, 150, 10, 85 )
--labText6  = UI_CreateCompent( frmDoublePwdInfo, LABELEX_TYPE, "labText6", 400, 150, 10, 105 )
UI_SetCaption( labText1, "Dear player, you have not create a security code.")
UI_SetCaption( labText2, "Please select a security code to keep your account safe.")
UI_SetCaption( labText3, "You will require the security code to")
UI_SetCaption( labText4, "delete character or unlock inventory")
UI_SetCaption( labText5, "Please remember your security code well!")
--UI_SetCaption( labText6,"Requires a valid Security Code to delete character or withdraw inventory items")
UI_SetTextColor( labText1, COLOR_BLACK )
UI_SetLabelExFont( labText1, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText2, COLOR_BLACK )
UI_SetLabelExFont( labText2, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText3, COLOR_BLACK )
UI_SetLabelExFont( labText3, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText4, COLOR_BLACK )
UI_SetLabelExFont( labText4, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText5, COLOR_BLACK )
UI_SetLabelExFont( labText5, DEFAULT_FONT, TRUE, COLOR_WHITE )
--UI_SetTextColor( labText6, COLOR_BLACK )
--UI_SetLabelExFont( labText6, DEFAULT_FONT, TRUE, COLOR_WHITE )

-----------------------------------------------------------------------
-- Version logo form
-----------------------------------------------------------------------	
-- Create version logo form
frmPathLogo = UI_CreateForm("frmPathLogo", TRUE, 256, 129, 50 , 450, TRUE,TRUE)
UI_ShowForm(frmPathLogo, FALSE )
UI_AddAllFormTemplete( frmPathLogo)
UI_SetIsDrag(frmPathLogo, FALSE)
UI_FormSetIsEscClose(frmPathLogo, FALSE)

--UI_FormSetHotKey( frmPathLogo, ALT_KEY, HOTKEY_Z ) 

-- Load background image
imgPathLogo = UI_CreateCompent(frmPathLogo, IMAGE_TYPE,"imgPathLogo", 256, 129, 0, 0)
--UI_LoadImage( imgPathLogo, "texture/ui/PathLogo.tga", NORMAL, 256,129, 0, 0)

--------------------
-- Welcome Notice -- 
--------------------
frmWelcomeNotice = UI_CreateForm("frmWelcomeNotice", TRUE, 500, 170, 0 , 0, TRUE,TRUE)
UI_ShowForm(frmWelcomeNotice, FALSE )
UI_SetIsDrag(frmWelcomeNotice, FALSE)
UI_FormSetIsEscClose(frmWelcomeNotice, FALSE)
UI_SetFormStyle(frmWelcomeNotice,1)
UI_AddFormToTemplete( frmWelcomeNotice, FORM_SELECT )

--UI_FormSetHotKey( frmWelcomeNotice, ALT_KEY, HOTKEY_Z ) 

imgWelcomeNotice1 = UI_CreateCompent(frmWelcomeNotice, IMAGE_TYPE,"imgWelcomeNotice1", 256, 170, 0, 0)
UI_LoadImage( imgWelcomeNotice1, "texture/ui/newshelp/login_out_01.tga", NORMAL, 256,170, 0, 0)
UI_SetImageAlpha(imgWelcomeNotice1,220)

imgWelcomeNotice2 = UI_CreateCompent(frmWelcomeNotice, IMAGE_TYPE,"imgWelcomeNotice2", 244, 170, 256, 0)
UI_LoadImage( imgWelcomeNotice2, "texture/ui/newshelp/login_out_02.tga", NORMAL, 244,170, 0, 0)
UI_SetImageAlpha(imgWelcomeNotice2,220)

labTitle = UI_CreateCompent( frmWelcomeNotice, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Newbie Guide")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labText1  = UI_CreateCompent( frmWelcomeNotice, LABELEX_TYPE, "labText1", 400, 150, 20, 25 )
labText2  = UI_CreateCompent( frmWelcomeNotice, LABELEX_TYPE, "labText2", 400, 150, 20, 45 )
labText3  = UI_CreateCompent( frmWelcomeNotice, LABELEX_TYPE, "labText3", 400, 150, 20, 65 )
labText4  = UI_CreateCompent( frmWelcomeNotice, LABELEX_TYPE, "labText4", 400, 150, 20, 85 )
labText5  = UI_CreateCompent( frmWelcomeNotice, LABELEX_TYPE, "labText5", 400, 150, 20, 105 )
labText6  = UI_CreateCompent( frmWelcomeNotice, LABELEX_TYPE, "labText6", 400, 150, 20, 125 )
UI_SetCaption( labText1, "Dear player, you can now enter the fantasy world of Tales of Pirates")
UI_SetCaption( labText2,"Please select the \"Create\" button to make your first character.")
UI_SetCaption( labText3,"You will be guided you through to allow you to familiarize with the game background")
UI_SetCaption( labText4,"Learn about the unique features of each class and you eptomize your character")
UI_SetCaption( labText5,"Carefully consider a unique name as it cannot be altered once confirmed" )
UI_SetCaption( labText6,"Create a character and join us now!")
UI_SetTextColor( labText1, COLOR_BLACK )
UI_SetLabelExFont( labText1, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText2, COLOR_BLACK )
UI_SetLabelExFont( labText2, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText3, COLOR_BLACK )
UI_SetLabelExFont( labText3, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText4, COLOR_BLACK )
UI_SetLabelExFont( labText4, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText5, COLOR_BLACK )
UI_SetLabelExFont( labText5, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText6, COLOR_BLACK )
UI_SetLabelExFont( labText6, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmWelcomeNotice, BUTTON_TYPE, "btnYes", 41, 19, 230, 145 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

-------------------
-- Create Notice --
-------------------
frmCreateOKNotice = UI_CreateForm("frmCreateOKNotice", TRUE, 500, 170, 0 , 0, TRUE,TRUE)
UI_ShowForm(frmCreateOKNotice, FALSE )
UI_SetIsDrag(frmCreateOKNotice, FALSE)
UI_FormSetIsEscClose(frmCreateOKNotice, FALSE)
UI_SetFormStyle(frmCreateOKNotice,1)
UI_AddFormToTemplete( frmCreateOKNotice, FORM_SELECT )

--UI_FormSetHotKey( frmCreateOKNotice, ALT_KEY, HOTKEY_Z ) 

imgCreateOKNotice1 = UI_CreateCompent(frmCreateOKNotice, IMAGE_TYPE,"imgCreateOKNotice1", 256, 150, 0, 0)
UI_LoadImage( imgCreateOKNotice1, "texture/ui/newshelp/login_out_01.tga", NORMAL, 256,150, 0, 0)
UI_SetImageAlpha(imgCreateOKNotice1,220)

imgCreateOKNotice2 = UI_CreateCompent(frmCreateOKNotice, IMAGE_TYPE,"imgCreateOKNotice2", 244, 150, 256, 0)
UI_LoadImage( imgCreateOKNotice2, "texture/ui/newshelp/login_out_02.tga", NORMAL, 244,150, 0, 0)
UI_SetImageAlpha(imgCreateOKNotice2,220)

labTitle = UI_CreateCompent( frmCreateOKNotice, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Created successfully")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labText1  = UI_CreateCompent( frmCreateOKNotice, LABELEX_TYPE, "labText1", 400, 150, 20, 30 )
labText2  = UI_CreateCompent( frmCreateOKNotice, LABELEX_TYPE, "labText2", 400, 150, 20, 50 )
labText3  = UI_CreateCompent( frmCreateOKNotice, LABELEX_TYPE, "labText3", 400, 150, 20, 70 )
labText4  = UI_CreateCompent( frmCreateOKNotice, LABELEX_TYPE, "labText4", 400, 150, 20, 90 )
UI_SetCaption( labText1, "You have successfully created a character. You can still create 2 more characters")
UI_SetCaption( labText2, "Now you only need to double click on character or select the Enter button to enter game")
UI_SetCaption( labText3, "to enter game and begins your journey")
UI_SetCaption( labText4, "Tips: Look for the Newbie Guide NPC after you entered the game. It will be helpful")
UI_SetTextColor( labText1, COLOR_BLACK )
UI_SetLabelExFont( labText1, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText2, COLOR_BLACK )
UI_SetLabelExFont( labText2, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText3, COLOR_BLACK )
UI_SetLabelExFont( labText3, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetTextColor( labText4, COLOR_BLACK )
UI_SetLabelExFont( labText4, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYes = UI_CreateCompent( frmCreateOKNotice, BUTTON_TYPE, "btnYes", 41, 19, 230, 125 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )


-----------------------------------------------------------------------
-- Server management panel (for internal/beta version, disabled by default)
-----------------------------------------------------------------------	
frmChaNameAlter = UI_CreateForm("frmChaNameAlter", TRUE, 500, 170, 0 , 0, TRUE,TRUE)
UI_ShowForm(frmChaNameAlter, FALSE )
UI_SetIsDrag(frmChaNameAlter, FALSE)
UI_FormSetIsEscClose(frmChaNameAlter, FALSE)
UI_SetFormStyle(frmChaNameAlter,1)
UI_AddFormToTemplete( frmChaNameAlter, FORM_SELECT )

-- Load background
imgCreateOKNotice1 = UI_CreateCompent(frmChaNameAlter, IMAGE_TYPE,"imgCreateOKNotice1", 256, 150, 0, 0)
UI_LoadImage( imgCreateOKNotice1, "texture/ui/newshelp/login_out_01.tga", NORMAL, 256,150, 0, 0)
UI_SetImageAlpha(imgCreateOKNotice1,220)

-- Load background
labTitle = UI_CreateCompent( frmChaNameAlter, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Edit character name")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Edit account
labText1 = UI_CreateCompent( frmChaNameAlter, LABELEX_TYPE, "labText1", 400, 150, 10, 20)
UI_SetCaption( labText1, "Please change character name")
UI_SetTextColor( labText1, COLOR_BLACK )
UI_SetLabelExFont( labText1, DEFAULT_FONT, TRUE, COLOR_WHITE )


-- New character
labTitle = UI_CreateCompent( frmChaNameAlter, LABELEX_TYPE, "labTitle", 400, 150, 40, 30 )
UI_SetCaption( labTitle, "New character name")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- View data
labTitle = UI_CreateCompent( frmChaNameAlter, LABELEX_TYPE, "labTitle", 400, 150, 40, 50 )
UI_SetCaption( labTitle, "Edit character name")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Edit slot
edtChaNameNew = UI_CreateCompent(frmChaNameAlter, EDIT_TYPE,"edtChaNameNew", 66, 14, 140, 30)
UI_SetEditMaxNum( edtDoublePwdAlterNew, 12 )
UI_SetEditMaxNumVisible(edtDoublePwdAlterNew,12)

edtChaNameNewRetry = UI_CreateCompent(frmChaNameAlter, EDIT_TYPE,"edtChaNameNewRetry" ,66, 14, 140, 50)
UI_SetEditMaxNum( edtDoublePwdAlterNewRetry, 12 )
UI_SetEditMaxNumVisible(edtDoublePwdAlterNewRetry,12)

-- Confirm and cancel buttons
btnYes = UI_CreateCompent( frmChaNameAlter, BUTTON_TYPE, "btnYes", 41, 19, 50, 120 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnClear = UI_CreateCompent( frmChaNameAlter, BUTTON_TYPE, "btnClear", 41, 19, 80, 120 )
UI_LoadButtonImage( btnClear, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )



frmModel = UI_CreateForm( "frmModel", FALSE, 280, 450, 0, 0, TRUE, FALSE )
UI_ShowForm( frmModel, TRUE )
UI_AddFormToTemplete( frmModel, FORM_LOGIN )

cardBack = UI_CreateCompent( frmModel, IMAGE_TYPE, "cardBack", 280, 450, 0, 0 )
UI_LoadImage( cardBack, "texture/ui/corsairs/card.png", NORMAL, 280, 450, 22,120 )

iconBack = UI_CreateCompent( frmModel, IMAGE_TYPE, "iconBack", 130, 130, 0, 0 )
UI_LoadImage( iconBack, "texture/ui/corsairs/discord.png", NORMAL, 128, 128, 0,0 )


ui3dCha = UI_CreateCompent( frmModel, UI3D_COMPENT, "ui3dCha", 280, 450, 0, 40 )
ui3dIcon = UI_CreateCompent( frmModel, UI3D_COMPENT, "ui3dIcon", 128, 128, 0, 0 )
