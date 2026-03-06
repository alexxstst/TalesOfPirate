-----------------------------------------------------------------------
-- 
-----------------------------------------------------------------------	
-- 
frmStartHelp = UI_CreateForm( "frmStartHelp", FALSE, 285, 327, 198, 160, TRUE, FALSE )
UI_FormSetHotKey( frmStartHelp, ALT_KEY, HOTKEY_H ) 
UI_ShowForm( frmStartHelp, FALSE )
UI_AddFormToTemplete( frmStartHelp, FORM_MAIN )
UI_SetIsDrag( frmStartHelp, TRUE )

imgHelp1 = UI_CreateCompent( frmStartHelp, IMAGE_TYPE, "imgHelp1", 256, 256, 0, 0 )
UI_LoadImage( imgHelp1, "texture/ui/help1.tga", NORMAL, 256, 256, 0, 0 )

imgHelp2 = UI_CreateCompent( frmStartHelp, IMAGE_TYPE, "imgHelp2", 29, 256, 256, 0 )
UI_LoadImage( imgHelp2, "texture/ui/help2.tga", NORMAL, 29, 256, 0, 0 )

imgHelp3 = UI_CreateCompent( frmStartHelp, IMAGE_TYPE, "imgHelp3", 256, 61, 0, 256 )
UI_LoadImage( imgHelp3, "texture/ui/help4.tga", NORMAL, 256, 61, 0, 0 )

imgHelp4 = UI_CreateCompent( frmStartHelp, IMAGE_TYPE, "imgHelp4", 29, 61, 256, 256 )
UI_LoadImage( imgHelp4, "texture/ui/help2.tga", NORMAL, 29, 61, 29, 0 )

--
btnClose = UI_CreateCompent( frmStartHelp, BUTTON_TYPE, "btnClose", 14, 14, 268, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )



-- 
grdHelp = UI_CreateCompent( frmStartHelp, GRID_TYPE, "grdAction", 266, 276, 14, 27 ) 
UI_SetGridUnitSize( grdHelp, 256, 256 )
UI_SetIsShow( grdHelp, TRUE )
UI_SetMargin( grdHelp, 0, 0, 0, 0 )
UI_SetGridIsDragSize( grdHelp, FALSE )

UI_AddFaceToGrid( grdHelp,"texture/ui/help/1.bmp" , 256, 256, 0, 0, 1, 1 )
UI_AddFaceToGrid( grdHelp,"texture/ui/help/fast.bmp" , 256, 256, 0, 0, 1, 2 )
UI_AddFaceToGrid( grdHelp,"texture/ui/help/channel1.bmp" , 256, 256, 0, 0, 1, 3 )
UI_AddFaceToGrid( grdHelp,"texture/ui/help/channel2.bmp" , 256, 256, 0, 0, 1, 4 )
UI_AddFaceToGrid( grdHelp,"texture/ui/help/channel3.bmp" , 256, 256, 0, 0, 1, 5 )
UI_AddFaceToGrid( grdHelp,"texture/ui/help/channel4.bmp" , 256, 256, 0, 0, 1, 6 )
UI_AddFaceToGrid( grdHelp,"texture/ui/help/channel5.bmp" , 256, 256, 0, 0, 1, 7 )
UI_AddFaceToGrid( grdHelp,"texture/ui/help/turn.bmp" , 256, 256, 0, 0, 1, 8 )
UI_AddFaceToGrid( grdHelp,"texture/ui/help/chat.bmp" , 256, 256, 0, 0, 1, 9 )


--
btnOpenHelp = UI_CreateCompent( frmStartHelp, BUTTON_TYPE, "btnOpenHelp", 49, 13, 200, 290 )
UI_LoadButtonImage( btnOpenHelp, "texture/ui/SysBotton4.tga", 49, 13, 202, 86, FALSE )			      
