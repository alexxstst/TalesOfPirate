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
| SDL | 1.2.7 | LGPL v2 | [SDL.txt](SDL.txt) | https://www.libsdl.org |
| SDL_mixer | 1.2.x | zlib License | [SDL_mixer.txt](SDL_mixer.txt) | https://www.libsdl.org/projects/SDL_mixer |
| Discord RPC | — | MIT | [Discord-RPC.txt](Discord-RPC.txt) | https://github.com/discord/discord-rpc |
| DirectX (D3D9) | DX9 SDK | Microsoft EULA (редистрибутив) | [DirectX.txt](DirectX.txt) | Microsoft DirectX SDK |

## Шрифты

| Шрифт | Лицензия | Файл | Источник |
|-------|----------|------|----------|
| Inter | SIL OFL 1.1 | [fonts/Inter.txt](fonts/Inter.txt) | https://github.com/rsms/inter |
| Noto Sans | SIL OFL 1.1 | [fonts/NotoSans.txt](fonts/NotoSans.txt) | https://fonts.google.com/noto/specimen/Noto+Sans |
| Open Sans | SIL OFL 1.1 | [fonts/OpenSans.txt](fonts/OpenSans.txt) | https://fonts.google.com/specimen/Open+Sans |
| PT Sans | SIL OFL 1.1 | [fonts/PTSans.txt](fonts/PTSans.txt) | https://www.paratype.com/public/ |
| Roboto | Apache License 2.0 | [fonts/Roboto.txt](fonts/Roboto.txt) | https://github.com/googlefonts/roboto-2 |
| Source Sans 3 | SIL OFL 1.1 | [fonts/SourceSans3.txt](fonts/SourceSans3.txt) | https://github.com/adobe-fonts/source-sans |

## Важно про LGPL (SDL 1.2)

SDL 1.2 распространяется по LGPL v2. Конечному пользователю должна быть
обеспечена возможность заменить библиотеку на свою сборку — поэтому линковка
выполняется динамически (через `SDL.dll`), и полный текст LGPL поставляется
вместе с клиентом в этой папке (`SDL.txt`). SDL_mixer 1.2 с официального
репозитория libsdl-org — под zlib License (менее строгой, чем LGPL).
