// Реализация EffectLoader::ExportToYaml / ImportFromYaml.
//
// Общий YAML-движок (DOM, парсер, эмиттер примитивов, enum-фреймворк, таблица
// D3DBLEND) — в YamlCommon.h. Здесь — только enum-таблица EFFECT_TYPE и
// маппинг полей EffectFileInfo / I_Effect ↔ YAML.
//
// EffectLoader друг I_Effect, поэтому методы здесь читают/пишут все
// private/protected поля I_Effect напрямую.

#include "AssetLoaders.h"

#include "MPModelEff.h"  // EffectFileInfo, EffParameter
#include "I_Effect.h"    // I_Effect, EFFECT_TYPE, TEXCOORD, ModelParam

#include "YamlCommon.h"

#include "logutil.h"

#include <cstdint>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace Corsairs::Engine::Render {

namespace y = ::Corsairs::Engine::Render::yaml;

namespace {

// =============================================================================
// EFFECT_TYPE table
// =============================================================================

constexpr y::EnumName kEffectTypeNames[] = {
    {0, "NONE"},          // unused / empty effect slot
    {1, "FRAMETEX"},      // sprite-sheet frame textures (m_CTexFrame)
    {2, "MODELUV"},       // animated UV on a static model
    {3, "MODELTEXTURE"},  // frame-texture cycling on a model
    {4, "MODEL"},         // static / moving model with no UV/tex anim
};

[[nodiscard]] std::string EffectTypeToYaml(EFFECT_TYPE t) {
    return y::EnumToYaml(static_cast<int>(t), kEffectTypeNames);
}

[[nodiscard]] EFFECT_TYPE EffectTypeFromYaml(const y::YamlNode& n) {
    if (n.kind != y::YamlNode::Scalar) {
        return EFFECT_NONE;
    }
    return static_cast<EFFECT_TYPE>(
        y::EnumFromYaml(n.scalar, kEffectTypeNames, static_cast<int>(EFFECT_NONE)));
}

[[nodiscard]] std::string_view EffectTypeChoicesComment() {
    static const std::string s = y::MakeChoicesComment(kEffectTypeNames);
    return s;
}

} // namespace

// =============================================================================
// EffectLoader::ExportToYaml
// =============================================================================

LW_RESULT EffectLoader::ExportToYaml(const ::EffectFileInfo& info,
                                      std::string_view file) {
    std::ostringstream out;

    out << "version: " << info.version << '\n';

    out << "param:\n";
    out << "  idxTech: "   << info.param.m_iIdxTech << '\n';
    out << "  usePath: "   << y::FmtBool(info.param.m_bUsePath) << '\n';
    out << "  pathName: "  << y::QuoteString(info.param.m_szPathName) << '\n';
    out << "  useSound: "  << y::FmtBool(info.param.m_bUseSound) << '\n';
    out << "  soundName: " << y::QuoteString(info.param.m_szSoundName) << '\n';
    out << "  rotating: "  << y::FmtBool(info.param.m_bRotating) << '\n';
    out << "  verRota: "   << y::FmtVec3(info.param.m_SVerRota) << '\n';
    out << "  rotaVel: "   << y::FmtFloat(info.param.m_fRotaVel) << '\n';

    out << "effects:";
    if (info.effects.empty()) {
        out << " []\n";
    }
    else {
        out << '\n';
        for (const ::I_Effect& e : info.effects) {
            const WORD fc = e._wFrameCount;

            out << "  - name: "      << y::QuoteString(e.m_strEffectName) << '\n';
            out << "    type: "      << EffectTypeToYaml(e._eEffectType)
                                     << "  " << EffectTypeChoicesComment() << '\n';
            out << "    srcBlend: "  << y::BlendToYaml(e._eSrcBlend)
                                     << "  " << y::BlendChoicesComment() << '\n';
            out << "    destBlend: " << y::BlendToYaml(e._eDestBlend)
                                     << "  " << y::BlendChoicesComment() << '\n';
            out << "    length: "     << y::FmtFloat(e._fLength) << '\n';
            out << "    frameCount: " << fc << '\n';

            out << "    frameTimes: [";
            for (WORD n = 0; n < fc; ++n) {
                if (n) { out << ", "; }
                out << y::FmtFloat(e._vecFrameTime[n]);
            }
            out << "]\n";

            auto emitVec3List = [&](std::string_view key,
                                    const std::vector<D3DXVECTOR3>& v) {
                out << "    " << key << ":";
                if (fc == 0) { out << " []\n"; return; }
                out << '\n';
                for (WORD n = 0; n < fc; ++n) {
                    out << "      - " << y::FmtVec3(v[n]) << '\n';
                }
            };
            emitVec3List("frameSizes",  e._vecFrameSize);
            emitVec3List("frameAngles", e._vecFrameAngle);
            emitVec3List("framePos",    e._vecFramePos);

            out << "    frameColors:";
            if (fc == 0) {
                out << " []\n";
            }
            else {
                out << '\n';
                for (WORD n = 0; n < fc; ++n) {
                    out << "      - " << y::FmtColor(e._vecFrameColor[n]) << '\n';
                }
            }

            out << "    texCoord:\n";
            out << "      verCount: "   << e.m_CTexCoordlist.m_wVerCount << '\n';
            out << "      coordCount: " << e.m_CTexCoordlist.m_wCoordCount << '\n';
            out << "      frameTime: "  << y::FmtFloat(e.m_CTexCoordlist.m_fFrameTime) << '\n';
            out << "      coords:";
            if (e.m_CTexCoordlist.m_wCoordCount == 0) {
                out << " []\n";
            }
            else {
                out << '\n';
                for (WORD n = 0; n < e.m_CTexCoordlist.m_wCoordCount; ++n) {
                    out << "        - [";
                    for (WORD k = 0; k < e.m_CTexCoordlist.m_wVerCount; ++k) {
                        if (k) { out << ", "; }
                        out << y::FmtVec2(e.m_CTexCoordlist.m_vecCoordList[n][k]);
                    }
                    out << "]\n";
                }
            }

            out << "    texList:\n";
            out << "      texCount: "  << e.m_CTextruelist.m_wTexCount << '\n';
            out << "      frameTime: " << y::FmtFloat(e.m_CTextruelist.m_fFrameTime) << '\n';
            out << "      texName: "   << y::QuoteString(e.m_CTextruelist.m_vecTexName) << '\n';
            out << "      texs:";
            if (e.m_CTextruelist.m_wTexCount == 0) {
                out << " []\n";
            }
            else {
                out << '\n';
                for (WORD n = 0; n < e.m_CTextruelist.m_wTexCount; ++n) {
                    out << "        - [";
                    for (WORD k = 0; k < e.m_CTexCoordlist.m_wVerCount; ++k) {
                        if (k) { out << ", "; }
                        out << y::FmtVec2(e.m_CTextruelist.m_vecTexList[n][k]);
                    }
                    out << "]\n";
                }
            }

            out << "    modelName: "  << y::QuoteString(e.m_strModelName) << '\n';
            out << "    billBoard: "  << y::FmtBool(e._bBillBoard) << '\n';
            out << "    vsIndex: "    << e._iVSIndex << '\n';
            out << "    nSegments: "  << e.m_nSegments << '\n';
            out << "    rHeight: "    << y::FmtFloat(e.m_rHeight) << '\n';
            out << "    rRadius: "    << y::FmtFloat(e.m_rRadius) << '\n';
            out << "    rBotRadius: " << y::FmtFloat(e.m_rBotRadius) << '\n';

            out << "    texFrame:\n";
            out << "      texCount: "  << e.m_CTexFrame.m_wTexCount << '\n';
            out << "      frameTime: " << y::FmtFloat(e.m_CTexFrame.m_fFrameTime) << '\n';
            out << "      texNames:";
            if (e.m_CTexFrame.m_wTexCount == 0) {
                out << " []\n";
            }
            else {
                out << '\n';
                for (WORD n = 0; n < e.m_CTexFrame.m_wTexCount; ++n) {
                    out << "        - " << y::QuoteString(e.m_CTexFrame.m_vecTexName[n]) << '\n';
                }
            }

            out << "    useParam: " << e._iUseParam << '\n';
            out << "    cylinderParams:";
            if (e._iUseParam <= 0 || fc == 0) {
                out << " []\n";
            }
            else {
                out << '\n';
                for (WORD n = 0; n < fc; ++n) {
                    out << "      - segments: "  << e._CylinderParam[n].iSegments << '\n';
                    out << "        hei: "       << y::FmtFloat(e._CylinderParam[n].fHei) << '\n';
                    out << "        topRadius: " << y::FmtFloat(e._CylinderParam[n].fTopRadius) << '\n';
                    out << "        botRadius: " << y::FmtFloat(e._CylinderParam[n].fBottomRadius) << '\n';
                }
            }

            out << "    rotaLoop: "  << y::FmtBool(e._bRotaLoop) << '\n';
            out << "    vRotaLoop: " << y::FmtVec4(e._vRotaLoop) << '\n';
            out << "    alpha: "     << y::FmtBool(e._bAlpha) << '\n';
            out << "    rotaBoard: " << y::FmtBool(e._bRotaBoard) << '\n';
        }
    }

    std::ofstream f{std::string{file}, std::ios::binary};
    if (!f) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] open for write failed (EffectLoader::ExportToYaml)", file);
        return LW_RET_FAILED;
    }
    const std::string buf = out.str();
    f.write(buf.data(), static_cast<std::streamsize>(buf.size()));
    if (!f) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] write failed (EffectLoader::ExportToYaml)", file);
        return LW_RET_FAILED;
    }
    return LW_RET_OK;
}

// =============================================================================
// EffectLoader::ImportFromYaml
// =============================================================================

LW_RESULT EffectLoader::ImportFromYaml(::EffectFileInfo& info,
                                        std::string_view file) {
    info = ::EffectFileInfo{};

    std::ifstream f{std::string{file}, std::ios::binary};
    if (!f) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] open for read failed (EffectLoader::ImportFromYaml)", file);
        return LW_RET_FAILED;
    }
    std::string text{std::istreambuf_iterator<char>(f),
                     std::istreambuf_iterator<char>()};

    auto lines = y::Tokenize(text);
    y::DomParser parser{std::move(lines)};
    y::YamlNode root = parser.ParseRoot();

    if (root.kind != y::YamlNode::Mapping) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] yaml root is not a mapping (EffectLoader::ImportFromYaml)", file);
        return LW_RET_FAILED;
    }

    if (auto* n = root.Find("version")) {
        info.version = static_cast<std::uint32_t>(y::ParseInt(*n));
    }

    if (auto* p = root.Find("param"); p && p->kind == y::YamlNode::Mapping) {
        if (auto* x = p->Find("idxTech"))   info.param.m_iIdxTech    = y::ParseInt(*x);
        if (auto* x = p->Find("usePath"))   info.param.m_bUsePath    = y::ParseBool(*x);
        if (auto* x = p->Find("pathName"))  info.param.m_szPathName  = y::ParseString(*x);
        if (auto* x = p->Find("useSound"))  info.param.m_bUseSound   = y::ParseBool(*x);
        if (auto* x = p->Find("soundName")) info.param.m_szSoundName = y::ParseString(*x);
        if (auto* x = p->Find("rotating"))  info.param.m_bRotating   = y::ParseBool(*x);
        if (auto* x = p->Find("verRota"))   info.param.m_SVerRota    = y::ParseVec3(*x);
        if (auto* x = p->Find("rotaVel"))   info.param.m_fRotaVel    = y::ParseFloat(*x);
    }

    auto* arr = root.Find("effects");
    if (!arr || arr->kind != y::YamlNode::Sequence) {
        return LW_RET_OK;
    }

    info.effects.resize(arr->sequence.size());
    for (std::size_t i = 0; i < arr->sequence.size(); ++i) {
        const y::YamlNode& en = arr->sequence[i];
        if (en.kind != y::YamlNode::Mapping) { continue; }
        ::I_Effect& e = info.effects[i];

        if (auto* x = en.Find("name"))       e.m_strEffectName = y::ParseString(*x);
        if (auto* x = en.Find("type"))       e._eEffectType    = EffectTypeFromYaml(*x);
        if (auto* x = en.Find("srcBlend"))   e._eSrcBlend      = y::BlendFromYaml(*x);
        if (auto* x = en.Find("destBlend"))  e._eDestBlend     = y::BlendFromYaml(*x);
        if (auto* x = en.Find("length"))     e._fLength        = y::ParseFloat(*x);
        if (auto* x = en.Find("frameCount")) e._wFrameCount    = static_cast<WORD>(y::ParseInt(*x));

        const WORD fc = e._wFrameCount;
        e._vecFrameTime.assign(fc, 0.0f);
        if (auto* x = en.Find("frameTimes"); x && x->kind == y::YamlNode::Sequence) {
            for (WORD n = 0; n < fc && n < x->sequence.size(); ++n) {
                e._vecFrameTime[n] = y::ParseFloat(x->sequence[n]);
            }
        }

        auto loadVec3List = [&](std::string_view key, std::vector<D3DXVECTOR3>& dst) {
            dst.assign(fc, D3DXVECTOR3{0.0f, 0.0f, 0.0f});
            if (auto* x = en.Find(key); x && x->kind == y::YamlNode::Sequence) {
                for (WORD n = 0; n < fc && n < x->sequence.size(); ++n) {
                    dst[n] = y::ParseVec3(x->sequence[n]);
                }
            }
        };
        loadVec3List("frameSizes",  e._vecFrameSize);
        loadVec3List("frameAngles", e._vecFrameAngle);
        loadVec3List("framePos",    e._vecFramePos);

        e._vecFrameColor.assign(fc, D3DXCOLOR{0.0f, 0.0f, 0.0f, 0.0f});
        if (auto* x = en.Find("frameColors"); x && x->kind == y::YamlNode::Sequence) {
            for (WORD n = 0; n < fc && n < x->sequence.size(); ++n) {
                e._vecFrameColor[n] = y::ParseColor(x->sequence[n]);
            }
        }

        if (auto* tc = en.Find("texCoord"); tc && tc->kind == y::YamlNode::Mapping) {
            if (auto* x = tc->Find("verCount"))   e.m_CTexCoordlist.m_wVerCount   = static_cast<WORD>(y::ParseInt(*x));
            if (auto* x = tc->Find("coordCount")) e.m_CTexCoordlist.m_wCoordCount = static_cast<WORD>(y::ParseInt(*x));
            if (auto* x = tc->Find("frameTime"))  e.m_CTexCoordlist.m_fFrameTime  = y::ParseFloat(*x);
            const WORD vc = e.m_CTexCoordlist.m_wVerCount;
            e.m_CTexCoordlist.m_vecCoordList.assign(e.m_CTexCoordlist.m_wCoordCount, TEXCOORD{});
            if (auto* x = tc->Find("coords"); x && x->kind == y::YamlNode::Sequence) {
                for (std::size_t n = 0;
                     n < e.m_CTexCoordlist.m_wCoordCount && n < x->sequence.size();
                     ++n) {
                    const y::YamlNode& row = x->sequence[n];
                    e.m_CTexCoordlist.m_vecCoordList[n].assign(vc, D3DXVECTOR2{});
                    if (row.kind == y::YamlNode::Sequence) {
                        for (std::size_t k = 0; k < vc && k < row.sequence.size(); ++k) {
                            e.m_CTexCoordlist.m_vecCoordList[n][k] = y::ParseVec2(row.sequence[k]);
                        }
                    }
                }
            }
        }

        if (auto* tl = en.Find("texList"); tl && tl->kind == y::YamlNode::Mapping) {
            if (auto* x = tl->Find("texCount"))  e.m_CTextruelist.m_wTexCount   = static_cast<WORD>(y::ParseInt(*x));
            if (auto* x = tl->Find("frameTime")) e.m_CTextruelist.m_fFrameTime  = y::ParseFloat(*x);
            if (auto* x = tl->Find("texName"))   e.m_CTextruelist.m_vecTexName  = y::ParseString(*x);
            const WORD vc = e.m_CTexCoordlist.m_wVerCount;
            e.m_CTextruelist.m_vecTexList.assign(e.m_CTextruelist.m_wTexCount, TEXCOORD{});
            if (auto* x = tl->Find("texs"); x && x->kind == y::YamlNode::Sequence) {
                for (std::size_t n = 0;
                     n < e.m_CTextruelist.m_wTexCount && n < x->sequence.size();
                     ++n) {
                    const y::YamlNode& row = x->sequence[n];
                    e.m_CTextruelist.m_vecTexList[n].assign(vc, D3DXVECTOR2{});
                    if (row.kind == y::YamlNode::Sequence) {
                        for (std::size_t k = 0; k < vc && k < row.sequence.size(); ++k) {
                            e.m_CTextruelist.m_vecTexList[n][k] = y::ParseVec2(row.sequence[k]);
                        }
                    }
                }
            }
        }

        if (auto* x = en.Find("modelName"))  e.m_strModelName = y::ParseString(*x);
        if (auto* x = en.Find("billBoard"))  e._bBillBoard    = y::ParseBool(*x);
        if (auto* x = en.Find("vsIndex"))    e._iVSIndex      = y::ParseInt(*x);
        if (auto* x = en.Find("nSegments"))  e.m_nSegments    = y::ParseInt(*x);
        if (auto* x = en.Find("rHeight"))    e.m_rHeight      = y::ParseFloat(*x);
        if (auto* x = en.Find("rRadius"))    e.m_rRadius      = y::ParseFloat(*x);
        if (auto* x = en.Find("rBotRadius")) e.m_rBotRadius   = y::ParseFloat(*x);

        if (auto* tf = en.Find("texFrame"); tf && tf->kind == y::YamlNode::Mapping) {
            if (auto* x = tf->Find("texCount"))  e.m_CTexFrame.m_wTexCount  = static_cast<WORD>(y::ParseInt(*x));
            if (auto* x = tf->Find("frameTime")) e.m_CTexFrame.m_fFrameTime = y::ParseFloat(*x);
            e.m_CTexFrame.m_vecTexName.assign(e.m_CTexFrame.m_wTexCount, std::string{});
            e.m_CTexFrame.m_vecTexs.assign(e.m_CTexFrame.m_wTexCount, nullptr);
            if (auto* x = tf->Find("texNames"); x && x->kind == y::YamlNode::Sequence) {
                for (std::size_t n = 0;
                     n < e.m_CTexFrame.m_wTexCount && n < x->sequence.size();
                     ++n) {
                    e.m_CTexFrame.m_vecTexName[n] = y::ParseString(x->sequence[n]);
                }
            }
        }

        if (auto* x = en.Find("useParam")) e._iUseParam = y::ParseInt(*x);
        e._CylinderParam.assign(fc, ModelParam{});
        if (e._iUseParam > 0) {
            if (auto* x = en.Find("cylinderParams"); x && x->kind == y::YamlNode::Sequence) {
                for (std::size_t n = 0; n < fc && n < x->sequence.size(); ++n) {
                    const y::YamlNode& cp = x->sequence[n];
                    if (cp.kind != y::YamlNode::Mapping) { continue; }
                    if (auto* z = cp.Find("segments"))  e._CylinderParam[n].iSegments     = y::ParseInt(*z);
                    if (auto* z = cp.Find("hei"))       e._CylinderParam[n].fHei          = y::ParseFloat(*z);
                    if (auto* z = cp.Find("topRadius")) e._CylinderParam[n].fTopRadius    = y::ParseFloat(*z);
                    if (auto* z = cp.Find("botRadius")) e._CylinderParam[n].fBottomRadius = y::ParseFloat(*z);
                }
            }
        }

        if (auto* x = en.Find("rotaLoop"))  e._bRotaLoop  = y::ParseBool(*x);
        if (auto* x = en.Find("vRotaLoop")) e._vRotaLoop  = y::ParseVec4(*x);
        if (auto* x = en.Find("alpha"))     e._bAlpha     = y::ParseBool(*x);
        if (auto* x = en.Find("rotaBoard")) e._bRotaBoard = y::ParseBool(*x);
    }

    return LW_RET_OK;
}

} // namespace Corsairs::Engine::Render
