namespace Corsairs.Platform.Protocol.Routing

open Corsairs.Platform.Network.Network
open Corsairs.Platform.Network.Protocol

/// Утилиты для построения и отправки пакетов.
[<RequireQualifiedAccess>]
module PacketBuilder =

    /// Переслать пакет через канал (создаёт копию).
    let forward (channel: ChannelIO) (packet: IRPacket) =
        channel.ForwardPacket(packet)
