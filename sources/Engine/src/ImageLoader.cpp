#include "stdafx.h"
#include "ImageLoader.h"

#include "logutil.h"
#include "stb_image.h"

#include <cstdio>
#include <format>
#include <string>
#include <vector>

namespace Corsairs::Engine::Render {

namespace {

[[noreturn]] void FailDecode(std::string_view context,
                             std::string_view fileName,
                             std::string_view reason)
{
    const std::string msg = std::format(
        "ImageLoader: {} ({}): {}", fileName, context, reason);
    ToLogService("errors", LogLevel::Error, "{}", msg);
    throw ImageDecodeError(msg);
}

} // namespace

bool ImageLoader::CanHandle(std::string_view extension)
{
    return extension == "bmp"
        || extension == "tga"
        || extension == "png";
}

void ImageLoader::LoadImage(std::string_view fileName,
                             std::uint32_t colorKey,
                             std::string_view context,
                             DecodedImage& out)
{
    // fopen требует zero-terminated — копируем string_view в std::string.
    const std::string path(fileName);

    FILE* fp = std::fopen(path.c_str(), "rb");
    if (fp == nullptr) {
        FailDecode(context, path, "file not found");
    }

    std::fseek(fp, 0, SEEK_END);
    long size = std::ftell(fp);
    if (size <= 0) {
        std::fclose(fp);
        FailDecode(context, path, "empty or invalid file");
    }

    std::vector<unsigned char> fileBuf(static_cast<std::size_t>(size));
    std::fseek(fp, 0, SEEK_SET);
    const std::size_t read = std::fread(fileBuf.data(), 1, fileBuf.size(), fp);
    std::fclose(fp);
    if (read != fileBuf.size()) {
        FailDecode(context, path, "short read");
    }

    int width    = 0;
    int height   = 0;
    int channels = 0;
    // stb отдаёт RGBA8 top-down при запросе 4 каналов.
    stbi_uc* pixels = stbi_load_from_memory(
        fileBuf.data(),
        static_cast<int>(fileBuf.size()),
        &width,
        &height,
        &channels,
        4);

    if (pixels == nullptr || width <= 0 || height <= 0) {
        const char* reason = stbi_failure_reason();
        if (pixels != nullptr) {
            stbi_image_free(pixels);
        }
        FailDecode(context, path,
            std::format("unsupported/corrupted format ({})",
                        reason ? reason : "unknown"));
    }

    const std::size_t pixelCount =
        static_cast<std::size_t>(width) * static_cast<std::size_t>(height);

    auto* buf = new MindPower::lwColorValue4b[pixelCount];

    // colorKey сравнивается ТОЛЬКО по RGB (младшие 24 бита), alpha игнорируется —
    // это совместимо с прежним поведением lwBitmap::_SetAlphaChannel.
    // lwColorValue4b физически лежит как BGRA; RGB занимает младшие 24 бита
    // на little-endian (байты B,G,R).
    const bool applyKey = (colorKey != 0);
    const std::uint32_t keyRgb = colorKey & 0x00FFFFFFu;

    for (std::size_t i = 0; i < pixelCount; ++i) {
        const stbi_uc* src = pixels + i * 4;
        MindPower::lwColorValue4b* dst = &buf[i];
        dst->r = src[0];
        dst->g = src[1];
        dst->b = src[2];
        dst->a = src[3];

        if (applyKey && (dst->color & 0x00FFFFFFu) == keyRgb) {
            // Совпадение с цветовым ключом — обнуляем пиксель целиком
            // (совместимо с прежней семантикой lwBitmap::_SetAlphaChannel).
            dst->color = 0x00000000;
        }
    }

    stbi_image_free(pixels);

    out.ImageData = buf;
    out.Width     = static_cast<std::uint32_t>(width);
    out.Height    = static_cast<std::uint32_t>(height);
}

} // namespace Corsairs::Engine::Render
