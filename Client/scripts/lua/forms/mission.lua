-----------------------------------------------------------------------
-- Quest ui By aLex
-----------------------------------------------------------------------
frmMission = UI_CreateForm( "frmMission", FALSE, 235, 394, 470, 150, FALSE, FALSE )
UI_FormSetHotKey( frmMission, ALT_KEY, HOTKEY_Q ) 
UI_ShowForm( frmMission, TRUE )
UI_AddFormToTemplete( frmMission, FORM_MAIN )
UI_SetIsDrag( frmMission, TRUE )
--UI_SetFormStyle( frmMission, 0 )
--UI_SetAlpha( frmMission, 200 )

imgMissonT = UI_CreateCompent( frmMission, IMAGE_TYPE, "imgMissonT", 235, 256, 0, 0 )
UI_LoadImage( imgMissonT, "texture/ui/mission1.tga", NORMAL, 235, 256, 0, 0 )
imgMissonD = UI_CreateCompent( frmMission, IMAGE_TYPE, "imgMissonD", 235, 138, 0, 256 )
UI_LoadImage( imgMissonD, "texture/ui/mission2.tga", NORMAL, 235, 138, 0, 0 )

labTitle = UI_CreateCompent( frmMission, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Quest Log")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMisson = UI_CreateCompent( frmMission, LABELEX_TYPE, "labMisson", 47, 11, 78, 150 )
--UI_SetCaption( labMisson, "Quest Hint" )
UI_SetTextColor( labMisson, COLOR_BLACK )
UI_SetLabelExFont( labMisson, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmMission, BUTTON_TYPE, "btnClose", 14, 14, 216, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

trvMission = UI_CreateCompent( frmMission, TREE_TYPE, "trvMission", 223, 110, 4, 40 )

UI_SetIsDrag( trvEditor, TRUE )

UI_TreeLoadImage( trvMission, enumTreeAddImage, "texture/ui/SystemBotton3.tga", 16, 16, 205, 120, 12, 12 )
UI_TreeLoadImage( trvMission, enumTreeSubImage, "texture/ui/SystemBotton3.tga", 16, 16, 222, 120, 12, 12 )

scrollid = UI_GetScroll( trvMission )
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

memMiss  = UI_CreateCompent( frmMission, MEMOEX_TYPE, "memMiss", 215, 190, 17, 171 )
UI_SetCaption( memMiss, "400" )
UI_SetTextColor( memMiss, COLOR_BLACK )
UI_SetLabelExFont( memMiss, DEFAULT_FONT, TRUE, COLOR_WHITE )
UI_SetMemoPageShowNum( memMiss, 12 )

scrollid = UI_GetScroll( memMiss )
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

btnBreak = UI_CreateCompent( frmMission, BUTTON_TYPE, "btnBreak", 41, 19, 130, 368 )
UI_LoadButtonImage( btnBreak, "texture/ui/SystemBotton3.tga", 41, 19, 212, 169, FALSE )
UI_SetHint( btnBreak, "Abandon Quest" )
