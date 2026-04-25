#include "stdafx.h"
#include "TextureLog.h"

#include "logutil.h"

#include <cstring>
#include <format>
#include <string>


namespace Corsairs::Engine::Render {

    namespace {
        //  Символическое имя D3DFORMAT (ради читаемости лога — раньше
        //  печатался как сырой int, например DXT5 = 894720068).
        std::string_view FormatToString(D3DFORMAT format)
        {
            switch (format) {
            case D3DFMT_A8R8G8B8:        return "A8R8G8B8";
            case D3DFMT_X8R8G8B8:        return "X8R8G8B8";
            case D3DFMT_R8G8B8:          return "R8G8B8";
            case D3DFMT_R5G6B5:          return "R5G6B5";
            case D3DFMT_A1R5G5B5:        return "A1R5G5B5";
            case D3DFMT_X1R5G5B5:        return "X1R5G5B5";
            case D3DFMT_A4R4G4B4:        return "A4R4G4B4";
            case D3DFMT_X4R4G4B4:        return "X4R4G4B4";
            case D3DFMT_A8:              return "A8";
            case D3DFMT_L8:              return "L8";
            case D3DFMT_A8L8:            return "A8L8";
            case D3DFMT_A4L4:            return "A4L4";
            case D3DFMT_DXT1:            return "DXT1";
            case D3DFMT_DXT2:            return "DXT2";
            case D3DFMT_DXT3:            return "DXT3";
            case D3DFMT_DXT4:            return "DXT4";
            case D3DFMT_DXT5:            return "DXT5";
            case D3DFMT_A16B16G16R16:    return "A16B16G16R16";
            case D3DFMT_A16B16G16R16F:   return "A16B16G16R16F";
            case D3DFMT_A32B32G32R32F:   return "A32B32G32R32F";
            case D3DFMT_R16F:            return "R16F";
            case D3DFMT_G16R16:          return "G16R16";
            case D3DFMT_G16R16F:         return "G16R16F";
            case D3DFMT_UNKNOWN:         return "UNKNOWN";
            default:                     return "?";
            }
        }

        std::string_view OpToString(TextureLogOp op)
        {
            switch (op) {
            case TextureLogOp::LOAD:    return "LOAD";
            case TextureLogOp::RELEASE: return "RELEASE";
            }
            return "?";
        }
    } // anonymous


    TextureLog& TextureLog::Instance()
    {
        static TextureLog instance;
        return instance;
    }

    TextureLog::TextureLog() = default;

    TextureLog::~TextureLog()
    {
        //  При штатном shutdown процесса — финальная сводка, если что-то
        //  было накоплено и лог был включён хотя бы раз.
        if (_totalCount > 0) {
            EmitSummary();
        }
    }

    void TextureLog::SetEnabled(bool enabled)
    {
        std::scoped_lock lock(_mutex);
        if (_enabled == enabled) {
            return;
        }
        _enabled = enabled;

        if (enabled) {
            ToLogService("textures", LogLevel::Info, "TextureLog enabled");
        }
        else if (_totalCount > 0) {
            //  При явном выключении — печатаем сводку накопленного.
            EmitSummary();
        }
    }

    bool TextureLog::IsEnabled() const
    {
        //  Чтение bool — атомарно на x64 для интересующих платформ.
        //  Гонка с SetEnabled даст значение «до или после», что приемлемо.
        return _enabled;
    }

    void TextureLog::RegisterCategory(std::string_view substring)
    {
        if (substring.empty()) {
            return;
        }
        std::scoped_lock lock(_mutex);
        for (const auto& cat : _categories) {
            if (cat.Substring == substring) {
                return; //  идемпотентно — повторная регистрация игнорируется
            }
        }
        _categories.push_back(Category{std::string(substring), 0, 0});
    }

    void TextureLog::Log(TextureLogOp op,
                         std::string_view file,
                         std::uint32_t width,
                         std::uint32_t height,
                         D3DFORMAT format,
                         std::uint32_t devmemSize)
    {
        if (!_enabled) {
            return;
        }

        std::scoped_lock lock(_mutex);

        //  Глобальные счётчики.
        if (op == TextureLogOp::LOAD) {
            _totalBytes += devmemSize;
            _totalCount += 1;
        }
        else if (_totalCount > 0) {
            //  Защита от ухода в «underflow» при Release-без-Load (обратное
            //  поведение старого lwTexLogMgr, где DWORD заворачивался).
            _totalBytes = (devmemSize <= _totalBytes) ? _totalBytes - devmemSize : 0;
            _totalCount -= 1;
        }

        //  Пер-категорийные счётчики — substring-матч по пути файла.
        for (auto& cat : _categories) {
            if (file.find(cat.Substring) == std::string_view::npos) {
                continue;
            }
            if (op == TextureLogOp::LOAD) {
                cat.TotalBytes += devmemSize;
                cat.Count += 1;
            }
            else if (cat.Count > 0) {
                cat.TotalBytes = (devmemSize <= cat.TotalBytes) ? cat.TotalBytes - devmemSize : 0;
                cat.Count -= 1;
            }
        }

        //  Одна строка на событие — без дублирования по категориям.
        ToLogService(
            "textures",
            LogLevel::Info,
            "{} file='{}' size={}x{} fmt={} mem={}B",
            OpToString(op),
            file,
            width,
            height,
            FormatToString(format),
            devmemSize);
    }

    void TextureLog::EmitSummary()
    {
        ToLogService(
            "textures",
            LogLevel::Info,
            "summary total={} textures, {}B",
            _totalCount,
            _totalBytes);

        for (const auto& cat : _categories) {
            ToLogService(
                "textures",
                LogLevel::Info,
                "summary category='{}' count={} bytes={}",
                cat.Substring,
                cat.Count,
                cat.TotalBytes);
        }
    }

} // namespace Corsairs::Engine::Render
