namespace Corsairs.Platform.Protocol.Routing

open System.Threading.Tasks
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Network

/// Обработчик одной сетевой команды.
/// Регистрируется в DI, CommandRouter диспатчит по CommandId.
[<AllowNullLiteral>]
type ICommandHandler =
    /// ID команды, которую обрабатывает этот handler (из Commands module).
    abstract CommandId: uint16
    /// Обработать команду из полученного пакета.
    abstract Handle: channel: ChannelIO * packet: IRPacket -> ValueTask
