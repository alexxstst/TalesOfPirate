-- Create font; dwStyle is a combination of flags: 0x0001-bold, 0x0002-italic, 0x0004-underline, 0 for normal
-- Имя семейства приходит из C++ FontManager (секция [Fonts] SystemFont в system.ini).
-- При отсутствии глобала — fallback на Arial.
local systemFont = g_SystemFont or "Arial"

DEFAULT_FONT = UI_CreateFont( systemFont, 12, 12, 0 )
FONT14       = UI_CreateFont( systemFont, 13, 13, 0 )
FONT16       = UI_CreateFont( systemFont, 13, 13, 1 )
FONT20       = UI_CreateFont( systemFont, 20, 20, 1 )
FONT28       = UI_CreateFont( systemFont, 28, 28, 1 )
BIGFONT      = UI_CreateFont( systemFont, 48, 48, 0 )
ARIAL_FONT   = UI_CreateFont( "Arial",    12, 12, 0 )
TIPFONT      = DEFAULT_FONT
