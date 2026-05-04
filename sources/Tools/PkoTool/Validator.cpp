#include "Validator.h"

#include "AssetLoaders.h"
#include "lwExpObj.h"

#include "stb_image.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <format>
#include <string>
#include <vector>

namespace pkotool {

namespace fs = std::filesystem;

namespace {

[[nodiscard]] std::string ExtLower(const fs::path& p) {
    std::string ext = p.extension().string();
    if (!ext.empty() && ext.front() == '.') {
        ext.erase(ext.begin());
    }
    for (auto& ch : ext) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return ext;
}

// Маппинг диагностики LgoLoader → ValidationRecord.
// Возвращает пару (status, recommendation): problem/version подставляются caller'ом.
[[nodiscard]] std::pair<ValidationStatus, std::string>
ClassifyLgoStatus(Corsairs::Engine::Render::LgoLoadStatus s) {
    using S = Corsairs::Engine::Render::LgoLoadStatus;
    switch (s) {
    case S::Ok:
        return {ValidationStatus::Ok, ""};
    case S::OkWithTrailingData:
        return {ValidationStatus::Warning,
                "File parses, but a trailing block exists and will be dropped on re-save. "
                "Run `pkotool --mode=fix` to strip it."};
    case S::FileOpenFailed:
        return {ValidationStatus::Error,
                "Failed to open file (missing file or permissions)."};
    case S::VersionTruncated:
        return {ValidationStatus::Error,
                "File too short: even the DWORD version is missing. Re-export from source."};
    case S::VersionUnknown:
        return {ValidationStatus::Error,
                "Unknown format version. File was created by another tool; re-export needed."};
    case S::HeaderTruncated:
        return {ValidationStatus::Error,
                "Header truncated: file corrupted. Re-export from source."};
    case S::BlockSizesInconsistent:
        return {ValidationStatus::Error,
                "Header block sizes do not match actual file size. Re-export from source."};
    case S::ParseFailed:
        return {ValidationStatus::Error,
                "Parser returned an error (corrupt data inside a block). "
                "Re-export from source."};
    case S::UnconsumedTrailingData:
        return {ValidationStatus::Warning,
                "Unconsumed trailing data in the file."};
    }
    return {ValidationStatus::Error, "Unknown LgoLoader status."};
}

ValidationRecord ValidateLgo(const fs::path& file) {
    ValidationRecord rec;
    rec.file = file;
    rec.extension = "lgo";

    Corsairs::Engine::Render::LgoLoadDiagnostics diag;
    auto* info = Corsairs::Engine::Render::LgoLoader::LoadEx(file.string(), diag);
    rec.version = diag.version;

    auto [status, recommendation] = ClassifyLgoStatus(diag.status);
    rec.status = status;
    rec.problem = (status == ValidationStatus::Ok)
        ? std::string{}
        : std::format("{}: {}", Corsairs::Engine::Render::ToString(diag.status), diag.detail);
    rec.recommendation = std::move(recommendation);

    delete info;
    return rec;
}

ValidationRecord ValidateLmo(const fs::path& file) {
    ValidationRecord rec;
    rec.file = file;
    rec.extension = "lmo";

    MindPower::lwModelObjInfo info;
    Corsairs::Engine::Render::LgoLoadDiagnostics diag;
    Corsairs::Engine::Render::LgoLoader::LoadModelObjEx(info, file.string(), diag);
    rec.version = diag.version;

    auto [status, recommendation] = ClassifyLgoStatus(diag.status);
    rec.status = status;
    rec.problem = (status == ValidationStatus::Ok)
        ? std::string{}
        : std::format("{}: {}", Corsairs::Engine::Render::ToString(diag.status), diag.detail);
    rec.recommendation = std::move(recommendation);

    return rec;
}

ValidationRecord ValidateLxo(const fs::path& file) {
    ValidationRecord rec;
    rec.file = file;
    rec.extension = "lxo";

    MindPower::lwModelInfo info;
    Corsairs::Engine::Render::LgoLoadDiagnostics diag;
    Corsairs::Engine::Render::LgoLoader::LoadModelEx(info, file.string(), diag);
    rec.version = diag.version;

    auto [status, recommendation] = ClassifyLgoStatus(diag.status);
    rec.status = status;
    rec.problem = (status == ValidationStatus::Ok)
        ? std::string{}
        : std::format("{}: {}", Corsairs::Engine::Render::ToString(diag.status), diag.detail);
    rec.recommendation = std::move(recommendation);

    return rec;
}

ValidationRecord ValidateLab(const fs::path& file) {
    ValidationRecord rec;
    rec.file = file;
    rec.extension = "lab";

    MindPower::lwAnimDataBone info;
    Corsairs::Engine::Render::LgoLoadDiagnostics diag;
    Corsairs::Engine::Render::LgoLoader::LoadAnimDataBoneEx(info, file.string(), diag);
    rec.version = diag.version;

    auto [status, recommendation] = ClassifyLgoStatus(diag.status);
    rec.status = status;
    rec.problem = (status == ValidationStatus::Ok)
        ? std::string{}
        : std::format("{}: {}", Corsairs::Engine::Render::ToString(diag.status), diag.detail);
    rec.recommendation = std::move(recommendation);

    return rec;
}

// Считывает файл целиком в память. Для текстур (килобайты-мегабайты) это OK;
// stb_image без STBI_NO_STDIO в нашей сборке не доступен, поэтому идём через
// stbi_info_from_memory.
[[nodiscard]] bool ReadFileToBuffer(const fs::path& file, std::vector<unsigned char>& out) {
    std::FILE* fp = std::fopen(file.string().c_str(), "rb");
    if (fp == nullptr) {
        return false;
    }
    if (std::fseek(fp, 0, SEEK_END) != 0) {
        std::fclose(fp);
        return false;
    }
    const long sz = std::ftell(fp);
    if (sz < 0) {
        std::fclose(fp);
        return false;
    }
    if (std::fseek(fp, 0, SEEK_SET) != 0) {
        std::fclose(fp);
        return false;
    }
    out.resize(static_cast<std::size_t>(sz));
    const std::size_t read = std::fread(out.data(), 1, out.size(), fp);
    std::fclose(fp);
    return read == out.size();
}

// Базовая валидация текстур: stbi_info_from_memory читает только заголовок,
// не декодирует пиксели; возвращает (width, height, channels).
ValidationRecord ValidateTexture(const fs::path& file, std::string ext) {
    ValidationRecord rec;
    rec.file = file;
    rec.extension = std::move(ext);
    rec.version = 0;

    std::vector<unsigned char> buf;
    if (!ReadFileToBuffer(file, buf)) {
        rec.status = ValidationStatus::Error;
        rec.problem = "failed to read file";
        rec.recommendation = "Check path and access permissions.";
        return rec;
    }
    if (buf.empty()) {
        rec.status = ValidationStatus::Error;
        rec.problem = "empty file";
        rec.recommendation = "Zero-length file — replace with a valid one.";
        return rec;
    }

    int x = 0, y = 0, comp = 0;
    if (stbi_info_from_memory(buf.data(), static_cast<int>(buf.size()), &x, &y, &comp) == 0) {
        rec.status = ValidationStatus::Error;
        rec.problem = std::format("stbi_info_from_memory failed: {}",
                                  stbi_failure_reason() ? stbi_failure_reason() : "unknown");
        rec.recommendation = "File is not a recognised texture format.";
        return rec;
    }
    if (x <= 0 || y <= 0 || comp < 1 || comp > 4) {
        rec.status = ValidationStatus::Error;
        rec.problem = std::format("invalid dimensions: {}x{}x{}", x, y, comp);
        rec.recommendation = "Header parsed but dimensions/channels are out of range.";
        return rec;
    }

    rec.status = ValidationStatus::Ok;
    return rec;
}

} // namespace

std::string_view ToString(ValidationStatus s) noexcept {
    switch (s) {
    case ValidationStatus::Ok:      return "Ok";
    case ValidationStatus::Warning: return "Warning";
    case ValidationStatus::Error:   return "Error";
    }
    return "?";
}

ValidationRecord ValidateFile(const fs::path& file) {
    const std::string ext = ExtLower(file);

    if (ext == "lgo") return ValidateLgo(file);
    if (ext == "lmo") return ValidateLmo(file);
    if (ext == "lxo") return ValidateLxo(file);
    if (ext == "lab") return ValidateLab(file);
    if (ext == "bmp" || ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "tga") {
        return ValidateTexture(file, ext);
    }

    ValidationRecord rec;
    rec.file = file;
    rec.extension = ext;
    rec.status = ValidationStatus::Error;
    rec.problem = std::format("unsupported extension: .{}", ext);
    rec.recommendation = "Remove file from --scope or add support in Validator.";
    return rec;
}

} // namespace pkotool
