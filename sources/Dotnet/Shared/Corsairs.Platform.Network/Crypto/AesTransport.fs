namespace Corsairs.Platform.Network.Crypto

open System
open System.Security.Cryptography

/// AES-256-GCM шифрование/дешифрование пакетов.
/// Формат зашифрованного блока: [12 байт nonce][16 байт tag][ciphertext]
type AesTransport(key: byte array) =

    do
        if key.Length <> 32 then
            raise (ArgumentException($"AES-256 ключ должен быть 32 байта, получено {key.Length}"))

    let _aes = new AesGcm(key, 16)

    /// Размер nonce для AES-GCM.
    static let NonceSize = 12
    /// Размер authentication tag.
    static let TagSize = 16
    /// Суммарный overhead (nonce + tag).
    static let Overhead = NonceSize + TagSize

    // ThreadStatic буферы фиксированного размера — избегаем аллокаций на каждый пакет.
    [<ThreadStatic; DefaultValue>]
    static val mutable private _tlsNonce: byte array

    [<ThreadStatic; DefaultValue>]
    static val mutable private _tlsTag: byte array

    static member private Nonce =
        if isNull AesTransport._tlsNonce then
            AesTransport._tlsNonce <- Array.zeroCreate<byte> NonceSize
        AesTransport._tlsNonce

    static member private Tag =
        if isNull AesTransport._tlsTag then
            AesTransport._tlsTag <- Array.zeroCreate<byte> TagSize
        AesTransport._tlsTag

    interface ICryptoMiddleware with

        member _.Encrypt(data: ReadOnlySpan<byte>, output: Memory<byte>) =
            let resultLen = Overhead + data.Length
            if output.Length < resultLen then -1
            else
                try
                    let nonce = AesTransport.Nonce
                    RandomNumberGenerator.Fill(nonce.AsSpan())
                    let tag = AesTransport.Tag
                    let outSpan = output.Span

                    // [nonce(12)][tag(16)][ciphertext] — всё пишем прямо в output
                    _aes.Encrypt(nonce.AsSpan(), data, outSpan.Slice(Overhead, data.Length), tag.AsSpan())
                    nonce.AsSpan().CopyTo(outSpan)
                    tag.AsSpan().CopyTo(outSpan.Slice(NonceSize))
                    resultLen
                with _ -> -1

        member _.Decrypt(data: ReadOnlySpan<byte>, output: Memory<byte>) =
            if data.Length < Overhead then -1
            else
                let plaintextLen = data.Length - Overhead
                if output.Length < plaintextLen then -1
                else
                    try
                        let nonce = data.Slice(0, NonceSize)
                        let tag = data.Slice(NonceSize, TagSize)
                        let ciphertext = data.Slice(Overhead)
                        _aes.Decrypt(nonce, ciphertext, tag, output.Span.Slice(0, plaintextLen))
                        plaintextLen
                    with _ -> -1

        member _.Overhead = Overhead
        member _.IsActive = true

    interface IDisposable with
        member _.Dispose() = _aes.Dispose()
