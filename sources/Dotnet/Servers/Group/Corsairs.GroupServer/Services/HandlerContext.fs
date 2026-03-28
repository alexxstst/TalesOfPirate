namespace Corsairs.GroupServer.Services

open System.Collections.Concurrent
open Microsoft.Extensions.DependencyInjection
open Microsoft.Extensions.Logging
open Corsairs.GroupServer.Config
open Corsairs.GroupServer.Domain
open Corsairs.Platform.Network
open Corsairs.Platform.Network.Protocol

/// Сообщение в очереди гильд-банка.
type GuildBankMsg =
    { Player: PlayerRecord
      Packet: WPacket }

/// Контекст для обработчиков команд (передаётся вместо конкретного GateServerSystem).
type HandlerContext =
    { Registry: PlayerRegistry
      Invitations: InvitationManager
      Config: GroupServerConfig
      AccountSystem: IAccountServerSystem
      ScopeFactory: IServiceScopeFactory
      Logger: ILogger
      Gates: GateServerIO array
      Guilds: ConcurrentDictionary<int, GuildData>
      Teams: ConcurrentDictionary<int, TeamData>
      ChatSessions: ConcurrentDictionary<uint32, ChatSessionData>
      MasterRelations: ConcurrentDictionary<struct (int * int), bool>
      Ranking: OrderInfo array
      GuildBankQueues: ConcurrentDictionary<int, ConcurrentQueue<GuildBankMsg>>
      SendRpcResponse: GateServerIO -> uint32 -> WPacket -> unit
      SendToSingleClient: PlayerRecord -> WPacket -> unit
      SendToClients: PlayerRecord array -> WPacket -> unit
      SendToAllClients: WPacket -> unit
      KickUser: uint32 -> uint32 -> unit
      AllocateGpAddr: unit -> uint32
      AllocateTeamId: unit -> int
      AllocateSessionId: unit -> uint32 }
