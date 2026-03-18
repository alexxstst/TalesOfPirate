module Corsairs.Platform.Network.Tests.SystemCommandTests

open System
open System.Net
open System.Threading
open System.Threading.Tasks
open Xunit
open Corsairs.Platform.Network.Network
open Corsairs.Platform.Network.Protocol
open Microsoft.Extensions.Logging

// ═══════════════════════════════════════════════════════════════
//  Утилиты
// ═══════════════════════════════════════════════════════════════

/// Тестовый ChannelIO без дополнительной логики.
[<AllowNullLiteral>]
type TestChannel(socket, handler) =
    inherit ChannelIO(socket, handler)

let private loggerFactory = LoggerFactory.Create(fun b -> b.SetMinimumLevel(LogLevel.Debug) |> ignore)

/// Найти свободный TCP-порт.
let private freePort () =
    let l = new System.Net.Sockets.TcpListener(IPAddress.Loopback, 0)
    l.Start()
    let port = (l.LocalEndpoint :?> IPEndPoint).Port
    l.Stop()
    port

/// Фабрика каналов.
let private channelFactory (socket, handler) = TestChannel(socket, handler)

/// Ожидание с таймаутом.
let private awaitTimeout (tcs: TaskCompletionSource<'T>) (ms: int) =
    task {
        let! completed = Task.WhenAny(tcs.Task, Task.Delay(ms))
        if completed = (tcs.Task :> Task) then return tcs.Task.Result
        else return failwith $"Таймаут ожидания ({ms}мс)"
    }

let private awaitSignal (tcs: TaskCompletionSource<unit>) (ms: int) =
    task {
        let! completed = Task.WhenAny(tcs.Task, Task.Delay(ms))
        if completed <> (tcs.Task :> Task) then
            failwith $"Таймаут ожидания сигнала ({ms}мс)"
    }

// ═══════════════════════════════════════════════════════════════
//  DirectSystemCommand — базовые тесты подключения
// ═══════════════════════════════════════════════════════════════

module DirectConnection =

    [<Fact>]
    let ``Сервер принимает подключение клиента`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)

        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! _ = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! ch = awaitTimeout serverConnected 5000
        Assert.NotNull(ch)
    }

    [<Fact>]
    let ``При закрытии канала срабатывает OnDisconnected`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let serverDisconnected = TaskCompletionSource<TestChannel>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)

        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnDisconnected.Add(fun ch -> serverDisconnected.TrySetResult(ch) |> ignore)
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000
        clientCh.Close()
        let! disconnected = awaitTimeout serverDisconnected 5000
        Assert.NotNull(disconnected)
    }

    [<Fact>]
    let ``Множественные клиенты подключаются одновременно`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let mutable connCount = 0
        let allConnected = TaskCompletionSource<unit>()
        let total = 5

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)

        server.OnConnected.Add(fun _ ->
            if Interlocked.Increment(&connCount) >= total then
                allConnected.TrySetResult(()) |> ignore)
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.Start(cts.Token)

        for _ in 1..total do
            let! _ = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
            ()

        do! awaitSignal allConnected 5000
        Assert.Equal(total, connCount)
    }

// ═══════════════════════════════════════════════════════════════
//  DirectSystemCommand — обмен пакетами (все типы данных)
// ═══════════════════════════════════════════════════════════════

module DirectPacketExchange =

    /// Поднять пару server+client, вернуть каналы.
    let private setup () = task {
        let port = freePort ()
        let cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()

        let server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)
        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.Start(cts.Token)

        let client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! serverCh = awaitTimeout serverConnected 5000
        return (server, client, serverCh, clientCh, cts)
    }

    [<Fact>]
    let ``Отправка UInt8 клиент→сервер`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<byte>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(byte (pkt.ReadInt64())) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(1us)
        w.WriteInt64(0xABL)
        clientCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal(0xABuy, value)
    }

    [<Fact>]
    let ``Отправка Int8 сервер→клиент`` () = task {
        let! (server, client, serverCh, _clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<sbyte>()

        client.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(sbyte (pkt.ReadInt64())) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(2us)
        w.WriteInt64(-42L)
        serverCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal(-42y, value)
    }

    [<Fact>]
    let ``Отправка UInt16`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<uint16>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(uint16 (pkt.ReadInt64())) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(3us)
        w.WriteInt64(54321L)
        clientCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal(54321us, value)
    }

    [<Fact>]
    let ``Отправка Int16`` () = task {
        let! (server, client, serverCh, _clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<int16>()

        client.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(int16 (pkt.ReadInt64())) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(4us)
        w.WriteInt64(-12345L)
        serverCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal(-12345s, value)
    }

    [<Fact>]
    let ``Отправка UInt32`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<uint32>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(uint32 (pkt.ReadInt64())) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(5us)
        w.WriteInt64(0xDEADBEEFL)
        clientCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal(0xDEADBEEFu, value)
    }

    [<Fact>]
    let ``Отправка Int32`` () = task {
        let! (server, client, serverCh, _clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<int32>()

        client.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(int32 (pkt.ReadInt64())) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(6us)
        w.WriteInt64(-999_999L)
        serverCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal(-999_999, value)
    }

    [<Fact>]
    let ``Отправка Int64`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<int64>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(pkt.ReadInt64()) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(7us)
        w.WriteInt64(Int64.MaxValue)
        clientCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal(Int64.MaxValue, value)
    }

    [<Fact>]
    let ``Отправка UInt64`` () = task {
        let! (server, client, serverCh, _clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<uint64>()

        client.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(pkt.ReadUInt64()) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(8us)
        w.WriteUInt64(0xCAFEBABEDEADC0DEuL)
        serverCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal(0xCAFEBABEDEADC0DEuL, value)
    }

    [<Fact>]
    let ``Отправка Float32`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<float32>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(pkt.ReadFloat32()) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(9us)
        w.WriteFloat32(3.14159f)
        clientCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal(3.14159f, value)
    }

    [<Fact>]
    let ``Отправка String ASCII`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<string>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(pkt.ReadString()) |> ignore)

        let w = WPacket(64)
        w.WriteCmd(10us)
        w.WriteString("Hello World")
        clientCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal("Hello World", value)
    }

    [<Fact>]
    let ``Отправка String UTF-8 кириллица`` () = task {
        let! (server, client, serverCh, _clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<string>()

        client.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(pkt.ReadString()) |> ignore)

        let w = WPacket(128)
        w.WriteCmd(11us)
        w.WriteString("Привет, мир! 日本語 emoji: 🎮")
        serverCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal("Привет, мир! 日本語 emoji: 🎮", value)
    }

    [<Fact>]
    let ``Отправка пустой строки`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<string>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(pkt.ReadString()) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(12us)
        w.WriteString("")
        clientCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal("", value)
    }

    [<Fact>]
    let ``Отправка Sequence`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<byte[]>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(pkt.ReadSequence().ToArray()) |> ignore)

        let data = [| 0xAAuy; 0xBBuy; 0xCCuy; 0xDDuy; 0xEEuy |]
        let w = WPacket(32)
        w.WriteCmd(13us)
        w.WriteSequence(ReadOnlySpan(data))
        clientCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal<byte[]>(data, value)
    }

    [<Fact>]
    let ``Cmd и Sess передаются корректно`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<uint16 * uint32>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(pkt.GetCmd(), pkt.Sess) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(0x1234us)
        w.WriteSess(0xABCDEF01u)
        clientCh.SendPacket(w)

        let! (cmd, sess) = awaitTimeout received 5000
        Assert.Equal(0x1234us, cmd)
        Assert.Equal(0xABCDEF01u, sess)
    }

    [<Fact>]
    let ``Пакет со всеми типами данных`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<IRPacket>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            // Копируем пакет т.к. DirectRPacket zero-copy
            let copy = BufferedRPacket.Create(pkt.GetPacketMemory()) :> IRPacket
            received.TrySetResult(copy) |> ignore)

        let w = WPacket(256)
        w.WriteCmd(500us)
        w.WriteSess(777u)
        w.WriteInt64(255L)
        w.WriteInt64(-128L)
        w.WriteInt64(65535L)
        w.WriteInt64(-32768L)
        w.WriteInt64(int64 UInt32.MaxValue)
        w.WriteInt64(int64 Int32.MinValue)
        w.WriteInt64(Int64.MaxValue)
        w.WriteUInt64(UInt64.MaxValue)
        w.WriteFloat32(-99.5f)
        w.WriteString("Тест всех типов")
        w.WriteSequence(ReadOnlySpan([| 1uy; 2uy; 3uy |]))
        clientCh.SendPacket(w)

        let! pkt = awaitTimeout received 5000
        Assert.Equal(500us, pkt.GetCmd())
        Assert.Equal(777u, pkt.Sess)
        Assert.Equal(255uy, byte (pkt.ReadInt64()))
        Assert.Equal(-128y, sbyte (pkt.ReadInt64()))
        Assert.Equal(65535us, uint16 (pkt.ReadInt64()))
        Assert.Equal(-32768s, int16 (pkt.ReadInt64()))
        Assert.Equal(UInt32.MaxValue, uint32 (pkt.ReadInt64()))
        Assert.Equal(Int32.MinValue, int32 (pkt.ReadInt64()))
        Assert.Equal(Int64.MaxValue, pkt.ReadInt64())
        Assert.Equal(UInt64.MaxValue, pkt.ReadUInt64())
        Assert.Equal(-99.5f, pkt.ReadFloat32())
        Assert.Equal("Тест всех типов", pkt.ReadString())
        let seq = pkt.ReadSequence()
        Assert.Equal(3, seq.Length)
        Assert.False(pkt.HasData)
        pkt.Dispose()
    }

    [<Fact>]
    let ``Пакет с граничными значениями`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<IRPacket>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            let copy = BufferedRPacket.Create(pkt.GetPacketMemory()) :> IRPacket
            received.TrySetResult(copy) |> ignore)

        let w = WPacket(128)
        w.WriteCmd(UInt16.MaxValue)
        w.WriteInt64(int64 Byte.MinValue)
        w.WriteInt64(int64 Byte.MaxValue)
        w.WriteInt64(int64 SByte.MinValue)
        w.WriteInt64(int64 SByte.MaxValue)
        w.WriteInt64(int64 UInt16.MinValue)
        w.WriteInt64(int64 UInt16.MaxValue)
        w.WriteInt64(int64 Int16.MinValue)
        w.WriteInt64(int64 Int16.MaxValue)
        w.WriteInt64(int64 UInt32.MinValue)
        w.WriteInt64(int64 UInt32.MaxValue)
        w.WriteInt64(int64 Int32.MinValue)
        w.WriteInt64(int64 Int32.MaxValue)
        w.WriteInt64(Int64.MinValue)
        w.WriteInt64(Int64.MaxValue)
        w.WriteUInt64(UInt64.MinValue)
        w.WriteUInt64(UInt64.MaxValue)
        w.WriteFloat32(Single.Epsilon)
        w.WriteFloat32(Single.MaxValue)
        clientCh.SendPacket(w)

        let! pkt = awaitTimeout received 5000
        Assert.Equal(UInt16.MaxValue, pkt.GetCmd())
        Assert.Equal(Byte.MinValue, byte (pkt.ReadInt64()))
        Assert.Equal(Byte.MaxValue, byte (pkt.ReadInt64()))
        Assert.Equal(SByte.MinValue, sbyte (pkt.ReadInt64()))
        Assert.Equal(SByte.MaxValue, sbyte (pkt.ReadInt64()))
        Assert.Equal(UInt16.MinValue, uint16 (pkt.ReadInt64()))
        Assert.Equal(UInt16.MaxValue, uint16 (pkt.ReadInt64()))
        Assert.Equal(Int16.MinValue, int16 (pkt.ReadInt64()))
        Assert.Equal(Int16.MaxValue, int16 (pkt.ReadInt64()))
        Assert.Equal(UInt32.MinValue, uint32 (pkt.ReadInt64()))
        Assert.Equal(UInt32.MaxValue, uint32 (pkt.ReadInt64()))
        Assert.Equal(Int32.MinValue, int32 (pkt.ReadInt64()))
        Assert.Equal(Int32.MaxValue, int32 (pkt.ReadInt64()))
        Assert.Equal(Int64.MinValue, pkt.ReadInt64())
        Assert.Equal(Int64.MaxValue, pkt.ReadInt64())
        Assert.Equal(UInt64.MinValue, pkt.ReadUInt64())
        Assert.Equal(UInt64.MaxValue, pkt.ReadUInt64())
        Assert.Equal(Single.Epsilon, pkt.ReadFloat32())
        Assert.Equal(Single.MaxValue, pkt.ReadFloat32())
        Assert.False(pkt.HasData)
        pkt.Dispose()
    }

    [<Fact>]
    let ``Несколько строк подряд`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let received = TaskCompletionSource<IRPacket>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            let copy = BufferedRPacket.Create(pkt.GetPacketMemory()) :> IRPacket
            received.TrySetResult(copy) |> ignore)

        let w = WPacket(256)
        w.WriteCmd(20us)
        w.WriteString("first")
        w.WriteString("")
        w.WriteString("Третья строка")
        w.WriteString("fourth")
        w.WriteString("日本語テスト")
        clientCh.SendPacket(w)

        let! pkt = awaitTimeout received 5000
        Assert.Equal("first", pkt.ReadString())
        Assert.Equal("", pkt.ReadString())
        Assert.Equal("Третья строка", pkt.ReadString())
        Assert.Equal("fourth", pkt.ReadString())
        Assert.Equal("日本語テスト", pkt.ReadString())
        Assert.False(pkt.HasData)
        pkt.Dispose()
    }

    [<Fact>]
    let ``Двунаправленный обмен: клиент→сервер→клиент`` () = task {
        let! (server, client, serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let clientReceived = TaskCompletionSource<uint32>()

        // Сервер получает команду и отвечает
        server.OnCommand.Add(fun (ch, pkt) ->
            let value = uint32 (pkt.ReadInt64())
            let w = WPacket(16)
            w.WriteCmd(200us)
            w.WriteInt64(int64 (value + 1u))
            ch.SendPacket(w))

        // Клиент получает ответ
        client.OnCommand.Add(fun (_ch, pkt) ->
            clientReceived.TrySetResult(uint32 (pkt.ReadInt64())) |> ignore)

        let w = WPacket(16)
        w.WriteCmd(100us)
        w.WriteInt64(42L)
        clientCh.SendPacket(w)

        let! result = awaitTimeout clientReceived 5000
        Assert.Equal(43u, result)
    }

    [<Fact>]
    let ``Множественные пакеты подряд (burst 20)`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let count = 20
        let mutable receivedCount = 0
        let allReceived = TaskCompletionSource<unit>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            let idx = int32 (pkt.ReadInt64())
            Assert.True(idx >= 0 && idx < count)
            if Interlocked.Increment(&receivedCount) >= count then
                allReceived.TrySetResult(()) |> ignore)

        for i in 0..count-1 do
            let w = WPacket(16)
            w.WriteCmd(30us)
            w.WriteInt64(int64 i)
            clientCh.SendPacket(w)

        do! awaitSignal allReceived 5000
        Assert.Equal(count, receivedCount)
    }

    [<Fact>]
    let ``Burst с разными типами команд`` () = task {
        let! (server, client, _serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let results = System.Collections.Concurrent.ConcurrentBag<uint16>()
        let total = 10
        let allReceived = TaskCompletionSource<unit>()

        server.OnCommand.Add(fun (_ch, pkt) ->
            results.Add(pkt.GetCmd())
            if results.Count >= total then
                allReceived.TrySetResult(()) |> ignore)

        for i in 1us..uint16 total do
            let w = WPacket(16)
            w.WriteCmd(i * 100us)
            w.WriteInt64(int64 i)
            clientCh.SendPacket(w)

        do! awaitSignal allReceived 5000
        Assert.Equal(total, results.Count)
        // Все 10 разных Cmd получены
        let sorted = results |> Seq.sort |> Seq.toArray
        for i in 0..total-1 do
            Assert.Equal(uint16 (i + 1) * 100us, sorted[i])
    }

    [<Fact>]
    let ``ForwardPacket пересылает пакет обратно клиенту`` () = task {
        let! (server, client, serverCh, clientCh, cts) = setup ()
        use _s = server
        use _c = client
        use _cts = cts
        let forwarded = TaskCompletionSource<IRPacket>()

        // Клиент получает пересланный пакет
        client.OnCommand.Add(fun (_ch, pkt) ->
            let copy = BufferedRPacket.Create(pkt.GetPacketMemory()) :> IRPacket
            forwarded.TrySetResult(copy) |> ignore)

        // Сервер получает и пересылает обратно клиенту через ForwardPacket
        server.OnCommand.Add(fun (_ch, pkt) ->
            serverCh.ForwardPacket(pkt))

        let w = WPacket(32)
        w.WriteCmd(999us)
        w.WriteString("forward me")
        clientCh.SendPacket(w)

        let! pkt = awaitTimeout forwarded 5000
        Assert.Equal(999us, pkt.GetCmd())
        Assert.Equal("forward me", pkt.ReadString())
        pkt.Dispose()
    }

// ═══════════════════════════════════════════════════════════════
//  DirectSystemCommand — Ping
// ═══════════════════════════════════════════════════════════════

module DirectPing =

    [<Fact>]
    let ``Клиент отправляет ping, сервер получает OnPing`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let pingReceived = TaskCompletionSource<TestChannel>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)
        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnPing.Add(fun ch -> pingReceived.TrySetResult(ch) |> ignore)
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000

        clientCh.SendPing()

        let! pingedCh = awaitTimeout pingReceived 5000
        Assert.NotNull(pingedCh)
    }

    [<Fact>]
    let ``Сервер отправляет ping, клиент получает OnPing`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let pingReceived = TaskCompletionSource<TestChannel>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)
        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.OnPing.Add(fun ch -> pingReceived.TrySetResult(ch) |> ignore)
        client.Start(cts.Token)

        let! _ = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! serverCh = awaitTimeout serverConnected 5000

        server.SendPing(serverCh)

        let! pingedCh = awaitTimeout pingReceived 5000
        Assert.NotNull(pingedCh)
    }

    [<Fact>]
    let ``Ping не мешает обмену данными`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let cmdReceived = TaskCompletionSource<uint32>()
        let mutable pingCount = 0

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)
        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnPing.Add(fun _ -> Interlocked.Increment(&pingCount) |> ignore)
        server.OnCommand.Add(fun (_ch, pkt) ->
            cmdReceived.TrySetResult(uint32 (pkt.ReadInt64())) |> ignore)
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000

        // Отправляем ping, данные, ping
        clientCh.SendPing()
        let w = WPacket(16)
        w.WriteCmd(50us)
        w.WriteInt64(12345L)
        clientCh.SendPacket(w)
        clientCh.SendPing()

        let! value = awaitTimeout cmdReceived 5000
        Assert.Equal(12345u, value)
        // Даём время на обработку второго пинга
        do! Task.Delay(100)
        Assert.True(pingCount >= 1)
    }

    [<Fact>]
    let ``Множественные пинги подряд`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let mutable pingCount = 0
        let total = 10
        let allPings = TaskCompletionSource<unit>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)
        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnPing.Add(fun _ ->
            if Interlocked.Increment(&pingCount) >= total then
                allPings.TrySetResult(()) |> ignore)
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000

        for _ in 1..total do
            clientCh.SendPing()

        do! awaitSignal allPings 5000
        Assert.Equal(total, pingCount)
    }

// ═══════════════════════════════════════════════════════════════
//  DirectSystemCommand — RPC (SyncCall паттерн через SESS)
// ═══════════════════════════════════════════════════════════════

module DirectRpc =

    let private SESS_FLAG = 0x80000000u

    [<Fact>]
    let ``RPC запрос→ответ через SESS`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let rpcResult = TaskCompletionSource<int16>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)

        // Сервер: при получении RPC-запроса (Sess>0) — отвечает с Sess|SESS_FLAG
        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnCommand.Add(fun (ch, pkt) ->
            let sess = pkt.Sess
            if sess > 0u && sess < SESS_FLAG then
                // Это RPC запрос — обрабатываем и отвечаем
                let value = int32 (pkt.ReadInt64())
                let w = WPacket(16)
                w.WriteCmd(0us) // Cmd не важен для ответа
                w.WriteSess(sess ||| SESS_FLAG)
                w.WriteInt64(0L) // ERR_SUCCESS
                w.WriteInt64(int64 (value * 2)) // Результат
                ch.SendPacket(w))
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)

        // Клиент: ответ с SESS > SESS_FLAG — это ответ на наш RPC
        client.OnCommand.Add(fun (_ch, pkt) ->
            let sess = pkt.Sess
            if sess > SESS_FLAG then
                let err = int16 (pkt.ReadInt64())
                rpcResult.TrySetResult(err) |> ignore)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000

        // Отправляем RPC запрос с Sess=1
        let w = WPacket(16)
        w.WriteCmd(100us)
        w.WriteSess(1u)
        w.WriteInt64(21L)
        clientCh.SendPacket(w)

        let! err = awaitTimeout rpcResult 5000
        Assert.Equal(0s, err) // ERR_SUCCESS
    }

    [<Fact>]
    let ``RPC запрос с данными и полным ответом`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let rpcResult = TaskCompletionSource<IRPacket>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)

        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnCommand.Add(fun (ch, pkt) ->
            let sess = pkt.Sess
            if sess > 0u && sess < SESS_FLAG then
                let name = pkt.ReadString()
                let age = int32 (pkt.ReadInt64())
                // Ответ: err + вычисленные данные
                let w = WPacket(128)
                w.WriteCmd(0us)
                w.WriteSess(sess ||| SESS_FLAG)
                w.WriteInt64(0L) // success
                w.WriteString($"Hello, {name}!")
                w.WriteInt64(int64 (age + 10))
                w.WriteUInt64(0xDEADCAFEuL)
                ch.SendPacket(w))
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)

        client.OnCommand.Add(fun (_ch, pkt) ->
            if pkt.Sess > SESS_FLAG then
                let copy = BufferedRPacket.Create(pkt.GetPacketMemory()) :> IRPacket
                rpcResult.TrySetResult(copy) |> ignore)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000

        let w = WPacket(64)
        w.WriteCmd(200us)
        w.WriteSess(42u)
        w.WriteString("Алексей")
        w.WriteInt64(25L)
        clientCh.SendPacket(w)

        let! pkt = awaitTimeout rpcResult 5000
        Assert.Equal(42u ||| SESS_FLAG, pkt.Sess)
        Assert.Equal(0s, int16 (pkt.ReadInt64()))
        Assert.Equal("Hello, Алексей!", pkt.ReadString())
        Assert.Equal(35, int32 (pkt.ReadInt64()))
        Assert.Equal(0xDEADCAFEuL, pkt.ReadUInt64())
        pkt.Dispose()
    }

    [<Fact>]
    let ``Множественные параллельные RPC`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let total = 10
        let results = System.Collections.Concurrent.ConcurrentDictionary<uint32, int32>()
        let allDone = TaskCompletionSource<unit>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)

        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnCommand.Add(fun (ch, pkt) ->
            let sess = pkt.Sess
            if sess > 0u && sess < SESS_FLAG then
                let input = int32 (pkt.ReadInt64())
                let w = WPacket(16)
                w.WriteCmd(0us)
                w.WriteSess(sess ||| SESS_FLAG)
                w.WriteInt64(0L)
                w.WriteInt64(int64 (input * input)) // Квадрат
                ch.SendPacket(w))
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)

        client.OnCommand.Add(fun (_ch, pkt) ->
            let sess = pkt.Sess
            if sess > SESS_FLAG then
                let origSess = sess - SESS_FLAG
                let _err = int16 (pkt.ReadInt64())
                let result = int32 (pkt.ReadInt64())
                results[origSess] <- result
                if results.Count >= total then
                    allDone.TrySetResult(()) |> ignore)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000

        // Отправляем 10 RPC параллельно с разными SESS
        for i in 1..total do
            let w = WPacket(16)
            w.WriteCmd(300us)
            w.WriteSess(uint32 i)
            w.WriteInt64(int64 i)
            clientCh.SendPacket(w)

        do! awaitSignal allDone 5000

        // Проверяем: каждый ответ = i * i
        for i in 1..total do
            Assert.True(results.ContainsKey(uint32 i))
            Assert.Equal(i * i, results[uint32 i])
    }

    [<Fact>]
    let ``RPC ошибка — ненулевой код`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let rpcResult = TaskCompletionSource<int16>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)

        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnCommand.Add(fun (ch, pkt) ->
            let sess = pkt.Sess
            if sess > 0u && sess < SESS_FLAG then
                let w = WPacket(16)
                w.WriteCmd(0us)
                w.WriteSess(sess ||| SESS_FLAG)
                w.WriteInt64(501L) // ERR_PT_LOGFAIL
                ch.SendPacket(w))
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)

        client.OnCommand.Add(fun (_ch, pkt) ->
            if pkt.Sess > SESS_FLAG then
                rpcResult.TrySetResult(int16 (pkt.ReadInt64())) |> ignore)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000

        let w = WPacket(16)
        w.WriteCmd(400us)
        w.WriteSess(5u)
        clientCh.SendPacket(w)

        let! err = awaitTimeout rpcResult 5000
        Assert.Equal(501s, err)
    }

// ═══════════════════════════════════════════════════════════════
//  ChannelSystemCommand — poll-based
// ═══════════════════════════════════════════════════════════════

module ChannelSystem =

    [<Fact>]
    let ``Подключение через ChannelSystemCommand`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)

        use server = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        server.Start(cts.Token)

        use client = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [||]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! _ = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! evt = server.ReadEventAsync()

        match evt with
        | SystemEvent.Connected ch -> Assert.NotNull(ch)
        | _ -> Assert.Fail("Ожидался Connected")
    }

    [<Fact>]
    let ``Обмен пакетами через ChannelSystemCommand`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)

        use server = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        server.Start(cts.Token)

        use client = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [||]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _connEvt = server.ReadEventAsync() // Connected

        // Отправляем пакет
        let w = WPacket(64)
        w.WriteCmd(100us)
        w.WriteString("Channel test")
        w.WriteInt64(42L)
        clientCh.SendPacket(w)

        let! cmdEvt = server.ReadEventAsync()
        match cmdEvt with
        | SystemEvent.CommandReceived(_ch, pkt) ->
            Assert.Equal(100us, pkt.GetCmd())
            Assert.Equal("Channel test", pkt.ReadString())
            Assert.Equal(42u, uint32 (pkt.ReadInt64()))
            pkt.Dispose()
        | _ -> Assert.Fail("Ожидался CommandReceived")
    }

    [<Fact>]
    let ``Ping через ChannelSystemCommand`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)

        use server = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        server.Start(cts.Token)

        use client = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [||]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _connEvt = server.ReadEventAsync()

        clientCh.SendPing()

        let! pingEvt = server.ReadEventAsync()
        match pingEvt with
        | SystemEvent.PingReceived ch -> Assert.NotNull(ch)
        | _ -> Assert.Fail("Ожидался PingReceived")
    }

    [<Fact>]
    let ``Disconnected через ChannelSystemCommand`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)

        use server = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        server.Start(cts.Token)

        use client = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [||]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _connEvt = server.ReadEventAsync()

        clientCh.Close()

        let! discEvt = server.ReadEventAsync()
        match discEvt with
        | SystemEvent.Disconnected _ -> ()
        | _ -> Assert.Fail("Ожидался Disconnected")
    }

    [<Fact>]
    let ``ChannelSystemCommand: все типы в одном пакете`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)

        use server = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        server.Start(cts.Token)

        use client = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [||]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _connEvt = server.ReadEventAsync()

        let w = WPacket(256)
        w.WriteCmd(777us)
        w.WriteSess(999u)
        w.WriteInt64(0xFFL)
        w.WriteInt64(-1L)
        w.WriteInt64(0xABCDL)
        w.WriteInt64(-9999L)
        w.WriteInt64(0xDEADBEEFL)
        w.WriteInt64(-123456L)
        w.WriteInt64(0x123456789ABCDEF0L)
        w.WriteUInt64(UInt64.MaxValue)
        w.WriteFloat32(42.5f)
        w.WriteString("Тест Channel")
        w.WriteSequence(ReadOnlySpan([| 10uy; 20uy; 30uy |]))
        clientCh.SendPacket(w)

        let! cmdEvt = server.ReadEventAsync()
        match cmdEvt with
        | SystemEvent.CommandReceived(_ch, pkt) ->
            Assert.Equal(777us, pkt.GetCmd())
            Assert.Equal(999u, pkt.Sess)
            Assert.Equal(0xFFuy, byte (pkt.ReadInt64()))
            Assert.Equal(-1y, sbyte (pkt.ReadInt64()))
            Assert.Equal(0xABCDus, uint16 (pkt.ReadInt64()))
            Assert.Equal(-9999s, int16 (pkt.ReadInt64()))
            Assert.Equal(0xDEADBEEFu, uint32 (pkt.ReadInt64()))
            Assert.Equal(-123456, int32 (pkt.ReadInt64()))
            Assert.Equal(0x123456789ABCDEF0L, pkt.ReadInt64())
            Assert.Equal(UInt64.MaxValue, pkt.ReadUInt64())
            Assert.Equal(42.5f, pkt.ReadFloat32())
            Assert.Equal("Тест Channel", pkt.ReadString())
            let seq = pkt.ReadSequence()
            Assert.Equal(3, seq.Length)
            Assert.Equal(10uy, seq.Span[0])
            Assert.Equal(20uy, seq.Span[1])
            Assert.Equal(30uy, seq.Span[2])
            Assert.False(pkt.HasData)
            pkt.Dispose()
        | _ -> Assert.Fail("Ожидался CommandReceived")
    }

    [<Fact>]
    let ``ChannelSystemCommand: RPC запрос-ответ`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let SESS_FLAG = 0x80000000u

        use server = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        server.Start(cts.Token)

        use client = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [||]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! connEvt = server.ReadEventAsync()
        let! _clientConn = client.ReadEventAsync() // Connected на стороне клиента
        let serverCh =
            match connEvt with
            | SystemEvent.Connected ch -> ch
            | _ -> failwith "Ожидался Connected"

        // Отправляем RPC
        let w = WPacket(32)
        w.WriteCmd(500us)
        w.WriteSess(7u)
        w.WriteString("ping-data")
        clientCh.SendPacket(w)

        // Сервер получает запрос
        let! cmdEvt = server.ReadEventAsync()
        match cmdEvt with
        | SystemEvent.CommandReceived(ch, pkt) ->
            Assert.Equal(7u, pkt.Sess)
            let data = pkt.ReadString()
            pkt.Dispose()

            // Отвечаем
            let resp = WPacket(32)
            resp.WriteCmd(0us)
            resp.WriteSess(7u ||| SESS_FLAG)
            resp.WriteInt64(0L)
            resp.WriteString($"echo: {data}")
            ch.SendPacket(resp)
        | _ -> failwith "Ожидался CommandReceived"

        // Клиент получает ответ
        let! respEvt = client.ReadEventAsync()
        match respEvt with
        | SystemEvent.CommandReceived(_ch, pkt) ->
            Assert.Equal(7u ||| SESS_FLAG, pkt.Sess)
            Assert.Equal(0s, int16 (pkt.ReadInt64()))
            Assert.Equal("echo: ping-data", pkt.ReadString())
            pkt.Dispose()
        | _ -> Assert.Fail("Ожидался CommandReceived")
    }

    [<Fact>]
    let ``ChannelSystemCommand: burst пакетов`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let count = 20

        use server = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |]; ChannelCapacity = 1000 },
            channelFactory, loggerFactory)
        server.Start(cts.Token)

        use client = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [||]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _connEvt = server.ReadEventAsync()

        for i in 0..count-1 do
            let w = WPacket(32)
            w.WriteCmd(uint16 (i + 1))
            w.WriteInt64(int64 (i * i))
            w.WriteString($"пакет #{i}")
            clientCh.SendPacket(w)

        let mutable received = 0
        while received < count do
            let! evt = server.ReadEventAsync()
            match evt with
            | SystemEvent.CommandReceived(_ch, pkt) ->
                let idx = received
                Assert.Equal(uint16 (idx + 1), pkt.GetCmd())
                Assert.Equal(idx * idx, int32 (pkt.ReadInt64()))
                Assert.Equal($"пакет #{idx}", pkt.ReadString())
                pkt.Dispose()
                received <- received + 1
            | _ -> ()

        Assert.Equal(count, received)
    }

// ═══════════════════════════════════════════════════════════════
//  Смешанные сценарии
// ═══════════════════════════════════════════════════════════════

module MixedScenarios =

    [<Fact>]
    let ``Большой пакет (4KB payload)`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let received = TaskCompletionSource<IRPacket>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)
        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnCommand.Add(fun (_ch, pkt) ->
            let copy = BufferedRPacket.Create(pkt.GetPacketMemory()) :> IRPacket
            received.TrySetResult(copy) |> ignore)
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000

        // Генерируем 4KB данных
        let bigData = Array.init 4096 (fun i -> byte (i % 256))
        let w = WPacket(4200)
        w.WriteCmd(600us)
        w.WriteSequence(ReadOnlySpan(bigData))
        clientCh.SendPacket(w)

        let! pkt = awaitTimeout received 5000
        Assert.Equal(600us, pkt.GetCmd())
        let seq = pkt.ReadSequence()
        Assert.Equal(4096, seq.Length)
        for i in 0..4095 do
            Assert.Equal(byte (i % 256), seq.Span[i])
        pkt.Dispose()
    }

    [<Fact>]
    let ``Пинг + данные + пинг + данные чередуются`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)

        use server = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        server.Start(cts.Token)

        use client = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [||]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _connEvt = server.ReadEventAsync()

        // Чередуем ping и данные
        clientCh.SendPing()

        let w1 = WPacket(16)
        w1.WriteCmd(1us)
        w1.WriteInt64(111L)
        clientCh.SendPacket(w1)

        clientCh.SendPing()

        let w2 = WPacket(16)
        w2.WriteCmd(2us)
        w2.WriteInt64(222L)
        clientCh.SendPacket(w2)

        clientCh.SendPing()

        // Собираем все события
        let mutable pingCount = 0
        let mutable cmdCount = 0
        let cmds = System.Collections.Generic.List<uint16 * uint32>()

        while pingCount + cmdCount < 5 do
            let! evt = server.ReadEventAsync()
            match evt with
            | SystemEvent.PingReceived _ -> pingCount <- pingCount + 1
            | SystemEvent.CommandReceived(_ch, pkt) ->
                cmds.Add(pkt.GetCmd(), uint32 (pkt.ReadInt64()))
                pkt.Dispose()
                cmdCount <- cmdCount + 1
            | _ -> ()

        Assert.Equal(3, pingCount)
        Assert.Equal(2, cmdCount)
        Assert.Contains((1us, 111u), cmds)
        Assert.Contains((2us, 222u), cmds)
    }

    [<Fact>]
    let ``Trailer паттерн: обратное чтение через TCP`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let received = TaskCompletionSource<IRPacket>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)
        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnCommand.Add(fun (_ch, pkt) ->
            let copy = BufferedRPacket.Create(pkt.GetPacketMemory()) :> IRPacket
            received.TrySetResult(copy) |> ignore)
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000

        // Пакет в стиле GateServer: [body][playerId][gpAddr][aimnum]
        let w = WPacket(64)
        w.WriteCmd(5000us) // PC range
        w.WriteString("chat message")
        w.WriteInt64(100L)   // playerId
        w.WriteInt64(200L)   // gpAddr
        w.WriteInt64(1L)    // aimnum
        clientCh.SendPacket(w)

        let! pkt = awaitTimeout received 5000

        // Обратное чтение trailer
        let aimnum = uint16 (pkt.ReverseReadInt64())
        Assert.Equal(1us, aimnum)
        let gpAddr = uint32 (pkt.ReverseReadInt64())
        Assert.Equal(200u, gpAddr)
        let playerId = uint32 (pkt.ReverseReadInt64())
        Assert.Equal(100u, playerId)

        // Отбрасываем trailer
        Assert.True(pkt.DiscardLast(3)) // 3 элемента: playerId + gpAddr + aimnum

        // Прямое чтение body
        Assert.Equal("chat message", pkt.ReadString())
        Assert.False(pkt.HasData)
        pkt.Dispose()
    }

    [<Fact>]
    let ``Двунаправленный обмен с разными типами`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)

        use server = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        server.Start(cts.Token)

        use client = new ChannelSystemCommand<TestChannel>(
            { Endpoints = [||]; ChannelCapacity = 100 },
            channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! connEvt = server.ReadEventAsync()
        let! _clientConn = client.ReadEventAsync() // Connected на стороне клиента
        let serverCh =
            match connEvt with
            | SystemEvent.Connected ch -> ch
            | _ -> failwith "Ожидался Connected"

        // Клиент → Сервер: UInt32 + String
        let w1 = WPacket(64)
        w1.WriteCmd(100us)
        w1.WriteInt64(42L)
        w1.WriteString("request")
        clientCh.SendPacket(w1)

        let! evt1 = server.ReadEventAsync()
        match evt1 with
        | SystemEvent.CommandReceived(_ch, pkt) ->
            Assert.Equal(100us, pkt.GetCmd())
            Assert.Equal(42u, uint32 (pkt.ReadInt64()))
            Assert.Equal("request", pkt.ReadString())
            pkt.Dispose()
        | _ -> failwith "Ожидался CommandReceived"

        // Сервер → Клиент: Int16 + Float32 + UInt64
        let w2 = WPacket(32)
        w2.WriteCmd(200us)
        w2.WriteInt64(-1L)
        w2.WriteFloat32(99.9f)
        w2.WriteUInt64(0xCAFEBABEuL)
        serverCh.SendPacket(w2)

        let! evt2 = client.ReadEventAsync()
        match evt2 with
        | SystemEvent.CommandReceived(_ch, pkt) ->
            Assert.Equal(200us, pkt.GetCmd())
            Assert.Equal(-1s, int16 (pkt.ReadInt64()))
            Assert.Equal(99.9f, pkt.ReadFloat32())
            Assert.Equal(0xCAFEBABEuL, pkt.ReadUInt64())
            pkt.Dispose()
        | _ -> failwith "Ожидался CommandReceived"
    }

    [<Fact>]
    let ``Пакеты с нулевым payload (только заголовок)`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let received = TaskCompletionSource<uint16>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)
        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(pkt.GetCmd()) |> ignore)
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000

        // Пакет без payload — только CMD
        let w = WPacket(0)
        w.WriteCmd(999us)
        clientCh.SendPacket(w)

        let! cmd = awaitTimeout received 5000
        Assert.Equal(999us, cmd)
    }

    [<Fact>]
    let ``Длинная строка (1KB)`` () = task {
        let port = freePort ()
        use cts = new CancellationTokenSource(10000)
        let serverConnected = TaskCompletionSource<TestChannel>()
        let received = TaskCompletionSource<string>()

        use server = new DirectSystemCommand<TestChannel>(
            { Endpoints = [| IPEndPoint(IPAddress.Loopback, port) |] },
            channelFactory, loggerFactory)
        server.OnConnected.Add(fun ch -> serverConnected.TrySetResult(ch) |> ignore)
        server.OnCommand.Add(fun (_ch, pkt) ->
            received.TrySetResult(pkt.ReadString()) |> ignore)
        server.Start(cts.Token)

        use client = new DirectSystemCommand<TestChannel>(
            { Endpoints = [||] }, channelFactory, loggerFactory)
        client.Start(cts.Token)

        let! clientCh = client.ConnectAsync(IPEndPoint(IPAddress.Loopback, port), cts.Token)
        let! _ = awaitTimeout serverConnected 5000

        let longStr = String.replicate 1024 "A"
        let w = WPacket(1100)
        w.WriteCmd(700us)
        w.WriteString(longStr)
        clientCh.SendPacket(w)

        let! value = awaitTimeout received 5000
        Assert.Equal(longStr, value)
    }
