namespace Corsairs.Platform.Network.Crypto

open System

/// Интерфейс криптографического middleware для шифрования/дешифрования пакетов.
/// Встраивается в pipeline между framing (PacketCodec) и обработкой (routing).
/// Encrypt/Decrypt пишут результат в output-буфер и возвращают кол-во записанных байт, или -1 при ошибке.
[<AllowNullLiteral>]
type ICryptoMiddleware =
    /// Зашифровать data → output. Output должен вместить data.Length + Overhead байт.
    /// Возвращает кол-во записанных байт, или -1 при ошибке.
    abstract member Encrypt: data: ReadOnlySpan<byte> * output: Memory<byte> -> int

    /// Дешифровать data → output. Output должен вместить data.Length - Overhead байт.
    /// Возвращает кол-во записанных байт, или -1 при ошибке.
    abstract member Decrypt: data: ReadOnlySpan<byte> * output: Memory<byte> -> int

    /// Overhead шифрования в байтах (nonce + tag). Для AES-256-GCM = 28.
    abstract member Overhead: int

    /// Активно ли шифрование (после завершения key exchange).
    abstract member IsActive: bool

/// Пустая реализация — без шифрования (passthrough).
type NoCryptoMiddleware() =
    interface ICryptoMiddleware with
        member _.Encrypt(data, output) =
            data.CopyTo(output.Span)
            data.Length
        member _.Decrypt(data, output) =
            data.CopyTo(output.Span)
            data.Length
        member _.Overhead = 0
        member _.IsActive = false
