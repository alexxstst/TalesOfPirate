NORMAL  = 0
HOVER   = 1
DOWN    = 2
DISABLE = 3
ALL     = 4

-----------------------------------------------------------------------
-- 
-----------------------------------------------------------------------

-- 
TRUE = 1
FALSE = 0

-- 
caLeft =1
caLeftUp =2
caUp = 3
caRightUp = 4
caRight = 5
caRightBottom = 6
caBottom = 7
caLeftBottom = 8
caClient = 9
caCenter  = 10          -- 
caWidthCenter = 11      -- 
caHeightCenter = 12     --  

-- 
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

-- 
COLOR_BLACK = 4278190080
COLOR_RED = 4294901760
COLOR_WHITE = 4294967295
COLOR_PURPLE = 4293990336		-- 
COLOR_YELLOW = 4294967040           -- 
COLOR_BLUE = 4278190335 --
COLOR_GREEN = 4278255360 --
COLOR_PINK = 4294902015  --

TREE_TEXT_COLOR = COLOR_WHITE


-- 
NORMAL  = 0
HOVER   = 1
DOWN    = 2
DISABLE = 3

-- 
COMPENT_BACK = 0		-- 

-- 
PROGRESS_PROGRESS = 1 		-- 

-- 
UNCHECKED = 0			-- 
CHECKED = 1			-- 

-- 
PROGRESS_HORIZONTAL = 0		-- ,
PROGRESS_VERTICAL = 1		-- 


-- 
-- 0 :; 12x 3 Y 4 567   8 9 10  11 
FORM_NONE=0			-- 
FORM_ALLCENTER=1		-- 
FORM_XCENTER=2			-- x
FORM_YCENTER=3			-- Y
FORM_LEFT=4			-- 
FORM_RIGHT=5			-- 
FORM_TOP=6			-- 
FORM_BOTTOM=7			-- 
FORM_LEFTTOP=8			-- 
FORM_RIGHTTOP=9			-- 
FORM_LEFTBOTTOM=10		-- 
FORM_RIGHTBOTTOM=11		-- 


-----------------------------------------------------------------------
-- 
-----------------------------------------------------------------------


ALT_KEY = 0
CTRL_KEY = 1
SHIFT_KEY = 2

-- ,-1
-- style
eSimpleTitle = 0		-- ,
eWindowTitle = 1		-- windows,
eNoTitle     = 2 		-- 


BUTTON_NONE  	= 0
BUTTON_CLOSE	= 1
BUTTON_YES	= 2
BUTTON_NO	= 3
BUTTON_OK	= 4
BUTTON_CANCLE	= 5

-- :up,down,scroll
SCROLL_UP = 0
SCROLL_DOWN = 1
SCROLL_SCROLL = 2

PROGRESS_HINT_NUM = 0
PROGRESS_HINT_PERCENT = 1

enumTreeAddImage = 0
enumTreeSubImage = 1
enumTreeNodeImage = 2

enumSPIRIT_ONE = 1
enumSPIRIT_TWO = 2
enumSPIRIT_OUT = 3

-- Page
PAGE_BUTTON_LEFT_UP=0		-- 
PAGE_BUTTON_FULL_UP=1		-- 
PAGE_BUTTON_CUSTOM =2		-- 


-- PageItem
PAGE_ITEM_IMAGE=0
PAGE_ITEM_TITLE=1	


-- PAGE_ITEM_TITLE
PAGE_ITEM_TITLE_NORMAL=0
PAGE_ITEM_TITLE_ACTIVE=1

-- frame
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


-- ASCII,
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

-- ,UITemplete.h
FORM_LOGIN = 0
FORM_MAIN = 1
FORM_SELECT_CHA = 2
FORM_EDITOR = 3
FORM_SWITCH_SCENE = 4
FORM_CREATE_CHA = 5
FORM_SELECT = 6
UI_SetFormTempleteMax( 7 )	-- 

FORM_DIALOG = FORM_SWITCH_SCENE	-- -4

-- 
UI_LoadScript("scripts/lua/forms/main.lua")
--UI_LoadScript("scripts/lua/forms/selectcha.clu")
UI_LoadScript("scripts/lua/forms/login.lua")
UI_LoadScript("scripts/lua/forms/loading.lua")
UI_LoadScript("scripts/lua/forms/dialog.lua")	-- 
UI_LoadScript("scripts/lua/forms/NPC.lua")
UI_LoadScript("scripts/lua/forms/Player.lua")
--UI_LoadScript("scripts/lua/textparse.clu")

UI_LoadScript("scripts/lua/forms/minimap.lua")	-- 
UI_LoadScript("scripts/lua/forms/help.lua")		-- 
UI_LoadScript("scripts/lua/forms/system.lua")	-- 
UI_LoadScript("scripts/lua/forms/equip.lua")	-- ,
UI_LoadScript("scripts/lua/forms/chat.lua")		-- 
UI_LoadScript("scripts/lua/forms/preperty.lua")	-- 
UI_LoadScript("scripts/lua/forms/mission.lua")	-- 
UI_LoadScript("scripts/lua/forms/coze.lua")		-- 
UI_LoadScript("scripts/lua/forms/ship.lua")		-- 
UI_LoadScript("scripts/lua/forms/traderoom.lua")	-- 
UI_LoadScript("scripts/lua/forms/manage.lua")	-- 
UI_LoadScript("scripts/lua/forms/select.lua")

--
UI_LoadScript("scripts/lua/filter.lua")
