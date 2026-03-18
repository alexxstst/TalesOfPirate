[<AutoOpen>]
module Corsairs.GroupServer.Domain

open System

// ── ID-обёртки ──

[<Struct>]
type GpAddr = GpAddr_ of uint32

[<Struct>]
type GtAddr = GtAddr_ of uint32

// ── Данные слота персонажа (in-memory) ──

type CharacterSlot =
    { ChaId: int
      ChaName: string
      Job: string
      Level: int16
      TypeId: int16
      EquipIds: int64[]
      mutable Motto: string
      mutable Icon: int16
      mutable GuildId: int
      mutable GuildPermission: uint32
      mutable ChatColour: uint32
      mutable Estop: bool }

// ── Данные гильдии (in-memory) ──

/// Статус гильдии (битовая маска, аналог BitMaskStatus в C++).
[<Flags>]
type GuildStatus =
    | None = 0
    | ApplicantsPending = 1
    | MembersPending = 2
    | DisbandPending = 4

type GuildMemberInfo =
    { ChaId: int
      ChaName: string
      mutable Permission: uint32 }

type GuildData =
    { Id: int
      mutable Name: string
      mutable Motto: string
      mutable LeaderId: int
      mutable Status: GuildStatus
      mutable RemainMinute: int
      mutable Tick: int64
      mutable Members: Map<int, GuildMemberInfo>
      mutable ChallMoney: int
      mutable ChallPrizeMoney: int }

// ── Данные команды (in-memory) ──

type TeamMemberInfo =
    { ChaId: int
      ChaName: string
      Motto: string
      Icon: int16
      GpAddr: uint32 }

type TeamData =
    { Id: int
      mutable LeaderId: int
      mutable Members: Map<int, TeamMemberInfo> }

// ── Сессия чата (in-memory) ──

type ChatSessionData =
    { Id: uint32
      mutable Players: Map<uint32, uint32> } // gpAddr → gpAddr (просто набор)

// ── GM BBS ──

type GmBbsEntry =
    { mutable Interval: int
      mutable TimesLeft: int
      Word: string }

// ── Тип приглашения ──

[<Struct>]
type InvitationType =
    | FriendInvite
    | TeamInvite
    | MasterInvite

/// Приглашение с таймаутом.
type Invitation =
    { Type: InvitationType
      FromGpAddr: uint32
      FromChaId: int
      FromChaName: string
      ToGpAddr: uint32
      ToChaId: int
      ExpiresAt: DateTimeOffset
      /// Дополнительные данные (teamId для TeamInvite, etc.)
      ExtraData: int }

// ── Рейтинг (TBLParam) ──

[<Literal>]
let MaxOrderNum = 5

type OrderInfo =
    { mutable Nid: int
      mutable FightPoint: int
      mutable Name: string
      mutable Level: int
      mutable Job: string }

module OrderInfo =
    let empty () =
        { Nid = -1; FightPoint = 0; Name = ""; Level = -1; Job = "" }
