
local _c_GetTerrainTextureID = GetTerrainTextureID
function GetTerrainTextureID(terrain_id)
	local tex_id = _c_GetTerrainTextureID(terrain_id) 	
	if tex_id==0 then 
	  return nil 
	end
	return tex_id
end

local _c_GetChaPhotoTexID = GetChaPhotoTexID
function GetChaPhotoTexID(cha_id)

	local tex = _c_GetChaPhotoTexID(cha_id)
	if tex==0 then 
 	    return nil
	end
	return tex
end

local _c_GetSceneObjPhotoTexID = GetSceneObjPhotoTexID
function GetSceneObjPhotoTexID(obj_id)

	local tex = _c_GetSceneObjPhotoTexID(obj_id)
	if tex==0 then 
 	    return nil
	end
	return tex
end

local _c_GetEffectPhotoTexID = GetEffectPhotoTexID
function GetEffectPhotoTexID(eff_id)

	local tex = _c_GetEffectPhotoTexID(eff_id)
	if tex==0 then 
 	    return nil
	end
	return tex
end

-- GetTerrainTextureType, GetSceneObjPhotoTexType: C functions via LuaBridge



NORMAL  = 0
HOVER   = 1
DOWN    = 2
DISABLE = 3
ALL     = 4


-----------------------------------------------------------------------
-- Macro definitions
-----------------------------------------------------------------------

-- Boolean values
TRUE = 1
FALSE = 0

-- Control display alignment
caLeft =1
caLeftUp =2
caUp = 3
caRightUp = 4
caRight = 5
caRightBottom = 6
caBottom = 7
caLeftBottom = 8
caClient = 9
caCenter  = 10          -- Fully centered
caWidthCenter = 11      -- Horizontally centered
caHeightCenter = 12     -- Vertically centered

-- Control types
LABEL_TYPE		= 0
LABELEX_TYPE		= 1
BUTTON_TYPE		= 2
COMBO_TYPE		= 3
EDIT_TYPE		= 4
IMAGE_TYPE		= 5
LIST_TYPE		= 6
PROGRESS_TYPE		= 7
CHECK_TYPE		= 8
CHECK_GROUP_TYPE 	= 9
GRID_TYPE		= 10
PAGE_TYPE		= 11
FIX_LIST_TYPE		= 12
CHECK_FIX_LIST_TYPE	= 13
DRAG_TITLE_TYPE		= 14
TREE_TYPE		= 15
IMAGE_FRAME_TYPE	= 16
UI3D_COMPENT_TYPE	= 17
MEMO_TYPE		= 18
MEMOEX_TYPE		= 19
GOODS_GRID_TYPE		= 20
FAST_COMMANG_TYPE	= 21
COMMAND_ONE_TYPE	= 22
IMAGE_FLASH_TYPE	= 23
SCROLL_TYPE		= 24
SKILL_LIST_TYPE		= 25
LISTEX_TYPE		= 26
MENU_TYPE		= 27
RICHMEMO_TYPE		= 28
TITLE_TYPE 		= 29
RICHEDIT_TYPE 		= 30 

UI3D_COMPENT = UI3D_COMPENT_TYPE

-- Colors
COLOR_BLACK = 4278190080
COLOR_RED = 4294901760
COLOR_WHITE = 4294967295
COLOR_PURPLE = 4293990336		-- Purple
COLOR_YELLOW = 4294967040           -- Yellow
COLOR_BLUE = 4278190335 -- Blue
COLOR_GREEN = 4278255360 -- Green
COLOR_PINK = 4294902015  -- Pink
COLOR_RED_SP = COLOR_RED

TREE_TEXT_COLOR = COLOR_WHITE


-- Four button states: normal, hover, down, disabled
NORMAL  = 0
HOVER   = 1
DOWN    = 2
DISABLE = 3

-- Background image
COMPENT_BACK = 0		-- Control background

-- Progress bar images
PROGRESS_PROGRESS = 1 		-- Progress bar

-- Checkbox images
UNCHECKED = 0			-- Unchecked state
CHECKED = 1			-- Checked state

-- Progress bar display style
PROGRESS_HORIZONTAL = 0		-- Horizontal, left to right
PROGRESS_VERTICAL = 1		-- Vertical, bottom to top


-- Form style
-- 0: None; 1: All centered; 2: X centered; 3: Y centered; 4: Left; 5: Right; 6: Top; 7: Bottom; 8: Top-left; 9: Top-right; 10: Bottom-left; 11: Bottom-right
FORM_NONE=0			-- None
FORM_ALLCENTER=1		-- All centered
FORM_XCENTER=2			-- X centered
FORM_YCENTER=3			-- Y centered
FORM_LEFT=4			-- Left
FORM_RIGHT=5			-- Right
FORM_TOP=6			-- Top
FORM_BOTTOM=7			-- Bottom
FORM_LEFTTOP=8			-- Top-left
FORM_RIGHTTOP=9			-- Top-right
FORM_LEFTBOTTOM=10		-- Bottom-left
FORM_RIGHTBOTTOM=11		-- Bottom-right


-----------------------------------------------------------------------
-- Function definitions
-----------------------------------------------------------------------
-- Unless otherwise noted, return value is 1 on success, -1 on failure

-- Load UI script

-- Create form. Params: name, isModal, width, height, X, Y, isTiled, isShowFrame
-- Returns: -1 on failure, form ID on success

-- Set whether form can be closed with ESC

-- Set which button event to trigger when Enter is pressed

ALT_KEY = 0
CTRL_KEY = 1
SHIFT_KEY = 2
-- Set hotkey to show/hide Form

-- Set max Form template count, must be greater than zero

-- Add Form to template nTempleteNo (must be >= 0 and < max)

-- Add Form to every template

-- Switch current Form template; if nTempleteNo < 0, switch to default template

-- Load form image. client=tile file, cw/ch=tile size, bw/bh=border size (pixels)

-- Set form alignment type (centered, bottom, etc.)
-- 0: None; 1: All centered; 2: X centered; 3: Y centered; 4: Left; 5: Right; 6: Top; 7: Bottom; 8: Top-left; 9: Top-right; 10: Bottom-left; 11: Bottom-right

-- Set form alignment type (centered, bottom, etc.)
-- 0: None; 1: All centered; 2: X centered; 3: Y centered; 4: Left; 5: Right; 6: Top; 7: Bottom; 8: Top-left; 9: Top-right; 10: Bottom-left; 11: Bottom-right
--offWidth: horizontal offset from edge
--offHeight: vertical offset from edge

-- Load FrameImage. client=tile file, cw/ch=tile size, bw/bh=border size (pixels)

-- Show or hide form: form ID, isVisible

-- Create control: parent form, type, name, width/height, X, Y
-- Types: 1-Button, 2-Combo, 3-Edit, 4-Image, 5-Label, 6-List
-- Returns: -1 on failure, control ID on success

-- Set whether control (including form) is draggable

-- Set control tooltip

-- Create multi-column list view, returns -1 on failure
-- style is the header style
eSimpleTitle = 0		-- Simple header, just one image
eWindowTitle = 1		-- Windows-style header, requires loading each header image
eNoTitle     = 2 		-- No title bar

-- Set listview header. Params: id, header index, header width, header image, image width/height/start coords

-- Set listview header height

-- Set whether selection bar follows mouse

-- Set control tag parameter (integer)

-- Set control size

-- Set control position

-- Set whether control accepts keyboard focus

-- Set control display text

-- Copy image from another control
-- Returns: 1 on success, -1 on failure

-- Set control alpha (255=opaque, 0=transparent)

-- Set control background alpha (255=opaque, 0=transparent)

-- Set control display alignment

-- Set control visibility

-- Set control enabled state

-- Set control margin on all sides

-- Set chat colors: World, Stranger, Party, Guild, GM, System, Trade, Whisper

-- Load control image. Params: control id, image name, frame index, width, height, start X, Y

-- Set control background image frame count

-- Load control image (fixed aspect ratio). Params: control id, image name, frame index, width, height, start X, Y


-- Load control image (fixed aspect ratio). Params: control id, image name, frame index, width, height, start X, Y

-- Load SkillList upgrade button image

-- Load button image

BUTTON_NONE  	= 0
BUTTON_CLOSE	= 1
BUTTON_YES	= 2
BUTTON_NO	= 3
BUTTON_OK	= 4
BUTTON_CANCLE	= 5

-- Set button return value


-- Set button hint

-- Get scroll bar from combo or list, for loading scroll images and setting size

-- Get list from combo or CListView

-- Load grid selection bar

-- Set whether grid can be resized by dragging bottom-left corner

-- Get scroll bar objects: up button, down button, scroll image
SCROLL_UP = 0
SCROLL_DOWN = 1
SCROLL_SCROLL = 2

-- Load combo image. edit=input bg file, w/h=size, ex/ey=image pos; button=dropdown bg file, bw/bh=btn size, bx/by=image pos, isHorizontal=horizontal layout

-- Set combo list direction: up or down

-- Set combo text color

-- Load FixList checkbox image

-- Copy all states from another control (fails if different control types)

-- Add text to list

-- Load List selection bar background image

-- Load List item background image

-- Set margin for each List item

-- Set background image margin for each List item

-- Add text to list (with progress bar), progress param 0.0~1.0

-- Set List text background color and selection color


-- Set List row height

-- Add checkbox to group

	
-- Set progress bar display style

PROGRESS_HINT_NUM = 0
PROGRESS_HINT_PERCENT = 1
-- Set progress bar hint display style



-- Set scroll bar display style

-- Set edit box max input length (bytes)

-- Set edit box cursor color

-- Set button triggered by Enter in edit box

-- Set edit box max input length (bytes)

-- Set control font color

-- Set grid cell spacing (default: 2)

-- Set grid capacity (params must be non-zero)

-- Set GoodGrid cell background

-- Set grid cell width/height (default: 40)

-- Add emoticon to Grid, frame = animation frame count

-- Set max line count

-- Set number of characters displayed per line

-- Set Memo lines per page

-- Set Memo line height

-- Set Rich clipping area

-- Set Rich max line capacity

-- Set text for each row

-- Set line spacing

-- Set CheckFixList checkbox display margin


enumTreeAddImage = 0
enumTreeSubImage = 1
enumTreeNodeImage = 2
-- Load tree control image. nType=which image, recommended size 16x16, display size itemw/itemh



-- Create a text Item for inserting into a treenode

-- Create a graphic Item for inserting into a treenode

-- Create a background Item for inserting into a treenode

-- Create a single treenode. Params: treeid, created item, parent node id (-1 = insert at root)

-- Create a grid treenode. Params: treeid, columns per row, cell width/height, parent node id (-1 = insert at root)

-- Create a Graph


-- Add itemid to nodeid

-- Create a PageItem object within a Page

-- Set Page tab button layout and cell width/height
PAGE_BUTTON_LEFT_UP=0		-- Sequential, top-left
PAGE_BUTTON_FULL_UP=1		-- Fill top
PAGE_BUTTON_CUSTOM =2		-- Custom

-- Get object from PageItem
PAGE_ITEM_IMAGE=0
PAGE_ITEM_TITLE=1	

-- Two states when loading PAGE_ITEM_TITLE images
PAGE_ITEM_TITLE_NORMAL=0
PAGE_ITEM_TITLE_ACTIVE=1

-- Add an object to a container

-- Set extended Label shadow background color

-- Set drag snap-to-grid cell width/height

-- Map text to image. nIndex=index, followed by image

-- Load progress bar image for text with progress

-- Load menu background image

-- Load menu selection bar image

-- Add menu item

-- Add username filter characters

-- Add chat content filter characters

-- Change overhead chat bubble background color

-- Set Title font and color

-- Set extended Label font and shadow visibility

-- Load skill activation border image

-- Load shell charging image

-- Load emoticons into text input, frame = animation frame count
--UI_SetTextParse( 0 , "texture/ui/face/em001.tga", 40, 40 , 0 , 0 ,4  )
--UI_SetTextParse( 1 , "texture/ui/face/em003.tga", 40, 40 , 0 , 0 ,4  )
--UI_SetTextParse( 2 , "texture/ui/face/em001.tga", 40, 40 , 0 , 0 ,4  )
--UI_SetTextParse( 3 , "texture/ui/face/em003.tga", 40, 40 , 0 , 0 ,4  )
--UI_SetTextParse( 4 , "texture/ui/face/em001.tga", 40, 40 , 0 , 0 ,4  )
--UI_SetTextParse( 5 , "texture/ui/face/em003.tga", 40, 40 , 0 , 0 ,4  )
--UI_SetTextParse( 6 , "texture/ui/face/em001.tga", 40, 40 , 0 , 0 ,4  )
--UI_SetTextParse( 7 , "texture/ui/face/em003.tga", 40, 40 , 0 , 0 ,4  )
--UI_SetTextParse( 8 , "texture/ui/face/em001.tga", 40, 40 , 0 , 0 ,4  )
--UI_SetTextParse( 9 , "texture/ui/face/em003.tga", 40, 40 , 0 , 0 ,4  )

UI_ItemBarLoadImage( "texture/ui/system/progress.tga", 64, 16, 0, 0 )


-- ASCII lookup table for hotkeys
HOTKEY_A  = 65
HOTKEY_B  = 66
HOTKEY_C  = 67
HOTKEY_D  = 68
HOTKEY_E  = 69
HOTKEY_F  = 70
HOTKEY_G  = 71
HOTKEY_H  = 72
HOTKEY_I  = 73
HOTKEY_J  = 74
HOTKEY_K  = 75
HOTKEY_L  = 76
HOTKEY_M  = 77
HOTKEY_N  = 78
HOTKEY_O  = 79
HOTKEY_P  = 80
HOTKEY_Q  = 81
HOTKEY_R  = 82
HOTKEY_S  = 83
HOTKEY_T  = 84
HOTKEY_U  = 85
HOTKEY_V  = 86
HOTKEY_W  = 87
HOTKEY_X  = 88
HOTKEY_Y  = 89
HOTKEY_Z  = 90

UI_SetDragSnapToGrid( 4, 4 )

-- Declare all form templates for template switching, corresponds to UITemplete.h
FORM_LOGIN = 0
FORM_MAIN = 1
FORM_SELECT_CHA = 2
FORM_EDITOR = 3
FORM_SWITCH_SCENE = 4
FORM_CREATE_CHA = 5
FORM_SELECT = 6
UI_SetFormTempleteMax( 7 )	-- Set max template count

FORM_DIALOG = FORM_SWITCH_SCENE	-- Dialog template - 4

-- Load related UI scripts
UI_LoadScript("scripts/lua/forms/main.lua")
UI_LoadScript("scripts/lua/forms/selectcha.lua")
UI_LoadScript("scripts/lua/forms/login.lua")
UI_LoadScript("scripts/lua/forms/loading.lua")
UI_LoadScript("scripts/lua/forms/NPC.lua")
UI_LoadScript("scripts/lua/forms/Player.lua")
--UI_LoadScript("scripts/lua/textparse.clu")

UI_LoadScript("scripts/lua/forms/minimap.lua")	-- Minimap
UI_LoadScript("scripts/lua/forms/help.lua")		-- Help
UI_LoadScript("scripts/lua/forms/system.lua")	-- System
UI_LoadScript("scripts/lua/forms/equip.lua")	-- Skills, Equipment
UI_LoadScript("scripts/lua/forms/chat.lua")		-- Chat
UI_LoadScript("scripts/lua/forms/preperty.lua")	-- Character properties
UI_LoadScript("scripts/lua/forms/mission.lua")	-- Quests
UI_LoadScript("scripts/lua/forms/coze.lua")		-- Bottom-left chat
UI_LoadScript("scripts/lua/forms/dialog.lua")	-- Common dialogs
UI_LoadScript("scripts/lua/forms/ship.lua")		-- Shipbuilding
UI_LoadScript("scripts/lua/forms/traderoom.lua")	-- Trading post
UI_LoadScript("scripts/lua/forms/manage.lua")	-- Guild management
UI_LoadScript("scripts/lua/forms/select.lua")

-- Character filter table
UI_LoadScript("scripts/lua/filter.lua")
