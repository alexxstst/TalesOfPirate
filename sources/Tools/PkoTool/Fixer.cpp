#include "Fixer.h"

#include "AssetLoaders.h"
#include "lwExpObj.h"
#include "logutil.h"

#include <filesystem>
#include <string>
#include <system_error>

namespace pkotool {

namespace fs = std::filesystem;

namespace {

constexpr const char* kLogChannel = "pkotool";

// Резулдьтат одной попытки re-save: успех, либо причина сбоя.
struct ResaveResult {
    bool ok{false};
    std::string detail;
};

// Universal scheme: переименовать original → original.bak, затем загрузить .bak,
// сохранить под original. Если save упал — откатить (.bak → original).
template <typename LoadFn, typename SaveFn>
ResaveResult ResaveViaBak(const fs::path& path, LoadFn&& load, SaveFn&& save) {
    const fs::path bak = path.string() + ".bak";

    std::error_code ec;
    fs::rename(path, bak, ec);
    if (ec) {
        return {false, std::format("rename → .bak failed: {}", ec.message())};
    }

    if (!load(bak.string())) {
        // Откатываем: возвращаем .bak на место.
        std::error_code ec2;
        fs::rename(bak, path, ec2);
        return {false, "load from .bak failed (rolled back)"};
    }
    if (!save(path.string())) {
        std::error_code ec2;
        fs::remove(path, ec2);
        ec2.clear();
        fs::rename(bak, path, ec2);
        return {false, "save failed (rolled back)"};
    }

    return {true, {}};
}

ResaveResult ResaveLgo(const fs::path& path) {
    using LgoLoader = Corsairs::Engine::Render::LgoLoader;
    MindPower::lwGeomObjInfo* info = nullptr;
    auto cleanup = [&](){ delete info; info = nullptr; };

    auto load = [&](const std::string& f) {
        info = LgoLoader::Load(f);
        return info != nullptr;
    };
    auto save = [&](const std::string& f) {
        const LW_RESULT r = LgoLoader::Save(info, f);
        cleanup();
        return !LW_FAILED(r);
    };

    auto result = ResaveViaBak(path, load, save);
    cleanup();
    return result;
}

ResaveResult ResaveLmo(const fs::path& path) {
    using LgoLoader = Corsairs::Engine::Render::LgoLoader;
    MindPower::lwModelObjInfo info;

    auto load = [&](const std::string& f) {
        return !LW_FAILED(LgoLoader::LoadModelObj(info, f));
    };
    auto save = [&](const std::string& f) {
        return !LW_FAILED(LgoLoader::SaveModelObj(info, f));
    };
    return ResaveViaBak(path, load, save);
}

ResaveResult ResaveLab(const fs::path& path) {
    using LgoLoader = Corsairs::Engine::Render::LgoLoader;
    MindPower::lwAnimDataBone info;

    auto load = [&](const std::string& f) {
        return !LW_FAILED(LgoLoader::LoadAnimDataBone(info, f));
    };
    auto save = [&](const std::string& f) {
        return !LW_FAILED(LgoLoader::SaveAnimDataBone(info, f));
    };
    return ResaveViaBak(path, load, save);
}

[[nodiscard]] bool DeleteFile(const fs::path& path) {
    std::error_code ec;
    fs::remove(path, ec);
    return !ec;
}

} // namespace

FixSummary ApplyFix(const std::vector<ValidationRecord>& records) {
    FixSummary s;

    for (const auto& r : records) {
        switch (r.status) {
        case ValidationStatus::Ok:
            ++s.skippedOk;
            break;

        case ValidationStatus::Error:
            if (DeleteFile(r.file)) {
                ToLogService(kLogChannel, LogLevel::Warning,
                             "DELETED: {} ({})", r.file.string(), r.problem);
                ++s.deleted;
            }
            else {
                ToLogService(kLogChannel, LogLevel::Error,
                             "delete failed: {}", r.file.string());
                ++s.failed;
            }
            break;

        case ValidationStatus::Warning: {
            // Re-save поддержан только для бинарных Mindpower-форматов.
            const bool supported = (r.extension == "lgo" || r.extension == "lmo"
                                    || r.extension == "lab");
            if (!supported) {
                ToLogService(kLogChannel, LogLevel::Info,
                             "skip (re-save for .{} is not implemented): {}",
                             r.extension, r.file.string());
                ++s.skippedUnsupported;
                break;
            }

            ResaveResult res;
            if (r.extension == "lgo") res = ResaveLgo(r.file);
            else if (r.extension == "lmo") res = ResaveLmo(r.file);
            else                            res = ResaveLab(r.file);

            if (res.ok) {
                ToLogService(kLogChannel, LogLevel::Warning,
                             "RESAVED (with .bak): {} ({})",
                             r.file.string(), r.problem);
                ++s.resaved;
            }
            else {
                ToLogService(kLogChannel, LogLevel::Error,
                             "resave failed: {}: {}", r.file.string(), res.detail);
                ++s.failed;
            }
            break;
        }
        }
    }

    return s;
}

} // namespace pkotool
