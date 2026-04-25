#pragma once

#include "lwDirectX.h"

#include <cstdint>
#include <stdexcept>
#include <string_view>


namespace Corsairs::Engine::Render {

class ImageDecodeError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

struct DecodedImage {
    MindPower::lwColorValue4b* ImageData{};
    std::uint32_t Width{};
    std::uint32_t Height{};
};

// Единая точка декодирования изображений. stateless, все методы — static.
// Под капотом — stb_image. Поддерживаемые расширения: "bmp", "tga", "png".
class ImageLoader {
public:
    // ext — в lowercase, без точки ("bmp", "tga", "png").
    static bool CanHandle(std::string_view extension);

    // Бросает ImageDecodeError при ошибке (логирует в "errors" перед throw).
    // out.ImageData — new[]-массив lwColorValue4b[W*H], освобождение — за caller'ом (delete[]).
    // colorKey: 0 — не применять; иначе сравнение по RGB (младшие 24 бита), совпавшие пиксели обнуляются.
    static void LoadImage(std::string_view fileName,
                          std::uint32_t colorKey,
                          std::string_view context,
                          DecodedImage& out);
};

} // namespace Corsairs::Engine::Render
