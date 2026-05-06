// Shared helpers для YAML-сериализации форматов MindPower3D (.eff, .par и т.д.).
//
// Содержит DOM-модель, минимальный indent-based парсер, эмиттер примитивов
// (float/bool/vec/color) и template-based enum mapping. Подключается из
// EffectYaml.cpp / PartCtrlYaml.cpp — каждый формат добавляет собственные
// enum-таблицы и схему верхнего уровня.
//
// Все функции — inline или template, чтобы не плодить ещё один TU и не
// разводить linker-зависимости. Реализация компилируется в любой translation
// unit, который её включает (single-definition rule выполняется через inline).

#pragma once

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "lwDirectX.h"  // D3DXVECTOR2/3/4, D3DXCOLOR, D3DBLEND

namespace Corsairs::Engine::Render::yaml {

// =============================================================================
// DOM
// =============================================================================

struct YamlNode {
    enum Kind { Null, Scalar, Sequence, Mapping };
    Kind kind{Null};
    std::string scalar;
    std::vector<YamlNode> sequence;
    std::vector<std::pair<std::string, YamlNode>> mapping;

    [[nodiscard]] const YamlNode* Find(std::string_view key) const noexcept {
        if (kind != Mapping) {
            return nullptr;
        }
        for (const auto& [k, v] : mapping) {
            if (k == key) {
                return &v;
            }
        }
        return nullptr;
    }
};

// =============================================================================
// String helpers
// =============================================================================

[[nodiscard]] inline std::string_view TrimLeft(std::string_view s) {
    while (!s.empty() && (s.front() == ' ' || s.front() == '\t')) {
        s.remove_prefix(1);
    }
    return s;
}

[[nodiscard]] inline std::string_view TrimRight(std::string_view s) {
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r')) {
        s.remove_suffix(1);
    }
    return s;
}

[[nodiscard]] inline std::string_view Trim(std::string_view s) {
    return TrimRight(TrimLeft(s));
}

// Index of the first ':' at top level (outside `"..."` and `[...]`), or npos.
[[nodiscard]] inline std::size_t FindKeyColon(std::string_view s) {
    int depth = 0;
    bool inStr = false;
    bool esc = false;
    for (std::size_t i = 0; i < s.size(); ++i) {
        const char c = s[i];
        if (esc) { esc = false; continue; }
        if (inStr) {
            if (c == '\\') { esc = true; }
            else if (c == '"') { inStr = false; }
            continue;
        }
        if (c == '"') { inStr = true; continue; }
        if (c == '[') { ++depth; }
        else if (c == ']') { --depth; }
        else if (c == ':' && depth == 0) { return i; }
    }
    return std::string_view::npos;
}

// Index of the first top-level '#' (comment marker), or npos.
[[nodiscard]] inline std::size_t FindInlineComment(std::string_view s) {
    int depth = 0;
    bool inStr = false;
    bool esc = false;
    for (std::size_t i = 0; i < s.size(); ++i) {
        const char c = s[i];
        if (esc) { esc = false; continue; }
        if (inStr) {
            if (c == '\\') { esc = true; }
            else if (c == '"') { inStr = false; }
            continue;
        }
        if (c == '"') { inStr = true; continue; }
        if (c == '[') { ++depth; }
        else if (c == ']') { --depth; }
        else if (c == '#' && depth == 0) { return i; }
    }
    return std::string_view::npos;
}

// Split by `sep` at top level (outside `"..."` and `[...]`).
[[nodiscard]] inline std::vector<std::string_view> SplitTopLevel(std::string_view s, char sep) {
    std::vector<std::string_view> out;
    int depth = 0;
    bool inStr = false;
    bool esc = false;
    std::size_t start = 0;
    for (std::size_t i = 0; i < s.size(); ++i) {
        const char c = s[i];
        if (esc) { esc = false; continue; }
        if (inStr) {
            if (c == '\\') { esc = true; }
            else if (c == '"') { inStr = false; }
            continue;
        }
        if (c == '"') { inStr = true; continue; }
        if (c == '[') { ++depth; }
        else if (c == ']') { --depth; }
        else if (c == sep && depth == 0) {
            out.push_back(s.substr(start, i - start));
            start = i + 1;
        }
    }
    out.push_back(s.substr(start));
    return out;
}

// Drop "..." and decode \", \\, \n, \t, \r, \xHH escapes.
[[nodiscard]] inline std::string UnquoteString(std::string_view s) {
    s = Trim(s);
    if (s.size() < 2 || s.front() != '"' || s.back() != '"') {
        return std::string{s};
    }
    s = s.substr(1, s.size() - 2);
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        const char c = s[i];
        if (c == '\\' && i + 1 < s.size()) {
            const char n = s[++i];
            switch (n) {
                case '"':  out += '"';  break;
                case '\\': out += '\\'; break;
                case 'n':  out += '\n'; break;
                case 't':  out += '\t'; break;
                case 'r':  out += '\r'; break;
                case '0':  out += '\0'; break;
                case 'x': {
                    if (i + 2 < s.size()) {
                        const char hex[3] = {s[i + 1], s[i + 2], 0};
                        const unsigned v = std::strtoul(hex, nullptr, 16);
                        out += static_cast<char>(static_cast<unsigned char>(v));
                        i += 2;
                    }
                    break;
                }
                default: out += n; break;
            }
        }
        else {
            out += c;
        }
    }
    return out;
}

// Wrap into "..." with non-printable / non-ASCII as \xHH.
[[nodiscard]] inline std::string QuoteString(std::string_view s) {
    std::string out;
    out.reserve(s.size() + 2);
    out += '"';
    for (const unsigned char c : s) {
        if (c == '"') { out += "\\\""; }
        else if (c == '\\') { out += "\\\\"; }
        else if (c == '\n') { out += "\\n"; }
        else if (c == '\r') { out += "\\r"; }
        else if (c == '\t') { out += "\\t"; }
        else if (c < 0x20 || c >= 0x7F) {
            char buf[5];
            std::snprintf(buf, sizeof(buf), "\\x%02X", static_cast<unsigned>(c));
            out += buf;
        }
        else {
            out += static_cast<char>(c);
        }
    }
    out += '"';
    return out;
}

// =============================================================================
// Inline value parser ("[a, b]", "[[a, b], [c, d]]", scalars, "...")
// =============================================================================

[[nodiscard]] inline YamlNode ParseInlineValue(std::string_view s) {
    s = Trim(s);
    YamlNode node;
    if (s.empty()) {
        return node;
    }
    if (s.front() == '[') {
        if (s.size() < 2 || s.back() != ']') {
            node.kind = YamlNode::Scalar;
            node.scalar = std::string{s};
            return node;
        }
        s = s.substr(1, s.size() - 2);
        s = Trim(s);
        node.kind = YamlNode::Sequence;
        if (s.empty()) {
            return node;
        }
        for (auto part : SplitTopLevel(s, ',')) {
            node.sequence.push_back(ParseInlineValue(part));
        }
        return node;
    }
    node.kind = YamlNode::Scalar;
    node.scalar = std::string{s};
    return node;
}

// =============================================================================
// Tokenizer + block-style DOM parser
// =============================================================================

struct YLine {
    int indent;
    std::string content;
    int lineNum;
};

[[nodiscard]] inline std::vector<YLine> Tokenize(std::string_view text) {
    std::vector<YLine> out;
    std::size_t pos = 0;
    int lineNum = 0;
    while (pos <= text.size()) {
        ++lineNum;
        const std::size_t end = text.find('\n', pos);
        std::string_view line = text.substr(
            pos, (end == std::string_view::npos ? text.size() : end) - pos);
        while (!line.empty() &&
               (line.back() == '\r' || line.back() == ' ' || line.back() == '\t')) {
            line.remove_suffix(1);
        }
        int indent = 0;
        while (indent < static_cast<int>(line.size()) && line[indent] == ' ') {
            ++indent;
        }
        std::string_view rest = line.substr(indent);
        if (const std::size_t hash = FindInlineComment(rest);
            hash != std::string_view::npos) {
            rest = TrimRight(rest.substr(0, hash));
        }
        if (!rest.empty() && rest.front() != '#') {
            out.push_back({indent, std::string{rest}, lineNum});
        }
        if (end == std::string_view::npos) {
            break;
        }
        pos = end + 1;
    }
    return out;
}

class DomParser {
public:
    explicit DomParser(std::vector<YLine> lines) : _lines(std::move(lines)) {}

    [[nodiscard]] YamlNode ParseRoot() {
        return ParseMapping(0);
    }

private:
    std::vector<YLine> _lines;
    std::size_t _idx{0};

    [[nodiscard]] YamlNode ParseValueAt(int indent) {
        if (_idx >= _lines.size() || _lines[_idx].indent < indent) {
            return YamlNode{};
        }
        const std::string& c = _lines[_idx].content;
        if (c == "-" || c.starts_with("- ")) {
            return ParseSequence(indent);
        }
        return ParseMapping(indent);
    }

    [[nodiscard]] YamlNode ParseMapping(int indent) {
        YamlNode node;
        node.kind = YamlNode::Mapping;
        while (_idx < _lines.size() && _lines[_idx].indent == indent) {
            const std::string& content = _lines[_idx].content;
            if (content == "-" || content.starts_with("- ")) { break; }
            const std::size_t colon = FindKeyColon(content);
            if (colon == std::string::npos) { break; }
            std::string key{Trim(std::string_view{content}.substr(0, colon))};
            const std::string_view rest = Trim(std::string_view{content}.substr(colon + 1));
            ++_idx;
            YamlNode val;
            if (rest.empty()) {
                if (_idx < _lines.size() && _lines[_idx].indent > indent) {
                    val = ParseValueAt(_lines[_idx].indent);
                }
            }
            else {
                val = ParseInlineValue(rest);
            }
            node.mapping.emplace_back(std::move(key), std::move(val));
        }
        return node;
    }

    [[nodiscard]] YamlNode ParseSequence(int indent) {
        YamlNode node;
        node.kind = YamlNode::Sequence;
        while (_idx < _lines.size() && _lines[_idx].indent == indent) {
            const std::string& content = _lines[_idx].content;
            std::string_view rest;
            if (content == "-") { rest = std::string_view{}; }
            else if (content.starts_with("- ")) { rest = std::string_view{content}.substr(2); }
            else { break; }
            ++_idx;

            if (rest.empty()) {
                if (_idx < _lines.size() && _lines[_idx].indent > indent) {
                    node.sequence.push_back(ParseValueAt(_lines[_idx].indent));
                }
                else {
                    node.sequence.push_back(YamlNode{});
                }
                continue;
            }

            const std::size_t colon = FindKeyColon(rest);
            if (colon != std::string::npos) {
                YamlNode item;
                item.kind = YamlNode::Mapping;

                std::string firstKey{Trim(rest.substr(0, colon))};
                const std::string_view firstRest = Trim(rest.substr(colon + 1));

                YamlNode firstVal;
                if (firstRest.empty()) {
                    if (_idx < _lines.size() && _lines[_idx].indent > indent + 2) {
                        firstVal = ParseValueAt(_lines[_idx].indent);
                    }
                }
                else {
                    firstVal = ParseInlineValue(firstRest);
                }
                item.mapping.emplace_back(std::move(firstKey), std::move(firstVal));

                YamlNode tail = ParseMapping(indent + 2);
                for (auto& kv : tail.mapping) {
                    item.mapping.emplace_back(std::move(kv));
                }
                node.sequence.push_back(std::move(item));
            }
            else {
                node.sequence.push_back(ParseInlineValue(rest));
            }
        }
        return node;
    }
};

// =============================================================================
// Scalar accessors
// =============================================================================

[[nodiscard]] inline bool ParseBool(const YamlNode& n) {
    if (n.kind != YamlNode::Scalar) { return false; }
    return n.scalar == "true" || n.scalar == "True" || n.scalar == "TRUE";
}

[[nodiscard]] inline int ParseInt(const YamlNode& n) {
    if (n.kind != YamlNode::Scalar) { return 0; }
    return std::atoi(n.scalar.c_str());
}

[[nodiscard]] inline float ParseFloat(const YamlNode& n) {
    if (n.kind != YamlNode::Scalar) { return 0.0f; }
    // "bits:0xFFAAB1B8" — raw bit pattern fallback for non-finite values
    // (NaN/Inf survive byte-exact round-trip).
    constexpr std::string_view kBitsPrefix = "bits:0x";
    if (n.scalar.starts_with(kBitsPrefix)) {
        const std::uint32_t bits = static_cast<std::uint32_t>(
            std::strtoul(n.scalar.c_str() + kBitsPrefix.size(), nullptr, 16));
        return std::bit_cast<float>(bits);
    }
    return std::strtof(n.scalar.c_str(), nullptr);
}

[[nodiscard]] inline std::string ParseString(const YamlNode& n) {
    if (n.kind != YamlNode::Scalar) { return std::string{}; }
    return UnquoteString(n.scalar);
}

[[nodiscard]] inline D3DXVECTOR2 ParseVec2(const YamlNode& n) {
    D3DXVECTOR2 v{};
    if (n.kind == YamlNode::Sequence && n.sequence.size() >= 2) {
        v.x = ParseFloat(n.sequence[0]);
        v.y = ParseFloat(n.sequence[1]);
    }
    return v;
}

[[nodiscard]] inline D3DXVECTOR3 ParseVec3(const YamlNode& n) {
    D3DXVECTOR3 v{};
    if (n.kind == YamlNode::Sequence && n.sequence.size() >= 3) {
        v.x = ParseFloat(n.sequence[0]);
        v.y = ParseFloat(n.sequence[1]);
        v.z = ParseFloat(n.sequence[2]);
    }
    return v;
}

[[nodiscard]] inline D3DXVECTOR4 ParseVec4(const YamlNode& n) {
    D3DXVECTOR4 v{};
    if (n.kind == YamlNode::Sequence && n.sequence.size() >= 4) {
        v.x = ParseFloat(n.sequence[0]);
        v.y = ParseFloat(n.sequence[1]);
        v.z = ParseFloat(n.sequence[2]);
        v.w = ParseFloat(n.sequence[3]);
    }
    return v;
}

[[nodiscard]] inline D3DXCOLOR ParseColor(const YamlNode& n) {
    D3DXCOLOR c{};
    if (n.kind == YamlNode::Sequence && n.sequence.size() >= 4) {
        c.r = ParseFloat(n.sequence[0]);
        c.g = ParseFloat(n.sequence[1]);
        c.b = ParseFloat(n.sequence[2]);
        c.a = ParseFloat(n.sequence[3]);
    }
    return c;
}

// =============================================================================
// Emitter helpers
// =============================================================================

[[nodiscard]] inline std::string FmtFloat(float v) {
    // Decimal shortest round-trip; non-finite (NaN/Inf) → raw bit pattern.
    if (!std::isfinite(v)) {
        return std::format("bits:0x{:08X}", std::bit_cast<std::uint32_t>(v));
    }
    return std::format("{}", v);
}

[[nodiscard]] inline std::string FmtBool(bool v) {
    return v ? "true" : "false";
}

[[nodiscard]] inline std::string FmtVec2(const D3DXVECTOR2& v) {
    return std::format("[{}, {}]", FmtFloat(v.x), FmtFloat(v.y));
}

[[nodiscard]] inline std::string FmtVec3(const D3DXVECTOR3& v) {
    return std::format("[{}, {}, {}]",
                       FmtFloat(v.x), FmtFloat(v.y), FmtFloat(v.z));
}

[[nodiscard]] inline std::string FmtVec4(const D3DXVECTOR4& v) {
    return std::format("[{}, {}, {}, {}]",
                       FmtFloat(v.x), FmtFloat(v.y),
                       FmtFloat(v.z), FmtFloat(v.w));
}

[[nodiscard]] inline std::string FmtColor(const D3DXCOLOR& c) {
    return std::format("[{}, {}, {}, {}]",
                       FmtFloat(c.r), FmtFloat(c.g),
                       FmtFloat(c.b), FmtFloat(c.a));
}

// =============================================================================
// Enum <-> string framework
// =============================================================================

struct EnumName {
    int value;
    std::string_view name;
};

// Round-trip-safe: unknown values fall through to plain integer at both ends.
template <std::size_t N>
[[nodiscard]] inline std::string EnumToYaml(int v, const EnumName (&table)[N]) {
    for (const auto& e : table) {
        if (e.value == v) {
            return std::string{e.name};
        }
    }
    return std::format("{}", v);
}

template <std::size_t N>
[[nodiscard]] inline int EnumFromYaml(std::string_view s, const EnumName (&table)[N],
                                       int fallback) {
    s = Trim(s);
    if (s.empty()) { return fallback; }
    const char first = s.front();
    if (first == '-' || first == '+' ||
        (first >= '0' && first <= '9')) {
        return std::atoi(std::string{s}.c_str());
    }
    for (const auto& e : table) {
        if (e.name == s) {
            return e.value;
        }
    }
    return fallback;
}

// Build a comment string "# A | B | C | ..." from an enum table.
template <std::size_t N>
[[nodiscard]] inline std::string MakeChoicesComment(const EnumName (&table)[N]) {
    std::string out{"# "};
    for (std::size_t i = 0; i < N; ++i) {
        if (i) { out += " | "; }
        out += table[i].name;
    }
    return out;
}

// =============================================================================
// Shared D3DBLEND table (used by both .eff and .par)
// =============================================================================

inline constexpr EnumName kBlendNames[] = {
    { 1, "ZERO"},              // factor = (0, 0, 0, 0)
    { 2, "ONE"},               // factor = (1, 1, 1, 1)
    { 3, "SRCCOLOR"},          // factor = (Rs,  Gs,  Bs,  As)
    { 4, "INVSRCCOLOR"},       // factor = (1-Rs, 1-Gs, 1-Bs, 1-As)
    { 5, "SRCALPHA"},          // factor = (As,  As,  As,  As)
    { 6, "INVSRCALPHA"},       // factor = (1-As, 1-As, 1-As, 1-As)
    { 7, "DESTALPHA"},         // factor = (Ad,  Ad,  Ad,  Ad)
    { 8, "INVDESTALPHA"},      // factor = (1-Ad, 1-Ad, 1-Ad, 1-Ad)
    { 9, "DESTCOLOR"},         // factor = (Rd,  Gd,  Bd,  Ad)
    {10, "INVDESTCOLOR"},      // factor = (1-Rd, 1-Gd, 1-Bd, 1-Ad)
    {11, "SRCALPHASAT"},       // factor = (f, f, f, 1) where f = min(As, 1-Ad)
    {12, "BOTHSRCALPHA"},      // (deprecated)  src=As,    dest=1-As
    {13, "BOTHINVSRCALPHA"},   // (deprecated)  src=1-As,  dest=As
    {14, "BLENDFACTOR"},       // factor = render-state BLENDFACTOR rgba
    {15, "INVBLENDFACTOR"},    // factor = 1 - BLENDFACTOR rgba
    {16, "SRCCOLOR2"},         // multi-element: src1 rgba
    {17, "INVSRCCOLOR2"},      // multi-element: 1 - src1 rgba
};

[[nodiscard]] inline std::string BlendToYaml(D3DBLEND b) {
    return EnumToYaml(static_cast<int>(b), kBlendNames);
}

[[nodiscard]] inline D3DBLEND BlendFromYaml(const YamlNode& n) {
    if (n.kind != YamlNode::Scalar) {
        return D3DBLEND_ZERO;
    }
    return static_cast<D3DBLEND>(
        EnumFromYaml(n.scalar, kBlendNames, static_cast<int>(D3DBLEND_ZERO)));
}

[[nodiscard]] inline std::string_view BlendChoicesComment() {
    static const std::string s = MakeChoicesComment(kBlendNames);
    return s;
}

} // namespace Corsairs::Engine::Render::yaml
