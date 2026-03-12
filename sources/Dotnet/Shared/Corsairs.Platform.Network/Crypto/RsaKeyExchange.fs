namespace Corsairs.Platform.Network.Crypto

open System
open System.Security.Cryptography

/// RSA-3072 обмен ключами для установки сессионного AES-256 ключа.
[<AllowNullLiteral>]
type RsaKeyExchange() =

    let _rsa = RSA.Create(3072)

    /// Экспортировать публичный ключ сервера в SPKI DER формате (для отправки клиенту).
    /// SPKI DER совместим с Windows CryptAPI (CryptImportPublicKeyInfoEx2).
    member _.ExportPublicKey() : byte array =
        _rsa.ExportSubjectPublicKeyInfo()

    /// Импортировать публичный ключ удалённой стороны и зашифровать AES-ключ.
    /// Возвращает зашифрованный AES-ключ для передачи по сети.
    static member EncryptSessionKey(remotePublicKey: byte array, aesKey: byte array) : byte array =
        use rsa = RSA.Create()
        let mutable bytesRead = 0
        rsa.ImportSubjectPublicKeyInfo(ReadOnlySpan<byte>(remotePublicKey), &bytesRead)
        rsa.Encrypt(aesKey, RSAEncryptionPadding.OaepSHA256)

    /// Дешифровать AES-ключ, полученный от клиента.
    member _.DecryptSessionKey(encryptedKey: byte array) : byte array =
        _rsa.Decrypt(encryptedKey, RSAEncryptionPadding.OaepSHA256)

    /// Сгенерировать случайный AES-256 ключ (32 байта).
    static member GenerateAesKey() : byte array =
        let key = Array.zeroCreate<byte> 32
        RandomNumberGenerator.Fill(key.AsSpan())
        key

    /// Выполнить полный key exchange на стороне сервера:
    /// 1. Экспортировать публичный ключ
    /// 2. Принять зашифрованный AES-ключ
    /// 3. Расшифровать → вернуть AesTransport
    member this.CompleteServerHandshake(encryptedAesKey: byte array) : AesTransport =
        let aesKey = this.DecryptSessionKey(encryptedAesKey)
        new AesTransport(aesKey)

    /// Выполнить полный key exchange на стороне клиента:
    /// 1. Получить публичный ключ сервера
    /// 2. Сгенерировать AES-ключ
    /// 3. Зашифровать его публичным ключом → отправить серверу
    /// 4. Вернуть (зашифрованный ключ для отправки, AesTransport для использования)
    static member CompleteClientHandshake(serverPublicKey: byte array) : byte array * AesTransport =
        let aesKey = RsaKeyExchange.GenerateAesKey()
        let encryptedKey = RsaKeyExchange.EncryptSessionKey(serverPublicKey, aesKey)
        let transport = new AesTransport(aesKey)
        (encryptedKey, transport)

    interface IDisposable with
        member _.Dispose() = _rsa.Dispose()
