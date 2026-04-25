# Лицензии сторонних компонентов

В этой папке хранятся тексты лицензий всех сторонних библиотек, шрифтов и
иных компонентов, поставляемых вместе с клиентом Tales of Pirate.

## Правила

- При добавлении нового стороннего компонента в клиент **обязательно** положить
  сюда файл с текстом его лицензии.
- Имя файла — имя компонента, расширение `.txt`.
- Шрифты — в подпапке `fonts/`.
- Здесь, в `README.md`, в соответствующей таблице добавить строку с ссылкой на
  файл, указать версию и источник (URL).
- При удалении компонента — удалить и файл лицензии, и строку из таблицы.

## Библиотеки

| Компонент | Версия | Лицензия | Файл | Источник |
|-----------|--------|----------|------|----------|
| SQLite3 | amalgamation | Public Domain | [sqlite3.txt](sqlite3.txt) | https://www.sqlite.org |
| LuaJIT (+ Lua 5.1) | 2.1 | MIT | [LuaJIT.txt](LuaJIT.txt) | https://luajit.org |
| LuaBridge3 | header-only | MIT | [LuaBridge.txt](LuaBridge.txt) | https://github.com/kunitoki/LuaBridge3 |
| Crypto++ | 8.4+ | Boost Software License 1.0 | [Cryptopp.txt](Cryptopp.txt) | https://www.cryptopp.com |
| SDL3 | 3.4.4 | zlib License | [SDL3.txt](SDL3.txt) | https://github.com/libsdl-org/SDL |
| SDL3_mixer | 3.2.0 | zlib License | [SDL3_mixer.txt](SDL3_mixer.txt) | https://github.com/libsdl-org/SDL_mixer |
| Discord RPC | — | MIT | [Discord-RPC.txt](Discord-RPC.txt) | https://github.com/discord/discord-rpc |
| DirectX (D3D9) | DX9 SDK | Microsoft EULA (редистрибутив) | [DirectX.txt](DirectX.txt) | Microsoft DirectX SDK |
| FreeType | 2.x | FTL (BSD-подобная, по выбору) / GPLv2 | [FreeType.txt](FreeType.txt), [FreeType_FTL.txt](FreeType_FTL.txt), [FreeType_GPLv2.txt](FreeType_GPLv2.txt) | https://freetype.org |
| fontstash | header-only | zlib-style (Mikko Mononen) | [fontstash.txt](fontstash.txt) | https://github.com/memononen/fontstash |
| stb_image | 2.30 (header-only) | MIT / Public Domain (dual) | [stb_image.txt](stb_image.txt) | https://github.com/nothings/stb |

## Шрифты

| Шрифт | Лицензия | Файл | Источник |
|-------|----------|------|----------|
| Inter | SIL OFL 1.1 | [fonts/Inter.txt](fonts/Inter.txt) | https://github.com/rsms/inter |
| Noto Sans | SIL OFL 1.1 | [fonts/NotoSans.txt](fonts/NotoSans.txt) | https://fonts.google.com/noto/specimen/Noto+Sans |
| Open Sans | SIL OFL 1.1 | [fonts/OpenSans.txt](fonts/OpenSans.txt) | https://fonts.google.com/specimen/Open+Sans |
| PT Sans | SIL OFL 1.1 | [fonts/PTSans.txt](fonts/PTSans.txt) | https://www.paratype.com/public/ |
| Roboto | Apache License 2.0 | [fonts/Roboto.txt](fonts/Roboto.txt) | https://github.com/googlefonts/roboto-2 |
| Source Sans 3 | SIL OFL 1.1 | [fonts/SourceSans3.txt](fonts/SourceSans3.txt) | https://github.com/adobe-fonts/source-sans |

## Про SDL3 и динамическую линковку

SDL3 и SDL3_mixer распространяются под zlib License (MIT-подобная, атрибуция в
бинарных дистрибутивах). Линкуются **динамически** — `SDL3.dll` и
`SDL3_mixer.dll` лежат рядом с `Game.exe` в `Client/system/`. Ogg Vorbis для
фоновой музыки (`Client/music/*.ogg`) декодируется встроенным `stb_vorbis`
внутри `SDL3_mixer.dll` — внешний `libogg` не нужен.
