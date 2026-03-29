-------------------------------------------------------------
-- Equipment
-------------------------------------------------------------
enumEQUIP_HEAD		= 0 	-- Equipment slots: head, face, body, gloves, shoes
enumEQUIP_FACE		= 1
enumEQUIP_BODY		= 2
enumEQUIP_GLOVE		= 3	-- Gloves
enumEQUIP_SHOES		= 4	-- Shoes

enumEQUIP_LHAND		= 6	-- Left hand	-- Weapon enum values are client Link IDs
enumEQUIP_RHAND		= 9	-- Right hand

enumEQUIP_NECK		= 5	-- Neck: necklace, pendant
enumEQUIP_HAND1		= 7	-- Ring slot 1
enumEQUIP_HAND2		= 8

enumEQUIP_Jewelry1	= 10
enumEQUIP_Jewelry2	= 11
enumEQUIP_Jewelry3	= 12
enumEQUIP_Jewelry4	= 13
enumEQUIP_WING		= 14

-- SN_CreateScene is registered directly via LuaBridge (C function)

-- Set the center point of the map to display

-- Set whether to show the 3D cursor

-- Set whether to show the minimap

-- Set the light color on the character when the background moves


-------------------------------------------------------------
-- Characters
-------------------------------------------------------------
-- CH_Create is registered directly via LuaBridge (C function)

-- Set character position

-- Set character angle 0-360 (north is 0)

-- Play character animation
PLAY_ONCE = 1
PLAY_LOOP = 2
PLAY_FRAME = 3
PLAY_ONCE_SMOOTH = 4
PLAY_LOOP_SMOOTH = 5
PLAY_PAUSE = 6
PLAY_CONTINUE = 7
PLAY_INVALID = 0

-------------------------------------------------------------
-- Camera
-------------------------------------------------------------
-- Set the game camera position: eye position and look-at target

-- Set whether to use the scene defined in the script

-- Switch to another scene, sceneid is the return value of SN_CreateScene

-- Scene types
enumLoginScene    =0
enumWorldScene    =1		-- Game scene
enumSelectChaScene = 2		-- Character selection scene
enumCreateChaScene = 3		-- Character creation scene

----------------------------------------------------------------
-- Global variables below
----------------------------------------------------------------
SN_SetAttackChaColor( 255, 180, 180 )
