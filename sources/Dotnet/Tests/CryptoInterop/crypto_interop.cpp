// ══════════════════════════════════════════════════════════
// CryptoInterop — C++ тестовая утилита для верификации
// совместимости AES-GCM шифрования между C++ и F#.
//
// Реализует два формата:
//
// 1) "gate" — формат C++ GateServer (оригинальный):
//    AES-128-GCM, tag=12, IV=16
//    Wire: [Base64(ciphertext + tag)][0x00][IV(16)]
//
// 2) "new" — новый формат F# AesTransport:
//    AES-256-GCM, tag=16, nonce=12
//    Wire: [nonce(12)][tag(16)][ciphertext]
//
// Использует Crypto++ (идентичная библиотека с GateServer).
//
// Команды:
//   crypto_interop gate-vectors     — тестовые вектора формата GateServer
//   crypto_interop gate-encrypt <hex_key_16b> <hex_iv_16b> <hex_pt>
//   crypto_interop gate-decrypt <hex_key_16b> <hex_wire>
//   crypto_interop new-vectors      — тестовые вектора нового формата
//   crypto_interop new-encrypt <hex_key_32b> <hex_nonce_12b> <hex_pt>
//   crypto_interop new-decrypt <hex_key_32b> <hex_wire>
//   crypto_interop roundtrip        — самопроверка обоих форматов
// ══════════════════════════════════════════════════════════

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <cstring>

#include "aes.h"
#include "gcm.h"
#include "filters.h"
#include "osrng.h"
#include "base64.h"

using namespace CryptoPP;

// ── Hex утилиты ──

static std::string toHex(const std::vector<uint8_t>& data) {
    std::ostringstream oss;
    for (auto b : data)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)b;
    return oss.str();
}

static std::string toHex(const uint8_t* data, size_t len) {
    std::ostringstream oss;
    for (size_t i = 0; i < len; i++)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    return oss.str();
}

static std::vector<uint8_t> fromHex(const std::string& hex) {
    std::vector<uint8_t> result;
    for (size_t i = 0; i + 1 < hex.size(); i += 2) {
        result.push_back((uint8_t)strtoul(hex.substr(i, 2).c_str(), nullptr, 16));
    }
    return result;
}

// ══════════════════════════════════════════════════════════
//  Формат "gate" — точная копия C++ GateServer
//  AES-128-GCM, IV=16(BLOCKSIZE), tag=12
//  Wire: [Base64(ciphertext + tag)][0x00][IV(16)]
// ══════════════════════════════════════════════════════════

static const int GATE_TAG_SIZE = 12;

// Шифрование как в GateServer::EncryptAES
static std::vector<uint8_t> gateEncrypt(
    const uint8_t* key, size_t keyLen,     // 16 байт (AES::MIN_KEYLENGTH)
    const uint8_t* iv, size_t ivLen,       // 16 байт (AES::BLOCKSIZE)
    const uint8_t* plaintext, size_t ptLen)
{
    // 1. AES-GCM encrypt → output = ciphertext + tag(12)
    std::string output;
    GCM<AES>::Encryption e;
    e.SetKeyWithIV(key, keyLen, iv, ivLen);
    StringSource ss(plaintext, ptLen, true,
        new AuthenticatedEncryptionFilter(e,
            new StringSink(output),
            false,    // MAC не в начало
            GATE_TAG_SIZE));

    // 2. Base64 encode
    std::string base64;
    StringSource ss2(output, true,
        new Base64Encoder(new StringSink(base64), false));

    // 3. Собираем wire: [base64][0x00][IV(16)]
    std::vector<uint8_t> wire;
    wire.reserve(base64.size() + 1 + ivLen);
    wire.insert(wire.end(), base64.begin(), base64.end());
    wire.push_back(0x00); // разделитель
    wire.insert(wire.end(), iv, iv + ivLen);

    return wire;
}

// Расшифровка как в GateServer::DecryptAES
static std::vector<uint8_t> gateDecrypt(
    const uint8_t* key, size_t keyLen,
    const uint8_t* wire, size_t wireLen)
{
    if (wireLen < AES::BLOCKSIZE + 1)
        throw std::runtime_error("Wire data too short");

    // 1. Извлекаем IV с конца: последние 16 байт
    size_t base64Len = wireLen - AES::BLOCKSIZE - 1;
    SecByteBlock iv(wire + base64Len + 1, AES::BLOCKSIZE);

    // 2. Base64 decode
    std::string base64decoded;
    StringSource ss(wire, base64Len, true,
        new Base64Decoder(new StringSink(base64decoded)));

    // 3. AES-GCM decrypt
    GCM<AES>::Decryption d;
    d.SetKeyWithIV(key, keyLen, iv.data(), AES::BLOCKSIZE);

    std::string plain;
    AuthenticatedDecryptionFilter df(d,
        new StringSink(plain),
        AuthenticatedDecryptionFilter::DEFAULT_FLAGS,
        GATE_TAG_SIZE);
    StringSource ss2(base64decoded, true, new Redirector(df));

    if (!df.GetLastResult())
        throw std::runtime_error("Authentication failed");

    return std::vector<uint8_t>(plain.begin(), plain.end());
}

// ══════════════════════════════════════════════════════════
//  Формат "new" — F# AesTransport
//  AES-256-GCM, nonce=12, tag=16
//  Wire: [nonce(12)][tag(16)][ciphertext]
// ══════════════════════════════════════════════════════════

static const int NEW_NONCE_SIZE = 12;
static const int NEW_TAG_SIZE = 16;

static std::vector<uint8_t> newEncrypt(
    const uint8_t* key, size_t keyLen,       // 32 байта
    const uint8_t* nonce, size_t nonceLen,   // 12 байт
    const uint8_t* plaintext, size_t ptLen)
{
    GCM<AES>::Encryption e;
    e.SetKeyWithIV(key, keyLen, nonce, nonceLen);

    // ciphertextAndTag = [ciphertext][tag(16)]
    std::string ciphertextAndTag;
    StringSource ss(plaintext, ptLen, true,
        new AuthenticatedEncryptionFilter(e,
            new StringSink(ciphertextAndTag),
            false,
            NEW_TAG_SIZE));

    size_t ctLen = ciphertextAndTag.size() - NEW_TAG_SIZE;

    // Собираем: [nonce(12)][tag(16)][ciphertext]
    std::vector<uint8_t> result;
    result.reserve(nonceLen + NEW_TAG_SIZE + ctLen);
    result.insert(result.end(), nonce, nonce + nonceLen);
    result.insert(result.end(),
        ciphertextAndTag.begin() + ctLen,
        ciphertextAndTag.end());
    result.insert(result.end(),
        ciphertextAndTag.begin(),
        ciphertextAndTag.begin() + ctLen);

    return result;
}

static std::vector<uint8_t> newDecrypt(
    const uint8_t* key, size_t keyLen,
    const uint8_t* data, size_t dataLen)
{
    if (dataLen < (size_t)(NEW_NONCE_SIZE + NEW_TAG_SIZE))
        throw std::runtime_error("Data too short");

    const uint8_t* nonce = data;
    const uint8_t* tag = data + NEW_NONCE_SIZE;
    const uint8_t* ciphertext = data + NEW_NONCE_SIZE + NEW_TAG_SIZE;
    size_t ctLen = dataLen - NEW_NONCE_SIZE - NEW_TAG_SIZE;

    GCM<AES>::Decryption d;
    d.SetKeyWithIV(key, keyLen, nonce, NEW_NONCE_SIZE);

    // Crypto++ ожидает [ciphertext][tag]
    std::string ctAndTag;
    ctAndTag.append((const char*)ciphertext, ctLen);
    ctAndTag.append((const char*)tag, NEW_TAG_SIZE);

    std::string plain;
    AuthenticatedDecryptionFilter df(d,
        new StringSink(plain),
        AuthenticatedDecryptionFilter::DEFAULT_FLAGS,
        NEW_TAG_SIZE);
    StringSource ss(ctAndTag, true, new Redirector(df));

    if (!df.GetLastResult())
        throw std::runtime_error("Authentication failed");

    return std::vector<uint8_t>(plain.begin(), plain.end());
}

// ══════════════════════════════════════════════════════════
//  Команды
// ══════════════════════════════════════════════════════════

static void cmdGateVectors() {
    struct TestCase {
        std::vector<uint8_t> key;   // 16 bytes
        std::vector<uint8_t> iv;    // 16 bytes
        std::vector<uint8_t> plaintext;
    };

    std::vector<TestCase> cases;

    // Тест 1: "Hello, World!"
    {
        TestCase tc;
        tc.key.resize(AES::MIN_KEYLENGTH);
        for (int i = 0; i < (int)AES::MIN_KEYLENGTH; i++) tc.key[i] = (uint8_t)i;
        tc.iv.resize(AES::BLOCKSIZE);
        for (int i = 0; i < (int)AES::BLOCKSIZE; i++) tc.iv[i] = (uint8_t)(i + 1);
        const char* msg = "Hello, World!";
        tc.plaintext.assign((uint8_t*)msg, (uint8_t*)msg + strlen(msg));
        cases.push_back(tc);
    }

    // Тест 2: пустые данные
    {
        TestCase tc;
        tc.key.resize(AES::MIN_KEYLENGTH);
        for (int i = 0; i < (int)AES::MIN_KEYLENGTH; i++) tc.key[i] = (uint8_t)(i * 3 + 7);
        tc.iv.resize(AES::BLOCKSIZE);
        for (int i = 0; i < (int)AES::BLOCKSIZE; i++) tc.iv[i] = (uint8_t)(i * 2 + 5);
        cases.push_back(tc);
    }

    // Тест 3: бинарные данные (64 байта)
    {
        TestCase tc;
        tc.key.resize(AES::MIN_KEYLENGTH);
        for (int i = 0; i < (int)AES::MIN_KEYLENGTH; i++) tc.key[i] = (uint8_t)(255 - i);
        tc.iv.resize(AES::BLOCKSIZE);
        for (int i = 0; i < (int)AES::BLOCKSIZE; i++) tc.iv[i] = (uint8_t)(i ^ 0xAA);
        tc.plaintext.resize(64);
        for (int i = 0; i < 64; i++) tc.plaintext[i] = (uint8_t)i;
        cases.push_back(tc);
    }

    // Тест 4: имитация пакета CMD+payload
    {
        TestCase tc;
        tc.key.resize(AES::MIN_KEYLENGTH);
        for (int i = 0; i < (int)AES::MIN_KEYLENGTH; i++) tc.key[i] = (uint8_t)(i * 7 + 13);
        tc.iv.resize(AES::BLOCKSIZE);
        for (int i = 0; i < (int)AES::BLOCKSIZE; i++) tc.iv[i] = (uint8_t)(i + 0x10);
        tc.plaintext = {0x01, 0x00, 0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE};
        cases.push_back(tc);
    }

    for (const auto& tc : cases) {
        auto wire = gateEncrypt(tc.key.data(), tc.key.size(),
            tc.iv.data(), tc.iv.size(),
            tc.plaintext.data(), tc.plaintext.size());

        // Verify roundtrip
        auto dec = gateDecrypt(tc.key.data(), tc.key.size(),
            wire.data(), wire.size());

        bool ok = (dec.size() == tc.plaintext.size() &&
                   (dec.empty() || memcmp(dec.data(), tc.plaintext.data(), dec.size()) == 0));
        if (!ok) {
            std::cerr << "GATE ROUNDTRIP FAILED!" << std::endl;
            return;
        }

        std::cout << toHex(tc.key) << " "
                  << toHex(tc.iv) << " "
                  << toHex(tc.plaintext) << " "
                  << toHex(wire) << std::endl;
    }
}

static void cmdNewVectors() {
    struct TestCase {
        std::vector<uint8_t> key;   // 32 bytes
        std::vector<uint8_t> nonce; // 12 bytes
        std::vector<uint8_t> plaintext;
    };

    std::vector<TestCase> cases;

    // Тест 1: "Hello, World!"
    {
        TestCase tc;
        tc.key.resize(32);
        for (int i = 0; i < 32; i++) tc.key[i] = (uint8_t)i;
        tc.nonce.resize(12);
        for (int i = 0; i < 12; i++) tc.nonce[i] = (uint8_t)(i + 1);
        const char* msg = "Hello, World!";
        tc.plaintext.assign((uint8_t*)msg, (uint8_t*)msg + strlen(msg));
        cases.push_back(tc);
    }

    // Тест 2: пустые данные
    {
        TestCase tc;
        tc.key.resize(32);
        for (int i = 0; i < 32; i++) tc.key[i] = (uint8_t)(i * 3 + 7);
        tc.nonce.resize(12);
        for (int i = 0; i < 12; i++) tc.nonce[i] = (uint8_t)(i * 2 + 5);
        cases.push_back(tc);
    }

    // Тест 3: 256 байт бинарных данных
    {
        TestCase tc;
        tc.key.resize(32);
        for (int i = 0; i < 32; i++) tc.key[i] = (uint8_t)(255 - i);
        tc.nonce.resize(12);
        for (int i = 0; i < 12; i++) tc.nonce[i] = (uint8_t)(i ^ 0xAA);
        tc.plaintext.resize(256);
        for (int i = 0; i < 256; i++) tc.plaintext[i] = (uint8_t)i;
        cases.push_back(tc);
    }

    // Тест 4: имитация пакета
    {
        TestCase tc;
        tc.key.resize(32);
        for (int i = 0; i < 32; i++) tc.key[i] = (uint8_t)(i * 7 + 13);
        tc.nonce.resize(12);
        for (int i = 0; i < 12; i++) tc.nonce[i] = (uint8_t)(i + 0x10);
        tc.plaintext = {0x01, 0x00, 0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE};
        cases.push_back(tc);
    }

    for (const auto& tc : cases) {
        auto wire = newEncrypt(tc.key.data(), tc.key.size(),
            tc.nonce.data(), tc.nonce.size(),
            tc.plaintext.data(), tc.plaintext.size());

        auto dec = newDecrypt(tc.key.data(), tc.key.size(),
            wire.data(), wire.size());

        bool ok = (dec.size() == tc.plaintext.size() &&
                   (dec.empty() || memcmp(dec.data(), tc.plaintext.data(), dec.size()) == 0));
        if (!ok) {
            std::cerr << "NEW ROUNDTRIP FAILED!" << std::endl;
            return;
        }

        std::cout << toHex(tc.key) << " "
                  << toHex(tc.nonce) << " "
                  << toHex(tc.plaintext) << " "
                  << toHex(wire) << std::endl;
    }
}

static void cmdRoundtrip() {
    AutoSeededRandomPool rng;
    int passed = 0, total = 0;

    std::cout << "=== Gate format (AES-128-GCM, Base64, tag=12) ===" << std::endl;
    {
        uint8_t key[AES::MIN_KEYLENGTH], iv[AES::BLOCKSIZE];
        rng.GenerateBlock(key, sizeof(key));
        rng.GenerateBlock(iv, sizeof(iv));

        const char* messages[] = { "", "A", "Hello, World!",
            "AES-128-GCM gate format interop test" };
        for (auto msg : messages) {
            total++;
            size_t len = strlen(msg);
            auto wire = gateEncrypt(key, AES::MIN_KEYLENGTH, iv, AES::BLOCKSIZE,
                (const uint8_t*)msg, len);
            auto dec = gateDecrypt(key, AES::MIN_KEYLENGTH, wire.data(), wire.size());
            bool ok = (dec.size() == len && (len == 0 || memcmp(dec.data(), msg, len) == 0));
            std::cout << (ok ? "  PASS" : "  FAIL")
                      << ": \"" << msg << "\" (" << len << " bytes)" << std::endl;
            if (ok) passed++;
        }
    }

    std::cout << "=== New format (AES-256-GCM, binary, tag=16) ===" << std::endl;
    {
        uint8_t key[32], nonce[12];
        rng.GenerateBlock(key, sizeof(key));
        rng.GenerateBlock(nonce, sizeof(nonce));

        const char* messages[] = { "", "A", "Hello, World!",
            "AES-256-GCM new format interop test" };
        for (auto msg : messages) {
            total++;
            size_t len = strlen(msg);
            auto wire = newEncrypt(key, 32, nonce, 12, (const uint8_t*)msg, len);
            auto dec = newDecrypt(key, 32, wire.data(), wire.size());
            bool ok = (dec.size() == len && (len == 0 || memcmp(dec.data(), msg, len) == 0));
            std::cout << (ok ? "  PASS" : "  FAIL")
                      << ": \"" << msg << "\" (" << len << " bytes)" << std::endl;
            if (ok) passed++;
        }
    }

    std::cout << passed << "/" << total << " tests passed" << std::endl;
}

static void cmdGateEncrypt(const std::string& hexKey, const std::string& hexIv, const std::string& hexPt) {
    auto key = fromHex(hexKey);
    auto iv = fromHex(hexIv);
    auto pt = fromHex(hexPt);
    if (key.size() != AES::MIN_KEYLENGTH) { std::cerr << "ERROR: key must be 16 bytes" << std::endl; return; }
    if (iv.size() != AES::BLOCKSIZE) { std::cerr << "ERROR: iv must be 16 bytes" << std::endl; return; }
    auto wire = gateEncrypt(key.data(), key.size(), iv.data(), iv.size(), pt.data(), pt.size());
    std::cout << toHex(wire) << std::endl;
}

static void cmdGateDecrypt(const std::string& hexKey, const std::string& hexWire) {
    auto key = fromHex(hexKey);
    auto wire = fromHex(hexWire);
    if (key.size() != AES::MIN_KEYLENGTH) { std::cerr << "ERROR: key must be 16 bytes" << std::endl; return; }
    try {
        auto pt = gateDecrypt(key.data(), key.size(), wire.data(), wire.size());
        std::cout << toHex(pt) << std::endl;
    } catch (const std::exception& ex) { std::cerr << "ERROR: " << ex.what() << std::endl; }
}

static void cmdNewEncrypt(const std::string& hexKey, const std::string& hexNonce, const std::string& hexPt) {
    auto key = fromHex(hexKey);
    auto nonce = fromHex(hexNonce);
    auto pt = fromHex(hexPt);
    if (key.size() != 32) { std::cerr << "ERROR: key must be 32 bytes" << std::endl; return; }
    if (nonce.size() != 12) { std::cerr << "ERROR: nonce must be 12 bytes" << std::endl; return; }
    auto wire = newEncrypt(key.data(), key.size(), nonce.data(), nonce.size(), pt.data(), pt.size());
    std::cout << toHex(wire) << std::endl;
}

static void cmdNewDecrypt(const std::string& hexKey, const std::string& hexWire) {
    auto key = fromHex(hexKey);
    auto wire = fromHex(hexWire);
    if (key.size() != 32) { std::cerr << "ERROR: key must be 32 bytes" << std::endl; return; }
    try {
        auto pt = newDecrypt(key.data(), key.size(), wire.data(), wire.size());
        std::cout << toHex(pt) << std::endl;
    } catch (const std::exception& ex) { std::cerr << "ERROR: " << ex.what() << std::endl; }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "CryptoInterop — AES-GCM test utility (Crypto++)" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Gate format (C++ GateServer): AES-128-GCM, tag=12, IV=16" << std::endl;
        std::cerr << "  Wire: [Base64(ciphertext+tag)][0x00][IV(16)]" << std::endl;
        std::cerr << std::endl;
        std::cerr << "New format (F# AesTransport): AES-256-GCM, tag=16, nonce=12" << std::endl;
        std::cerr << "  Wire: [nonce(12)][tag(16)][ciphertext]" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Usage:" << std::endl;
        std::cerr << "  crypto_interop gate-vectors" << std::endl;
        std::cerr << "  crypto_interop gate-encrypt <hex_key_16> <hex_iv_16> <hex_pt>" << std::endl;
        std::cerr << "  crypto_interop gate-decrypt <hex_key_16> <hex_wire>" << std::endl;
        std::cerr << "  crypto_interop new-vectors" << std::endl;
        std::cerr << "  crypto_interop new-encrypt  <hex_key_32> <hex_nonce_12> <hex_pt>" << std::endl;
        std::cerr << "  crypto_interop new-decrypt  <hex_key_32> <hex_wire>" << std::endl;
        std::cerr << "  crypto_interop roundtrip" << std::endl;
        return 1;
    }

    std::string cmd = argv[1];
    try {
        if (cmd == "gate-vectors") cmdGateVectors();
        else if (cmd == "gate-encrypt" && argc >= 5) cmdGateEncrypt(argv[2], argv[3], argv[4]);
        else if (cmd == "gate-decrypt" && argc >= 4) cmdGateDecrypt(argv[2], argv[3]);
        else if (cmd == "new-vectors") cmdNewVectors();
        else if (cmd == "new-encrypt" && argc >= 5) cmdNewEncrypt(argv[2], argv[3], argv[4]);
        else if (cmd == "new-decrypt" && argc >= 4) cmdNewDecrypt(argv[2], argv[3]);
        else if (cmd == "roundtrip") cmdRoundtrip();
        else { std::cerr << "Unknown command: " << cmd << std::endl; return 1; }
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 2;
    }
    return 0;
}
