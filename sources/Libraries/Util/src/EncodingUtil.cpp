#include "EncodingUtil.h"

#include <windows.h>

#include <array>
#include <format>

namespace encoding {

std::string AnsiToUtf8(std::string_view ansi) {
    if (ansi.empty()) {
        return {};
    }
    const int wlen = ::MultiByteToWideChar(
        CP_ACP, 0, ansi.data(), static_cast<int>(ansi.size()), nullptr, 0);
    if (wlen <= 0) {
        return {};
    }
    std::wstring w(static_cast<std::size_t>(wlen), L'\0');
    ::MultiByteToWideChar(
        CP_ACP, 0, ansi.data(), static_cast<int>(ansi.size()), w.data(), wlen);
    const int ulen = ::WideCharToMultiByte(
        CP_UTF8, 0, w.c_str(), wlen, nullptr, 0, nullptr, nullptr);
    if (ulen <= 0) {
        return {};
    }
    std::string out(static_cast<std::size_t>(ulen), '\0');
    ::WideCharToMultiByte(
        CP_UTF8, 0, w.c_str(), wlen, out.data(), ulen, nullptr, nullptr);
    return out;
}

std::string Utf8ToAnsi(std::string_view utf8) {
    if (utf8.empty()) {
        return {};
    }
    const int wlen = ::MultiByteToWideChar(
        CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    if (wlen <= 0) {
        return {};
    }
    std::wstring w(static_cast<std::size_t>(wlen), L'\0');
    ::MultiByteToWideChar(
        CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), w.data(), wlen);
    const int alen = ::WideCharToMultiByte(
        CP_ACP, 0, w.c_str(), wlen, nullptr, 0, nullptr, nullptr);
    if (alen <= 0) {
        return {};
    }
    std::string out(static_cast<std::size_t>(alen), '\0');
    ::WideCharToMultiByte(
        CP_ACP, 0, w.c_str(), wlen, out.data(), alen, nullptr, nullptr);
    return out;
}

std::string WideToUtf8(std::wstring_view wide) {
    if (wide.empty()) {
        return {};
    }
    const int ulen = ::WideCharToMultiByte(
        CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()),
        nullptr, 0, nullptr, nullptr);
    if (ulen <= 0) {
        return {};
    }
    std::string out(static_cast<std::size_t>(ulen), '\0');
    ::WideCharToMultiByte(
        CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()),
        out.data(), ulen, nullptr, nullptr);
    return out;
}

std::wstring Utf8ToWide(std::string_view utf8) {
    if (utf8.empty()) {
        return {};
    }
    const int wlen = ::MultiByteToWideChar(
        CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    if (wlen <= 0) {
        return {};
    }
    std::wstring out(static_cast<std::size_t>(wlen), L'\0');
    ::MultiByteToWideChar(
        CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), out.data(), wlen);
    return out;
}

void AppendAnsiByteAsUtf8(unsigned char byte, std::string& out) {
    if (byte < 0x80) {
        out.push_back(static_cast<char>(byte));
        return;
    }
    const char mb[2] = { static_cast<char>(byte), '\0' };
    wchar_t wc = 0;
    if (::MultiByteToWideChar(CP_ACP, 0, mb, 1, &wc, 1) != 1) {
        return;
    }
    char utf8[8]{};
    const int ulen = ::WideCharToMultiByte(
        CP_UTF8, 0, &wc, 1, utf8, sizeof(utf8), nullptr, nullptr);
    if (ulen > 0) {
        out.append(utf8, static_cast<std::size_t>(ulen));
    }
}

void PopLastUtf8Codepoint(std::string& s) {
    if (s.empty()) {
        return;
    }
    do {
        s.pop_back();
    } while (!s.empty() && !IsUtf8StartByte(static_cast<unsigned char>(s.back())));
}

bool EqualsIgnoreCaseAscii(std::string_view a, std::string_view b) noexcept {
    if (a.size() != b.size()) {
        return false;
    }
    for (std::size_t i = 0; i < a.size(); ++i) {
        const unsigned char ca = static_cast<unsigned char>(a[i]);
        const unsigned char cb = static_cast<unsigned char>(b[i]);
        const unsigned char la = (ca >= 'A' && ca <= 'Z') ? ca + ('a' - 'A') : ca;
        const unsigned char lb = (cb >= 'A' && cb <= 'Z') ? cb + ('a' - 'A') : cb;
        if (la != lb) {
            return false;
        }
    }
    return true;
}

std::string HexDump(std::string_view bytes, std::size_t maxBytes) {
    std::string out;
    const std::size_t n = bytes.size() < maxBytes ? bytes.size() : maxBytes;
    out.reserve(n * 3 + 4);
    for (std::size_t i = 0; i < n; ++i) {
        if (i > 0) {
            out.push_back(' ');
        }
        out += std::format("{:02x}", static_cast<unsigned char>(bytes[i]));
    }
    if (bytes.size() > maxBytes) {
        out += " ...";
    }
    return out;
}

}  // namespace encoding
