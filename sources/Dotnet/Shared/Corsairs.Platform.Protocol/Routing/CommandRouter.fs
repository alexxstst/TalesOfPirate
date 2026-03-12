namespace Corsairs.Platform.Protocol.Routing

open System.Collections.Generic
open System.Threading.Tasks
open Microsoft.Extensions.Logging
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Network

/// Роутер команд: uint16 cmd → ICommandHandler.
/// O(1) lookup по ID команды.
type CommandRouter(handlers: ICommandHandler seq, logger: ILogger<CommandRouter>) =
    let _map = Dictionary<uint16, ICommandHandler>()

    do
        for h in handlers do
            if _map.ContainsKey(h.CommandId) then
                logger.LogWarning("Дублирующийся handler для CMD {Cmd}, перезаписан", h.CommandId)
            _map[h.CommandId] <- h
        logger.LogInformation("CommandRouter: зарегистрировано {Count} handler'ов", _map.Count)

    /// Диспатчить пакет на соответствующий handler.
    member _.Dispatch(channel: ChannelIO, packet: IRPacket) : ValueTask =
        match _map.TryGetValue(packet.GetCmd()) with
        | true, handler ->
            handler.Handle(channel, packet)
        | _ ->
            logger.LogDebug("Неизвестная команда {Cmd} от {Channel}", packet.GetCmd(), channel)
            ValueTask.CompletedTask

    /// Количество зарегистрированных handler'ов.
    member _.HandlerCount = _map.Count

    /// Проверить, есть ли handler для данной команды.
    member _.HasHandler(cmdId: uint16) = _map.ContainsKey(cmdId)
