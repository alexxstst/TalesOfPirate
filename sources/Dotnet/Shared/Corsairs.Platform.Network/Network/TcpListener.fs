namespace Corsairs.Platform.Network.Network

open System
open System.Collections.Concurrent
open System.Net
open System.Net.Sockets
open System.Threading
open System.Threading.Tasks
open Microsoft.Extensions.Logging

/// TCP-сервер для приёма входящих соединений.
/// Поддерживает несколько endpoint'ов с CancellationToken.
type TcpListener(logger: ILogger<TcpListener>) =
    let _listeners = ConcurrentBag<Socket>()
    let _onAccept = Event<Socket>()

    /// Событие: принят новый клиентский сокет.
    [<CLIEvent>]
    member _.OnAccept = _onAccept.Publish

    /// Запустить прослушивание на указанных endpoint'ах.
    member _.Start(endpoints: IPEndPoint[], ct: CancellationToken) =
        for ep in endpoints do
            Task.Run(Func<Task>(fun () -> (task {
                let listener = new Socket(ep.AddressFamily, SocketType.Stream, ProtocolType.Tcp)
                listener.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true)
                listener.Bind(ep)
                listener.Listen(512)
                _listeners.Add(listener)

                logger.LogInformation("TCP слушает на {Endpoint}", ep)

                try
                    while not ct.IsCancellationRequested do
                        let! clientSocket = listener.AcceptAsync(ct)
                        clientSocket.NoDelay <- true
                        _onAccept.Trigger(clientSocket)
                with
                | :? OperationCanceledException -> ()
                | :? SocketException as ex ->
                    logger.LogError("Ошибка listener на {Endpoint}: {Error}", ep, ex.Message)
                | :? ObjectDisposedException -> ()

                logger.LogInformation("TCP listener остановлен на {Endpoint}", ep)
            } :> Task)), ct) |> ignore

    /// Остановить все listener'ы.
    member _.Stop() =
        for listener in _listeners do
            try listener.Close() with _ -> ()

    interface IDisposable with
        member this.Dispose() = this.Stop()
