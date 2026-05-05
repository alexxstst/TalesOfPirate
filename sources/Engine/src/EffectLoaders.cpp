// Реализация EffectLoader (.eff) и PartCtrlLoader (.par). Вынесено в отдельный
// TU из AssetLoaders.cpp специально: эти лоадеры тянут MPParticleCtrl/MPModelEff/
// I_Effect (которые транзитом тянут TextureManager + Cryptopp + Version.lib).
// Тулзы вроде PkoTool/AssetLoaderTests, которые работают только с LgoLoader,
// не дёргают EffectLoader — и линкер не пуллит эти .obj-ы из MindPower3D.lib,
// что избавляет от навешивания Cryptopp.lib/Version.lib в их .vcxproj.

#include "AssetLoaders.h"

#include "MPModelEff.h"      // EffectFileInfo, EffParameter, I_Effect
#include "MPParticleCtrl.h"  // CMPPartCtrl

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <format>
#include <memory>
#include <string>
#include <string_view>

namespace Corsairs::Engine::Render {

namespace {

// RAII для std::FILE*: тот же helper, что и в AssetLoaders.cpp, повторён локально
// (анонимный namespace в каждом TU независим).
struct FileCloser {
    void operator()(std::FILE* fp) const noexcept {
        if (fp != nullptr) {
            std::fclose(fp);
        }
    }
};
using UniqueFile = std::unique_ptr<std::FILE, FileCloser>;

// Записать строку в фиксированный 32-байтный буфер (формат .eff требует
// именно столько). Излишек обрезается, [31] всегда нулевой.
void WriteFixedName32(const std::string& src, std::FILE* fp) {
    char buf[32]{};
    std::memcpy(buf, src.data(),
                std::min<std::size_t>(src.size(), sizeof(buf) - 1));
    std::fwrite(buf, 1, sizeof(buf), fp);
}

constexpr std::uint32_t kMinEffectVersion = 1;
constexpr std::uint32_t kMaxEffectVersion = EffectLoader::kCurrentVersion; // = 7

} // namespace

// =============================================================================
// EffectLoader (.eff)
// =============================================================================

LW_RESULT EffectLoader::Load(::EffectFileInfo& info, std::string_view file) {
    EffectLoadDiagnostics diag;
    return LoadEx(info, file, diag);
}

LW_RESULT EffectLoader::LoadEx(::EffectFileInfo& info, std::string_view file,
                               EffectLoadDiagnostics& diag) {
    diag = {};
    info = ::EffectFileInfo{};

    UniqueFile fp{std::fopen(std::string{file}.c_str(), "rb")};
    if (!fp) {
        diag.status = EffectLoadStatus::FileOpenFailed;
        diag.detail = "fopen failed";
        return LW_RET_FAILED;
    }

    DWORD version = 0;
    if (std::fread(&version, sizeof(version), 1, fp.get()) != 1) {
        diag.status = EffectLoadStatus::VersionTruncated;
        diag.detail = "short read of version DWORD";
        return LW_RET_FAILED;
    }
    info.version = version;
    diag.version = version;

    if (version < kMinEffectVersion || version > kMaxEffectVersion) {
        diag.status = EffectLoadStatus::VersionUnknown;
        diag.detail = std::format("version=0x{:08X} (expected {}..{})",
                                  version, kMinEffectVersion, kMaxEffectVersion);
        return LW_RET_FAILED;
    }

    int idxTech = 0;
    if (std::fread(&idxTech, sizeof(int), 1, fp.get()) != 1) {
        diag.status = EffectLoadStatus::HeaderTruncated;
        diag.detail = "short read of m_iIdxTech";
        return LW_RET_FAILED;
    }
    info.param.m_iIdxTech = idxTech;

    char nameBuf[32]{};

    if (std::fread(&info.param.m_bUsePath, sizeof(bool), 1, fp.get()) != 1 ||
        std::fread(nameBuf, 1, sizeof(nameBuf), fp.get()) != sizeof(nameBuf)) {
        diag.status = EffectLoadStatus::HeaderTruncated;
        diag.detail = "short read of path block";
        return LW_RET_FAILED;
    }
    info.param.m_szPathName = nameBuf;

    if (std::fread(&info.param.m_bUseSound, sizeof(bool), 1, fp.get()) != 1 ||
        std::fread(nameBuf, 1, sizeof(nameBuf), fp.get()) != sizeof(nameBuf)) {
        diag.status = EffectLoadStatus::HeaderTruncated;
        diag.detail = "short read of sound block";
        return LW_RET_FAILED;
    }
    info.param.m_szSoundName = nameBuf;

    if (std::fread(&info.param.m_bRotating, sizeof(bool), 1, fp.get()) != 1 ||
        std::fread(&info.param.m_SVerRota, sizeof(D3DXVECTOR3), 1, fp.get()) != 1 ||
        std::fread(&info.param.m_fRotaVel, sizeof(float), 1, fp.get()) != 1) {
        diag.status = EffectLoadStatus::HeaderTruncated;
        diag.detail = "short read of rotation block";
        return LW_RET_FAILED;
    }

    int count = 0;
    if (std::fread(&count, sizeof(int), 1, fp.get()) != 1) {
        diag.status = EffectLoadStatus::HeaderTruncated;
        diag.detail = "short read of effect count";
        return LW_RET_FAILED;
    }
    if (count < 0) {
        diag.status = EffectLoadStatus::ParseFailed;
        diag.detail = std::format("negative effect count={}", count);
        return LW_RET_FAILED;
    }

    info.effects.resize(static_cast<std::size_t>(count));
    for (int n = 0; n < count; ++n) {
        if (!LoadElement(info.effects[n], fp.get(), version)) {
            diag.status = EffectLoadStatus::ParseFailed;
            diag.detail = std::format("I_Effect[{}].Load failed", n);
            return LW_RET_FAILED;
        }
    }

    diag.status = EffectLoadStatus::Ok;
    return LW_RET_OK;
}

LW_RESULT EffectLoader::Save(::EffectFileInfo& info, std::string_view file) {
    UniqueFile fp{std::fopen(std::string{file}.c_str(), "wb")};
    if (!fp) {
        return LW_RET_FAILED;
    }

    // Save всегда пишет latest format. Если info.version < kCurrentVersion —
    // старые файлы при пересохранении подтягиваются к актуальной структуре.
    DWORD version = kCurrentVersion;
    std::fwrite(&version, sizeof(version), 1, fp.get());

    int idxTech = info.param.m_iIdxTech;
    std::fwrite(&idxTech, sizeof(int), 1, fp.get());

    std::fwrite(&info.param.m_bUsePath, sizeof(bool), 1, fp.get());
    WriteFixedName32(info.param.m_szPathName, fp.get());

    std::fwrite(&info.param.m_bUseSound, sizeof(bool), 1, fp.get());
    WriteFixedName32(info.param.m_szSoundName, fp.get());

    std::fwrite(&info.param.m_bRotating, sizeof(bool), 1, fp.get());
    std::fwrite(&info.param.m_SVerRota, sizeof(D3DXVECTOR3), 1, fp.get());
    std::fwrite(&info.param.m_fRotaVel, sizeof(float), 1, fp.get());

    int count = static_cast<int>(info.effects.size());
    std::fwrite(&count, sizeof(int), 1, fp.get());

    for (int n = 0; n < count; ++n) {
        if (!SaveElement(info.effects[n], fp.get())) {
            return LW_RET_FAILED;
        }
    }

    return LW_RET_OK;
}

// =============================================================================
// I_Effect (один элемент .eff). Перенесено сюда из I_Effect::Load/SaveToFile —
// data-классы по соглашению проекта не должны содержать I/O.
// =============================================================================

bool EffectLoader::SaveElement(::I_Effect& e, std::FILE* fp) {
    // Имена в .eff-файле — фиксированный 32-байтный буфер.
    auto writeFixedName = [&](std::string_view src) {
        char t_pszName[32]{};
        std::memcpy(t_pszName, src.data(),
                    std::min<std::size_t>(src.size(), sizeof(t_pszName) - 1));
        std::fwrite(t_pszName, sizeof(char), 32, fp);
    };
    writeFixedName(e.m_strEffectName);

    int t_temp = (int)e._eEffectType;
    std::fwrite(&t_temp, sizeof(int), 1, fp);

    t_temp = (int)e._eSrcBlend;
    std::fwrite(&t_temp, sizeof(int), 1, fp);
    t_temp = (int)e._eDestBlend;
    std::fwrite(&t_temp, sizeof(int), 1, fp);

    std::fwrite(&e._fLength, sizeof(float), 1, fp);
    std::fwrite(&e._wFrameCount, sizeof(WORD), 1, fp);

    for (WORD n = 0; n < e._wFrameCount; n++) {
        std::fwrite(&e._vecFrameTime[n], sizeof(float), 1, fp);
    }
    for (WORD n = 0; n < e._wFrameCount; n++) {
        std::fwrite(&e._vecFrameSize[n], sizeof(D3DXVECTOR3), 1, fp);
    }
    for (WORD n = 0; n < e._wFrameCount; n++) {
        std::fwrite(&e._vecFrameAngle[n], sizeof(D3DXVECTOR3), 1, fp);
    }
    for (WORD n = 0; n < e._wFrameCount; n++) {
        std::fwrite(&e._vecFramePos[n], sizeof(D3DXVECTOR3), 1, fp);
    }
    for (WORD n = 0; n < e._wFrameCount; n++) {
        std::fwrite(&e._vecFrameColor[n], sizeof(D3DXCOLOR), 1, fp);
    }

    std::fwrite(&e.m_CTexCoordlist.m_wVerCount, sizeof(WORD), 1, fp);
    std::fwrite(&e.m_CTexCoordlist.m_wCoordCount, sizeof(WORD), 1, fp);
    std::fwrite(&e.m_CTexCoordlist.m_fFrameTime, sizeof(float), 1, fp);

    for (WORD n = 0; n < e.m_CTexCoordlist.m_wCoordCount; ++n) {
        std::fwrite(&e.m_CTexCoordlist.m_vecCoordList[n].front(),
                    sizeof(D3DXVECTOR2), e.m_CTexCoordlist.m_wVerCount, fp);
    }

    std::fwrite(&e.m_CTextruelist.m_wTexCount, sizeof(WORD), 1, fp);
    std::fwrite(&e.m_CTextruelist.m_fFrameTime, sizeof(float), 1, fp);

    writeFixedName(e.m_CTextruelist.m_vecTexName);

    for (WORD n = 0; n < e.m_CTextruelist.m_wTexCount; n++) {
        std::fwrite(&e.m_CTextruelist.m_vecTexList[n].front(),
                    sizeof(D3DXVECTOR2), e.m_CTexCoordlist.m_wVerCount, fp);
    }

    // Имя модели и cylinder-параметры пишем из собственных полей I_Effect
    // (LoadElement наполняет именно их). Раньше Save обращался через
    // m_pCModel->m_strName / ->m_nSegments — что валится с nullptr-deref'ом
    // на тулз-/тестовом round-trip'е, где engine-binding (Reset) пропущен.
    // При нормальной работе клиента m_pCModel и I_Effect-поля синхронны.
    writeFixedName(e.m_strModelName);

    std::fwrite(&e._bBillBoard, sizeof(bool), 1, fp);
    std::fwrite(&e._iVSIndex, sizeof(int), 1, fp);

    std::fwrite(&e.m_nSegments, sizeof(int), 1, fp);
    std::fwrite(&e.m_rHeight, sizeof(float), 1, fp);
    std::fwrite(&e.m_rRadius, sizeof(float), 1, fp);
    std::fwrite(&e.m_rBotRadius, sizeof(float), 1, fp);

    std::fwrite(&e.m_CTexFrame.m_wTexCount, sizeof(WORD), 1, fp);
    std::fwrite(&e.m_CTexFrame.m_fFrameTime, sizeof(float), 1, fp);
    for (WORD n = 0; n < e.m_CTexFrame.m_wTexCount; ++n) {
        writeFixedName(e.m_CTexFrame.m_vecTexName[n]);
    }
    std::fwrite(&e.m_CTexFrame.m_fFrameTime, sizeof(float), 1, fp);

    std::fwrite(&e._iUseParam, sizeof(int), 1, fp);
    if (e._iUseParam > 0) {
        for (int n = 0; n < e._wFrameCount; ++n) {
            std::fwrite(&e._CylinderParam[n].iSegments, sizeof(int), 1, fp);
            std::fwrite(&e._CylinderParam[n].fHei, sizeof(float), 1, fp);
            std::fwrite(&e._CylinderParam[n].fTopRadius, sizeof(float), 1, fp);
            std::fwrite(&e._CylinderParam[n].fBottomRadius, sizeof(float), 1, fp);
        }
    }
    std::fwrite(&e._bRotaLoop, sizeof(bool), 1, fp);
    std::fwrite(&e._vRotaLoop, sizeof(D3DXVECTOR4), 1, fp);

    std::fwrite(&e._bAlpha, sizeof(bool), 1, fp);
    std::fwrite(&e._bRotaBoard, sizeof(bool), 1, fp);

    return true;
}

bool EffectLoader::LoadElement(::I_Effect& e, std::FILE* fp, DWORD dwVersion) {
    e.ReleaseAll();

    char t_pszName[32];
    std::fread(t_pszName, sizeof(char), 32, fp);
    e.m_strEffectName = t_pszName;

    int t_temp;
    std::fread(&t_temp, sizeof(int), 1, fp);
    e._eEffectType = (EFFECT_TYPE)t_temp;

    std::fread(&t_temp, sizeof(int), 1, fp);
    e._eSrcBlend = (D3DBLEND)t_temp;
    std::fread(&t_temp, sizeof(int), 1, fp);
    e._eDestBlend = (D3DBLEND)t_temp;

    std::fread(&e._fLength, sizeof(float), 1, fp);
    std::fread(&e._wFrameCount, sizeof(WORD), 1, fp);

    e._vecFrameTime.resize(e._wFrameCount);
    for (WORD n = 0; n < e._wFrameCount; n++) {
        std::fread(&e._vecFrameTime[n], sizeof(float), 1, fp);
    }
    e._vecFrameSize.resize(e._wFrameCount);
    for (WORD n = 0; n < e._wFrameCount; n++) {
        std::fread(&e._vecFrameSize[n], sizeof(D3DXVECTOR3), 1, fp);
    }
    e._vecFrameAngle.resize(e._wFrameCount);
    for (WORD n = 0; n < e._wFrameCount; n++) {
        std::fread(&e._vecFrameAngle[n], sizeof(D3DXVECTOR3), 1, fp);
    }
    e._vecFramePos.resize(e._wFrameCount);
    for (WORD n = 0; n < e._wFrameCount; n++) {
        std::fread(&e._vecFramePos[n], sizeof(D3DXVECTOR3), 1, fp);
    }
    e._vecFrameColor.resize(e._wFrameCount);
    for (WORD n = 0; n < e._wFrameCount; n++) {
        std::fread(&e._vecFrameColor[n], sizeof(D3DXCOLOR), 1, fp);
    }

    std::fread(&e.m_CTexCoordlist.m_wVerCount, sizeof(WORD), 1, fp);
    std::fread(&e.m_CTexCoordlist.m_wCoordCount, sizeof(WORD), 1, fp);
    std::fread(&e.m_CTexCoordlist.m_fFrameTime, sizeof(float), 1, fp);
    e.m_CTexCoordlist.m_vecCoordList.resize(e.m_CTexCoordlist.m_wCoordCount);
    for (WORD n = 0; n < e.m_CTexCoordlist.m_wCoordCount; n++) {
        e.m_CTexCoordlist.m_vecCoordList[n].resize(e.m_CTexCoordlist.m_wVerCount);
        std::fread(&e.m_CTexCoordlist.m_vecCoordList[n].front(),
                   sizeof(D3DXVECTOR2), e.m_CTexCoordlist.m_wVerCount, fp);
    }

    std::fread(&e.m_CTextruelist.m_wTexCount, sizeof(WORD), 1, fp);
    std::fread(&e.m_CTextruelist.m_fFrameTime, sizeof(float), 1, fp);

    std::fread(t_pszName, sizeof(char), 32, fp);
    std::string lowerName{t_pszName};
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    std::string_view lowerView{lowerName};
    if (lowerView.ends_with(".dds") || lowerView.ends_with(".tga")) {
        e.m_CTextruelist.m_vecTexName.assign(lowerView.substr(0, lowerView.size() - 4));
    }
    else {
        e.m_CTextruelist.m_vecTexName = lowerName;
    }

    e.m_CTextruelist.m_vecTexList.resize(e.m_CTextruelist.m_wTexCount);
    for (WORD n = 0; n < e.m_CTextruelist.m_wTexCount; n++) {
        e.m_CTextruelist.m_vecTexList[n].resize(e.m_CTexCoordlist.m_wVerCount);
        std::fread(&e.m_CTextruelist.m_vecTexList[n].front(),
                   sizeof(D3DXVECTOR2), e.m_CTexCoordlist.m_wVerCount, fp);
    }

    std::fread(t_pszName, sizeof(char), 32, fp);
    e.m_strModelName = t_pszName;

    std::fread(&e._bBillBoard, sizeof(bool), 1, fp);
    std::fread(&e._iVSIndex, sizeof(int), 1, fp);

    if (dwVersion > 1) {
        std::fread(&e.m_nSegments, sizeof(int), 1, fp);
        std::fread(&e.m_rHeight, sizeof(float), 1, fp);
        std::fread(&e.m_rRadius, sizeof(float), 1, fp);
        std::fread(&e.m_rBotRadius, sizeof(float), 1, fp);
    }
    if (dwVersion > 2) {
        std::fread(&e.m_CTexFrame.m_wTexCount, sizeof(WORD), 1, fp);
        std::fread(&e.m_CTexFrame.m_fFrameTime, sizeof(float), 1, fp);

        e.m_CTexFrame.m_vecTexName.resize(e.m_CTexFrame.m_wTexCount);
        e.m_CTexFrame.m_vecTexs.resize(e.m_CTexFrame.m_wTexCount);

        for (WORD n = 0; n < e.m_CTexFrame.m_wTexCount; ++n) {
            std::fread(t_pszName, sizeof(char), 32, fp);
            e.m_CTexFrame.m_vecTexName[n] = t_pszName;
        }
        std::fread(&e.m_CTexFrame.m_fFrameTime, sizeof(float), 1, fp);
    }
    e._iUseParam = 0;
    e._CylinderParam.resize(e._wFrameCount);

    if (dwVersion > 3) {
        std::fread(&e._iUseParam, sizeof(int), 1, fp);
        if (e._iUseParam > 0) {
            for (int n = 0; n < e._wFrameCount; ++n) {
                std::fread(&e._CylinderParam[n].iSegments, sizeof(int), 1, fp);
                std::fread(&e._CylinderParam[n].fHei, sizeof(float), 1, fp);
                std::fread(&e._CylinderParam[n].fTopRadius, sizeof(float), 1, fp);
                std::fread(&e._CylinderParam[n].fBottomRadius, sizeof(float), 1, fp);

                e._CylinderParam[n].Create();
            }
        }
        else {
            if (IsCylinderMesh(e.m_strModelName)) {
                for (int n = 0; n < e._wFrameCount; ++n) {
                    e._CylinderParam[n].iSegments = e.m_nSegments;
                    e._CylinderParam[n].fTopRadius = e.m_rRadius;
                    e._CylinderParam[n].fBottomRadius = e.m_rBotRadius;
                    e._CylinderParam[n].fHei = e.m_rHeight;
                    e._CylinderParam[n].Create();
                }
            }
        }
    }
    else {
        if (IsCylinderMesh(e.m_strModelName)) {
            e._iUseParam = 0;

            for (int n = 0; n < e._wFrameCount; ++n) {
                e._CylinderParam[n].iSegments = e.m_nSegments;
                e._CylinderParam[n].fTopRadius = e.m_rRadius;
                e._CylinderParam[n].fBottomRadius = e.m_rBotRadius;
                e._CylinderParam[n].fHei = e.m_rHeight;
                e._CylinderParam[n].Create();
            }
        }
    }
    if (dwVersion > 4) {
        std::fread(&e._bRotaLoop, sizeof(bool), 1, fp);
        std::fread(&e._vRotaLoop, sizeof(D3DXVECTOR4), 1, fp);
    }
    if (dwVersion > 5) {
        std::fread(&e._bAlpha, sizeof(bool), 1, fp);
    }
    if (dwVersion > 6) {
        std::fread(&e._bRotaBoard, sizeof(bool), 1, fp);
    }

    e.IsSame();
    return true;
}

// =============================================================================
// PartCtrlLoader (.par) — чтение/запись CMPPartCtrl и его per-element данных
// (CMPPartSys/CMPStrip/CChaModel). Все методы data-классов перенесены сюда
// через friend-доступ; data-классы стали POD-данными без I/O.
// =============================================================================

// --- CMPPartSys (один particle system внутри .par) ----------------------------

bool PartCtrlLoader::SavePartSys(::CMPPartSys& ps, std::FILE* fp) {
    auto writeFixedName = [&](std::string_view src) {
        char buf[32]{};
        std::memcpy(buf, src.data(),
                    std::min<std::size_t>(src.size(), sizeof(buf) - 1));
        std::fwrite(buf, sizeof(char), 32, fp);
    };

    int tm;
    std::fwrite(&ps._iType, sizeof(int), 1, fp);
    writeFixedName(ps._strPartName);
    std::fwrite(&ps._iParNum, sizeof(int), 1, fp);
    writeFixedName(ps._strTexName);
    writeFixedName(ps._strModelName);

    std::fwrite(&ps._fRange[0], sizeof(float), 1, fp);
    std::fwrite(&ps._fRange[1], sizeof(float), 1, fp);
    std::fwrite(&ps._fRange[2], sizeof(float), 1, fp);

    std::fwrite(&ps._wFrameCount, sizeof(WORD), 1, fp);

    for (WORD n = 0; n < ps._wFrameCount; n++) {
        std::fwrite(ps._vecFrameSize[n], sizeof(float), 1, fp);
    }
    for (WORD n = 0; n < ps._wFrameCount; n++) {
        std::fwrite(ps._vecFrameAngle[n], sizeof(D3DXVECTOR3), 1, fp);
    }
    for (WORD n = 0; n < ps._wFrameCount; n++) {
        std::fwrite(ps._vecFrameColor[n], sizeof(D3DXCOLOR), 1, fp);
    }
    std::fwrite(&ps._bBillBoard, sizeof(bool), 1, fp);

    tm = (int)ps._eSrcBlend;
    std::fwrite(&tm, sizeof(int), 1, fp);
    tm = (int)ps._eDestBlend;
    std::fwrite(&tm, sizeof(int), 1, fp);
    tm = (int)ps._eMinFilter;
    std::fwrite(&tm, sizeof(int), 1, fp);
    tm = (int)ps._eMagFilter;
    std::fwrite(&tm, sizeof(int), 1, fp);

    std::fwrite(&ps._fLife, sizeof(float), 1, fp);
    std::fwrite(&ps._fVecl, sizeof(float), 1, fp);
    std::fwrite(&ps._vDir, sizeof(D3DXVECTOR3), 1, fp);
    std::fwrite(&ps._vAccel, sizeof(D3DXVECTOR3), 1, fp);
    std::fwrite(&ps._fStep, sizeof(float), 1, fp);

    std::fwrite(&ps._bModelRange, sizeof(bool), 1, fp);
    writeFixedName(ps._strVirualModel);

    std::fwrite(&ps._vOffset, sizeof(D3DXVECTOR3), 1, fp);
    std::fwrite(&ps._fDelayTime, sizeof(float), 1, fp);
    std::fwrite(&ps._fPlayTime, sizeof(float), 1, fp);

    bool busepsth = ps._pcPath ? true : false;
    std::fwrite(&busepsth, sizeof(bool), 1, fp);
    if (busepsth) {
        ps._pcPath->SavePath(fp);
    }

    std::fwrite(&ps.m_bShade, sizeof(bool), 1, fp);

    writeFixedName(ps.m_strHitEff);

    if (ps._bModelRange) {
        std::fwrite(&ps._wVecNum, sizeof(WORD), 1, fp);
        std::fwrite(&ps._vecPointRange.front(), sizeof(D3DXVECTOR3), ps._wVecNum, fp);
    }
    std::fwrite(&ps._iRoadom, sizeof(int), 1, fp);
    std::fwrite(&ps._bModelDir, sizeof(bool), 1, fp);
    std::fwrite(&ps._bMediay, sizeof(bool), 1, fp);

    return true;
}

bool PartCtrlLoader::LoadPartSys(::CMPPartSys& ps, std::FILE* fp, DWORD dwVersion) {
    int tm;
    std::fread(&ps._iType, sizeof(int), 1, fp);

    char t_pszName[32];
    std::fread(t_pszName, sizeof(char), 32, fp);
    ps._strPartName = t_pszName;

    std::fread(&ps._iParNum, sizeof(int), 1, fp);
    ps._vecParticle.setsize(ps._iParNum);

    std::fread(t_pszName, sizeof(char), 32, fp);
    s_string sFileName = t_pszName;
    std::transform(sFileName.begin(), sFileName.end(),
                   sFileName.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (sFileName.rfind(".dds") == std::string::npos
        && sFileName.rfind(".tga") == std::string::npos) {
        ps._strTexName = sFileName;
    }
    else {
        ps._strTexName = sFileName.substr(0, sFileName.rfind('.'));
    }

    std::fread(t_pszName, sizeof(char), 32, fp);
    if (IsDefaultMesh(t_pszName)) {
        ps._strModelName = t_pszName;
    }
    else {
        sFileName = t_pszName;
        std::transform(sFileName.begin(), sFileName.end(),
                       sFileName.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        ps._strModelName = sFileName;
    }
    std::fread(&ps._fRange[0], sizeof(float), 1, fp);
    std::fread(&ps._fRange[1], sizeof(float), 1, fp);
    std::fread(&ps._fRange[2], sizeof(float), 1, fp);

    std::fread(&ps._wFrameCount, sizeof(WORD), 1, fp);

    ps._vecFrameSize.setsize(ps._wFrameCount);
    for (WORD n = 0; n < ps._wFrameCount; n++) {
        std::fread(ps._vecFrameSize[n], sizeof(float), 1, fp);
    }
    ps._vecFrameAngle.setsize(ps._wFrameCount);
    for (WORD n = 0; n < ps._wFrameCount; n++) {
        std::fread(ps._vecFrameAngle[n], sizeof(D3DXVECTOR3), 1, fp);
    }
    ps._vecFrameColor.setsize(ps._wFrameCount);
    for (WORD n = 0; n < ps._wFrameCount; n++) {
        std::fread(ps._vecFrameColor[n], sizeof(D3DXCOLOR), 1, fp);
    }
    std::fread(&ps._bBillBoard, sizeof(bool), 1, fp);

    std::fread(&tm, sizeof(int), 1, fp);
    ps._eSrcBlend = (D3DBLEND)tm;
    std::fread(&tm, sizeof(int), 1, fp);
    ps._eDestBlend = (D3DBLEND)tm;
    std::fread(&tm, sizeof(int), 1, fp);
    ps._eMinFilter = (D3DTEXTUREFILTERTYPE)tm;
    std::fread(&tm, sizeof(int), 1, fp);
    ps._eMagFilter = (D3DTEXTUREFILTERTYPE)tm;

    std::fread(&ps._fLife, sizeof(float), 1, fp);
    std::fread(&ps._fVecl, sizeof(float), 1, fp);
    std::fread(&ps._vDir, sizeof(D3DXVECTOR3), 1, fp);
    std::fread(&ps._vAccel, sizeof(D3DXVECTOR3), 1, fp);
    std::fread(&ps._fStep, sizeof(float), 1, fp);

    if (dwVersion > 3) {
        std::fread(&ps._bModelRange, sizeof(bool), 1, fp);
        std::fread(t_pszName, sizeof(char), 32, fp);
    }
    if (dwVersion > 4) {
        std::fread(&ps._vOffset, sizeof(D3DXVECTOR3), 1, fp);
    }
    if (dwVersion > 5) {
        std::fread(&ps._fDelayTime, sizeof(float), 1, fp);
        std::fread(&ps._fPlayTime, sizeof(float), 1, fp);
    }
    if (dwVersion > 8) {
        SAFE_DELETE(ps._pcPath);
        bool busepsth;
        std::fread(&busepsth, sizeof(bool), 1, fp);
        if (busepsth) {
            ps._pcPath = new CEffPath;
            ps._pcPath->LoadPath(fp);
            ps._pcPath->Reset();
        }
    }
    if (dwVersion > 9) {
        std::fread(&ps.m_bShade, sizeof(bool), 1, fp);
    }
    if (dwVersion > 10) {
        std::fread(t_pszName, sizeof(char), 32, fp);
        ps.m_strHitEff = t_pszName;
    }
    if (dwVersion > 11) {
        if (ps._bModelRange) {
            std::fread(&ps._wVecNum, sizeof(WORD), 1, fp);
            ps._vecPointRange.resize(ps._wVecNum);
            std::fread(&ps._vecPointRange.front(), sizeof(D3DXVECTOR3), ps._wVecNum, fp);
        }
    }
    if (dwVersion > 12) {
        std::fread(&ps._iRoadom, sizeof(int), 1, fp);
    }
    else {
        if (ps._iType == PARTTICLE_SNOW
            || ps._iType == PARTTICLE_FIRE
            || ps._iType == PARTTICLE_DUMMY) {
            ps._iRoadom = 4;
        }
        else {
            ps._iRoadom = 2;
        }
    }
    if (dwVersion > 13) {
        std::fread(&ps._bModelDir, sizeof(bool), 1, fp);
    }
    if (dwVersion > 14) {
        std::fread(&ps._bMediay, sizeof(bool), 1, fp);
    }

    return true;
}

// --- CMPStrip (один strip внутри .par, версия >= 7) ---------------------------

bool PartCtrlLoader::SaveStrip(::CMPStrip& s, std::FILE* fp) {
    std::fwrite(&s.m_iMaxLen, sizeof(int), 1, fp);
    std::fwrite(&s._iDummy, sizeof(int), 2, fp);
    std::fwrite(&s._dwColor, sizeof(D3DXCOLOR), 1, fp);
    std::fwrite(&s._fLife, sizeof(float), 1, fp);
    std::fwrite(&s._fStep, sizeof(float), 1, fp);

    char buf[32]{};
    std::memcpy(buf, s._strTexName.data(),
                std::min<std::size_t>(s._strTexName.size(), sizeof(buf) - 1));
    std::fwrite(buf, sizeof(char), 32, fp);

    int te = (int)s._eSrcBlend;
    std::fwrite(&te, sizeof(int), 1, fp);
    te = (int)s._eDestBlend;
    std::fwrite(&te, sizeof(int), 1, fp);
    return true;
}

bool PartCtrlLoader::LoadStrip(::CMPStrip& s, std::FILE* fp, DWORD /*dwVersion*/) {
    std::fread(&s.m_iMaxLen, sizeof(int), 1, fp);
    std::fread(&s._iDummy, sizeof(int), 2, fp);
    std::fread(&s._dwColor, sizeof(D3DXCOLOR), 1, fp);
    std::fread(&s._fLife, sizeof(float), 1, fp);
    std::fread(&s._fStep, sizeof(float), 1, fp);

    char buf[32]{};
    std::fread(buf, sizeof(char), 32, fp);

    std::string_view nameView{buf};
    if (nameView.ends_with(".dds") || nameView.ends_with(".tga")) {
        s._strTexName.assign(nameView.substr(0, nameView.size() - 4));
    }
    else {
        s._strTexName = nameView;
    }
    int te;
    std::fread(&te, sizeof(int), 1, fp);
    s._eSrcBlend = (D3DBLEND)te;
    std::fread(&te, sizeof(int), 1, fp);
    s._eDestBlend = (D3DBLEND)te;
    return true;
}

// --- CChaModel (один character-model внутри .par, версия >= 8) ----------------

void PartCtrlLoader::SaveCharModel(::CChaModel& m, std::FILE* fp) {
    std::fwrite(&m._iID, sizeof(int), 1, fp);
    std::fwrite(&m._fVel, sizeof(float), 1, fp);
    std::fwrite(&m._iPlayType, sizeof(int), 1, fp);
    std::fwrite(&m._iCurPose, sizeof(int), 1, fp);

    int eblend = (int)m._eSrcBlend;
    std::fwrite(&eblend, sizeof(int), 1, fp);
    eblend = (int)m._eDestBlend;
    std::fwrite(&eblend, sizeof(int), 1, fp);
    std::fwrite(&m._dwCurColor, sizeof(D3DXCOLOR), 1, fp);
}

void PartCtrlLoader::LoadCharModel(::CChaModel& m, std::FILE* fp) {
    std::fread(&m._iID, sizeof(int), 1, fp);
    std::fread(&m._fVel, sizeof(float), 1, fp);
    std::fread(&m._iPlayType, sizeof(int), 1, fp);
    std::fread(&m._iCurPose, sizeof(int), 1, fp);

    int eblend;
    std::fread(&eblend, sizeof(int), 1, fp);
    m._eSrcBlend = (D3DBLEND)eblend;
    std::fread(&eblend, sizeof(int), 1, fp);
    m._eDestBlend = (D3DBLEND)eblend;
    std::fread(&m._dwCurColor, sizeof(D3DXCOLOR), 1, fp);

    // Engine-binding после чтения raw-данных. Те же действия, что делал
    // CChaModel::LoadFromFile до миграции (script binding + initial pose).
    const std::string psID = std::format("{}", m._iID);
    m.LoadScript(psID);
    m.SetVel((int)(m._fVel * 1000));
    m.SetPlayType(m._iPlayType);
    m.SetCurPose(m._iCurPose);
    m.PlayPose(m._iCurPose, PLAY_PAUSE);
}

bool PartCtrlLoader::LoadCtrl(::CMPPartCtrl& ctrl, std::string_view pszName) {
    const std::string nameStr{pszName};
    std::FILE* t_pFile = std::fopen(nameStr.c_str(), "rb");
    if (t_pFile == nullptr) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] was not opened.(PartCtrlLoader::Load)", pszName);
        return false;
    }
    UniqueFile fp{t_pFile};

    DWORD t_dwVersion = 0;
    std::fread(&t_dwVersion, sizeof(t_dwVersion), 1, fp.get());
    if (t_dwVersion > static_cast<DWORD>(::CMPPartCtrl::ParVersion)) {
        ToLogService("errors", LogLevel::Error,
                     "[{}][{}][{}] (PartCtrlLoader::Load)", pszName, t_dwVersion,
                     static_cast<int>(::CMPPartCtrl::ParVersion));
        return false;
    }
    if (t_dwVersion < 2) {
        ToLogService("errors", LogLevel::Error,
                     "[{}][{}][{}] (PartCtrlLoader::Load)", pszName, t_dwVersion, 2);
        return false;
    }

    char pszPartName[32];
    std::fread(pszPartName, sizeof(char), 32, fp.get());
    ctrl.m_strName = pszPartName;

    std::fread(&ctrl.m_iPartNum, sizeof(int), 1, fp.get());
#ifdef USE_GAME
    ctrl.m_vecPartSys.resize(ctrl.m_iPartNum);
#endif
    ctrl.m_vecPartSys.setsize(ctrl.m_iPartNum);

    if (t_dwVersion >= 3) {
        std::fread(&ctrl.m_fLength, sizeof(float), 1, fp.get());
    }
    else {
        ctrl.m_fLength = 0;
    }
    for (int n = 0; n < ctrl.m_iPartNum; ++n) {
        if (!LoadPartSys(*ctrl.m_vecPartSys[n], fp.get(), t_dwVersion)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}][{}].(PartCtrlLoader::Load)", pszName, n);
            return false;
        }
        if (t_dwVersion < 6) {
            ctrl.m_vecPartSys[n]->SetPlayTime(ctrl.m_fLength);
        }
    }
    if (t_dwVersion >= 7) {
        SAFE_DELETE_ARRAY(ctrl.m_pcStrip);

        std::fread(&ctrl.m_iStripNum, sizeof(int), 1, fp.get());

        ctrl.m_pcStrip = new ::CMPStrip[ctrl.m_iStripNum];

        for (int n = 0; n < ctrl.m_iStripNum; ++n) {
            if (!LoadStrip(ctrl.m_pcStrip[n], fp.get(), t_dwVersion)) {
                ToLogService("errors", LogLevel::Error,
                             "[{}][{}]Strip.(PartCtrlLoader::Load)", pszName, n);
                delete[] ctrl.m_pcStrip;
                ctrl.m_pcStrip = nullptr;
                return false;
            }
        }
    }
    if (t_dwVersion >= 8) {
        std::fread(&ctrl.m_iModelNum, sizeof(int), 1, fp.get());
        ctrl.m_vecModel.resize(ctrl.m_iModelNum);
        for (int n = 0; n < ctrl.m_iModelNum; ++n) {
            ctrl.m_vecModel[n] = new ::CChaModel;
            LoadCharModel(*ctrl.m_vecModel[n], fp.get());
        }
    }
    return true;
}

bool PartCtrlLoader::SaveCtrl(::CMPPartCtrl& ctrl, std::string_view pszName) {
    const std::string nameStr{pszName};
    std::FILE* t_pFile = std::fopen(nameStr.c_str(), "wb");
    if (t_pFile == nullptr) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] was not opened.(PartCtrlLoader::Save)", pszName);
        return false;
    }
    UniqueFile fp{t_pFile};

    DWORD t_dwVersion = static_cast<DWORD>(::CMPPartCtrl::ParVersion);
    std::fwrite(&t_dwVersion, sizeof(t_dwVersion), 1, fp.get());

    char pszPartName[32]{};
    std::memcpy(pszPartName, ctrl.m_strName.data(),
                std::min<std::size_t>(ctrl.m_strName.size(), sizeof(pszPartName) - 1));
    std::fwrite(pszPartName, sizeof(char), 32, fp.get());

    std::fwrite(&ctrl.m_iPartNum, sizeof(int), 1, fp.get());
    std::fwrite(&ctrl.m_fLength, sizeof(float), 1, fp.get());

    for (int n = 0; n < ctrl.m_iPartNum; ++n) {
        SavePartSys(*ctrl.m_vecPartSys[n], fp.get());
    }
    std::fwrite(&ctrl.m_iStripNum, sizeof(int), 1, fp.get());
    for (int n = 0; n < ctrl.m_iStripNum; ++n) {
        SaveStrip(ctrl.m_pcStrip[n], fp.get());
    }
    std::fwrite(&ctrl.m_iModelNum, sizeof(int), 1, fp.get());
    for (int n = 0; n < ctrl.m_iModelNum; ++n) {
        SaveCharModel(*ctrl.m_vecModel[n], fp.get());
    }
    return true;
}

LW_RESULT PartCtrlLoader::Load(::CMPPartCtrl& ctrl, std::string_view file) {
    return LoadCtrl(ctrl, file) ? LW_RET_OK : LW_RET_FAILED;
}

LW_RESULT PartCtrlLoader::LoadEx(::CMPPartCtrl& ctrl, std::string_view file,
                                  PartCtrlLoadDiagnostics& diag) {
    diag = {};

    // Сначала пытаемся прочитать только version-DWORD — для осмысленной
    // диагностики (FileOpenFailed / VersionTruncated / VersionUnknown) до
    // того, как дёрнем сам CMPPartCtrl::LoadFromFile.
    {
        UniqueFile fp{std::fopen(std::string{file}.c_str(), "rb")};
        if (!fp) {
            diag.status = PartCtrlLoadStatus::FileOpenFailed;
            diag.detail = "fopen failed";
            return LW_RET_FAILED;
        }

        DWORD version = 0;
        if (std::fread(&version, sizeof(version), 1, fp.get()) != 1) {
            diag.status = PartCtrlLoadStatus::VersionTruncated;
            diag.detail = "short read of version DWORD";
            return LW_RET_FAILED;
        }
        diag.version = version;

        // Минимум 2 — раньше CMPPartCtrl::LoadFromFile требует именно это;
        // максимум — текущий ParVersion (15 на момент написания).
        constexpr DWORD kMinParVersion = 2;
        const DWORD     kMaxParVersion = static_cast<DWORD>(::CMPPartCtrl::ParVersion);
        if (version < kMinParVersion || version > kMaxParVersion) {
            diag.status = PartCtrlLoadStatus::VersionUnknown;
            diag.detail = std::format("version=0x{:08X} (expected {}..{})",
                                      version, kMinParVersion, kMaxParVersion);
            return LW_RET_FAILED;
        }
    }

    if (!LoadCtrl(ctrl, file)) {
        diag.status = PartCtrlLoadStatus::ParseFailed;
        diag.detail = "PartCtrlLoader::Load returned false";
        return LW_RET_FAILED;
    }

    diag.status = PartCtrlLoadStatus::Ok;
    return LW_RET_OK;
}

LW_RESULT PartCtrlLoader::Save(::CMPPartCtrl& ctrl, std::string_view file) {
    return SaveCtrl(ctrl, file) ? LW_RET_OK : LW_RET_FAILED;
}

} // namespace Corsairs::Engine::Render
