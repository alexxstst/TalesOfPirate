-- 
nLoginScene = SN_CreateScene( enumLoginScene, "", "", FORM_LOGIN, 300, 200, 100, 100 )	
SN_SetIsShowMinimap( FALSE )
SN_SetIsShow3DCursor( FALSE )

-- UI
UI_ShowForm( nLoginScene, TRUE )

GP_GotoScene( nLoginScene )	-- GotoScene,
