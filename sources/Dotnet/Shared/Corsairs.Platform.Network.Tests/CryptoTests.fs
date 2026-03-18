module Corsairs.Platform.Network.Tests.CryptoTests

open System
open System.Diagnostics
open System.Security.Cryptography
open Xunit
open Corsairs.Platform.Network.Crypto

// ── Хелперы для тестов ──

/// Зашифровать data через ICryptoMiddleware, вернуть byte array результата.
let private encrypt (crypto: ICryptoMiddleware) (data: byte array) =
    let outLen = data.Length + crypto.Overhead
    let output = Array.zeroCreate<byte> outLen
    let written = crypto.Encrypt(ReadOnlySpan<byte>(data), Memory<byte>(output))
    Assert.True(written >= 0, "Encrypt failed")
    output[..written - 1]

/// Дешифровать data через ICryptoMiddleware, вернуть byte array результата.
let private decrypt (crypto: ICryptoMiddleware) (data: byte array) =
    let outLen = data.Length - crypto.Overhead
    let output = Array.zeroCreate<byte> (max outLen 0)
    let written = crypto.Decrypt(ReadOnlySpan<byte>(data), Memory<byte>(output))
    Assert.True(written >= 0, "Decrypt failed")
    output[..written - 1]

/// Дешифровать ReadOnlySpan через ICryptoMiddleware.
let private decryptSpan (crypto: ICryptoMiddleware) (data: ReadOnlySpan<byte>) =
    let outLen = data.Length - crypto.Overhead
    let output = Array.zeroCreate<byte> (max outLen 0)
    let written = crypto.Decrypt(data, Memory<byte>(output))
    Assert.True(written >= 0, "Decrypt failed")
    output[..written - 1]

// ══════════════════════════════════════════════════════════
//  AesTransport — AES-256-GCM
// ══════════════════════════════════════════════════════════

module AesTransportTests =

    let private makeKey () =
        let key = Array.zeroCreate<byte> 32
        RandomNumberGenerator.Fill(key.AsSpan())
        key

    [<Fact>]
    let ``Encrypt then Decrypt roundtrip`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware

        let original = "Hello, AES-256-GCM!"B
        let encrypted = encrypt crypto original
        let decrypted = decrypt crypto encrypted

        Assert.Equal<byte>(original, decrypted)

    [<Fact>]
    let ``Encrypt then Decrypt empty payload`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware

        let original = Array.empty<byte>
        let encrypted = encrypt crypto original
        let decrypted = decrypt crypto encrypted

        Assert.Equal<byte>(original, decrypted)

    [<Fact>]
    let ``Encrypt then Decrypt large payload`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware

        let original = Array.zeroCreate<byte> 65536
        RandomNumberGenerator.Fill(original.AsSpan())
        let encrypted = encrypt crypto original
        let decrypted = decrypt crypto encrypted

        Assert.Equal<byte>(original, decrypted)

    [<Fact>]
    let ``Encrypted format is nonce(12) + tag(16) + ciphertext`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware

        let original = Array.zeroCreate<byte> 100
        let encrypted = encrypt crypto original

        // nonce(12) + tag(16) + ciphertext(100) = 128
        Assert.Equal(12 + 16 + 100, encrypted.Length)

    [<Fact>]
    let ``Each encryption produces different ciphertext (random nonce)`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware

        let original = "Same data"B
        let enc1 = encrypt crypto original
        let enc2 = encrypt crypto original

        // Nonces различаются → зашифрованные данные не совпадают
        Assert.NotEqual<byte>(enc1, enc2)

        // Но расшифровка обоих даёт одинаковый результат
        let dec1 = decrypt crypto enc1
        let dec2 = decrypt crypto enc2
        Assert.Equal<byte>(dec1, dec2)
        Assert.Equal<byte>(original, dec1)

    [<Fact>]
    let ``Decrypt with wrong key fails`` () =
        let key1 = makeKey ()
        let key2 = makeKey ()
        use t1 = new AesTransport(key1)
        use t2 = new AesTransport(key2)
        let c1 = t1 :> ICryptoMiddleware
        let c2 = t2 :> ICryptoMiddleware

        let encrypted = encrypt c1 "Secret"B
        let output = Array.zeroCreate<byte> encrypted.Length
        let written = c2.Decrypt(ReadOnlySpan<byte>(encrypted), Memory<byte>(output))
        Assert.Equal(-1, written)

    [<Fact>]
    let ``Decrypt truncated data fails`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware

        // Слишком короткие данные (меньше nonce + tag = 28)
        let short = Array.zeroCreate<byte> 20
        let output = Array.zeroCreate<byte> 20
        let written = crypto.Decrypt(ReadOnlySpan<byte>(short), Memory<byte>(output))
        Assert.Equal(-1, written)

    [<Fact>]
    let ``Decrypt tampered ciphertext fails`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware

        let encrypted = encrypt crypto "Original"B
        // Повредить один байт в ciphertext
        encrypted[encrypted.Length - 1] <- encrypted[encrypted.Length - 1] ^^^ 0xFFuy

        let output = Array.zeroCreate<byte> encrypted.Length
        let written = crypto.Decrypt(ReadOnlySpan<byte>(encrypted), Memory<byte>(output))
        Assert.Equal(-1, written)

    [<Fact>]
    let ``Decrypt tampered tag fails`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware

        let encrypted = encrypt crypto "Original"B
        // Повредить один байт в tag (offset 12..28)
        encrypted[15] <- encrypted[15] ^^^ 0xFFuy

        let output = Array.zeroCreate<byte> encrypted.Length
        let written = crypto.Decrypt(ReadOnlySpan<byte>(encrypted), Memory<byte>(output))
        Assert.Equal(-1, written)

    [<Fact>]
    let ``Constructor rejects non-32-byte key`` () =
        Assert.ThrowsAny<ArgumentException>(fun () ->
            new AesTransport(Array.zeroCreate<byte> 16) |> ignore)
        |> ignore

        Assert.ThrowsAny<ArgumentException>(fun () ->
            new AesTransport(Array.zeroCreate<byte> 24) |> ignore)
        |> ignore

    [<Fact>]
    let ``IsActive is true`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware
        Assert.True(crypto.IsActive)

    [<Fact>]
    let ``Overhead is 28`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware
        Assert.Equal(28, crypto.Overhead)

    [<Fact>]
    let ``Two transports with same key can cross-decrypt`` () =
        let key = makeKey ()
        use t1 = new AesTransport(key)
        use t2 = new AesTransport(key |> Array.copy)
        let c1 = t1 :> ICryptoMiddleware
        let c2 = t2 :> ICryptoMiddleware

        let original = "Cross-decrypt test"B
        let encrypted = encrypt c1 original
        let decrypted = decrypt c2 encrypted

        Assert.Equal<byte>(original, decrypted)

    [<Fact>]
    let ``Encrypt returns -1 if output too small`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware
        let data = "test"B
        let tooSmall = Array.zeroCreate<byte> 5
        let written = crypto.Encrypt(ReadOnlySpan<byte>(data), Memory<byte>(tooSmall))
        Assert.Equal(-1, written)

    [<Fact>]
    let ``Decrypt returns -1 if output too small`` () =
        let key = makeKey ()
        use transport = new AesTransport(key)
        let crypto = transport :> ICryptoMiddleware
        let encrypted = encrypt crypto "test"B
        let tooSmall = Array.zeroCreate<byte> 1
        let written = crypto.Decrypt(ReadOnlySpan<byte>(encrypted), Memory<byte>(tooSmall))
        Assert.Equal(-1, written)

// ══════════════════════════════════════════════════════════
//  NoCryptoMiddleware
// ══════════════════════════════════════════════════════════

module NoCryptoTests =

    [<Fact>]
    let ``NoCrypto passes data through unchanged`` () =
        let crypto = NoCryptoMiddleware() :> ICryptoMiddleware

        let data = "Hello"B
        let encrypted = encrypt crypto data
        let decrypted = decrypt crypto encrypted

        Assert.Equal<byte>(data, encrypted)
        Assert.Equal<byte>(data, decrypted)

    [<Fact>]
    let ``NoCrypto IsActive is false`` () =
        let crypto = NoCryptoMiddleware() :> ICryptoMiddleware
        Assert.False(crypto.IsActive)

    [<Fact>]
    let ``NoCrypto Overhead is 0`` () =
        let crypto = NoCryptoMiddleware() :> ICryptoMiddleware
        Assert.Equal(0, crypto.Overhead)

// ══════════════════════════════════════════════════════════
//  RsaKeyExchange — RSA-3072 + AES handshake
// ══════════════════════════════════════════════════════════

module RsaKeyExchangeTests =

    [<Fact>]
    let ``ExportPublicKey returns valid SPKI DER`` () =
        use rsa = new RsaKeyExchange()
        let pubKey = rsa.ExportPublicKey()

        Assert.NotNull(pubKey)
        Assert.True(pubKey.Length > 100, "RSA-3072 SPKI public key should be > 100 bytes")

        // Проверяем что это валидный SPKI DER — его можно импортировать обратно
        use rsa2 = RSA.Create()
        let mutable bytesRead = 0
        rsa2.ImportSubjectPublicKeyInfo(ReadOnlySpan<byte>(pubKey), &bytesRead)
        Assert.Equal(pubKey.Length, bytesRead)

    [<Fact>]
    let ``Encrypt then Decrypt session key roundtrip`` () =
        use rsa = new RsaKeyExchange()
        let pubKey = rsa.ExportPublicKey()

        let aesKey = RsaKeyExchange.GenerateAesKey()
        Assert.Equal(32, aesKey.Length)

        let encrypted = RsaKeyExchange.EncryptSessionKey(pubKey, aesKey)
        Assert.NotNull(encrypted)
        Assert.True(encrypted.Length > aesKey.Length)

        let decrypted = rsa.DecryptSessionKey(encrypted)
        Assert.Equal<byte>(aesKey, decrypted)

    [<Fact>]
    let ``GenerateAesKey returns 32 bytes`` () =
        let key = RsaKeyExchange.GenerateAesKey()
        Assert.Equal(32, key.Length)

    [<Fact>]
    let ``GenerateAesKey returns unique keys`` () =
        let k1 = RsaKeyExchange.GenerateAesKey()
        let k2 = RsaKeyExchange.GenerateAesKey()
        Assert.NotEqual<byte>(k1, k2)

    [<Fact>]
    let ``CompleteServerHandshake returns working AesTransport`` () =
        use rsa = new RsaKeyExchange()
        let pubKey = rsa.ExportPublicKey()

        let aesKey = RsaKeyExchange.GenerateAesKey()
        let encryptedKey = RsaKeyExchange.EncryptSessionKey(pubKey, aesKey)

        use transport = rsa.CompleteServerHandshake(encryptedKey)
        let crypto = transport :> ICryptoMiddleware

        let data = "Server handshake test"B
        let encrypted = encrypt crypto data
        let decrypted = decrypt crypto encrypted

        Assert.Equal<byte>(data, decrypted)

    [<Fact>]
    let ``CompleteClientHandshake returns matching AesTransport`` () =
        use rsa = new RsaKeyExchange()
        let pubKey = rsa.ExportPublicKey()

        // Клиент выполняет handshake
        let encryptedKey, clientTransport = RsaKeyExchange.CompleteClientHandshake(pubKey)
        use _ct = clientTransport

        // Сервер завершает handshake
        use serverTransport = rsa.CompleteServerHandshake(encryptedKey)

        let clientCrypto = clientTransport :> ICryptoMiddleware
        let serverCrypto = serverTransport :> ICryptoMiddleware

        // Клиент шифрует → сервер расшифровывает
        let msg1 = "Client to Server"B
        let enc1 = encrypt clientCrypto msg1
        let dec1 = decrypt serverCrypto enc1
        Assert.Equal<byte>(msg1, dec1)

        // Сервер шифрует → клиент расшифровывает
        let msg2 = "Server to Client"B
        let enc2 = encrypt serverCrypto msg2
        let dec2 = decrypt clientCrypto enc2
        Assert.Equal<byte>(msg2, dec2)

    [<Fact>]
    let ``Decrypt with wrong RSA key fails`` () =
        use rsa1 = new RsaKeyExchange()
        use rsa2 = new RsaKeyExchange()

        let pubKey1 = rsa1.ExportPublicKey()
        let aesKey = RsaKeyExchange.GenerateAesKey()
        let encrypted = RsaKeyExchange.EncryptSessionKey(pubKey1, aesKey)

        // Расшифровка другим RSA-ключом → ошибка
        Assert.ThrowsAny<CryptographicException>(fun () ->
            rsa2.DecryptSessionKey(encrypted) |> ignore)

// ══════════════════════════════════════════════════════════
//  Full Handshake — полный цикл как в реальном протоколе
// ══════════════════════════════════════════════════════════

module FullHandshakeTests =

    [<Fact>]
    let ``Full protocol handshake and encrypted communication`` () =
        // === Сервер: генерация RSA ключей ===
        use serverRsa = new RsaKeyExchange()
        let serverPublicKey = serverRsa.ExportPublicKey()

        // === Клиент: генерация AES-ключа, шифрование RSA ===
        let encryptedAesKey, clientTransport = RsaKeyExchange.CompleteClientHandshake(serverPublicKey)
        use _ct = clientTransport

        // === Сервер: расшифровка AES-ключа ===
        use serverTransport = serverRsa.CompleteServerHandshake(encryptedAesKey)

        let clientCrypto = clientTransport :> ICryptoMiddleware
        let serverCrypto = serverTransport :> ICryptoMiddleware

        Assert.True(clientCrypto.IsActive)
        Assert.True(serverCrypto.IsActive)

        // === Двунаправленный обмен ===
        for i in 0..99 do
            let payload = Text.Encoding.UTF8.GetBytes($"Message #{i} from client")
            let enc = encrypt clientCrypto payload
            let dec = decrypt serverCrypto enc
            Assert.Equal<byte>(payload, dec)

        for i in 0..99 do
            let payload = Text.Encoding.UTF8.GetBytes($"Message #{i} from server")
            let enc = encrypt serverCrypto payload
            let dec = decrypt clientCrypto enc
            Assert.Equal<byte>(payload, dec)

    [<Fact>]
    let ``Shared RSA key works for multiple clients`` () =
        // Один RSA ключ на сервере, несколько клиентов
        use serverRsa = new RsaKeyExchange()
        let pubKey = serverRsa.ExportPublicKey()

        let clients =
            [| for _ in 0..4 ->
                let encKey, clientTransport = RsaKeyExchange.CompleteClientHandshake(pubKey)
                let serverTransport = serverRsa.CompleteServerHandshake(encKey)
                (clientTransport, serverTransport) |]

        // Каждый клиент может общаться с сервером
        for clientTransport, serverTransport in clients do
            let cc = clientTransport :> ICryptoMiddleware
            let sc = serverTransport :> ICryptoMiddleware

            let msg = "Hello from client"B
            let enc = encrypt cc msg
            let dec = decrypt sc enc
            Assert.Equal<byte>(msg, dec)

        // Клиенты НЕ могут расшифровывать сообщения друг друга
        let c0 = (fst clients[0]) :> ICryptoMiddleware
        let c1ServerSide = (snd clients[1]) :> ICryptoMiddleware

        let encByClient0 = encrypt c0 "Secret"B
        let output = Array.zeroCreate<byte> encByClient0.Length
        let written = c1ServerSide.Decrypt(ReadOnlySpan<byte>(encByClient0), Memory<byte>(output))
        Assert.Equal(-1, written)

        // Cleanup
        for ct, st in clients do
            (ct :> IDisposable).Dispose()
            (st :> IDisposable).Dispose()

// ══════════════════════════════════════════════════════════
//  AES-256-GCM с известными тестовыми векторами
//  (для верификации совместимости с другими реализациями)
// ══════════════════════════════════════════════════════════

module AesKnownVectorTests =

    /// Шифрование с фиксированным ключом, nonce и данными →
    /// расшифровка другим экземпляром AesTransport с тем же ключом.
    [<Fact>]
    let ``Known key roundtrip across instances`` () =
        // Фиксированный ключ (32 байта)
        let key = Array.init 32 (fun i -> byte i)

        use t1 = new AesTransport(key)
        use t2 = new AesTransport(key |> Array.copy)

        let data = [| 0x48uy; 0x65uy; 0x6Cuy; 0x6Cuy; 0x6Fuy |] // "Hello"

        let encrypted = encrypt (t1 :> ICryptoMiddleware) data
        let decrypted = decrypt (t2 :> ICryptoMiddleware) encrypted

        Assert.Equal<byte>(data, decrypted)

    /// Проверка формата: ручная сборка [nonce][tag][ciphertext]
    /// и расшифровка через AesTransport.
    [<Fact>]
    let ``Manual AES-GCM encrypt matches AesTransport decrypt`` () =
        let key = Array.init 32 (fun i -> byte (i * 3 + 7))
        let nonce = Array.init 12 (fun i -> byte (i + 1))
        let plaintext = "Test vector"B

        // Шифруем вручную через .NET AesGcm
        use aes = new AesGcm(key, 16)
        let ciphertext = Array.zeroCreate<byte> plaintext.Length
        let tag = Array.zeroCreate<byte> 16
        aes.Encrypt(nonce.AsSpan(), ReadOnlySpan<byte>(plaintext), ciphertext.AsSpan(), tag.AsSpan())

        // Собираем в формат AesTransport: [nonce(12)][tag(16)][ciphertext]
        let packet = Array.zeroCreate<byte> (12 + 16 + ciphertext.Length)
        Buffer.BlockCopy(nonce, 0, packet, 0, 12)
        Buffer.BlockCopy(tag, 0, packet, 12, 16)
        Buffer.BlockCopy(ciphertext, 0, packet, 28, ciphertext.Length)

        // Расшифровываем через AesTransport
        use transport = new AesTransport(key |> Array.copy)
        let decrypted = decrypt (transport :> ICryptoMiddleware) packet

        Assert.Equal<byte>(plaintext, decrypted)

    /// Обратное: AesTransport шифрует → ручная AesGcm расшифровывает.
    [<Fact>]
    let ``AesTransport encrypt can be manually decrypted`` () =
        let key = Array.init 32 (fun i -> byte (i * 5 + 11))
        let plaintext = "Reverse test"B

        use transport = new AesTransport(key)
        let encrypted = encrypt (transport :> ICryptoMiddleware) plaintext

        // Разбираем формат: [nonce(12)][tag(16)][ciphertext]
        let nonce = encrypted[0..11]
        let tag = encrypted[12..27]
        let ciphertext = encrypted[28..]

        Assert.Equal(12, nonce.Length)
        Assert.Equal(16, tag.Length)
        Assert.Equal(plaintext.Length, ciphertext.Length)

        // Расшифровываем вручную
        use aes = new AesGcm(key, 16)
        let decrypted = Array.zeroCreate<byte> ciphertext.Length
        aes.Decrypt(
            ReadOnlySpan<byte>(nonce),
            ReadOnlySpan<byte>(ciphertext),
            ReadOnlySpan<byte>(tag),
            decrypted.AsSpan())

        Assert.Equal<byte>(plaintext, decrypted)

// ══════════════════════════════════════════════════════════
//  C++ ↔ F# кросс-языковая верификация AES-256-GCM
//  Тестовые вектора сгенерированы CryptoInterop.exe (Windows BCrypt API)
// ══════════════════════════════════════════════════════════

module CppInteropTests =

    /// Конвертация hex-строки в byte array.
    let private fromHex (hex: string) =
        Array.init (hex.Length / 2) (fun i ->
            Convert.ToByte(hex.Substring(i * 2, 2), 16))

    /// Конвертация byte array в hex-строку.
    let private toHex (data: byte array) =
        data |> Array.map (fun b -> b.ToString("x2")) |> String.concat ""

    // Тестовые вектора из C++ CryptoInterop.exe (BCrypt AES-256-GCM)
    // Формат: key(32), nonce(12), plaintext, encrypted = [nonce(12)][tag(16)][ciphertext]

    /// Тест 1: "Hello, World!" с последовательными ключом [0..31] и nonce [1..12]
    [<Fact>]
    let ``F# decrypts C++ encrypted Hello World`` () =
        let key = fromHex "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
        let plaintext = "Hello, World!"B
        let cppEncrypted = fromHex "0102030405060708090a0b0c47c27f9a9e439c48384c2aa2b3473a894d8f36b983b8d0d123d00f2331"

        use transport = new AesTransport(key)
        let decrypted = decrypt (transport :> ICryptoMiddleware) cppEncrypted

        Assert.Equal<byte>(plaintext, decrypted)

    /// Тест 2: пустой plaintext
    [<Fact>]
    let ``F# decrypts C++ encrypted empty data`` () =
        let key = fromHex "070a0d101316191c1f2225282b2e3134373a3d404346494c4f5255585b5e6164"
        let cppEncrypted = fromHex "0507090b0d0f11131517191b58d757c1c14ac702a8771b936e0a0c50"

        use transport = new AesTransport(key)
        let decrypted = decrypt (transport :> ICryptoMiddleware) cppEncrypted

        Assert.Empty(decrypted)

    /// Тест 3: 256 байт бинарных данных [0x00..0xFF]
    [<Fact>]
    let ``F# decrypts C++ encrypted 256 binary bytes`` () =
        let key = fromHex "fffefdfcfbfaf9f8f7f6f5f4f3f2f1f0efeeedecebeae9e8e7e6e5e4e3e2e1e0"
        let plaintext = Array.init 256 (fun i -> byte i)
        let cppEncrypted = fromHex "aaaba8a9aeafacada2a3a0a1164d3d0e09d415e94219e92ad418b5338bd2349b782f3a4d01fca9ffef5b8ab2991d1ba3f2a757219976f3bc5cf5acf20d99acb6a8dbfd7d09e85ea2056cbe544d1501530d0fcfa51cde551eefbdd690793edb2064b07853de5fa7c6ec0d48d50ff6d636e12a619cc9bb79fbdd2222d12c660bc2cd681d6dcab44e09e40050627113ac40b2e74f3e4d76b0cb4ce68b578bab459eeb76c9e1f09e664c95c6c17047d5f1cd5c97b54878bfe22f7501394d470249f58cc4e69875ce605ccd25d696559302adb9dd34e2f68d3372f71b5d2475af1d9164c523e40c4bc2e7ea0de62f347a75cd8a912cb1eb85a851e5f0b2cf0082d970adea4d10dca6a0b7507d2e241c3b18373d644f0e90e91aa8459d4c65"

        use transport = new AesTransport(key)
        let decrypted = decrypt (transport :> ICryptoMiddleware) cppEncrypted

        Assert.Equal<byte>(plaintext, decrypted)

    /// Тест 4: пакет-подобные данные (CMD + payload)
    [<Fact>]
    let ``F# decrypts C++ encrypted packet data`` () =
        let key = fromHex "0d141b222930373e454c535a61686f767d848b9299a0a7aeb5bcc3cad1d8dfe6"
        let plaintext = [| 0x01uy; 0x00uy; 0xDEuy; 0xADuy; 0xBEuy; 0xEFuy; 0xCAuy; 0xFEuy |]
        let cppEncrypted = fromHex "101112131415161718191a1b1ed0340d2a8b6b2000f69a06c11af56e1071a2a59ccadc3f"

        use transport = new AesTransport(key)
        let decrypted = decrypt (transport :> ICryptoMiddleware) cppEncrypted

        Assert.Equal<byte>(plaintext, decrypted)

    // ── Утилита для запуска CryptoInterop.exe ──

    let private findExe () =
        let candidates = [|
            IO.Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "..", "..", "Tests", "CryptoInterop", "bin", "Release", "CryptoInterop.exe")
            @"D:\Projects\MMORPG\TalesOfPirate\sources\Dotnet\Tests\CryptoInterop\bin\Release\CryptoInterop.exe"
        |]
        candidates |> Array.tryFind IO.File.Exists

    let private runExe (exe: string) (args: string) =
        let psi = ProcessStartInfo(exe, args)
        psi.RedirectStandardOutput <- true
        psi.RedirectStandardError <- true
        psi.UseShellExecute <- false
        psi.CreateNoWindow <- true
        use proc = Process.Start(psi)
        let stdout = proc.StandardOutput.ReadToEnd().Trim()
        let stderr = proc.StandardError.ReadToEnd().Trim()
        proc.WaitForExit()
        (proc.ExitCode, stdout, stderr)

    // ── Новый формат (AES-256-GCM) через Crypto++ ──

    /// F# шифрует → C++ (Crypto++) расшифровывает.
    [<Fact>]
    let ``C++ Crypto++ decrypts F# encrypted data (new format)`` () =
        match findExe () with
        | None -> ()
        | Some exe ->
            let key = Array.init 32 (fun i -> byte (i * 2 + 1))
            let plaintext = "Cross-language interop test!"B

            use transport = new AesTransport(key)
            let encrypted = encrypt (transport :> ICryptoMiddleware) plaintext

            let exitCode, stdout, stderr = runExe exe $"new-decrypt {toHex key} {toHex encrypted}"
            Assert.Equal(0, exitCode)
            Assert.Empty(stderr)
            Assert.Equal<byte>(plaintext, fromHex stdout)

    /// C++ (Crypto++) шифрует → F# расшифровывает.
    [<Fact>]
    let ``F# decrypts C++ Crypto++ encrypted data (new format)`` () =
        match findExe () with
        | None -> ()
        | Some exe ->
            let key = Array.init 32 (fun i -> byte (i * 4 + 3))
            let nonce = Array.init 12 (fun i -> byte (i + 0x20))
            let plaintext = "Dynamic C++ to F# test"B

            let exitCode, stdout, _ = runExe exe $"new-encrypt {toHex key} {toHex nonce} {toHex plaintext}"
            Assert.Equal(0, exitCode)

            use transport = new AesTransport(key)
            let decrypted = decryptSpan (transport :> ICryptoMiddleware) (ReadOnlySpan<byte>(fromHex stdout))
            Assert.Equal<byte>(plaintext, decrypted)

    // ── Gate формат (AES-128-GCM, Base64, tag=12, IV=16) — как в C++ GateServer ──

    /// C++ gate-формат: roundtrip через CryptoInterop.exe (encrypt → decrypt).
    [<Fact>]
    let ``C++ gate format roundtrip via exe`` () =
        match findExe () with
        | None -> ()
        | Some exe ->
            let key = Array.init 16 (fun i -> byte (i * 5 + 11))
            let iv = Array.init 16 (fun i -> byte (i + 0x30))
            let plaintext = "Gate format roundtrip test!"B

            // C++ шифрует
            let exitCode1, encHex, _ = runExe exe $"gate-encrypt {toHex key} {toHex iv} {toHex plaintext}"
            Assert.Equal(0, exitCode1)

            // C++ расшифровывает
            let exitCode2, decHex, stderr = runExe exe $"gate-decrypt {toHex key} {encHex}"
            Assert.Equal(0, exitCode2)
            Assert.Empty(stderr)
            Assert.Equal<byte>(plaintext, fromHex decHex)

    /// C++ gate-формат: хардкод-вектор (Hello, World!) верифицируется через exe.
    [<Fact>]
    let ``C++ decrypts gate format known vector via exe`` () =
        match findExe () with
        | None -> ()
        | Some exe ->
            let key = "000102030405060708090a0b0c0d0e0f"
            let wire = "44794d6151366554796362724e6e6132517543326c4b6b527738524d4e74686743673d3d000102030405060708090a0b0c0d0e0f10"
            let expectedPlaintext = "Hello, World!"B

            let exitCode, stdout, stderr = runExe exe $"gate-decrypt {key} {wire}"
            Assert.Equal(0, exitCode)
            Assert.Empty(stderr)
            Assert.Equal<byte>(expectedPlaintext, fromHex stdout)

    /// C++ gate-формат: хардкод-вектор (пустые данные) верифицируется через exe.
    [<Fact>]
    let ``C++ decrypts gate format empty vector via exe`` () =
        match findExe () with
        | None -> ()
        | Some exe ->
            let key = "070a0d101316191c1f2225282b2e3134"
            let wire = "4f4c2b46446d51614a654a63692f4247000507090b0d0f11131517191b1d1f2123"

            let exitCode, stdout, stderr = runExe exe $"gate-decrypt {key} {wire}"
            Assert.Equal(0, exitCode)
            Assert.Empty(stderr)
            Assert.Empty(fromHex stdout)

    /// C++ gate-формат: хардкод-вектор (CMD+payload) верифицируется через exe.
    [<Fact>]
    let ``C++ decrypts gate format packet vector via exe`` () =
        match findExe () with
        | None -> ()
        | Some exe ->
            let key = "0d141b222930373e454c535a61686f76"
            let wire = "6a657467776946384f6f64394e75757073717473635730776b656b3d00101112131415161718191a1b1c1d1e1f"
            let expectedPlaintext = [| 0x01uy; 0x00uy; 0xDEuy; 0xADuy; 0xBEuy; 0xEFuy; 0xCAuy; 0xFEuy |]

            let exitCode, stdout, stderr = runExe exe $"gate-decrypt {key} {wire}"
            Assert.Equal(0, exitCode)
            Assert.Empty(stderr)
            Assert.Equal<byte>(expectedPlaintext, fromHex stdout)
