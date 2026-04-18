-- console_bootstrap.lua — применение настроек [Console Debug] из system.ini.
--
-- Зависимости (должны быть уже загружены на момент вызова):
--   * SystemIni — глобал типа IniFile (LuaBridge), см. ConsoleBridge::_RegisterLuaNamespace
--   * UI_CreateFont, MPFONT_BOLD — из font_bootstrap.lua
--   * g_SystemFont — семейство по умолчанию (из FontManager::PushToLua)
--   * console.set_size / console.set_backdrop — из scripts/lua/console/init.lua
--
-- Порядок в Script.cpp / font_bootstrap:
--   debug.lua → console/helpers.lua → console/init.lua → font_bootstrap.lua → console_bootstrap.lua

local SEC = "Console"

local function read_ini_string(key, default)
    local sec = SystemIni and SystemIni:Section(SEC) or nil
    if not sec then return default end
    return sec:GetString(key, default or "")
end

local function read_ini_int(key, default)
    local sec = SystemIni and SystemIni:Section(SEC) or nil
    if not sec then return default end
    return sec:GetInt64(key, default or 0)
end

-- Дефолты (должны совпадать с значениями по умолчанию в INI).
local DEFAULTS = {
    font_name            = "Roboto",
    font_size            = 16,
    font_bold            = 1,
    width_percent        = 100,
    transparency_percent = 60,
    lines                = 20,
}

local font_name           = read_ini_string("font_name",            DEFAULTS.font_name)
local font_size           = read_ini_int   ("font_size",            DEFAULTS.font_size)
local font_bold           = read_ini_int   ("font_bold",            DEFAULTS.font_bold)
local width_pct           = read_ini_int   ("width_percent",        DEFAULTS.width_percent)
local transparency_pct    = read_ini_int   ("transparency_percent", DEFAULTS.transparency_percent)
local lines               = read_ini_int   ("lines",                DEFAULTS.lines)

-- Санитизация.
if font_size < 6  then font_size = 6  end
if font_size > 48 then font_size = 48 end
if width_pct < 10  then width_pct = 10  end
if width_pct > 100 then width_pct = 100 end
if transparency_pct < 0   then transparency_pct = 0   end
if transparency_pct > 100 then transparency_pct = 100 end
if lines < 3   then lines = 3   end
if lines > 200 then lines = 200 end

-- Шрифт консоли. Fallback — g_SystemFont, если в INI семейство не указано.
local family = (font_name ~= "" and font_name) or g_SystemFont or "Arial"
local flags  = (font_bold ~= 0) and MPFONT_BOLD or 0
UI_CreateFont("Console", family, font_size, font_size, 1, flags)

-- Размер консоли: ширина как % окна, высота = lines * шаг строки + padding (в C++).
if console and console.set_size then
    console.set_size(width_pct, lines)
end

-- Backdrop: тёмный фон с заданной прозрачностью. ARGB = alpha << 24 | 0x101018.
--   transparency_pct=0   → alpha=0xFF (полностью непрозрачный)
--   transparency_pct=100 → alpha=0x00 (полностью прозрачный)
local alpha = math.floor((100 - transparency_pct) * 255 / 100 + 0.5)
local argb  = alpha * 0x1000000 + 0x101018
if console and console.set_backdrop then
    console.set_backdrop(argb)
end

if ToDebugLog then
    ToDebugLog(string.format(
        "[console_bootstrap] font='%s' size=%d bold=%d width=%d%% transp=%d%% lines=%d",
        family, font_size, font_bold, width_pct, transparency_pct, lines))
end
