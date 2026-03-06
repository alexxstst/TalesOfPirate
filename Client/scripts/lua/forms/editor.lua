-----------------------------------------------------------------------
-- Editor
-----------------------------------------------------------------------	
frmEditor = UI_CreateForm( "frmEditor", FALSE, 192, 768, 0, 0, FALSE, FALSE )
-- UI_FormSetHotKey( frmEditor, ALT_KEY, 69 ) --'E'
-- UI_LoadFormImage( frmEditor, "texture/ui/title2.tga", 128, 128, 0, 0, "texture/ui/board2.tga", 16, 16 )
-- UI_AddAllFormTemplete( frmEditor )
UI_ShowForm( frmEditor, TRUE )
UI_AddFormToTemplete( frmEditor, FORM_EDITOR )

imgTreeFrame = UI_CreateCompent( frmEditor, IMAGE_FRAME_TYPE, "imgTreeFrame", 192, 768, 0, 0 )
UI_LoadFrameImage( imgTreeFrame, "texture/ui/editor/title2.tga", 192, 192, 0, 0, "texture/ui/editor/board2.tga", 16, 16 )

imgEditor = UI_CreateCompent( frmEditor, IMAGE_TYPE, "imgEditor", 285, 35, 195, 0 )
UI_LoadImage( imgEditor,  "texture/ui/editor/bkg.tga", NORMAL, 1, 1, 0, 0 )
UI_SetAlpha( imgEditor, 100 )

chkSize01 = UI_CreateCompent( frmEditor, CHECK_TYPE, "chkSize01", 20, 20, 200, 5 )
UI_LoadImage( chkSize01, "texture/ui/editor/brushsize.tga", CHECKED, 20, 20, 0, 0 )
UI_LoadImage( chkSize01, "texture/ui/editor/brushsize.tga", UNCHECKED, 20, 20, 0, 20 )

chkSize02 = UI_CreateCompent( frmEditor, CHECK_TYPE, "chkSize02", 20, 20, 220, 5 )
UI_LoadImage( chkSize02, "texture/ui/editor/brushsize.tga", CHECKED, 20, 20, 20, 0 )
UI_LoadImage( chkSize02, "texture/ui/editor/brushsize.tga", UNCHECKED, 20, 20, 20, 20 )

chkSize03 = UI_CreateCompent( frmEditor, CHECK_TYPE, "chkSize03", 20, 20, 240, 5 )
UI_LoadImage( chkSize03, "texture/ui/editor/brushsize.tga", CHECKED, 20, 20, 40, 0 )
UI_LoadImage( chkSize03, "texture/ui/editor/brushsize.tga", UNCHECKED, 20, 20, 40, 20 )

chgGroup = UI_CreateCompent( frmEditor, CHECK_GROUP_TYPE, "chgGroup", 0, 0, 0, 0 )
UI_AddGroupBox( chgGroup, chkSize01 )
UI_AddGroupBox( chgGroup, chkSize02 )
UI_AddGroupBox( chgGroup, chkSize03 )

chkWireFrame = UI_CreateCompent( frmEditor, CHECK_TYPE, "chkWireFrame", 20, 20, 270, 5 )
UI_LoadImage( chkWireFrame, "texture/ui/editor/wireframe.tga", CHECKED, 20, 20, 0, 0 )
UI_LoadImage( chkWireFrame, "texture/ui/editor/wireframe.tga", UNCHECKED, 20, 20, 0, 20 )

chkModifyHeight = UI_CreateCompent( frmEditor, CHECK_TYPE, "chkModifyHeight", 20, 20, 290, 5 )
UI_LoadImage( chkModifyHeight, "texture/ui/editor/modifyheight.tga", CHECKED, 20, 20, 0, 0 )
UI_LoadImage( chkModifyHeight, "texture/ui/editor/modifyheight.tga", UNCHECKED, 20, 20, 0, 20 )

chkEnabledAlpha = UI_CreateCompent( frmEditor, CHECK_TYPE, "chkEnabledAlpha", 20, 20, 310, 5 )
UI_LoadImage( chkEnabledAlpha, "texture/ui/editor/texalpha.tga", CHECKED, 20, 20, 0, 0 )
UI_LoadImage( chkEnabledAlpha, "texture/ui/editor/texalpha.tga", UNCHECKED, 20, 20, 0, 20 )

chkSmooth = UI_CreateCompent( frmEditor, CHECK_TYPE, "chkSmooth", 20, 20, 330, 5 )
UI_LoadImage( chkSmooth, "texture/ui/editor/smooth.tga", CHECKED, 20, 20, 0, 0 )
UI_LoadImage( chkSmooth, "texture/ui/editor/smooth.tga", UNCHECKED, 20, 20, 0, 20 )

chkHideTree = UI_CreateCompent( frmEditor, CHECK_TYPE, "chkHideTree", 20, 20, 350, 5 )
UI_LoadImage( chkHideTree, "texture/ui/editor/hidetree.tga", CHECKED, 20, 20, 0, 0 )
UI_LoadImage( chkHideTree, "texture/ui/editor/hidetree.tga", UNCHECKED, 20, 20, 0, 20 )

local start = 380

btnColor = UI_CreateCompent( frmEditor, BUTTON_TYPE, "btnColor", 14, 20, start, 5 )
UI_LoadImage( btnColor, "texture/ui/white.tga", NORMAL, 1, 1, 0, 0  )
UI_LoadImage( btnColor, "texture/ui/white.tga", HOVER, 1, 1, 0, 0 )
UI_LoadImage( btnColor, "texture/ui/white.tga", DOWN, 1, 1, 0, 0 )
UI_LoadImage( btnColor, "texture/ui/white.tga", DISABLE, 1, 1, 0, 0 )

start = start + 15
chkColor = UI_CreateCompent( frmEditor, CHECK_TYPE, "chkColor", 20, 20, start, 5 )
UI_LoadImage( chkColor, "texture/ui/editor/color.tga", CHECKED, 20, 20, 0, 0 )
UI_LoadImage( chkColor, "texture/ui/editor/color.tga", UNCHECKED, 20, 20, 0, 20 )

start = start + 20
chkShowLightBrush = UI_CreateCompent( frmEditor, CHECK_TYPE, "chkShowLightBrush", 20, 20,start, 5 )
UI_LoadImage( chkShowLightBrush, "texture/ui/editor/lightcolor.tga", CHECKED, 20, 20, 0, 0 )
UI_LoadImage( chkShowLightBrush, "texture/ui/editor/lightcolor.tga", UNCHECKED, 20, 20, 0, 20 )


start = start + 25
ckhShowCompass =  UI_CreateCompent( frmEditor, CHECK_TYPE, "ckhShowCompass", 20, 20, start, 5 )
UI_LoadImage( ckhShowCompass, "texture/ui/editor/compass.tga", CHECKED, 20, 20, 0, 0 )
UI_LoadImage( ckhShowCompass, "texture/ui/editor/compass.tga", UNCHECKED, 20, 20, 0, 20 )

start = start + 18
-- ,
ckhHideOptionEx = UI_CreateCompent( frmEditor, CHECK_TYPE, "ckhHideOptionEx", 20, 20, start, 5 )
UI_LoadImage( ckhHideOptionEx, "texture/ui/editor/hideoption.tga", CHECKED, 32, 32, 0, 0 )
UI_LoadImage( ckhHideOptionEx, "texture/ui/editor/hideoption.tga", UNCHECKED, 32, 32, 0, 32 )

imgEditorEx = UI_CreateCompent( frmEditor, IMAGE_TYPE, "imgEditorEx", 245, 65, 570, 0 )
UI_LoadImage( imgEditorEx,  "texture/ui/editor/bkg.tga", NORMAL, 30, 16, 0, 0 )
UI_SetAlpha( imgEditorEx, 80 )

-- 
lblSetMainChaPos = UI_CreateCompent( frmEditor, LABEL_TYPE, "lblSetMainChaPos", 40, 30, 575, 0 )
UI_SetCaption( lblSetMainChaPos, "Instant\nMovement" )
UI_SetTextColor( lblSetMainChaPos, COLOR_WHITE )

edtSetMainChaPos = UI_CreateCompent( frmEditor, EDIT_TYPE, "edtSetMainChaPos", 100, 30, 610, 0 )
UI_LoadImage( edtSetMainChaPos, "texture/ui/editor/edit.tga", NORMAL, 128, 32, 0, 0 )

lblSetBrushHeight = UI_CreateCompent( frmEditor, LABEL_TYPE, "lblSetBrushHeight", 40, 30, 715, 0 )
UI_SetCaption( lblSetBrushHeight, "Brush\nAltitude" )
UI_SetTextColor( lblSetBrushHeight, COLOR_WHITE )

edtSetBrushHeight = UI_CreateCompent( frmEditor, EDIT_TYPE, "edtSetBrushHeight", 50, 30, 755, 0 )
UI_LoadImage( edtSetBrushHeight, "texture/ui/editor/edit.tga", NORMAL, 128, 32, 0, 0 )

-- 
lblShotMiniMap = UI_CreateCompent( frmEditor, LABEL_TYPE, "lblShotMiniMap", 40, 30, 575, 32 )
UI_SetCaption( lblShotMiniMap, "Mini-map\nScreenshot" )
UI_SetTextColor( lblShotMiniMap, COLOR_WHITE )

edtShotMiniMap = UI_CreateCompent( frmEditor, EDIT_TYPE, "edtShotMiniMap", 250, 30, 620, 32 )
UI_LoadImage( edtShotMiniMap, "texture/ui/editor/edit.tga", NORMAL, 128, 32, 0, 0 )


--  add by claude for entering island index
cld_xpos = 200
cld_ypos = 50
lblSetIslandIndex = UI_CreateCompent( frmEditor, LABEL_TYPE, "lblSetIslandIndex", 40, 30, cld_xpos, cld_ypos)
UI_SetCaption( lblSetIslandIndex, "Island\n ID" )
UI_SetTextColor( lblSetIslandIndex, COLOR_WHITE )
cld_xpos = cld_xpos + 50
edtSetIslandIndex = UI_CreateCompent( frmEditor, EDIT_TYPE, "edtSetIslandIndex", 50, 30, cld_xpos, cld_ypos)
UI_LoadImage( edtSetIslandIndex, "texture/ui/editor/edit.tga", NORMAL, 128, 32, 0, 0)
-- 
cld_xpos = cld_xpos + 50
lblEraseAttrib = UI_CreateCompent(frmEditor, LABEL_TYPE, "lblEraseAttrib", 40, 30, cld_xpos, cld_ypos)
UI_SetCaption(lblEraseAttrib, "Reset \n Attribute")
UI_SetTextColor(lblEraseAttrib, COLOR_WHITE)
cld_xpos = cld_xpos + 40
chkEraseAttrib =  UI_CreateCompent( frmEditor, CHECK_TYPE, "chkEraseAttrib", 20, 20, cld_xpos, cld_ypos + 5)
UI_LoadImage( chkEraseAttrib, "texture/ui/editor/lightcolor.tga", CHECKED, 20, 20, 0, 0 )
UI_LoadImage( chkEraseAttrib, "texture/ui/editor/lightcolor.tga", UNCHECKED, 20, 20, 0, 20 )
-- add by claude end


trvEditor = UI_CreateCompent( frmEditor, TREE_TYPE, "trvEditor", 192 - 5, 768 - 9, 5, 5 )
-- UI_LoadImage( trvEditor,  "texture/ui/editor/frame.tga", NORMAL, 1, 1, 0, 0 )
scrollid = UI_GetScroll( trvEditor )
UI_SetSize( scrollid, 15, 15 )
-- UI_LoadImage( scrollid, "texture/ui/editor/chat.tga", COMPENT_BACK, 9, 9, 0, 51 )
scrollid = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( scrollid, "texture/ui/editor/scroll.tga", COMPENT_BACK, 16, 32, 0, 0 )
UI_SetSize( scrollid, 10, 32 )
-- UI_SetAlpha( trvEditor, 100 )

itemid = UI_CreateTextItem( "earth surface", TREE_TEXT_COLOR )
sndNode = UI_CreateSingleNode( trvEditor, itemid, -1 )
	itemid = UI_CreateTextItem( "Style 1", TREE_TEXT_COLOR )
	gridid1 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
	
	itemid = UI_CreateTextItem( "Style 2", TREE_TEXT_COLOR )
	gridid2 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
	
	itemid = UI_CreateTextItem( "Style 3", TREE_TEXT_COLOR )
	gridid3 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
		
	itemid = UI_CreateTextItem( "Public", TREE_TEXT_COLOR )
	gridid100 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
	
	local n, type
	n  = 1		
	while 1 do			
	    local terrain_tex = GetTerrainTextureID(n)
	    if terrain_tex==nil then break end
	    
		graphid = UI_CreateGraphItemTex( 0, 0, 32, 32, 1.0, 1.0, terrain_tex, n )
	    	
	    	type = GetTerrainTextureType(n)
	    	if type==1 then
		    	UI_GridNodeAddItem( gridid1, graphid )		    	
		end
		if type==2 then 
		    	UI_GridNodeAddItem( gridid2, graphid )		    	
		end
		if type==3 then 
		    	UI_GridNodeAddItem( gridid3, graphid )		    	
		end
		if type==100 then 
		    	UI_GridNodeAddItem( gridid100, graphid )		    	
		end
		
	    	n = n + 1
	end	   	
	
itemid = UI_CreateTextItem( "Building", TREE_TEXT_COLOR )
sndNode = UI_CreateSingleNode( trvEditor, itemid, -1 )

	itemid = UI_CreateTextItem( "Argent City", TREE_TEXT_COLOR )
	gridid1 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
	
	itemid = UI_CreateTextItem( "Thundoria Castle", TREE_TEXT_COLOR )
	gridid2 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
			
	itemid = UI_CreateTextItem( "Shaitan City", TREE_TEXT_COLOR )
	gridid3 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
				
	itemid = UI_CreateTextItem( "Icicle Castle", TREE_TEXT_COLOR )
	gridid4 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
				
	itemid = UI_CreateTextItem( "Style 5", TREE_TEXT_COLOR )
	gridid5 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
				
	itemid = UI_CreateTextItem( "Style 6", TREE_TEXT_COLOR )
	gridid6 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
				
	itemid = UI_CreateTextItem( "Maze", TREE_TEXT_COLOR )
	gridid7 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
	
	itemid = UI_CreateTextItem( "Tree", TREE_TEXT_COLOR )
	gridid8 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
	
	itemid = UI_CreateTextItem( "Others", TREE_TEXT_COLOR )
	gridid9 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
		
	itemid = UI_CreateTextItem( "Public", TREE_TEXT_COLOR )
	gridid100 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )
	
	itemid = UI_CreateTextItem( "dummy item", TREE_TEXT_COLOR )
	gridid200 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, sndNode )

		n  = 1			
		while 1 do
			local tex_id = GetSceneObjPhotoTexID(n)
		    	if tex_id~=nil then 
		    	
				graphid = UI_CreateGraphItemTex( 0, 0, 32, 32, 1.0, 1.0, tex_id, n  )

			    	type = GetSceneObjPhotoTexType(n)
					-- 
			    	if type == 1 then
				    	UI_GridNodeAddItem( gridid1, graphid )
			    	end 
			    	if type == 2 then
				    	UI_GridNodeAddItem( gridid2, graphid )
			    	end 
			    	if type == 3 then
				    	UI_GridNodeAddItem( gridid3, graphid )
			    	end 
			    	if type == 4 then
				    	UI_GridNodeAddItem( gridid4, graphid )
			    	end 
			    	if type == 5 then
				    	UI_GridNodeAddItem( gridid5, graphid )
			    	end 
			    	if type == 6 then
				    	UI_GridNodeAddItem( gridid6, graphid )
			    	end 
			    	if type == 7 then
				    	UI_GridNodeAddItem( gridid7, graphid )
			    	end 
			    	if type == 8 then
				    	UI_GridNodeAddItem( gridid8, graphid )
			    	end 
			    	if type == 9 then
				    	UI_GridNodeAddItem( gridid9, graphid )
			    	end 
			    	if type == 100 then
				    	UI_GridNodeAddItem( gridid100, graphid )
			    	end 
			    	if type == 200 then
				    	UI_GridNodeAddItem( gridid200, graphid )
			    	end 			    	
		    			        				
		        end
			n = n + 1
		        if n==550 then break end
		end	

			
	itemid = UI_CreateTextItem( "Character", TREE_TEXT_COLOR )
	gridid1 = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, -1 )
	n  = 1	
	local valid_cnt = 0 --
	while 1 do
	    local tex_id = GetChaPhotoTexID(n)
	    
	    if tex_id~=nil then 
		graphid = UI_CreateGraphItemTex( 0, 0, 32, 32, 1.0, 1.0, tex_id, n  )
	    	UI_GridNodeAddItem( gridid1, graphid )		    			
	        valid_cnt = valid_cnt + 1
	    end
	    n = n + 1
	    if n==1000 then break end
	end

itemid = UI_CreateTextItem( "Effect", TREE_TEXT_COLOR )
gridid = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, -1 )
	n = 0
	while 1 do
	    local tex_id = GetEffectPhotoTexID(n)
	    if tex_id~=nil then 
		    graphid = UI_CreateGraphItemTex( 0, 0, 30, 30, 1.0, 1.0, tex_id, n  )
		    UI_GridNodeAddItem( gridid, graphid )
	    end
	    n = n + 1
            if n==1000 then break end
	end

itemid = UI_CreateTextItem( "Region attribute", TREE_TEXT_COLOR )
gridid = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, -1 )
	n = 1
	while 1 do
	    name = "texture/photo/.bmp"
	    tex_id = GetTextureID( name )
	    if tex_id then
		    graphid = UI_CreateGraphItemTex( 0, 0, 30, 30, 1.0, 1.0, tex_id, n  )
		    UI_GridNodeAddItem( gridid, graphid )
	    end
	    n = n + 1
            if n==17 then break end
	end

itemid = UI_CreateTextItem( "Island ID", TREE_TEXT_COLOR )
gridid = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, -1 )
	n = 1
	while 1 do
	    name = "texture/photo/.bmp"
	    tex_id = GetTextureID( name )
	    if tex_id then
		    graphid = UI_CreateGraphItemTex( 0, 0, 30, 30, 1.0, 1.0, tex_id, n  )
		    UI_GridNodeAddItem( gridid, graphid )
	    end
	    n = n + 1
            if n==2 then break end
	end

itemid = UI_CreateTextItem( "Map List", TREE_TEXT_COLOR )
gridid = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, -1 )
	n = 1
	while 1 do
	    name = "texture/photo/sceneobj/map"..n..".tga"
	    tex_id = GetTextureID( name )
	    if tex_id then
		    graphid = UI_CreateGraphItemTex( 0, 0, 30, 30, 1.0, 1.0, tex_id, n  )
		    UI_GridNodeAddItem( gridid, graphid )
	    end
	    n = n + 1
            if n==30 then break end
	end

itemid = UI_CreateTextItem( "Character Action", TREE_TEXT_COLOR )
gridid = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, -1 )
	n = 1
	while 1 do
--	    name = "texture/photo/pose"..n..".tga"
--	    tex_id = GetTextureID( name )
	    if tex_id then
		    graphid = UI_CreateGraphItemTex( 0, 0, 30, 30, 1.0, 1.0, tex_id, n  )
		    UI_GridNodeAddItem( gridid, graphid )
	    end
	    n = n + 1
            if n==55 then break end
	end

itemid = UI_CreateTextItem( "Character Image", TREE_TEXT_COLOR )
gridid = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, -1 )
	n = 0
	while 1 do
	    name = "texture/photo/part"..(n+1)..".bmp"
	    tex_id = GetTextureID( name )
	    if tex_id then
		    graphid = UI_CreateGraphItemTex( 0, 0, 30, 30, 1.0, 1.0, tex_id, n  )
		    UI_GridNodeAddItem( gridid, graphid )
	    end
	    n = n + 1
            if n==5 then break end
	end

itemid = UI_CreateTextItem( "Mouse Shape", TREE_TEXT_COLOR )
gridid = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, -1 )
	n = 1
	while 1 do
--	    name = "texture/photo/mouse"..n..".tga"
--	    tex_id = GetTextureID( name )
	    if tex_id then
		    graphid = UI_CreateGraphItemTex( 0, 0, 30, 30, 1.0, 1.0, tex_id, n  )
		    UI_GridNodeAddItem( gridid, graphid )
	    end
	    n = n + 1
            if n==21 then break end
	end

itemid = UI_CreateTextItem( "Character Effect", TREE_TEXT_COLOR )
gridid = UI_CreateGridNode( trvEditor, itemid, 4, 32, 32, -1 )
	n = 0
	while 1 do
--	    name = "texture/photo/mouse"..n..".tga"
--	    tex_id = GetTextureID( name )
	    if tex_id then
		    graphid = UI_CreateGraphItemTex( 0, 0, 30, 30, 1.0, 1.0, tex_id, n  )
		    UI_GridNodeAddItem( gridid, graphid )
	    end
	    n = n + 1
            if n==2 then break end
	end
