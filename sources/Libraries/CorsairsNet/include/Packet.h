#pragma once

// Packet.h — WPacket (запись) и RPacket (чтение) бинарных пакетов.
// Аналог F# WPacket/RPacket из Corsairs.Platform.Network.Protocol.
//
// Формат кадра: [2b size BE][4b SESS BE][2b CMD BE][payload]
// Размер в первых 2 байтах — единственный source of truth (как в F#).
// Буферы выделяются из PacketPool::Shared().

#include "BinaryIO.h"
#include "PacketPool.h"
#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace net {

// ═══════════════════════════════════════════════════════════════
//  WPacket — запись пакетов
// ═══════════════════════════════════════════════════════════════

class WPacket {
public:
    // Создать WPacket с ёмкостью payload. Заголовок (8 байт) резервируется автоматически.
    explicit WPacket(int payloadCapacity = 0);

    // Deep copy
    WPacket(const WPacket& other);
    WPacket& operator=(const WPacket& other);

    // Move
    WPacket(WPacket&& other) noexcept;
    WPacket& operator=(WPacket&& other) noexcept;

    // Деструктор — возвращает буфер в пул
    ~WPacket();

    // ── Заголовок (фиксированные позиции) ───────────────────

    void     WriteCmd(uint16_t cmd);        // Запись CMD в [6..7]
    void     WriteSess(uint32_t sess);      // Запись SESS в [2..5]
    uint16_t GetCmd() const;                // Чтение CMD из [6..7]
    uint32_t GetSess() const;               // Чтение SESS из [2..5]

    // ── Размеры ─────────────────────────────────────────────

    int  GetPacketSize() const;             // Чтение из [0..1] BE
    void SetPacketSize(int size);           // Запись в [0..1] BE
    int  PayloadLength() const;             // GetPacketSize() - 8

    // ── Запись payload ──────────────────────────────────────

    void WriteUInt8(uint8_t v);
    void WriteInt8(int8_t v);
    void WriteUInt16(uint16_t v);
    void WriteInt16(int16_t v);
    void WriteUInt32(uint32_t v);
    void WriteInt32(int32_t v);
    void WriteInt64(int64_t v);
    void WriteUInt64(uint64_t v);
    void WriteFloat32(float v);

    // Бинарная последовательность: [2b длина BE][данные]
    void WriteSequence(const uint8_t* data, uint16_t len);
    void WriteSequence(const char* data, uint16_t len);

    // Строка: [2b длина+1 BE][UTF-8][null]
    void WriteString(const char* str);

    // ── Доступ к буферу ─────────────────────────────────────

    uint8_t*       Data()       { return _data; }
    const uint8_t* Data() const { return _data; }
    int            Capacity() const { return _capacity; }

    // Сброс для повторного использования
    void Reset();

    // Проверка валидности
    explicit operator bool() const { return _data != nullptr; }

private:
    // Проверить/расширить буфер. Возвращает текущую позицию записи (= GetPacketSize()).
    int EnsureCapacity(int needed);

    uint8_t* _data;
    int _capacity;  // Размер аллоцированного буфера (бакет пула)
};

// ═══════════════════════════════════════════════════════════════
//  RPacket — чтение пакетов
// ═══════════════════════════════════════════════════════════════

class RPacket {
public:
    // Создать RPacket из буфера. Если ownsBuffer=true, деструктор вернёт буфер в пул.
    RPacket(uint8_t* data, int dataLen, bool ownsBuffer = false);

    // Пустой пакет (невалидный)
    RPacket();

    // Нет копирования
    RPacket(const RPacket&) = delete;
    RPacket& operator=(const RPacket&) = delete;

    // Move
    RPacket(RPacket&& other) noexcept;
    RPacket& operator=(RPacket&& other) noexcept;

    // Деструктор — возвращает буфер в пул если ownsBuffer
    ~RPacket();

    // ── Заголовок (читается из буфера напрямую) ─────────────

    uint32_t GetSess() const;               // [2..5]
    uint16_t GetCmd() const;                // [6..7]
    int      GetPacketSize() const;         // [0..1] BE
    int      PayloadLength() const;         // GetPacketSize() - 8

    // ── Состояние чтения ────────────────────────────────────

    int  RemainingBytes() const;            // GetPacketSize() - _pos
    int  Position() const { return _pos; }
    bool HasData() const;                   // _pos < GetPacketSize()

    // ── Последовательное чтение payload ─────────────────────

    uint8_t  ReadUInt8();
    int8_t   ReadInt8();
    uint16_t ReadUInt16();
    int16_t  ReadInt16();
    uint32_t ReadUInt32();
    int32_t  ReadInt32();
    int64_t  ReadInt64();
    uint64_t ReadUInt64();
    float    ReadFloat32();

    // Бинарная последовательность: [2b длина BE][данные]. Возвращает указатель внутрь буфера.
    const char* ReadSequence(uint16_t& outLen);

    // Строка: [2b длина BE][UTF-8 + null]. Возвращает указатель внутрь буфера.
    const char* ReadString(uint16_t* outLen = nullptr);

    // ── Обратное чтение (с конца пакета) ────────────────────

    uint8_t  ReverseReadUInt8();
    uint16_t ReverseReadUInt16();
    uint32_t ReverseReadUInt32();
    int64_t  ReverseReadInt64();

    // ── Мутации ─────────────────────────────────────────────

    // Отбросить count байт с конца. Возвращает false если останется < 8 байт.
    bool DiscardLast(int count);

    // Пропустить count байт
    void Skip(int count);

    // Сбросить позицию чтения. Восстанавливает size = _dataLen.
    void Reset();

    // ── Доступ к буферу ─────────────────────────────────────

    uint8_t*       Data()       { return _data; }
    const uint8_t* Data() const { return _data; }

    // Проверка валидности
    explicit operator bool() const { return _data != nullptr && _dataLen >= 8; }

private:
    int packetSize() const;  // Вспомогательная: чтение size из заголовка

    void ensureAvailable(int count) const;
    void ensureReverseAvailable(int count) const;

    uint8_t* _data;
    int _dataLen;       // Размер аллокации (для Reset и Free)
    int _pos;           // Курсор чтения (начинается с 8)
    int _revPos;        // Курсор обратного чтения
    bool _ownsBuffer;   // Вернуть в пул при destroy
};

} // namespace net
