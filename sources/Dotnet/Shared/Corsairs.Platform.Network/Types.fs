namespace Corsairs.Platform.Network.Network

open System.Threading

/// Результат парсинга пакета из Span<byte> буфера.
[<Struct>]
type ParseResult =
    /// Ошибка разбора — клиент невалидный.
    | ParseError of errorMsg: string
    /// Пинг (2 байта: 0x00, 0x02).
    | Ping
    /// Полный пакет извлечён. consumedBytes = сколько байт потреблено из буфера.
    | ParsedCommand of consumedBytes: int
    /// Недостаточно данных — нужно ждать ещё.
    | Incomplete

/// Статус завершения отправки.
[<Struct>]
type SendStatus =
    | SendOk
    | SendClosing

/// Атомарные счётчики per-socket статистики.
type ChannelStats() =
    let mutable _totalRecvBytes = 0UL
    let mutable _totalSendBytes = 0UL
    let mutable _totalCmdRecv = 0UL
    let mutable _totalCmdSend = 0UL

    member _.AddRecvBytes(n: uint64) = Interlocked.Add(&_totalRecvBytes, n) |> ignore
    member _.AddSendBytes(n: uint16) =
        Interlocked.Add(&_totalSendBytes, uint64 n) |> ignore
        Interlocked.Increment(&_totalCmdSend) |> ignore
    member _.AddRecvCmd() = Interlocked.Increment(&_totalCmdRecv) |> ignore

    member _.TotalRecvBytes = Interlocked.Read(&_totalRecvBytes)
    member _.TotalSendBytes = Interlocked.Read(&_totalSendBytes)
    member _.TotalCmdRecv = Interlocked.Read(&_totalCmdRecv)
    member _.TotalCmdSend = Interlocked.Read(&_totalCmdSend)
