-- Create character creation scene
nCreateScene = SN_CreateScene( enumCreateChaScene, "��������", "", FORM_CREATE_CHA, 300, 200, 100, 100 )

SN_SetIsShowMinimap( FALSE )
SN_SetIsShow3DCursor( FALSE )

-- Show UI


GP_GotoScene( nCreateScene )	-- GotoScene must be last, because switching modes changes variable initialization
