-- Create login scene
nLoginScene = SN_CreateScene( enumLoginScene, "��½����", "", FORM_LOGIN, 300, 200, 100, 100 )
SN_SetIsShowMinimap( FALSE )
SN_SetIsShow3DCursor( FALSE )

-- Show UI
UI_ShowForm( frmLOGO, TRUE )

GP_GotoScene( nLoginScene )	-- GotoScene must be last, because switching modes changes variable initialization