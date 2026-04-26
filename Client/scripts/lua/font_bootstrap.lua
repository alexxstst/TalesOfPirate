-- font_bootstrap.lua — первичная регистрация шрифтов клиента.
--
-- Вызывается из Main.cpp сразу после FontManager::Init / PushToLua,
-- до GameAppInit. К моменту загрузки GameApp C++ слоты (TipText,
-- MidAnnounce, BottomShadow) и Lua-шрифты (DEFAULT_FONT, FONT14, ...)
-- должны быть уже созданы в FontManager.
--
-- Сигнатура:
--   UI_CreateFont(name, family, size800, size1024, level, flags) -> int handle
--   UI_InstallFontDir(dir)     — рекурсивная регистрация всех *.ttf/*.ttc/*.otf
--   UI_InstallFontFile(path)   — одиночный файл
--   UI_DumpFonts(dir)          — превью всех зарегистрированных шрифтов (BMP)
--
-- Константы флагов в C++ MPFONT_BOLD/ITALIC/UNLINE выставлены как Lua globals.
--
-- NB: параметр `level` (размер атласа) остаётся в API для обратной совместимости,
-- но игнорируется — атлас теперь общий и управляется fontstash (расширяется сам
-- через fonsExpandAtlas при переполнении). MPFONT_BOLD/ITALIC тоже пока
-- игнорируются (fontstash не имеет API для синтетических стилей).

-- 1) Регистрируем все TTF/TTC/OTF из ./font (рекурсивно).
--    Любой файл-шрифт становится доступен по GDI-имени семейства
--    — через UI_CreateFont его можно запрашивать наравне с системным.
UI_InstallFontDir("./font")

-- 2) Резолв семейства из [Fonts] SystemFont. Fallback — Roboto
--    (есть в client/font/Roboto/, в отличие от системного Arial,
--    который через FreeType+fontstash не подхватывается — см. баг ниже).
local family = g_SystemFont or "Roboto"

-- 3) Слоты C++ (имя слота совпадает с FontSlot::X).
UI_CreateFont("TipText",      family, 12, 12, 1, 0)
UI_CreateFont("MidAnnounce",  family, 16, 16, 3, MPFONT_BOLD)
UI_CreateFont("BottomShadow", family, 12, 12, 3, MPFONT_BOLD)
UI_CreateFont("Console",      family, 16, 18, 1, 0)

-- 4) Lua-шрифты (handle — int, совместимо со старым API).
DEFAULT_FONT = UI_CreateFont("DEFAULT_FONT", family,  12, 12, 1, 0)
FONT14       = UI_CreateFont("FONT14",       family,  13, 13, 1, 0)
FONT16       = UI_CreateFont("FONT16",       family,  13, 13, 1, MPFONT_BOLD)
FONT20       = UI_CreateFont("FONT20",       family,  20, 20, 1, MPFONT_BOLD)
FONT28       = UI_CreateFont("FONT28",       family,  28, 28, 1, MPFONT_BOLD)
BIGFONT      = UI_CreateFont("BIGFONT",      family,  48, 48, 1, 0)
ARIAL_FONT   = UI_CreateFont("ARIAL_FONT",  "Roboto", 12, 12, 1, 0)
TIPFONT      = DEFAULT_FONT

-- 5) DrawTextShadow рендерит текст дважды (shadow + main), что может давать
--    визуальное "утолщение" на малых кеглях. Отключение оставит только main.
UI_SetFontShadowEnabled(1)

-- Диагностика (по необходимости раскомментировать):
-- UI_DumpFonts("./font/debug")             -- превью всех шрифтов в BMP
-- UI_DumpFontAtlases("./font/debug")       -- текущее содержимое D3D-атласов
-- UI_SetFontDumpEnabled(1)                 -- дамп каждого уникального рендеренного текста
