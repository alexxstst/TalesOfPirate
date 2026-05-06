// Реализация PartCtrlLoader::ExportToYaml / ImportFromYaml для .par.
//
// Общий YAML-движок (DOM, парсер, эмиттер примитивов, enum-фреймворк, таблица
// D3DBLEND) — в YamlCommon.h. Здесь — enum-таблицы PARTTICLE_*, фильтры
// текстур, lwPlayPoseEnum + маппинг полей CMPPartCtrl/CMPPartSys/CEffPath/
// CMPStrip/CChaModel ↔ YAML.
//
// PartCtrlLoader друг CMPPartCtrl, CMPPartSys, CMPStrip, CChaModel, поэтому
// статические методы здесь ходят в private/protected напрямую. Поля CEffPath
// public — friend не нужен.
//
// Особенность формата .par: SavePath / LoadPath пишут и читают m_vecDist
// (объявлено как float[200]) через `sizeof(D3DXVECTOR3) = 12` байт за слот.
// Итерации перекрываются по памяти (writes m_vecDist[k..k+2]). Чтобы
// round-trip через YAML был побайтным, m_vecDist сериализуется как массив
// (frameCount + 1) float'ов — этого достаточно, чтобы Save reproduce те же
// 12*(frameCount-1) байт за счёт перекрывающихся read'ов.

#include "AssetLoaders.h"

// MPModelEff.h тянет I_Effect.h + MPSceneItem.h + MPCharacter.h, нужные для
// определения CEffPath, MPMap, CMPShadeCtrl и т.п. до парсинга MPParticleSys.h.
// EffectLoaders.cpp подключает в том же порядке.
#include "MPModelEff.h"
#include "MPParticleCtrl.h"

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
// PARTTICLE_* — particle system class
// =============================================================================

constexpr y::EnumName kPartTypeNames[] = {
    { 1, "SNOW"},          // ambient drop particles (snow/rain/leaves)
    { 2, "FIRE"},          // fire / heat-shimmer
    { 3, "BLAST"},         // omnidirectional explosion
    { 4, "RIPPLE"},        // ground ripple
    { 5, "MODEL"},         // particles emit a 3D model
    { 6, "STRIP"},         // ribbon/strip trail
    { 7, "WIND"},          // directional wind/dust
    { 8, "ARRAW"},         // arrow/projectile (sic: "ARRAW")
    { 9, "ROUND"},         // circular sweep
    {10, "BLAST2"},        // alt explosion variant
    {11, "BLAST3"},        // alt explosion variant
    {12, "SHRINK"},        // shrinking pieces
    {13, "SHADE"},         // shaded/translucent area
    {14, "RANGE"},         // bounded volume burst
    {15, "RANGE2"},        // alt range variant
    {16, "DUMMY"},         // attached to dummy bones
    {17, "LINE_SINGLE"},   // single line between two dummies
    {18, "LINE_ROUND"},    // looped line ring
};

[[nodiscard]] std::string PartTypeToYaml(int t) {
    return y::EnumToYaml(t, kPartTypeNames);
}

[[nodiscard]] int PartTypeFromYaml(const y::YamlNode& n) {
    if (n.kind != y::YamlNode::Scalar) {
        return 0;
    }
    return y::EnumFromYaml(n.scalar, kPartTypeNames, 0);
}

[[nodiscard]] std::string_view PartTypeChoicesComment() {
    static const std::string s = y::MakeChoicesComment(kPartTypeNames);
    return s;
}

// =============================================================================
// D3DTEXTUREFILTERTYPE — DX9 sampler filter modes
// =============================================================================

constexpr y::EnumName kFilterNames[] = {
    {0, "NONE"},             // disable filtering
    {1, "POINT"},            // nearest sample
    {2, "LINEAR"},           // bilinear
    {3, "ANISOTROPIC"},      // anisotropic
    {4, "FLATCUBIC"},        // 4-sample cubic
    {5, "GAUSSIANCUBIC"},    // 4-sample cubic (Gaussian kernel)
    {6, "PYRAMIDALQUAD"},    // 4-sample pyramidal
    {7, "GAUSSIANQUAD"},     // 4-sample Gaussian quad
    {8, "CONVOLUTIONMONO"},  // mono-convolution (DX9.0c)
};

[[nodiscard]] std::string FilterToYaml(D3DTEXTUREFILTERTYPE f) {
    return y::EnumToYaml(static_cast<int>(f), kFilterNames);
}

[[nodiscard]] D3DTEXTUREFILTERTYPE FilterFromYaml(const y::YamlNode& n) {
    if (n.kind != y::YamlNode::Scalar) {
        return D3DTEXF_NONE;
    }
    return static_cast<D3DTEXTUREFILTERTYPE>(
        y::EnumFromYaml(n.scalar, kFilterNames, static_cast<int>(D3DTEXF_NONE)));
}

[[nodiscard]] std::string_view FilterChoicesComment() {
    static const std::string s = y::MakeChoicesComment(kFilterNames);
    return s;
}

// =============================================================================
// lwPlayPoseEnum — character animation playback mode
// =============================================================================

constexpr y::EnumName kPlayPoseNames[] = {
    {0, "INVALID"},       // not initialized
    {1, "ONCE"},          // play once and stop on last frame
    {2, "LOOP"},          // play and loop
    {3, "FRAME"},         // hold a single frame
    {4, "ONCE_SMOOTH"},   // ONCE with cross-fade-in
    {5, "LOOP_SMOOTH"},   // LOOP with cross-fade-in
    {6, "PAUSE"},         // freeze at current frame
    {7, "CONTINUE"},      // resume from pause
};

[[nodiscard]] std::string PlayPoseToYaml(int p) {
    return y::EnumToYaml(p, kPlayPoseNames);
}

[[nodiscard]] int PlayPoseFromYaml(const y::YamlNode& n) {
    if (n.kind != y::YamlNode::Scalar) {
        return 0;
    }
    return y::EnumFromYaml(n.scalar, kPlayPoseNames, 0);
}

[[nodiscard]] std::string_view PlayPoseChoicesComment() {
    static const std::string s = y::MakeChoicesComment(kPlayPoseNames);
    return s;
}

// =============================================================================
// CEffPath emit / parse
// =============================================================================

void EmitPath(std::ostringstream& out, const ::CEffPath* path, int indent) {
    const std::string pad(indent, ' ');
    if (!path) {
        out << pad << "null: true  # path absent — bool flag in .par is false\n";
        return;
    }
    out << pad << "frameCount: " << path->m_iFrameCount << '\n';
    out << pad << "vel: "        << y::FmtFloat(path->m_fVel) << '\n';

    out << pad << "vecPath:";
    if (path->m_iFrameCount == 0) {
        out << " []\n";
    }
    else {
        out << '\n';
        for (int n = 0; n < path->m_iFrameCount; ++n) {
            out << pad << "  - " << y::FmtVec3(path->m_vecPath[n]) << '\n';
        }
    }

    const int iter = path->m_iFrameCount - 1;
    out << pad << "vecDir:";
    if (iter <= 0) {
        out << " []\n";
    }
    else {
        out << '\n';
        for (int n = 0; n < iter; ++n) {
            out << pad << "  - " << y::FmtVec3(path->m_vecDir[n]) << '\n';
        }
    }

    // m_vecDist is float[200], but Save writes sizeof(vec3)=12 bytes per iter
    // starting at &m_vecDist[k]. Iterations overlap; final byte stream covers
    // m_vecDist[0..iter+1] (= frameCount floats). Persist exactly that range
    // to make Save reproduce identical bytes.
    out << pad << "vecDistFloats: [";
    if (path->m_iFrameCount > 0) {
        for (int n = 0; n <= iter + 1 && n < 200; ++n) {
            if (n) { out << ", "; }
            out << y::FmtFloat(path->m_vecDist[n]);
        }
    }
    out << "]\n";
}

[[nodiscard]] ::CEffPath* ParsePath(const y::YamlNode& node) {
    if (node.kind != y::YamlNode::Mapping) {
        return nullptr;
    }
    if (auto* x = node.Find("null"); x && y::ParseBool(*x)) {
        return nullptr;
    }
    auto* p = new ::CEffPath{};
    if (auto* x = node.Find("frameCount")) p->m_iFrameCount = y::ParseInt(*x);
    if (auto* x = node.Find("vel"))        p->m_fVel        = y::ParseFloat(*x);

    if (auto* x = node.Find("vecPath"); x && x->kind == y::YamlNode::Sequence) {
        for (std::size_t n = 0;
             n < x->sequence.size() && n < 200 &&
             static_cast<int>(n) < p->m_iFrameCount;
             ++n) {
            p->m_vecPath[n] = y::ParseVec3(x->sequence[n]);
        }
    }

    const int iter = p->m_iFrameCount - 1;
    if (auto* x = node.Find("vecDir"); x && x->kind == y::YamlNode::Sequence) {
        for (std::size_t n = 0;
             n < x->sequence.size() && n < 200 &&
             static_cast<int>(n) < iter;
             ++n) {
            p->m_vecDir[n] = y::ParseVec3(x->sequence[n]);
        }
    }

    if (auto* x = node.Find("vecDistFloats"); x && x->kind == y::YamlNode::Sequence) {
        for (std::size_t n = 0; n < x->sequence.size() && n < 200; ++n) {
            p->m_vecDist[n] = y::ParseFloat(x->sequence[n]);
        }
    }

    return p;
}

} // namespace

// =============================================================================
// PartCtrlLoader::ExportToYaml
// =============================================================================

LW_RESULT PartCtrlLoader::ExportToYaml(const ::CMPPartCtrl& ctrl,
                                        std::string_view file) {
    std::ostringstream out;

    // ctrl is not modified, but iteration over S_BVECTOR/std::vector requires
    // non-const accessors (S_BVECTOR::operator[] is non-const). Cast away.
    auto& c = const_cast<::CMPPartCtrl&>(ctrl);

    out << "version: " << static_cast<int>(::CMPPartCtrl::ParVersion) << '\n';
    out << "name: "    << y::QuoteString(c.m_strName) << '\n';
    out << "length: "  << y::FmtFloat(c.m_fLength) << '\n';

    out << "particles:";
    if (c.m_iPartNum <= 0) {
        out << " []\n";
    }
    else {
        out << '\n';
        for (int i = 0; i < c.m_iPartNum; ++i) {
            ::CMPPartSys* psPtr = c.m_vecPartSys[i];
            if (!psPtr) {
                out << "  - {}\n";
                continue;
            }
            ::CMPPartSys& ps = *psPtr;
            const WORD fc = ps._wFrameCount;

            out << "  - type: "       << PartTypeToYaml(ps._iType)
                                      << "  " << PartTypeChoicesComment() << '\n';
            out << "    partName: "   << y::QuoteString(ps._strPartName) << '\n';
            out << "    parNum: "     << ps._iParNum << '\n';
            out << "    texName: "    << y::QuoteString(ps._strTexName) << '\n';
            out << "    modelName: "  << y::QuoteString(ps._strModelName) << '\n';
            out << "    range: ["     << y::FmtFloat(ps._fRange[0]) << ", "
                                      << y::FmtFloat(ps._fRange[1]) << ", "
                                      << y::FmtFloat(ps._fRange[2]) << "]\n";
            out << "    frameCount: " << fc << '\n';

            out << "    frameSizes: [";
            for (WORD n = 0; n < fc; ++n) {
                if (n) { out << ", "; }
                out << y::FmtFloat(*ps._vecFrameSize[n]);
            }
            out << "]\n";

            out << "    frameAngles:";
            if (fc == 0) {
                out << " []\n";
            }
            else {
                out << '\n';
                for (WORD n = 0; n < fc; ++n) {
                    out << "      - " << y::FmtVec3(*ps._vecFrameAngle[n]) << '\n';
                }
            }

            out << "    frameColors:";
            if (fc == 0) {
                out << " []\n";
            }
            else {
                out << '\n';
                for (WORD n = 0; n < fc; ++n) {
                    out << "      - " << y::FmtColor(*ps._vecFrameColor[n]) << '\n';
                }
            }

            out << "    billBoard: " << y::FmtBool(ps._bBillBoard) << '\n';
            out << "    srcBlend: "  << y::BlendToYaml(ps._eSrcBlend)
                                     << "  " << y::BlendChoicesComment() << '\n';
            out << "    destBlend: " << y::BlendToYaml(ps._eDestBlend)
                                     << "  " << y::BlendChoicesComment() << '\n';
            out << "    minFilter: " << FilterToYaml(ps._eMinFilter)
                                     << "  " << FilterChoicesComment() << '\n';
            out << "    magFilter: " << FilterToYaml(ps._eMagFilter)
                                     << "  " << FilterChoicesComment() << '\n';

            out << "    life: "      << y::FmtFloat(ps._fLife) << '\n';
            out << "    vecl: "      << y::FmtFloat(ps._fVecl) << '\n';
            out << "    dir: "       << y::FmtVec3(ps._vDir) << '\n';
            out << "    accel: "     << y::FmtVec3(ps._vAccel) << '\n';
            out << "    step: "      << y::FmtFloat(ps._fStep) << '\n';

            out << "    modelRange: "  << y::FmtBool(ps._bModelRange) << '\n';
            out << "    virualModel: " << y::QuoteString(ps._strVirualModel) << '\n';
            out << "    offset: "      << y::FmtVec3(ps._vOffset) << '\n';
            out << "    delayTime: "   << y::FmtFloat(ps._fDelayTime) << '\n';
            out << "    playTime: "    << y::FmtFloat(ps._fPlayTime) << '\n';

            out << "    path:";
            if (!ps._pcPath) {
                out << " null\n";
            }
            else {
                out << '\n';
                EmitPath(out, ps._pcPath, 6);
            }

            out << "    shade: "  << y::FmtBool(ps.m_bShade) << '\n';
            out << "    hitEff: " << y::QuoteString(ps.m_strHitEff) << '\n';

            out << "    pointRange:";
            if (!ps._bModelRange || ps._wVecNum == 0) {
                out << " []\n";
            }
            else {
                out << '\n';
                for (WORD n = 0; n < ps._wVecNum; ++n) {
                    out << "      - " << y::FmtVec3(ps._vecPointRange[n]) << '\n';
                }
            }

            out << "    roadom: "   << ps._iRoadom << '\n';
            out << "    modelDir: " << y::FmtBool(ps._bModelDir) << '\n';
            out << "    mediay: "   << y::FmtBool(ps._bMediay) << '\n';
        }
    }

    out << "strips:";
    if (c.m_iStripNum <= 0) {
        out << " []\n";
    }
    else {
        out << '\n';
        for (int i = 0; i < c.m_iStripNum; ++i) {
            ::CMPStrip& s = c.m_pcStrip[i];
            out << "  - maxLen: "    << s.m_iMaxLen << '\n';
            out << "    dummy: ["    << s._iDummy[0] << ", " << s._iDummy[1] << "]\n";
            out << "    color: "     << y::FmtColor(s._dwColor) << '\n';
            out << "    life: "      << y::FmtFloat(s._fLife) << '\n';
            out << "    step: "      << y::FmtFloat(s._fStep) << '\n';
            out << "    texName: "   << y::QuoteString(s._strTexName) << '\n';
            out << "    srcBlend: "  << y::BlendToYaml(s._eSrcBlend)
                                     << "  " << y::BlendChoicesComment() << '\n';
            out << "    destBlend: " << y::BlendToYaml(s._eDestBlend)
                                     << "  " << y::BlendChoicesComment() << '\n';
        }
    }

    out << "charModels:";
    if (c.m_iModelNum <= 0) {
        out << " []\n";
    }
    else {
        out << '\n';
        for (int i = 0; i < c.m_iModelNum; ++i) {
            ::CChaModel* mPtr = c.m_vecModel[i];
            if (!mPtr) {
                out << "  - {}\n";
                continue;
            }
            ::CChaModel& m = *mPtr;
            out << "  - id: "        << m._iID << '\n';
            out << "    vel: "       << y::FmtFloat(m._fVel) << '\n';
            out << "    playType: "  << PlayPoseToYaml(m._iPlayType)
                                     << "  " << PlayPoseChoicesComment() << '\n';
            out << "    curPose: "   << m._iCurPose << '\n';
            out << "    srcBlend: "  << y::BlendToYaml(m._eSrcBlend)
                                     << "  " << y::BlendChoicesComment() << '\n';
            out << "    destBlend: " << y::BlendToYaml(m._eDestBlend)
                                     << "  " << y::BlendChoicesComment() << '\n';
            out << "    curColor: "  << y::FmtColor(m._dwCurColor) << '\n';
        }
    }

    std::ofstream f{std::string{file}, std::ios::binary};
    if (!f) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] open for write failed (PartCtrlLoader::ExportToYaml)", file);
        return LW_RET_FAILED;
    }
    const std::string buf = out.str();
    f.write(buf.data(), static_cast<std::streamsize>(buf.size()));
    if (!f) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] write failed (PartCtrlLoader::ExportToYaml)", file);
        return LW_RET_FAILED;
    }
    return LW_RET_OK;
}

// =============================================================================
// PartCtrlLoader::ImportFromYaml
// =============================================================================

LW_RESULT PartCtrlLoader::ImportFromYaml(::CMPPartCtrl& ctrl,
                                          std::string_view file) {
    std::ifstream f{std::string{file}, std::ios::binary};
    if (!f) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] open for read failed (PartCtrlLoader::ImportFromYaml)", file);
        return LW_RET_FAILED;
    }
    std::string text{std::istreambuf_iterator<char>(f),
                     std::istreambuf_iterator<char>()};

    auto lines = y::Tokenize(text);
    y::DomParser parser{std::move(lines)};
    y::YamlNode root = parser.ParseRoot();

    if (root.kind != y::YamlNode::Mapping) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] yaml root is not a mapping (PartCtrlLoader::ImportFromYaml)", file);
        return LW_RET_FAILED;
    }

    if (auto* x = root.Find("name"))   ctrl.m_strName = y::ParseString(*x);
    if (auto* x = root.Find("length")) ctrl.m_fLength = y::ParseFloat(*x);

    auto* pArr = root.Find("particles");
    if (pArr && pArr->kind == y::YamlNode::Sequence) {
        ctrl.m_iPartNum = static_cast<int>(pArr->sequence.size());
#ifdef USE_GAME
        ctrl.m_vecPartSys.resize(ctrl.m_iPartNum);
#endif
        ctrl.m_vecPartSys.setsize(ctrl.m_iPartNum);

        for (int i = 0; i < ctrl.m_iPartNum; ++i) {
            const y::YamlNode& pn = pArr->sequence[i];
            if (pn.kind != y::YamlNode::Mapping) { continue; }
            ::CMPPartSys& ps = *ctrl.m_vecPartSys[i];

            if (auto* x = pn.Find("type"))       ps._iType        = PartTypeFromYaml(*x);
            if (auto* x = pn.Find("partName"))   ps._strPartName  = y::ParseString(*x);
            if (auto* x = pn.Find("parNum"))     ps._iParNum      = y::ParseInt(*x);
            if (auto* x = pn.Find("texName"))    ps._strTexName   = y::ParseString(*x);
            if (auto* x = pn.Find("modelName"))  ps._strModelName = y::ParseString(*x);
            if (auto* x = pn.Find("range"); x && x->kind == y::YamlNode::Sequence
                                            && x->sequence.size() >= 3) {
                ps._fRange[0] = y::ParseFloat(x->sequence[0]);
                ps._fRange[1] = y::ParseFloat(x->sequence[1]);
                ps._fRange[2] = y::ParseFloat(x->sequence[2]);
            }
            if (auto* x = pn.Find("frameCount")) ps._wFrameCount = static_cast<WORD>(y::ParseInt(*x));

            const WORD fc = ps._wFrameCount;
            ps._vecFrameSize.setsize(fc);
            ps._vecFrameAngle.setsize(fc);
            ps._vecFrameColor.setsize(fc);

            if (auto* x = pn.Find("frameSizes"); x && x->kind == y::YamlNode::Sequence) {
                for (WORD n = 0; n < fc && n < x->sequence.size(); ++n) {
                    *ps._vecFrameSize[n] = y::ParseFloat(x->sequence[n]);
                }
            }
            if (auto* x = pn.Find("frameAngles"); x && x->kind == y::YamlNode::Sequence) {
                for (WORD n = 0; n < fc && n < x->sequence.size(); ++n) {
                    *ps._vecFrameAngle[n] = y::ParseVec3(x->sequence[n]);
                }
            }
            if (auto* x = pn.Find("frameColors"); x && x->kind == y::YamlNode::Sequence) {
                for (WORD n = 0; n < fc && n < x->sequence.size(); ++n) {
                    *ps._vecFrameColor[n] = y::ParseColor(x->sequence[n]);
                }
            }

            if (auto* x = pn.Find("billBoard")) ps._bBillBoard = y::ParseBool(*x);
            if (auto* x = pn.Find("srcBlend"))  ps._eSrcBlend  = y::BlendFromYaml(*x);
            if (auto* x = pn.Find("destBlend")) ps._eDestBlend = y::BlendFromYaml(*x);
            if (auto* x = pn.Find("minFilter")) ps._eMinFilter = FilterFromYaml(*x);
            if (auto* x = pn.Find("magFilter")) ps._eMagFilter = FilterFromYaml(*x);

            if (auto* x = pn.Find("life"))      ps._fLife    = y::ParseFloat(*x);
            if (auto* x = pn.Find("vecl"))      ps._fVecl    = y::ParseFloat(*x);
            if (auto* x = pn.Find("dir"))       ps._vDir     = y::ParseVec3(*x);
            if (auto* x = pn.Find("accel"))     ps._vAccel   = y::ParseVec3(*x);
            if (auto* x = pn.Find("step"))      ps._fStep    = y::ParseFloat(*x);

            if (auto* x = pn.Find("modelRange"))  ps._bModelRange    = y::ParseBool(*x);
            if (auto* x = pn.Find("virualModel")) ps._strVirualModel = y::ParseString(*x);
            if (auto* x = pn.Find("offset"))      ps._vOffset        = y::ParseVec3(*x);
            if (auto* x = pn.Find("delayTime"))   ps._fDelayTime     = y::ParseFloat(*x);
            if (auto* x = pn.Find("playTime"))    ps._fPlayTime      = y::ParseFloat(*x);

            // Path: explicit `null` scalar OR mapping with the path fields.
            ps._pcPath = nullptr;
            if (auto* x = pn.Find("path"); x) {
                if (x->kind == y::YamlNode::Mapping) {
                    ps._pcPath = ParsePath(*x);
                }
                // Scalar "null" → keep ps._pcPath = nullptr.
            }

            if (auto* x = pn.Find("shade"))  ps.m_bShade   = y::ParseBool(*x);
            if (auto* x = pn.Find("hitEff")) ps.m_strHitEff = y::ParseString(*x);

            if (auto* x = pn.Find("pointRange"); x && x->kind == y::YamlNode::Sequence) {
                ps._wVecNum = static_cast<WORD>(x->sequence.size());
                ps._vecPointRange.assign(ps._wVecNum, D3DXVECTOR3{});
                for (std::size_t n = 0; n < x->sequence.size(); ++n) {
                    ps._vecPointRange[n] = y::ParseVec3(x->sequence[n]);
                }
            }
            else {
                ps._wVecNum = 0;
                ps._vecPointRange.clear();
            }

            if (auto* x = pn.Find("roadom"))   ps._iRoadom   = y::ParseInt(*x);
            if (auto* x = pn.Find("modelDir")) ps._bModelDir = y::ParseBool(*x);
            if (auto* x = pn.Find("mediay"))   ps._bMediay   = y::ParseBool(*x);
        }
    }

    if (ctrl.m_pcStrip) {
        delete[] ctrl.m_pcStrip;
        ctrl.m_pcStrip = nullptr;
    }
    auto* sArr = root.Find("strips");
    if (sArr && sArr->kind == y::YamlNode::Sequence) {
        ctrl.m_iStripNum = static_cast<int>(sArr->sequence.size());
        if (ctrl.m_iStripNum > 0) {
            ctrl.m_pcStrip = new ::CMPStrip[ctrl.m_iStripNum];
            for (int i = 0; i < ctrl.m_iStripNum; ++i) {
                const y::YamlNode& sn = sArr->sequence[i];
                if (sn.kind != y::YamlNode::Mapping) { continue; }
                ::CMPStrip& s = ctrl.m_pcStrip[i];

                if (auto* x = sn.Find("maxLen")) s.m_iMaxLen = y::ParseInt(*x);
                if (auto* x = sn.Find("dummy"); x && x->kind == y::YamlNode::Sequence
                                                && x->sequence.size() >= 2) {
                    s._iDummy[0] = y::ParseInt(x->sequence[0]);
                    s._iDummy[1] = y::ParseInt(x->sequence[1]);
                }
                if (auto* x = sn.Find("color"))     s._dwColor    = y::ParseColor(*x);
                if (auto* x = sn.Find("life"))      s._fLife      = y::ParseFloat(*x);
                if (auto* x = sn.Find("step"))      s._fStep      = y::ParseFloat(*x);
                if (auto* x = sn.Find("texName"))   s._strTexName = y::ParseString(*x);
                if (auto* x = sn.Find("srcBlend"))  s._eSrcBlend  = y::BlendFromYaml(*x);
                if (auto* x = sn.Find("destBlend")) s._eDestBlend = y::BlendFromYaml(*x);
            }
        }
    }
    else {
        ctrl.m_iStripNum = 0;
    }

    for (auto* p : ctrl.m_vecModel) {
        delete p;
    }
    ctrl.m_vecModel.clear();

    auto* mArr = root.Find("charModels");
    if (mArr && mArr->kind == y::YamlNode::Sequence) {
        ctrl.m_iModelNum = static_cast<int>(mArr->sequence.size());
        ctrl.m_vecModel.resize(ctrl.m_iModelNum, nullptr);
        for (int i = 0; i < ctrl.m_iModelNum; ++i) {
            const y::YamlNode& mn = mArr->sequence[i];
            if (mn.kind != y::YamlNode::Mapping) { continue; }
            auto* model = new ::CChaModel{};
            ctrl.m_vecModel[i] = model;
            ::CChaModel& m = *model;

            if (auto* x = mn.Find("id"))        m._iID        = y::ParseInt(*x);
            if (auto* x = mn.Find("vel"))       m._fVel       = y::ParseFloat(*x);
            if (auto* x = mn.Find("playType"))  m._iPlayType  = PlayPoseFromYaml(*x);
            if (auto* x = mn.Find("curPose"))   m._iCurPose   = y::ParseInt(*x);
            if (auto* x = mn.Find("srcBlend"))  m._eSrcBlend  = y::BlendFromYaml(*x);
            if (auto* x = mn.Find("destBlend")) m._eDestBlend = y::BlendFromYaml(*x);
            if (auto* x = mn.Find("curColor"))  m._dwCurColor = y::ParseColor(*x);
        }
    }
    else {
        ctrl.m_iModelNum = 0;
    }

    return LW_RET_OK;
}

} // namespace Corsairs::Engine::Render
