namespace Corsairs.GateServer.Services

open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Crypto

/// Сервис шифрования для GateServer.
/// Управляет RSA key exchange и AES transport.
type EncryptionService(logger: ILogger<EncryptionService>) =
    let _rsa = new RsaKeyExchange()

    /// Получить публичный ключ сервера для отправки клиенту.
    member _.GetServerPublicKey() = _rsa.ExportPublicKey()

    /// Расшифровать AES-ключ от клиента.
    member _.DecryptClientAesKey(encryptedKey: byte[]) =
        try
            let key = _rsa.DecryptSessionKey(encryptedKey)
            logger.LogDebug("AES ключ расшифрован ({Len} байт)", key.Length)
            ValueSome key
        with ex ->
            logger.LogError(ex, "Ошибка расшифровки AES ключа")
            ValueNone

    /// Создать AES middleware для сессии.
    member _.CreateAesTransport(aesKey: byte[]) =
        new AesTransport(aesKey)
