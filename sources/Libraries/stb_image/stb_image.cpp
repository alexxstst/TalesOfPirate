// Единственный TU, в котором лежит реализация stb_image.
// Файл собирается БЕЗ PCH (stdafx.h) — см. per-file override в MindPower3D.vcxproj.
//
// Отключаем форматы, которые в проекте не используются, чтобы не тянуть лишний код:
//   - JPEG: отключён (не читаем jpg)
//   - HDR/LINEAR: отключены (нет HDR-пайплайна)
//   - GIF/PIC/PNM/PSD: отключены (текстур в этих форматах нет)
// Оставляем: BMP, PNG, TGA — то, что реально встречается в ресурсах клиента.

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#define STBI_NO_JPEG
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_NO_GIF
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STBI_NO_PSD
#include "stb_image.h"
